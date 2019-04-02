/*
 * Decompose.cpp
 *
 *  Created on: Jan 3, 2013
 *      Author: 10088000201
 */

#include "DecomposeDialog.h"

DecomposeDialog::DecomposeDialog() : CustomDialog() {
	setDialogWindowTitle(tr("Decompose method"));
	configureDialogLayout();
	initWidgetsComponents();
	includeDefaultButtons();
}

DecomposeDialog::~DecomposeDialog() {}

void DecomposeDialog::setDialogWindowTitle(const QString& titleName) {
	setWindowTitle(titleName);
}

void DecomposeDialog::configureDialogLayout() {
	gLayout = new QGridLayout;
	getMainDialogLayout()->addLayout(gLayout);
	getMainDialogLayout()->addStretch();
}

void DecomposeDialog::initWidgetsComponents() {
	browseSourceImageButton = new QPushButton(tr("Browse"));
	selectSourceImageLabel = new QLabel(tr("Select source image:"));
	sigmaLabel = new QLabel(tr("Sigma value:"));
	filterLabel = new QLabel(tr("Filter value:"));
	limitLabel = new QLabel(tr("Limit value:"));
	selectSourceImageCombo = new QComboBox;
	sigmaLineEdit = new QLineEdit;
	filterLineEdit = new QLineEdit;
	limitLineEdit = new QLineEdit;

	gLayout->addWidget(selectSourceImageLabel, 0, 0);
	gLayout->addWidget(selectSourceImageCombo, 0, 1);
	gLayout->addWidget(browseSourceImageButton, 0, 2);
	gLayout->addWidget(sigmaLabel, 1, 0);
	gLayout->addWidget(sigmaLineEdit, 1, 1);
	gLayout->addWidget(filterLabel, 2, 0);
	gLayout->addWidget(filterLineEdit, 2, 1);
	gLayout->addWidget(limitLabel, 3, 0);
	gLayout->addWidget(limitLineEdit, 3, 1);
}

QString DecomposeDialog::getSourceImageSelected() {
	return selectSourceImageCombo->currentText();
}

int DecomposeDialog::getSigmaValue() {
	return sigmaLineEdit->text().toInt();
}

int DecomposeDialog::getFilterValue() {
	return filterLineEdit->text().toInt();
}

int DecomposeDialog::getLimitValue() {
	return limitLineEdit->text().toInt();
}
