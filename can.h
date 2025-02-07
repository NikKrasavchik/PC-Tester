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
	//			WindowType windowType - enum хранищий в себе идентификатор опрделяющий какое окно сейчас открыто.
	// Return: bool
	//			false - в случае если b_adapterSelected == false, или ошибку драйверов адаптера.	
	//			true  - в случае если can прошёл инициализацию.
	// ------------------------------------
	bool initCan(WindowType windowType);
	// ------------------------------------
	// Name: deinitCan
	// Return: bool
	//			false - в случае если b_adapterSelected == false, или ошибку драйверов адаптера.	
	//			true  - в случае если can прошёл деинициализацию.
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
	//			ConnectorId pad - enum переедающий в себе идентификатор коложки (A - 1; B - 2; C - 3; C - 4; ...).
	//			int pin - номер пина в колодки.
	//			int type - bдентификатор показывающий какого типа пин.
	// Return: bool
	//			false - в случае если type == NOT_SET, или ошибку драйверов адаптера.	
	//			true  - в случае если сообщение отправилось.
	// ------------------------------------
	static bool sendTestMsg(ConnectorId pad, int pin, TypeCable type, NameTestingBlock nameBlock);
	static void sendTestMsg(ConnectorId pad, int pin, int digValue, int pwmValue);


private:
	// ------------------------------------
	// Name: writeCan
	// Varibals:
	//			int* id - указатель на переменную в которой храниться id по которому отправиться can-сообщения.
	//			int* msg - указатель на переменную в которая отправиться в can.
	// Return: bool
	//			false - в случае если b_adapterSelected == false, или ошибку драйверов адаптера.	
	//			true  - в случае если can-сообщение отправленно.
	// ------------------------------------
	static bool writeCan(int id, int* msg);
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
	bool b_flagStatusConnection;		// Флаг показывающий присоединён ли Stand

	QTimer* timerReadCan;				// Таймер для считывания Can-сообщений.
	QTimer* timerSendConnectMsg;		// Таймер для отправки сообщений на подключение или проверки подключения.
	QTimer* timerCheckStandConnection;	// Таймер для проверки времени времени прихода переодического сообщения конекта.


private slots:
	void Timer_ReadCan();				// Слот для считывания Can-сообщений.
	void Timer_SendConnectMsg();		// Слот для отправки сообщений на подключение или проверки подключения.
	void Timer_CheckStandConnection();	// Слот для проверки времени времени прихода переодического сообщения конекта.

signals:
	void Signal_ChangedStatusStandConnect(bool statusConnect);
	void Signal_AfterTest(int connector, int pin, std::vector<Measureds*> measureds, double voltage, double current);


};

