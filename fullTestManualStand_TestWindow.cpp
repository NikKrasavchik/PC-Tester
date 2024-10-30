#include "TestWindow.h"

void TestWindow::initUiFullTestManualStand()
{
	fullTestManualStandSortButton = new QPushButton(usefulSpaceWidget);
	fullTestManualStandSortButton->setObjectName("fullTestManualStandSortButton");
	fullTestManualStandSortButton->setFixedSize(QSize(FIXED_HEADER_BUTTON_WIDTH, FIXED_HEADER_BUTTON_HEIGHT));
	usefulSpaceHLayout->addWidget(fullTestManualStandSortButton);

	initUiTableFullTestManualStand();
}

void TestWindow::initUiTableFullTestManualStand()
{
	mainTableWidget->setColumnCount(7);
	mainTableWidget->setHorizontalHeaderLabels(QStringList() << QString::fromLocal8Bit("������")
															<< QString::fromLocal8Bit("���")
															<< QString::fromLocal8Bit("��������")
															<< QString::fromLocal8Bit("���")
															<< ""
															<< QString::fromLocal8Bit("������")
															<< QString::fromLocal8Bit("���������"));
}

void TestWindow::on_fullTestManualStandSortButton_clicked()
{

}