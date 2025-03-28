#include "can.h"
#include "Cable.h"

Can::modelAdapter *Can::kvaser = new modelAdapter;
Can::modelAdapter *Can::marathon = new modelAdapter;
canHandle Can::hnd = 0;
std::vector<std::pair<Cable, int>> Can::Cables;
QMap<int, std::vector<std::pair<Cable, int>>> mapCable;
bool Can::b_flagStatusConnection;

Can::Can()
{
	b_adapterSelected = false;
	b_frequencySelected = false;
	b_flagStatusConnection = false;

	counterConnectMsg = 0;
	windowType = WindowType();

	timerReadCan = new QTimer();
	connect(timerReadCan, SIGNAL(timeout()), this, SLOT(Timer_ReadCan()));
	timerCheckStandConnection = new QTimer();
	connect(timerCheckStandConnection, SIGNAL(timeout()), this, SLOT(Timer_CheckStandConnection()));
	timerSendConnectMsg = new QTimer();
	connect(timerSendConnectMsg, SIGNAL(timeout()), this, SLOT(Timer_SendConnectMsg()));
}

// ------------------------------------
// Name: initCan
// Variables: 
//			WindowType windowType - enum �������� � ���� ������������� ����������� ����� ���� ������ �������.
// Return: bool
//			false - � ������ ���� b_adapterSelected == false, ��� ������ ��������� ��������.	
//			true  - � ������ ���� can ������ �������������.
// ------------------------------------
bool Can::initCan(WindowType windowType)
{
	if (!b_adapterSelected)
		return false;
	
	this->windowType = windowType;

	if (kvaser->activeAdapter != NOT_SET) // kvaser
	{
		// �������� �������� ������ kvasera
		canInitializeLibrary(); // ������������� api kvaser
		hnd = canOpenChannel(kvaser->activeAdapter, canOPEN_ACCEPT_VIRTUAL); // �������� ������ ����� �� CAN.
		canSetBusParams(hnd, kvaser->p_frequency.first, 0, 0, 0, 0, 0); // ��������� ���������� �� CAN-����.
		canBusOn(hnd); // ������ CAN-����
	}
	else
	{ // �������� �������� ������ Marathon
		auto statusTmp = CiInit();
		statusTmp = CiOpen( marathon->activeAdapter, CIO_CAN11);
		statusTmp = CiSetBaud(marathon->activeAdapter, marathon->p_frequency.first, marathon->p_frequency.second);
		statusTmp = CiSetFilter(marathon->activeAdapter,0,0);
		statusTmp = CiRcQueResize(marathon->activeAdapter, 0xFA);
		UINT16 threshold = 0xFF;
		statusTmp = CiTrQueThreshold(marathon->activeAdapter, CI_CMD_SET, &threshold);
		statusTmp = CiRcQueThreshold(marathon->activeAdapter, CI_CMD_SET, &threshold);
		statusTmp = CiStart(marathon->activeAdapter);
	}

	timerReadCan->start(2); // � �������� ������]
	switch (windowType)
	{
	case WindowType::MAINWINDOW:
	case WindowType::CONFIGURATOR:
	case WindowType::IN_TEST_MANUAL_STAND:
	case WindowType::OUT_TEST_MANUAL_STAND:
	case WindowType::FULL_TEST_MANUAL_STAND:
		timerCheckStandConnection->start(TIME_CHECKCONNECTION + 100); // � �������� ������
		break;

	case WindowType::IN_MANUAL_TEST_AUTO_STAND:
	case WindowType::OUT_MANUAL_TEST_AUTO_STAND:
	case WindowType::IN_AUTO_TEST_AUTO_STAND:
	case WindowType::OUT_AUTO_TEST_AUTO_STAND:
	case WindowType::FULL_TEST_AUTO_STAND:
		timerSendConnectMsg->start(100); // � �������� ������]
		timerCheckStandConnection->start(TIME_CHECKCONNECTION); // � �������� ������
		break;

	default:
		break;
	}


	counterConnectMsg = 0;

	measureds.clear();

	return true;
}

// ------------------------------------
// Name: deinitCan
// Return: bool
//			false - � ������ ���� b_adapterSelected == false, ��� ������ ��������� ��������.	
//			true  - � ���	��� ���� can ������ ���������������.
// ------------------------------------
bool Can::deinitCan()
{
	timerReadCan->stop();
	timerSendConnectMsg->stop();
	timerCheckStandConnection->stop();

	b_flagStatusConnection = false;

	measureds.clear();
	
	if (!b_adapterSelected)
		return false;
	if (kvaser->activeAdapter != NOT_SET) // kvaser 
	{
		canBusOff(hnd);  // �������� �������� ������ kvasera
		canClose(hnd);
	}
	else
	{
		CiStop(marathon->activeAdapter);  // �������� �������� ������ Marathon
		CiClose(marathon->activeAdapter);
	}
	return true;
}

