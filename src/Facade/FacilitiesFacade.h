/*
 * FacilitiesFacade.h
 *
 *  Created on: Feb 22, 2013
 *      Author: 10088000201
 */

#ifndef FACILITIESFACADE_H_
#define FACILITIESFACADE_H_

class RegionDetection;
class TeRaster;
#include "../TerraLibPro/StructElements/StructElement.h"
#include <TePDIFilterMask.hpp>

class FacilitiesFacade {
private:
	FacilitiesFacade();
	static void convertQImage2Raster(QImage &imgQt, TeRaster &imgRaster);
	static void convertRaster2QImage(TeRaster &imgRaster, QImage &imgQt);

public:
	virtual ~FacilitiesFacade();
	// static member functions can't call normal function from inside
	static QImage *applyRegionDetection(QImage &imgQt, const double &fc, const double &fs);
	static QImage *applyErosion(QImage &imgQt, TePDIFilterMask::pointer erosionSe, int erosionSize);
	static QImage *applyDilation(QImage &imgQt, TePDIFilterMask::pointer dilationSe, int dilationSize);
	static TePDIFilterMask::pointer createStructElement(StructElement::seType erosionSe, int erosionSize, int v[] = 0);
	static QImage *applyInpaint(QImage &imgQtSource, QImage &imgQtOpenedOrClosed, int it);
};

#endif /* FACILITIESFACADE_H_ */
