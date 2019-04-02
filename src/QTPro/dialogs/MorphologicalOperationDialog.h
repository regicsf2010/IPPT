/*
 * MorphologicalOpen.h
 *
 *  Created on: Dec 7, 2012
 *      Author: 10088000201
 */

#ifndef MORPHOLOGICALOPEN_H_
#define MORPHOLOGICALOPEN_H_

#include <QtGui>
#include "abstractDialog/CustomDialog.h"

class QGridLayout;
class QPushButton;
class QLabel;
class QLineEdit;
class QComboBox;
class QCheckBox;
class QRadioButton;
class QRegExp;
class QValidator;

class MorphologicalOperationDialog: public CustomDialog {

	Q_OBJECT

private slots:
	void updateErosionWidgets(int state);
	void updateDilationWidgets(int state);


public:
	MorphologicalOperationDialog();
	virtual ~MorphologicalOperationDialog();
	int getSESizeErosion();
	int getSESizeDilation();
	void getSESizeAndVErosion(int &seSize, int v[]);
	void getSESizeAndVDilation(int &seSize, int v[]);

	void setImageName(const QString &fileName);
	int getTypeErosionSelected();
	int getTypeDilationSelected();
	bool isErosionChecked();
	bool isDilationChecked();
	bool isOpeningChecked();
	bool isClosingChecked();

	virtual void setDialogWindowTitle(const QString &titleName);
	virtual void configureDialogLayout();
	virtual void initWidgetsComponents();
	void configureRegValidator();


private:
	QGridLayout *gLayout;
	QLabel *imagePathLabel, *selectImageLabel, *seTypeErosionLabel, *seSizeErosionLabel, *seTypeDilationLabel, *seSizeDilationLabel, *chooseOperationLabel;
	QLineEdit *seSizeErosionLineEdit, *seSizeDilationLineEdit;
	QComboBox *seErosionTypeCombo, *seDilationTypeCombo;
	QCheckBox *erosionCheckBox, *dilationCheckBox;
	QRadioButton *openingRadioButton, *closingRadioButton;
	QRegExp *rx;
	QValidator *validator;

	void verifyCheckableComponents();
	enum Types{ SQUARE, DISK, PERIODICLINE };
};

#endif /* MORPHOLOGICALOPEN_H_ */
