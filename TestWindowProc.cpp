#include "TestWindow.h"

void TestWindow::generateCableRows(WindowType testType, std::vector<Cable> cables)
{
	for (int i = 0; i < cables.size(); i++)
	{
		cableRows.push_back(new TestTableRowProperties(this));
		// Test
		for (int j = 0; j < cables[i].getThresholds().size(); j++)
		{
			Measured* tmpMeasured = new Measured;
			cableRows[i]->measureds.push_back(tmpMeasured);
		}
		// Test
		cableRows[i]->id = cables[i].getId();
		cableRows[i]->connectorStr = (char)(PRIMARY_CONNECTOR_SYMBOL + (int)cables[i].getConnector());
		cableRows[i]->connectorInt = cables[i].getConnector();
		cableRows[i]->pin = QString::number(cables[i].getPin());
		cableRows[i]->name = cables[i].getName();
		cableRows[i]->component = cables[i].getComponent();
		for (int j = 0; j < cables[i].getThresholds().size(); j++)
			cableRows[i]->thresholds.push_back(cables[i].getThresholds()[j]);
		cableRows[i]->stateDigital = BUTTON_NOT_SET;
		cableRows[i]->statePWM = BUTTON_NOT_SET;
		cableRows[i]->canId = cables[i].getCanId();
		cableRows[i]->bit = cables[i].getBit();

		switch (cables[i].getDirection())
		{
		case DIRECTION_OUT:
			cableRows[i]->direction = "OUT";
			break;

		case DIRECTION_IN:
			cableRows[i]->direction = "IN";
			break;
		}

		if (testType == WindowType::IN_AUTO_TEST_AUTO_STAND ||
			testType == WindowType::IN_MANUAL_TEST_AUTO_STAND ||
			testType == WindowType::IN_TEST_MANUAL_STAND ||
			testType == WindowType::FULL_TEST_AUTO_STAND ||
			testType == WindowType::FULL_TEST_MANUAL_STAND)
			switch (cables[i].getType())
			{
			case TYPE_DIGITAL:
				cableRows[i]->type = "DIGITAL";
				break;

			case TYPE_ANALOG:
				cableRows[i]->type = "ANALOG";
				break;

			case TYPE_HALL:
				cableRows[i]->type = "HALL";
				break;

			default:
				break;
			}

		if (testType == WindowType::OUT_AUTO_TEST_AUTO_STAND ||
			testType == WindowType::OUT_MANUAL_TEST_AUTO_STAND ||
			testType == WindowType::OUT_TEST_MANUAL_STAND ||
			testType == WindowType::FULL_TEST_AUTO_STAND ||
			testType == WindowType::FULL_TEST_MANUAL_STAND)
		{
			switch (cables[i].getType())
			{
			case TYPE_DIGITAL:
				if (testType != WindowType::FULL_TEST_AUTO_STAND || testType != WindowType::FULL_TEST_MANUAL_STAND)
					cableRows[i]->type = "DIGITAL";
				break;

			case TYPE_PWM:
				cableRows[i]->type = "PWM";
				break;

			case TYPE_VNH:
				cableRows[i]->type = "VNH";
				break;

			default:
				break;
			}
		}
		else if (testType != WindowType::FULL_TEST_AUTO_STAND || testType != WindowType::FULL_TEST_MANUAL_STAND)
			cableRows[i]->direction = "";

		cableRows[i]->generateInteractionButtons(testType, cables[i].getType());
		connect((cableRows[i]), &TestTableRowProperties::selectCurrentCell, this, &TestWindow::selectCurrentCell);
	}
}

