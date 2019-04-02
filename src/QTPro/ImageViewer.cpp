/*
 * ImageViewer.cpp
 *
 *  Created on: Nov 23, 2012
 *      Author: Reginaldo Filho
 */
#include <QtGui>

#include "ImageViewer.h"
#include "dialogs/SepRegionsDialog.h"
#include "dialogs/MorphologicalOperationDialog.h"
#include "dialogs/InpaintDialog.h"
#include "dialogs/TextureSynthesisDialog.h"
#include "dialogs/DecomposeDialog.h"
#include "dialogs/MedianFilterDialog.h"
//-------------------------------------------------
#include "../Facade/FacilitiesFacade.h"
#include "../Util/Utilities.h"
#include <TePDIFilterMask.hpp>


ImageViewer::ImageViewer() {
	softwareName = new QString(tr("Imagement"));
	//setFixedSize(WIDTH, HEIGHT);
	resize(WIDTH, HEIGHT);
	setWindowTitle(*softwareName);
	setWindowIcon(QIcon(":/icons/mainWindow.png"));
	//statusBar()->setVisible(true);

	configureImagePointer();
	configureImageLabel();
	configureImageScrollArea();

	setCentralWidget(scrollArea);

	createActions();
	createToolBar();
	createDock();
	createMenus();
	readState();
}
ImageViewer::~ImageViewer(){
	delete fileName;
	delete image1; delete image2; delete image3;
}
//------------------------------------CONFIGURATIONS AND NORMAL FUNCTIONS
void ImageViewer::configureImagePointer(){
	image1 = 0;
	image2 = 0;
	image3 = 0;
}
void ImageViewer::configureImageLabel(){
	imageLabel = new QLabel;
	//imageLabel->setBackgroundRole(QPalette::Base);
	// habilita o usuário a dimensionar a imagem quando a opção fitToWindow está habilitada
	imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	// faz com que o conteúdo do label seja redimensionado de forma a cobrir totalmente o espaço do label,
	// caso contrário, apenas o imageLabel se redimensionará, deixando o conteúdo do mesmo tamanho
	imageLabel->setScaledContents(true);
	imageLabel->setAlignment(Qt::AlignCenter);

}
void ImageViewer::configureImageScrollArea(){
	scrollArea = new QScrollArea;
	scrollArea->setBackgroundRole(QPalette::Dark);
	scrollArea->setWidget(imageLabel);
	scrollArea->setAlignment(Qt::AlignCenter);
}
void ImageViewer::adjustScrollBar(QScrollBar* scrollBar, double factor) {
	// esta conta configura o scroll bar para que o mesmo mantenha a proporção quando
	// a imagem é expandida ou reduzida
	scrollBar->setValue(int(factor * scrollBar->value() + ((factor - 1) * scrollBar->pageStep()/2)));
}
void ImageViewer::scaleImage(double factor) {
    Q_ASSERT(imageLabel->pixmap());
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    zoomInAct->setEnabled(scaleFactor < 3.0);
    zoomOutAct->setEnabled(scaleFactor > 0.333);
}
void ImageViewer::updateActions() {
    zoomInAct->setEnabled(!fitToWindowAct->isChecked());
    zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
    normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}
