#pragma once

#define DEBUG_CAN

#include <vector>
#include "qobject.h"
#include "qtimer.h"
#include "qdebug.h"
#include "QTime"

#include "qstring.h"
#include "canlib.h"
#include "chai.h"
//#include "Components.h"
#include "Cable.h"

#define ID_CAN_AUTOSTAND 0x51
#define ID_CAN_MANUALSTAND 0x100

class Can : public QObject
{
	Q_OBJECT
public:
	Can();

	// ------------------------------------
	// Name: initCan
	// Varibals: 
	//			WindowType windowType - enum �������� � ���� ������������� ����������� ����� ���� ������ �������.
	// Return: bool
	//			false - � ������ ���� b_adapterSelected == false, ��� ������ ��������� ��������.	
	//			true  - � ������ ���� can ������ �������������.
	// ------------------------------------
	bool initCan(WindowType windowType);
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

	// ------------------------------------
	// Name: sendTestMsg
	// Varibals: 
	//			ConnectorId pad - enum ����������� � ���� ������������� ������� (A - 1; B - 2; C - 3; C - 4; ...).
	//			int pin - ����� ���� � �������.
	//			int type - b������������ ������������ ������ ���� ���.
	// Return: bool
	//			false - � ������ ���� type == NOT_SET, ��� ������ ��������� ��������.	
	//			true  - � ������ ���� ��������� �����������.
	// ------------------------------------
	static bool sendTestMsg(ConnectorId pad, int pin, TypeCable type, NameTestingBlock nameBlock);
	static void sendTestMsg(ConnectorId pad, int pin, int digValue, int pwmValue);


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
	//uint8_t generateFlags(int typeCable, NameTestingBlock nameBlock);

// Varibals:
	struct modelAdapter
	{
		std::vector<QString> nameAdapters;
		int activeAdapter;
		std::pair<int, int> p_frequency;
	};
	static modelAdapter *kvaser;
	static modelAdapter *marathon;
	static canHandle hnd;

	WindowType windowType;
	std::vector<Measureds*> measureds;
	uint8_t counterConnectMsg;

	bool b_adapterSelected;
	bool b_frequencySelected;
	bool b_flagStandConnectionCheck;
	bool b_flagStatusConnection;		// ���� ������������ ���������� �� Stand

	QTimer* timerReadCan;				// ������ ��� ���������� Can-���������.
	QTimer* timerSendConnectMsg;		// ������ ��� �������� ��������� �� ����������� ��� �������� �����������.
	QTimer* timerCheckStandConnection;	// ������ ��� �������� ������� ������� ������� �������������� ��������� �������.


private slots:
	void Timer_ReadCan();				// ���� ��� ���������� Can-���������.
	void Timer_SendConnectMsg();		// ���� ��� �������� ��������� �� ����������� ��� �������� �����������.
	void Timer_CheckStandConnection();	// ���� ��� �������� ������� ������� ������� �������������� ��������� �������.

signals:
	void Signal_ChangedStatusStandConnect(bool statusConnect);
	void Signal_AfterTest(int connector, int pin, std::vector<Measureds*> measureds, double voltage, double current);


};

