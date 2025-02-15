#include "can.h"


Can::modelAdapter *Can::kvaser = new modelAdapter;
Can::modelAdapter *Can::marathon = new modelAdapter;
canHandle Can::hnd = 0;


Can::Can()
{
	b_adapterSelected = false;
	b_frequencySelected = false;
	b_flagStandConnectionCheck = false;
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

bool Can::initCan(WindowType windowType)
{
	if (!b_adapterSelected)
		return false;
	
	this->windowType = windowType;

	if (kvaser->activeAdapter != NOT_SET) // kvaser
	{
		// Дописать проверку ошибок kvasera
		canInitializeLibrary(); // Инициализация api kvaser
		hnd = canOpenChannel(kvaser->activeAdapter, canOPEN_ACCEPT_VIRTUAL); // Открытие канала связи по CAN.
		canSetBusParams(hnd, kvaser->p_frequency.first, 0, 0, 0, 0, 0); // Установка параматров на CAN-шину.
		canBusOn(hnd); // Запуск CAN-шины
	}
	else
	{ // Дописать проверку ошибок Marathon
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

	timerReadCan->start(1); // И запустим таймер]
	timerSendConnectMsg->start(100); // И запустим таймер]
	timerCheckStandConnection->start(300); // И запустим таймер

	counterConnectMsg = 0;

	measureds.clear();

	return true;
}

bool Can::deinitCan()
{
	timerReadCan->stop();
	timerSendConnectMsg->stop();
	timerCheckStandConnection->stop();

	b_flagStandConnectionCheck = false;
	b_flagStatusConnection = false;

	measureds.clear();
	
	if (!b_adapterSelected)
		return false;
	if (kvaser->activeAdapter != NOT_SET) // kvaser 
	{
		canBusOff(hnd);  // Дописать проверку ошибок kvasera
		canClose(hnd);
	}
	else
	{
		CiStop(marathon->activeAdapter);  // Дописать проверку ошибок Marathon
		CiClose(marathon->activeAdapter);
	}
	return true;
}

bool Can::writeCan(int id, int* msg)
{
	
	if (kvaser->activeAdapter != NOT_SET) // kvaser
	{
		unsigned char msgSendKvase[8];
		for (int i = 0; i < 8; i++)
			msgSendKvase[i] = msg[i];

		canWrite(hnd, id, msgSendKvase, 8, 0); // Дописать проверку ошибок kvasera
		return true;
	}
	else
	{
		static canmsg_t msgTransmit;
		msgTransmit.id = id;
		msgTransmit.len = 8;
		for (int i = 0; i < 8; i++)
			msgTransmit.data[i] = msg[i];
		
		auto statusTmp = CiTransmit(marathon->activeAdapter, &msgTransmit); // Дописать проверку ошибок Marathon
		return true;
	}
	return false;
}



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

void Can::setSelectedAdapterNeme(QString adapter)
{
	kvaser->activeAdapter = NOT_SET;
	marathon->activeAdapter = NOT_SET;

	if (adapter == "..." || adapter == "")
	{
		b_adapterSelected = false;
		return;
	}

	for (int i = 0; i < kvaser->nameAdapters.size(); i++) // Проходим по kvaser
		if (kvaser->nameAdapters[i] == adapter)
		{
			kvaser->activeAdapter = i;
			b_adapterSelected = true;
			return;
		}
	for (int i = 0; i < marathon->nameAdapters.size(); i++) // Проходим по marathon
		if (marathon->nameAdapters[i] == adapter)
		{
			marathon->activeAdapter = i;
			b_adapterSelected = true;
			return;
		}
	b_adapterSelected = false;

}

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
		frequency.remove(-4, 5);																// Обрезаем конец строки
		kvaser->p_frequency = conversionFrequency(frequency.remove(' ').toInt(), KVASER);		// Удаляем пробелы в строке, переводим строку в int, записываем частоту
		marathon->p_frequency = conversionFrequency(frequency.remove(' ').toInt(), MARATHON);

		b_frequencySelected = true;
	}
}

