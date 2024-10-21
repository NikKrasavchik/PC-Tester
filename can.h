#pragma once

#include <vector>

#include "qstring.h"
#include "canlib.h"
#include "chai.h"

#define KVASER		0
#define MARATHON	1

class Can
{
public:
	Can();

	void initCan();
	void deinitCan();

	void sendCan(int id, int* msg);
	void receiveCan();

	void setAdapterNeme(QString adapter);
	void setFrequency(QString frequency);

	bool getAdapterSelected() { return b_adapterSelected; }
	bool getFrequencySelected() { return b_frequencySelected; }

	std::vector<QString> getNameAdapters();

private:

	canHandle hnd;

	std::pair<int, int> conversionFrequency(int frequency, int modelAdapter);

	bool b_adapterSelected;
	bool b_frequencySelected;

	struct modelAdapter
	{
		std::vector<QString> nameAdapters;
		int activeAdapter;
		std::pair<int, int> p_frequency;
	};

	modelAdapter* kvaser;
	modelAdapter* marathon;
};
