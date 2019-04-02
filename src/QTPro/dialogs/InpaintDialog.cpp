/*
 * InpaintDialog.cpp
 *
 *  Created on: Jan 3, 2013
 *      Author: 10088000201
 */

#include "InpaintDialog.h"

InpaintDialog::InpaintDialog() : CustomDialog() {
	setDialogWindowTitle(tr("Inpaint method"));
	initWidgetsComponents();
	configureDialogLayout();
	includeDefaultButtons();
}

InpaintDialog::~InpaintDialog() {}

void InpaintDialog::setDialogWindowTitle(const QString& titleName) {
	setWindowTitle(titleName);
}

void InpaintDialog::configureDialogLayout() {
	getMainDialogLayout()->addLayout(hItLayout);
	getMainDialogLayout()->addStretch();
}

void InpaintDialog::initWidgetsComponents() {
	selectSourceImageLabel = new QLabel(tr("Source image path:"));
	selectModifiedImageLabel = new QLabel(tr("Modified image path:"));
	iterationNumberLabel = new QLabel(tr("Iteration number: "));
	iterationNumberLineEdit = new QLineEdit;
	hItLayout = new QHBoxLayout;
	getMainDialogLayout()->addWidget(selectSourceImageLabel);
	getMainDialogLayout()->addWidget(selectModifiedImageLabel);
	hItLayout->addWidget(iterationNumberLabel);
	hItLayout->addWidget(iterationNumberLineEdit);
}

int InpaintDialog::getIterationNumber() {
	return iterationNumberLineEdit->text().toInt();
}

void InpaintDialog::setSourceImgName(const QString &fileName){
	selectSourceImageLabel->setText(selectSourceImageLabel->text() + " " + fileName);
}

void InpaintDialog::setModifiedImgName(const QString &fileName){
	selectModifiedImageLabel->setText(selectModifiedImageLabel->text() + " " + fileName);
}

