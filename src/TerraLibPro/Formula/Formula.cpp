/*
 * Formula.cpp
 *
 *  Created on: Feb 19, 2013
 *      Author: 10088000201
 */


#include <math.h> // math first because of NAN definition that will be undefined in Formula
#include "Formula.h"
#include <TeRaster.h>

Formula *Formula::instance = 0;

Formula::Formula() {
	// Nothing happens; Constructor is private;
	// Singleton design pattern applied;
}

Formula::~Formula() {}

double Formula::meanImage(TeRaster &img) {
	mean = 0; double meanTemp = 0;
	for (int band = 0; band < img.nBands(); band++) {
		for(int i = 0; i < img.params_.nlines_; i++ ){
			for(int j = 0; j < img.params_.ncols_; j++ ){
				img.getElement(j, i, val, band);
				meanTemp += val;
			}
		}

		mean += meanTemp / ( img.params_.nlines_ * img.params_.ncols_ );
		meanTemp = 0;
	}
	return mean / img.nBands();
}

double Formula::meanImageBand(TeRaster &img, int band){
	mean = 0; double meanTemp = 0;
	for(int i = 0; i < img.params_.nlines_; i++){
		for(int j = 0; j < img.params_.ncols_; j++){
			img.getElement(j, i, val, band);
			meanTemp += val;
		}
	}
	return ( meanTemp / ( img.params_.nlines_ * img.params_.ncols_) );
}

double Formula::sdImage(TeRaster &img, double mean) {
	sd = 0; double sdTemp = 0;
	for(int band = 0; band < img.nBands(); band++ ){
		for(int i = 0; i < img.params_.nlines_; i++ ){
			for(int j = 0; j < img.params_.ncols_; j++ ){
				img.getElement(j, i, val, band);
				sdTemp += pow(( val - mean ), 2);
			}
		}
	}

	return sqrt(sdTemp / (img.params_.nlines_ * img.params_.ncols_ * img.nBands()) );
}

double Formula::sdImage(TeRaster &img, double mean1, double mean2, double mean3){
	sd = 0; double sd1Temp = 0, sd2Temp = 0, sd3Temp = 0, linXcol = img.params_.nlines_ * img.params_.ncols_;
	for(int i = 0; i < img.params_.nlines_; i++ ){
		for(int j = 0; j < img.params_.ncols_; j++ ){
			img.getElement(j, i, val, 0);
			sd1Temp += pow(( val - mean1 ), 2);

			img.getElement(j, i, val, 1);
			sd2Temp += pow(( val - mean2 ), 2);

			img.getElement(j, i, val, 2);
			sd3Temp += pow(( val - mean3 ), 2);
		}
	}
	return  ( (sqrt(sd1Temp / linXcol) + sqrt(sd2Temp / linXcol) + sqrt(sd3Temp / linXcol) ) / 3);
}

Formula* Formula::getInstance() {
	if(instance == 0)
		instance = new Formula;
	return instance;
}

TeRaster* Formula::rgb2grayByMean(TeRaster &img) {
	TeRaster *imgOut = new TeRaster(img.params_.ncols_, img.params_.nlines_, 1, TeUNSIGNEDCHAR);
	if(!imgOut->init()) {cout << imgOut->errorMessage() << "\n";}
	double R = 0, G = 0, B = 0;
	for (int i = 0; i < img.params_.nlines_; i++) {
		for (int j = 0; j < img.params_.ncols_; j++) {
			img.getElement(j, i, R, 0);
			img.getElement(j, i, G, 1);
			img.getElement(j, i, B, 2);
			imgOut->setElement(j, i, round((R + G + B) / 3), 0);
		}
	}
	return imgOut;
}

