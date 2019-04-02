/*
 * StructElement.h
 *
 *  Created on: Mar 14, 2013
 *      Author: 10088000201
 */

#ifndef STRUCTELEMENT_H_
#define STRUCTELEMENT_H_

#define PI 3.1416
#include <vector>
using std::vector;
class TeRaster;
#include <TePDIFilterMask.hpp>

class StructElement {
private:
	struct Coords {
			int x, y;
		};

	struct Map {
		Coords *coord;
		int size;
	};
	vector< TeRaster* > createMiniMasks( int r, int n );
	Map mapOnes( TeRaster &img, int center[] );
	Map mapOnes( TeRaster &img );
	Map sum ( Map mapA, Map mapB );
	TeRaster *createRaster ( int nrows, int ncols, Map map );
	TeRaster *fullBinaryDilation ( TeRaster &inRaster, TeRaster &se );
	TeRaster *dilateMiniMasks ( vector< TeRaster* > minimasks );
	TeRaster *compensation ( TeRaster &mask, int r );
	TeRaster *makeCompleteDisk ( int r );
	TeRaster *padding (TeRaster &inRaster, TeRaster &se, int center[] );
	TeRaster *makePeriodicLine ( int p, int v[] );
	TePDIFilterMask::pointer makeDisk ( int r, int n = 4 );
	TePDIFilterMask::pointer makeSquare ( int dim );
	TePDIFilterMask::pointer convertion ( TeRaster &img );
	TePDIFilterMask::pointer makePDIPeriodicLine ( int p, int v[] );

public:
	StructElement();
	virtual ~StructElement();

	enum seType{ SQUARE, DISK, PERIODICLINE };

	TePDIFilterMask::pointer setStructElement ( seType type, int r, int n = 4 );
	TePDIFilterMask::pointer setStructElement ( seType type, int r, int v[] );
};

#endif /* STRUCTELEMENT_H_ */
