#pragma once

#include <QDialog>
#include "ui_verificationtest.h"
#include <QDateTime>
#include <qtimer.h>
#include <qmessagebox.h>
#include "Components.h"
#include "Can.h"
#define TIME_SECOND		100 // Необходимо для отладки и стандартное значение 1000. Уменьшает задержки
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
	bool isStendConnection;
	QTime timeTest; // Время теста

	// init
	void initTable();
	void initConnect();
	void initStyles();
	void initText();


	//
	void setTimeTest(QTime time);
	QString getTextByMsg(int msg[8]);
	void setTextTable(int row, int column, QString textRus, QString textEng = QString(""));
	bool configProgressBar();

	void resizeEvent(QResizeEvent* event);

private slots:
	// Buttons
	void slot_StartStopButton_clicked();
	// Timer
	void slot_TimerTimeTest();
	// CheckBox
	void slot_CheckBox();

public slots:
	// Signals
	void Slot_ReciveMsg(int msg[8]); // Сообщение с этапом теста
	void Slot_ChangedStatusStandConnect(bool statusConnect);

};


class CustomListItem : public QWidget {
	Q_OBJECT  

public:
	CustomListItem(const QString& leftText, const QString& rightText, QWidget* parent = nullptr);
};

