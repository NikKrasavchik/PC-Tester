#pragma once

#include <vector>
#include "qobject.h"
#include "qtimer.h"
#include "qdebug.h"
#include "QTime"

#include "qstring.h"
#include "canlib.h"
#include "chai.h"
#include "Components.h"

#define RECEIVE_ID_CAN_AUTO_STAND 0x51

class Can : public QObject
{
	Q_OBJECT
public:
	Can();

	// ------------------------------------
	// Name: initCan
	// Return: bool
	//			false - � ������ ���� b_adapterSelected == false, ��� ������ ��������� ��������.	
	//			true  - � ������ ���� can ������ �������������.
	// ------------------------------------
	bool initCan();
	// ------------------------------------
	// Name: deinitCan
	// Return: bool
	//			false - � ������ ���� b_adapterSelected == false, ��� ������ ��������� ��������.	
	//			true  - � ������ ���� can ������ ���������������.
	// ------------------------------------
	bool deinitCan();

	void setSelectedAdapterNeme(QString adapter);
	void setSelectedFrequency(QString frequency);

	bool getStatusAdapterSelected() { return b_adapterSelected; }
	bool getStatusFrequencySelected() { return b_frequencySelected; }

	std::vector<QString> getNameAdapters();

	static void sendTestMsg(int pad, int pin);
	static void sendTestMsg(int pad, int pin, int digValue, int pwmValue);


private:
	// ------------------------------------
	// Name: writeCan
	// Varibals:
	//			int* id - ��������� �� ���������� � ������� ��������� id �� �������� ����������� can-���������.
	//			int* msg - ��������� �� ���������� � ������� ����������� � can.
	// Return: bool
	//			false - � ������ ���� b_adapterSelected == false, ��� ������ ��������� ��������.	
	//			true  - � ������ ���� can-��������� �����������.
	// ------------------------------------
	static bool writeCan(int id, int* msg);
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

	std::pair<int, int> conversionFrequency(int frequency, int modelAdapter);

// Varibals:
	struct modelAdapter
	{
		std::vector<QString> nameAdapters;
		int activeAdapter;
		std::pair<int, int> p_frequency;
	};
	static modelAdapter *kvaser;
	static modelAdapter *marathon;

	bool b_adapterSelected;
	bool b_frequencySelected;
	bool b_flagStandConnectionCheck;
	bool b_flagIsChangedStandConnection;

	static canHandle hnd;
	QTimer* timerReadCan;
	QTimer* timerSendConnectMsg;
	QTimer* timerCheckStandConnection;


private slots:
	void Timer_ReadCan();
	void Timer_SendConnectMsg();
	void Timer_CheckStandConnection();

signals:
	void Signal_ChangedStatusStandConnect(bool statusConnect);
	void Signal_AfterTest(int connector, int pin, std::vector<Measured*> measureds, double voltage, double current);


};