void ImageViewer::deleteStage(){
	imageLabel->setPixmap(0);
	delete image1; delete image2; delete image3;
	configureImagePointer();
	zoomInAct->setEnabled(false);
	zoomOutAct->setEnabled(false);
	normalSizeAct->setEnabled(false);
	fitToWindowAct->setEnabled(false);
	saveAct->setEnabled(false);
	newAct->setEnabled(false);
	statusBar()->removeWidget(imageSizeStatusBar);
}
void ImageViewer::setPermanentStatusBarMessage(){
	statusBar()->removeWidget(imageSizeStatusBar);
	imageSizeStatusBar = new QLabel(QString::number(image1->width()) + " x " + QString::number(image1->height()) + " pixels", this);
	statusBar()->addPermanentWidget(imageSizeStatusBar);
}
void ImageViewer::readState(){
	QSettings settings("OpenSourceDPI", *softwareName);
	QSize sizeTemp = settings.value("MainWindow/Size", sizeHint()).toSize();
	if(sizeTemp.width() >= WIDTH && sizeTemp.height() >= HEIGHT)
		resize(sizeTemp);
	restoreState(settings.value("MainWindow/Properties").toByteArray());


	toolBarVisibilityAct->setChecked(!fileToolBar->isHidden() && !zoomControlToolBar->isHidden());
	dockVisibilityAct->setChecked(docker->isVisible());
	statusBarVisibilityAct->setChecked(statusBar()->isEnabled());
}
//------------------------------------CREATING ELEMENTS
void ImageViewer::createActions() {
	openAct = new QAction(QIcon(":/icons/open.png"), tr("&Open..."), this);
	openAct->setShortcut(tr("Ctrl+O"));
	openAct->setStatusTip(tr("Open an image"));
	connect(openAct, SIGNAL(triggered()), this, SLOT(openFile()));

	newAct = new QAction(QIcon(":/icons/new.png"), tr("&New..."), this);
	newAct->setShortcut(tr("Ctrl+N"));
	newAct->setStatusTip(tr("New project"));
	newAct->setEnabled(false);
	connect(newAct, SIGNAL(triggered()), this, SLOT(newStage()));

	saveAct = new QAction(QIcon(":/icons/save.png"), tr("&Save"), this);
	saveAct->setShortcut(tr("Ctrl+S"));
	saveAct->setEnabled(false);
	saveAct->setStatusTip(tr("Save an image"));
	connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

	exitAct = new QAction(tr("E&xit"), this);
	exitAct->setShortcut(tr("Ctrl+Q"));
	exitAct->setStatusTip(tr("Quit application"));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(closeApp()));

	toolBarVisibilityAct = new QAction(tr("&Toolbar"), this);
	toolBarVisibilityAct->setObjectName("&Toolbar");
	toolBarVisibilityAct->setShortcut(tr("Ctrl+T"));
	toolBarVisibilityAct->setCheckable(true);
	toolBarVisibilityAct->setChecked(true);
	toolBarVisibilityAct->setStatusTip(tr("Show/hide tool bar"));
	connect(toolBarVisibilityAct, SIGNAL(triggered()), this, SLOT(toolBarVisible()));

	dockVisibilityAct = new QAction(tr("Doc&k"), this);
	dockVisibilityAct->setObjectName("Doc&k");
	dockVisibilityAct->setShortcut(tr("Ctrl+K"));
	dockVisibilityAct->setCheckable(true);
	dockVisibilityAct->setChecked(true);
	dockVisibilityAct->setStatusTip(tr("Show/hide dock"));
	connect(dockVisibilityAct, SIGNAL(triggered()), this, SLOT(dockVisible()));

	statusBarVisibilityAct = new QAction(tr("S&tatusbar"), this);
	statusBarVisibilityAct->setObjectName("S&tatusbar");
	statusBarVisibilityAct->setShortcut(tr("Ctrl+Y"));
	statusBarVisibilityAct->setCheckable(true);
	statusBarVisibilityAct->setChecked(true);
	statusBarVisibilityAct->setStatusTip(tr("Show/hide status bar"));
	connect(statusBarVisibilityAct, SIGNAL(triggered()), this, SLOT(statusBarVisible()));

	zoomInAct = new QAction(QIcon(":/icons/zoomin.png"), tr("Zoom &In (25%)"), this);
	zoomInAct->setShortcut(tr("Ctrl++"));
	zoomInAct->setEnabled(false);
	zoomInAct->setStatusTip(tr("Zoom in"));
	connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

	zoomOutAct = new QAction(QIcon(":/icons/zoomout.png"), tr("Zoom &Out (25%)"), this);
	zoomOutAct->setShortcut(tr("Ctrl+-"));
	zoomOutAct->setEnabled(false);
	zoomOutAct->setStatusTip(tr("Zoom out"));
	connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

	normalSizeAct = new QAction(QIcon(":/icons/normalsize.png"), tr("Nor&mal Size"), this);
	normalSizeAct->setShortcut(tr("Ctrl+M"));
	normalSizeAct->setEnabled(false);
	normalSizeAct->setStatusTip(tr("Normal size"));
	connect(normalSizeAct, SIGNAL(triggered()), this, SLOT(normalSize()));

	fitToWindowAct = new QAction(QIcon(":/icons/fitwindow.png"), tr("&Fit to Window"), this);
	fitToWindowAct->setEnabled(false);
	fitToWindowAct->setCheckable(true);
	fitToWindowAct->setShortcut(tr("Ctrl+F"));
	fitToWindowAct->setStatusTip(tr("Fit to window"));
	connect(fitToWindowAct, SIGNAL(triggered()), this, SLOT(fitToWindow()));

	aboutAct = new QAction(tr("&About"), this);
	aboutAct->setShortcut(tr("Ctrl+A"));
	aboutAct->setStatusTip(tr("About application"));
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

	regDetectionAct = new QAction(tr("Region detection"), this);
	regDetectionAct->setStatusTip(tr("Detect clouds and shadows"));
	connect(regDetectionAct, SIGNAL(triggered()), this, SLOT(regionDetection()));

	morphOperationAct = new QAction(tr("Morphological operation"), this);
	morphOperationAct->setStatusTip(tr("Morphologically open or close an image"));
	connect(morphOperationAct, SIGNAL(triggered()), this, SLOT(morphologicalOperation()));

	inpaintMethodAct = new QAction(tr("Inpaint method"), this);
	inpaintMethodAct->setStatusTip(tr("Apply Inpaint in an image"));
	connect(inpaintMethodAct, SIGNAL(triggered()), this, SLOT(inpaintMethod()));

	textureSynthesisAct = new QAction(tr("Texture synthesis method"), this);
	textureSynthesisAct->setStatusTip(tr("Apply texture synthesis in an image"));
	connect(textureSynthesisAct, SIGNAL(triggered()), this, SLOT(textureSynthesisMethod()));

	decomposeAct = new QAction(tr("Decompose method"), this);
	decomposeAct->setStatusTip(tr("Apply decompose in an image"));
	connect(decomposeAct, SIGNAL(triggered()), this, SLOT(decomposeMethod()));

	medianFilterAct = new QAction(tr("Median filter method"), this);
	medianFilterAct->setStatusTip(tr("Apply median filter in an image"));
	connect(medianFilterAct, SIGNAL(triggered()), this, SLOT(medianFilterMethod()));
}
void ImageViewer::createMenus() {
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addAction(newAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = new QMenu(tr("&Edit"), this);

    viewMenu = new QMenu(tr("&View"), this);
    viewMenu->addAction(toolBarVisibilityAct);
    viewMenu->addAction(dockVisibilityAct);
    viewMenu->addAction(statusBarVisibilityAct);

    zoomSubMenu = new QMenu(tr("&Zoom"), viewMenu);
    zoomSubMenu->addAction(zoomInAct);
    zoomSubMenu->addAction(zoomOutAct);
    zoomSubMenu->addAction(normalSizeAct);
    zoomSubMenu->addSeparator();
    zoomSubMenu->addAction(fitToWindowAct);

    dpiMenu = new QMenu(tr("DPI"), this);
    dpiMenu->addAction(regDetectionAct);
    dpiMenu->addAction(morphOperationAct);
    dpiMenu->addAction(inpaintMethodAct);
    dpiMenu->addAction(textureSynthesisAct);
    dpiMenu->addAction(decomposeAct);

    filtersSubMenu = new QMenu(tr("Filters"), dpiMenu);
    filtersSubMenu->addAction(medianFilterAct);

    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);

    viewMenu->addAction(zoomSubMenu->menuAction());
    dpiMenu->addAction(filtersSubMenu->menuAction());
    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(editMenu);
    menuBar()->addMenu(viewMenu);
    menuBar()->addMenu(dpiMenu);
    menuBar()->addMenu(helpMenu);
}
void ImageViewer::createToolBar(){
	fileToolBar = addToolBar(tr("File"));
	fileToolBar->setObjectName("File");
	fileToolBar->addAction(newAct);
	fileToolBar->addAction(openAct);
	fileToolBar->addAction(saveAct);

	zoomControlToolBar = addToolBar(tr("Control zoom"));
	zoomControlToolBar->setObjectName("Control zoom");
	zoomControlToolBar->addAction(zoomInAct);
	zoomControlToolBar->addAction(zoomOutAct);
	zoomControlToolBar->addAction(normalSizeAct);
	zoomControlToolBar->addAction(fitToWindowAct);
}
void ImageViewer::createDock(){
	docker = new QDockWidget(tr("Informations"), this);
	docker->setObjectName("Informations");
	docker->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

	docker->setMinimumWidth(150);
	connect(docker, SIGNAL(visibilityChanged(bool)), this, SLOT(dockVisibleFromDocker(bool)));
	addDockWidget(Qt::LeftDockWidgetArea, docker);
}
//------------------------------------SLOTS
void ImageViewer::openFile() {
	QString *aux = new QString(QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath()));
	if(!aux->isEmpty()) fileName = aux;
	if (!fileName->isEmpty()) {
		image1 = new QImage(*fileName);
		image2 = 0;
		image3 = 0;
		if (image1->isNull()) {
			QMessageBox::information(this, tr("Image Viewer"), tr("Cannot load %1.").arg(*fileName));
			return;
		}

		imageLabel->setPixmap(QPixmap::fromImage(*image1));
		setPermanentStatusBarMessage();
		scaleFactor = 1.0;
		fitToWindowAct->setEnabled(true);
		saveAct->setEnabled(true);
		newAct->setEnabled(true);
		updateActions();

		if (!fitToWindowAct->isChecked())
			imageLabel->adjustSize();

		statusBar()->showMessage(tr("Image successfully loaded"));
	}
}
void ImageViewer::save(){
	if(!image3->isNull()){
		QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QDir::currentPath(), tr("Images (*.png *.xpm *.jpg)"));
		image3->save(fileName);
	}

}
void ImageViewer::newStage(){
	if(!image1->isNull()){
		int r = QMessageBox::warning(this, tr("Warning"), tr("The project will be closed. "
				"Do you really want to do it?"), QMessageBox::Yes | QMessageBox::No );
		if(r == QMessageBox::Yes){
			deleteStage();
		} else if(r == QMessageBox::No){
			//do nothing
		}
	}

}
void ImageViewer::zoomIn() {
	// valor configurado para que seja possível voltar ao
	// tamanho original da imagem | 1.25 * 0.8 = 1
	scaleImage(1.25);
}
void ImageViewer::zoomOut() {
	// valor configurado para que seja possível voltar ao
	// tamanho original da imagem
	scaleImage(0.8);
}
void ImageViewer::normalSize() {
	// usa o método sizeHint() que retorna o tamanho recomendado para a widget
	// em outras palavras, faz com que o label tenha o tamanho de seu filho
	imageLabel->adjustSize();
	scaleFactor = 1.0;
}
void ImageViewer::fitToWindow() {
    bool fitToWindow = fitToWindowAct->isChecked();
    // faz com que o scroll area reconfigure o tamanho de seus filhos para evitar o aparecimento
    // de scroll bar, ou seja, fará com que seu filho seja do seu próprio tamanho
    // quando é falso (default), o scroll area honra o tamanho de seu filho
    scrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow)
        normalSize();

    updateActions();
}
void ImageViewer::about() {
	const QString message = "<p>The <b> " + *softwareName + " </b> helps you to process images in an easy way.</p>"
		            		 "<p>There are several options to play. :)</p>";
	QMessageBox::about(this, *softwareName, message);
}
void ImageViewer::toolBarVisible(){
	if(!toolBarVisibilityAct->isChecked()){
		fileToolBar->setVisible(false);
		zoomControlToolBar->setVisible(false);
	} else {
		fileToolBar->setVisible(true);
		zoomControlToolBar->setVisible(true);
	}
}
void ImageViewer::dockVisible(){
	docker->setVisible(!dockVisibilityAct->isChecked()? false : true);
}
void ImageViewer::dockVisibleFromDocker(bool visible){
	dockVisibilityAct->setChecked(visible);
}
void ImageViewer::statusBarVisible(){
	statusBar()->setVisible(!statusBarVisibilityAct->isChecked()? false : true);
}
void ImageViewer::closeApp(){
	QSettings settings("OpenSourceDPI", *softwareName);
	settings.setValue("MainWindow/Size", size());
	settings.setValue("MainWindow/Properties", saveState());
	close();
}
//------------------------------------DPI SLOTS
void ImageViewer::regionDetection(){
	SepRegionsDialog *sepRegions = new SepRegionsDialog;
	if(image1 != 0)
		sepRegions->setImageName(*fileName);
	else
		sepRegions->setImageName("None");

	if(sepRegions->exec() == PROCESS){
		if(image2 == 0){
			image2 = FacilitiesFacade::applyRegionDetection(*image1, sepRegions->getCloudFactor(), sepRegions->getShadowFactor());
			if(image3 != 0) delete image3;
			image3 = Utilities::prepareRegionDetection(*image1, *image2);
			imageLabel->setPixmap(QPixmap::fromImage(*image3));
		} else { // Differenciate the entry image for detection
			delete image2;
			image2 = FacilitiesFacade::applyRegionDetection(*image2, sepRegions->getCloudFactor(), sepRegions->getShadowFactor());
			if(image3 != 0) delete image3;
			image3 = Utilities::prepareRegionDetection(*image1, *image2);
			imageLabel->setPixmap(QPixmap::fromImage(*image3));
		}
	}
	delete sepRegions;
}

