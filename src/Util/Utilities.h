/*
 * Utilities.h
 *
 *  Created on: Mar 20, 2013
 *      Author: 10088000201
 */

#ifndef UTILITIES_H_
#define UTILITIES_H_

class QImage;

class Utilities {
private:
	Utilities();
	static void copyImage(QImage &image1, QImage &image2);

public:
	virtual ~Utilities();
	static QImage *prepareRegionDetection(QImage &imageSource, QImage &imageSep);
};

#endif /* UTILITIES_H_ */
