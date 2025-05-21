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
	//	WindowType windowType - enum хранищий в себе идентификатор опрделяющий какое окно сейчас открыто.
	// Return: bool
	//	false - в случае если b_adapterSelected == false, или ошибку драйверов адаптера.	
	//	true  - в случае если can прошёл инициализацию.
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

	static QString getSelectedAdapterNeme();

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
	//			true: Частота выбрана.
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
	static std::vector<QString> getNameAdapters();

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
	//			Отправка сообщения на can
	// Variables: 
	//			ConnectorId pad: Коннектор отправляемого кабеля.
	//			int pin: Пин отправляемого кабеля.
	//			int digValue: Цифровое значение отправляемого кабеля.
	//			int pwmValue: ШИМ значение отправляемого кабеля.
	// ------------------------------------
	static void sendTestMsg(ConnectorId pad, int pin, int digValue, int pwmValue);

	// Отправляет сообщение блоку о засыпании или просыпании. 
	// @name sendGoToSleepMsg
	// 
	// @param bool isGoToSleep == true - команда на засыпание.
	// @param bool isGoToSleep == false - команда на пробуждение.
	// 
	// @return void
	static void sendGoToSleepMsg(bool isGoToSleep);

	static QString getDiagBlock(DiagInformation diagInf, TestBlockName blockName);


	void setCable(std::vector<Cable> cable);

	// Отчишает старые значения кабеля. После вызова сработает большое количество сигналов Signal_ChangedByte.
	// @name clearOldValue
	// 
	// @param void
	// 
	// @return void
	static void clearOldValue();

	// Метод для проверки не основных CAN канов
	// @name checkInformationBus_Can
	// 
	// @param QString checkAdapter - Название адаптера по которому будет производится проверка.
	// @param int canId - Can id по которому отправится тест сообщение
	// 
	// @return bool true - can исправен / false - не пришло сообщение или оно неправильное.
	static bool checkInformationBus_Can(QString checkAdapter, int canId);

	// Метод для проверки LIN канов
	// @name checkInformationBus_Lin
	// 
	// @param QString checkAdapter - Название адаптера по которому будет производится проверка.
	// @param int canId - Lin id по мы как slave будем отвечать.
	// 
	// @return bool true - can исправен / false - не пришло сообщение или оно неправильное.
	static bool checkInformationBus_Lin(QString checkAdapter, int canId);
	
	
	static void checkInformationBus(int canId);
private:
	// Отправляет сообщение в CAN
	// @name writeCan
	// 
	// @param int id - id по которому отправиться can-сообщения
	// @param int* msg - указатель на переменную которая отправиться в can
	// 
	// @return bool - В случае удачой отправки возращает tru
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

	
	WindowType windowType; // Переменная хранящая идентификатор окна которое сейчас открыто
	std::vector<Measureds*> measureds;
	uint8_t counterConnectMsg; 

	static QMap<int, std::vector<std::pair<Cable, int>>> mapCable; // Мапа в которой ключ это id can, а значение массив пар. Пара состоит из кабеля и значение которое у него было до этого значения. Такая сложная структура необходима для более оптимального поиска кабеля при приходе сообщения.

	bool b_adapterSelected; 
	bool b_frequencySelected;
	static bool b_flagStatusConnection;		// Флаг показывающий присоединён ли Stand

	QTimer* timerReadCan;					// Таймер для считывания Can-сообщений.
	QTimer* timerSendConnectMsg;			// Таймер для отправки сообщений на первичное подключение или подтверждение подключения.
	QTimer* timerCheckStandConnection;		// Таймер для проверки времени времени прихода переодического сообщения конекта.

private slots:
	void Timer_ReadCan();					// Слот для считывания Can-сообщений.
	void Timer_SendConnectMsg();			// Слот для отправки сообщений на подключение или подтверждения подключения.
	void Timer_CheckStandConnection();		// Слот для проверки времени прихода переодического сообщения конекта.

signals:

	void Signal_ChangedStatusStandConnect(bool statusConnect); // Сигнал который говорит что статус присоеденения к стенду изменён 
	void Signal_AfterTest(int connector, int pin, std::vector<Measureds*> measureds); // Сигнал означающий завершение теста у автостенда
	void Signal_ChangedByte(int idCable, int newValue); // Сигнал срабатывающий когда поменялся байт у какого либо кабеля из mapCable
	void Signal_changeStatusCheckInformationBus(int id, bool status);

};

