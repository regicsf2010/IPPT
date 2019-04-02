/*
 * MorphologicalOpen.cpp
 *
 *  Created on: Dec 7, 2012
 *      Author: 10088000201
 */

#include "MorphologicalOperationDialog.h"
#include <QRegExp>
#include <QValidator>

MorphologicalOperationDialog::MorphologicalOperationDialog() : CustomDialog(){
	setDialogWindowTitle(tr("Morphological operation"));
	configureDialogLayout();
	initWidgetsComponents();
	configureRegValidator();
	includeDefaultButtons();
}

MorphologicalOperationDialog::~MorphologicalOperationDialog() {}

void MorphologicalOperationDialog::configureRegValidator(){
	rx = new QRegExp("^([0-9]*)[[]([0-9]*)[,]([0-9]*)[]]$");
	validator = new QRegExpValidator(*rx, this);
	seSizeErosionLineEdit->setValidator(validator);
	seSizeDilationLineEdit->setValidator(validator);
}

void MorphologicalOperationDialog::setDialogWindowTitle(const QString &titleName){
	setWindowTitle(titleName);
}

void MorphologicalOperationDialog::configureDialogLayout(){
	gLayout = new QGridLayout;
	getMainDialogLayout()->addLayout(gLayout);
	getMainDialogLayout()->addStretch();
}

void MorphologicalOperationDialog::initWidgetsComponents(){
	selectImageLabel = new QLabel(tr("Image path:"));
	seTypeErosionLabel = new QLabel(tr("Struct element type:"));
	seTypeErosionLabel->setEnabled(false);
	seSizeErosionLabel = new QLabel(tr("Struct element size:"));
	seSizeErosionLabel->setEnabled(false);
	seTypeDilationLabel = new QLabel(tr("Struct element type:"));
	seTypeDilationLabel->setEnabled(false);
	seSizeDilationLabel = new QLabel(tr("Struct element size:"));
	seSizeDilationLabel->setEnabled(false);
	seSizeErosionLineEdit = new QLineEdit;
	seSizeErosionLineEdit->setEnabled(false);
	seSizeDilationLineEdit = new QLineEdit;
	seSizeDilationLineEdit->setEnabled(false);
	imagePathLabel = new QLabel;
	seErosionTypeCombo = new QComboBox;
	seErosionTypeCombo->setEnabled(false);
	seErosionTypeCombo->addItem("SQUARE");
	seErosionTypeCombo->addItem("DISK");
	seErosionTypeCombo->addItem("PERIODICLINE");
	seDilationTypeCombo = new QComboBox;
	seDilationTypeCombo->setEnabled(false);
	seDilationTypeCombo->addItem("SQUARE");
	seDilationTypeCombo->addItem("DISK");
	seDilationTypeCombo->addItem("PERIODICLINE");
	erosionCheckBox = new QCheckBox(tr("Apply erosion"));
	connect(erosionCheckBox, SIGNAL(stateChanged (int)), this, SLOT(updateErosionWidgets(int)));
	dilationCheckBox = new QCheckBox(tr("Apply dilation"));
	connect(dilationCheckBox, SIGNAL(stateChanged (int)), this, SLOT(updateDilationWidgets(int)));
	chooseOperationLabel = new QLabel(tr("Choose operation:"));
	chooseOperationLabel->setEnabled(false);
	openingRadioButton = new QRadioButton(tr("Opening"));
	openingRadioButton->setChecked(true);
	openingRadioButton->setEnabled(false);
	closingRadioButton = new QRadioButton(tr("Closing"));
	closingRadioButton->setEnabled(false);

	gLayout->addWidget(selectImageLabel, 0, 0);
	gLayout->addWidget(imagePathLabel, 0, 1);

	gLayout->addWidget(erosionCheckBox, 1, 0);
	gLayout->addWidget(seTypeErosionLabel, 2, 0);
	gLayout->addWidget(seErosionTypeCombo, 2, 1);
	gLayout->addWidget(seSizeErosionLabel, 3, 0);
	gLayout->addWidget(seSizeErosionLineEdit, 3, 1);

	gLayout->addWidget(dilationCheckBox, 4, 0);
	gLayout->addWidget(seTypeDilationLabel, 5, 0);
	gLayout->addWidget(seDilationTypeCombo, 5, 1);
	gLayout->addWidget(seSizeDilationLabel, 6, 0);
	gLayout->addWidget(seSizeDilationLineEdit, 6, 1);

	gLayout->addWidget(chooseOperationLabel, 7, 0);
	gLayout->addWidget(openingRadioButton, 8, 0);
	gLayout->addWidget(closingRadioButton, 8, 1);
}