void ImageViewer::morphologicalOperation(){
	MorphologicalOperationDialog *morph = new MorphologicalOperationDialog;
	if(image1 != 0)
			morph->setImageName(*fileName);
		else
			morph->setImageName("None");
	if(morph->exec() == PROCESS){
		TePDIFilterMask::pointer pErosion, pDilation;
		int seSizeErosion, seSizeDilation;
		if(morph->isErosionChecked() && morph->isDilationChecked()){
			if(morph->isOpeningChecked()){ // opening
				apErosion(*morph, seSizeErosion, pErosion);
				apDilation(*morph, seSizeDilation, pDilation);
			} else { // closing
				apDilation(*morph, seSizeDilation, pDilation);
				apErosion(*morph, seSizeErosion, pErosion);
			}
		} else if(morph->isErosionChecked()){ // only erosion
			apErosion(*morph, seSizeErosion, pErosion);
		} else { // only dilation
			apDilation(*morph, seSizeDilation, pDilation);
		}
		if(image3 != 0) delete image3;
		image3 = Utilities::prepareRegionDetection(*image1, *image2);
		imageLabel->setPixmap(QPixmap::fromImage(*image3));
	}
	delete morph;
}

void ImageViewer::inpaintMethod(){
	InpaintDialog *inp = new InpaintDialog;
	if(image1 != 0){
		inp->setSourceImgName(*fileName);
		inp->setModifiedImgName("Current stage image");
	} else {
		inp->setSourceImgName("None");
		inp->setModifiedImgName("None");
	}
	if(inp->exec() == PROCESS){
		if(image2 == 0){
			image2 = FacilitiesFacade::applyInpaint(*image1, *image1, inp->getIterationNumber());
		} else {
			QImage *imgAux = FacilitiesFacade::applyInpaint(*image1, *image2, inp->getIterationNumber());
			delete image2;
			image2 = imgAux;
		}
		if(image3 != 0) delete image3;
		image3 = image2;
		imageLabel->setPixmap(QPixmap::fromImage(*image3));
	}
	delete inp;
}

