/*
 * StructElement.cpp
 *
 *  Created on: Mar 14, 2013
 *      Author: 10088000201
 */

#include "StructElement.h"
#include <TeRaster.h>
#include <math.h>
#include <stdlib.h>
#include <string>
using std::string;
#include <TeDecoder.h>
#include <TeAgnostic.h>
#include <TePDIUtils.hpp>
#include <TePDITypes.hpp>
#include <TePDIFilterMask.hpp>
#include <TePDIMorfFilter.hpp>
#include <TePDIParameters.hpp>
#include <TeInitRasterDecoders.h>
using namespace std;


StructElement::StructElement() {}

StructElement::~StructElement() {}

vector< TeRaster* > StructElement::createMiniMasks(int r, int n) {
	int v[n][2], tmpv[2];
	double theta, k;
	vector< TeRaster* > masks(n);
	switch(n) {
		case 4:
			v[0][0]=1; v[0][1]=0;
			v[1][0]=1; v[1][1]=1;
			v[2][0]=0; v[2][1]=1;
			v[3][0]=-1; v[3][1]=1;
			break;
		case 6:
			v[0][0]=1; v[0][1]=0;
			v[1][0]=1; v[1][1]=2;
			v[2][0]=2; v[2][1]=1;
			v[3][0]=0; v[3][1]=1;
			v[4][0]=-1; v[4][1]=2;
			v[5][0]=-2; v[5][1]=1;

			break;
		case 8:
			v[0][0]=1; v[0][1]=0;
			v[1][0]=2; v[1][1]=1;
			v[2][0]=1; v[2][1]=1;
			v[3][0]=1; v[3][1]=2;
			v[4][0]=0; v[4][1]=1;
			v[5][0]=-1; v[5][1]=2;
			v[6][0]=-1; v[6][1]=1;
			v[7][0]=-2; v[7][1]=1;
			break;
	}
	theta = PI / (2 * n);
	k =  2 * r / (cos(theta) / sin(theta) + 1 / sin(theta));
	int p;
	for (int i = 0 ; i < n; i++) {
		p = floor( k / (sqrt(pow((double)v[i][0],2) + pow((double)v[i][1],2)  )));
		tmpv[0] = v[i][0];
		tmpv[1] = v[i][1];
		masks[i] = makePeriodicLine( p, tmpv );
	}
	return masks;
}

StructElement::Map StructElement::mapOnes(TeRaster &img, int center[]) {
	double val;
	Map map;
	map.size = 0;
	for (int i = 0; i < img.params_.nlines_;i++)
		for(int j = 0; j < img.params_.ncols_; j++) {
			img.getElement( j, i, val );
			if(val == 1)
				map.size++;
		}
	map.coord = (Coords*)malloc(map.size*sizeof(Coords));
	map.size = 0;
	for (int i = 0; i <img.params_.nlines_; i++)
		for(int j = 0; j <img.params_.ncols_; j++) {
			img.getElement( j, i, val );
			if(val == 1) {
				map.coord[map.size].x = (center[0] - i) * -1;
				map.coord[map.size].y = (center[1] - j) * -1;
				map.size++;
			}
		}
	return map;
}

StructElement::Map StructElement::mapOnes(TeRaster &img) {
	double val;
	Map map;
	map.size = 0;
	for (int i = 0; i < img.params_.nlines_; i++)
		for(int j = 0; j < img.params_.ncols_; j++) {
			img.getElement( j, i, val );
			if(val == 1)
				map.size++;
		}

	map.coord = (Coords*)malloc(map.size*sizeof(Coords));
	map.size = 0;
	for (int i = 0; i < img.params_.nlines_; i++)
		for(int j = 0; j<img.params_.ncols_; j++) {
			img.getElement( j, i, val );
			if(val == 1) {
				map.coord[map.size].x = i;
				map.coord[map.size].y = j;
				map.size++;
			}
		}
	return map;
}

