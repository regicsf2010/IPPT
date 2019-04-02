/*
 * FacilitiesFacade.cpp
 *
 *  Created on: Feb 22, 2013
 *      Author: 10088000201
 */
#include <QImage>
#include <QDebug>
#include "FacilitiesFacade.h"
#include "../TerraLibPro/RegionDetection/RegionDetection.h"
#include "../TerraLibPro/MorphologicalOperations/MorphologicalOperation.h"
#include "../TerraLibPro/StructElements/StructElement.h"
#include "../TerraLibPro/Formula/Formula.h"
#include "../TerraLibPro/Inpaint/Inpaint.h"
#include <TeRaster.h>
#include <TePDIFilterMask.hpp>

FacilitiesFacade::FacilitiesFacade() {
	// Nothing happens; Constructor is private;
	// Facade design pattern applied;
	// Using static member functions;
}

FacilitiesFacade::~FacilitiesFacade() {}

void FacilitiesFacade::convertQImage2Raster(QImage &imgQt, TeRaster &imgRaster){
	QRgb rgb;
	if(imgRaster.nBands() == 3){
		for (int i = 0; i < imgRaster.params_.nlines_; i++) {
			for (int j = 0; j < imgRaster.params_.ncols_; j++) {
				rgb = imgQt.pixel(j, i);
				imgRaster.setElement(j, i, qRed(rgb), qGreen(rgb), qBlue(rgb));
			}
		}
	} else { //bandValue == 1
		for (int i = 0; i < imgRaster.params_.nlines_; i++) {
			for (int j = 0; j < imgRaster.params_.ncols_; j++) {
				rgb = imgQt.pixel(j, i);
				// caso a imagem tenha uma banda
				// tanto faz a banda que se obtem o pixel
				// o QImage guarda o mesmo valor nos métodos qRed(), qGreen() e qBlue()
				// então, arbitrariamente escolhi qRed().
				imgRaster.setElement(j, i, qRed(rgb));
			}
		}
	}
}

void FacilitiesFacade::convertRaster2QImage(TeRaster &imgRaster, QImage &imgQt){
	double valR = 0, valG = 0, valB = 0;
	if(imgRaster.nBands() == 3){
		for (int i = 0; i < imgRaster.params_.nlines_; i++) {
			for (int j = 0; j < imgRaster.params_.ncols_; j++) {
				imgRaster.getElement(j, i, valR, 0);
				imgRaster.getElement(j, i, valG, 1);
				imgRaster.getElement(j, i, valB, 2);
				imgQt.setPixel(j, i, qRgb(valR, valG, valB));
			}
		}
	} else { //bandValue == 1
		// preenche os valores da tabela de cores de 0-255 (valores possíveis)
		for (int i = 0; i < 255; i++)
			imgQt.setColor(i, qRgb(i, i, i));

			// se os valores da tabela já estão preenchidos,
		    // os pixels raster se tornarão índices da tabela de cores
		for (int i = 0; i < imgRaster.params_.nlines_; i++) {
			for (int j = 0; j < imgRaster.params_.ncols_; j++) {
				imgRaster.getElement(j, i, valR, 0); // só tem uma banda
				imgQt.setPixel(j, i, valR);
			}
		}
	}
}

QImage *FacilitiesFacade::applyRegionDetection(QImage &imgQt, const double &fc, const double &fs){
	RegionDetection *rd = new RegionDetection;
	const int bandValue = (imgQt.isGrayscale())? 1:3;
	TeRaster imgRaster(imgQt.width(), imgQt.height(), bandValue, TeDOUBLE);
	imgRaster.init();
	convertQImage2Raster(imgQt, imgRaster);
	TeRaster *imgRasterResult = rd->sepRegion(imgRaster, fc, fs);
	delete rd;
	QImage *imgResult = new QImage(imgRasterResult->params_.ncols_, imgRasterResult->params_.nlines_, QImage::Format_Indexed8);
	convertRaster2QImage(*imgRasterResult, *imgResult);
	delete imgRasterResult;
	return imgResult;
}

