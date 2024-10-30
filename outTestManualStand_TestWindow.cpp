#include "TestWindow.h"

void TestWindow::initUiOutTestManualStand()
{
	initUiTableOutTestManualStand();
}

void TestWindow::initUiTableOutTestManualStand()
{
	mainTableWidget->setColumnCount(8);
	mainTableWidget->setHorizontalHeaderLabels(QStringList() << QString::fromLocal8Bit("Разъём") 
															<< QString::fromLocal8Bit("Пин") 
															<< QString::fromLocal8Bit("Название") 
															<< QString::fromLocal8Bit("Тип") 
															<< "" 
															<< QString::fromLocal8Bit("Отправка (hex)")
															<< ""
															<< QString::fromLocal8Bit("Подробнее"));
}