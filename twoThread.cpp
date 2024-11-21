#include "twoThread.h"

AutoStandTwoThread::AutoStandTwoThread(Can* can, StandStatusFlags* statusFlags)
{
	this->can = can;

	this->statusFlags = statusFlags;
	this->statusFlags->StatusConnected = false;
	this->statusFlags->StatusTest = false;

	nowTesting.pad = 0;
	nowTesting.pin = 0;

	can->initCan();
}

AutoStandTwoThread::~AutoStandTwoThread()
{
	int msgSendDisconect[8] = SEND_MSG_DISCONNECT;
	can->writeCan(SEND_ID_CAN, msgSendDisconect);
	can->deinitCan();
}

void AutoStandTwoThread::run()
{
	while (true)
	{
		while (!(statusFlags->StatusConnected)) // Отправляем сообщение пока не подключимся к стенду
		{
			int msgSendConnect[8] = SEND_MSG_CONNECT;
			can->writeCan(SEND_ID_CAN, msgSendConnect);

			int msgReceiveConnect[8];
			int id = -2;
			if (can->readWaitCan(&id, msgReceiveConnect, 100))
			{
				if (id == RECEIVE_ID_CAN &&
					msgReceiveConnect[0] == 0x0 &&
					msgReceiveConnect[1] == 0xAA &&
					msgReceiveConnect[2] == 0x0 &&
					msgReceiveConnect[3] == 0xAA &&
					msgReceiveConnect[4] == 0x0 &&
					msgReceiveConnect[5] == 0xAA &&
					msgReceiveConnect[6] == 0x0 &&
					msgReceiveConnect[7] == 0xAA)
				{
					statusFlags->StatusConnected = true;
					msgToTestWindowStatusConnect_AutoTwoThread(statusFlags->StatusConnected);
				}
			}
		}

		std::chrono::system_clock::time_point timeStartSentConnect = std::chrono::system_clock::now();
		std::chrono::system_clock::time_point timeStartReceiveConnect = std::chrono::system_clock::now();

		while (statusFlags->StatusConnected)
		{
			int id = -2;
			int msgReceive[8];
			if (can->readWaitCan(&id, msgReceive, 10))
			{
				if (id == RECEIVE_ID_CAN && // переодическое сообшение о конекте
					msgReceive[0] == 0x0 &&
					msgReceive[1] == 0xAA &&
					msgReceive[2] == 0x0 &&
					msgReceive[3] == 0xAA &&
					msgReceive[4] == 0x0 &&
					msgReceive[5] == 0xAA &&
					msgReceive[6] == 0x0 &&
					msgReceive[7] == 0xFF)
				{
					timeStartReceiveConnect = std::chrono::system_clock::now();
				}
				if (id == RECEIVE_ID_CAN && // переодическое сообшение о конекте
					msgReceive[0] == nowTesting.pad &&
					msgReceive[1] == nowTesting.pin)
				{
					msgToTestWindowAfterTest_AutoTwoThread(nowTesting.pad, nowTesting.pin, 0.1, 0.1, 0x10);
					nowTesting.pad = 0;
					nowTesting.pin = 0;
					statusFlags->StatusTest = false;
				}
			}
			if (std::chrono::system_clock::now() - timeStartSentConnect > std::chrono::milliseconds(100)) // Отправлять каждые 100 мс сообшение о конекте
			{
				int msgSendConnect[8] = SEND_MSG_CONNECT_PERIODICALLY;
				can->writeCan(SEND_ID_CAN, msgSendConnect);

				timeStartSentConnect = std::chrono::system_clock::now();
			}

			if (std::chrono::system_clock::now() - timeStartReceiveConnect > std::chrono::milliseconds(150))
			{
				statusFlags->StatusConnected = false;
				msgToTestWindowStatusConnect_AutoTwoThread(statusFlags->StatusConnected);
				break;
			}
		}
	}
}

void AutoStandTwoThread::msgToTwoThreadStartTest_AutoTwoThread(int pad, int pin, int digValue, int pwmValue)
{
	if (statusFlags->StatusConnected && !statusFlags->StatusTest)
	{
		statusFlags->StatusTest = true;

		nowTesting.pad = pad;
		nowTesting.pin = pin;

		msgToTestWindowBeforeTest_AutoTwoThread(nowTesting.pad, nowTesting.pin);

		int msgSendConnect[8] = { pad, pin, digValue, pwmValue, 0, 0, 0, 0 };
		can->writeCan(SEND_ID_CAN, msgSendConnect);
	}
}

//
//
//
//
//
//
//
//
//
//
//
//
ManualStandTwoThread::ManualStandTwoThread(Can* can, std::vector<Cable> cables, StandStatusFlags* statusFlags)
{
	this->can = can;
	this->cables = cables;

	this->statusFlags = statusFlags;
	this->statusFlags->StatusConnected = false;
	this->statusFlags->StatusTest = false;

	can->initCan();
}

ManualStandTwoThread::~ManualStandTwoThread()
{
	can->deinitCan();
}

void ManualStandTwoThread::run()
{
	std::chrono::system_clock::time_point timeStartSentConnect = std::chrono::system_clock::now();
	while (true)
	{
		int msgReceiveConnect[8];
		int id = -2;
		if (can->readWaitCan(&id, msgReceiveConnect, 10))
		{
			if (id == 0x20) // сообшение о конекте
			{
				timeStartSentConnect = std::chrono::system_clock::now();
				if (!statusFlags->StatusConnected)
				{
					statusFlags->StatusConnected = true;
					msgToTestWindowStatusConnect_ManualTwoThread(true);
				}
			}
		}
		if (std::chrono::system_clock::now() - timeStartSentConnect > std::chrono::milliseconds(100) && statusFlags->StatusConnected) // Если стенд не отвечает 100 мс
		{
			statusFlags->StatusConnected = false;
			msgToTestWindowStatusConnect_ManualTwoThread(false);
		}
	}
}

void ManualStandTwoThread::msgToTwoThreadStartTest_ManualTwoThread(int pad, int pin, int digValue, int pwmValue)
{
	if (statusFlags->StatusConnected && !statusFlags->StatusTest)
	{
		statusFlags->StatusTest = true;

		//msgToTestWindowBeforeTest_AutoTwoThread(nowTesting.pad, nowTesting.pin);

		int msgSendConnect[8] = { pad, pin, digValue, pwmValue, 0, 0, 0, 0 };
		can->writeCan(SEND_ID_CAN, msgSendConnect);
	}
}