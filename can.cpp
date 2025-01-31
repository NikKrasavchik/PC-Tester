#include "can.h"


Can::modelAdapter *Can::kvaser = new modelAdapter;
Can::modelAdapter *Can::marathon = new modelAdapter;
canHandle Can::hnd = 0;


Can::Can()
{
	b_adapterSelected = false;
	b_frequencySelected = false;
	b_flagStandConnectionCheck = false;
	b_flagIsChangedStandConnection = false;

	timerReadCan = new QTimer();
	connect(timerReadCan, SIGNAL(timeout()), this, SLOT(Slot_ReadCan()));
	timerCheckStandConnection = new QTimer();
	connect(timerCheckStandConnection, SIGNAL(timeout()), this, SLOT(Slot_CheckStandConnection()));
}

bool Can::initCan()
{
	if (!b_adapterSelected)
		return false;
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

	timerReadCan->start(10); // И запустим таймер]


	timerCheckStandConnection->start(100); // И запустим таймер

	return true;
}

bool Can::deinitCan()
{
	timerReadCan->stop();
	timerCheckStandConnection->stop();

	b_flagStandConnectionCheck = false;
	b_flagIsChangedStandConnection = false;
	
	
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

void Can::Slot_ReadCan()
{
	int id = -2;
	int msgReceive[8];
	if (Can::readWaitCan(&id, msgReceive, 10))
	{
		if (id == RECEIVE_ID_CAN_AUTO_STAND && // переодическое сообшение о конекте
			msgReceive[0] == 0x0 &&
			msgReceive[1] == 0xAA &&
			msgReceive[2] == 0x0 &&
			msgReceive[3] == 0xAA &&
			msgReceive[4] == 0x0 &&
			msgReceive[5] == 0xAA &&
			msgReceive[6] == 0x0 &&
			msgReceive[7] == 0xFF)
		{
			b_flagStandConnectionCheck = true;
		}
	}

	qDebug() << QTime::currentTime().toString("hh:mm:ss:z") << "Send";
//	delete msgReceive;
}
void Can::Slot_CheckStandConnection()
{
	if (b_flagIsChangedStandConnection != b_flagStandConnectionCheck)
	{
		b_flagIsChangedStandConnection = b_flagStandConnectionCheck;
		if (b_flagStandConnectionCheck)
		{
			Signal_ChangedStatusStandConnect(true);
		}
		else
		{
			Signal_ChangedStatusStandConnect(false);
		}
	}

	qDebug() << QTime::currentTime().toString("hh:mm:ss:z") << "Change";
	b_flagStandConnectionCheck = false;
}

void Can::sendTestMsg(int pad, int pin)
{
}

void Can::sendTestMsg(int pad, int pin, int digValue, int pwmValue)
{
}