// ------------------------------------
// Name: writeCan
// Variables:
//			int* id - ��������� �� ���������� � ������� ��������� id �� �������� ����������� can-���������.
//			int* msg - ��������� �� ���������� � ������� ����������� � can.
// Return: bool
//			false - � ������ ���� b_adapterSelected == false, ��� ������ ��������� ��������.	
//			true  - � ������ ���� can-��������� �����������.
// ------------------------------------
bool Can::writeCan(int id, int* msg)
{
	if (kvaser->activeAdapter != NOT_SET) // kvaser
	{
		unsigned char msgSendKvase[8];
		for (int i = 0; i < 8; i++)
			msgSendKvase[i] = msg[i];

		canWrite(hnd, id, msgSendKvase, 8, 0); // �������� �������� ������ kvasera
		return true;
	}
	else
	{
		static canmsg_t msgTransmit;
		msgTransmit.id = id;
		msgTransmit.len = 8;
		for (int i = 0; i < 8; i++)
			msgTransmit.data[i] = msg[i];
		
		auto statusTmp = CiTransmit(marathon->activeAdapter, &msgTransmit); // �������� �������� ������ Marathon
		return true;
	}
	return false;
}

// ------------------------------------
// Name: readWaitCan
// Variables:
//			int* id - ��������� �� ���������� � ������� ���������� id ���������� can-���������.
//			int* msg - ��������� �� ���������� � ������� ���������� ��������� ��������� �� can.
//			int  timeout - ����� � ������������� ������� �� ����� ����� ��������� �� can-����.
// Return: bool
//			false - � ������ ���� �� ����� timeout �� ������ ��������� �� can-����.
//			true  - � ������ ���� �� ����� timeout ������ ��������� �� can-����.
// ------------------------------------
canmsg_t msgReceive;
bool Can::readWaitCan(int* id, int* msg, int timeout)
{
	//Can::coun++;
	if (kvaser->activeAdapter != NOT_SET) // kvaser
	{
		unsigned int* dlc = new unsigned int(), *flags = new unsigned int();
		unsigned long* timestamp = new unsigned long();
		unsigned char msgReceive[8] = { 0, };

		*id = NOT_SET;
		canReadWait(hnd, (long*)id, msgReceive, dlc, flags, timestamp, timeout);
		if (*id != NOT_SET)
		{
			for (int i = 0; i < 8; i++)
				msg[i] = msgReceive[i];
		}

		delete dlc;
		delete flags;
		delete timestamp;

		if (*id != NOT_SET)
			return true;
		else
			return false;
	}
	else
	{
		msgReceive.id = 0;
		auto statusTmp = CiRead(marathon->activeAdapter, &msgReceive, 1);
		if (msgReceive.id != 0)
		{
			*id = msgReceive.id;
			for (int i = 0; i < 8; i++)
				msg[i] = msgReceive.data[i];
			return true;
		}
		else
			return false;
	}
}

// ------------------------------------
// Name: setSelectedAdapterNeme
// Variables: 
//			QString adapter - ��� �������� ������� ����� ������ � �������� ��������.
//			�������� ����������� ������ �������������� ������ ���� �������� ����������� �� ������ getNameAdapters().
// ------------------------------------
void Can::setSelectedAdapterNeme(QString adapter)
{
	kvaser->activeAdapter = NOT_SET;
	marathon->activeAdapter = NOT_SET;

	if (adapter == "..." || adapter == "")
	{
		b_adapterSelected = false;
		return;
	}

	for (int i = 0; i < kvaser->nameAdapters.size(); i++) // �������� �� kvaser
		if (kvaser->nameAdapters[i] == adapter)
		{
			kvaser->activeAdapter = i;
			b_adapterSelected = true;
			return;
		}

	for (int i = 0; i < marathon->nameAdapters.size(); i++) // �������� �� marathon
		if (marathon->nameAdapters[i] == adapter)
		{
			marathon->activeAdapter = i;
			b_adapterSelected = true;
			return;
		}

	b_adapterSelected = false;
}

