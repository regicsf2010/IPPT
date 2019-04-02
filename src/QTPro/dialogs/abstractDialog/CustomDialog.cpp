/*
 * CustomDialog.cpp
 *
 *  Created on: Dec 7, 2012
 *      Author: 10088000201
 */

#include "CustomDialog.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

CustomDialog::CustomDialog() {
	initializeMainDialogLayout();
	// Faz com que o layout do dialog administre o tamanho mínimo da tela
	// e a opção resizable assume false
	layout()->setSizeConstraint(QLayout::SetFixedSize);

}

CustomDialog::~CustomDialog() {}

void CustomDialog::initializeMainDialogLayout(){
	vMainLayout = new QVBoxLayout(this);
}

void CustomDialog::includeDefaultButtons(){
	okButton = new QPushButton(tr("Process"));
	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
	cancelButton = new QPushButton(tr("Cancel"));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	hLayout = new QHBoxLayout;
	vMainLayout->addLayout(hLayout);
	hLayout->addStretch();
	hLayout->addWidget(cancelButton);
	hLayout->addWidget(okButton);
}

QVBoxLayout *CustomDialog::getMainDialogLayout(){
	return vMainLayout;
}