StructElement::Map StructElement::sum(Map mapA, Map mapB) {
	int i, j;
	Map mapOut;
	mapOut.size = mapA.size * mapB.size;
	mapOut.coord = (Coords*) malloc(mapOut.size * sizeof(Coords));
	for (j = 0; j < mapA.size; j++)
		for (i = 0; i < mapB.size; i++) {
			mapOut.coord[i + j * mapB.size].x = mapA.coord[j].x	+ mapB.coord[i].x;
			mapOut.coord[i + j * mapB.size].y = mapA.coord[j].y + mapB.coord[i].y;
		}
	return mapOut;
}

TeRaster* StructElement::createRaster(int nrows, int ncols, Map map) {
	TeRaster *outRaster = new TeRaster(ncols, nrows, 1, TeUNSIGNEDCHAR);
	outRaster->init();
	for (int i = 0; i < nrows; i++)
		for (int j = 0; j < ncols; j++)
			outRaster->setElement(j, i, 0);
	for (int i = 0; i < map.size; i++)
		outRaster->setElement(map.coord[i].y, map.coord[i].x, 1);
	return outRaster;
}

TeRaster* StructElement::fullBinaryDilation(TeRaster &inRaster, TeRaster &se) {
	int center[] = { floor((se.params_.nlines_ + 1) / 2) - 1, floor((se.params_.ncols_ + 1) / 2) - 1 };
	TeRaster *pad = padding(inRaster, se, center);
	TeRaster *outRaster = createRaster(pad->params_.nlines_, pad->params_.ncols_, sum(mapOnes(*pad), mapOnes(se, center)));
	//pad->clear();
	delete pad;
	return outRaster;
}

TeRaster* StructElement::dilateMiniMasks(vector<TeRaster*> minimasks) {
	TeRaster *init = new TeRaster(1, 1, 1, TeUNSIGNEDCHAR);
	init->init();
	init->setElement(0, 0, 1);
	TeRaster *bigMask = fullBinaryDilation(*init, *minimasks[0]);
	delete init;
	for (unsigned int i = 1; i < minimasks.size(); i++)
		bigMask = fullBinaryDilation(*bigMask, *minimasks[i]);
	return bigMask;
}

TeRaster* StructElement::compensation(TeRaster &mask, int r) {
	int i, j, len;
	Map map = mapOnes(mask);
	TeRaster *outMask;
	for (i = 0; i < map.size; i++)
		map.coord[i].y = map.coord[i].y	- floor((mask.params_.nlines_ + 1) / 2.0) + 1;

	len = map.coord[0].y;

	for (i = 0; i < map.size; i++) {
		if (map.coord[i].y > len)
			len = map.coord[i].y;
	}

	len = 2 * (r - len - 1) + 1;

	if (len >= 3) {
		outMask = new TeRaster(len, len, 1, TeUNSIGNEDCHAR);
		outMask->init();
		for (i = 0; i < len; i++)
			for (j = 0; j < len; j++)
				outMask->setElement(j, i, 1);
	} else {
		outMask = new TeRaster(1, 1, 1, TeUNSIGNEDCHAR);
		outMask->init();
		outMask->setElement(0, 0, 1);
	}
	return outMask;
}

TeRaster* StructElement::makeCompleteDisk(int r) {
	int n, dim = 2 * r + 1;
	TeRaster *outRaster = new TeRaster(dim, dim, 1, TeUNSIGNEDCHAR);
	outRaster->init();
	for (int i = 0; i < dim; i++)
		for (int j = 0; j < dim; j++)
			outRaster->setElement(j, i, 1);

	for (int j = 0; j < r; j++) {
		outRaster->setElement(j, 0, 0);
		outRaster->setElement(j, dim - 1, 0);
		outRaster->setElement(j + r + 1, 0, 0);
		outRaster->setElement(j + r + 1, dim - 1, 0);
	}

	for (int i = 1; i < r; i++) {
		n = (int) sqrt(pow((double) r, 2) - pow((double) i - r, 2));
		for (int j = 0; j < r - n; j++) {
			outRaster->setElement(j, i, 0);
			outRaster->setElement(j + r + n + 1, i, 0);
			outRaster->setElement(j, dim - i - 1, 0);
			outRaster->setElement(j + r + n + 1, dim - i - 1, 0);
		}
	}
	return outRaster;
}

