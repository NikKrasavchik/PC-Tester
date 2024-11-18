#include <qthread.h>
#include <chrono>

#include "can.h"

#define SEND_ID_CAN 0x50
//#define SEND_MSG_CONNECT(msg) {msg[0] = 0xAA, msg[1] = 0x0, msg[2] = 0xAA, msg[3] = 0x0, msg[4] = 0xAA, msg[5] = 0x0, msg[6] = 0xAA, msg[7] = 0x0}
#define SEND_MSG_CONNECT				{0xAA, 0x0, 0xAA, 0x0, 0xAA, 0x0, 0xAA, 0x0, }
#define SEND_MSG_CONNECT_PERIODICALLY	{0xAA, 0x0, 0xAA, 0x0, 0xAA, 0x0, 0xAA, 0xFF, }

#define RECEIVE_ID_CAN 0x51

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

	explicit AutoStandTwoThread(Can* can);

	//AutoStandTwoThread(Can* can);

	void run();

public slots:
	void msgToTwoThreadStartTest(int pad, int pin, int digValue, int pwmValue);

signals:
	void msgToTestWindowStatusConnect(bool statusConnect);

private:

	Can* can;

	bool standConected;
};
