/*
 * RegionDetection.h
 *
 *  Created on: Feb 22, 2013
 *      Author: 10088000201
 */

#ifndef REGIONDETECTION_H_
#define REGIONDETECTION_H_

class TeRaster;
class RegionDetection {
public:
	RegionDetection();
	virtual ~RegionDetection();
	/*
	 * <p>Apply cloud, shadow, any other region detection</p>
	 * @param img Input Image
	 * @param fc Factor cloud
	 * @param fs Factor shadow
	 */
	TeRaster *sepRegion(TeRaster &img, double fc = 1, double fs = 1);


};

#endif /* REGIONDETECTION_H_ */