void MorphologicalOperationDialog::setImageName(const QString &fileName) {
	imagePathLabel->setText(fileName);
}

int MorphologicalOperationDialog::getTypeErosionSelected() {
	return seErosionTypeCombo->currentIndex();
}

int MorphologicalOperationDialog::getSESizeErosion(){
	return seSizeErosionLineEdit->text().toInt();
}

int MorphologicalOperationDialog::getTypeDilationSelected() {
	return seDilationTypeCombo->currentIndex();
}

int MorphologicalOperationDialog::getSESizeDilation(){
	return seSizeDilationLineEdit->text().toInt();
}

bool MorphologicalOperationDialog::isErosionChecked(){
	return erosionCheckBox->isChecked();
}

bool MorphologicalOperationDialog::isDilationChecked(){
	return dilationCheckBox->isChecked();
}

bool MorphologicalOperationDialog::isOpeningChecked(){
	return openingRadioButton->isChecked();
}

bool MorphologicalOperationDialog::isClosingChecked(){
	return closingRadioButton->isChecked();
}

void MorphologicalOperationDialog::updateErosionWidgets(int state){
	switch(state){
	case Qt::Unchecked:
		seTypeErosionLabel->setEnabled(false);
		seSizeErosionLabel->setEnabled(false);
		seErosionTypeCombo->setEnabled(false);
		seSizeErosionLineEdit->setEnabled(false);
		break;

	case Qt::Checked:
		seTypeErosionLabel->setEnabled(true);
		seSizeErosionLabel->setEnabled(true);
		seErosionTypeCombo->setEnabled(true);
		seSizeErosionLineEdit->setEnabled(true);
		break;
	}
	verifyCheckableComponents();
}

void MorphologicalOperationDialog::updateDilationWidgets(int state){
	switch(state){
		case Qt::Unchecked:
			seTypeDilationLabel->setEnabled(false);
			seSizeDilationLabel->setEnabled(false);
			seDilationTypeCombo->setEnabled(false);
			seSizeDilationLineEdit->setEnabled(false);
			break;

		case Qt::Checked:
			seTypeDilationLabel->setEnabled(true);
			seSizeDilationLabel->setEnabled(true);
			seDilationTypeCombo->setEnabled(true);
			seSizeDilationLineEdit->setEnabled(true);
			break;
		}
	verifyCheckableComponents();
}

void MorphologicalOperationDialog::verifyCheckableComponents(){
	if(isErosionChecked() & isDilationChecked()){
		chooseOperationLabel->setEnabled(true);
		openingRadioButton->setEnabled(true);
		closingRadioButton->setEnabled(true);
	} else {
		chooseOperationLabel->setEnabled(false);
		openingRadioButton->setEnabled(false);
		closingRadioButton->setEnabled(false);
	}
}

void MorphologicalOperationDialog::getSESizeAndVErosion(int &seSize, int v[]){
	int pos = rx->indexIn(seSizeErosionLineEdit->text());
	if(pos > -1){
		seSize = rx->cap(1).toInt();
		v[0] = rx->cap(2).toInt();
		v[1] = rx->cap(3).toInt();
	}
}

void MorphologicalOperationDialog::getSESizeAndVDilation(int &seSize, int v[]){
	int pos = rx->indexIn(seSizeDilationLineEdit->text());
	if(pos > -1){
		seSize = rx->cap(1).toInt();
		v[0] = rx->cap(2).toInt();
		v[1] = rx->cap(3).toInt();
	}
}
