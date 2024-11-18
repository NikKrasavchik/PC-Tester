#include "twoThread.h"

autoStandTwoThread::autoStandTwoThread(Can* can)
{
	this->can = can;
	standConected = false;
}

void autoStandTwoThread::run()
{
	can->initCan();
	while (true) // ���������� ��������� ���� �� ����������� � ������
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
				// ������ ������ � ��� ��� ����� ����������
				standConected = true;
				break;
			}
		}
	}

	std::chrono::system_clock::time_point timeStartSentConnect = std::chrono::system_clock::now();
	std::chrono::system_clock::time_point timeStartReceiveConnect = std::chrono::system_clock::now();

	while (true)
	{
		int id = -2;
		int msgReceiveConnect[8];
		if (can->readWaitCan(&id, msgReceiveConnect, 10))
		{
			if (id == RECEIVE_ID_CAN &&
				msgReceiveConnect[0] == 0x0 &&
				msgReceiveConnect[1] == 0xAA &&
				msgReceiveConnect[2] == 0x0 &&
				msgReceiveConnect[3] == 0xAA &&
				msgReceiveConnect[4] == 0x0 &&
				msgReceiveConnect[5] == 0xAA &&
				msgReceiveConnect[6] == 0x0 &&
				msgReceiveConnect[7] == 0xFF)
			{
				timeStartReceiveConnect = std::chrono::system_clock::now();
			}
		}

		if (std::chrono::system_clock::now() - timeStartSentConnect > std::chrono::milliseconds(100)) // ���������� ������ 100 �� ��������� � �������
		{
			int msgSendConnect[8] = SEND_MSG_CONNECT_PERIODICALLY;
			can->writeCan(SEND_ID_CAN, msgSendConnect);

			timeStartSentConnect = std::chrono::system_clock::now();
		}
		if (std::chrono::system_clock::now() - timeStartReceiveConnect > std::chrono::milliseconds(200))
		{
			//int msgSendConnect[8] = SEND_MSG_CONNECT_PERIODICALLY;
			//can->writeCan(SEND_ID_CAN, msgSendConnect);
			timeStartReceiveConnect = std::chrono::system_clock::now();
			break;
		}
	}
}