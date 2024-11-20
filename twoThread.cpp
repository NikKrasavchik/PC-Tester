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
			//int msgSendConnect[8] = SEND_MSG_CONNECT(msgSendConnect);
			int msgSendConnect[8] = SEND_MSG_CONNECT;
			can->writeCan(SEND_ID_CAN, msgSendConnect);

			//std::vector<int> msgReceiveConnect;
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
					msgToTestWindowStatusConnect(statusFlags->StatusConnected, statusFlags->StatusTest);
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
					nowTesting.pad = 0;
					nowTesting.pin = 0;
					statusFlags->StatusTest = false;
					// сигнал никите о том что тест завершился
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
				msgToTestWindowStatusConnect(statusFlags->StatusConnected, statusFlags->StatusTest);
				break;
			}
		}
	}
}

void AutoStandTwoThread::msgToTwoThreadStartTest(int pad, int pin, int digValue, int pwmValue)
{
	if (statusFlags->StatusConnected && !statusFlags->StatusTest)
	{
		statusFlags->StatusTest = true;

		nowTesting.pad = pad;
		nowTesting.pin = pin;
		msgToTestWindowStatusConnect(statusFlags->StatusConnected, statusFlags->StatusTest);

		int msgSendConnect[8] = { pad, pin, digValue, pwmValue, 0, 0, 0, 0 };
		can->writeCan(SEND_ID_CAN, msgSendConnect);
	}
}