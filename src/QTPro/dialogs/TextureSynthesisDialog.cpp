/*
 * TextureSynthesisDialog.cpp
 *
 *  Created on: Jan 3, 2013
 *      Author: 10088000201
 */

#include "TextureSynthesisDialog.h"

TextureSynthesisDialog::TextureSynthesisDialog() : CustomDialog() {
	setDialogWindowTitle(tr("Texture Synthesis method"));
	configureDialogLayout();
	initWidgetsComponents();
	includeDefaultButtons();
}

TextureSynthesisDialog::~TextureSynthesisDialog() {}

void TextureSynthesisDialog::setDialogWindowTitle(const QString& titleName) {
	setWindowTitle(titleName);
}

void TextureSynthesisDialog::configureDialogLayout() {
	gLayout = new QGridLayout;
	getMainDialogLayout()->addLayout(gLayout);
	getMainDialogLayout()->addStretch();
}

void TextureSynthesisDialog::initWidgetsComponents() {
	browseSourceImageButton = new QPushButton(tr("Browse"));
	browseModifiedImageButton = new QPushButton(tr("Browse"));
	selectSourceImageLabel = new QLabel(tr("Select source image:"));
	selectModifiedImageLabel = new QLabel(tr("Select modified image:"));
	selectSourceImageCombo = new QComboBox;
	selectModifiedImageCombo = new QComboBox;
	gLayout->addWidget(selectSourceImageLabel, 0, 0);
	gLayout->addWidget(selectSourceImageCombo, 0, 1);
	gLayout->addWidget(browseSourceImageButton, 0, 2);
	gLayout->addWidget(selectModifiedImageLabel, 1, 0);
	gLayout->addWidget(selectModifiedImageCombo, 1, 1);
	gLayout->addWidget(browseModifiedImageButton, 1, 2);
}

QString TextureSynthesisDialog::getSourceImageSelected() {
	return selectSourceImageCombo->currentText();
}

QString TextureSynthesisDialog::getModifiedImageSelected() {
	return selectModifiedImageCombo->currentText();
}
