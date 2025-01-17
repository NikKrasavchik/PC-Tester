#include <qthread.h>
#include <chrono>

#include "can.h"

#define SEND_ID_CAN 0x55
#define SEND_MSG_CONNECT_AUTO_STAND				{0xAA, 0x0, 0xAA, 0x0, 0xAA, 0x0, 0xAA, 0x0, }
#define SEND_MSG_CONNECT_PERIODICALLY			{0xAA, 0x0, 0xAA, 0x0, 0xAA, 0x0, 0xAA, 0xFF, }
#define SEND_MSG_DISCONNECT						{0x0, 0xAA, 0x0, 0xAA, 0x0, 0xAA, 0x0, 0xAA, }

#define RECEIVE_MSG_CONNECT						{0x0, 0xAA, 0x0, 0xAA, 0x0, 0xAA, 0x0, 0xAA, }

#define RECEIVE_ID_CAN_AUTO_STAND 0x51
#define RECEIVE_ID_CAN_CONNECT_MANUAL_STAND		0x100

#define PAD_A 0
#define PAD_B 1
#define PAD_C 2
#define PAD_D 3

//#define SEND_MSG_CONNECT(msg) {msg[0] = 0xAA, msg[1] = 0x0, msg[2] = 0xAA, msg[3] = 0x0, msg[4] = 0xAA, msg[5] = 0x0, msg[6] = 0xAA, msg[7] = 0x0}
class TestWindow;

enum MsgType
{
	CONNECT,
	DISCONNECT,
	TEST
};

class AutoStandTwoThread : public QThread
{
	Q_OBJECT

public:

	explicit AutoStandTwoThread(StandStatusFlags* statusFlags);
	~AutoStandTwoThread();

	void run();

private:

	struct nowTesting
	{
		int pad;
		int pin;
	};

	nowTesting nowTesting;
	Can* can;
	StandStatusFlags* statusFlags;
public slots:
	void msgToTwoThreadStartTest_AutoTwoThread(int pad, int pin);

signals:
	void msgToTestWindowStatusConnect_AutoTwoThread(bool statusConnect);
	void msgToTestWindowAfterTest_AutoTwoThread(int pad, int pin, double voltage, double current);
};

class ManualStandTwoThread : public QThread
{
	Q_OBJECT

public:

	explicit ManualStandTwoThread(std::vector<Cable> cables, StandStatusFlags* statusFlags);
	~ManualStandTwoThread();

	void run();

private:

	struct TestCables
	{
		int pad;
		int pin;
		int canId;
		int byte;

		int oldValue;

		int direction;
		int type;
		double minCurrent;
		double maxCurrent;
		double minVoltage;
		double maxVoltage;
		QString name;
	};

	Can* can;
	std::vector<TestCables*> cables;

	StandStatusFlags* statusFlags;

public slots:
	void msgToTwoThreadStartTest_ManualTwoThread(int pad, int pin, int digValue, int pwmValue);

signals:
	void msgToTestWindowStatusConnect_ManualTwoThread(bool statusConnect);
	void msgToTestWindowChangeValue_ManualTwoThread(int pad, int pin, int newValue);
};
