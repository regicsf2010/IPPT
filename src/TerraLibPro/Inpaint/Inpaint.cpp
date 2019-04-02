/*

 * Inpaint.cpp
 *
 *  Created on: Mar 27, 2013
 *      Author: 10088000201

*/
#include "Inpaint.h"
#include "fftw3.h"
#include "TeRaster.h"
#include "TeInitRasterDecoders.h"
#include "../Formula/Formula.h"
#include <math.h>

#include <QDebug>

#define PII 3.1416

Inpaint::Inpaint() {}

Inpaint::~Inpaint() {}

TeRaster* Inpaint::initialGuess(TeRaster &imgNAN, TeRaster &isFiniteImg) {
	TeRaster *L = new TeRaster(imgNAN.params_.ncols_, imgNAN.params_.nlines_, imgNAN.nBands(), TeDOUBLE);
	L->init();
	L = Formula::getInstance()->bwDist(imgNAN, isFiniteImg);
	return L;
}

TeRaster* Inpaint::inpaintn(TeRaster &img, int band, int it) {
	TeInitRasterDecoders();

	// Cópia de uma banda da imagem de entrada para a execução do inpaint
	TeRaster *imageOneBand = new TeRaster(img.params_.ncols_, img.params_.nlines_, 1, TeDOUBLE);
	imageOneBand->init();

	Formula::getInstance()->copyRaster(img, *imageOneBand, band, 0);

	const double PI = atan(1.0) * 4;
	double value = 0.0, value2 = 0.0;
	const int dims = 2; //numero de dimensoes

	TeRasterParams params = imageOneBand->params();
	const int cols = params.ncols_;
	const int lines = params.nlines_;
	const int bands = imageOneBand->nBands();

	//sizx pega as dimensões da imagem de entrada
	int sizx[3] = { lines, cols, bands };

	//Inicialização de Lambda e Gamma
	double* Lambda = new double[lines * cols];
	double* Gamma = new double[lines * cols];

	for (int i = 0; i < lines; i++) {
		for (int j = 0; j < cols; j++) {
			Lambda[j + i * cols] = 0;
			Gamma[j + i * cols] = 0;
		}
	}

	//Inicialização de siz0, necessário p/ cálculo do Lambda
	int* siz0 = new int[dims];

	//Em todas as linhas e colunas de uma imagem em escala de cinza...
	for (int i = 0; i < dims; i++) {

		for (int j = 0; j < dims; j++) {
			siz0[j] = 1;
		}
		siz0[i] = sizx[i];

		//vetor temporário para auxiliar no cálculo do Lambda (1:sizx(i))
		int* temp = new int[sizx[i]];
		for (int j = 0; j < sizx[i]; j++) {
			temp[j] = j + 1;
		}

		//inicialização da matriz resultante do reshape
		double out[siz0[0] * siz0[1]];

		//mesma idéia do reshape do matlab
		reshape(temp, out, siz0[0], siz0[1]);

		//Define o vetor que somará ao Lambda na bsxfun
		//(cos(pi*(reshape(1:sizx(i),siz0)-1)/sizx(i))))
		for (int ii = 0; ii < siz0[0]; ii++) {
			for (int j = 0; j < siz0[1]; j++) {
				out[j + ii * siz0[1]] = cos((out[j + ii * siz0[1]] - 1) * PI / sizx[i]);
			}
		}

		//bsxfun(@plus,Lambda,...) para os casos da soma do vetor linhas e
		//do vetor coluna, somando-os na matriz
		if (siz0[1] == 1) {
			for (int ii = 0; ii < lines; ii++) {
				for (int j = 0; j < cols; j++) {
					Lambda[j + ii * cols] = Lambda[j + ii * cols] + out[ii];
				}
			}
		} else if (siz0[0] == 1) {

			for (int ii = 0; ii < lines; ii++) {
				for (int j = 0; j < cols; j++) {
					Lambda[j + ii * cols] = Lambda[j + ii * cols] + out[j];
				}
			}
		}

		delete[] temp;
	}

	delete[] siz0;

	int* W = new int[lines * cols];

	//Lambda = -2*(d-Lambda) e Lambda = Lambda.^2;
	for (int i = 0; i < lines; i++) {
		for (int j = 0; j < cols; j++) {
			Lambda[j + i * cols] = pow((-2 * (dims - Lambda[j + i * cols])), 2);
			imageOneBand->getElement(j, i, value, 0);

			if (value == NAN)
				W[j + (i * cols)] = 0;
			else
				W[j + (i * cols)] = 1;
		}
	}


	//Chama Initial guess e obtém a imagem da interpolação do vizinho
	//mais proximo (chute inicial).
	TeRaster* imageInterp = new TeRaster(cols, lines, 1, TeDOUBLE);
	imageInterp->init();
	TeRaster *isFiniteImg = Formula::getInstance()->isFinite(*imageOneBand);
	imageInterp = initialGuess(*imageOneBand, *isFiniteImg);
	delete isFiniteImg;

	//s = logspace(s0,s2,n);
	double s0 = 3, s2 = -3;
	if (s2 == PI) {
		s2 = log10(s2);
	}
	//double s[n-1];
	double s[it];
	for (int i = 0; i <= it - 1; i++) {
		s[i] = i;
		s[i] = pow(10, (s0 + s[i] * (s2 - s0) / (floor(it) - 1)));
		if (i == it - 1)
			s[i] = pow(10, s2);

	}

	double RF = 2;

	//Inicialização de váriáveis pra execução do inpainting

	//guardar valores no dominio da DCT
	double* imDCT = new double[lines * cols];
	//guardar valores fora do dominio da DCT
	double* tempDCT = new double[lines * cols];

	//Para todas as iterações, fazer:
	//Gamma = 1./(1+s(i)*Lambda);
	//y = RF*idctn(Gamma.*dctn(W.*(x-y)+y)) + (1-RF)*y;
	for (int ii = 0; ii < it; ii++) {
		for (int i = 0; i < lines; i++) {
			for (int j = 0; j < cols; j++) {
				Gamma[j + i * cols] = 1 / (1 + (s[ii] * Lambda[j + i * cols]));
				imageOneBand->getElement(j, i, value, 0);
				if (value == NAN) {
					value = 0;
					imageOneBand->setElement(j, i, value, 0);
					imageOneBand->getElement(j, i, value, 0);
				}

				imageInterp->getElement(j, i, value2);

				tempDCT[j + i * cols] = (W[j + i * cols] * (value - value2)) + value2;

			}

		}

		//DCT (param 1 indica a dct)
		dct2(tempDCT, imDCT, lines, cols, 1);

		for (int i = 0; i < lines; i++) {
			for (int j = 0; j < cols; j++) {
				imDCT[j + i * cols] = Gamma[j + i * cols] * imDCT[j + i * cols];
			}
		}

		//IDCT (param -1 indica a idct)
		dct2(imDCT, tempDCT, lines, cols, -1);

		for (int i = 0; i < lines; i++) {
			for (int j = 0; j < cols; j++) {
				imageInterp->getElement(j, i, value, 0);
				value2 = RF * tempDCT[j + (i * cols)] + (1 - RF) * value;
				imageInterp->setElement(j, i, value2, 0);
			}
		}

		/*if (ii == n - 1)
			fclose(pFile);*/

	} //Fim de todas as iterações

	delete[] Lambda;
	delete[] Gamma;
	delete[] imDCT;
	delete[] tempDCT;

	//y(W) = x(W);

	for (int i = 0; i < lines; i++) {
		for (int j = 0; j < cols; j++) {
			if (W[j + i * cols] == 1) {
				imageOneBand->getElement(j, i, value, 0);
				imageInterp->setElement(j, i, value, 0);

			}
		}
	}


	delete[] W;
	return imageInterp;
}