// ------------------------------------
// Name: setSelectedFrequency
//		��������� ��������� ������� ��� ������ can
// Variables: 
//			Qstring frequency: ���������� �������.
// ------------------------------------
void Can::setSelectedFrequency(QString frequency)
{
	if (frequency == "...")
	{
		kvaser->p_frequency = conversionFrequency(0, KVASER);
		marathon->p_frequency = conversionFrequency(0, MARATHON);

		b_frequencySelected = false;
	}
	else
	{
		frequency.remove(-4, 5);																// �������� ����� ������
		kvaser->p_frequency = conversionFrequency(frequency.remove(' ').toInt(), KVASER);		// ������� ������� � ������, ��������� ������ � int, ���������� �������
		marathon->p_frequency = conversionFrequency(frequency.remove(' ').toInt(), MARATHON);

		b_frequencySelected = true;
	}
}

// ------------------------------------
// Name: getNameAdapters
// Return: std::vector<QString>
//			������ �������� Can-��������� ������� ���������� � �� � �� ����� � ���� �������.
//			����������:
//				� Kvaser ���� ����� � ������� �� ����������. Kvaser ������� ��������� �� ��. ��������� ���������. 
//				���������� �������. �� ���� �� ������ �������� (��� ��� ������ marathon), �� ��� ��� �� ����������.
// ------------------------------------
std::vector<QString> Can::getNameAdapters()
{
	// ������ ������ � ����������� ���������, ��� ���� ��� �� ��������������� ������������ ��������
	kvaser->nameAdapters.clear();
	marathon->nameAdapters.clear();	

	std::vector<QString> resultVector;

	// kvaser
	int chanCount;					// ���-�� ��������� kvaser
	char charNameAdapter[255];		// �������� ��������

	canInitializeLibrary();
	canGetNumberOfChannels(&chanCount);
	QString strNameAdapter;
	for (int i = 0; i < chanCount; i++) {
		canGetChannelData(i, canCHANNELDATA_CHANNEL_NAME, charNameAdapter, sizeof(charNameAdapter));
		if (charNameAdapter[0] == 0)
			continue;
		strNameAdapter = QString::fromStdString(charNameAdapter);
		QString str;
		bool isHaveTransfer = false;
		for (int j = 0; j < strNameAdapter.size(); j++)
		{
			if (j > 10 && strNameAdapter[j] == ' ' && !isHaveTransfer)
			{
				str += '\n';
				isHaveTransfer = true;
				continue;
			}
			str += strNameAdapter[j];
		}
		kvaser->nameAdapters.push_back(str);
		resultVector.push_back(str);
	}
	canBusOff(hnd);
	canClose(hnd);

	//marathon

	CiInit();
	canboard_t binfo;
	for (int j = 0; j < 5; j++)
	{
		binfo.brdnum = j;
		if (CiBoardInfo(&binfo) >= 0)
		{
			strNameAdapter = "Marathon\n";
			for (int i = 0; i < 4; i++)
			{
				if (binfo.chip[i] == NOT_SET)
					continue;
				strNameAdapter += QString::fromStdString(binfo.name);
				strNameAdapter += " " + QString::number(binfo.chip[i]);
				marathon->nameAdapters.push_back(strNameAdapter);
				resultVector.push_back(strNameAdapter);
				strNameAdapter = "Marathon\n";

			}
		}
	}

	return resultVector;
}

std::pair<int, int> Can::conversionFrequency(int frequency, int modelAdapter)
{
	std::pair<int, int> resultPair;
	resultPair.first = 0;
	resultPair.second = 0;

	switch (frequency)
	{
	case FREQUENCY_50K:
		if (modelAdapter == KVASER)
			resultPair.first = BAUD_50K;
		else
		{
			resultPair.first = BCI_50K_bt0;
			resultPair.second = BCI_50K_bt1;
		}
		break;

	case FREQUENCY_100K:
		if (modelAdapter == KVASER)
			resultPair.first = BAUD_100K;
		else
		{
			resultPair.first = BCI_100K_bt0;
			resultPair.second = BCI_100K_bt1;
		}
		break;

	case FREQUENCY_125K:
		if (modelAdapter == KVASER)
			resultPair.first = BAUD_125K;
		else
		{
			resultPair.first = BCI_125K_bt0;
			resultPair.second = BCI_125K_bt1;
		}
		break;

	case FREQUENCY_250K:
		if (modelAdapter == KVASER)
			resultPair.first = BAUD_250K;
		else
		{
			resultPair.first = BCI_250K_bt0;
			resultPair.second = BCI_250K_bt1;
		}
		break;

	case FREQUENCY_500K:
		if (modelAdapter == KVASER)
			resultPair.first = BAUD_500K;
		else
		{
			resultPair.first = BCI_500K_bt0;
			resultPair.second = BCI_500K_bt1;
		}
		break;

	case FREQUENCY_1000K:
		if (modelAdapter == KVASER)
			resultPair.first = BAUD_1M;
		else
		{
			resultPair.first = BCI_1M_bt0;
			resultPair.second = BCI_1M_bt1;
		}
		break;

	default:
		break;
	}
	return resultPair;
}

