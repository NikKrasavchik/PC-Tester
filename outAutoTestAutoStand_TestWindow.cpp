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
	mainTableWidget->setHorizontalHeaderLabels(QStringList() << QString::fromLocal8Bit("Разъём")
															<< QString::fromLocal8Bit("Пин")
															<< QString::fromLocal8Bit("Название")
															<< QString::fromLocal8Bit("Тип")
															<< QString::fromLocal8Bit("Стенд")
															<< QString::fromLocal8Bit("ПК")
															<< QString::fromLocal8Bit("Подробнее"));
}

void TestWindow::on_outAutoTestAutoStandConnectButton_clicked()
{

}

void TestWindow::on_outAutoTestAutoStandStartTestButton_clicked()
{

}