void TestTableRowProperties::generateInteractionButtons(WindowType testType, int type)
{
	if (testType == WindowType::OUT_TEST_MANUAL_STAND ||
		testType == WindowType::FULL_TEST_MANUAL_STAND)
		switch (type)
		{
		case TYPE_DIGITAL:
			if (direction == "OUT")
			{
				buttons = new DigitalButtons();

				((DigitalButtons*)buttons)->onButton = new QPushButton();
				((DigitalButtons*)buttons)->onButton->setObjectName("onButton");
				((DigitalButtons*)buttons)->onButton->setText("on");
				((DigitalButtons*)buttons)->onButton->setFixedWidth(FIXED_CHECK_BUTTON_WIDTH);
				((DigitalButtons*)buttons)->onButton->setFixedHeight(FIXED_CHECK_BUTTON_HEIGHT);

				((DigitalButtons*)buttons)->offButton = new QPushButton();
				((DigitalButtons*)buttons)->offButton->setObjectName("offButton");
				((DigitalButtons*)buttons)->offButton->setText("off");
				((DigitalButtons*)buttons)->offButton->setFixedWidth(FIXED_CHECK_BUTTON_WIDTH);
				((DigitalButtons*)buttons)->offButton->setFixedHeight(FIXED_CHECK_BUTTON_HEIGHT);
			}
			break;

		case TYPE_PWM:
			buttons = new PWMButtons();

			((PWMButtons*)buttons)->load0Button = new QPushButton();
			((PWMButtons*)buttons)->load0Button->setObjectName("load0Button");
			((PWMButtons*)buttons)->load0Button->setText("0%");
			((PWMButtons*)buttons)->load0Button->setFixedWidth(FIXED_CHECK_BUTTON_WIDTH);
			((PWMButtons*)buttons)->load0Button->setFixedHeight(FIXED_CHECK_BUTTON_HEIGHT);

			((PWMButtons*)buttons)->load25Button = new QPushButton();
			((PWMButtons*)buttons)->load25Button->setObjectName("load25Button");
			((PWMButtons*)buttons)->load25Button->setText("25%");
			((PWMButtons*)buttons)->load25Button->setFixedWidth(FIXED_CHECK_BUTTON_WIDTH);
			((PWMButtons*)buttons)->load25Button->setFixedHeight(FIXED_CHECK_BUTTON_HEIGHT);

			((PWMButtons*)buttons)->load50Button = new QPushButton();
			((PWMButtons*)buttons)->load50Button->setObjectName("load50Button");
			((PWMButtons*)buttons)->load50Button->setText("50%");
			((PWMButtons*)buttons)->load50Button->setFixedWidth(FIXED_CHECK_BUTTON_WIDTH);
			((PWMButtons*)buttons)->load50Button->setFixedHeight(FIXED_CHECK_BUTTON_HEIGHT);

			((PWMButtons*)buttons)->load75Button = new QPushButton();
			((PWMButtons*)buttons)->load75Button->setObjectName("load75Button");
			((PWMButtons*)buttons)->load75Button->setText("75%");
			((PWMButtons*)buttons)->load75Button->setFixedWidth(FIXED_CHECK_BUTTON_WIDTH);
			((PWMButtons*)buttons)->load75Button->setFixedHeight(FIXED_CHECK_BUTTON_HEIGHT);

			((PWMButtons*)buttons)->load100Button = new QPushButton();
			((PWMButtons*)buttons)->load100Button->setObjectName("load100Button");
			((PWMButtons*)buttons)->load100Button->setText("100%");
			((PWMButtons*)buttons)->load100Button->setFixedWidth(FIXED_CHECK_WBUTTON_WIDTH);
			((PWMButtons*)buttons)->load100Button->setFixedHeight(FIXED_CHECK_BUTTON_HEIGHT);
			break;

		case TYPE_VNH:
			buttons = new VNHButtons();

			((VNHButtons*)buttons)->load0Button = new QPushButton();
			((VNHButtons*)buttons)->load0Button->setObjectName("load0Button");
			((VNHButtons*)buttons)->load0Button->setText("0%");
			((VNHButtons*)buttons)->load0Button->setFixedWidth(FIXED_CHECK_BUTTON_WIDTH);
			((VNHButtons*)buttons)->load0Button->setFixedHeight(FIXED_CHECK_BUTTON_HEIGHT);

			((VNHButtons*)buttons)->load25Button = new QPushButton();
			((VNHButtons*)buttons)->load25Button->setObjectName("load25Button");
			((VNHButtons*)buttons)->load25Button->setText("25%");
			((VNHButtons*)buttons)->load25Button->setFixedWidth(FIXED_CHECK_BUTTON_WIDTH);
			((VNHButtons*)buttons)->load25Button->setFixedHeight(FIXED_CHECK_BUTTON_HEIGHT);

			((VNHButtons*)buttons)->load50Button = new QPushButton();
			((VNHButtons*)buttons)->load50Button->setObjectName("load50Button");
			((VNHButtons*)buttons)->load50Button->setText("50%");
			((VNHButtons*)buttons)->load50Button->setFixedWidth(FIXED_CHECK_BUTTON_WIDTH);
			((VNHButtons*)buttons)->load50Button->setFixedHeight(FIXED_CHECK_BUTTON_HEIGHT);

			((VNHButtons*)buttons)->load75Button = new QPushButton();
			((VNHButtons*)buttons)->load75Button->setObjectName("load75Button");
			((VNHButtons*)buttons)->load75Button->setText("75%");
			((VNHButtons*)buttons)->load75Button->setFixedWidth(FIXED_CHECK_BUTTON_WIDTH);
			((VNHButtons*)buttons)->load75Button->setFixedHeight(FIXED_CHECK_BUTTON_HEIGHT);

			((VNHButtons*)buttons)->load100Button = new QPushButton();
			((VNHButtons*)buttons)->load100Button->setObjectName("load100Button");
			((VNHButtons*)buttons)->load100Button->setText("100%");
			((VNHButtons*)buttons)->load100Button->setFixedWidth(FIXED_CHECK_WBUTTON_WIDTH);
			((VNHButtons*)buttons)->load100Button->setFixedHeight(FIXED_CHECK_BUTTON_HEIGHT);

			((VNHButtons*)buttons)->onButton = new QPushButton();
			((VNHButtons*)buttons)->onButton->setObjectName("onButton");
			((VNHButtons*)buttons)->onButton->setText("on");
			((VNHButtons*)buttons)->onButton->setFixedWidth(FIXED_CHECK_BUTTON_WIDTH);
			((VNHButtons*)buttons)->onButton->setFixedHeight(FIXED_CHECK_BUTTON_HEIGHT);

			((VNHButtons*)buttons)->offButton = new QPushButton();
			((VNHButtons*)buttons)->offButton->setObjectName("offButton");
			((VNHButtons*)buttons)->offButton->setText("off");
			((VNHButtons*)buttons)->offButton->setFixedWidth(FIXED_CHECK_BUTTON_WIDTH);
			((VNHButtons*)buttons)->offButton->setFixedHeight(FIXED_CHECK_BUTTON_HEIGHT);
			break;

		default:
			break;
		}
	else if (testType == WindowType::OUT_MANUAL_TEST_AUTO_STAND ||
			 testType == WindowType::IN_MANUAL_TEST_AUTO_STAND ||
			 testType == WindowType::OUT_AUTO_TEST_AUTO_STAND ||
			 testType == WindowType::FULL_TEST_AUTO_STAND)
	{
		buttons = new CheckButton();

		((CheckButton*)buttons)->checkButton = new QPushButton();
		((CheckButton*)buttons)->checkButton->setObjectName("checkButton");
		((CheckButton*)buttons)->checkButton->setFixedSize(FIXED_ACHECK_BUTTON_WIDTH, FIXED_ACHECK_BUTTON_HEIGHT);
		connect(((CheckButton*)buttons)->checkButton, &QPushButton::clicked, this, &TestTableRowProperties::on_checkButton_clicked);
	}

	moreButton = new QPushButton();
	moreButton->setObjectName("moreButton");
	moreButton->setFixedSize(FIXED_MORE_BUTTON_SIZE, FIXED_MORE_BUTTON_SIZE);
}

