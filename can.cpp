#include "can.h"
#include "Cable.h"
#include "qmap.h"
#include <iostream>
#include <fstream>

Can::modelAdapter *Can::kvaser = new modelAdapter;
Can::modelAdapter *Can::marathon = new modelAdapter;
Can::modelAdapter *Can::pcan = new modelAdapter;
canHandle Can::hnd = 0;
HANDLE Can::hEventPcan = 0;
QTimer* Can::wakeBoot = new QTimer();
QMap<int, std::vector<std::pair<Cable, int>>> Can::mapCable;
bool Can::b_flagStatusConnectionBlock;
bool Can::testSleep = false;

Can::Can()
{
	b_adapterSelected = false;
	b_frequencySelected = false;
	b_flagStatusConnectionBlock = false;

	counterConnectMsg = 0;
	windowType = WindowType();

	timerReadCan = new QTimer();
	timerReadCan->setTimerType(Qt::PreciseTimer);
	connect(timerReadCan, SIGNAL(timeout()), this, SLOT(Timer_ReadCan()));
	timerCheckBlockConnection = new QTimer();
	connect(timerCheckBlockConnection, SIGNAL(timeout()), this, SLOT(Timer_CheckBlockConnection()));

	for (int i = 0; i < COUNT_PCB_STAND; i++)
	{
		standConnect[i].TimerSendConnectMsg = new QTimer();
		standConnect[i].TimerSendConnectMsg->setTimerType(Qt::PreciseTimer);
		//standConnect[i].timerIdSend = standConnect[i].TimerSendConnectMsg->timerId();
		connect(standConnect[i].TimerSendConnectMsg, SIGNAL(timeout()), this, SLOT(Timer_SendConnectMsgStand()));

		standConnect[i].TimerCheckConnectMsg = new QTimer();
		//standConnect[i].timerIdCheck = standConnect[i].TimerCheckConnectMsg->timerId();
		connect(standConnect[i].TimerCheckConnectMsg, SIGNAL(timeout()), this, SLOT(Timer_CheckStandConnection()));
	}


}

