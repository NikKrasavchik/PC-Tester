#include "TestWindow.h"

void TestWindow::initUiOutTestManualStand()
{
	initUiTableOutTestManualStand();
}

void TestWindow::initUiTableOutTestManualStand()
{
	mainTableWidget->setColumnCount(8);
	mainTableWidget->setHorizontalHeaderLabels(QStringList() << QString::fromLocal8Bit("������") 
															<< QString::fromLocal8Bit("���") 
															<< QString::fromLocal8Bit("��������") 
															<< QString::fromLocal8Bit("���") 
															<< "" 
															<< QString::fromLocal8Bit("�������� (hex)")
															<< ""
															<< QString::fromLocal8Bit("���������"));
}