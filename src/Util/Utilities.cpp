/*
 * Utilities.cpp
 *
 *  Created on: Mar 20, 2013
 *      Author: 10088000201
 */

#include "Utilities.h"
#include <QImage>

Utilities::Utilities() {
	// Private
	// do nothing
}

Utilities::~Utilities() {}

void Utilities::copyImage(QImage &image1, QImage &image2){
	QRgb rgb;
	for (int i = 0; i < image1.height(); ++i) {
		for (int j = 0; j < image1.width(); ++j) {
			rgb = image1.pixel(j, i);
			image2.setPixel(j, i, qRgb(qRed(rgb), qGreen(rgb), qBlue(rgb)));
		}
	}
}


QImage *Utilities::prepareRegionDetection(QImage &imageSource, QImage &imageSep){
	QRgb rgb;
	QImage *imageOut = new QImage(imageSource.width(), imageSource.height(), imageSource.format());
	copyImage(imageSource, *imageOut); // Copy original image
	for (int i = 0; i < imageSource.height(); ++i) {
		for (int j = 0; j < imageSource.width(); ++j) {
			rgb = imageSep.pixel(j, i); // only one band
			if(qRed(rgb) == 2){ // SPARSE ---> Blue
				imageOut->setPixel(j, i, qRgb(0, 0, 255));
			} else if(qRed(rgb) == 3){ // DENSE --- > Red
				imageOut->setPixel(j, i, qRgb(255, 0, 0));
			} else if(qRed(rgb) == 0){ // SHADOW ---> Green
				imageOut->setPixel(j, i, qRgb(0, 255, 0));
			}
		}
	}

	return imageOut;
}