void TestWindow::selectCurrentCell(QString connector, QString pin)
{
	for (int i = 0; i < cableRows.size(); i++)
		if (cableRows[i]->connectorStr == connector && cableRows[i]->pin == pin)
			mainTableWidget->setCurrentCell(i, 0);
}

void TestTableRowProperties::switchButtonState(TestButtons testButton)
{
	Styles* currentStyles = nullptr;
	switch (viewWindowState->appTheme)
	{
	case (LIGHT_THEME):
		currentStyles = &lightStyles;
		break;

	case (DARK_THEME):
		currentStyles = &darkStyles;
		break;
	}

	if (type == "DIGITAL")
	{
		((DigitalButtons*)buttons)->onButton->setStyleSheet(currentStyles->inactiveTableButton);
		((DigitalButtons*)buttons)->offButton->setStyleSheet(currentStyles->inactiveTableButton);

		((DigitalButtons*)buttons)->onButton->setDisabled(false);
		((DigitalButtons*)buttons)->offButton->setDisabled(false);

		switch (testButton)
		{
		case TestButtons::BUTTON_ON:
			((DigitalButtons*)buttons)->onButton->setStyleSheet(currentStyles->activeTableButton);
			((DigitalButtons*)buttons)->onButton->setDisabled(true);
			break;

		case TestButtons::BUTTON_OFF:
			((DigitalButtons*)buttons)->offButton->setStyleSheet(currentStyles->activeTableButton);
			((DigitalButtons*)buttons)->offButton->setDisabled(true);
			break;

		default:
			break;
		}
	}
	else if (type == "PWM")
	{
		((PWMButtons*)buttons)->load0Button->setStyleSheet(currentStyles->inactiveTableButton);
		((PWMButtons*)buttons)->load25Button->setStyleSheet(currentStyles->inactiveTableButton);
		((PWMButtons*)buttons)->load50Button->setStyleSheet(currentStyles->inactiveTableButton);
		((PWMButtons*)buttons)->load75Button ->setStyleSheet(currentStyles->inactiveTableButton);
		((PWMButtons*)buttons)->load100Button->setStyleSheet(currentStyles->inactiveTableButton);

		((PWMButtons*)buttons)->load0Button->setDisabled(false);
		((PWMButtons*)buttons)->load25Button->setDisabled(false);
		((PWMButtons*)buttons)->load50Button->setDisabled(false);
		((PWMButtons*)buttons)->load75Button->setDisabled(false);
		((PWMButtons*)buttons)->load100Button->setDisabled(false);

		switch (testButton)
		{
		case TestButtons::BUTTON_LOAD_0:
			((PWMButtons*)buttons)->load0Button->setStyleSheet(currentStyles->activeTableButton);
			((PWMButtons*)buttons)->load0Button->setDisabled(true);
			break;

		case TestButtons::BUTTON_LOAD_25:
			((PWMButtons*)buttons)->load25Button->setStyleSheet(currentStyles->activeTableButton);
			((PWMButtons*)buttons)->load25Button->setDisabled(true);
			break;

		case TestButtons::BUTTON_LOAD_50:
			((PWMButtons*)buttons)->load50Button->setStyleSheet(currentStyles->activeTableButton);
			((PWMButtons*)buttons)->load50Button->setDisabled(true);
			break;

		case TestButtons::BUTTON_LOAD_75:
			((PWMButtons*)buttons)->load75Button->setStyleSheet(currentStyles->activeTableButton);
			((PWMButtons*)buttons)->load75Button->setDisabled(true);
			break;

		case TestButtons::BUTTON_LOAD_100:
			((PWMButtons*)buttons)->load100Button->setStyleSheet(currentStyles->activeTableButton);
			((PWMButtons*)buttons)->load100Button->setDisabled(true);
			break;

		default:
			break;
		}
	}
	else if (type == "VNH")
	{
		switch (testButton)
		{
		case TestButtons::BUTTON_ON:
			((VNHButtons*)buttons)->onButton->setStyleSheet(currentStyles->activeTableButton);
			((VNHButtons*)buttons)->onButton->setDisabled(true);

			((VNHButtons*)buttons)->offButton->setStyleSheet(currentStyles->inactiveTableButton);
			((VNHButtons*)buttons)->offButton->setDisabled(false);
			break;

		case TestButtons::BUTTON_OFF:
			((VNHButtons*)buttons)->offButton->setStyleSheet(currentStyles->activeTableButton);
			((VNHButtons*)buttons)->offButton->setDisabled(true);

			((VNHButtons*)buttons)->onButton->setStyleSheet(currentStyles->inactiveTableButton);
			((VNHButtons*)buttons)->onButton->setDisabled(false);
			break;

		case TestButtons::BUTTON_LOAD_0:
		case TestButtons::BUTTON_LOAD_25:
		case TestButtons::BUTTON_LOAD_50:
		case TestButtons::BUTTON_LOAD_75:
		case TestButtons::BUTTON_LOAD_100:
			((VNHButtons*)buttons)->load0Button->setStyleSheet(currentStyles->inactiveTableButton);
			((VNHButtons*)buttons)->load25Button->setStyleSheet(currentStyles->inactiveTableButton);
			((VNHButtons*)buttons)->load50Button->setStyleSheet(currentStyles->inactiveTableButton);
			((VNHButtons*)buttons)->load75Button->setStyleSheet(currentStyles->inactiveTableButton);
			((VNHButtons*)buttons)->load100Button->setStyleSheet(currentStyles->inactiveTableButton);

			((VNHButtons*)buttons)->load0Button->setDisabled(false);
			((VNHButtons*)buttons)->load25Button->setDisabled(false);
			((VNHButtons*)buttons)->load50Button->setDisabled(false);
			((VNHButtons*)buttons)->load75Button->setDisabled(false);
			((VNHButtons*)buttons)->load100Button->setDisabled(false);

			switch (testButton)
			{
			case TestButtons::BUTTON_LOAD_0:
				((VNHButtons*)buttons)->load0Button->setStyleSheet(currentStyles->activeTableButton);
				((VNHButtons*)buttons)->load0Button->setDisabled(true);
				break;

			case TestButtons::BUTTON_LOAD_25:
				((VNHButtons*)buttons)->load25Button->setStyleSheet(currentStyles->activeTableButton);
				((VNHButtons*)buttons)->load25Button->setDisabled(true);
				break;

			case TestButtons::BUTTON_LOAD_50:
				((VNHButtons*)buttons)->load50Button->setStyleSheet(currentStyles->activeTableButton);
				((VNHButtons*)buttons)->load50Button->setDisabled(true);
				break;

			case TestButtons::BUTTON_LOAD_75:
				((VNHButtons*)buttons)->load75Button->setStyleSheet(currentStyles->activeTableButton);
				((VNHButtons*)buttons)->load75Button->setDisabled(true);
				break;

			case TestButtons::BUTTON_LOAD_100:
				((VNHButtons*)buttons)->load100Button->setStyleSheet(currentStyles->activeTableButton);
				((VNHButtons*)buttons)->load100Button->setDisabled(true);
				break;

			default:
				break;
			}
			break;
		}
	}
}