TeRaster* Formula::rgb2grayByConstants(TeRaster& img) {
	TeRaster *imgOut = new TeRaster(img.params_.ncols_, img.params_.nlines_, 1, TeUNSIGNEDCHAR);
	if(!imgOut->init()) {cout << imgOut->errorMessage() << "\n";}
	double R = 0, G = 0, B = 0;
	for (int i = 0; i < img.params_.nlines_; i++) {
		for (int j = 0; j < img.params_.ncols_; j++) {
			img.getElement(j, i, R, 0);
			img.getElement(j, i, G, 1);
			img.getElement(j, i, B, 2);
			imgOut->setElement(j, i, round((0.2989 * R + 0.5870 * G + 0.1140 * B)), 0);
		}
	}
	return imgOut;
}

TeRaster* Formula::im2bw(TeRaster& img, double threshold) {
	TeRaster *imgOut = new TeRaster(img.params_.ncols_, img.params_.nlines_, 1, TeUNSIGNEDCHAR);
	if(!imgOut->init()) {cout << imgOut->errorMessage() << "\n";}
	for (int i = 0; i < img.params_.nlines_; i++) {
		for (int j = 0; j < img.params_.ncols_; j++) {
			img.getElement(j, i, val, 0);
			if(val <= threshold)
				imgOut->setElement(j, i, 0, 0); //BLACK = 0
			else
				imgOut->setElement(j, i, 1, 0); //WHITE = 1
		}
	}
	return imgOut;
}

TeRaster* Formula::absRaster(TeRaster& img) {
	TeRaster *imgOut = new TeRaster(img.params_.ncols_, img.params_.nlines_, img.nBands(), TeUNSIGNEDCHAR);
	if(!imgOut->init()) {cout << imgOut->errorMessage() << "\n";}
	for(int b = 0; b < img.nBands(); b++){
		for (int i = 0; i < img.params_.nlines_; i++) {
			for (int j = 0; j < img.params_.ncols_; j++) {
				img.getElement(j, i, val, b);
				if(val < 0)
					imgOut->setElement(j, i, val * -1, b);
				else
					imgOut->setElement(j, i, val, b);
			}
		}
	}
	return imgOut;
}

TeRaster *Formula::isFinite(TeRaster &imgNAN){
	TeRaster *imgOut = new TeRaster(imgNAN.params_.ncols_, imgNAN.params_.nlines_, imgNAN.nBands(), TeDOUBLE);
	imgOut->init();
	for(int b = 0; b < imgNAN.nBands(); b++ ){
		for(int i = 0; i < imgNAN.params_.nlines_; i++){
			for(int j = 0; j < imgNAN.params_.ncols_; j++){
				imgNAN.getElement(j, i, val, b);
				if(val == NAN){
					imgOut->setElement(j, i, 0, b);
				} else{
					imgOut->setElement(j, i, 1, b);
				}
			}
		}
	}
	return imgOut;
}


