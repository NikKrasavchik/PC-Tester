#include "TestWindow.h"

void TestWindow::initUiInAutoTestAutoStand()
{
	inAutoTestAutoStandConnectButton = new QPushButton(usefulSpaceWidget);
	inAutoTestAutoStandConnectButton->setObjectName("inAutoTestAutoStandConnectButton");
	inAutoTestAutoStandConnectButton->setFixedSize(QSize(FIXED_HEADER_BUTTON_WIDTH, FIXED_HEADER_BUTTON_HEIGHT));
	usefulSpaceHLayout->addWidget(inAutoTestAutoStandConnectButton);

	inAutoTestAutoStandStartTestButton = new QPushButton(usefulSpaceWidget);
	inAutoTestAutoStandStartTestButton->setObjectName("inAutoTestAutoStandStartTestButton");
	inAutoTestAutoStandStartTestButton->setFixedSize(QSize(FIXED_HEADER_BUTTON_WIDTH, FIXED_HEADER_BUTTON_HEIGHT));
	usefulSpaceHLayout->addWidget(inAutoTestAutoStandStartTestButton);

	initUiTableInAutoTestAutoStand();
}

void TestWindow::initUiTableInAutoTestAutoStand()
{
	mainTableWidget->setColumnCount(7);
	mainTableWidget->setHorizontalHeaderLabels(QStringList() << QString::fromLocal8Bit("������")
															<< QString::fromLocal8Bit("���")
															<< QString::fromLocal8Bit("��������")
															<< QString::fromLocal8Bit("���")
															<< QString::fromLocal8Bit("�����")
															<< QString::fromLocal8Bit("��")
															<< QString::fromLocal8Bit("���������"));
}

void TestWindow::on_inAutoTestAutoStandConnectButton_clicked()
{

}

void TestWindow::on_inAutoTestAutoStandStartTestButton_clicked()
{

}