Measureds* getMeasureds(int* msg)
{
	Measureds* measured = nullptr;
	int tmp = msg[2] >> 5;
	TypeCable type = (TypeCable)(msg[2] >> 5);
	double voltage;
	double current;

	switch (type)
	{
	case TypeCable::DIG_IN:
	case TypeCable::ANALOG_IN:
		measured = new Measureds(msg[6], msg[7]);
		break;

	case TypeCable::HALL_IN:
		//flags += 5;
		break;

	case TypeCable::DIG_OUT:
	case TypeCable::PWM_OUT:
		voltage = ((msg[4] << 8) + msg[5]) / 1000.0;
		current = ((msg[6] << 8) + msg[7]) / 1000.0;
		measured = new Measureds(voltage, current);
		break;

	case TypeCable::VNH_OUT:
		voltage = ((msg[4] << 8) + msg[5]) / 1000.0;
		current = ((msg[6] << 8) + msg[7]) / 1000.0;
		measured = new Measureds(voltage, current);
		break;

	case TypeCable::HLD_OUT:
		break;
	}

	return measured;
}

void Can::Timer_ReadCan()
{
	qDebug() << QTime::currentTime().toString("hh:mm:ss:z") << "Start";

	int id = NOT_SET;
	int msgReceive[8];
	bool isValueChange = false;
	if (Can::readWaitCan(&id, msgReceive, 1))
	{
		switch (windowType)
		{
		case WindowType::IN_TEST_MANUAL_STAND:
		case WindowType::OUT_TEST_MANUAL_STAND:
		case WindowType::FULL_TEST_MANUAL_STAND:
			if (id == ID_CAN_MANUALSTAND)// ��������� � �������
			{
				if(b_flagStatusConnection)
					timerCheckStandConnection->start(TIME_CHECKCONNECTION + 100);
				else
				{
					Signal_ChangedStatusStandConnect(true);
					b_flagStatusConnection = true;
					timerCheckStandConnection->start(TIME_CHECKCONNECTION + 100);

				}
			}
			for (int i = 0; i < mapCable[id].size(); i++)
				if (mapCable[id][i].second != msgReceive[mapCable[id][i].first.getBit()])
				{
					mapCable[id][i].second = msgReceive[mapCable[id][i].first.getBit()];
					Signal_ChangedByte(mapCable[id][i].first.getId(), mapCable[id][i].second);
				}

			break;

		case WindowType::IN_MANUAL_TEST_AUTO_STAND:
		case WindowType::OUT_MANUAL_TEST_AUTO_STAND:
		case WindowType::IN_AUTO_TEST_AUTO_STAND:
		case WindowType::OUT_AUTO_TEST_AUTO_STAND:
		case WindowType::FULL_TEST_AUTO_STAND:
			if (id == ID_CAN_AUTOSTAND && // ��������� � �������
				!b_flagStatusConnection &&
				msgReceive[0] == 0x0 &&
				msgReceive[1] == 0xAA &&
				msgReceive[2] == 0x0 &&
				msgReceive[3] == 0xAA &&
				msgReceive[4] == 0x0 &&
				msgReceive[5] == 0xAA &&
				msgReceive[6] == 0x0 &&
				msgReceive[7] == 0xFA)
			{
				Signal_ChangedStatusStandConnect(true);
				b_flagStatusConnection = true;
				timerCheckStandConnection->start(TIME_CHECKCONNECTION);
				counterConnectMsg = 0;


			}
			else if (id == ID_CAN_AUTOSTAND && // ������������� ��������� � �������
				b_flagStatusConnection &&
				msgReceive[0] == counterConnectMsg &&
				msgReceive[1] == 0xAA &&
				msgReceive[2] == 0x0 &&
				msgReceive[3] == 0xAA &&
				msgReceive[4] == 0x0 &&
				msgReceive[5] == 0xAA &&
				msgReceive[6] == 0x0 &&
				msgReceive[7] == 0xFA)
			{
				timerCheckStandConnection->start(TIME_CHECKCONNECTION);
				counterConnectMsg++;
#ifdef DEBUG_CAN
				qDebug() << QTime::currentTime().toString("hh:mm:ss:z") << "I received a message about a periodic connection";
#endif // DEBUG_CAN
			}
			else if (id == 2 ) // ��������� � ���������� �����
			{
				measureds.push_back(getMeasureds(msgReceive));
				if ((msgReceive[2] & 0x01) == 1) // ����� �����
				{
					if ((TypeCable)(msgReceive[2] >> 5) == TypeCable::ANALOG_IN) // �������
					{
						measureds.push_back(new Measureds(measureds[0]->current,-1));
						measureds[0]->current = NOT_SET;
					}
					Signal_AfterTest(msgReceive[0], msgReceive[1], measureds);
					measureds.clear();
				}
			}
			break;
		}
	}

#ifdef DEBUG_CAN
	qDebug() << QTime::currentTime().toString("hh:mm:ss:z") << "Send";
#endif // DEBUG_CAN
	qDebug() << QTime::currentTime().toString("hh:mm:ss:z") << "end";
}

