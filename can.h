#pragma once

#include <vector>

#include "qstring.h"
#include "canlib.h"
#include "chai.h"
#include "Components.h"

class Can
{
public:
	Can();

	static void initCan();
	static void deinitCan();

	static void writeCan(int id, int* msg);
	// ------------------------------------
	// Name: readWaitCan
	// Varibals:
	//			int* id - указатель на переменную в которую запишеться id пришедшего can-сообщения.
	//			int* msg - указатель на переменную в которую запишеться сообщение пришедшее из can.
	//			int  timeout - время в миллисикундах сколько мы будем ждать сообщение из can-шыны.
	// Return: bool
	//			false - в случае если за время timeout не пришло сообщение на can-шину.
	//			true  - в случае если за время timeout пришло сообшение на can-шину.
	// ------------------------------------
	static bool readWaitCan(int* id, int* msg, int timeout);

	void setAdapterNeme(QString adapter);
	void setFrequency(QString frequency);

	bool getAdapterSelected() { return b_adapterSelected; }
	bool getFrequencySelected() { return b_frequencySelected; }

	std::vector<QString> getNameAdapters();

private:

	struct modelAdapter
	{
		std::vector<QString> nameAdapters;
		int activeAdapter;
		std::pair<int, int> p_frequency;
	};
	static canHandle hnd;

	std::pair<int, int> conversionFrequency(int frequency, int modelAdapter);

	bool b_adapterSelected;
	bool b_frequencySelected;

	static modelAdapter *kvaser;
	static modelAdapter *marathon;
};