QImage *FacilitiesFacade::applyErosion(QImage &imgQt, TePDIFilterMask::pointer erosionSe, int erosionSize){
	MorphologicalOperation *morphOperations = new MorphologicalOperation;
	const int bandValue = (imgQt.isGrayscale())? 1:3;
	TeRaster imgRaster(imgQt.width(), imgQt.height(), bandValue, TeDOUBLE);
	imgRaster.init();
	convertQImage2Raster(imgQt, imgRaster);
	TeRaster *imgRasterResult = morphOperations->erosion(imgRaster, erosionSe, erosionSize);
	delete morphOperations;
	QImage *imgResult = new QImage(imgRasterResult->params_.ncols_, imgRasterResult->params_.nlines_, QImage::Format_Indexed8);
	convertRaster2QImage(*imgRasterResult, *imgResult);
	delete imgRasterResult;
	return imgResult;
}

QImage *FacilitiesFacade::applyDilation(QImage &imgQt, TePDIFilterMask::pointer dilationSe, int dilationSize){
	MorphologicalOperation *morphOperations = new MorphologicalOperation;
	const int bandValue = (imgQt.isGrayscale())? 1:3;
	TeRaster imgRaster(imgQt.width(), imgQt.height(), bandValue, TeDOUBLE);
	imgRaster.init();
	convertQImage2Raster(imgQt, imgRaster);
	TeRaster *imgRasterResult = morphOperations->dilation(imgRaster, dilationSe, dilationSize);
	delete morphOperations;
	QImage *imgResult = new QImage(imgRasterResult->params_.ncols_, imgRasterResult->params_.nlines_, QImage::Format_Indexed8);
	convertRaster2QImage(*imgRasterResult, *imgResult);
	delete imgRasterResult;
	return imgResult;
}

TePDIFilterMask::pointer FacilitiesFacade::createStructElement(StructElement::seType erosionSe, int erosionSize, int v[]){
	TePDIFilterMask::pointer erosionPointer;
	StructElement *se = new StructElement;
	switch (erosionSe) {
	case StructElement::SQUARE:
	case StructElement::DISK:
		erosionPointer = se->setStructElement(erosionSe, erosionSize);
		break;
	case StructElement::PERIODICLINE:
		erosionPointer = se->setStructElement(erosionSe, erosionSize, v);
		break;
	}
	delete se;
	return erosionPointer;
}

QImage *FacilitiesFacade::applyInpaint(QImage &imgQtSource, QImage &imgQtOpenedOrClosed, int it){
	Inpaint *inpaint = new Inpaint;
	const int bandValue = (imgQtSource.isGrayscale())? 1:3;

	// copy qt image source
	TeRaster imgRasterInpainted(imgQtSource.width(), imgQtSource.height(), bandValue, TeDOUBLE);
	imgRasterInpainted.init();
	convertQImage2Raster(imgQtSource, imgRasterInpainted);

	// copy qt opened or closed image
	// Surely has one band, therefore band = 1
	TeRaster imgRasterOpenedOrClosed(imgQtOpenedOrClosed.width(), imgQtOpenedOrClosed.height(), 1, TeDOUBLE);
	imgRasterOpenedOrClosed.init();
	convertQImage2Raster(imgQtOpenedOrClosed, imgRasterOpenedOrClosed);

	double val;
	for (int i = 0; i < imgRasterOpenedOrClosed.params_.nlines_; i++) {
		for (int j = 0; j < imgRasterOpenedOrClosed.params_.ncols_; j++) {
			imgRasterOpenedOrClosed.getElement(j, i, val, 0);
			if (val == 2 || val == 3) {
				imgRasterInpainted.setElement(j, i, NAN, NAN, NAN);
			}
		}
	}

	TeRaster result(imgRasterInpainted.params_.ncols_, imgRasterInpainted.params_.nlines_, imgRasterInpainted.nBands(), TeDOUBLE);
	result.init();

	TeRaster *resultInpaintband = inpaint->inpaintn(imgRasterInpainted, 0, it);
	Formula::getInstance()->copyRaster(*resultInpaintband, result, 0, 0);
	delete resultInpaintband;
	resultInpaintband = inpaint->inpaintn(imgRasterInpainted, 1, it);
	Formula::getInstance()->copyRaster(*resultInpaintband, result, 0, 1);
	delete resultInpaintband;
	resultInpaintband = inpaint->inpaintn(imgRasterInpainted, 2, it);
	Formula::getInstance()->copyRaster(*resultInpaintband, result, 0, 2);
	delete resultInpaintband;
	delete inpaint;
	QImage *imgResult = new QImage(result.params_.ncols_, result.params_.nlines_, QImage::Format_RGB32);
	convertRaster2QImage(result, *imgResult);
	return imgResult;
}
