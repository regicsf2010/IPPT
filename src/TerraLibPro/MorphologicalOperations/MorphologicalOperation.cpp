/*
 * MorphologicalOperation.cpp
 *
 *  Created on: Mar 13, 2013
 *      Author: 10088000201
 */

#include "MorphologicalOperation.h"
#include <TePDIMorfFilter.hpp>
#include <TePDIFilterMask.hpp>
#include <TePDIParameters.hpp>
#include <TeAgnostic.h>
#include <TePDIUtils.hpp>
#include <TeProgress.h>
#include <TeStdIOProgress.h>
#include <TePDITypes.hpp>
#include <TeSharedPtr.h>
using namespace TePDITypes;
#include <iostream> // trying to resolve mutex problem

MorphologicalOperation::MorphologicalOperation() {}

MorphologicalOperation::~MorphologicalOperation() {}

TeRaster *MorphologicalOperation::erosion( TeRaster &img, TePDIFilterMask::pointer se, int seSize ){
	TePDIParameters params;
	double padding = floor(seSize / 2);
	TeRaster *imgAux = new TeRaster( img.params_.ncols_ + seSize - 1, img.params_.nlines_ + seSize - 1, img.nBands(), TeUNSIGNEDCHAR );
	imgAux->init();
	double val = 0;
	for(int i = 0; i < img.params_.nlines_; i++){
		for(int j = 0; j < img.params_.ncols_; j++){
			img.getElement(j, i, val, 0);
			imgAux->setElement(j + padding, i + padding, val, 0);
		}
	}
	TePDITypes::TePDIRasterPtrType inRaster( imgAux );
	params.SetParameter( "input_image", inRaster );
	TePDITypes::TePDIRasterPtrType outRaster;
	TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outRaster, 1, 1, 1, false, TeDOUBLE, 0 ), "RAM Raster Alloc error" );
	params.SetParameter( "output_image", outRaster );
	std::vector<int> channels;
	channels.push_back( 0 );
	params.SetParameter( "channels", channels );
	std::cout << std::flush << std::endl; // trying millisecond stop here, solve mutex problem
	params.SetParameter( "filter_mask", se );
	std::cout << std::flush << std::endl; // trying millisecond stop here, solve mutex problem
	params.SetParameter( "filter_type", TePDIMorfFilter::TePDIMErosionType );
	std::cout << std::flush << std::endl; // trying millisecond stop here, solve mutex problem
	params.SetParameter( "iterations", (int)1 );
	TePDIMorfFilter filter;
	TEAGN_TRUE_OR_THROW( filter.Reset( params ), "Invalid Parameters" );
	TEAGN_TRUE_OR_THROW( filter.Apply(),"Apply error" );
	outRaster->updateParams( inRaster->params() );
	TeRaster *imgOut = new TeRaster(img.params_.ncols_, img.params_.nlines_, img.nBands(), TeUNSIGNEDCHAR);
	imgOut->init();
	for(int i = 0; i < img.params_.nlines_; i++){
		for(int j = 0; j < img.params_.ncols_; j++){
			outRaster->getElement(j + padding, i + padding, val, 0);
			imgOut->setElement(j, i, val, 0);
		}
	}
//  Testar deletando inRaster
//	delete inRaster;
	return imgOut;
}

TeRaster *MorphologicalOperation::dilation( TeRaster &img, TePDIFilterMask::pointer se, int seSize ){
	TePDIParameters params;
	double padding = floor((seSize * 2 - 1) / 2);
	TeRaster *imgAux = new TeRaster(img.params_.ncols_ + padding * 2, img.params_.nlines_ + padding * 2, img.nBands(), TeUNSIGNEDCHAR);
	imgAux->init();
	for(int i = 0; i < imgAux->params_.nlines_; i++)
		for(int j = 0; j < imgAux->params_.ncols_; j++)
			imgAux->setElement(j, i, 0, 0);

	double val;
	for (int i = 0; i < img.params_.nlines_; i++) {
		for (int j = 0; j < img.params_.ncols_; j++) {
			img.getElement(j, i, val, 0);
			imgAux->setElement(j + padding, i + padding, val, 0);
		}
	}
	TePDITypes::TePDIRasterPtrType inRaster( imgAux );
	params.SetParameter("input_image", inRaster);
	TePDITypes::TePDIRasterPtrType outRaster;
	TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outRaster, 1, 1, 1, false, TeDOUBLE, 0 ), "RAM Raster Alloc error");
	params.SetParameter("output_image", outRaster);
	std::vector<int> channels;
	channels.push_back(0);
	params.SetParameter("channels", channels);
	std::cout << std::flush << std::endl; // trying millisecond stop here, solve mutex problem
	params.SetParameter("filter_mask", se);
	std::cout << std::flush << std::endl; // trying millisecond stop here, solve mutex problem
	params.SetParameter("filter_type", TePDIMorfFilter::TePDIMDilationType);
	std::cout << std::flush << std::endl; // trying millisecond stop here, solve mutex problem
	params.SetParameter("iterations", (int) 1);
	TePDIMorfFilter filter;
	TEAGN_TRUE_OR_THROW( filter.Reset( params ), "Invalid Parameters");
	TEAGN_TRUE_OR_THROW( filter.Apply(), "Apply error");
	outRaster->updateParams( inRaster->params() );
	TeRaster *imgOut = new TeRaster(img.params_.ncols_, img.params_.nlines_, img.nBands(), TeUNSIGNEDCHAR);
	imgOut->init();
	for (int i = 0; i < img.params_.nlines_; i++) {
		for (int j = 0; j < img.params_.ncols_; j++) {
			outRaster->getElement(j + padding, i + padding, val, 0);
			imgOut->setElement(j, i, val, 0);
		}
	}
//  Testar deletando inRaster
//	delete inRaster;
	return imgOut;
}
