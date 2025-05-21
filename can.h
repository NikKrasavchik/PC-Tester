#pragma once

//#define DEBUG_CAN

#include "qmap.h"
#include <vector>
#include "qobject.h"
#include "qtimer.h"
#include "qdebug.h"
#include "QTime"
#include "qstring.h"

#include "canlib.h"
#include "linlib.h"
#include "chai.h"
#include "Cable.h"

// Diag
#define DIAG_ID_TO_BLOCK(NameBlock) (NameBlock == TestBlockName::DTM) ? DIAG_ID_TO_DMFL : DIAG_ID_TO_BCM
#define DIAG_ID_TO_DMFL 0x7A4
#define DIAG_ID_TO_BCM 0x7A3

#define DIAG_ID_FROM_BLOCK(NameBlock) (NameBlock == TestBlockName::DTM) ? DIAG_ID_FROM_DMFL : DIAG_ID_FROM_BCM
#define DIAG_ID_FROM_DMFL 0x7B4
#define DIAG_ID_FROM_BCM 0x7B3

#define DIAG_GET_APP_NAME(arr) arr[0] = 0x03; arr[1] = 0x22; arr[2] = 0xF1; arr[3] = 0x81; arr[4] = 0; arr[5] = 0; arr[6] = 0; arr[7] = 0;
#define DIAG_GET_APP_CALIBRATION(arr) arr[0] = 0x03; arr[1] = 0x22; arr[2] = 0xF1; arr[3] = 0x82; arr[4] = 0; arr[5] = 0; arr[6] = 0; arr[7] = 0;
#define DIAG_GET_EQUIPMENT_NAME(arr) arr[0] = 0x03; arr[1] = 0x22; arr[2] = 0xF1; arr[3] = 0x97; arr[4] = 0; arr[5] = 0; arr[6] = 0; arr[7] = 0;
#define DIAG_GET_DATA_MANUFACTURE(arr) arr[0] = 0x03; arr[1] = 0x22; arr[2] = 0xF1; arr[3] = 0x8B; arr[4] = 0; arr[5] = 0; arr[6] = 0; arr[7] = 0;
#define DIAG_GET_NUMBER_HARDWARE(arr) arr[0] = 0x03; arr[1] = 0x22; arr[2] = 0xF1; arr[3] = 0x92; arr[4] = 0; arr[5] = 0; arr[6] = 0; arr[7] = 0;
#define DIAG_GET_NUMBER_PART(arr) arr[0] = 0x03; arr[1] = 0x22; arr[2] = 0xF1; arr[3] = 0x87; arr[4] = 0; arr[5] = 0; arr[6] = 0; arr[7] = 0;
#define DIAG_GET_NUMBER_SERIAL(arr) arr[0] = 0x03; arr[1] = 0x22; arr[2] = 0xF1; arr[3] = 0x8C; arr[4] = 0; arr[5] = 0; arr[6] = 0; arr[7] = 0;

