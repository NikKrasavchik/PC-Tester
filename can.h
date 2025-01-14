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
	//			int* id - ��������� �� ���������� � ������� ���������� id ���������� can-���������.
	//			int* msg - ��������� �� ���������� � ������� ���������� ��������� ��������� �� can.
	//			int  timeout - ����� � ������������� ������� �� ����� ����� ��������� �� can-����.
	// Return: bool
	//			false - � ������ ���� �� ����� timeout �� ������ ��������� �� can-����.
	//			true  - � ������ ���� �� ����� timeout ������ ��������� �� can-����.
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
