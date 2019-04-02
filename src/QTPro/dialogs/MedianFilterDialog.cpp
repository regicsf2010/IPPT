/*
 * MedianFilterDialog.cpp
 *
 *  Created on: Jan 3, 2013
 *      Author: 10088000201
 */

#include "MedianFilterDialog.h"

MedianFilterDialog::MedianFilterDialog() : CustomDialog() {
	setDialogWindowTitle(tr("Median filter method"));
	configureDialogLayout();
	initWidgetsComponents();
	includeDefaultButtons();
}

MedianFilterDialog::~MedianFilterDialog() {}

void MedianFilterDialog::setDialogWindowTitle(const QString& titleName) {
	setWindowTitle(titleName);
}

void MedianFilterDialog::configureDialogLayout() {
	gLayout = new QGridLayout;
	getMainDialogLayout()->addLayout(gLayout);
	getMainDialogLayout()->addStretch();
}

void MedianFilterDialog::initWidgetsComponents() {
	browseSourceImageButton = new QPushButton(tr("Browse"));
	selectSourceImageLabel = new QLabel(tr("Select source image:"));
	maskDimensionLabel = new QLabel(tr("Inform mask dimension:"));
	linesLabel = new QLabel(tr("Lines:"));
	colsLabel = new QLabel(tr("Columns:"));
	selectSourceImageCombo = new QComboBox;
	linesLineEdit = new QLineEdit;
	colsLineEdit = new QLineEdit;

	gLayout->addWidget(selectSourceImageLabel, 0, 0);
	gLayout->addWidget(selectSourceImageCombo, 0, 1);
	gLayout->addWidget(browseSourceImageButton, 0, 2);
	gLayout->addWidget(maskDimensionLabel, 1, 0);
	gLayout->addWidget(linesLabel, 2, 0);
	gLayout->addWidget(linesLineEdit, 2, 1);
	gLayout->addWidget(colsLabel, 3, 0);
	gLayout->addWidget(colsLineEdit, 3, 1);
}

QString MedianFilterDialog::getSourceImageSelected() {
	return selectSourceImageCombo->currentText();
}

int MedianFilterDialog::getLines() {
	return linesLineEdit->text().toInt();
}

int MedianFilterDialog::getCols() {
	return colsLineEdit->text().toInt();
}
