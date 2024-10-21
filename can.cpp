#include "can.h"

Can::Can()
{
	b_adapterSelected = false;
	b_frequencySelected = false;

	kvaser = new modelAdapter;
	marathon = new modelAdapter;
}

void Can::initCan()
{
	if (kvaser->activeAdapter != -1) // kvaser
	{
		canInitializeLibrary(); // Инициализация api kvaser
		hnd = canOpenChannel(0, canOPEN_ACCEPT_VIRTUAL); // Открытие канала связи по CAN.
		canSetBusParams(hnd, kvaser->p_frequency.first, 0, 0, 0, 0, 0); // Установка параматров на CAN-шину.
		canBusOn(hnd); // Запуск CAN-шины
	}
	else
	{
	}
}

void Can::deinitCan()
{
	if (kvaser->activeAdapter != -1) // kvaser
	{
		canBusOff(hnd);
		canClose(hnd);
	}
	else
	{
	}
}

void Can::sendCan(int id, int* msg)
{
	if (kvaser->activeAdapter != -1) // kvaser
	{
		canWrite(hnd, id, msg, 8, 0);
	}
	else
	{
	}
}

void Can::receiveCan()
{
}

void Can::setAdapterNeme(QString adapter)
{
	kvaser->activeAdapter = -1;
	marathon->activeAdapter = -1;

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

	if (kvaser->activeAdapter == -1 || marathon->activeAdapter == -1)
	{
		if (1)
		{
			1;
		}
	}
}

void Can::setFrequency(QString frequency)
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

	// marathon

	CiInit();

	for (int i = 0; i < 5; i++)
	{
		canboard_t binfo;
		QString strNameAdapter = "Marathon\n";

		binfo.brdnum = i;
		if (CiBoardInfo(&binfo) >= 0)
		{
			strNameAdapter += QString::fromStdString(binfo.name);
			marathon->nameAdapters.push_back(strNameAdapter);
			resultVector.push_back(strNameAdapter);
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