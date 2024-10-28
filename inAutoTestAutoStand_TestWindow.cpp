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
	mainTableWidget->setHorizontalHeaderLabels(QStringList() << QString::fromLocal8Bit("Разъём")
															<< QString::fromLocal8Bit("Пин")
															<< QString::fromLocal8Bit("Название")
															<< QString::fromLocal8Bit("Тип")
															<< QString::fromLocal8Bit("Стенд")
															<< QString::fromLocal8Bit("ПК")
															<< QString::fromLocal8Bit("Подробнее"));
}

void TestWindow::on_inAutoTestAutoStandConnectButton_clicked()
{

}

void TestWindow::on_inAutoTestAutoStandStartTestButton_clicked()
{

}