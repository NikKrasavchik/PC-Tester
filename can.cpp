#include "can.h"
#include "Cable.h"
#include "qmap.h"
#include <iostream>
#include <fstream>

Can::modelAdapter *Can::kvaser = new modelAdapter;
Can::modelAdapter *Can::marathon = new modelAdapter;
canHandle Can::hnd = 0;
QMap<int, std::vector<std::pair<Cable, int>>> Can::mapCable;
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


bool Can::checkInformationBus_Can(QString checkAdapter, int canId)
{
	for (int numAdapter = 0; numAdapter < kvaser->nameAdapters.size(); numAdapter++) // Ищем номер адаптера по его названию
		if (kvaser->nameAdapters[numAdapter] == checkAdapter)
		{

			// Инит адаптера
			canInitializeLibrary();
			canHandle hndTmp = canOpenChannel(numAdapter, canOPEN_ACCEPT_VIRTUAL); // Открытие канала связи по CAN.
			canSetBusParams(hndTmp, kvaser->p_frequency.first, 0, 0, 0, 0, 0); // Установка параматров на CAN-шину.
			canBusOn(hndTmp);

			// Отправка проверочного сообщения
			unsigned char msgSendKvase[8] = { 1,0,0,0,0,0,0,0 };
			canWrite(hndTmp, canId, msgSendKvase, 8, 0); // Дописать проверку ошибок kvasera

			// Ждем обратноее сообщение в течении 100 мс
			long id = NOT_SET;
			unsigned int* dlc = new unsigned int(), * flags = new unsigned int();
			unsigned long* timestamp = new unsigned long();
			unsigned char msgReceive[8] = { 0, };
			canReadWait(hndTmp, &id, msgReceive, dlc, flags, timestamp, 100);

			// Выключаем адаптер
			canBusOff(hndTmp);
			canClose(hndTmp);

			// Проверка пришедшего сообщения
			if (id == canId + 1 && msgReceive[0] == 2)
				return true;
			return false;

		}

	for (int numAdapter = 0; numAdapter < marathon->nameAdapters.size(); numAdapter++) // Ищем номер адаптера по его названию
		if (marathon->nameAdapters[numAdapter] == checkAdapter)
		{



			return true;
		}
}

bool Can::checkInformationBus_Lin(QString checkAdapter, int canId)
{
	bool allTrue = true;
	QTime timeWork = QTime::currentTime().addMSecs(100); // Время для авариного выхода из цикла.
	for (int i = 0; i < kvaser->nameAdapters.size(); i++) // Ищем номер адаптера по его названию
		if (kvaser->nameAdapters[i] == checkAdapter)
		{
			linInitializeLibrary();
 			LinHandle hndTmp = linOpenChannel(i, LIN_SLAVE);
			int status = linSetBitrate(hndTmp, 19200);
			status = linBusOn(hndTmp);
			status =  linSetupLIN(hndTmp, 3, 19200);
			unsigned char msgSendKvase[8] = { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA };
			unsigned char msgResiveKvase[8] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
			unsigned int id, dlc, flags;
			LinMessageInfo inf;

			while (true)
			{
				status = linUpdateMessage(hndTmp, canId, msgSendKvase, 8);
				status = linReadMessageWait(hndTmp, &id, msgResiveKvase, &dlc, &flags, &inf, 500);
				if (id == canId + 1)
				{
					for (int j = 0; j < 8; j++)
					{
						if (msgSendKvase[i] != msgResiveKvase[i])
						{
							allTrue = false;
						}
					}
					break;
				}
				if (QTime::currentTime() > timeWork)
				{
					allTrue = false;
					break;
				}
			}


			status = linBusOff(hndTmp);
			linClose(hndTmp);
			status = 0;
			
		}
	if (allTrue)
		return true;
	else
		return false;
}

void Can::checkInformationBus(int canId)
{
	if (b_flagStatusConnection && canId == 1)
	{
		mapCable[canId][0].second = -1;
		return;
	}
	int msgSend[8] = { 1, 0, 0, 0, 0, 0, 0, 0 };
	if(canId > 0x7FF)
		writeCan(canId - 1, msgSend, canMSG_EXT);
	else
		writeCan(canId - 1, msgSend);
	mapCable[canId][0].second = 3;

}