TeRaster *Formula::bwDist(TeRaster &imgNAN, TeRaster &isFiniteImg){
	lines = isFiniteImg.params_.nlines_;
	cols = isFiniteImg.params_.ncols_;
	Pos pos[lines][cols];
	TeRaster *L = new TeRaster(cols, lines, isFiniteImg.nBands(), TeDOUBLE); L->init();
	TeRaster *Lband1 = new TeRaster(cols, lines, 1, TeDOUBLE); Lband1->init();
	TeRaster *Lband2 = new TeRaster(cols, lines, 1, TeDOUBLE); Lband2->init();
	TeRaster *Lband3 = new TeRaster(cols, lines, 1, TeDOUBLE); Lband3->init();
	int maxN = (lines > cols)? lines : cols;
	int n = 1, auxContPos = 0, volta = 0;

	for (int b = 0; b < isFiniteImg.nBands(); b++) {
		for (int i = 0; i < lines; i++) {
			for (int j = 0; j < cols; j++) {
				if(isOne(isFiniteImg, i, j, b)){
					pos[i][j].lin = i;
					pos[i][j].col = j;
					pos[i][j].dist = 0;
				} else {
					while (control == true && n <= maxN * 2) { // maxN * 2 para abranger matrizes grandes
						sweepMatrix(isFiniteImg, i, j, n, b, auxContPos);
						if(validation)
							volta++;

						if(validation && volta == 2){
							int ind = indLessDist(auxContPos);
							pos[i][j].lin = auxPos[ind].lin;
							pos[i][j].col = auxPos[ind].col;
							pos[i][j].dist = auxPos[ind].dist;
							control = false;
						}
						n++;
					}
					volta = 0;
					auxContPos = 0;
					validation = false;
					changeControl();
					n = 1;
				}
			}
		}
		if(b == 0){
			for (int i = 0; i < lines; i++) {
				for (int j = 0; j < cols; j++) {
					imgNAN.getElement(pos[i][j].col, pos[i][j].lin, val, b);
					Lband1->setElement(j, i, val, 0);
				}
			}
		}
		if (b == 1) {
			for (int i = 0; i < lines; i++) {
				for (int j = 0; j < cols; j++) {
					imgNAN.getElement(pos[i][j].col, pos[i][j].lin, val, b);
					Lband2->setElement(j, i, val, 0);
				}
			}
		}

		if (b == 2) {
			for (int i = 0; i < lines; i++) {
				for (int j = 0; j < cols; j++) {
					imgNAN.getElement(pos[i][j].col, pos[i][j].lin, val, b);
					Lband3->setElement(j, i, val, 0);
				}
			}
		}
	} // band 'for'
	double val1, val2, val3;
	if(isFiniteImg.nBands() == 1){
		for (int i = 0; i < L->params_.nlines_; i++) {
				for (int j = 0; j < L->params_.ncols_; j++) {
					Lband1->getElement(j, i, val1, 0);
					L->setElement(j, i, val1, 0);
				}
		}
	} else {

		for (int i = 0; i < L->params_.nlines_; i++) {
			for (int j = 0; j < L->params_.ncols_; j++) {
				Lband1->getElement(j, i, val1, 0); // é zero, pois esses raster só tem uma banda
				Lband2->getElement(j, i, val2, 0);
				Lband3->getElement(j, i, val3, 0);

				L->setElement(j, i, val1, 0);
				L->setElement(j, i, val2, 1);
				L->setElement(j, i, val3, 2);
			}
		}
	}
	return L;
}

bool Formula::isOne(TeRaster &img, const int i, const int j, const int b){
	img.getElement(j, i, val, b);
	if (val == 1)
		return true;
	return false;
}

