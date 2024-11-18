#include <qthread.h>
#include <chrono>

#include "can.h"

#define SEND_ID_CAN 0x50
//#define SEND_MSG_CONNECT(msg) {msg[0] = 0xAA, msg[1] = 0x0, msg[2] = 0xAA, msg[3] = 0x0, msg[4] = 0xAA, msg[5] = 0x0, msg[6] = 0xAA, msg[7] = 0x0}
#define SEND_MSG_CONNECT				{0xAA, 0x0, 0xAA, 0x0, 0xAA, 0x0, 0xAA, 0x0, }
#define SEND_MSG_CONNECT_PERIODICALLY	{0xAA, 0x0, 0xAA, 0x0, 0xAA, 0x0, 0xAA, 0xFF, }

#define RECEIVE_ID_CAN 0x51

class TestWindow;

class autoStandTwoThread : public QThread
{
	Q_OBJECT

public:

	explicit autoStandTwoThread(Can* can);

	//autoStandTwoThread(Can* can);

	void run();

private:

	Can* can;

	bool standConected;
};