void Can::verificationStartStop(bool seq1, bool seq2, bool seq3, bool seq4, bool seq5, bool seq6, bool seq7, bool seq8, bool seq9, bool seq10)
{
	int byte1 = 0;
	int byte2 = 0;
	if (seq1)
		byte1 += 128;
	if (seq2)
		byte1 += 64;
	if (seq3)
		byte1 += 32;
	if (seq4)
		byte1 += 16;
	if (seq5)
		byte1 += 8;
	if (seq6)
		byte1 += 4;
	if (seq7)
		byte1 += 2;
	if (seq8)
		byte1 += 1;
	if (seq9)
		byte2 += 128;
	if (seq10)
		byte2 += 64;

	int msg[8] = { 0xBC, byte1, byte2, 0x0, 0x0, 0x0, 0x0, 0x0 };

	writeCan(0x55, msg);
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

	timerReadCan->start(2); // И запустим таймер]
	switch (windowType)
	{
	case WindowType::MAINWINDOW:
	case WindowType::CONFIGURATOR:
	case WindowType::IN_TEST_MANUAL_STAND:
	case WindowType::OUT_TEST_MANUAL_STAND:
	case WindowType::FULL_TEST_MANUAL_STAND:
	case WindowType::VERIFICATIONTEST:
		timerCheckStandConnection->start(TIME_CHECKCONNECTION + 100); // И запустим таймер
		break;

	case WindowType::IN_MANUAL_TEST_AUTO_STAND:
	case WindowType::OUT_MANUAL_TEST_AUTO_STAND:
	case WindowType::IN_AUTO_TEST_AUTO_STAND:
	case WindowType::OUT_AUTO_TEST_AUTO_STAND:
	case WindowType::FULL_TEST_AUTO_STAND:
		timerSendConnectMsg->start(100); // И запустим таймер]
		timerCheckStandConnection->start(TIME_CHECKCONNECTION); // И запустим таймер
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
//			false - в случае если b_adapterSelected == false, или ошибку драйверов адаптера.	
//			true  - в слу	чае если can прошёл деинициализацию.
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
  
bool Can::writeCan(int id, int* msg, unsigned int flags)
{
	if (kvaser->activeAdapter != NOT_SET) // kvaser
	{
		unsigned char msgSendKvase[8];
		for (int i = 0; i < 8; i++)
			msgSendKvase[i] = msg[i];
		
		canWrite(hnd, id, msgSendKvase, 8, flags); // Дописать проверку ошибок kvasera
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

// ------------------------------------
// Name: readWaitCan
// Variables:
//			int* id - указатель на переменную в которую запишеться id пришедшего can-сообщения.
//			int* msg - указатель на переменную в которую запишеться сообщение пришедшее из can.
//			int  timeout - время в миллисикундах сколько мы будем ждать сообщение из can-шыны.
// Return: bool
//			false - в случае если за время timeout не пришло сообщение на can-шину.
//			true  - в случае если за время timeout пришло сообшение на can-шину.
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
//			QString adapter - имя адаптера который будет выбран в качестве рабочего.
//			Значения обезательно должно соответсвовать какому либо элементу полученному из метода getNameAdapters().
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

QString Can::getSelectedAdapterNeme()
{
	if (kvaser->activeAdapter != NOT_SET)
		return QString(kvaser->nameAdapters[kvaser->activeAdapter]);
	else if (marathon->activeAdapter != NOT_SET)
		return QString(marathon->nameAdapters[marathon->activeAdapter]);
	else
		return QString();
}

// ------------------------------------
// Name: setSelectedFrequency
//		Установка выбранной частоты для работы can
// Variables: 
//			Qstring frequency: Выбираемая частота.
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
		frequency.remove(-4, 5);																// Обрезаем конец строки
		kvaser->p_frequency = conversionFrequency(frequency.remove(' ').toInt(), KVASER);		// Удаляем пробелы в строке, переводим строку в int, записываем частоту
		marathon->p_frequency = conversionFrequency(frequency.remove(' ').toInt(), MARATHON);

		b_frequencySelected = true;
	}
}

// ------------------------------------
// Name: getNameAdapters
// Return: std::vector<QString>
//			Массив названий Can-адаптеров которые подключены к ПК и мы можем с ними работат.
//			Примечание:
//				У Kvaser есть косяк с которым не поборолись. Kvaser адаптер отключаем от ПК. Запускаем программу. 
//				Подключаем адаптер. По идее он должен появится (как это делает marathon), но так это не происходит.
// ------------------------------------
std::vector<QString> Can::getNameAdapters()
{
	// Чистим массив с названияями адаптеров, для того что бы актуализировать подключенные адаптеры
	kvaser->nameAdapters.clear();
	marathon->nameAdapters.clear();	

	std::vector<QString> resultVector;

	// kvaser
	int chanCount;					// Кол-во адаптеров kvaser
	char charNameAdapter[255];		// Название адаптера

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
int count = 0;

void Can::Timer_ReadCan()
{
#ifdef DEBUG_OUTPUT
	qDebug() << QTime::currentTime().toString("hh:mm:ss:z") << "Start";
#endif

	int id = NOT_SET;
	int msgReceive[8];
	bool isValueChange = false;
	if (Can::readWaitCan(&id, msgReceive, 1))
	{
		for (int key : mapCable.keys())
		{
			if (mapCable[key].size() == 1)
			{
				if (mapCable[key][0].second == -1 && key == 1)
				{
					Signal_ChangedByte(mapCable[key][0].first.getId(), 2);
					mapCable[key][0].second = 400;
					continue; 
				}

				if (mapCable[key][0].second == 2)
					continue;

				mapCable[key][0].second++;
				if (mapCable[key][0].second > 200 && mapCable[key][0].second < 202)
					Signal_ChangedByte(mapCable[key][0].first.getId(), mapCable[key][0].second);
			}
		}

		if (id == DIAG_ID_FROM_DMFL)
		{
			if (1)
			{
				1; // Сюда не должно заходить. Иначе в методе getDiagBlock будет сбой в логике
			}
		}
		switch (windowType)
		{
		case WindowType::IN_TEST_MANUAL_STAND:
		case WindowType::OUT_TEST_MANUAL_STAND:
		case WindowType::FULL_TEST_MANUAL_STAND:
			if (id == ID_CAN_MANUALSTAND)// сообшение о конекте
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
					//if(!(mapCable[id][i].first.getType() == TYPE_CAN || mapCable[id][i].first.getType() == TYPE_LIN)) // если не can и не lin
						mapCable[id][i].second = msgReceive[mapCable[id][i].first.getBit()];
					//else
						//mapCable[id][0].second == 10000;

					Signal_ChangedByte(mapCable[id][i].first.getId(), msgReceive[mapCable[id][i].first.getBit()]);
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
				timerCheckStandConnection->start(TIME_CHECKCONNECTION);
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
				timerCheckStandConnection->start(TIME_CHECKCONNECTION);
				counterConnectMsg++;
#ifdef DEBUG_CAN
				qDebug() << QTime::currentTime().toString("hh:mm:ss:z") << "I received a message about a periodic connection";
#endif // DEBUG_CAN
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
		case WindowType::VERIFICATIONTEST:
			if (id == ID_CAN_MANUALSTAND)// сообшение о конекте
			{
				if (b_flagStatusConnection)
					timerCheckStandConnection->start(TIME_CHECKCONNECTION + 100);
				else
				{
					Signal_ChangedStatusStandConnect(true);
					b_flagStatusConnection = true;
					timerCheckStandConnection->start(TIME_CHECKCONNECTION + 100);

				}
			}
			if (id == 0xAA &&
				msgReceive[0] == 0xBC)
			{
				Signal_ReciveMsg(msgReceive);
			}
			break;
		}
	}

#ifdef DEBUG_CAN
	qDebug() << QTime::currentTime().toString("hh:mm:ss:z") << "Send";
#endif // DEBUG_CAN
#ifdef DEBUG_OUTPUT
	qDebug() << QTime::currentTime().toString("hh:mm:ss:z") << "end";
#endif
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
//			ConnectorId pad - enum переедающий в себе идентификатор коложки (A - 1; B - 2; C - 3; C - 4; ...).
//			int pin - номер пина в колодки.
//			int type - bдентификатор показывающий какого типа пин.
// Return: bool
//			false - в случае если type == NOT_SET, или ошибку драйверов адаптера.	
//			true  - в случае если сообщение отправилось.
// ------------------------------------
bool Can::sendTestMsg(ConnectorId pad, int pin, TypeCable typeCable, TestBlockName nameBlock)
{
	if ((int)typeCable == NOT_SET)
		return false;

	int msgSendConnect[8] = { (int)pad, pin, generateFlags(typeCable, nameBlock), 0, 0, 0, 0, 0 };
	
	writeCan(10, msgSendConnect);
	return true;
}

// ------------------------------------
// Name: sendTestMsg
//		Отправка сообщения на can
// Variables: 
//			ConnectorId pad: Коннектор отправляемого кабеля
//			int pin: Пин отправляемого кабеля
//			int digValue: Цифровое значение отправляемого кабеля
//			int pwmValue: ШИМ значение отправляемого кабеля
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
		else if (pad == ConnectorId::C && pin == 11) // С11
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
		else if (pad == ConnectorId::C && pin == 12) // С12
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

void Can::sendGoToSleepMsg(bool isGoToSleep)
{
	int msgSendConnect[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	if (isGoToSleep)
		msgSendConnect[0] = 0xFF;
	Can::writeCan(0x55, msgSendConnect);
} 


void Can::setCable(std::vector<Cable> cable)
{
	mapCable.clear();
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
	for (int j : mapCable.keys())
		for (int i = 0; i < mapCable[j].size(); i++)
			mapCable[j][i].second = NOT_SET;
	b_flagStatusConnection = false;
}

int intToAscii(int number) {
	return '0' + number;
}

QString Can::getDiagBlock(DiagInformation diagInf, TestBlockName blockName)
{
	int msgSend[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	QTime timeWork = QTime::currentTime().addMSecs(200); // Время для авариного выхода из цикла.
	int id;
	int msg[8];
	int counter = 0;
	int sizeArr = 0;

	QString ansverStr;
	int idTmp = DIAG_ID_FROM_BLOCK(blockName);

	switch (diagInf)
	{
	case DiagInformation::Application_NAME:
		DIAG_GET_APP_NAME(msgSend);
		sizeArr = 31;
		break;
	case DiagInformation::Calibration_NAME:
		DIAG_GET_APP_CALIBRATION(msgSend);
		sizeArr = 79;
		break;
	case DiagInformation::Equipment_NAME:
		DIAG_GET_EQUIPMENT_NAME(msgSend);
		sizeArr = 9;
		break;
	case DiagInformation::Manufacture_DATE:
		DIAG_GET_DATA_MANUFACTURE(msgSend);
		writeCan(DIAG_ID_TO_BLOCK(blockName), msgSend);
		while (true)
		{
			if (readWaitCan(&id, msg, 20))
				if (id == idTmp)
				{
					if (counter == 0)
					{

						ansverStr = QString::number(msg[5],16);
						ansverStr += ".";
						ansverStr += QString::number(msg[6],16);
						ansverStr += ".";
						ansverStr += QString::number(msg[7],16);
	

						counter++;
						DIAG_VERIFICATION(msgSend);
						writeCan(DIAG_ID_TO_BLOCK(blockName), msgSend);
					}
					else
					{
						ansverStr += QString::number(msg[1], 16);


						return ansverStr;
					}
				}
			if (QTime::currentTime() > timeWork)
				return QString("Error. Long delay");

		}
		sizeArr = 15;
		break;
	case DiagInformation::Hardware_NUMBER:
		DIAG_GET_NUMBER_HARDWARE(msgSend);
		sizeArr = 15;
		break;
	case DiagInformation::Part_NUMBER:
		DIAG_GET_NUMBER_PART(msgSend);
		sizeArr = 26;
		break;
	case DiagInformation::Serial_NUMBER:
		DIAG_GET_NUMBER_SERIAL(msgSend);
		sizeArr = 19;
		break;
	}

	writeCan(DIAG_ID_TO_BLOCK(blockName), msgSend);
	while (true)
	{
		if (readWaitCan(&id, msg, 20))
			if (id == idTmp)
			{
				if (counter == 0)
				{
					if(msg[5] != 0)
						ansverStr += QChar(msg[5]);
					if(msg[5] != 0)
						ansverStr += QChar(msg[6]);
					if(msg[7] != 0)
						ansverStr += QChar(msg[7]);

					counter = 3;
					DIAG_VERIFICATION(msgSend);
					writeCan(DIAG_ID_TO_BLOCK(blockName), msgSend);
				}
				else
					for (int i = 1; i < 8; i++)
					{
						counter++;
						if (msg[i] == 0)
						{
							if (counter > sizeArr)
								return ansverStr;
							continue;
						}
						ansverStr += QChar(msg[i]);
						if (counter > sizeArr)
							return ansverStr;
					}
			}
		if (QTime::currentTime() > timeWork)
			return QString("Error. Long delay");


	}


}
