#include "verificationtest.h"


VerificationTest::VerificationTest(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	isTestRun = false;
	isStendConnection = false;
	TimerTimeTest = new QTimer();

	initTable();
	initConnect();
	initStyles();
	initText();

}

VerificationTest::~VerificationTest()
{}

void VerificationTest::initTable()
{
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
	ui.tableWidget->horizontalHeader()->hide();
	ui.tableWidget->verticalHeader()->hide();

	if (viewWindowState->selectedBlock == TestBlockName::DTM)
	{
		ui.label->setText(QString("DTM"));

		ui.tableWidget->setRowCount(11);
		ui.tableWidget->setColumnCount(14);

		ui.tableWidget->setRowHeight(0, 30);
		for (int i = 1; i < 11; i++)
			ui.tableWidget->verticalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);

		for (int i = 0; i < 14; i++)
			ui.tableWidget->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);

		ui.tableWidget->setSpan(0, 0, 1, 8);
		ui.tableWidget->setSpan(0, 8, 1, 14);

		setTextTable(0, 0, QString("Постоянно включен"), QString("Continiously ON"));
		setTextTable(0, 8, QString("Переодические"), QString("Periodic"));

		setTextTable(1, 0, QString("Mirror"));
		setTextTable(1, 8, QString("Mirror Adj Y Up (D7-D5) "));
		setTextTable(1, 9, QString("Mirror Adj Y Down (D5-D7)"));
		setTextTable(1, 10, QString("Mirror Adj X Right (D6-D7)"));
		setTextTable(1, 11, QString("Mirror Adj X Left (D7-D6)"));
		setTextTable(1, 12, QString("Mirror Fold (2A) / Mirror Adj (D7-D8)"));
		setTextTable(1, 13, QString("Mirror Unfold (2A) (D8-D7)"));


		setTextTable(2, 0, QString("Lights"));
		setTextTable(2, 1, QString("Background Illumination (A14)"));
		setTextTable(2, 2, QString("Indication Door Lock Switch (A17)"));
		setTextTable(2, 3, QString("Reserved (D15)"));
		setTextTable(2, 4, QString("Reserved (D16)"));
		setTextTable(2, 5, QString("Reserved (D17)"));
		setTextTable(2, 6, QString("Reserved (D18)"));
		setTextTable(2, 7, QString("Brake Light High Mounted (B2)"));
		setTextTable(2, 8, QString("Door Handle Light (A6)"));
		setTextTable(2, 9, QString("Safety light / Sill light (A16)"));
		setTextTable(2, 10, QString("Turn Indicator (D9)"));
		setTextTable(2, 11, QString("Blind Spot LED (D13)"));

		setTextTable(3, 0, QString("Lock"));
		setTextTable(3, 1, QString("Soft close Motor Feed (B3)"));
		setTextTable(3, 8, QString("Single Lock (C11-C12)"));
		setTextTable(3, 9, QString("Single Unlock (C12-C11)"));
		setTextTable(3, 10, QString("Double Lock (C12-C9)"));
		setTextTable(3, 11, QString("Double Unlock (C9-C12)"));

		setTextTable(4, 0, QString("Window"));
		setTextTable(4, 1, QString("Window Lifter Feed (B15)"));
		setTextTable(4, 8, QString("Window Up (A19)"));
		setTextTable(4, 9, QString("Window Down (A20)"));

		setTextTable(5, 0, QString("Heat"));
		setTextTable(5, 8, QString("Mirror Heating DefrostM (D1)"));
		setTextTable(5, 9, QString("Armrest Heat (B7)"));

		setTextTable(6, 0, QString("Child"));
		setTextTable(6, 8, QString("Child lock (4A) (B9)"));
		setTextTable(6, 9, QString("Child Unlock (4A) (B12)"));

		setTextTable(7, 0, QString("Terminals"));
		setTextTable(7, 1, QString("DMx_T15_1 (B5)"));
		setTextTable(7, 2, QString("Interior Light (B11)"));
		setTextTable(7, 3, QString("DMx_T30d (B14)"));

		setTextTable(8, 0, QString("Actuator"));
		setTextTable(8, 8, QString("Door_movement_motor (+ close) (B10)"));
		setTextTable(8, 9, QString("Door_movement_motor (+ open) (B13)"));

		setTextTable(9, 0, QString("Others"));
		setTextTable(9, 1, QString("Reserved (B8)"));
		setTextTable(9, 2, QString("Fire extinguisher system relay (C3)"));
		setTextTable(9, 3, QString("ExMr+ (D4)"));
		setTextTable(9, 4, QString("Additional (T30*) battery relay (C8)"));
		setTextTable(9, 8, QString("Reserved (A15)"));
		setTextTable(9, 9, QString("Reserved (C6)"));

		setTextTable(10, 0, QString("SunShade"));
		setTextTable(10, 8, QString("SunShade Motor Door Open (B1)"));
		setTextTable(10, 9, QString("SunShade Motor Door Close (B4)"));
		setTextTable(10, 10, QString("Sun Shade Motor Rear Window Close (C7)"));
		setTextTable(10, 11, QString("Sun Shade Motor Rear Window Open (C10)"));


	}
	else
	{
		ui.label->setText(QString("BCM"));

		ui.tableWidget->setRowCount(12);
		ui.tableWidget->setColumnCount(24);

		ui.tableWidget->setRowHeight(0, 30);
		for (int i = 1; i < 12; i++)
			ui.tableWidget->verticalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);

		for (int i = 0; i < 24; i++)
			ui.tableWidget->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);

		ui.tableWidget->setSpan(0, 0, 1, 17);
		ui.tableWidget->setSpan(0, 17, 1, 7);

		setTextTable(0, 0, QString("Постоянно включен"), QString("Continiously ON"));
		setTextTable(0, 17, QString("Переодические"), QString("Periodic"));

		setTextTable(1, 0, QString("Steering Wheel"));
		setTextTable(1, 17, QString("Steering Wheel In Motor"));
		setTextTable(1, 18, QString("Steering Wheel Out Motor"));
		setTextTable(1, 19, QString("Steering Wheel Down Motor"));
		setTextTable(1, 20, QString("Steering Wheel Up Motor"));


		setTextTable(2, 0, QString("Heater Window"));
		setTextTable(2, 1, QString("Heated washer Tub/Jets"));
		setTextTable(2, 17, QString("Heated Aquablade"));
		setTextTable(2, 18, QString("Heated windshield"));
		setTextTable(2, 19, QString("Windscreen FCU film heater"));


		setTextTable(3, 0, QString("Heater Driver"));
		setTextTable(3, 17, QString("Windscreen FCU film heater"));
		setTextTable(3, 17, QString("Armrest Heat Front Left Inner"));
		setTextTable(3, 17, QString("Armrest Heat Rear Left Inner"));
		setTextTable(3, 17, QString("Armrest Heat Rear Right Inner"));
		setTextTable(3, 17, QString("Steering Wheel Heating Indicator"));


		setTextTable(4, 0, QString("Washer"));
		setTextTable(4, 1, QString("Front Wiper Speed High"));
		setTextTable(4, 2, QString("Front Wiper Speed Low"));
		setTextTable(4, 17, QString("Rear Window Wash"));
		setTextTable(4, 18, QString("Windscreen Wash"));
		setTextTable(4, 19, QString("Washer sys	tem valve 1 (lidars 1 and 2)"));
		setTextTable(4, 20, QString("Washer system valve 2 (radar)"));
		setTextTable(4, 21, QString("Washer system valve 3 (thermal imager)"));
		setTextTable(4, 22, QString("Washer system valve 4 (headlights)"));
		setTextTable(4, 23, QString("Headlights and ADAS washer pump"));


		setTextTable(5, 0, QString("Indicators"));
		setTextTable(5, 1, QString("Rear ceiling lights button indication"));
		setTextTable(5, 2, QString("Stealth button indication"));
		setTextTable(5, 3, QString("Reserved - Power distribution unit indication"));
		setTextTable(5, 4, QString("Hazard Switch Illumination"));


		setTextTable(6, 0, QString("Internal Lights"));
		setTextTable(6, 1, QString("Side Marker  Left"));
		setTextTable(6, 2, QString("Side Marker Right"));
		setTextTable(6, 3, QString("Background Illumination"));
		setTextTable(6, 4, QString("Door sill FL Power"));
		setTextTable(6, 5, QString("Door sill FR Power"));
		setTextTable(6, 6, QString("Door sill RL Power"));
		setTextTable(6, 7, QString("Door sill RR Power"));
		setTextTable(6, 17, QString("Rear Courtesy Light (MPV)"));
		setTextTable(6, 18, QString("Ultraviolet ceiling plafon Light"));
		setTextTable(6, 19, QString("Underhood box illumination light (BEV)"));
		setTextTable(6, 20, QString("Glove box light"));


		setTextTable(7, 0, QString("Terminals"));
		setTextTable(7, 1, QString("3_Interior Light Terminal"));
		setTextTable(7, 2, QString("Light BCM"));
		setTextTable(7, 3, QString("T15 (engine 1)"));
		setTextTable(7, 4, QString("ADAS terminal (engine)"));
		setTextTable(7, 5, QString("T15 (cabin 1)"));
		setTextTable(7, 6, QString("T15 (cabin 6)"));
		setTextTable(7, 7, QString("2_Interior Light Terminal"));
		setTextTable(7, 8, QString("4_Interior Light Terminal"));
		setTextTable(7, 9, QString("1_Interior Light Terminal"));
		setTextTable(7, 10, QString("T15 (cabin 5)"));
		setTextTable(7, 11, QString("T15 (cabin 3)"));
		setTextTable(7, 12, QString("Head-up display terminal"));
		setTextTable(7, 13, QString("CC Terminal"));
		setTextTable(7, 14, QString("T30d"));
		setTextTable(7, 15, QString("T15h"));
		setTextTable(7, 16, QString("T30i relay"));


		setTextTable(8, 0, QString("APU and RLS"));
		setTextTable(8, 1, QString("APU Heat "));
		setTextTable(8, 2, QString("RLS Power"));


		setTextTable(9, 0, QString("Powers"));
		setTextTable(9, 1, QString("Sun roof Logic Power "));
		setTextTable(9, 2, QString("ARS power left"));
		setTextTable(9, 3, QString("ARS power right"));
		setTextTable(9, 4, QString("Power Steering Fluid Ctrl"));
		setTextTable(9, 5, QString("12V Power Sockets (front)"));
		setTextTable(9, 17, QString("HV lid unlock motor"));
		setTextTable(9, 18, QString("Vibration motor (CDA)"));
		setTextTable(9, 19, QString("Horn Ctrl"));


		setTextTable(10, 0, QString("Footrest"));
		setTextTable(10, 17, QString("RL passenger footrest motor (up)"));
		setTextTable(10, 18, QString("RL passenger footrest motor (down)"));
		setTextTable(10, 19, QString("RR passenger footrest motor (down)"));
		setTextTable(10, 20, QString("RR passenger footrest motor(up)"));


		setTextTable(11, 0, QString("Spare"));
		setTextTable(11, 1, QString("Spare output (high-side 1)"));
		setTextTable(11, 2, QString("Spare output (high-side 2)"));
		setTextTable(11, 3, QString("Spare output (high-side 10)"));
		setTextTable(11, 4, QString("Spare output (high-side 7)"));
		setTextTable(11, 5, QString("Spare output (high-side 8)"));
		setTextTable(11, 6, QString("Spare output (high-side 9)"));
		setTextTable(11, 7, QString("Spare output (high-side 11)"));
		setTextTable(11, 8, QString("Spare output (high-side 6)"));
		setTextTable(11, 9, QString("Spare output (high-side 5)"));
		setTextTable(11, 17, QString("Spare output (half-bridge 1)"));
		setTextTable(11, 18, QString("Spare output (half-bridge 2)"));
		setTextTable(11, 19, QString("Spare output (half-bridge 3)"));
		setTextTable(11, 20, QString("Spare output (half-bridge 4)"));

	}


}

