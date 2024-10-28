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
	mainTableWidget->setHorizontalHeaderLabels(QStringList() << QString::fromLocal8Bit("Разъём")
															<< QString::fromLocal8Bit("Пин")
															<< QString::fromLocal8Bit("Название")
															<< QString::fromLocal8Bit("Тип")
															<< ""
															<< QString::fromLocal8Bit("Статус")
															<< QString::fromLocal8Bit("Подробнее"));
}

void TestWindow::on_fullTestManualStandSortButton_clicked()
{

}