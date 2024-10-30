#include "TestWindow.h"

void TestWindow::initUiInTestManualStand()
{
	initUiTableInTestManualStand();
}

void TestWindow::initUiTableInTestManualStand()
{
	mainTableWidget->setColumnCount(6);
	mainTableWidget->setHorizontalHeaderLabels(QStringList() << QString::fromLocal8Bit("Разъём")
															<< QString::fromLocal8Bit("Пин")
															<< QString::fromLocal8Bit("Название")
															<< QString::fromLocal8Bit("Тип")
															<< QString::fromLocal8Bit("Статус")
															<< QString::fromLocal8Bit("Подробнее"));
}