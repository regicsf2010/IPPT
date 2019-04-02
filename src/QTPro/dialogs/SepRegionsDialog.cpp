/*
 * SepRegionsDialog.cpp
 *
 *  Created on: Dec 3, 2012
 *      Author: 10088000201
 */

#include "SepRegionsDialog.h"

SepRegionsDialog::SepRegionsDialog() : CustomDialog(){
	setDialogWindowTitle(tr("Region detection"));
	configureDialogLayout();
	initWidgetsComponents();
	includeDefaultButtons();
}

void SepRegionsDialog::setDialogWindowTitle(const QString &titleName) {
	setWindowTitle(titleName);
}

void SepRegionsDialog::configureDialogLayout(){
	gLayout = new QGridLayout;
	hImageNameLayout = new QHBoxLayout;
	getMainDialogLayout()->addLayout(hImageNameLayout);
	getMainDialogLayout()->addLayout(gLayout);
	getMainDialogLayout()->addStretch();
}

void SepRegionsDialog::initWidgetsComponents(){
	imagePathLabel = new QLabel(tr("Image path:"));
	cFactorLabel = new QLabel(tr("Cloud factor:"));
	sFactorLabel = new QLabel(tr("Shadow factor:"));
	cloudFactorLineEdit = new QLineEdit;
	shadowFactorLineEdit = new QLineEdit;

	hImageNameLayout->addWidget(imagePathLabel);
	gLayout->addWidget(cFactorLabel, 0, 0);
	gLayout->addWidget(cloudFactorLineEdit, 0, 1);
	gLayout->addWidget(sFactorLabel, 1, 0);
	gLayout->addWidget(shadowFactorLineEdit, 1, 1);
}

void SepRegionsDialog::setImageName(const QString &fileName){
	imagePathLabel->setText(imagePathLabel->text() + " " + fileName);
}

double SepRegionsDialog::getCloudFactor(){
	return cloudFactorLineEdit->text().toDouble();
}

double SepRegionsDialog::getShadowFactor(){
	return shadowFactorLineEdit->text().toDouble();
}