void VerificationTest::initConnect()
{
	connect(ui.StartStopButton, &QPushButton::clicked, this, &VerificationTest::slot_StartStopButton_clicked);
	connect(TimerTimeTest, SIGNAL(timeout()), this, SLOT(slot_TimerTimeTest()));

	connect(ui.checkBox, &QCheckBox::clicked, this, &VerificationTest::slot_CheckBox);
	connect(ui.checkBox_2, &QCheckBox::clicked, this, &VerificationTest::slot_CheckBox);
	connect(ui.checkBox_3, &QCheckBox::clicked, this, &VerificationTest::slot_CheckBox);
	connect(ui.checkBox_4, &QCheckBox::clicked, this, &VerificationTest::slot_CheckBox);
	connect(ui.checkBox_5, &QCheckBox::clicked, this, &VerificationTest::slot_CheckBox);
	connect(ui.checkBox_6, &QCheckBox::clicked, this, &VerificationTest::slot_CheckBox);
	connect(ui.checkBox_7, &QCheckBox::clicked, this, &VerificationTest::slot_CheckBox);
	connect(ui.checkBox_8, &QCheckBox::clicked, this, &VerificationTest::slot_CheckBox);
	connect(ui.checkBox_9, &QCheckBox::clicked, this, &VerificationTest::slot_CheckBox);
	connect(ui.checkBox_10, &QCheckBox::clicked, this, &VerificationTest::slot_CheckBox);
}

