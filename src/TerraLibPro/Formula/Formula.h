/*
 * Formula.h
 *
 *  Created on: Feb 19, 2013
 *      Author: 10088000201
 */

#ifndef FORMULA_H_
#define FORMULA_H_
#undef NAN
#define NAN 0x7fc00000UL
#define INVALID -1
class TeRaster;

class Formula {
private:
	Formula();
	static Formula *instance;
	double mean, sd, val;
	bool control, validation;
	int lines, cols; // necessário para agilizar consulta de posição válida
	struct Pos{
		int lin, col;
		double dist;
	};
	Pos auxPos[200];
public:
	virtual ~Formula();
	static Formula *getInstance();

	double meanImage(TeRaster &img);
	double meanImageBand(TeRaster &img, int band);
	double sdImage(TeRaster &img, double mean);
	double sdImage(TeRaster &img, double mean1, double mean2, double mean3);
	void copyRaster(TeRaster &imgIn, TeRaster &imgOut, int bandIn = INVALID, int bandOut = INVALID);
	TeRaster *rgb2grayByMean(TeRaster &img);
	TeRaster *rgb2grayByConstants(TeRaster &img);
	TeRaster *im2bw(TeRaster &img, double threshold);
	TeRaster *absRaster(TeRaster &img);

	//--------------------- INTERPOLATION -------------------------
	TeRaster *isFinite(TeRaster &imgNAN);
	TeRaster *bwDist(TeRaster &imgNAN, TeRaster &isFiniteImg);
	bool isOne(TeRaster &img, const int i, const int j, const int b);
	void sweepMatrix(TeRaster &isFiniteImg, const int &lin, const int &col, const int &n, const int &band, int &auxCont);
	int indLessDist(const int &n);
	void changeControl();
	int numPixelsToSweep(int n);
	bool isValid(const int i, const int j);
	//-------------------------------------------------------------
};

#endif /* FORMULA_H_ */
