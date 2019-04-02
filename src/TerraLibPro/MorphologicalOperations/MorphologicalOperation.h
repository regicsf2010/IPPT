/*
 * MorphologicalOperation.h
 *
 *  Created on: Mar 13, 2013
 *      Author: 10088000201
 */

#ifndef MORPHOLOGICALOPERATION_H_
#define MORPHOLOGICALOPERATION_H_

class TeRaster;
#include <TePDIFilterMask.hpp>

class MorphologicalOperation {
public:
	MorphologicalOperation();
	virtual ~MorphologicalOperation();

	TeRaster *erosion( TeRaster &img, TePDIFilterMask::pointer se, int seSize );
	TeRaster *dilation( TeRaster &img, TePDIFilterMask::pointer se, int seSize );
};

#endif /* MORPHOLOGICALOPERATION_H_ */