void ImageViewer::textureSynthesisMethod(){
	TextureSynthesisDialog *texSyn = new TextureSynthesisDialog;
	/*int r = */texSyn->exec();
}

void ImageViewer::decomposeMethod(){
	DecomposeDialog *dec = new DecomposeDialog;
	/*int r = */dec->exec();
}

void ImageViewer::medianFilterMethod(){
	MedianFilterDialog *medFil = new MedianFilterDialog;
	/*int r = */medFil->exec();
}

void ImageViewer::apErosion(MorphologicalOperationDialog &morph, int &seSizeErosion, TePDIFilterMask::pointer &p){
	switch(morph.getTypeErosionSelected()){
	case 0: // SQUARE
		seSizeErosion = morph.getSESizeErosion();
		p = FacilitiesFacade::createStructElement(StructElement::SQUARE, seSizeErosion);
		break;
	case 1: // DISK
		seSizeErosion = morph.getSESizeErosion();
		p = FacilitiesFacade::createStructElement(StructElement::DISK, seSizeErosion);
		break;
	case 2: // PERIODICLINE
		int v[2];
		morph.getSESizeAndVErosion(seSizeErosion, v);
		p = FacilitiesFacade::createStructElement(StructElement::PERIODICLINE, seSizeErosion, v);
		break;
	}// fim switch

	// apply erosion
	if(image2 == 0) {
		image2 = FacilitiesFacade::applyErosion(*image1, p, seSizeErosion);
	} else {
		QImage *imgAux = FacilitiesFacade::applyErosion(*image2, p, seSizeErosion);
		delete image2;
		image2 = imgAux;
	}
}

void ImageViewer::apDilation(MorphologicalOperationDialog &morph, int &seSizeDilation, TePDIFilterMask::pointer &p){
	switch(morph.getTypeDilationSelected()){
	case 0: // SQUARE
		seSizeDilation = morph.getSESizeDilation();
		p = FacilitiesFacade::createStructElement(StructElement::SQUARE, seSizeDilation);
		break;
	case 1: // DISK
		seSizeDilation = morph.getSESizeDilation();
		p = FacilitiesFacade::createStructElement(StructElement::DISK, seSizeDilation);
		break;
	case 2: // PERIODICLINE
		int v[2];
		morph.getSESizeAndVDilation(seSizeDilation, v);
		p = FacilitiesFacade::createStructElement(StructElement::PERIODICLINE, seSizeDilation, v);
		break;
	}

	// apply dilation
	if(image2 == 0) {
		image2 = FacilitiesFacade::applyDilation(*image1, p, seSizeDilation);
	} else {
		QImage *imgAux = FacilitiesFacade::applyDilation(*image2, p, seSizeDilation);
		delete image2;
		image2 = imgAux;
	}
}
