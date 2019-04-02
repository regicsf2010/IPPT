/*
 * ImageViewer.h
 *
 *  Created on: Nov 23, 2012
 *      Author: Reginaldo Filho
 *
 *      Based on: http://doc.qt.digia.com/qt/widgets-imageviewer.html
 */

#ifndef IMAGEVIEWER_H_
#define IMAGEVIEWER_H_
#define WIDTH 1000
#define HEIGHT 700

#include <QMainWindow>
#define PROCESS 1
#define CANCEL 0
class QLabel;
class QScrollArea;
class QScrollBar;
class QHBoxLayout;
class MorphologicalOperationDialog;
#include <TePDIFilterMask.hpp>


class ImageViewer : public QMainWindow {

	Q_OBJECT

public:
	ImageViewer();
	~ImageViewer();

private slots:
	void openFile();
	void newStage();
	void save();
    void zoomIn();
    void zoomOut();
    void normalSize();
    void fitToWindow();
    void about();
    void toolBarVisible();
    void statusBarVisible();
    void dockVisible();
    void dockVisibleFromDocker(bool visible);
    void regionDetection();
    void morphologicalOperation();
    void inpaintMethod();
    void textureSynthesisMethod();
    void decomposeMethod();
    void medianFilterMethod();
    void closeApp();



private:
    void createActions();
    void createMenus();
    void createToolBar();
    void createDock();
    void updateActions();
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);

    void configureImagePointer();
    void configureImageLabel();
    void configureImageScrollArea();
    void configureMainLayout();
    void deleteStage();
    void setPermanentStatusBarMessage();
    void readState();

    void apErosion(MorphologicalOperationDialog &morph, int &seSizeErosion, TePDIFilterMask::pointer &p);
    void apDilation(MorphologicalOperationDialog &morph, int &seSizeDilation, TePDIFilterMask::pointer &p);

    const QString *softwareName;
    QLabel *imageSizeStatusBar;
    double scaleFactor;
    QLabel *imageLabel;
    QImage *image1, *image2, *image3;
    QScrollArea *scrollArea;
    QHBoxLayout *mainLayout;
    QString *fileName;


/*---------------------------------------MENU BAR---------------------------------*/
    QMenu *fileMenu;
		QAction *openAct;
		QAction *newAct;
		QAction *saveAct;
		QAction *exitAct;

    QMenu *editMenu;

    QMenu *viewMenu;
    	QAction *toolBarVisibilityAct;
    	QAction *dockVisibilityAct;
    	QAction *statusBarVisibilityAct;
    	QMenu *zoomSubMenu;
			QAction *zoomInAct;
			QAction *zoomOutAct;
			QAction *normalSizeAct;
			QAction *fitToWindowAct;

	QMenu *dpiMenu;
		QAction *regDetectionAct;
		QAction *morphOperationAct;
		QAction *inpaintMethodAct;
		QAction *textureSynthesisAct;
		QAction *decomposeAct;
		QMenu *filtersSubMenu;
			QAction *medianFilterAct;


	QMenu *helpMenu;
    	QAction *aboutAct;


/*---------------------------------------TOOL BAR---------------------------------*/
    QToolBar *fileToolBar;
    QToolBar *zoomControlToolBar;

/*---------------------------------------DOCK-------------------------------------*/

    QDockWidget *docker;
};

#endif /* IMAGEVIEWER_H_ */