std::vector<QString> Can::getNameAdapters()
{
	// Чистим массив с названияями адаптеров, для того что бы актуализировать подключенные адаптеры
	kvaser->nameAdapters.clear();
	marathon->nameAdapters.clear();

	std::vector<QString> resultVector;

	// kvaser
	int chanCount;             // Кол-во адаптеров kvaser
	char charNameAdapter[255];     // Название адаптера

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
	case 50000:
		if (modelAdapter == KVASER)
			resultPair.first = BAUD_50K;
		else
		{
			resultPair.first = BCI_50K_bt0;
			resultPair.second = BCI_50K_bt1;
		}
		break;
	case 100000:
		if (modelAdapter == KVASER)
			resultPair.first = BAUD_100K;
		else
		{
			resultPair.first = BCI_100K_bt0;
			resultPair.second = BCI_100K_bt1;
		}
		break;
	case 125000:
		if (modelAdapter == KVASER)
			resultPair.first = BAUD_125K;
		else
		{
			resultPair.first = BCI_125K_bt0;
			resultPair.second = BCI_125K_bt1;
		}
		break;
	case 250000:
		if (modelAdapter == KVASER)
			resultPair.first = BAUD_250K;
		else
		{
			resultPair.first = BCI_250K_bt0;
			resultPair.second = BCI_250K_bt1;
		}
		break;
	case 500000:
		if (modelAdapter == KVASER)
			resultPair.first = BAUD_500K;
		else
		{
			resultPair.first = BCI_500K_bt0;
			resultPair.second = BCI_500K_bt1;
		}
		break;
	case 1000000:
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
	}

	return measured;
}

void Can::Timer_ReadCan()
{
	int id = NOT_SET;
	int msgReceive[8];
	if (Can::readWaitCan(&id, msgReceive, 1))
	{
		switch (windowType)
		{
		case WindowType::IN_TEST_MANUAL_STAND:
		case WindowType::OUT_TEST_MANUAL_STAND:
		case WindowType::FULL_TEST_MANUAL_STAND:
			if (id == ID_CAN_MANUALSTAND)// сообшение о конекте
			{
				b_flagStandConnectionCheck = true;
				if (!b_flagStatusConnection)
				{

					Signal_ChangedStatusStandConnect(true);
					b_flagStatusConnection = true;
				}
			}
			break;
		case WindowType::IN_MANUAL_TEST_AUTO_STAND:
		case WindowType::OUT_MANUAL_TEST_AUTO_STAND:
		case WindowType::IN_AUTO_TEST_AUTO_STAND:
		case WindowType::OUT_AUTO_TEST_AUTO_STAND:
		case WindowType::FULL_TEST_AUTO_STAND:
			if (id == ID_CAN_AUTOSTAND && // сообшение о конекте
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
				b_flagStandConnectionCheck = true;
				counterConnectMsg = 0;
			}
			else if (id == ID_CAN_AUTOSTAND && // переодическое сообшение о конекте
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
				b_flagStandConnectionCheck = true;
				counterConnectMsg++;
			}
			else if (id == 2 ) // Сообщение о результате теста
			{
				
				measureds.push_back(getMeasureds(msgReceive));
				if ((msgReceive[2] & 0x01) == 1) // Конец теста
				{
					if ((TypeCable)(msgReceive[2] >> 5) == TypeCable::ANALOG_IN) // Костыль
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
	if (b_flagStatusConnection)
	{
		if(b_flagStandConnectionCheck)
			b_flagStandConnectionCheck = false;
		else
		{
			b_flagStandConnectionCheck = false;
			b_flagStatusConnection = false;
			Signal_ChangedStatusStandConnect(false);
		}
	}
	
#ifdef DEBUG_CAN
	qDebug() << QTime::currentTime().toString("hh:mm:ss:z") << "Change";
#endif // DEBUG_CAN
}

uint8_t generateFlags(TypeCable typeCable, NameTestingBlock nameBlock)
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
	}
	flags = flags << 4;


	return flags;
}
bool Can::sendTestMsg(ConnectorId pad, int pin, TypeCable typeCable, NameTestingBlock nameBlock)
{

	if ((int)typeCable == NOT_SET)
		return false;

	int msgSendConnect[8] = { (int)pad, pin, generateFlags(typeCable, nameBlock), 0, 0, 0, 0, 0 };
	//generateFlags(typeCable, nameBlock);
	
	writeCan(10, msgSendConnect); // Дописать проверку ошибок kvasera
	return true;
}

void Can::sendTestMsg(ConnectorId pad, int pin, int digValue, int pwmValue)
{
	int msgSendConnect[8] = { (int)pad, pin, digValue, pwmValue, 0, 0, 0, 0 };
	Can::writeCan(0x55, msgSendConnect);
}