#include "TestWindow.h"

void TestWindow::generateCableRows(WindowType testType, std::vector<Cable> cables)
{
	for (int i = 0; i < cables.size(); i++)
	{
		cableRows.push_back(new TestTableRowProperties());

		cableRows[i]->connector = (char)(PRIMARY_CONNECTOR_SYMBOL + (int)cables[i].connector);
		cableRows[i]->pin = QString::number(cables[i].pin);
		cableRows[i]->name = cables[i].name;

		if (testType == WindowType::FULL_TEST_AUTO_STAND ||
			testType == WindowType::FULL_TEST_MANUAL_STAND)
			switch (cables[i].direction)
			{
			case DIRECTION_OUT:
				cableRows[i]->direction = "OUT";
				break;

			case DIRECTION_IN:
				cableRows[i]->direction = "IN";
				break;
			}
		else
			cableRows[i]->direction = "";

		if (testType == WindowType::IN_AUTO_TEST_AUTO_STAND ||
			testType == WindowType::IN_MANUAL_TEST_AUTO_STAND ||
			testType == WindowType::IN_TEST_MANUAL_STAND ||
			testType == WindowType::FULL_TEST_AUTO_STAND ||
			testType == WindowType::FULL_TEST_MANUAL_STAND)
			switch (cables[i].type)
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
		else
			cableRows[i]->direction = "";

		if (testType == WindowType::OUT_AUTO_TEST_AUTO_STAND ||
			testType == WindowType::OUT_MANUAL_TEST_AUTO_STAND ||
			testType == WindowType::OUT_TEST_MANUAL_STAND ||
			testType == WindowType::FULL_TEST_AUTO_STAND ||
			testType == WindowType::FULL_TEST_MANUAL_STAND)
			switch (cables[i].type)
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
		else if (testType != WindowType::FULL_TEST_AUTO_STAND || testType != WindowType::FULL_TEST_MANUAL_STAND)
			cableRows[i]->direction = "";

		cableRows[i]->generateInteractionButtons(cables[i].type);
	}
}

