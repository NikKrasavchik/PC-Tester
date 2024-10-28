#include "TestWindow.h"

void TestWindow::initUiInTestManualStand()
{
	initUiTableInTestManualStand();
}

void TestWindow::initUiTableInTestManualStand()
{
	mainTableWidget->setColumnCount(6);
	mainTableWidget->setHorizontalHeaderLabels(QStringList() << QString::fromLocal8Bit("������")
															<< QString::fromLocal8Bit("���")
															<< QString::fromLocal8Bit("��������")
															<< QString::fromLocal8Bit("���")
															<< QString::fromLocal8Bit("������")
															<< QString::fromLocal8Bit("���������"));
}