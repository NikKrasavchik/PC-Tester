#include "verificationtest.h"

VerificationTest::VerificationTest(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	isTestRun = false;
	TimerTimeTest = new QTimer();


	initConnect();
	initStyles();
	initText();

}

VerificationTest::~VerificationTest()
{}

void VerificationTest::initConnect()
{
	connect(ui.StartStopButton, &QPushButton::clicked, this, &VerificationTest::slot_StartStopButton_clicked);
	connect(TimerTimeTest, SIGNAL(timeout()), this, SLOT(slot_TimerTimeTest()));

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

void VerificationTest::initText()
{
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		ui.StartStopButton->setText(QString("Старт"));
		ui.TimeTestLable->setText(QString("Время тестирования\n00:00:00"));
		break;

	case ENGLISH_LANG:
		ui.StartStopButton->setText(QString("Start"));
		ui.TimeTestLable->setText(QString("Time test\n00:00:00"));
		break;
	}
}

void VerificationTest::setTimeTest(QTime time)
{
	ui.TimeTestLable->setText((viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Время тестирования\n") : QString("Time test\n")) + time.toString("hh:mm:ss"));
}

QString VerificationTest::getTextByMsg(int msg[8])
{

	if (msg[7] == 0 && msg[6] == 0 && msg[5] == 0 && msg[4] && msg[3] == 0 && msg[2] == 0)
	{
		if (msg[1] == 0)
			return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Тест остановлен") : QString("");
		else
			return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Тест запущен") : QString("");
	}
	else if (msg[7] == 0 && msg[6] == 0 && msg[5] == 0 && msg[4])
	{
		if (msg[2] == 1)
		{
			if(msg[3] == 0)
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("SEQ_1 окончен") : QString("");
			else
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("SEQ_1 запущен") : QString("");
		}
		else if (msg[2] == 2)
		{
			if (msg[3] == 0)
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("SEQ_2 окончен") : QString("");
			else
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("SEQ_2 запущен") : QString("");
		}
		if (msg[2] == 3)
		{
			if (msg[3] == 0)
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("SEQ_3 окончен") : QString("");
			else
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("SEQ_3 запущен") : QString("");
		}
		if (msg[2] == 4)
		{
			if (msg[3] == 0)
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("SEQ_4 окончен") : QString("");
			else
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("SEQ_4 запущен") : QString("");
		}
		if (msg[2] == 5)
		{
			if (msg[3] == 0)
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("SEQ_5 окончен") : QString("");
			else
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("SEQ_5 запущен") : QString("");
		}
		if (msg[2] == 6)
		{
			if (msg[3] == 0)
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("SEQ_6 окончен") : QString("");
			else
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("SEQ_6 запущен") : QString("");
		}
		if (msg[2] == 7)
		{
			if (msg[3] == 0)
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("SEQ_7 окончен") : QString("");
			else
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("SEQ_7 запущен") : QString("");
		}
		if (msg[2] == 8)
		{
			if (msg[3] == 0)
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("SEQ_8 окончен") : QString("");
			else
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("SEQ_8 запущен") : QString("");
		}
		if (msg[2] == 9)
		{
			if (msg[3] == 0)
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("SEQ_9 окончен") : QString("");
			else
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("SEQ_9 запущен") : QString("");
		}
		if (msg[2] == 10)
		{
			if (msg[3] == 0)
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("SEQ_10 окончен") : QString("");
			else
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("SEQ_10 запущен") : QString("");
		}

	}
	QString tmpStr = "Нераспознаное сообщение. С CAN пришло: id: 0xAA msg: " + QString::number(msg[0], 16) + " " + QString::number(msg[1], 16) + " " + QString::number(msg[2], 16) + " " + QString::number(msg[3], 16) + " " + QString::number(msg[4], 16) + " " + QString::number(msg[5], 16) + " " + QString::number(msg[6], 16) + " " + QString::number(msg[7], 16);
	return tmpStr;
}

void VerificationTest::slot_StartStopButton_clicked() // Кнопка старт/стоп тест
{
	if (isTestRun) 
	{
		// Заканчиваем тест
		TimerTimeTest->stop();

		ui.StartStopButton->setText(viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Старт") : QString("Start"));
	}
	else
	{
		// Начинаем тест
		timeTest = QTime(0,0,0);
		setTimeTest(timeTest);
		ui.StartStopButton->setText(viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Стоп") : QString("Stop"));
		ui.progressBar->setValue(0);
		ui.listWidget->clear();
		TimerTimeTest->start(TIME_SECOND);
	}
	isTestRun = !isTestRun;
}

void VerificationTest::slot_TimerTimeTest()
{
	timeTest = timeTest.addMSecs(TIME_SECOND);
	setTimeTest(timeTest);
	int tmpInt = ui.progressBar->value() + 1;
	ui.progressBar->setValue(tmpInt);
}

void VerificationTest::Slot_ReciveMsg(int msg[8])
{
	QListWidgetItem* item = new QListWidgetItem(ui.listWidget);
	CustomListItem* widget = new CustomListItem(getTextByMsg(msg), timeTest.toString("hh.mm.ss") + " | " + QTime::currentTime().toString("hh:mm:ss"));

	item->setSizeHint(widget->sizeHint());
	ui.listWidget->setItemWidget(item, widget);
}



CustomListItem::CustomListItem(const QString& leftText, const QString& rightText, QWidget* parent)
	: QWidget(parent)
{
	auto* layout = new QHBoxLayout(this);
	layout->setContentsMargins(5, 0, 5, 0);

	QLabel* leftLabel = new QLabel(leftText);
	QLabel* rightLabel = new QLabel(rightText);

	layout->addWidget(leftLabel, 0, Qt::AlignLeft);
	layout->addStretch();
	layout->addWidget(rightLabel, 0, Qt::AlignRight);
}