void TestTableRowProperties::generateInteractionButtons(int type)
{
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
		((PWMButtons*)buttons)->load0Button->setText("0");
		((PWMButtons*)buttons)->load0Button->setFixedWidth(FIXED_CHECK_BUTTON_WIDTH);
		((PWMButtons*)buttons)->load0Button->setFixedHeight(FIXED_CHECK_BUTTON_HEIGHT);

		((PWMButtons*)buttons)->load25Button = new QPushButton();
		((PWMButtons*)buttons)->load25Button->setObjectName("load25Button");
		((PWMButtons*)buttons)->load25Button->setText("25");
		((PWMButtons*)buttons)->load25Button->setFixedWidth(FIXED_CHECK_BUTTON_WIDTH);
		((PWMButtons*)buttons)->load25Button->setFixedHeight(FIXED_CHECK_BUTTON_HEIGHT);

		((PWMButtons*)buttons)->load50Button = new QPushButton();
		((PWMButtons*)buttons)->load50Button->setObjectName("load50Button");
		((PWMButtons*)buttons)->load50Button->setText("50");
		((PWMButtons*)buttons)->load50Button->setFixedWidth(FIXED_CHECK_BUTTON_WIDTH);
		((PWMButtons*)buttons)->load50Button->setFixedHeight(FIXED_CHECK_BUTTON_HEIGHT);

		((PWMButtons*)buttons)->load75Button = new QPushButton();
		((PWMButtons*)buttons)->load75Button->setObjectName("load75Button");
		((PWMButtons*)buttons)->load75Button->setText("75");
		((PWMButtons*)buttons)->load75Button->setFixedWidth(FIXED_CHECK_BUTTON_WIDTH);
		((PWMButtons*)buttons)->load75Button->setFixedHeight(FIXED_CHECK_BUTTON_HEIGHT);

		((PWMButtons*)buttons)->load100Button = new QPushButton();
		((PWMButtons*)buttons)->load100Button->setObjectName("load100Button");
		((PWMButtons*)buttons)->load100Button->setText("100");
		((PWMButtons*)buttons)->load100Button->setFixedWidth(FIXED_CHECK_WBUTTON_WIDTH);
		((PWMButtons*)buttons)->load100Button->setFixedHeight(FIXED_CHECK_BUTTON_HEIGHT);
		break;

	case TYPE_VNH:
		buttons = new VNHButtons();

		((VNHButtons*)buttons)->load0Button = new QPushButton();
		((VNHButtons*)buttons)->load0Button->setObjectName("load0Button");
		((VNHButtons*)buttons)->load0Button->setText("0");
		((VNHButtons*)buttons)->load0Button->setFixedWidth(FIXED_CHECK_BUTTON_WIDTH);
		((VNHButtons*)buttons)->load0Button->setFixedHeight(FIXED_CHECK_BUTTON_HEIGHT);

		((VNHButtons*)buttons)->load25Button = new QPushButton();
		((VNHButtons*)buttons)->load25Button->setObjectName("load25Button");
		((VNHButtons*)buttons)->load25Button->setText("25");
		((VNHButtons*)buttons)->load25Button->setFixedWidth(FIXED_CHECK_BUTTON_WIDTH);
		((VNHButtons*)buttons)->load25Button->setFixedHeight(FIXED_CHECK_BUTTON_HEIGHT);

		((VNHButtons*)buttons)->load50Button = new QPushButton();
		((VNHButtons*)buttons)->load50Button->setObjectName("load50Button");
		((VNHButtons*)buttons)->load50Button->setText("50");
		((VNHButtons*)buttons)->load50Button->setFixedWidth(FIXED_CHECK_BUTTON_WIDTH);
		((VNHButtons*)buttons)->load50Button->setFixedHeight(FIXED_CHECK_BUTTON_HEIGHT);

		((VNHButtons*)buttons)->load75Button = new QPushButton();
		((VNHButtons*)buttons)->load75Button->setObjectName("load75Button");
		((VNHButtons*)buttons)->load75Button->setText("75");
		((VNHButtons*)buttons)->load75Button->setFixedWidth(FIXED_CHECK_BUTTON_WIDTH);
		((VNHButtons*)buttons)->load75Button->setFixedHeight(FIXED_CHECK_BUTTON_HEIGHT);

		((VNHButtons*)buttons)->load100Button = new QPushButton();
		((VNHButtons*)buttons)->load100Button->setObjectName("load100Button");
		((VNHButtons*)buttons)->load100Button->setText("100");
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
}

void TestTableRowProperties::on_onButton_clicked()
{
	msgToTwoThreadStartTest(this->connector.toStdString()[0] - PRIMARY_CONNECTOR_SYMBOL, this->pin.toInt(), 1, -1);
}

void TestTableRowProperties::on_offButton_clicked()
{
	msgToTwoThreadStartTest(this->connector.toStdString()[0] - PRIMARY_CONNECTOR_SYMBOL, this->pin.toInt(), 0, -1);
}

void TestTableRowProperties::on_load0Button_clicked()
{
	msgToTwoThreadStartTest(this->connector.toStdString()[0] - PRIMARY_CONNECTOR_SYMBOL, this->pin.toInt(), -1, 0);
}

void TestTableRowProperties::on_load25Button_clicked()
{
	msgToTwoThreadStartTest(this->connector.toStdString()[0] - PRIMARY_CONNECTOR_SYMBOL, this->pin.toInt(), -1, 64);
}

void TestTableRowProperties::on_load50Button_clicked()
{
	msgToTwoThreadStartTest(this->connector.toStdString()[0] - PRIMARY_CONNECTOR_SYMBOL, this->pin.toInt(), -1, 128);
}

void TestTableRowProperties::on_load75Button_clicked()
{
	msgToTwoThreadStartTest(this->connector.toStdString()[0] - PRIMARY_CONNECTOR_SYMBOL, this->pin.toInt(), -1, 192);
}

void TestTableRowProperties::on_load100Button_clicked()
{
	msgToTwoThreadStartTest(this->connector.toStdString()[0] - PRIMARY_CONNECTOR_SYMBOL, this->pin.toInt(), -1, 255);
}