#define DIAG_VERIFICATION(arr) arr[0] = 0x30; arr[1] = 0x00; arr[2] = 0x05; arr[3] = 0; arr[4] = 0; arr[5] = 0; arr[6] = 0; arr[7] = 0;
//// Diag


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
	// Varibals: 
	//	WindowType windowType - enum �������� � ���� ������������� ����������� ����� ���� ������ �������.
	// Return: bool
	//	false - � ������ ���� b_adapterSelected == false, ��� ������ ��������� ��������.	
	//	true  - � ������ ���� can ������ �������������.
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

	static QString getSelectedAdapterNeme();

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
	//			true: ������� �������.
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
	static std::vector<QString> getNameAdapters();

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
	//			�������� ��������� �� can
	// Variables: 
	//			ConnectorId pad: ��������� ������������� ������.
	//			int pin: ��� ������������� ������.
	//			int digValue: �������� �������� ������������� ������.
	//			int pwmValue: ��� �������� ������������� ������.
	// ------------------------------------
	static void sendTestMsg(ConnectorId pad, int pin, int digValue, int pwmValue);

	// ���������� ��������� ����� � ��������� ��� ����������. 
	// @name sendGoToSleepMsg
	// 
	// @param bool isGoToSleep == true - ������� �� ���������.
	// @param bool isGoToSleep == false - ������� �� �����������.
	// 
	// @return void
	static void sendGoToSleepMsg(bool isGoToSleep);

	static QString getDiagBlock(DiagInformation diagInf, TestBlockName blockName);


	void setCable(std::vector<Cable> cable);

	// �������� ������ �������� ������. ����� ������ ��������� ������� ���������� �������� Signal_ChangedByte.
	// @name clearOldValue
	// 
	// @param void
	// 
	// @return void
	static void clearOldValue();

	// ����� ��� �������� �� �������� CAN �����
	// @name checkInformationBus_Can
	// 
	// @param QString checkAdapter - �������� �������� �� �������� ����� ������������ ��������.
	// @param int canId - Can id �� �������� ���������� ���� ���������
	// 
	// @return bool true - can �������� / false - �� ������ ��������� ��� ��� ������������.
	static bool checkInformationBus_Can(QString checkAdapter, int canId);

	// ����� ��� �������� LIN �����
	// @name checkInformationBus_Lin
	// 
	// @param QString checkAdapter - �������� �������� �� �������� ����� ������������ ��������.
	// @param int canId - Lin id �� �� ��� slave ����� ��������.
	// 
	// @return bool true - can �������� / false - �� ������ ��������� ��� ��� ������������.
	static bool checkInformationBus_Lin(QString checkAdapter, int canId);
	
	
	static void checkInformationBus(int canId);
private:
	// ���������� ��������� � CAN
	// @name writeCan
	// 
	// @param int id - id �� �������� ����������� can-���������
	// @param int* msg - ��������� �� ���������� ������� ����������� � can
	// 
	// @return bool - � ������ ������ �������� ��������� tru
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

//
// Varibals
//
	struct modelAdapter
	{
		std::vector<QString> nameAdapters;
		int activeAdapter;
		std::pair<int, int> p_frequency;
	};
	static modelAdapter *kvaser;
	static modelAdapter *marathon;
	static canHandle hnd;

	
	WindowType windowType; // ���������� �������� ������������� ���� ������� ������ �������
	std::vector<Measureds*> measureds;
	uint8_t counterConnectMsg; 

	static QMap<int, std::vector<std::pair<Cable, int>>> mapCable; // ���� � ������� ���� ��� id can, � �������� ������ ���. ���� ������� �� ������ � �������� ������� � ���� ���� �� ����� ��������. ����� ������� ��������� ���������� ��� ����� ������������ ������ ������ ��� ������� ���������.

	bool b_adapterSelected; 
	bool b_frequencySelected;
	static bool b_flagStatusConnection;		// ���� ������������ ���������� �� Stand

	QTimer* timerReadCan;					// ������ ��� ���������� Can-���������.
	QTimer* timerSendConnectMsg;			// ������ ��� �������� ��������� �� ��������� ����������� ��� ������������� �����������.
	QTimer* timerCheckStandConnection;		// ������ ��� �������� ������� ������� ������� �������������� ��������� �������.

private slots:
	void Timer_ReadCan();					// ���� ��� ���������� Can-���������.
	void Timer_SendConnectMsg();			// ���� ��� �������� ��������� �� ����������� ��� ������������� �����������.
	void Timer_CheckStandConnection();		// ���� ��� �������� ������� ������� �������������� ��������� �������.

signals:

	void Signal_ChangedStatusStandConnect(bool statusConnect); // ������ ������� ������� ��� ������ ������������� � ������ ������ 
	void Signal_AfterTest(int connector, int pin, std::vector<Measureds*> measureds); // ������ ���������� ���������� ����� � ����������
	void Signal_ChangedByte(int idCable, int newValue); // ������ ������������� ����� ��������� ���� � ������ ���� ������ �� mapCable
	void Signal_changeStatusCheckInformationBus(int id, bool status);

};