void VerificationTest::initStyles()
{
	if (viewWindowState->appTheme == LIGHT_THEME)
	{
		ui.StartTimeLabel->setStyleSheet(lightStyles.settingSelectText);
		ui.EndTimeLable->setStyleSheet(lightStyles.settingSelectText);
		ui.TimeTestLable->setStyleSheet(lightStyles.settingSelectText);
		ui.label->setStyleSheet(lightStyles.settingSelectText);
		ui.label_2->setStyleSheet(lightStyles.settingSelectText);
		ui.checkBox->setStyleSheet(lightStyles.settingSelectText);
		ui.checkBox_2->setStyleSheet(lightStyles.settingSelectText);
		ui.checkBox_3->setStyleSheet(lightStyles.settingSelectText);
		ui.checkBox_4->setStyleSheet(lightStyles.settingSelectText);
		ui.checkBox_5->setStyleSheet(lightStyles.settingSelectText);
		ui.checkBox_6->setStyleSheet(lightStyles.settingSelectText);
		ui.checkBox_7->setStyleSheet(lightStyles.settingSelectText);
		ui.checkBox_8->setStyleSheet(lightStyles.settingSelectText);
		ui.checkBox_9->setStyleSheet(lightStyles.settingSelectText);
		ui.checkBox_10->setStyleSheet(lightStyles.settingSelectText);
		ui.tableWidget->setStyleSheet(lightStyles.testwindowTableWidget);
		ui.StartStopButton->setStyleSheet(lightStyles.standButtons);

	}
	else
	{
		ui.StartTimeLabel->setStyleSheet(darkStyles.settingSelectText);
		ui.EndTimeLable->setStyleSheet(darkStyles.settingSelectText);
		ui.TimeTestLable->setStyleSheet(darkStyles.settingSelectText);
		ui.label->setStyleSheet(darkStyles.settingSelectText);
		ui.label_2->setStyleSheet(darkStyles.settingSelectText);
		ui.checkBox->setStyleSheet(darkStyles.settingSelectText);
		ui.checkBox_2->setStyleSheet(darkStyles.settingSelectText);
		ui.checkBox_3->setStyleSheet(darkStyles.settingSelectText);
		ui.checkBox_4->setStyleSheet(darkStyles.settingSelectText);
		ui.checkBox_5->setStyleSheet(darkStyles.settingSelectText);
		ui.checkBox_6->setStyleSheet(darkStyles.settingSelectText);
		ui.checkBox_7->setStyleSheet(darkStyles.settingSelectText);
		ui.checkBox_8->setStyleSheet(darkStyles.settingSelectText);
		ui.checkBox_9->setStyleSheet(darkStyles.settingSelectText);
		ui.checkBox_10->setStyleSheet(darkStyles.settingSelectText);
		ui.tableWidget->setStyleSheet(darkStyles.testwindowTableWidget);
		ui.StartStopButton->setStyleSheet(darkStyles.standButtons);



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
		if (viewWindowState->selectedBlock == TestBlockName::DTM)
		{


			if (msg[4] == 1)
			{
				if (msg[5] == 0)
				{
					ui.tableWidget->item(7, 1)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(7, 2)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(7, 3)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Continiously_Terminals") : QString("");
				}
				else
				{
					ui.tableWidget->item(7, 1)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(7, 2)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(7, 3)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Continiously_Terminals") : QString("");
				}
			}
			else if (msg[4] == 2)
			{
				if (msg[5] == 0)
				{
					ui.tableWidget->item(2, 1)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(2, 2)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(2, 3)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(2, 4)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(2, 5)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(2, 6)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(2, 7)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Continiously_Lights") : QString("");
				}
				else
				{
					ui.tableWidget->item(2, 1)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(2, 2)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(2, 3)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(2, 4)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(2, 5)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(2, 6)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(2, 7)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Continiously_Lights") : QString("");
				}
			}
			else if (msg[4] == 3)
			{
				if (msg[5] == 0)
				{
					ui.tableWidget->item(3, 1)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Continiously_Lock") : QString("");
				}
				else
				{
					ui.tableWidget->item(3, 1)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Continiously_Lock") : QString("");
				}
			}
			else if (msg[4] == 4)
			{
				if (msg[5] == 0)
				{
					ui.tableWidget->item(4, 1)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Continiously_Window") : QString("");
				}
				else
				{
					ui.tableWidget->item(4, 1)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Continiously_Window") : QString("");
				}
			}
			else if (msg[4] == 5)
			{
				if (msg[5] == 0)
				{
					ui.tableWidget->item(9, 1)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(9, 2)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(9, 3)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(9, 4)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Continiously_Others") : QString("");
				}
				else
				{
					ui.tableWidget->item(9, 1)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(9, 2)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(9, 3)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(9, 4)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Continiously_Others") : QString("");
				}
			}
		}
		else
		{
			if (msg[4] == 1)
			{
				if (msg[5] == 0)
				{
					ui.tableWidget->item(2, 1)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Continiously_Heater_Window") : QString("");
				}
				else
				{
					ui.tableWidget->item(2, 1)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Continiously_Heater_Window") : QString("");
				}
			}
			else if (msg[4] == 2)
			{
				if (msg[5] == 0)
				{
					ui.tableWidget->item(4, 1)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(4, 2)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Continiously_Washer") : QString("");
				}
				else
				{
					ui.tableWidget->item(4, 1)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(4, 2)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Continiously_Washer") : QString("");
				}
			}
			else if (msg[4] == 3)
			{
				if (msg[5] == 0)
				{
					ui.tableWidget->item(5, 1)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(5, 2)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(5, 3)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(5, 4)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Continiously_Indicators") : QString("");
				}
				else
				{
					ui.tableWidget->item(5, 1)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(5, 2)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(5, 3)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(5, 4)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Continiously_Indicators") : QString("");
				}
			}
			else if (msg[4] == 4)
			{
				if (msg[5] == 0)
				{
					ui.tableWidget->item(6, 1)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(6, 2)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(6, 3)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(6, 4)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(6, 5)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(6, 6)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(6, 7)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Continiously_Internal_Lights") : QString("");
				}
				else
				{
					ui.tableWidget->item(6, 1)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(6, 2)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(6, 3)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(6, 4)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(6, 5)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(6, 6)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(6, 7)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Continiously_Internal_Lights") : QString("");
				}
			}
			else if (msg[4] == 5)
			{
				if (msg[5] == 0)
				{
					ui.tableWidget->item(7, 1)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(7, 2)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(7, 3)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(7, 4)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(7, 5)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(7, 6)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(7, 7)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(7, 8)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(7, 9)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(7, 9)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(7, 10)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(7, 11)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(7, 12)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(7, 13)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(7, 14)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(7, 15)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(7, 16)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Continiously_Terminals") : QString("");
				}
				else
				{
					ui.tableWidget->item(7, 1)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(7, 2)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(7, 3)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(7, 4)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(7, 5)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(7, 6)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(7, 7)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(7, 8)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(7, 9)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(7, 10)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(7, 11)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(7, 12)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(7, 13)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(7, 14)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(7, 15)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(7, 16)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Continiously_Terminals") : QString("");
				}
			}
			else if (msg[4] == 6)
			{
				if (msg[5] == 0)
				{
					ui.tableWidget->item(8, 1)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(8, 2)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Continiously_APU_and_RLS") : QString("");
				}
				else
				{
					ui.tableWidget->item(8, 1)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(8, 2)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Continiously_APU_and_RLS") : QString("");
				}
			}
			else if (msg[4] == 7)
			{
				if (msg[5] == 0)
				{
					ui.tableWidget->item(9, 1)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(9, 2)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(9, 3)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(9, 4)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(9, 5)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Continiously_Powers") : QString("");
				}
				else
				{
					ui.tableWidget->item(9, 1)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(9, 2)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(9, 3)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(9, 4)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(9, 5)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Continiously_Powers") : QString("");
				}
			}
			else if (msg[4] == 8)
			{
				if (msg[5] == 0)
				{
					ui.tableWidget->item(11, 1)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(11, 2)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(11, 3)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(11, 4)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(11, 5)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(11, 6)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(11, 7)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(11, 8)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(11, 9)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Continiously_Spare") : QString("");
				}
				else
				{
					ui.tableWidget->item(11, 1)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(11, 2)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(11, 3)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(11, 4)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(11, 5)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(11, 6)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(11, 7)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(11, 8)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(11, 9)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Continiously_Spare") : QString("");
				}
			}
		}
	}
	else
	{
		if (viewWindowState->selectedBlock == TestBlockName::DTM)
		{
			if (msg[6] == 1)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(1, 10)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Mirror Adj X Right(D6-D7)") : QString("");
				}
				else
				{
					ui.tableWidget->item(1, 10)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Mirror Adj X Right(D6-D7)") : QString("");
				}
			}
			else if (msg[6] == 2)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(1, 11)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Mirror Adj X Left(D7-D6)") : QString("");
				}
				else
				{
					ui.tableWidget->item(1, 11)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Mirror Adj X Left(D7-D6)") : QString("");
				}
			}
			else if (msg[6] == 3)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(1, 8)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Mirror Adj Y Up(D7-D5)") : QString("");
				}
				else
				{
					ui.tableWidget->item(1, 8)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Mirror Adj Y Up(D7-D5)") : QString("");
				}
			}
			else if (msg[6] == 4)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(1, 9)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Mirror Adj Y Down(D5-D7)") : QString("");
				}
				else
				{
					ui.tableWidget->item(1, 9)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Mirror Adj Y Down(D5-D7)") : QString("");
				}
			}
			else if (msg[6] == 5)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(1, 12)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Mirror Fold (2A) / Mirror Adj(D7-D8)") : QString("");
				}
				else
				{
					ui.tableWidget->item(1, 12)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Mirror Fold (2A) / Mirror Adj(D7-D8)") : QString("");
				}
			}
			else if (msg[6] == 6)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(1, 13)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Mirror Unfold (2A)(D8-D7)") : QString("");
				}
				else
				{
					ui.tableWidget->item(1, 13)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Mirror Unfold (2A)(D8-D7)") : QString("");
				}
			}
			else if (msg[6] == 7)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(2, 8)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(2, 9)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(2, 10)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(2, 11)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Door Handle Light(A6) | Safety light / Sill light(A16) | DMFx: Turn Indicator(D9) | DMFx: Blind Spot LED(D13)") : QString("");
				}
				else
				{
					ui.tableWidget->item(2, 8)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(2, 9)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(2, 10)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(2, 11)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Door Handle Light(A6) | Safety light / Sill light(A16) | DMFx: Turn Indicator(D9) | DMFx: Blind Spot LED(D13)") : QString("");
				}
			}
			else if (msg[6] == 8)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(5, 8)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(5, 9)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Mirror Heating DefrostM(D1) | Armrest Heat(B7)") : QString("");
				}
				else
				{
					ui.tableWidget->item(5, 8)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(5, 9)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Mirror Heating DefrostM(D1) | Armrest Heat(B7)") : QString("");
				}
			}
			else if (msg[6] == 9)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(6, 8)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Child lock(4A)(B9)") : QString("");
				}
				else
				{
					ui.tableWidget->item(6, 8)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Child lock(4A)(B9)") : QString("");
				}
			}
			else if (msg[6] == 10)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(6, 9)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Child Unlock(4A)(B12)") : QString("");
				}
				else
				{
					ui.tableWidget->item(6, 9)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Child Unlock(4A)(B12)") : QString("");
				}
			}
			else if (msg[6] == 11)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(3, 8)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Single Lock(C11-C12)") : QString("");
				}
				else
				{
					ui.tableWidget->item(3, 8)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Single Lock(C11-C12)") : QString("");
				}
			}
			else if (msg[6] == 12)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(3, 9)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Single Unlock(C12-C11)") : QString("");
				}
				else
				{
					ui.tableWidget->item(3, 9)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Single Unlock(C12-C11)") : QString("");
				}
			}
			else if (msg[6] == 13)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(3, 10)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Double Lock(C12-C9)") : QString("");
				}
				else
				{
					ui.tableWidget->item(3, 10)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Double Lock(C12-C9)") : QString("");
				}
			}
			else if (msg[6] == 14)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(3, 11)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Double Unlock(C9-C12)") : QString("");
				}
				else
				{
					ui.tableWidget->item(3, 11)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Double Unlock(C9-C12)") : QString("");
				}
			}
			else if (msg[6] == 15)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(4, 8)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Window Up(A19)") : QString("");
				}
				else
				{
					ui.tableWidget->item(4, 8)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Window Up(A19)	") : QString("");
				}
			}
			else if (msg[6] == 16)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(4, 9)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Window Down(A20)") : QString("");
				}
				else
				{
					ui.tableWidget->item(4, 9)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Window Down(A20)") : QString("");
				}
			}
			else if (msg[6] == 17)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(9, 8)->setBackground(QColor(Qt::transparent));
					ui.tableWidget->item(9, 9)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Reserved(A15) | Reserved(C6)") : QString("");
				}
				else
				{
					ui.tableWidget->item(9, 8)->setBackground(QColor(COLOR_LIGHT_GREEN));
					ui.tableWidget->item(9, 9)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Reserved(A15) | Reserved(C6)") : QString("");
				}
			}
			else if (msg[6] == 18)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(8, 8)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Door_movement_motor (+ close)(B10)") : QString("");
				}
				else
				{
					ui.tableWidget->item(8, 8)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Door_movement_motor (+ close)(B10)") : QString("");
				}
			}
			else if (msg[6] == 19)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(8, 9)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Door_movement_motor (+ open)(B13)") : QString("");
				}
				else
				{
					ui.tableWidget->item(8, 9)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Door_movement_motor (+ open)(B13)") : QString("");
				}
			}
			else if (msg[6] == 20)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(10, 8)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. SunShade Motor Door Open(B1)") : QString("");
				}
				else
				{
					ui.tableWidget->item(10, 8)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. SunShade Motor Door Open(B1)") : QString("");
				}
			}
			else if (msg[6] == 21)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(10, 9)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. SunShade Motor Door Close(B4)") : QString("");
				}
				else
				{
					ui.tableWidget->item(10, 9)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. SunShade Motor Door Close(B4)") : QString("");
				}
			}
			else if (msg[6] == 22)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(10, 10)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Sun Shade Motor Rear Window Close(C7)") : QString("");
				}
				else
				{
					ui.tableWidget->item(10, 10)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Sun Shade Motor Rear Window Close(C7)") : QString("");
				}
			}
			else if (msg[6] == 23)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(10, 11)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Sun Shade Motor Rear Window Open(C10)") : QString("");
				}
				else
				{
					ui.tableWidget->item(10, 11)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Sun Shade Motor Rear Window Open(C10)") : QString("");
				}
			}
		}
		else
		{
			if (msg[6] == 1)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(1, 17)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл.Steering Wheel In Motor") : QString("");
				}
				else
				{
					ui.tableWidget->item(1, 17)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Steering Wheel In Motor") : QString("");
				}
			}
			else if (msg[6] == 2)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(1, 18)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Steering Wheel Out Motor") : QString("");
				}
				else
				{
					ui.tableWidget->item(1, 18)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Steering Wheel Out Motor") : QString("");
				}
			}
			else if (msg[6] == 3)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(1, 19)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Steering Wheel Down Motor") : QString("");
				}
				else
				{
					ui.tableWidget->item(1, 19)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Steering Wheel Down Motor") : QString("");
				}
			}
			else if (msg[6] == 4)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(1, 20)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Steering Wheel Up Motor") : QString("");
				}
				else
				{
					ui.tableWidget->item(1, 20)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Steering Wheel Up Motor") : QString("");
				}
			}
			else if (msg[6] == 5)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(2, 17)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Heated Aquablade") : QString("");
				}
				else
				{
					ui.tableWidget->item(2, 17)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Heated Aquablade") : QString("");
				}
			}
			else if (msg[6] == 6)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(2, 18)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Heated windshield") : QString("");
				}
				else
				{
					ui.tableWidget->item(2, 18)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Heated windshield") : QString("");
				}
			}
			else if (msg[6] == 7)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(2, 19)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Windscreen FCU film heater") : QString("");
				}
				else
				{
					ui.tableWidget->item(2, 19)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Windscreen FCU film heater") : QString("");
				}
			}
			else if (msg[6] == 8)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(3, 17)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Armrest Heat Front Right Inner") : QString("");
				}
				else
				{
					ui.tableWidget->item(3, 17)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Armrest Heat Front Right Inner") : QString("");
				}
			}
			else if (msg[6] == 9)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(3, 18)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Armrest Heat Front Left Inner") : QString("");
				}
				else
				{
					ui.tableWidget->item(3, 18)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Armrest Heat Front Left Inner") : QString("");
				}
			}
			else if (msg[6] == 10)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(3, 19)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Armrest Heat Rear Left Inner") : QString("");
				}
				else
				{
					ui.tableWidget->item(3, 19)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Armrest Heat Rear Left Inner") : QString("");
				}
			}
			else if (msg[6] == 11)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(3, 20)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Armrest Heat Rear Right Inner") : QString("");
				}
				else
				{
					ui.tableWidget->item(3, 20)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Armrest Heat Rear Right Inner") : QString("");
				}
			}
			else if (msg[6] == 12)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(3, 21)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Steering Wheel Heating Indicator") : QString("");
				}
				else
				{
					ui.tableWidget->item(3, 21)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Steering Wheel Heating Indicator") : QString("");
				}
			}
			else if (msg[6] == 13)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(4, 17)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Rear Window Wash") : QString("");
				}
				else
				{
					ui.tableWidget->item(4, 17)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Rear Window Wash") : QString("");
				}
			}
			else if (msg[6] == 14)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(4, 18)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Windscreen Wash") : QString("");
				}
				else
				{
					ui.tableWidget->item(4, 18)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Windscreen Wash") : QString("");
				}
			}
			else if (msg[6] == 15)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(4, 19)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Washer system valve 1") : QString("");
				}
				else
				{
					ui.tableWidget->item(4, 19)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Washer system valve 1") : QString("");
				}
			}
			else if (msg[6] == 16)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(4, 20)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Washer system valve 2 (radar)") : QString("");
				}
				else
				{
					ui.tableWidget->item(4, 20)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Washer system valve 2 (radar)") : QString("");
				}
			}
			else if (msg[6] == 17)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(4, 21)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Washer system valve 3 (thermal imager)") : QString("");
				}
				else
				{
					ui.tableWidget->item(4, 21)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Washer system valve 3 (thermal imager)") : QString("");
				}
			}
			else if (msg[6] == 18)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(4, 22)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Washer system valve 4 (headlights)") : QString("");
				}
				else
				{
					ui.tableWidget->item(4, 22)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Washer system valve 4 (headlights)") : QString("");
				}
			}
			else if (msg[6] == 19)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(4, 23)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Headlights and ADAS washer pump") : QString("");
				}
				else
				{
					ui.tableWidget->item(4, 23)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Headlights and ADAS washer pump") : QString("");
				}
			}
			else if (msg[6] == 20)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(6, 17)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Rear Courtesy Light (MPV)") : QString("");
				}
				else
				{
					ui.tableWidget->item(6, 17)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Rear Courtesy Light (MPV)") : QString("");
				}
			}
			else if (msg[6] == 21)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(6, 18)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Ultraviolet ceiling plafon Light") : QString("");
				}
				else
				{
					ui.tableWidget->item(6, 18)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Ultraviolet ceiling plafon Light") : QString("");
				}
			}
			else if (msg[6] == 22)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(6, 19)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Underhood box illumination light (BEV)") : QString("");
				}
				else
				{
					ui.tableWidget->item(6, 19)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Underhood box illumination light (BEV)") : QString("");
				}
			}
			else if (msg[6] == 23)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(6, 20)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Glove box light") : QString("");
				}
				else
				{
					ui.tableWidget->item(6, 20)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Glove box light") : QString("");
				}
			}
			else if (msg[6] == 24)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(9, 17)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. HV lid unlock motor") : QString("");
				}
				else
				{
					ui.tableWidget->item(9, 17)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. HV lid unlock motor") : QString("");
				}
			}
			else if (msg[6] == 25)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(9, 18)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Vibration motor (CDA)") : QString("");
				}
				else
				{
					ui.tableWidget->item(9, 18)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Vibration motor (CDA)") : QString("");
				}
			}
			else if (msg[6] == 26)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(9, 19)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Horn Ctrl") : QString("");
				}
				else
				{
					ui.tableWidget->item(9, 19)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Horn Ctrl") : QString("");
				}
			}
			else if (msg[6] == 27)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(10, 17)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. RL passenger footrest motor (up)") : QString("");
				}
				else
				{
					ui.tableWidget->item(10, 17)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. RL passenger footrest motor (up)") : QString("");
				}
			}
			else if (msg[6] == 28)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(10, 18)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. RL passenger footrest motor (down)") : QString("");
				}
				else
				{
					ui.tableWidget->item(10, 18)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. RL passenger footrest motor (down)") : QString("");
				}
			}
			else if (msg[6] == 29)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(10, 19)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. RR passenger footrest motor (down)") : QString("");
				}
				else
				{
					ui.tableWidget->item(10, 19)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. RR passenger footrest motor (down)") : QString("");
				}
			}
			else if (msg[6] == 30)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(10, 20)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. RR passenger footrest motor(up)") : QString("");
				}
				else
				{
					ui.tableWidget->item(10, 20)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. RR passenger footrest motor(up)") : QString("");
				}
			}
			else if (msg[6] == 31)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(11, 17)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Spare output (half-bridge 1)") : QString("");
				}
				else
				{
					ui.tableWidget->item(11, 17)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Spare output (half-bridge 1)") : QString("");
				}
			}
			else if (msg[6] == 32)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(11, 18)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Spare output (half-bridge 2)") : QString("");
				}
				else
				{
					ui.tableWidget->item(11, 18)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Spare output (half-bridge 2)") : QString("");
				}
			}
			else if (msg[6] == 33)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(11, 19)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Spare output (half-bridge 3)") : QString("");
				}
				else
				{
					ui.tableWidget->item(11, 19)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Spare output (half-bridge 3)") : QString("");
				}
			}
			else if (msg[6] == 34)
			{
				if (msg[7] == 0)
				{
					ui.tableWidget->item(11, 20)->setBackground(QColor(Qt::transparent));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Выкл. Spare output (half-bridge 4)") : QString("");
				}
				else
				{
					ui.tableWidget->item(11, 20)->setBackground(QColor(COLOR_LIGHT_GREEN));
					return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Вкл. Spare output (half-bridge 4)") : QString("");
				}
			}
		}
	}
	QString tmpStr = "Нераспознаное сообщение. С CAN пришло: id: 0xAA msg: 0x" + QString::number(msg[0], 16) + " 0x" + QString::number(msg[1], 16) + " 0x" + QString::number(msg[2], 16) + " 0x" + QString::number(msg[3], 16) + " 0x" + QString::number(msg[4], 16) + " 0x" + QString::number(msg[5], 16) + " 0x" + QString::number(msg[6], 16) + " 0x" + QString::number(msg[7], 16);
	return tmpStr;
}

