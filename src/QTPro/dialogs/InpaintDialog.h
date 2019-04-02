/*
 * InpaintDialog.h
 *
 *  Created on: Jan 3, 2013
 *      Author: 10088000201
 */

#ifndef INPAINTDIALOG_H_
#define INPAINTDIALOG_H_

#include <QtGui>
#include "abstractDialog/CustomDialog.h"

class QGridLayout;
class QPushButton;
class QLabel;
class QLineEdit;
class QComboBox;

class InpaintDialog: public CustomDialog {
	//por enquanto não se tem slots
	//Q_OBJECT

public:
	InpaintDialog();
	virtual ~InpaintDialog();
	int getIterationNumber();
	void setSourceImgName(const QString &fileName);
	void setModifiedImgName(const QString &fileName);
	virtual void configureDialogLayout();
	virtual void setDialogWindowTitle(const QString &titleName);
	virtual void initWidgetsComponents();

private:
	QHBoxLayout *hItLayout;
	QLabel *selectSourceImageLabel, *selectModifiedImageLabel, *iterationNumberLabel;
	QLineEdit *iterationNumberLineEdit;
};

#endif /* INPAINTDIALOG_H_ */