void TestTableRowProperties::on_onButton_clicked()
{
	if (stateDigital == ON_BUTTON_PRESSED)
		return;

	selectCurrentCell(connectorStr, pin);

	switchButtonState(TestButtons::BUTTON_ON);
	stateDigital = ON_BUTTON_PRESSED;

	sendSignal();
}

void TestTableRowProperties::on_offButton_clicked()
{
	if (stateDigital == OFF_BUTTON_PRESSED)
		return;

	selectCurrentCell(connectorStr, pin);

	switchButtonState(TestButtons::BUTTON_OFF);
	stateDigital = OFF_BUTTON_PRESSED;

	sendSignal();
}

void TestTableRowProperties::on_load0Button_clicked()
{
	if (statePWM == LOAD0_BUTTON_PRESSED)
		return;

	selectCurrentCell(connectorStr, pin);

	switchButtonState(TestButtons::BUTTON_LOAD_0);
	statePWM = LOAD0_BUTTON_PRESSED;

	sendSignal();
}

void TestTableRowProperties::on_load25Button_clicked()
{
	if (statePWM == LOAD25_BUTTON_PRESSED)
		return;

	selectCurrentCell(connectorStr, pin);

	switchButtonState(TestButtons::BUTTON_LOAD_25);
	statePWM = LOAD25_BUTTON_PRESSED;

	sendSignal();
}

