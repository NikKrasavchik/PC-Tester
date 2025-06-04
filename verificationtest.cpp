#include "verificationtest.h"


VerificationTest::VerificationTest(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	isTestRun = false;
	isStendConnection = false;
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

	if (msg[7] == 0 && msg[6] == 0 && msg[5] == 0 && msg[4] == 0 && msg[3] == 0 && msg[2] == 0)
	{
		if (msg[1] == 0)
			return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Тест остановлен") : QString("");
		else if(msg[1] == 1)
			return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Тест запущен") : QString("");
	}
	else if (msg[7] == 0 && msg[6] == 0 && msg[5] == 0 && msg[4] == 0)
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
	else if (msg[7] == 0 && msg[6] == 0)
	{
		if (msg[4] == 1)
		{
			if (msg[5] == 0)
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. ContiniouslyTerminals") : QString("");
			else
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. ContiniouslyTerminals") : QString("");
		}
		else if (msg[4] == 2)
		{
			if (msg[5] == 0)
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. ContiniouslyLights") : QString("");
			else
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. ContiniouslyLights") : QString("");
		}
		else if (msg[4] == 3)
		{
			if (msg[5] == 0)
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. ContiniouslyLock") : QString("");
			else
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. ContiniouslyLock") : QString("");
		}
		else if (msg[4] == 4)
		{
			if (msg[5] == 0)
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. ContiniouslyWindow") : QString("");
			else
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. ContiniouslyWindow") : QString("");
		}
		else if (msg[4] == 5)
		{
			if (msg[5] == 0)
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. ContiniouslyOthers") : QString("");
			else
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. ContiniouslyOthers") : QString("");
		}
	}
	else
	{
		if (msg[6] == 1)
		{
			if (msg[7] == 0)
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Mirror Adj X Right(D6-D7)") : QString("");
			else
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Mirror Adj X Right(D6-D7)") : QString("");
		}
		else if (msg[6] == 2)
		{
			if (msg[7] == 0)
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Mirror Adj X Left(D7-D6)") : QString("");
			else
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Mirror Adj X Left(D7-D6)") : QString("");
		}
		else if (msg[6] == 3)
		{
			if (msg[7] == 0)
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Mirror Adj Y Up(D7-D5)") : QString("");
			else
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Mirror Adj Y Up(D7-D5)") : QString("");
		}
		else if (msg[6] == 4)
		{
			if (msg[7] == 0)
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Mirror Adj Y Down(D5-D7)") : QString("");
			else
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Mirror Adj Y Down(D5-D7)") : QString("");
		}
		else if (msg[6] == 5)
		{
			if (msg[7] == 0)
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Mirror Fold (2A) / Mirror Adj(D7-D8)") : QString("");
			else
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Mirror Fold (2A) / Mirror Adj(D7-D8)") : QString("");
		}
		else if (msg[6] == 6)
		{
			if (msg[7] == 0)
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Mirror Unfold (2A)(D8-D7)") : QString("");
			else
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Mirror Unfold (2A)(D8-D7)") : QString("");
		}
		else if (msg[6] == 7)
		{
			if (msg[7] == 0)
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Door Handle Light(A6) | Safety light / Sill light(A16) | DMFx: Turn Indicator(D9) | DMFx: Blind Spot LED(D13)") : QString("");
			else
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Door Handle Light(A6) | Safety light / Sill light(A16) | DMFx: Turn Indicator(D9) | DMFx: Blind Spot LED(D13)") : QString("");
		}
		else if (msg[6] == 8)
		{
			if (msg[7] == 0)
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Mirror Heating DefrostM(D1) | Armrest Heat(B7)") : QString("");
			else
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Mirror Heating DefrostM(D1) | Armrest Heat(B7)") : QString("");
		}
		else if (msg[6] == 9)
		{
			if (msg[7] == 0)
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Child lock(4A)(B9)") : QString("");
			else
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Child lock(4A)(B9)") : QString("");
		}
		else if (msg[6] == 10)
		{
			if (msg[7] == 0)
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Child Unlock(4A)(B12)") : QString("");
			else
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Child Unlock(4A)(B12)") : QString("");
		}
		else if (msg[6] == 11)
		{
			if (msg[7] == 0)
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Single Lock(C11-C12)") : QString("");
			else
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Single Lock(C11-C12)") : QString("");
		}
		else if (msg[6] == 12)
		{
			if (msg[7] == 0)
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Single Unlock(C12-C11)") : QString("");
			else
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Single Unlock(C12-C11)") : QString("");
		}
		else if (msg[6] == 13)
		{
			if (msg[7] == 0)
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Double Lock(C12-C9)") : QString("");
			else
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Double Lock(C12-C9)") : QString("");
		}
		else if (msg[6] == 14)
		{
			if (msg[7] == 0)
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Double Unlock(C9-C12)") : QString("");
			else
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Double Unlock(C9-C12)") : QString("");
		}
		else if (msg[6] == 15)
		{
			if (msg[7] == 0)
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Window Up(A19)") : QString("");
			else
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Window Up(A19)	") : QString("");
		}
		else if (msg[6] == 16)
		{
		if (msg[7] == 0)
			return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Window Down(A20)") : QString("");
		else
			return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Window Down(A20)") : QString("");
		}
		else if (msg[6] == 17)
		{
		if (msg[7] == 0)
			return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Reserved(A15) | Reserved(C6)") : QString("");
		else
			return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Reserved(A15) | Reserved(C6)") : QString("");
		}
		else if (msg[6] == 18)
		{
		if (msg[7] == 0)
			return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Door_movement_motor (+ close)(B10)") : QString("");
		else
			return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Door_movement_motor (+ close)(B10)") : QString("");
		}
		else if (msg[6] == 19)
		{
		if (msg[7] == 0)
			return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Door_movement_motor (+ open)(B13)") : QString("");
		else
			return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Door_movement_motor (+ open)(B13)") : QString("");
		}
		else if (msg[6] == 20)
		{
		if (msg[7] == 0)
			return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. SunShade Motor Door Open(B1)") : QString("");
		else
			return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. SunShade Motor Door Open(B1)") : QString("");
		}
		else if (msg[6] == 21)
		{
		if (msg[7] == 0)
			return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. SunShade Motor Door Close(B4)") : QString("");
		else
			return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. SunShade Motor Door Close(B4)") : QString("");
		}
		else if (msg[6] == 22)
		{
		if (msg[7] == 0)
			return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Sun Shade Motor Rear Window Close(C7)") : QString("");
		else
			return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Sun Shade Motor Rear Window Close(C7)") : QString("");
		}
		else if (msg[6] == 23)
		{
		if (msg[7] == 0)
			return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Sun Shade Motor Rear Window Open(C10)") : QString("");
		else
			return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Sun Shade Motor Rear Window Open(C10)") : QString("");
		}

	}
	QString tmpStr = "Нераспознаное сообщение. С CAN пришло: id: 0xAA msg: 0x" + QString::number(msg[0], 16) + " 0x" + QString::number(msg[1], 16) + " 0x" + QString::number(msg[2], 16) + " 0x" + QString::number(msg[3], 16) + " 0x" + QString::number(msg[4], 16) + " 0x" + QString::number(msg[5], 16) + " 0x" + QString::number(msg[6], 16) + " 0x" + QString::number(msg[7], 16);
	return tmpStr;
}