void Can::Timer_SendConnectMsg()
{
	if (b_flagStatusConnection)
	{
		int msgSendConnect[8] = {counterConnectMsg, 0x0, 0xAA, 0x0, 0xAA, 0x0, 0xAA, 0xAF };
		writeCan(ID_CAN_AUTOSTAND, msgSendConnect);
		counterConnectMsg++;
	}
	else
	{
		int msgSendConnect[8] = { 0xAA, 0x0, 0xAA, 0x0, 0xAA, 0x0, 0xAA, 0xAF };
		writeCan(ID_CAN_AUTOSTAND, msgSendConnect);
	}
}

void Can::Timer_CheckStandConnection()
{
	timerCheckStandConnection->stop();
	Signal_ChangedStatusStandConnect(false);
	b_flagStatusConnection = false;
	
#ifdef DEBUG_CAN
	qDebug() << QTime::currentTime().toString("hh:mm:ss:z") << "Change";
#endif // DEBUG_CAN
}
uint8_t generateFlags(TypeCable typeCable, TestBlockName nameBlock)
{
	uint8_t flags = 0;
	// Block
	flags += 1;
	flags = flags << 3;
	// Type
	switch (typeCable)
	{
	case TypeCable::DIG_IN:
		flags += 0;
		break;

	case TypeCable::ANALOG_IN:
		flags += 1;
		break;

	case TypeCable::DIG_OUT:
		flags += 2;
		break;

	case TypeCable::PWM_OUT:
		flags += 3;
		break;

	case TypeCable::VNH_OUT:
		flags += 4;
		break;

	case TypeCable::HALL_IN:
		flags += 5;
		break;

	case TypeCable::HLD_OUT:
		flags += 6;
		break;
	}
	flags = flags << 4;

	return flags;
}

// ------------------------------------
// Name: sendTestMsg
// Variables: 
//			ConnectorId pad - enum ����������� � ���� ������������� ������� (A - 1; B - 2; C - 3; C - 4; ...).
//			int pin - ����� ���� � �������.
//			int type - b������������ ������������ ������ ���� ���.
// Return: bool
//			false - � ������ ���� type == NOT_SET, ��� ������ ��������� ��������.	
//			true  - � ������ ���� ��������� �����������.
// ------------------------------------
bool Can::sendTestMsg(ConnectorId pad, int pin, TypeCable typeCable, TestBlockName nameBlock)
{
	if ((int)typeCable == NOT_SET)
		return false;

	int msgSendConnect[8] = { (int)pad, pin, generateFlags(typeCable, nameBlock), 0, 0, 0, 0, 0 };
	
	writeCan(10, msgSendConnect); // �������� �������� ������ kvasera
	return true;
}