TeRaster* StructElement::padding(TeRaster &inRaster, TeRaster& se,	int center[]) {
	double val;
	int pad[] = { center[0], se.params_.nlines_ - 1 - center[0], center[1], se.params_.ncols_ - 1 - center[1] };
	TeRaster *outRaster = new TeRaster(	inRaster.params_.ncols_ + pad[2] + pad[3], inRaster.params_.nlines_ + pad[0] + pad[1], 1, TeUNSIGNEDCHAR);
	outRaster->init();
	for (int i = 0; i < outRaster->params_.nlines_; i++)
		for (int j = 0; j < outRaster->params_.ncols_; j++)
			outRaster->setElement(j, i, 0);
	for (int i = pad[0]; i < outRaster->params_.nlines_ - pad[1]; i++)
		for (int j = pad[2]; j < outRaster->params_.ncols_ - pad[3]; j++) {
			inRaster.getElement(j - pad[2], i - pad[0], val);
			outRaster->setElement(j, i, val);
		}
	inRaster.clear();
	return outRaster;
}

TeRaster* StructElement::makePeriodicLine(int p, int v[]) {
	int r, c, m;

	m = v[0] * v[1];
	v[0] = v[0] < 0 ? v[0] * -1 : v[0];
	v[1] = v[1] < 0 ? v[1] * -1 : v[1];

	r = v[0] == 0 ? 1 : v[0] * p * 2 + 1;
	c = v[1] == 0 ? 1 : v[1] * p * 2 + 1;

	TeRaster *mask = new TeRaster(c, r, 1, TeUNSIGNEDCHAR);
	mask->init();

	for (int i = 0; i < r; i++)
		for (int j = 0; j < c; j++)
			mask->setElement(j, i, 0);

	if (m > 0) {
		for (int i = 0; i <= 2 * p; i++)
			mask->setElement(i * v[1], i * v[0], 1);
	} else {
		for (int i = 0; i <= 2 * p; i++)
			mask->setElement(c - 1 - i * v[1], i * v[0], 1);
	}
	return mask;
}

TePDIFilterMask::pointer StructElement::makeDisk(int r, int n) {
	TeRaster *Mask;
	if (n == 0 || r <= 3)
		Mask = makeCompleteDisk(r);

	else {
		vector<TeRaster*> masks = createMiniMasks(r, n);
		TeRaster *tmpMask = dilateMiniMasks(masks);
		Mask = fullBinaryDilation(*tmpMask, *(compensation(*tmpMask, r)));
		delete tmpMask;
		while(!masks.empty()) delete masks.back(), masks.pop_back();
	}

	TePDIFilterMask::pointer outMask = convertion( *Mask );
	delete Mask;
	return outMask;
}

TePDIFilterMask::pointer StructElement::makeSquare(int dim) {
	TeRaster *outRaster = new TeRaster(dim, dim, 1, TeUNSIGNEDCHAR);
	outRaster->init();
	for (int i = 0; i < dim; i++)
		for (int j = 0; j < dim; j++)
			outRaster->setElement(j, i, 1);

	TePDIFilterMask::pointer result = convertion(*outRaster);
	delete outRaster;
	return result;
}

