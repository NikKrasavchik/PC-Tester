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

	void initCan();
	void deinitCan();

	void writeCan(int id, int* msg);
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
	bool readWaitCan(int* id, int* msg, int timeout);
	//bool readWaitCan(int* id, std::vector<int>* msg, int timeout);

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
