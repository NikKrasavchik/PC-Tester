#pragma once

#include <QDialog>
#include "ui_verificationtest.h"
#include "Components.h"


class VerificationTest : public QDialog
{
	Q_OBJECT

public:
	VerificationTest(QWidget *parent = nullptr);
	~VerificationTest();

private:
	Ui::VerificationTestClass ui;

	bool isTestRun;
	// init
	void initConnect();
	void initStyles();


private slots:
	// Buttons
	void slot_StartStopButton_clicked();

	// Signals
	void slot_ReciveMsg(int msg[8]); // Сообщение с этапом теста
};

