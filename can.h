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
	//			true  - в слу	чае если can прошёл деинициализацию.
	// ------------------------------------
	bool deinitCan();

	// ------------------------------------
	// Name: setSelectedAdapterNeme
	// Variables: 
	//			QString adapter - имя адаптера который будет выбран в качестве рабочего.
	//			Значения обезательно должно соответсвовать какому либо элементу полученному из метода getNameAdapters().
	// ------------------------------------
	void setSelectedAdapterNeme(QString adapter);

	// ------------------------------------
	// Name: setSelectedFrequency
	//		Установка выбранной частоты для работы can
	// Variables: 
	//			Qstring frequency: Выбираемая частота.
	// ------------------------------------
	void setSelectedFrequency(QString frequency);

	// ------------------------------------
	// Name: getStatusAdapterSelected
	//		Возвращение статуса выбора адаптера.
	// Return: bool
	//			true: Адаптер выбран.
	//			false: Адаптер не выбран.
	// ------------------------------------
	bool getStatusAdapterSelected() { return b_adapterSelected; }

	// ------------------------------------
	// Name: getStatusFrequencySelected
	//		Возврвщения статуса выбора статуса.
	// Return: bool
	//			true: Частота выбрана
	//			false: Частота не выбрана.
	// ------------------------------------
	bool getStatusFrequencySelected() { return b_frequencySelected; }

	// ------------------------------------
	// Name: getNameAdapters
	// Return: std::vector<QString>
	//			Массив названий Can-адаптеров которые подключены к ПК и мы можем с ними работат.
	//			Примечание:
	//		    У Kvaser есть косяк с которым не поборолись. Kvaser адаптер отключаем от ПК. Запускаем программу. 
	//		    Подключаем адаптер. По идее он должен появится (как это делает marathon), но так это не происходит.
	// ------------------------------------
	std::vector<QString> getNameAdapters();

	// ------------------------------------
	// Name: sendTestMsg
	// Variables: 
	//			ConnectorId pad - enum переедающий в себе идентификатор коложки (A - 1; B - 2; C - 3; C - 4; ...).
	//			int pin - номер пина в колодки.
	//			int type - bдентификатор показывающий какого типа пин.
	// Return: bool
	//			false - в случае если type == NOT_SET, или ошибку драйверов адаптера.	
	//			true  - в случае если сообщение отправилось.
	// ------------------------------------
	static bool sendTestMsg(ConnectorId pad, int pin, TypeCable type, TestBlockName nameBlock);

	// ------------------------------------
	// Name: sendTestMsg
	//		Отправка сообщения на can
	// Variables: 
	//			ConnectorId pad: Коннектор отправляемого кабеля
	//			int pin: Пин отправляемого кабеля
	//			int digValue: Цифровое значение отправляемого кабеля
	//			int pwmValue: ШИМ значение отправляемого кабеля
	// ------------------------------------
	static void sendTestMsg(ConnectorId pad, int pin, int digValue, int pwmValue);


private:
	// ------------------------------------
	// Name: writeCan
	// Variables:
	//			int* id - указатель на переменную в которой храниться id по которому отправиться can-сообщения.
	//			int* msg - указатель на переменную в которая отправиться в can.
	// Return: bool
	//			false - в случае если b_adapterSelected == false, или ошибку драйверов адаптера.	
	//			true  - в случае если can-сообщение отправленно.
	// ------------------------------------
	static bool writeCan(int id, int* msg);

	// ------------------------------------
	// Name: readWaitCan
	// Variables:
	//			int* id - указатель на переменную в которую запишеться id пришедшего can-сообщения.
	//			int* msg - указатель на переменную в которую запишеться сообщение пришедшее из can.
	//			int  timeout - время в миллисикундах сколько мы будем ждать сообщение из can-шыны.
	// Return: bool
	//			false - в случае если за время timeout не пришло сообщение на can-шину.
	//			true  - в случае если за время timeout пришло сообшение на can-шину.
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
	void Signal_AfterTest(int connector, int pin, std::vector<Measureds*> measureds);
};

