#pragma once

#include <QDialog>
#include "ui_verificationtest.h"

class VerificationTest : public QDialog
{
	Q_OBJECT

public:
	VerificationTest(QWidget *parent = nullptr);
	~VerificationTest();

private:
	Ui::VerificationTestClass ui;
};

