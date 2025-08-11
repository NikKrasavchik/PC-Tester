#include "TestWindow.h"

void TestWindow::generateCableRows(WindowType testType, std::vector<Cable> cables)
{
	for (int i = 0; i < cables.size(); i++)
	{
		cableRows.push_back(new TestTableRowProperties());
		// Test
		for (int j = 0; j < cables[i].getThresholds().size(); j++)
		{
			Measureds* tmpMeasured = new Measureds;
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
		cableRows[i]->stateHLD = BUTTON_NOT_SET;
		cableRows[i]->canId = cables[i].getCanId();
		cableRows[i]->bit = cables[i].getBit();

		switch (cables[i].getDirection())
		{
		case DIRECTION_OUT:
			cableRows[i]->direction = "OUT";
			if (cables[i].getType() == TYPE_DIGITAL)
				cableRows[i]->typeInt = TypeCable::DIG_OUT;
			else if (cables[i].getType() == TYPE_PWM)
				cableRows[i]->typeInt = TypeCable::PWM_OUT;
			else if (cables[i].getType() == TYPE_VNH)
				cableRows[i]->typeInt = TypeCable::VNH_OUT;
			else if (cables[i].getType() == TYPE_HLD)
				cableRows[i]->typeInt = TypeCable::HLD_OUT;
			break;

		case DIRECTION_IN:
			cableRows[i]->direction = "IN";
			if(cables[i].getType() == TYPE_DIGITAL)
				cableRows[i]->typeInt = TypeCable::DIG_IN;
			else if(cables[i].getType() == TYPE_ANALOG)
				cableRows[i]->typeInt = TypeCable::ANALOG_IN;
			else if (cables[i].getType() == TYPE_HALL)
			{
				cableRows[i]->typeInt = TypeCable::HALL_IN;
				hallLabels.push_back(std::pair<int, QLabel*> {-1, new QLabel()});
			}
			break;
		case NOT_SET:
			if (cables[i].getType() == TYPE_CAN)
				cableRows[i]->typeInt = TypeCable::CAN_OUT;
			else if (cables[i].getType() == TYPE_LIN)
				cableRows[i]->typeInt = TypeCable::LIN_OUT;
			break;
		}


		cableRows[i]->generateInteractionButtons(testType, this);
		connect((cableRows[i]), &TestTableRowProperties::selectCurrentCell, this, &TestWindow::selectCurrentCell);

		offsetMap[cableRows[i]->id] = i;
	}
}

TestTableRowProperties::TestTableRowProperties()
{
	id = -1;
	canId = -1;
	bit = -1;
	connectorStr = "";
	connectorInt = ConnectorId::EMPTY;
	pin = "";
	name = "";
	component = "";
	direction = "";
	typeInt = TypeCable::EMPTY;
	comment = "";
	manualChecked = false;
}

void TestTableRowProperties::generateInteractionButtons(WindowType testType, TestWindow *testwindow)
{
	switch (testType)
	{

	case WindowType::OUT_TEST_MANUAL_STAND:
	case WindowType::FULL_TEST_MANUAL_STAND:
		switch (typeInt)
		{
		case TypeCable::DIG_IN:
			break;
		case TypeCable::ANALOG_IN:
			break;
		case TypeCable::HALL_IN:
			break;
		case TypeCable::DIG_OUT:
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

			connect(((DigitalButtons*)buttons)->onButton, &QPushButton::clicked, this, &TestTableRowProperties::on_onButton_clicked);
			connect(((DigitalButtons*)buttons)->offButton, &QPushButton::clicked, this, &TestTableRowProperties::on_offButton_clicked);
			break;
		case TypeCable::PWM_OUT:
			buttons = new PWMButtons();

			((PWMButtons*)buttons)->load0Button = new QPushButton();
			((PWMButtons*)buttons)->load0Button->setObjectName("load0Button");
			((PWMButtons*)buttons)->load0Button->setText("0%");
			((PWMButtons*)buttons)->load0Button->setToolTip("0% PWM");
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

			connect(((PWMButtons*)buttons)->load0Button, &QPushButton::clicked, this, &TestTableRowProperties::on_load0Button_clicked);
			connect(((PWMButtons*)buttons)->load25Button, &QPushButton::clicked, this, &TestTableRowProperties::on_load25Button_clicked);
			connect(((PWMButtons*)buttons)->load50Button, &QPushButton::clicked, this, &TestTableRowProperties::on_load50Button_clicked);
			connect(((PWMButtons*)buttons)->load75Button, &QPushButton::clicked, this, &TestTableRowProperties::on_load75Button_clicked);
			connect(((PWMButtons*)buttons)->load100Button, &QPushButton::clicked, this, &TestTableRowProperties::on_load100Button_clicked);
			break;
		case TypeCable::VNH_OUT:
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

			connect(((VNHButtons*)buttons)->load0Button, &QPushButton::clicked, this, &TestTableRowProperties::on_load0Button_clicked);
			connect(((VNHButtons*)buttons)->load25Button, &QPushButton::clicked, this, &TestTableRowProperties::on_load25Button_clicked);
			connect(((VNHButtons*)buttons)->load50Button, &QPushButton::clicked, this, &TestTableRowProperties::on_load50Button_clicked);
			connect(((VNHButtons*)buttons)->load75Button, &QPushButton::clicked, this, &TestTableRowProperties::on_load75Button_clicked);
			connect(((VNHButtons*)buttons)->load100Button, &QPushButton::clicked, this, &TestTableRowProperties::on_load100Button_clicked);
			connect(((VNHButtons*)buttons)->onButton, &QPushButton::clicked, this, &TestTableRowProperties::on_onButton_clicked);
			connect(((VNHButtons*)buttons)->offButton, &QPushButton::clicked, this, &TestTableRowProperties::on_offButton_clicked);
			break;
		case TypeCable::HLD_OUT:
			buttons = new HLDButtons();

			((HLDButtons*)buttons)->highButton = new QPushButton();
			((HLDButtons*)buttons)->highButton->setObjectName("highButton");
			((HLDButtons*)buttons)->highButton->setText("High");
			((HLDButtons*)buttons)->highButton->setFixedWidth(FIXED_CHECK_BUTTON_WIDTH);
			((HLDButtons*)buttons)->highButton->setFixedHeight(FIXED_CHECK_BUTTON_HEIGHT);

			((HLDButtons*)buttons)->lowButton = new QPushButton();
			((HLDButtons*)buttons)->lowButton->setObjectName("lowButton");
			((HLDButtons*)buttons)->lowButton->setText("Low");
			((HLDButtons*)buttons)->lowButton->setFixedWidth(FIXED_CHECK_BUTTON_WIDTH);
			((HLDButtons*)buttons)->lowButton->setFixedHeight(FIXED_CHECK_BUTTON_HEIGHT);

			((HLDButtons*)buttons)->zeroButton = new QPushButton();
			((HLDButtons*)buttons)->zeroButton->setObjectName("zeroButton");
			((HLDButtons*)buttons)->zeroButton->setText("Zero");
			((HLDButtons*)buttons)->zeroButton->setFixedWidth(FIXED_CHECK_BUTTON_WIDTH);
			((HLDButtons*)buttons)->zeroButton->setFixedHeight(FIXED_CHECK_BUTTON_HEIGHT);

			connect(((HLDButtons*)buttons)->highButton, &QPushButton::clicked, this, &TestTableRowProperties::on_high_clicked);
			connect(((HLDButtons*)buttons)->lowButton, &QPushButton::clicked, this, &TestTableRowProperties::on_low_clicked);
			connect(((HLDButtons*)buttons)->zeroButton, &QPushButton::clicked, this, &TestTableRowProperties::on_zero_clicked);
			break;
		case TypeCable::CAN_OUT:
		case TypeCable::LIN_OUT:
			buttons = new CheckInfomationBus();

			((CheckInfomationBus*)buttons)->checkButton = new QPushButton();
			((CheckInfomationBus*)buttons)->checkButton->setText("Check");
			((CheckInfomationBus*)buttons)->checkButton->setFixedWidth(FIXED_CHECK_WBUTTON_WIDTH);
			((CheckInfomationBus*)buttons)->checkButton->setFixedHeight(FIXED_CHECK_BUTTON_HEIGHT);

			((CheckInfomationBus*)buttons)->comboBox = new QComboBox();
			((CheckInfomationBus*)buttons)->comboBox->setStyleSheet(lightStyles.settingComboBox);
			((CheckInfomationBus*)buttons)->comboBox->setFixedWidth(FIXED_CHECK_WBUTTON_WIDTH);
			((CheckInfomationBus*)buttons)->comboBox->setFixedHeight(FIXED_CHECK_BUTTON_HEIGHT + 10);


			for(QString nameAdapter : Can::getNameAdapters())
				if (Can::getSelectedAdapterNeme() != nameAdapter)
					((CheckInfomationBus*)buttons)->comboBox->addItem(nameAdapter);


			connect(((CheckInfomationBus*)buttons)->checkButton, &QPushButton::clicked, this, &TestTableRowProperties::on_check_clicked);
			connect(this, &TestTableRowProperties::Signal_ChangedByte, testwindow, &TestWindow::Slot_ChangedByte);

			break;

		default:
			break;
		}
	case WindowType::IN_TEST_MANUAL_STAND:
		manualCheckBox = new QCheckBox();
		manualCheckBox->setStyleSheet(lightStyles.testwindowManualCheckBox);
		connect(manualCheckBox, &QCheckBox::clicked, this, &TestTableRowProperties::on_manualCheckBox_clicked);
		break;
	case WindowType::IN_MANUAL_TEST_AUTO_STAND:
	case WindowType::OUT_MANUAL_TEST_AUTO_STAND:
		buttons = new CheckButton();
		((CheckButton*)buttons)->checkButton = new QPushButton();
		((CheckButton*)buttons)->checkButton->setObjectName("checkButton");
		((CheckButton*)buttons)->checkButton->setFixedSize(FIXED_ACHECK_BUTTON_WIDTH, FIXED_ACHECK_BUTTON_HEIGHT);
		connect(((CheckButton*)buttons)->checkButton, &QPushButton::clicked, this, &TestTableRowProperties::on_checkButton_clicked);
	case WindowType::IN_AUTO_TEST_AUTO_STAND:
	case WindowType::OUT_AUTO_TEST_AUTO_STAND:
	case WindowType::FULL_TEST_AUTO_STAND:
		moreButton = new QPushButton();
		moreButton->setObjectName("moreButton");
		moreButton->setFixedSize(FIXED_MORE_BUTTON_SIZE, FIXED_MORE_BUTTON_SIZE);

		connect(moreButton, &QPushButton::clicked, this, &TestTableRowProperties::on_moreButton_clicked);

		break;

	}
}

void TestWindow::selectCurrentCell(int id)
{
	mainTableWidget->setCurrentCell(offsetMap[id], 2);
}

void TestTableRowProperties::switchButtonState(TestButtons testButton)
{
	Styles* currentStyles = &lightStyles;

	switch (typeInt)
	{
	case TypeCable::DIG_OUT:
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
		break;
	case TypeCable::PWM_OUT:
		((PWMButtons*)buttons)->load0Button->setStyleSheet(currentStyles->inactiveTableButton);
		((PWMButtons*)buttons)->load25Button->setStyleSheet(currentStyles->inactiveTableButton);
		((PWMButtons*)buttons)->load50Button->setStyleSheet(currentStyles->inactiveTableButton);
		((PWMButtons*)buttons)->load75Button->setStyleSheet(currentStyles->inactiveTableButton);
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
		break;
	case TypeCable::VNH_OUT:
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
		break;
	case TypeCable::HLD_OUT:
		((HLDButtons*)buttons)->highButton->setStyleSheet(currentStyles->inactiveTableButton);
		((HLDButtons*)buttons)->lowButton->setStyleSheet(currentStyles->inactiveTableButton);
		((HLDButtons*)buttons)->zeroButton->setStyleSheet(currentStyles->inactiveTableButton);

		((HLDButtons*)buttons)->highButton->setDisabled(false);
		((HLDButtons*)buttons)->lowButton->setDisabled(false);
		((HLDButtons*)buttons)->zeroButton->setDisabled(false);

		switch (testButton)
		{
		case TestButtons::BUTTON_HIGH:
			((HLDButtons*)buttons)->highButton->setStyleSheet(currentStyles->activeTableButton);
			((HLDButtons*)buttons)->highButton->setDisabled(true);
			break;

		case TestButtons::BUTTON_LOW:
			((HLDButtons*)buttons)->lowButton->setStyleSheet(currentStyles->activeTableButton);
			((HLDButtons*)buttons)->lowButton->setDisabled(true);
			break;

		case TestButtons::BUTTON_ZERO:
			((HLDButtons*)buttons)->zeroButton->setStyleSheet(currentStyles->activeTableButton);
			((HLDButtons*)buttons)->zeroButton->setDisabled(true);
			break;

		default:
			break;
		}
		break;

	default:
		break;
	}


}

void TestTableRowProperties::on_onButton_clicked()
{
	if (stateDigital == ON_BUTTON_PRESSED)
		return;


	switchButtonState(TestButtons::BUTTON_ON);
	stateDigital = ON_BUTTON_PRESSED;

	selectCurrentCell(id);
	sendSignal();
}

void TestTableRowProperties::on_offButton_clicked()
{
	if (stateDigital == OFF_BUTTON_PRESSED)
		return;


	switchButtonState(TestButtons::BUTTON_OFF);
	stateDigital = OFF_BUTTON_PRESSED;

	selectCurrentCell(id);
	sendSignal();
}

void TestTableRowProperties::on_load0Button_clicked()
{
	if (statePWM == LOAD0_BUTTON_PRESSED)
		return;

	selectCurrentCell(id);

	switchButtonState(TestButtons::BUTTON_LOAD_0);
		if (connectorInt == ConnectorId::A && pin == "15")
		{
			Can::sendTestMsg(this->connectorInt, this->pin.toInt(), 0, LOAD100_BUTTON_PRESSED);

			statePWM = LOAD0_BUTTON_PRESSED;
			return;
		}
	else
		statePWM = LOAD0_BUTTON_PRESSED;

	sendSignal();
}

void TestTableRowProperties::on_load25Button_clicked()
{
	if (statePWM == LOAD25_BUTTON_PRESSED)
		return;

	selectCurrentCell(id);

	switchButtonState(TestButtons::BUTTON_LOAD_25);
		if (connectorInt == ConnectorId::A && pin == "15")
		{
			Can::sendTestMsg(this->connectorInt, this->pin.toInt(), 0, LOAD75_BUTTON_PRESSED);

			statePWM = LOAD25_BUTTON_PRESSED;
			return;
		}
	else
		statePWM = LOAD25_BUTTON_PRESSED;

	sendSignal();
}

void TestTableRowProperties::on_load50Button_clicked()
{
	if (statePWM == LOAD50_BUTTON_PRESSED)
		return;

	selectCurrentCell(id);

	switchButtonState(TestButtons::BUTTON_LOAD_50);
	statePWM = LOAD50_BUTTON_PRESSED;

	sendSignal();
}

void TestTableRowProperties::on_high_clicked()
{
	if (stateHLD == HIGH_BUTTON_PRESSED)
		return;

	selectCurrentCell(id);

	switchButtonState(TestButtons::BUTTON_HIGH);
	stateHLD = HIGH_BUTTON_PRESSED;

	Can::sendTestMsg(this->connectorInt, this->pin.toInt(), 1, 0);

}

void TestTableRowProperties::on_low_clicked()
{
	if (stateHLD == LOW_BUTTON_PRESSED)
		return;

	selectCurrentCell(id);

	switchButtonState(TestButtons::BUTTON_LOW);
	stateHLD = LOW_BUTTON_PRESSED;

	Can::sendTestMsg(this->connectorInt, this->pin.toInt(), 2, 0);
}

void TestTableRowProperties::on_zero_clicked()
{
	if (stateHLD == ZERO_BUTTON_PRESSED)
		return;

	selectCurrentCell(id);

	switchButtonState(TestButtons::BUTTON_ZERO);
	stateHLD = ZERO_BUTTON_PRESSED;

	Can::sendTestMsg(this->connectorInt, this->pin.toInt(), 0, 0);
}

void TestTableRowProperties::on_load75Button_clicked()
{
	if (statePWM == LOAD75_BUTTON_PRESSED)
		return;

	selectCurrentCell(id);

	switchButtonState(TestButtons::BUTTON_LOAD_75);
		if (connectorInt == ConnectorId::A && pin == "15")
		{
			Can::sendTestMsg(this->connectorInt, this->pin.toInt(), 0, LOAD25_BUTTON_PRESSED);

			statePWM = LOAD75_BUTTON_PRESSED;
			return;
		}
	else
		statePWM = LOAD75_BUTTON_PRESSED;

	sendSignal();
}

void TestTableRowProperties::on_load100Button_clicked()
{
	if (statePWM == LOAD100_BUTTON_PRESSED)
		return;

	selectCurrentCell(id);

	switchButtonState(TestButtons::BUTTON_LOAD_100);
	if (connectorInt == ConnectorId::A && pin == "15")
	{
		Can::sendTestMsg(this->connectorInt, this->pin.toInt(), 0, LOAD0_BUTTON_PRESSED);

		statePWM = LOAD100_BUTTON_PRESSED;
		return;
	}
	else
		statePWM = LOAD100_BUTTON_PRESSED;

	sendSignal();
}
void TestTableRowProperties::on_check_clicked()
{
	Signal_ChangedByte(id, NOT_SET);
		Can::checkInformationBus(canId);

}

void TestTableRowProperties::on_manualCheckBox_clicked()
{
	selectCurrentCell(id);
}

void TestTableRowProperties::sendSignal()
{
	Can::sendTestMsg(this->connectorInt, this->pin.toInt(), stateDigital == NOT_SET ? 0 : stateDigital, statePWM == NOT_SET ? 0 : statePWM);
}

void TestTableRowProperties::generateWarning(Warnings::TestWindow warning)
{
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		switch (warning)
		{
		case Warnings::TestWindow::OPEN_MORE_WINDOW:
			break;
		}
		break;

	case ENGLISH_LANG:
		switch (warning)
		{
		case Warnings::TestWindow::OPEN_MORE_WINDOW:
			break;
		}
		break;
	}
}

