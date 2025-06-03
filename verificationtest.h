#pragma once

#include <QDialog>
#include "ui_verificationtest.h"
#include <QDateTime>
#include <qtimer.h>

#include "Components.h"

#define TIME_SECOND		1000 // ���������� ��� ������� � ����������� �������� 1000. ��������� ��������
class VerificationTest : public QDialog
{
	Q_OBJECT

public:
	VerificationTest(QWidget *parent = nullptr);
	~VerificationTest();

private:
	Ui::VerificationTestClass ui;

	QTimer* TimerTimeTest; 
	
	bool isTestRun;
	QTime timeTest; // ����� ������ �����
	// init
	void initConnect();
	void initStyles();
	void initText();


	//
	void setTimeTest(QTime time);
	QString getTextByMsg(int msg[8]);

private slots:
	// Buttons
	void slot_StartStopButton_clicked();
	// Timer
	void slot_TimerTimeTest();

public slots:
	// Signals
	void Slot_ReciveMsg(int msg[8]); // ��������� � ������ �����
};


class CustomListItem : public QWidget {
	Q_OBJECT  

public:
	CustomListItem(const QString& leftText, const QString& rightText, QWidget* parent = nullptr);
};