void TestTableRowProperties::on_load50Button_clicked()
{
	if (statePWM == LOAD50_BUTTON_PRESSED)
		return;

	selectCurrentCell(connectorStr, pin);

	switchButtonState(TestButtons::BUTTON_LOAD_50);
	statePWM = LOAD50_BUTTON_PRESSED;

	sendSignal();
}

void TestTableRowProperties::on_load75Button_clicked()
{
	if (statePWM == LOAD75_BUTTON_PRESSED)
		return;

	selectCurrentCell(connectorStr, pin);

	switchButtonState(TestButtons::BUTTON_LOAD_75);
	statePWM = LOAD75_BUTTON_PRESSED;

	sendSignal();
}

void TestTableRowProperties::on_load100Button_clicked()
{
	if (statePWM == LOAD100_BUTTON_PRESSED)
		return;

	selectCurrentCell(connectorStr, pin);

	switchButtonState(TestButtons::BUTTON_LOAD_100);
	statePWM = LOAD100_BUTTON_PRESSED;

	sendSignal();
}

void TestTableRowProperties::sendSignal()
{
	msgToTwoThreadStartTest_ManualTwoThread(this->connectorStr.toStdString()[0] - PRIMARY_CONNECTOR_SYMBOL, this->pin.toInt(), stateDigital == NOT_SET ? 0 : stateDigital, statePWM == NOT_SET ? 0 : statePWM);
}

void TestTableRowProperties::msgFromTwoThreadAfterTest_AutoTwothread(int pad, int pin, float voltage, float curent)
{
}