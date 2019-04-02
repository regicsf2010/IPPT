/*
 * TextureSynthesisDialog.h
 *
 *  Created on: Jan 3, 2013
 *      Author: 10088000201
 */

#ifndef TEXTURESYNTHESISDIALOG_H_
#define TEXTURESYNTHESISDIALOG_H_

#include <QtGui>
#include "abstractDialog/CustomDialog.h"

class QGridLayout;
class QPushButton;
class QLabel;
class QComboBox;

class TextureSynthesisDialog: public CustomDialog {
	//por enquanto não se tem slots
	//Q_OBJECT

public:
	TextureSynthesisDialog();
	virtual ~TextureSynthesisDialog();
	QString getSourceImageSelected();
	QString getModifiedImageSelected();

	virtual void setDialogWindowTitle(const QString &titleName);
	virtual void configureDialogLayout();
	virtual void initWidgetsComponents();

private:
	QGridLayout *gLayout;
	QPushButton *browseSourceImageButton, *browseModifiedImageButton;
	QLabel *selectSourceImageLabel, *selectModifiedImageLabel;
	QComboBox *selectSourceImageCombo, *selectModifiedImageCombo;
};

#endif /* TEXTURESYNTHESISDIALOG_H_ */