void Inpaint::reshape(int *in, double *out, int new_lines, int new_cols){
	int k = 0;
	for (int i = 0; i < new_lines; i++) {
		for (int j = 0; j < new_cols; j++) {
			out[j + i * new_cols] = in[k];
			k++;
		}
	}
}

void Inpaint::dct2(double *input, double *output, int lines, int cols, int tmp){
	fftw_r2r_kind kind[2] = { FFTW_REDFT10, FFTW_REDFT10 };
	if (tmp < 0) {
		kind[0] = FFTW_REDFT01;
		kind[1] = FFTW_REDFT01;
	}
	/* allocates memory for FFTW, allowing fast memory access */
	int m = lines;
	int n = cols;
	double *in, *out;
	in = (double*) fftw_malloc(sizeof(double) * m * n);
	out = (double*) fftw_malloc(sizeof(double) * m * n);
	fftw_plan p;
	int dimentions[2] = { m, n }; /* dimentions */
	/* copies input data for fast memory access by FFTW */
	int i, j;
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			in[(i * n) + j] = input[(i * n) + j];
			if (tmp < 0) {
				if ((i * n) + j == 0)
					in[(i * n) + j] = in[(i * n) + j] * (4 * sqrt(m) * sqrt(n));
				else if (((i * n) + j % n) == 0 || ((i * n) + j) < m)
					in[(i * n) + j] = in[(i * n) + j] * (2 * sqrt(2) * sqrt(m) * sqrt(n));
				else
					in[(i * n) + j] = in[(i * n) + j] * (2 * sqrt(m) * sqrt(n));
			}
		}
	}
	/* executes DCT */
	p = fftw_plan_r2r(2, dimentions, in, out, kind, FFTW_MEASURE);
	fftw_execute(p);
	if (tmp >= 0) {
		for (i = 0; i < m; i++) {
			for (j = 0; j < n; j++) {
				if ((i * n) + j == 0)
					out[(i * n) + j] = out[(i * n) + j]	/ (4 * sqrt(m) * sqrt(n));
				//troquei m por n
				else if (((i * n) + j % n) == 0 || ((i * n) + j) < m)
					out[(i * n) + j] = out[(i * n) + j]	/ (2 * sqrt(2) * sqrt(m) * sqrt(n));
				else
					out[(i * n) + j] = out[(i * n) + j]	/ (2 * sqrt(m) * sqrt(n));
			}
		}
	}
	/* copies result to output raster */
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			if (tmp < 0) {
				if (i == 0)
					out[(i * n) + j] = out[(i * n) + j] / (2 * 2 * m * n);
				else if (((i * n) + j % n) == 0 || (i * n) + j < m)
					out[(i * n) + j] = out[(i * n) + j] / (2 * 2 * m * n);
				else
					out[(i * n) + j] = out[(i * n) + j] / (2 * 2 * m * n);
			}
			output[(i * n) + j] = out[(i * n) + j];
		}
	}

	/* free resources */
	fftw_destroy_plan(p);
	fftw_free(in);
	fftw_free(out);
}