void VerificationTest::slot_StartStopButton_clicked() // Кнопка старт/стоп тест
{
	if (isTestRun) 
	{
		// Заканчиваем тест
		TimerTimeTest->stop();

		ui.StartStopButton->setText(viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Старт") : QString("Start"));
		Can::verificationStartStop(false);
	}
	else
	{
		if (!isStendConnection)
		{
			QMessageBox::warning(this, QString("Внимание"), viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Блок не подключкен") : QString("The block is not connected"));
			return;
		}
		// Начинаем тест
		timeTest = QTime(0,0,0);
		setTimeTest(timeTest);
		
		ui.StartStopButton->setText(viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Стоп") : QString("Stop"));
		ui.progressBar->setValue(0);
		ui.listWidget->clear();

		TimerTimeTest->start(TIME_SECOND);
		Can::verificationStartStop(true);
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
	//int tmpMsg[8];
	//for (int i = 0, j = 7; i < 8; i++, j--)
	//	tmpMsg[i] = msg[j];
	QListWidgetItem* item = new QListWidgetItem(ui.listWidget);
	CustomListItem* widget = new CustomListItem(getTextByMsg(msg), timeTest.toString("hh.mm.ss") + " | " + QTime::currentTime().toString("hh:mm:ss"));

	item->setSizeHint(widget->sizeHint());
	ui.listWidget->setItemWidget(item, widget);
}

void VerificationTest::Slot_ChangedStatusStandConnect(bool statusConnect)
{
	isStendConnection = statusConnect;
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
