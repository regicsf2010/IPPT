/*
 * MedianFilterDialog.h
 *
 *  Created on: Jan 3, 2013
 *      Author: 10088000201
 */

#ifndef MEDIANFILTERDIALOG_H_
#define MEDIANFILTERDIALOG_H_

#include <QtGui>
#include "abstractDialog/CustomDialog.h"

class QGridLayout;
class QPushButton;
class QLabel;
class QLineEdit;
class QComboBox;

class MedianFilterDialog: public CustomDialog {
	//por enquanto não se tem slots
	//Q_OBJECT

public:
	MedianFilterDialog();
	virtual ~MedianFilterDialog();
	QString getSourceImageSelected();
	int getLines();
	int getCols();

	virtual void setDialogWindowTitle(const QString &titleName);
	virtual void configureDialogLayout();
	virtual void initWidgetsComponents();

private:
	QGridLayout *gLayout;
	QPushButton *browseSourceImageButton;
	QLabel *selectSourceImageLabel, *maskDimensionLabel, *linesLabel, *colsLabel;
	QLineEdit *linesLineEdit, *colsLineEdit;
	QComboBox *selectSourceImageCombo;
};

#endif /* MEDIANFILTERDIALOG_H_ */