TePDIFilterMask::pointer StructElement::convertion(TeRaster &img) {
	int a, b, c;
	double val;

	if (img.params_.nlines_ < img.params_.ncols_) {
		c = (img.params_.ncols_ / 2.0 == img.params_.ncols_ / 2) ? 1 : 0;

		TePDIFilterMask::pointer outmask( new TePDIFilterMask(img.params_.ncols_ + c, 0));
		a = floor((img.params_.ncols_ - img.params_.nlines_) / 2.0) + c;
		b = ceil((img.params_.ncols_ - img.params_.nlines_) / 2.0) + c;

		for (int i = 0; i < img.params_.ncols_ + c; i++)
			for (int j = 0; j < img.params_.ncols_ + c; j++)
				outmask->set(i, j, 0);

		for (int i = a; i < img.params_.ncols_ - b + 2 * c; i++)
			for (int j = 0; j < img.params_.ncols_; j++) {
				img.getElement(j, i - a, val);
				outmask->set(i, j, val);
			}
		return outmask;
	}

	else if (img.params_.nlines_ > img.params_.ncols_) {
		c = (img.params_.nlines_ / 2.0 == img.params_.nlines_ / 2) ? 1 : 0;

		TePDIFilterMask::pointer outmask(new TePDIFilterMask(img.params_.nlines_ + c, 0));
		a = floor((img.params_.nlines_ - img.params_.ncols_) / 2.0) + c;
		b = ceil((img.params_.nlines_ - img.params_.ncols_) / 2.0) + c;

		for (int i = 0; i < img.params_.nlines_; i++)
			for (int j = 0; j < img.params_.nlines_; j++)
				outmask->set(i, j, 0);

		for (int i = 0; i < img.params_.nlines_; i++)
			for (int j = a; j < img.params_.nlines_ - b + 2 * c; j++) {
				img.getElement(j - a, i, val);
				outmask->set(i, j, val);
			}
		return outmask;

	} else if (img.params_.nlines_ == 1 && img.params_.ncols_ == 1) {
		TePDIFilterMask::pointer outmask(new TePDIFilterMask(3, 0));
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				outmask->set(i, j, 0);
		outmask->set(1, 1, 1);
		return outmask;

	} else {
		c = (img.params_.nlines_ / 2.0 == img.params_.nlines_ / 2) ? 1 : 0;
		TePDIFilterMask::pointer outmask(new TePDIFilterMask(img.params_.nlines_ + c, 0));

		for (int i = 0; i < img.params_.nlines_; i++)
			for (int j = 0; j < img.params_.nlines_; j++) {
				img.getElement(j, i, val);
				outmask->set(i, j, val);
			}
		if (c == 1)
			for (int i = img.params_.nlines_; i < img.params_.nlines_ + 1; i++)
				for (int j = img.params_.nlines_; j < img.params_.nlines_ + 1; j++)
					outmask->set(i, j, 0);
		return outmask;
	}
}

TePDIFilterMask::pointer StructElement::makePDIPeriodicLine(int p, int v[]) {
	int r, c, m;
	m = v[0] * v[1];
	v[0] = v[0] < 0 ? v[0] * -1 : v[0];
	v[1] = v[1] < 0 ? v[1] * -1 : v[1];

	r = v[0] == 0 ? 1 : v[0] * p * 2 + 1;
	c = v[1] == 0 ? 1 : v[1] * p * 2 + 1;

	TeRaster *mask = new TeRaster(c, r, 1, TeUNSIGNEDCHAR);
	mask->init();

	for (int i = 0; i < r; i++)
		for (int j = 0; j < c; j++)
			mask->setElement(j, i, 0);

	if (m > 0) {
		for (int i = 0; i <= 2 * p; i++)
			mask->setElement(i * v[1], i * v[0], 1);
	} else {
		for (int i = 0; i <= 2 * p; i++)
			mask->setElement(c - 1 - i * v[1], i * v[0], 1);
	}
	return convertion( *mask );
}


TePDIFilterMask::pointer StructElement::setStructElement(seType type, int r, int n) {
	TePDIFilterMask::pointer SE;
	switch (type) {
	case SQUARE:
		SE = makeSquare(r);
		break;

	case DISK:
		SE = makeDisk(r, n);
		break;
	}
	return SE;
}

TePDIFilterMask::pointer StructElement::setStructElement(seType type, int r, int v[]) {
	TePDIFilterMask::pointer SE;
	switch (type) {
	case PERIODICLINE:
		SE = makePDIPeriodicLine(r, v);
		break;
	}
	return SE;
}


