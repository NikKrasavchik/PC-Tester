#pragma once

//#define DEBUG_CAN

#include <vector>
#include "qobject.h"
#include "qtimer.h"
#include "qdebug.h"
#include "QTime"
#include "qstring.h"

#include "canlib.h"
#include "chai.h"
#include "Cable.h"

#define ID_CAN_AUTOSTAND		0x51
#define ID_CAN_MANUALSTAND		0x100

#define TIME_CHECKCONNECTION	200

class Can : public QObject
{
	Q_OBJECT
public:
	Can();

	// ------------------------------------
	// Name: initCan
	// Variables: 
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
	//			true  - � ���	��� ���� can ������ ���������������.
	// ------------------------------------
	bool deinitCan();

	// ------------------------------------
	// Name: setSelectedAdapterNeme
	// Variables: 
	//			QString adapter - ��� �������� ������� ����� ������ � �������� ��������.
	//			�������� ����������� ������ �������������� ������ ���� �������� ����������� �� ������ getNameAdapters().
	// ------------------------------------
	void setSelectedAdapterNeme(QString adapter);

	// ------------------------------------
	// Name: setSelectedFrequency
	//		��������� ��������� ������� ��� ������ can
	// Variables: 
	//			Qstring frequency: ���������� �������.
	// ------------------------------------
	void setSelectedFrequency(QString frequency);

	// ------------------------------------
	// Name: getStatusAdapterSelected
	//		����������� ������� ������ ��������.
	// Return: bool
	//			true: ������� ������.
	//			false: ������� �� ������.
	// ------------------------------------
	bool getStatusAdapterSelected() { return b_adapterSelected; }

	// ------------------------------------
	// Name: getStatusFrequencySelected
	//		����������� ������� ������ �������.
	// Return: bool
	//			true: ������� �������
	//			false: ������� �� �������.
	// ------------------------------------
	bool getStatusFrequencySelected() { return b_frequencySelected; }

	// ------------------------------------
	// Name: getNameAdapters
	// Return: std::vector<QString>
	//			������ �������� Can-��������� ������� ���������� � �� � �� ����� � ���� �������.
	//			����������:
	//		    � Kvaser ���� ����� � ������� �� ����������. Kvaser ������� ��������� �� ��. ��������� ���������. 
	//		    ���������� �������. �� ���� �� ������ �������� (��� ��� ������ marathon), �� ��� ��� �� ����������.
	// ------------------------------------
	std::vector<QString> getNameAdapters();

	// ------------------------------------
	// Name: sendTestMsg
	// Variables: 
	//			ConnectorId pad - enum ����������� � ���� ������������� ������� (A - 1; B - 2; C - 3; C - 4; ...).
	//			int pin - ����� ���� � �������.
	//			int type - b������������ ������������ ������ ���� ���.
	// Return: bool
	//			false - � ������ ���� type == NOT_SET, ��� ������ ��������� ��������.	
	//			true  - � ������ ���� ��������� �����������.
	// ------------------------------------
	static bool sendTestMsg(ConnectorId pad, int pin, TypeCable type, TestBlockName nameBlock);

	// ------------------------------------
	// Name: sendTestMsg
	//		�������� ��������� �� can
	// Variables: 
	//			ConnectorId pad: ��������� ������������� ������
	//			int pin: ��� ������������� ������
	//			int digValue: �������� �������� ������������� ������
	//			int pwmValue: ��� �������� ������������� ������
	// ------------------------------------
	static void sendTestMsg(ConnectorId pad, int pin, int digValue, int pwmValue);


private:
	// ------------------------------------
	// Name: writeCan
	// Variables:
	//			int* id - ��������� �� ���������� � ������� ��������� id �� �������� ����������� can-���������.
	//			int* msg - ��������� �� ���������� � ������� ����������� � can.
	// Return: bool
	//			false - � ������ ���� b_adapterSelected == false, ��� ������ ��������� ��������.	
	//			true  - � ������ ���� can-��������� �����������.
	// ------------------------------------
	static bool writeCan(int id, int* msg);

	// ------------------------------------
	// Name: readWaitCan
	// Variables:
	//			int* id - ��������� �� ���������� � ������� ���������� id ���������� can-���������.
	//			int* msg - ��������� �� ���������� � ������� ���������� ��������� ��������� �� can.
	//			int  timeout - ����� � ������������� ������� �� ����� ����� ��������� �� can-����.
	// Return: bool
	//			false - � ������ ���� �� ����� timeout �� ������ ��������� �� can-����.
	//			true  - � ������ ���� �� ����� timeout ������ ��������� �� can-����.
	// ------------------------------------
	static bool readWaitCan(int* id, int* msg, int timeout);

	std::pair<int, int> conversionFrequency(int frequency, int modelAdapter);
	
// Variables:
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
	void Signal_AfterTest(int connector, int pin, std::vector<Measureds*> measureds);
};

