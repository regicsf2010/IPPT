/*
 * RegionDetection.cpp
 *
 *  Created on: Feb 22, 2013
 *      Author: 10088000201
 */


#include "RegionDetection.h"
#include "../Formula/Formula.h"
#include <TeRaster.h>

RegionDetection::RegionDetection() {}

RegionDetection::~RegionDetection() {}

TeRaster *RegionDetection::sepRegion(TeRaster &img, double fc, double fs){
	TeRaster *imgOut = new TeRaster( img.params_.ncols_, img.params_.nlines_, 1, TeUNSIGNEDCHAR);
	if( !imgOut->init() ){ cout << imgOut->errorMessage() << "\n";}
	double mean = Formula::getInstance()->meanImage(img), sd = 0, val = 0, val1 = 0, val2 = 0, val3 = 0;

	if(img.nBands() != 1) { // Three band image
		sd = Formula::getInstance()->sdImage(img, Formula::getInstance()->meanImageBand(img, 0), Formula::getInstance()->meanImageBand(img, 1), Formula::getInstance()->meanImageBand(img, 2));
		const double FCLOUDMEANSD = fc * (mean + sd);
		const double FSHADOWMEANSD = fs * (mean - sd);
		for(int i = 0 ; i < img.params_.nlines_; i++ ){
			for(int j = 0; j < img.params_.ncols_; j++ ){
				img.getElement(j, i, val1, 0);
				img.getElement(j, i, val2, 1);
				img.getElement(j, i, val3, 2);
				if( (val1 > mean) && (val1 < FCLOUDMEANSD) && (val2 > mean) && (val2 < FCLOUDMEANSD) && (val3 > mean) && (val3 < FCLOUDMEANSD) ){
					imgOut->setElement(j, i, 2, 0); // SPARSE CLOUD = 2
				} else if ( (val1 >= FCLOUDMEANSD) && (val2 >= FCLOUDMEANSD) && (val3 >= FCLOUDMEANSD)){
					imgOut->setElement(j, i, 3, 0); // DENSE CLOUD = 3
				} else if ( (val1 <= FSHADOWMEANSD) && (val2 <= FSHADOWMEANSD) && (val3 <= FSHADOWMEANSD) ){
					imgOut->setElement(j, i, 0, 0); // SHADOW = 0
				} else {
					imgOut->setElement(j, i, 1, 0); // ANY OTHER AREA = 1
				}
			}
		}

	} else {// One band image
		sd = Formula::getInstance()->sdImage(img, mean);
		const double FCLOUDMEANSD = fc * (mean + sd);
		const double FSHADOWMEANSD = fs * (mean - sd);
		for(int i = 0; i < img.params_.nlines_; i++ ){
			for(int j = 0; j < img.params_.ncols_; j++ ){
				img.getElement(j, i, val, 0);
				if( (val > mean) && (val < FCLOUDMEANSD) ){
					imgOut->setElement(j, i, 2, 0); // SPARSE CLOUD = 2
				}else if( (val >= FCLOUDMEANSD) ){
					imgOut->setElement(j, i, 3, 0); // DENSE CLOUD = 3
				}else if( val <= FSHADOWMEANSD){
					imgOut->setElement(j, i, 0, 0); // SHADOW = 0
				}else{
					imgOut->setElement(j, i, 1, 0); // ANY OTHER AREA = 1
				}
			}
		}
	}
	return imgOut;
}