void Can::checkInformationBus(int canId)
{
	if (b_flagStatusConnectionBlock && canId == 1)
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
	int status = -1;
	if (!b_adapterSelected)
		return false;
	
	this->windowType = windowType;
	testSleep = false;
	if (kvaser->activeAdapter != NOT_SET) // kvaser
	{
		// Äîïèñàòü ïðîâåðêó îøèáîê kvasera
		canInitializeLibrary(); // Èíèöèàëèçàöèÿ api kvaser
		hnd = canOpenChannel(kvaser->activeAdapter, canOPEN_ACCEPT_VIRTUAL); // Îòêðûòèå êàíàëà ñâÿçè ïî CAN.
		canSetBusParams(hnd, kvaser->p_frequency.first, 0, 0, 0, 0, 0); // Óñòàíîâêà ïàðàìàòðîâ íà CAN-øèíó.
		canBusOn(hnd); // Çàïóñê CAN-øèíû
	}
	else if(marathon->activeAdapter != NOT_SET)
	{ // Äîïèñàòü ïðîâåðêó îøèáîê Marathon
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
	else if (pcan->activeAdapter != NOT_SET)
	{
		status = CAN_Initialize(pcan->handlerChanel[pcan->activeAdapter], pcan->p_frequency.first);
		hEventPcan = CreateEvent(NULL, FALSE, FALSE, NULL);
		CAN_SetValue(pcan->handlerChanel[pcan->activeAdapter], PCAN_RECEIVE_EVENT, &hEventPcan, sizeof(hEventPcan));
	}



	timerReadCan->start(2); // È çàïóñòèì òàéìåð]
	switch (windowType)
	{
	case WindowType::MAINWINDOW:
	case WindowType::CONFIGURATOR:
	case WindowType::IN_TEST_MANUAL_STAND:
	case WindowType::OUT_TEST_MANUAL_STAND:
	case WindowType::FULL_TEST_MANUAL_STAND:
	case WindowType::VERIFICATIONTEST:
		timerCheckBlockConnection->start(TIME_CHECKCONNECTION + 100); // È çàïóñòèì òàéìåð
		break;

	case WindowType::IN_MANUAL_TEST_AUTO_STAND:
	case WindowType::OUT_MANUAL_TEST_AUTO_STAND:
	case WindowType::IN_AUTO_TEST_AUTO_STAND:
	case WindowType::OUT_AUTO_TEST_AUTO_STAND:
	case WindowType::FULL_TEST_AUTO_STAND:
		for (int i = 0; i < COUNT_PCB_STAND; i++)
		{
			standConnect[i].TimerSendConnectMsg->start(300); // È çàïóñòèì òàéìåð]
			standConnect[i].timerIdSend = standConnect[i].TimerSendConnectMsg->timerId();
		}
		timerCheckBlockConnection->start(TIME_CHECKCONNECTION); // È çàïóñòèì òàéìåð
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
//			false - â ñëó÷àå åñëè b_adapterSelected == false, èëè îøèáêó äðàéâåðîâ àäàïòåðà.	
//			true  - â ñëó	÷àå åñëè can ïðîø¸ë äåèíèöèàëèçàöèþ.
// ------------------------------------
bool Can::deinitCan()
{
	timerReadCan->stop();
	for (int i = 0; i < COUNT_PCB_STAND; i++)
		standConnect[i].TimerSendConnectMsg->stop();
	timerCheckBlockConnection->stop();

	b_flagStatusConnectionBlock = false;

	measureds.clear();
	
	if (!b_adapterSelected)
		return false;
	if (kvaser->activeAdapter != NOT_SET) // kvaser 
	{
		canBusOff(hnd);  // Äîïèñàòü ïðîâåðêó îøèáîê kvasera
		canClose(hnd);
	}
	else if(marathon->activeAdapter != NOT_SET)
	{
		CiStop(marathon->activeAdapter);  // Äîïèñàòü ïðîâåðêó îøèáîê Marathon
		CiClose(marathon->activeAdapter);
	}
	else if (pcan->activeAdapter != NOT_SET)
	{
		CAN_Uninitialize(pcan->activeAdapter);
	}
	return true;
}
  
bool Can::writeCan(int id, int* msg, unsigned int flags)
{
	int status = -1;

	if (kvaser->activeAdapter != NOT_SET) // kvaser
	{
		unsigned char msgSendKvase[8];
		for (int i = 0; i < 8; i++)
			msgSendKvase[i] = msg[i];
		
		canWrite(hnd, id, msgSendKvase, 8, flags); // Äîïèñàòü ïðîâåðêó îøèáîê kvasera
		return true;
	}
	else if(marathon->activeAdapter != NOT_SET)
	{
		static canmsg_t msgTransmit;
		msgTransmit.id = id;
		msgTransmit.len = 8;
		for (int i = 0; i < 8; i++)
			msgTransmit.data[i] = msg[i];
		
		auto statusTmp = CiTransmit(marathon->activeAdapter, &msgTransmit); // Äîïèñàòü ïðîâåðêó îøèáîê Marathon
		return true;
	}
	else if (pcan->activeAdapter != NOT_SET)
	{

		TPCANMsg msgPCAN;
		msgPCAN.ID = id;
		msgPCAN.LEN = 8;
		if (id <= 0x7FF)
			msgPCAN.MSGTYPE = PCAN_MESSAGE_STANDARD;
		else
			msgPCAN.MSGTYPE = PCAN_MESSAGE_EXTENDED;

		for (int i = 0; i < 8; i++)
			msgPCAN.DATA[i] = msg[i];
		 status = CAN_Write(pcan->handlerChanel[pcan->activeAdapter], &msgPCAN);

	}
	return false;
}

// ------------------------------------
// Name: readWaitCan
// Variables:
//			int* id - óêàçàòåëü íà ïåðåìåííóþ â êîòîðóþ çàïèøåòüñÿ id ïðèøåäøåãî can-ñîîáùåíèÿ.
//			int* msg - óêàçàòåëü íà ïåðåìåííóþ â êîòîðóþ çàïèøåòüñÿ ñîîáùåíèå ïðèøåäøåå èç can.
//			int  timeout - âðåìÿ â ìèëëèñèêóíäàõ ñêîëüêî ìû áóäåì æäàòü ñîîáùåíèå èç can-øûíû.
// Return: bool
//			false - â ñëó÷àå åñëè çà âðåìÿ timeout íå ïðèøëî ñîîáùåíèå íà can-øèíó.
//			true  - â ñëó÷àå åñëè çà âðåìÿ timeout ïðèøëî ñîîáøåíèå íà can-øèíó.
// ------------------------------------
canmsg_t msgReceive;
bool Can::readWaitCan(int* id, int* msg, int timeout)
{
	int status = -1;

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
	else if(marathon->activeAdapter != NOT_SET) // narathon
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
	else if (pcan->activeAdapter != NOT_SET) // pcan
	{

		TPCANMsg msgPCAN;
		if (CAN_Read(pcan->handlerChanel[pcan->activeAdapter], &msgPCAN, NULL) != 32)
		{
			*id = msgPCAN.ID;
			for (int i = 0; i < 8; i++)
				msg[i] = msgPCAN.DATA[i];
			return true;
		}
		return false;
	}
	return false;
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

	for (int i = 0; i < kvaser->nameAdapters.size(); i++)
		if (kvaser->nameAdapters[i] == adapter)
		{
			kvaser->activeAdapter = i;
			b_adapterSelected = true;
			return;
		}

	for (int i = 0; i < marathon->nameAdapters.size(); i++)
		if (marathon->nameAdapters[i] == adapter)
		{
			marathon->activeAdapter = i;
			b_adapterSelected = true;
			return;
		}
	for (int i = 0; i < pcan->nameAdapters.size(); i++)
		if (pcan->nameAdapters[i] == adapter)
		{
			pcan->activeAdapter = i;
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
	else if (pcan->activeAdapter != NOT_SET)
		return QString(pcan->nameAdapters[pcan->activeAdapter]);
	else
		return QString();
}

// ------------------------------------
// Name: setSelectedFrequency
//		Óñòàíîâêà âûáðàííîé ÷àñòîòû äëÿ ðàáîòû can
// Variables: 
//			Qstring frequency: Âûáèðàåìàÿ ÷àñòîòà.
// ------------------------------------
void Can::setSelectedFrequency(QString frequency)
{
	if (frequency == "...")
	{
		kvaser->p_frequency = conversionFrequency(0, ModelAdapter::Kvase);
		marathon->p_frequency = conversionFrequency(0, ModelAdapter::Marathon);
		marathon->p_frequency = conversionFrequency(0, ModelAdapter::PCan);

		b_frequencySelected = false;
	}
	else
	{
		frequency.remove(-4, 5);																// Îáðåçàåì êîíåö ñòðîêè
		kvaser->p_frequency = conversionFrequency(frequency.remove(' ').toInt(), ModelAdapter::Kvase);		// Óäàëÿåì ïðîáåëû â ñòðîêå, ïåðåâîäèì ñòðîêó â int, çàïèñûâàåì ÷àñòîòó
		marathon->p_frequency = conversionFrequency(frequency.remove(' ').toInt(), ModelAdapter::Marathon);
		pcan->p_frequency = conversionFrequency(frequency.remove(' ').toInt(), ModelAdapter::PCan);

		b_frequencySelected = true;
	}
}

// ------------------------------------
// Name: getNameAdapters
// Return: std::vector<QString>
//			Ìàññèâ íàçâàíèé Can-àäàïòåðîâ êîòîðûå ïîäêëþ÷åíû ê ÏÊ è ìû ìîæåì ñ íèìè ðàáîòàò.
//			Ïðèìå÷àíèå:
//				Ó Kvaser åñòü êîñÿê ñ êîòîðûì íå ïîáîðîëèñü. Kvaser àäàïòåð îòêëþ÷àåì îò ÏÊ. Çàïóñêàåì ïðîãðàììó. 
//				Ïîäêëþ÷àåì àäàïòåð. Ïî èäåå îí äîëæåí ïîÿâèòñÿ (êàê ýòî äåëàåò marathon), íî òàê ýòî íå ïðîèñõîäèò.
// ------------------------------------
std::vector<QString> Can::getNameAdapters()
{
	// ×èñòèì ìàññèâ ñ íàçâàíèÿÿìè àäàïòåðîâ, äëÿ òîãî ÷òî áû àêòóàëèçèðîâàòü ïîäêëþ÷åííûå àäàïòåðû
	kvaser->nameAdapters.clear();
	marathon->nameAdapters.clear();	
	pcan->nameAdapters.clear();	
	pcan->handlerChanel.clear();
	std::vector<QString> resultVector;

	// kvaser
	int chanCount;					// Êîë-âî àäàïòåðîâ kvaser
	char charNameAdapter[255];		// Íàçâàíèå àäàïòåðà

	canInitializeLibrary();
	canGetNumberOfChannels(&chanCount);
	QString strNameAdapter;
	for (int i = 0; i < chanCount; i++) 
	{
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

	//pcan

	TPCANChannelInformation channelInfo[16]; // áóôåð äëÿ èíôîðìàöèè î êàíàëàõ
	DWORD bufferSize = sizeof(channelInfo);
	for (int i = 0; i < bufferSize / sizeof(TPCANChannelInformation); i++)
		channelInfo[i].device_type = 0;
	CAN_GetValue(PCAN_NONEBUS, PCAN_ATTACHED_CHANNELS, channelInfo, bufferSize);

	int count = bufferSize / sizeof(TPCANChannelInformation);
	for (int i = 0; i < count; i++)
	{
		TPCANChannelInformation& info = channelInfo[i];
		if (info.device_type != 0)
		{
			pcan->nameAdapters.push_back(QString::fromStdString(info.device_name));
			pcan->handlerChanel.push_back(info.channel_handle);
			resultVector.push_back(QString::fromStdString(info.device_name));
		}
	}

	return resultVector;
}

std::pair<int, int> Can::conversionFrequency(int frequency, ModelAdapter modelAdapter)
{
	std::pair<int, int> resultPair;
	resultPair.first = 0;
	resultPair.second = 0;

	switch (frequency)
	{
	case FREQUENCY_50K:
		switch (modelAdapter)
		{
		case ModelAdapter::EMPTY:
			break;
		case ModelAdapter::Kvase:
			resultPair.first = BAUD_50K;
			break;
		case ModelAdapter::Marathon:
			resultPair.first = BCI_50K_bt0;
			resultPair.second = BCI_50K_bt1;
			break;
		case ModelAdapter::PCan:
			resultPair.first = PCAN_BAUD_50K;
			break;
		default:
			break;
		}
		break;

	case FREQUENCY_100K:
		switch (modelAdapter)
		{
		case ModelAdapter::EMPTY:
			break;
		case ModelAdapter::Kvase:
			resultPair.first = BAUD_100K;
			break;
		case ModelAdapter::Marathon:
			resultPair.first = BCI_100K_bt0;
			resultPair.second = BCI_100K_bt1;
			break;
		case ModelAdapter::PCan:
			resultPair.first = PCAN_BAUD_100K;
			break;
		default:
			break;
		}
		break;

	case FREQUENCY_125K:
		switch (modelAdapter)
		{
		case ModelAdapter::EMPTY:
			break;
		case ModelAdapter::Kvase:
			resultPair.first = BAUD_125K;
			break;
		case ModelAdapter::Marathon:
			resultPair.first = BCI_125K_bt0;
			resultPair.second = BCI_125K_bt1;
			break;
		case ModelAdapter::PCan:
			resultPair.first = PCAN_BAUD_125K;
			break;
		default:
			break;
		}
		break;

	case FREQUENCY_250K:
		switch (modelAdapter)
		{
		case ModelAdapter::EMPTY:
			break;
		case ModelAdapter::Kvase:
			resultPair.first = BAUD_250K;
			break;
		case ModelAdapter::Marathon:
			resultPair.first = BCI_250K_bt0;
			resultPair.second = BCI_250K_bt1;
			break;
		case ModelAdapter::PCan:
			resultPair.first = PCAN_BAUD_250K;
			break;
		default:
			break;
		}
		break;

	case FREQUENCY_500K:
		switch (modelAdapter)
		{
		case ModelAdapter::EMPTY:
			break;
		case ModelAdapter::Kvase:
			resultPair.first = BAUD_500K;
			break;
		case ModelAdapter::Marathon:
			resultPair.first = BCI_500K_bt0;
			resultPair.second = BCI_500K_bt1;
			break;
		case ModelAdapter::PCan:
			resultPair.first = PCAN_BAUD_500K;
			break;
		default:
			break;
		}
		break;

	case FREQUENCY_1000K:
		switch (modelAdapter)
		{
		case ModelAdapter::EMPTY:
			break;
		case ModelAdapter::Kvase:
			resultPair.first = BAUD_1M;
			break;
		case ModelAdapter::Marathon:
			resultPair.first = BCI_1M_bt0;
			resultPair.second = BCI_1M_bt1;
			break;
		case ModelAdapter::PCan:
			resultPair.first = PCAN_BAUD_1M;
			break;
		default:
			break;
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
	if (Can::readWaitCan(&id, msgReceive, 20))
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
				1; // Ñþäà íå äîëæíî çàõîäèòü. Èíà÷å â ìåòîäå getDiagBlock áóäåò ñáîé â ëîãèêå
			}
		}
		switch (windowType)
		{
		case WindowType::IN_TEST_MANUAL_STAND:
		case WindowType::OUT_TEST_MANUAL_STAND:
		case WindowType::FULL_TEST_MANUAL_STAND:
			if (id == ID_CAN_MANUALSTAND)// ñîîáøåíèå î êîíåêòå
			{
				if (b_flagStatusConnectionBlock)
					timerCheckBlockConnection->start(TIME_CHECKCONNECTION + 100);
				else
				{
					Signal_ChangedStatusBlockConnect(true);
					b_flagStatusConnectionBlock = true;
					timerCheckBlockConnection->start(TIME_CHECKCONNECTION + 100);

				}
			}
			for (int i = 0; i < mapCable[id].size(); i++)
			{

				if (mapCable[id][i].second != msgReceive[mapCable[id][i].first.getBit()])
				{
					mapCable[id][i].second = msgReceive[mapCable[id][i].first.getBit()];
					Signal_ChangedByte(mapCable[id][i].first.getId(), msgReceive[mapCable[id][i].first.getBit()]);
				}
			}

			break;

		case WindowType::IN_MANUAL_TEST_AUTO_STAND:
		case WindowType::OUT_MANUAL_TEST_AUTO_STAND:
		case WindowType::IN_AUTO_TEST_AUTO_STAND:
		case WindowType::OUT_AUTO_TEST_AUTO_STAND:
		case WindowType::FULL_TEST_AUTO_STAND:
			if (id == ID_CAN_MANUALSTAND)// ñîîáøåíèå î êîíåêòå Áëîêà
			{
				if (b_flagStatusConnectionBlock)
					timerCheckBlockConnection->start(TIME_CHECKCONNECTION + 100);
				else
				{
					Signal_ChangedStatusBlockConnect(true);
					b_flagStatusConnectionBlock = true;
					timerCheckBlockConnection->start(TIME_CHECKCONNECTION + 100);

				}
			}
			if (id == ID_CAN_AUTOSTAND_RECEIVE && // ñîîáøåíèå î êîíåêòå Ñòåíä
				!standConnect[(msgReceive[7] & 0xF) - 1].standConnection &&
				msgReceive[0] == 0x0 &&
				msgReceive[1] == 0xAA &&
				msgReceive[2] == 0x0 &&
				msgReceive[3] == 0xAA &&
				msgReceive[4] == 0x0 &&
				msgReceive[5] == 0xAA &&
				msgReceive[6] == 0x0 &&
				(msgReceive[7] & 0xF0) == 0xF0)
			{
				int idBord = (msgReceive[7] & 0xF) - 1;
				Signal_ChangedStatusStandConnect(true, idBord);
				standConnect[idBord].standConnection = true;
				standConnect[idBord].TimerCheckConnectMsg->start(TIME_CHECKCONNECTION);
				standConnect[idBord].timerIdCheck = standConnect[idBord].TimerCheckConnectMsg->timerId();
				standConnect[idBord].counterConnectMsg = 0;
				standConnect[idBord].counterStandDisconnecting = 0;
				standConnect[idBord].sandTestMsg = true;

			}
			else if (id == ID_CAN_AUTOSTAND_RECEIVE && // ïåðåîäè÷åñêîå ñîîáøåíèå î êîíåêòå
				standConnect[(msgReceive[7] & 0xF) - 1].standConnection &&
				msgReceive[0] == standConnect[(msgReceive[7] & 0xF) - 1].counterConnectMsg &&
				msgReceive[1] == 0xAA &&
				msgReceive[2] == 0x0 &&
				msgReceive[3] == 0xAA &&
				msgReceive[4] == 0x0 &&
				msgReceive[5] == 0xAA &&
				msgReceive[6] == 0x0 &&
				(msgReceive[7] & 0xF0) == 0xF0)
			{
				int idBord = (msgReceive[7] & 0xF) - 1;
				standConnect[idBord].TimerCheckConnectMsg->start(TIME_CHECKCONNECTION);
				standConnect[idBord].timerIdCheck = standConnect[idBord].TimerCheckConnectMsg->timerId();
				standConnect[idBord].counterConnectMsg++;
				standConnect[idBord].counterStandDisconnecting = 0;
				standConnect[idBord].sandTestMsg = true;

			}
			else if (id == ID_CAN_AUTOSTAND_RECEIVE && (msgReceive[7] & 0xF0) != 0xF0) // Ñîîáùåíèå î ðåçóëüòàòå òåñòà
			{
				if (msgReceive[7] == 7)
				{
					float v;
					uint32_t tmp =
						(static_cast<uint32_t>(msgReceive[2]) << 24) |
						(static_cast<uint32_t>(msgReceive[3]) << 16) |
						(static_cast<uint32_t>(msgReceive[4]) << 8) |
						(static_cast<uint32_t>(msgReceive[5]) << 0);
					std::memcpy(&v, &tmp, sizeof(float));

					measureds.push_back(new Measureds(0, v, msgReceive[6]));
					Signal_AfterTest(msgReceive[0], msgReceive[1], measureds);
					measureds.clear();
					testSleep = false;
				}
				if (msgReceive[2] < 6 && filled_ == 0)
				{


						for (int i = 0; i < msgReceive[2]; i++)
							measureds.push_back(new Measureds(msgReceive[3 + i]));
						Signal_AfterTest(msgReceive[0], msgReceive[1], measureds);
						measureds.clear();

				}
				else
				{
				
					uint8_t frame_num = expected_frame_;   // òåêóùèé îæèäàåìûé

					uint8_t expected_filled = frame_num * 8;
					if (expected_filled > 21) {
						reset();
						//return -1;
						return;
					}

					uint8_t remaining = 21 - filled_;
					uint8_t copy_len = (remaining > 8) ? 8 : remaining;

					for (int i = 0; i < copy_len; i++)
						buf_[expected_filled + i] = msgReceive[i];
					filled_ += copy_len;

					expected_frame_++;  // ñëåäóþùèé êàäð

					if (filled_ >= 21)
						complete_ = true;

					if (complete_)
						get_payload();
					//return 0;
					return;
				}

			}
			break;
		case WindowType::VERIFICATIONTEST:
			if (id == ID_CAN_MANUALSTAND)// ñîîáøåíèå î êîíåêòå
			{
				if (b_flagStatusConnectionBlock)
					timerCheckBlockConnection->start(TIME_CHECKCONNECTION + 100);
				else
				{
					Signal_ChangedStatusBlockConnect(true);
					b_flagStatusConnectionBlock = true;
					timerCheckBlockConnection->start(TIME_CHECKCONNECTION + 100);

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

void Can::Timer_SendConnectMsgStand()
{
	if (testSleep)
		return;
	QTimer* timer = qobject_cast<QTimer*>(sender());
	for (int i = 0; i < COUNT_PCB_STAND; i++)
	{
		if (timer->timerId() == standConnect[i].timerIdSend)
		{
			if (standConnect[i].standConnection && standConnect[i].sandTestMsg)
			{
				int msgSendConnect[8] = { standConnect[i].counterConnectMsg, 0x0, 0xAA, 0x0, 0xAA, 0x0, 0xAA, (((i+1) << 4) + 0xF)};
				writeCan(ID_CAN_AUTOSTAND_SEND, msgSendConnect);
				standConnect[i].counterConnectMsg++;
				standConnect[i].sandTestMsg = false;
			}
			else if(!standConnect[i].standConnection)
			{
				int msgSendConnect[8] = { 0xAA, 0x0, 0xAA, 0x0, 0xAA, 0x0, 0xAA, (((i+1) << 4) + 0xF) };
				writeCan(ID_CAN_AUTOSTAND_SEND, msgSendConnect);
			}
			
		}
	}
}
void Can::Timer_CheckBlockConnection()
{
	timerCheckBlockConnection->stop();
	Signal_ChangedStatusBlockConnect(false);
	b_flagStatusConnectionBlock = false;
	
}

void Can::Timer_CheckStandConnection()
{
	QTimer* timer = qobject_cast<QTimer*>(sender());
	for (int i = 0; i < COUNT_PCB_STAND; i++)
	{
		if (timer->timerId() == standConnect[i].timerIdCheck)
		{

				standConnect[i].TimerCheckConnectMsg->stop();
				Signal_ChangedStatusStandConnect(false, i);
				standConnect[i].standConnection = false;
				standConnect[i].counterConnectMsg = 0;

		}
	}

}

float read_float_be(const uint8_t* src) {
	uint32_t tmp =
		(static_cast<uint32_t>(src[0]) << 24) |
		(static_cast<uint32_t>(src[1]) << 16) |
		(static_cast<uint32_t>(src[2]) << 8) |
		(static_cast<uint32_t>(src[3]) << 0);
	float v;
	std::memcpy(&v, &tmp, sizeof(float));
	return v;
}

void Can::get_payload()
{

	const uint8_t* buf = buf_.data();

	float f1 = read_float_be(&buf[3]);
	float f2 = read_float_be(&buf[7]);
	int u1 = buf[11];
	float f3 = read_float_be(&buf[12]);
	float f4 = read_float_be(&buf[16]);
	int u2 = buf[20];

	measureds.push_back(new Measureds(read_float_be(&buf[3]), read_float_be(&buf[7]), buf[11]));
	measureds.push_back(new Measureds(read_float_be(&buf[12]), read_float_be(&buf[16]), buf[20]));

	Signal_AfterTest(buf[0], buf[1], measureds);
	measureds.clear();

	reset();
}

//int Can::process_frame(uint8_t* data) 
//{  
//	return 1;
//}

void Can::reset() 
{
	filled_ = 0;
	expected_frame_ = 0;
	complete_ = false;
	buf_.fill(0);
}

int Can::parse_payload() 
{
	const uint8_t* buf = buf_.data();
	if (buf[2] != 6) {
		reset();
		return -1;
	}
	reset();
	return 1;
}

uint8_t generateFlags(TypeCable typeCable, TestBlockName nameBlock)
{
	uint8_t flags = 0;
	// Block
	switch (nameBlock)
	{
	case TestBlockName::EMPTY:
		break;
	case TestBlockName::DTM:
		flags = 1;
		break;
	case TestBlockName::BCM:
		flags = 2;
		break;
	case TestBlockName::SMXX:
		flags = 3;
		break;
	default:
		break;
	}
	flags = flags << 4;
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
	case TypeCable::SLEEP:
		flags += 7;

		break;
	}


	return flags;
}

// ------------------------------------
// Name: sendTestMsg
// Variables: 
//			ConnectorId pad - enum ïåðååäàþùèé â ñåáå èäåíòèôèêàòîð êîëîæêè (A - 1; B - 2; C - 3; C - 4; ...).
//			int pin - íîìåð ïèíà â êîëîäêè.
//			int type - bäåíòèôèêàòîð ïîêàçûâàþùèé êàêîãî òèïà ïèí.
// Return: bool
//			false - â ñëó÷àå åñëè type == NOT_SET, èëè îøèáêó äðàéâåðîâ àäàïòåðà.	
//			true  - â ñëó÷àå åñëè ñîîáùåíèå îòïðàâèëîñü.
// ------------------------------------
bool Can::sendTestMsg(ConnectorId pad, int pin, int canId, int byteCan, TypeCable typeCable, TestBlockName nameBlock)
{
	if ((int)typeCable == NOT_SET)
		return false;
	if (typeCable == TypeCable::SLEEP)
		testSleep = true;
	int msgSendConnect[8] = { (int)pad, pin, generateFlags(typeCable, nameBlock), (canId - 0x100), byteCan, 0, 0, 0 };
	
	writeCan(0x51, msgSendConnect);
	Sleep(100);
	return true;
}

// ------------------------------------
// Name: sendTestMsg
//		Îòïðàâêà ñîîáùåíèÿ íà can
// Variables: 
//			ConnectorId pad: Êîííåêòîð îòïðàâëÿåìîãî êàáåëÿ
//			int pin: Ïèí îòïðàâëÿåìîãî êàáåëÿ
//			int digValue: Öèôðîâîå çíà÷åíèå îòïðàâëÿåìîãî êàáåëÿ	
//			int pwmValue: ØÈÌ çíà÷åíèå îòïðàâëÿåìîãî êàáåëÿ
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
		else if (pad == ConnectorId::C && pin == 11) // Ñ11
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
		else if (pad == ConnectorId::C && pin == 12) // Ñ12
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
		else if (pad == ConnectorId::C && pin == 9) // C9
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
	b_flagStatusConnectionBlock = false;
}

int intToAscii(int number) {
	return '0' + number;
}

QString Can::getDiagBlock(DiagInformation diagInf, TestBlockName blockName)
{
	int msgSend[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	QTime timeWork = QTime::currentTime().addMSecs(200); // Âðåìÿ äëÿ àâàðèíîãî âûõîäà èç öèêëà.
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
				return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Ошибка. Длительная задержка") : QString("Error. Long delay");

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
			return viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Ошибка. Длительная задержка") : QString("Error. Long delay");
	}


}
//DMxx
const unsigned int _rot_dir_DMxx = 21;
const unsigned int _rot_num_DMxx[] = { 12, 25, 13, 26 };
const unsigned int _inv_type_DMxx[] = { 29, 3 };
const unsigned int _bitwise_op_DMxx[] = { 2, 22 };
const unsigned int _inv_bits_DMxx[3][3] = { {27,28,30}, {8,19,14}, {29,24,6} };
//TM
const unsigned int _rot_dir_TM = 21;
const unsigned int _rot_num_TM[] = { 13, 19, 22, 8 };
const unsigned int _inv_type_TM[] = { 15, 26 };
const unsigned int _bitwise_op_TM[] = { 0, 29 };
const unsigned int _inv_bits_TM[3][3] = { {19,28,7}, {26,12,16}, {17,2,24} };
//BCM
const unsigned int _rot_dir_BCM = 18;
const unsigned int _rot_num_BCM[] = { 30, 11, 8, 19 };
const unsigned int _inv_type_BCM[] = { 14, 13 };
const unsigned int _bitwise_op_BCM[] = { 25, 5 };
const unsigned int _inv_bits_BCM[3][3] = { {17,21,15}, {2,7,19}, {29,14,0} };
//SMXX
const unsigned int _rot_dir_SMXX = 29;
const unsigned int _rot_num_SMXX[] = { 15, 3, 11, 16 };
const unsigned int _inv_type_SMXX[] = { 9, 18 };
const unsigned int _bitwise_op_SMXX[] = { 1, 2 };
const unsigned int _inv_bits_SMXX[3][3] = { {31,12,21}, {3,19,9}, {24,9,26} };

static unsigned int _bit_val(unsigned int num, unsigned int bit)
{
	return (num >> bit) & 1;
}

static unsigned int _field_val(unsigned int num, const unsigned int* field, unsigned int len)
{
	unsigned int res = 0;
	for (unsigned int i = 0; i < len; i++)
	{
		res <<= 1;
		res += _bit_val(num, field[i]);
	}
	return res;
}

enum VKeyGenResultEx_enum
{
	KGRE_Ok = 0,
	KGRE_BufferToSmall = 1,
	KGRE_SecurityLevelInvalid = 2,
	KGRE_VariantInvalid = 3,
	KGRE_UnspecifiedError = 4,
	KGRE_BadDll = 5,
	KGRE_FunctionNotFound = 6
};

uint32_t custom_bswap32(uint32_t value) {
	return (value >> 24) | ((value << 8) & 0x00FF0000) | ((value >> 8) & 0x0000FF00) | (value << 24);
}

VKeyGenResultEx_enum GenerateKeyEx_TM(
	const unsigned char* ipSeedArray, unsigned int iSeedArraySize,
	unsigned char* iopKeyArray, unsigned int iMaxKeyArraySize,
	unsigned int* oActualKeyArraySize)
{
	if (iSeedArraySize != 4) return KGRE_BadDll;
	if (iMaxKeyArraySize < 4) return KGRE_BufferToSmall;

	unsigned int seed = custom_bswap32(*((unsigned int*)ipSeedArray));

	unsigned int res;
	unsigned int rot_num = _field_val(seed, _rot_num_TM, sizeof(_rot_num_TM) / sizeof(int));
	if (_bit_val(seed, _rot_dir_TM)) { rot_num = (32 - rot_num); }
	res = seed << rot_num;
	res = (res & 0xFFFFFFFF) | (seed >> (32 - rot_num));

	unsigned int inv_type = _field_val(seed, _inv_type_TM, sizeof(_inv_type_TM) / sizeof(int));
	if (inv_type > 0)
	{
		for (unsigned int i = 0; i < 3; i++)
		{
			res ^= (1 << _inv_bits_TM[inv_type - 1][i]);
		}
	}

	unsigned int bitwise_op = _field_val(seed, _bitwise_op_TM, sizeof(_bitwise_op_TM) / sizeof(int));
	if (1 == bitwise_op) { res &= seed; }
	else if (2 == bitwise_op) { res ^= seed; }
	else if (3 == bitwise_op) { res |= seed; }

	*((unsigned int*)iopKeyArray) = custom_bswap32(res);
	*oActualKeyArraySize = 4;
	return KGRE_Ok;
}

VKeyGenResultEx_enum GenerateKeyEx_DMxx(
	const unsigned char* ipSeedArray, unsigned int iSeedArraySize,
	unsigned char* iopKeyArray, unsigned int iMaxKeyArraySize,
	unsigned int* oActualKeyArraySize)
{
	if (iSeedArraySize != 4) return KGRE_BadDll;
	if (iMaxKeyArraySize < 4) return KGRE_BufferToSmall;

	unsigned int seed = custom_bswap32(*((unsigned int*)ipSeedArray));

	unsigned int res;
	unsigned int rot_num = _field_val(seed, _rot_num_DMxx, sizeof(_rot_num_DMxx) / sizeof(int));
	if (_bit_val(seed, _rot_dir_DMxx)) { rot_num = (32 - rot_num); }
	res = seed << rot_num;
	res = (res & 0xFFFFFFFF) | (seed >> (32 - rot_num));

	unsigned int inv_type = _field_val(seed, _inv_type_DMxx, sizeof(_inv_type_DMxx) / sizeof(int));
	if (inv_type > 0)
	{
		for (unsigned int i = 0; i < 3; i++)
		{
			res ^= (1 << _inv_bits_DMxx[inv_type - 1][i]);
		}
	}

	unsigned int bitwise_op = _field_val(seed, _bitwise_op_DMxx, sizeof(_bitwise_op_DMxx) / sizeof(int));
	if (1 == bitwise_op) { res &= seed; }
	else if (2 == bitwise_op) { res ^= seed; }
	else if (3 == bitwise_op) { res |= seed; }

	*((unsigned int*)iopKeyArray) = custom_bswap32(res);
	*oActualKeyArraySize = 4;
	return KGRE_Ok;
}

VKeyGenResultEx_enum GenerateKeyEx_BCM(
	const unsigned char* ipSeedArray, unsigned int iSeedArraySize,
	unsigned char* iopKeyArray, unsigned int iMaxKeyArraySize,
	unsigned int* oActualKeyArraySize)
{
	if (iSeedArraySize != 4) return KGRE_BadDll;
	if (iMaxKeyArraySize < 4) return KGRE_BufferToSmall;

	unsigned int seed = custom_bswap32(*((unsigned int*)ipSeedArray));

	unsigned int res;
	unsigned int rot_num = _field_val(seed, _rot_num_BCM, sizeof(_rot_num_BCM) / sizeof(int));
	if (_bit_val(seed, _rot_dir_BCM)) { rot_num = (32 - rot_num); }
	res = seed << rot_num;
	res = (res & 0xFFFFFFFF) | (seed >> (32 - rot_num));

	unsigned int inv_type = _field_val(seed, _inv_type_BCM, sizeof(_inv_type_BCM) / sizeof(int));
	if (inv_type > 0)
	{
		for (unsigned int i = 0; i < 3; i++)
		{
			res ^= (1 << _inv_bits_BCM[inv_type - 1][i]);
		}
	}

	unsigned int bitwise_op = _field_val(seed, _bitwise_op_BCM, sizeof(_bitwise_op_BCM) / sizeof(int));
	if (1 == bitwise_op) { res &= seed; }
	else if (2 == bitwise_op) { res ^= seed; }
	else if (3 == bitwise_op) { res |= seed; }

	*((unsigned int*)iopKeyArray) = custom_bswap32(res);
	*oActualKeyArraySize = 4;
	return KGRE_Ok;
}

VKeyGenResultEx_enum GenerateKeyEx_SMXX(
	const unsigned char* ipSeedArray, unsigned int iSeedArraySize,
	unsigned char* iopKeyArray, unsigned int iMaxKeyArraySize,
	unsigned int* oActualKeyArraySize)
{
	if (iSeedArraySize != 4) return KGRE_BadDll;
	if (iMaxKeyArraySize < 4) return KGRE_BufferToSmall;

	unsigned int seed = custom_bswap32(*((unsigned int*)ipSeedArray));

	unsigned int res;
	unsigned int rot_num = _field_val(seed, _rot_num_SMXX, sizeof(_rot_num_SMXX) / sizeof(int));
	if (_bit_val(seed, _rot_dir_SMXX)) { rot_num = (32 - rot_num); }
	res = seed << rot_num;
	res = (res & 0xFFFFFFFF) | (seed >> (32 - rot_num));

	unsigned int inv_type = _field_val(seed, _inv_type_SMXX, sizeof(_inv_type_SMXX) / sizeof(int));
	if (inv_type > 0)
	{
		for (unsigned int i = 0; i < 3; i++)
		{
			res ^= (1 << _inv_bits_SMXX[inv_type - 1][i]);
		}
	}

	unsigned int bitwise_op = _field_val(seed, _bitwise_op_SMXX, sizeof(_bitwise_op_SMXX) / sizeof(int));
	if (1 == bitwise_op) { res &= seed; }
	else if (2 == bitwise_op) { res ^= seed; }
	else if (3 == bitwise_op) { res |= seed; }

	*((unsigned int*)iopKeyArray) = custom_bswap32(res);
	*oActualKeyArraySize = 4;
	return KGRE_Ok;
}

QString Can::eraseApp(QString typeBlock)
{
	int idSend = 0;
	int idReceive;
	int idReceiveRef = 0;

	int msgSend[8] = { 0x31, 0x31, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00 };
	int msgReceive[8];
	QTime timeWork = QTime::currentTime().addMSecs(500); // Âðåìÿ äëÿ àâàðèéíîãî âûõîäà èç öèêëà.


	if (typeBlock == "DMFL_NAMI")
	{
		idSend = DIAG_ID_TO_DMFL;
		idReceiveRef = DIAG_ID_FROM_DMFL;
	}
	else if (typeBlock == "DMFR_NAMI")
	{
		idSend = DIAG_ID_TO_DMFR;
		idReceiveRef = DIAG_ID_FROM_DMFR;
	}
	else if (typeBlock == "DMRL_NAMI")
	{
		idSend = DIAG_ID_TO_DMRL;
		idReceiveRef = DIAG_ID_FROM_DMRL;
	}
	else if (typeBlock == "DMRR_NAMI")
	{
		idSend = DIAG_ID_TO_DMRR;
		idReceiveRef = DIAG_ID_FROM_DMRR;
	}
	else if (typeBlock == "TM_NAMI")
	{
		idSend = DIAG_ID_TO_TM;
		idReceiveRef = DIAG_ID_FROM_TM;
	}
	else if (typeBlock == "BCM_NAMI")
	{
		idSend = DIAG_ID_TO_BCM;
		idReceiveRef = DIAG_ID_FROM_BCM;
	}
	else if (typeBlock == "SMFL_NAMI")
	{
		idSend = DIAG_ID_TO_SMFL;
		idReceiveRef = DIAG_ID_FROM_SMFL;
	}
	else if (typeBlock == "SMFR_NAMI")
	{
		idSend = DIAG_ID_TO_SMFR;
		idReceiveRef = DIAG_ID_FROM_SMFR;
	}
	else if (typeBlock == "SMRL_NAMI")
	{
		idSend = DIAG_ID_TO_SMRL;
		idReceiveRef = DIAG_ID_FROM_SMRL;
	}
	else if (typeBlock == "SMRR_NAMI")
	{
		idSend = DIAG_ID_TO_SMRR;
		idReceiveRef = DIAG_ID_FROM_SMRR;
	}

	//wakeBoot->start(500); // Çàïóñêàåì òàéìåð, êîòîðûé íå âûïóñêàåò áëîê èç boot
	writeCan(0x55, msgSend); // ïåðåõîäèì â boot
	Sleep(50);


	int msgSend_ProgrammingSession[8] = { 0x02, 0x10, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00 }; // ïåðåõîäèì â programming session
	writeCan(idSend, msgSend_ProgrammingSession);
	while (true) // Æä¸ì îòâåòà î ïåðåõîäå â programming session
	{
		if (readWaitCan(&idReceive, msgReceive, 20))
			if (idReceive == idReceiveRef && msgReceive[0] == 0x06 && msgReceive[1] == 0x50 && msgReceive[2] == 0x02 && msgReceive[3] == 0x00 && msgReceive[4] == 0x32 && msgReceive[5] == 0x01 && msgReceive[6] == 0xF4 && msgReceive[7] == 0xAA)
				break;

		if (QTime::currentTime() > timeWork)
			return QString("No response about switching to programming session"); // âûõîä ñ îøèáêîé
	}



	int msgSend_RequestSeedKey[8] = { 0x02, 0x27, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00 }; // çàïðàøèâàåì â Request seed key
	writeCan(idSend, msgSend_RequestSeedKey);
	while (true) // Æä¸ì êëþ÷ îò boot
	{
		if (readWaitCan(&idReceive, msgReceive, 20))
			if (idReceive == idReceiveRef && msgReceive[0] == 0x06 && msgReceive[1] == 0x67 && msgReceive[2] == 0x01) // Ïîëó÷èëè êëþ÷
			{
				int sendMsgKey[8] = { 0x06, 0x27, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00 };
				unsigned char* ipSeedArray = new unsigned char[4];
				ipSeedArray[0] = msgReceive[3];
				ipSeedArray[1] = msgReceive[4];
				ipSeedArray[2] = msgReceive[5];
				ipSeedArray[3] = msgReceive[6];
				unsigned char* keyAnswer = new unsigned char[4];
				unsigned int size;
				if (typeBlock == "BCM_NAMI")
					GenerateKeyEx_BCM(ipSeedArray, 4, keyAnswer, 4, &size);
				else if (typeBlock == "TM_NAMI")
					GenerateKeyEx_TM(ipSeedArray, 4, keyAnswer, 4, &size);
				else if (typeBlock == "SMXX_NAMI")
					GenerateKeyEx_SMXX(ipSeedArray, 4, keyAnswer, 4, &size);
				else
					GenerateKeyEx_DMxx(ipSeedArray, 4, keyAnswer, 4, &size);

				sendMsgKey[3] = keyAnswer[0];
				sendMsgKey[4] = keyAnswer[1];
				sendMsgKey[5] = keyAnswer[2];
				sendMsgKey[6] = keyAnswer[3];

				writeCan(idSend, sendMsgKey); // îòïðàâëÿåì êëþ÷

				break;
			}
		if (QTime::currentTime() > timeWork)
			return QString("No answer with key");  // âûõîä ñ îøèáêîé
	}
	while (true) // Æä¸ì ïîäòâåðæäåíèå î âàëèäíîñòè êëþ÷à
	{
		if (readWaitCan(&idReceive, msgReceive, 20))
			if (idReceive == idReceiveRef && msgReceive[0] == 0x02 && msgReceive[1] == 0x67 && msgReceive[2] == 0x02)
				break;
		if (QTime::currentTime() > timeWork)
			return QString("Didn't receive a response about the validity of the key");  // âûõîä ñ îøèáêîé
	}


	int msgSend_EraseMemory_1[8] = { 0x10, 0x0D, 0x31, 0x01, 0xFF, 0x00, 0x44, 0x80 }; // Äåëàåì óäàëåíèå ïàìÿòè
	writeCan(idSend, msgSend_EraseMemory_1); 

	while (true) // æä¸ì îòâåò
	{
		if (readWaitCan(&idReceive, msgReceive, 20))
			if (idReceive == idReceiveRef && msgReceive[0] == 0x30 && msgReceive[1] == 0x00 && msgReceive[2] == 0x01)
				break;
		if (QTime::currentTime() > timeWork)
			return QString("Didn't receive a response about the validity of the key");
	}
	if (typeBlock == "BCM_NAMI")
	{
		int msgSend_EraseMemory_2[8] = { 0x21, 0x08, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00 }; // Äåëàåì óäàëåíèå ïàìÿòè
		writeCan(idSend, msgSend_EraseMemory_2);
	}
	else
	{
		int msgSend_EraseMemory_2[8] = { 0x21, 0x08, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00 }; // Äåëàåì óäàëåíèå ïàìÿòè
		writeCan(idSend, msgSend_EraseMemory_2);
	}

		return QString("GOOD");
}


