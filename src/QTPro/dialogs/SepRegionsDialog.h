/*
 * SepRegionsDialog.h
 *
 *  Created on: Dec 3, 2012
 *      Author: 10088000201
 */

#ifndef SEPREGIONSDIALOG_H_
#define SEPREGIONSDIALOG_H_

#include <QtGui>
#include "abstractDialog/CustomDialog.h"

class QGridLayout;
class QHBoxLayout;
class QLabel;
class QLineEdit;

class SepRegionsDialog: public CustomDialog {

	Q_OBJECT

public:
	SepRegionsDialog();
	void setImageName(const QString &fileName);
	double getCloudFactor();
	double getShadowFactor();
	virtual void setDialogWindowTitle(const QString &titleName);
	virtual void configureDialogLayout();
	virtual void initWidgetsComponents();

private:
	QHBoxLayout *hImageNameLayout;
	QGridLayout *gLayout;

	QLabel *imagePathLabel, *cFactorLabel, *sFactorLabel;
	QLineEdit *cloudFactorLineEdit, *shadowFactorLineEdit;
};

#endif /* SEPREGIONSDIALOG_H_ */