void Formula::sweepMatrix(TeRaster &isFiniteImg, const int &lin, const int &col, const int &n, const int &band, int &auxCont){
	int k = numPixelsToSweep(n);
	//METADE CIMA PARA TRÁS
	int cont = 0;
	while (cont < round((double) (k / 2.))) {
		if (isValid(lin - n, col - cont)) {
			if (isOne(isFiniteImg, lin - n, col - cont, band)) {
				auxPos[auxCont].lin = lin - n;
				auxPos[auxCont].col = col - cont;
				if (col - cont != col) {
					auxPos[auxCont].dist = sqrt( pow( (double)(lin - (lin - n)), 2)	+ pow( (double)(col - (col - cont)), 2));
				} else {
					auxPos[auxCont].dist = n;
				}
				auxCont++;
				validation = true;
			}
		}
		cont++;
	}
	//TRÁS
	cont = 1;
	while (cont < k) {
		if (isValid((lin - n) + cont, col - n)) {
			if (isOne(isFiniteImg, (lin - n) + cont, col - n, band)) {
				auxPos[auxCont].lin = (lin - n) + cont;
				auxPos[auxCont].col = col - n;
				if ((lin - n) + cont != lin) {
					auxPos[auxCont].dist = sqrt(pow( (double)(lin - ((lin - n) + cont)), 2)	+ pow( (double)(col - (col - n)), 2));
				} else {
					auxPos[auxCont].dist = n;
				}
				auxCont++;
				validation = true;
			}
		}
		cont++;
	}
	//BAIXO
	cont = 1;
	while (cont < k) {
		if (isValid(lin + n, (col - n) + cont)) {
			if (isOne(isFiniteImg, lin + n, (col - n) + cont,  band)) {
				auxPos[auxCont].lin = lin + n;
				auxPos[auxCont].col = (col - n) + cont;
				if ((col - n) + cont != col) {
					auxPos[auxCont].dist = sqrt(pow( (double)(lin - (lin + n)), 2) + pow( (double)(col - ((col - n) + cont)), 2));
				} else {
					auxPos[auxCont].dist = n;
				}
				auxCont++;
				validation = true;
			}
		}
		cont++;
	}
	//FRENTE
	cont = 1;
	while (cont < k) {
		if (isValid((lin + n) - cont, col + n)) {
			if (isOne(isFiniteImg, (lin + n) - cont, col + n, band)) {
				auxPos[auxCont].lin = (lin + n) - cont;
				auxPos[auxCont].col = col + n;
				if ((lin + n) - cont != lin) {
					auxPos[auxCont].dist = sqrt(pow( (double)(lin - ((lin + n) - cont)), 2)	+ pow( (double)(col - (col + n)), 2));
				} else {
					auxPos[auxCont].dist = n;
				}
				auxCont++;
				validation = true;
			}
		}
		cont++;
	}
	//CIMA FRENTE ATÉ O MEIO CIMA
	cont = 1;
	while (cont < (k / 2)) {
		if (isValid(lin - n, (col + n) - cont)) {
			if (isOne(isFiniteImg, lin - n, (col + n) - cont, band)) {
				auxPos[auxCont].lin = lin - n;
				auxPos[auxCont].col = (col + n) - cont;
				if ((col + n) - cont != col) {
					auxPos[auxCont].dist = sqrt( pow( (double)(lin - (lin - n)), 2) + pow( (double)(col - ((col + n) - cont)), 2));
				} else {
					auxPos[auxCont].dist = n;
				}
				auxCont++;
				validation = true;
			}
		}
		cont++;
	}
}

int Formula::numPixelsToSweep(int n){
	return (2 * n + 1);
}

bool Formula::isValid(const int i, const int j){
	if (i >= 0 && i < lines) {
		if (j >= 0 && j < cols) {
			return true;
		}
	}
	return false;
}

int Formula::indLessDist(const int &n){
	double minValue = auxPos[0].dist;
	int ind = 0;
	for(int i = 1; i < n; i++){
		if(minValue > auxPos[i].dist){
			minValue = auxPos[i].dist;
			ind = i;
		}
	}
	return ind;
}

void Formula::changeControl(){
	control = (control == true)? false : true;
}

void Formula::copyRaster(TeRaster &imgIn, TeRaster &imgOut, int bandIn, int bandOut){
	double val;
	if(bandIn != INVALID && bandOut != INVALID){
		for (int i = 0; i < imgIn.params_.nlines_; i++) {
			for (int j = 0; j < imgIn.params_.ncols_; j++) {
				imgIn.getElement(j, i, val, bandIn);
				imgOut.setElement(j, i, val, bandOut);
			}
		}
	} else if(imgIn.nBands() == 1 && imgOut.nBands() == 1){ // copy the only band
		for (int i = 0; i < imgIn.params_.nlines_; i++) {
			for (int j = 0; j < imgIn.params_.ncols_; j++) {
				imgIn.getElement(j, i, val, 0);
				imgOut.setElement(j, i, val, 0);
			}
		}
	} else { // copy the three bands
		double valR, valG, valB;
		for (int i = 0; i < imgIn.params_.nlines_; i++) {
			for (int j = 0; j < imgIn.params_.ncols_; j++) {
				imgIn.getElement(j, i, valR, 0);
				imgIn.getElement(j, i, valG, 1);
				imgIn.getElement(j, i, valB, 2);
				imgOut.setElement(j, i, valR, valG, valB);
			}
		}
	}
}