// ------------------------------------
// Name: sendTestMsg
//		�������� ��������� �� can
// Variables: 
//			ConnectorId pad: ��������� ������������� ������
//			int pin: ��� ������������� ������
//			int digValue: �������� �������� ������������� ������
//			int pwmValue: ��� �������� ������������� ������
// ------------------------------------
void Can::sendTestMsg(ConnectorId pad, int pin, int digValue, int pwmValue)
{
	int msgSendConnect[8] = { (int)pad, pin, digValue, pwmValue, 0, 0, 0, 0 };
#ifndef FOR_DEVELOPER
	if (viewWindowState->selectedBlock == TestBlockName::DTM)
	{
		if (pad == ConnectorId::D && pin == 7) // D7
		{
			if (digValue == 0) // zero
			{
				Can::writeCan(0x55, msgSendConnect);
				msgSendConnect[1] = 6;
			}
			else if (digValue == 1) // high
			{
				Can::writeCan(0x55, msgSendConnect);
				msgSendConnect[1] = 6;
				msgSendConnect[2] = 2;

			}
			else if (digValue == 2) // low
			{
				Can::writeCan(0x55, msgSendConnect);
				msgSendConnect[1] = 6;
				msgSendConnect[2] = 1;

			}
		}
		else if (pad == ConnectorId::D && pin == 6) // D6
		{
			if (digValue == 0) // zero
			{
				Can::writeCan(0x55, msgSendConnect);
				msgSendConnect[1] = 7;
			}
			else if (digValue == 1) // high
			{
				Can::writeCan(0x55, msgSendConnect);
				msgSendConnect[1] = 7;
				msgSendConnect[2] = 2;

			}
			else if (digValue == 2) // low
			{
				Can::writeCan(0x55, msgSendConnect);
				msgSendConnect[1] = 7;
				msgSendConnect[2] = 1;

			}
		}
		else if (pad == ConnectorId::D && pin == 5) // D5
		{
			if (digValue == 0) // zero
			{
				Can::writeCan(0x55, msgSendConnect);
				msgSendConnect[1] = 7;
			}
			else if (digValue == 1) // high
			{
				Can::writeCan(0x55, msgSendConnect);
				msgSendConnect[1] = 7;
				msgSendConnect[2] = 2;

			}
			else if (digValue == 2) // low
			{
				Can::writeCan(0x55, msgSendConnect);
				msgSendConnect[1] = 7;
				msgSendConnect[2] = 1;

			}
		}
		else if (pad == ConnectorId::C && pin == 11) // �11
		{
			if (digValue == 0) // zero
			{
				Can::writeCan(0x55, msgSendConnect);
				msgSendConnect[1] = 12;
			}
			else if (digValue == 1) // high
			{
				Can::writeCan(0x55, msgSendConnect);
				msgSendConnect[1] = 12;
				msgSendConnect[2] = 2;

			}
			else if (digValue == 2) // low
			{
				Can::writeCan(0x55, msgSendConnect);
				msgSendConnect[1] = 12;
				msgSendConnect[2] = 1;

			}
		}
		else if (pad == ConnectorId::C && pin == 12) // �12
		{
			if (digValue == 0) // zero
			{
				Can::writeCan(0x55, msgSendConnect);
				msgSendConnect[1] = 11;
			}
			else if (digValue == 1) // high
			{
				Can::writeCan(0x55, msgSendConnect);
				msgSendConnect[1] = 11;
				msgSendConnect[2] = 2;

			}
			else if (digValue == 2) // low
			{
				Can::writeCan(0x55, msgSendConnect);
				msgSendConnect[1] = 11;
				msgSendConnect[2] = 1;

			}
		}
		else if (pad == ConnectorId::D && pin == 8) // D8
		{
			if (digValue == 0) // zero
			{
				Can::writeCan(0x55, msgSendConnect);
				msgSendConnect[1] = 7;
			}
			else if (digValue == 1) // high
			{
				Can::writeCan(0x55, msgSendConnect);
				msgSendConnect[1] = 7;
				msgSendConnect[2] = 2;

			}
			else if (digValue == 2) // low
			{
				Can::writeCan(0x55, msgSendConnect);
				msgSendConnect[1] = 7;
				msgSendConnect[2] = 1;

			}
		}
	}
#endif // !FOR_DEVELOPER
	Can::writeCan(0x55, msgSendConnect);
}


void Can::setCable(std::vector<Cable> cable)
{
	for (int i = 0; i < cable.size(); i++)
	{
		std::vector<std::pair<Cable, int>> tmpVector(mapCable[cable[i].getCanId()]);
		std::pair< Cable, int> tmpPair;

		tmpPair.first = cable[i];
		tmpPair.second = NOT_SET;

		tmpVector.push_back(tmpPair);
		mapCable[cable[i].getCanId()] = tmpVector;
	}
}
void Can::clearOldValue()
{

	for (int j = 256; j < 266; j++)
		for (int i = 0; i < mapCable[j].size(); i++)
			mapCable[j][i].second = NOT_SET;
	b_flagStatusConnection = false;
}