void VerificationTest::setTextTable(int row, int column, QString textRus, QString textEng)
{
	ui.tableWidget->setItem(row, column, new QTableWidgetItem(viewWindowState->appLanguage == RUSSIAN_LANG ? textRus : textEng));
	ui.tableWidget->item(row, column)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
}

bool VerificationTest::configProgressBar()
{
	int tmpValue = 0;
	if (ui.checkBox->isChecked())
		tmpValue += 3600;
	if (ui.checkBox_2->isChecked())
		tmpValue += 3600;
	if (ui.checkBox_3->isChecked())
		tmpValue += 3600;
	if (ui.checkBox_4->isChecked())
		tmpValue += 3600;
	if (ui.checkBox_5->isChecked())
		tmpValue += 3600;
	if (ui.checkBox_6->isChecked())
		tmpValue += 3600;
	if (ui.checkBox_7->isChecked())
		tmpValue += 3600;
	if (ui.checkBox_8->isChecked())
		tmpValue += 3600;
	if (ui.checkBox_9->isChecked())
		tmpValue += 3600;
	if (ui.checkBox_10->isChecked())
		tmpValue += 3600;
	if (tmpValue == 0)
	{
		QMessageBox::warning(this, QString("Внимание"), viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Хотябы один тест должен быть выбран") : QString("The block is not connected"));
		return false;
	}
	ui.progressBar->setMaximum(tmpValue);
	return true;
}

void VerificationTest::resizeEvent(QResizeEvent* event)
{
	if (viewWindowState->selectedBlock == TestBlockName::DTM)
	{

	}
	else
	{

	}
}

void VerificationTest::slot_StartStopButton_clicked() // Кнопка старт/стоп тест
{
	if (isTestRun) 
	{
		// Заканчиваем тест
		TimerTimeTest->stop();

		ui.StartStopButton->setText(viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Старт") : QString("Start"));
		Can::verificationStartStop();
	}
	else
	{
		if (!isStendConnection)
		{
			QMessageBox::warning(this, QString("Внимание"), viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Блок не подключкен") : QString("The block is not connected"));
			return;
		}
		if (!configProgressBar())
			return;


		// Начинаем тест
		timeTest = QTime(0,0,0);
		setTimeTest(timeTest);

		ui.StartStopButton->setText(viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Стоп") : QString("Stop"));
		ui.progressBar->setValue(0);
		ui.listWidget->clear();

		TimerTimeTest->start(TIME_SECOND);
		Can::verificationStartStop(ui.checkBox->isChecked(), ui.checkBox_2->isChecked(), ui.checkBox_3->isChecked(), ui.checkBox_4->isChecked(), ui.checkBox_5->isChecked(), ui.checkBox_6->isChecked(), ui.checkBox_7->isChecked(), ui.checkBox_8->isChecked(), ui.checkBox_9->isChecked(), ui.checkBox_10->isChecked());
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

void VerificationTest::slot_CheckBox()
{
	int tmpValue = 0;
	if (ui.checkBox->isChecked())
		tmpValue += 1;
	if (ui.checkBox_2->isChecked())
		tmpValue += 1;
	if (ui.checkBox_3->isChecked())
		tmpValue += 1;
	if (ui.checkBox_4->isChecked())
		tmpValue += 1;
	if (ui.checkBox_5->isChecked())
		tmpValue += 1;
	if (ui.checkBox_6->isChecked())
		tmpValue += 1;
	if (ui.checkBox_7->isChecked())
		tmpValue += 1;
	if (ui.checkBox_8->isChecked())
		tmpValue += 1;
	if (ui.checkBox_9->isChecked())
		tmpValue += 1;
	if (ui.checkBox_10->isChecked())
		tmpValue += 1;

	ui.EndTimeLable->setText(QString::number(tmpValue) + ":00:00");
}

void VerificationTest::Slot_ReciveMsg(int msg[8])
{
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
