#include "verificationtest.h"

VerificationTest::VerificationTest(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	isTestRun = false;


	initConnect();
	initStyles();
}

VerificationTest::~VerificationTest()
{}

void VerificationTest::initConnect()
{
	connect(ui.StartStopButton, &QPushButton::clicked, this, &VerificationTest::slot_StartStopButton_clicked);


}

void VerificationTest::initStyles()
{
	if (viewWindowState->appTheme == LIGHT_THEME)
	{
		ui.StartTimeLabel->setStyleSheet(lightStyles.settingSelectText);
		ui.EndTimeLable->setStyleSheet(lightStyles.settingSelectText);
		ui.TimeTestLable->setStyleSheet(lightStyles.settingSelectText);

	}
	else
	{
		ui.StartTimeLabel->setStyleSheet(darkStyles.settingSelectText);
		ui.EndTimeLable->setStyleSheet(darkStyles.settingSelectText);
		ui.TimeTestLable->setStyleSheet(darkStyles.settingSelectText);

	}

}

void VerificationTest::slot_StartStopButton_clicked() // Кнопка старт/стоп тест
{
	if (isTestRun) 
	{
		// Остонавливаем тест
	
	}
	else
	{
		// Запускаем тест
	
	}
	isTestRun = !isTestRun;
}

void VerificationTest::slot_ReciveMsg(int msg[8])
{
}




