#include "twoThread.h"

AutoStandTwoThread::AutoStandTwoThread(StandStatusFlags* statusFlags)
{
	this->can = can;

	this->statusFlags = statusFlags;
	this->statusFlags->StatusConnected = false;
	this->statusFlags->StatusTest = false;

	nowTesting.pad = 0;
	nowTesting.pin = 0;

	Can::initCan();
}

AutoStandTwoThread::~AutoStandTwoThread()
{
	int msgSendDisconect[8] = SEND_MSG_DISCONNECT;
	Can::writeCan(SEND_ID_CAN, msgSendDisconect);
	Can::deinitCan();
}

void AutoStandTwoThread::run()
{
	while (true)
	{
		while (!(statusFlags->StatusConnected)) // Отправляем сообщение пока не подключимся к стенду
		{
			int msgSendConnect[8] = SEND_MSG_CONNECT_AUTO_STAND;
			Can::writeCan(SEND_ID_CAN, msgSendConnect);

			int msgReceiveConnect[8];
			int id = -2;
			if (Can::readWaitCan(&id, msgReceiveConnect, 100))
			{
				if (id == RECEIVE_ID_CAN_AUTO_STAND &&
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
			if (Can::readWaitCan(&id, msgReceive, 10))
			{
				if (id == RECEIVE_ID_CAN_AUTO_STAND && // переодическое сообшение о конекте
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
				if (id == RECEIVE_ID_CAN_AUTO_STAND && // Пришло сообщение о завершение теста 
					msgReceive[0] == nowTesting.pad &&
					msgReceive[1] == nowTesting.pin)
				{
					msgToTestWindowAfterTest_AutoTwoThread(nowTesting.pad, nowTesting.pin, msgReceive[2], msgReceive[3]);
					nowTesting.pad = 0;
					nowTesting.pin = 0;
					statusFlags->StatusTest = false;
				}
			}
			if (std::chrono::system_clock::now() - timeStartSentConnect > std::chrono::milliseconds(100)) // Отправлять каждые 100 мс сообшение о конекте
			{
				int msgSendConnect[8] = SEND_MSG_CONNECT_PERIODICALLY;
				Can::writeCan(SEND_ID_CAN, msgSendConnect);

				timeStartSentConnect = std::chrono::system_clock::now();
			}

			if (std::chrono::system_clock::now() - timeStartReceiveConnect > std::chrono::milliseconds(150)) // Не приходило сообщение о конекте от стенда больше 150 мс
			{
				statusFlags->StatusConnected = false;
				msgToTestWindowStatusConnect_AutoTwoThread(statusFlags->StatusConnected);
				break;
			}
		}
	}
}

void AutoStandTwoThread::msgToTwoThreadStartTest_AutoTwoThread(int pad, int pin)
{
	if (statusFlags->StatusConnected && !statusFlags->StatusTest)
	{
		statusFlags->StatusTest = true;

		nowTesting.pad = pad;
		nowTesting.pin = pin;



		int msgSendConnect[8] = { pad, pin, 0, 0, 0, 0, 0, 0 };
		Can::writeCan(SEND_ID_CAN, msgSendConnect);
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
ManualStandTwoThread::ManualStandTwoThread(std::vector<Cable> cables, StandStatusFlags* statusFlags)
{


	this->statusFlags = statusFlags;
	this->statusFlags->StatusConnected = false;
	this->statusFlags->StatusTest = false;

	for (int i = 0; i < cables.size(); i++)
	{
		TestCables* cable = new TestCables;

		cable->pad = (int)cables[i].connector;
		cable->pin = cables[i].pin;
		cable->canId = cables[i].canId;
		cable->byte = cables[i].bit;

		cable->oldValue = NOT_SET;
		this->cables.push_back(cable);
	}

}

ManualStandTwoThread::~ManualStandTwoThread()
{
	for (int i = 0; i < cables.size(); i++)
	{
		int msgSendConnect[8] = { cables[i]->pad, cables[i]->pin, 0, 0, 0, 0, 0, 0 };
		Can::writeCan(SEND_ID_CAN, msgSendConnect);
	}
	for (int i = 0; i < cables.size(); i++)
		delete cables[i];
	Can::deinitCan();
}

void ManualStandTwoThread::run()
{
	Can::initCan();
	Sleep(50);
	std::chrono::system_clock::time_point timeStartSentConnect = std::chrono::system_clock::now();
	while (true)
	{
		int msgReceive[8];
		int id = -2;
		if (Can::readWaitCan(&id, msgReceive, 10))
		{
			if (id == RECEIVE_ID_CAN_CONNECT_MANUAL_STAND) // сообшение о конекте
			{
				timeStartSentConnect = std::chrono::system_clock::now();
				if (!statusFlags->StatusConnected)
				{
					statusFlags->StatusConnected = true;
					msgToTestWindowStatusConnect_ManualTwoThread(true);
				}
			}

			if (!statusFlags->StatusConnected)
				continue;

			for (int i = 0; i < cables.size(); i++)
			{
				TestCables* cable = cables[i];
				if (cable->canId == id && cable->oldValue != msgReceive[cable->byte])
				{
					cable->oldValue = msgReceive[cable->byte];
					
					msgToTestWindowChangeValue_ManualTwoThread(cable->pad, cable->pin, cable->oldValue);
				}
			}
		}
		if (std::chrono::system_clock::now() - timeStartSentConnect > std::chrono::milliseconds(300) && statusFlags->StatusConnected) // Если стенд не отвечает 100 мс
		{
			statusFlags->StatusConnected = false;
			msgToTestWindowStatusConnect_ManualTwoThread(false);
		}
	}
}

void ManualStandTwoThread::msgToTwoThreadStartTest_ManualTwoThread(int pad, int pin, int digValue, int pwmValue)
{
	if (statusFlags->StatusConnected)
	{
		//msgToTestWindowBeforeTest_AutoTwoThread(nowTesting.pad, nowTesting.pin);

		int msgSendConnect[8] = { pad, pin, digValue, pwmValue, 0, 0, 0, 0 };
		Can::writeCan(SEND_ID_CAN, msgSendConnect);
	}
}