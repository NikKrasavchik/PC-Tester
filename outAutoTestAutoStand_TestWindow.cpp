#include "TestWindow.h"

void TestWindow::initUiOutAutoTestAutoStand()
{
	outAutoTestAutoStandConnectButton = new QPushButton(usefulSpaceWidget);
	outAutoTestAutoStandConnectButton->setObjectName("outAutoTestAutoStandConnectButton");
	outAutoTestAutoStandConnectButton->setFixedSize(QSize(FIXED_HEADER_BUTTON_WIDTH, FIXED_HEADER_BUTTON_HEIGHT));
	usefulSpaceHLayout->addWidget(outAutoTestAutoStandConnectButton);

	outAutoTestAutoStandStartTestButton = new QPushButton(usefulSpaceWidget);
	outAutoTestAutoStandStartTestButton->setObjectName("outAutoTestAutoStandStartTestButton");
	outAutoTestAutoStandStartTestButton->setFixedSize(QSize(FIXED_HEADER_BUTTON_WIDTH, FIXED_HEADER_BUTTON_HEIGHT));
	usefulSpaceHLayout->addWidget(outAutoTestAutoStandStartTestButton);

	initUiTableOutAutoTestAutoStand();
}

void TestWindow::initUiTableOutAutoTestAutoStand()
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

void TestWindow::on_outAutoTestAutoStandConnectButton_clicked()
{

}

void TestWindow::on_outAutoTestAutoStandStartTestButton_clicked()
{

}