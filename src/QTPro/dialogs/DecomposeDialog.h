/*
 * Decompose.h
 *
 *  Created on: Jan 3, 2013
 *      Author: 10088000201
 */

#ifndef DECOMPOSE_H_
#define DECOMPOSE_H_

#include <QtGui>
#include "abstractDialog/CustomDialog.h"

class QGridLayout;
class QPushButton;
class QLabel;
class QLineEdit;
class QComboBox;

class DecomposeDialog: public CustomDialog {
	//por enquanto não se tem slots
	//Q_OBJECT

public:
	DecomposeDialog();
	virtual ~DecomposeDialog();
	QString getSourceImageSelected();
	int getSigmaValue();
	int getFilterValue();
	int getLimitValue();

	virtual void setDialogWindowTitle(const QString &titleName);
	virtual void configureDialogLayout();
	virtual void initWidgetsComponents();

private:
	QGridLayout *gLayout;
	QPushButton *browseSourceImageButton;
	QLabel *selectSourceImageLabel, *sigmaLabel, *filterLabel, *limitLabel;
	QLineEdit *sigmaLineEdit, *filterLineEdit, *limitLineEdit;
	QComboBox *selectSourceImageCombo;
};

#endif /* DECOMPOSE_H_ */
