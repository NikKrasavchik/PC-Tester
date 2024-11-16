#include "TestWindow.h"

void TestWindow::initUiFullTestAutoStand()
{
	fullTestAutoStandConnectButton = new QPushButton(usefulSpaceWidget);
	fullTestAutoStandConnectButton->setObjectName("fullTestAutoStandConnectButton");
	fullTestAutoStandConnectButton->setFixedSize(QSize(FIXED_HEADER_BUTTON_WIDTH, FIXED_HEADER_BUTTON_HEIGHT));
	usefulSpaceHLayout->addWidget(fullTestAutoStandConnectButton);

	fullTestAutoStandStartTestButton = new QPushButton(usefulSpaceWidget);
	fullTestAutoStandStartTestButton->setObjectName("fullTestAutoStandStartTestButton");
	fullTestAutoStandStartTestButton->setFixedSize(QSize(FIXED_HEADER_BUTTON_WIDTH, FIXED_HEADER_BUTTON_HEIGHT));
	usefulSpaceHLayout->addWidget(fullTestAutoStandStartTestButton);

	fullTestAutoStandSortButton = new QPushButton(usefulSpaceWidget);
	fullTestAutoStandSortButton->setObjectName("fullTestAutoStandSortButton");
	fullTestAutoStandSortButton->setFixedSize(QSize(FIXED_HEADER_BUTTON_WIDTH, FIXED_HEADER_BUTTON_HEIGHT));
	usefulSpaceHLayout->addWidget(fullTestAutoStandSortButton);

	initUiTableFullTestAutoStand();
}

void TestWindow::initUiTableFullTestAutoStand()
{
	mainTableWidget->setColumnCount(8);
	mainTableWidget->setHorizontalHeaderLabels(QStringList() << QString::fromLocal8Bit("������")
		<< QString::fromLocal8Bit("���")
		<< QString::fromLocal8Bit("��������")
		<< QString::fromLocal8Bit("��������������") // ???
		<< QString::fromLocal8Bit("���")
		<< QString::fromLocal8Bit("�����")
		<< QString::fromLocal8Bit("��")
		<< QString::fromLocal8Bit("���������"));
}

void TestWindow::on_fullTestAutoStandConnectButton_clicked()
{
}

void TestWindow::on_fullTestAutoStandStartTestButton_clicked()
{
}

void TestWindow::on_fullTestAutoStandSortButton_clicked()
{
}