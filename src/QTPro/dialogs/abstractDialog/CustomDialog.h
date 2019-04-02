/*
 * CustomDialog.h
 *
 *  Created on: Dec 7, 2012
 *      Author: 10088000201
 */

#ifndef CUSTOMDIALOG_H_
#define CUSTOMDIALOG_H_
#include <QDialog>

class QPushButton;
class QVBoxLayout;
class QHBoxLayout;

class CustomDialog : public QDialog{
public:
	CustomDialog();
	virtual ~CustomDialog();
	virtual void setDialogWindowTitle(const QString &titleName) = 0;
	virtual void configureDialogLayout() = 0;
	virtual void initWidgetsComponents() = 0;

private:
	void initializeMainDialogLayout();
	QVBoxLayout *vMainLayout;
	QHBoxLayout *hLayout;
	QPushButton *okButton, *cancelButton;

protected:
	void includeDefaultButtons();
	QVBoxLayout *getMainDialogLayout();
};

#endif /* CUSTOMDIALOG_H_ */
