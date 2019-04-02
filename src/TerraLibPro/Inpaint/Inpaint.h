/*
 * Inpaint.h
 *
 *  Created on: Mar 27, 2013
 *      Author: 10088000201
 */

#ifndef INPAINT_H_
#define INPAINT_H_

class TeRaster;

class Inpaint {
private:
	TeRaster *initialGuess(TeRaster &imgNAN, TeRaster &isFiniteImg);
	void reshape(int *in, double *out, int new_lines, int new_cols);
	void dct2(double *input, double *output, int lines, int cols, int tmp);

public:
	Inpaint();
	virtual ~Inpaint();

	TeRaster *inpaintn(TeRaster &img, int band = -1, int it = 10);

};

#endif /* INPAINT_H_ */