void TestWindow::slot_mainTableWidget_cellClicked(int row, int column)
{
	if (column == mainTableWidget->columnCount() - 1)
		cableRows[row]->manualCheckBox->setChecked(!cableRows[row]->manualCheckBox->isChecked());
}

void TestWindow::on_rotateTimer_timeout()
{

	for (int i = 0; i < hallLabels.size(); i++)
	{
		if (hallLabels[i].first == -1)
			continue;

		hallLabels[i].second->setText("");
		hallLabels[i].first++;


		QPixmap pixmap = hallLabels[i].second->pixmap();
		pixmap.transformed(QTransform().rotate(1));
		hallLabels[i].second->setPixmap(pixmap);


		if (hallLabels[i].first == 360)
		{
			hallLabels[i].first = -1;
			switch (viewWindowState->appTheme)
			{
			case DARK_THEME:
				hallLabels[i].second->setPixmap(QPixmap(*noClockwiseDarkPixmap));
				break;

			case LIGHT_THEME:
				hallLabels[i].second->setPixmap(QPixmap(*noClockwiseLightPixmap));
				break;
			}
		}
	}
}

void TestWindow::on_delayStartTimer()
{
	for (int i = 0; i < cableRows.size(); i++)
		if (cableRows[i]->canId > 0x300)
		{
			Can::checkInformationBus(cableRows[i]->canId);
			Sleep(25);
		}
	const int costil = 0;

	if (costil == 0)
	{
		for (int i = 0; i < cableRows.size(); i++)
		{
			if (cableRows[i]->connectorInt == ConnectorId::A && cableRows[i]->pin == "15")
			{
				Can::sendTestMsg(cableRows[i]->connectorInt, cableRows[i]->pin.toInt(), 0, LOAD100_BUTTON_PRESSED);

				//((PWMButtons*)cableRows[i]->buttons)->load0Button->click();
				selectCurrentCell(0);
			}

		}
	}
	delayStartTimer->stop();
}
