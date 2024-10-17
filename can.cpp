#include "can.h"

Can::Can()
{
}

std::vector<QString> Can::getNameAdapters()
{
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
		resultVector.push_back(str);
	}
	canClose(hndTmp); // Завершение работы с api

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
			resultVector.push_back(strNameAdapter);
		}
	}

	return resultVector;
}