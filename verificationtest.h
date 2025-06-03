#pragma once

#include <QDialog>
#include "ui_verificationtest.h"
#include <QDateTime>
#include <qtimer.h>

#include "Components.h"

#define TIME_SECOND		1000 // Необходимо для отладки и стандартное значение 1000. Уменьшает задержки
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
	QTime timeTest; // Время начала теста
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
	void Slot_ReciveMsg(int msg[8]); // Сообщение с этапом теста
};


class CustomListItem : public QWidget {
	Q_OBJECT  

public:
	CustomListItem(const QString& leftText, const QString& rightText, QWidget* parent = nullptr);
};

