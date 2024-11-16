#include "TestWindow.h"

#define COLOUMN_DIRECTION	3
#define COLOUMN_TYPE		4
#define COLOUMN_MORE		7

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
	mainTableWidget->setRowCount(cableRows.size());
	mainTableWidget->setColumnCount(8);
	mainTableWidget->setHorizontalHeaderLabels(QStringList() << QString::fromLocal8Bit("Разъём")
															<< QString::fromLocal8Bit("Пин")
															<< QString::fromLocal8Bit("Название")
															<< QString::fromLocal8Bit("Направленность") // ???
															<< QString::fromLocal8Bit("Тип")
															<< ""
															<< QString::fromLocal8Bit("Статус")
															<< QString::fromLocal8Bit("Подробнее"));

	QAbstractItemModel* model = mainTableWidget->model();
	for (int currentRowNum = 0; currentRowNum < cableRows.size(); currentRowNum++)
	{
		model->setData(model->index(currentRowNum, COLOUMN_CONNECTOR), cableRows[currentRowNum]->connector);
		model->setData(model->index(currentRowNum, COLOUMN_PIN), cableRows[currentRowNum]->pin);
		model->setData(model->index(currentRowNum, COLOUMN_NAME), cableRows[currentRowNum]->name);
		model->setData(model->index(currentRowNum, COLOUMN_DIRECTION), cableRows[currentRowNum]->direction);
		model->setData(model->index(currentRowNum, COLOUMN_TYPE), cableRows[currentRowNum]->type);

		cableRows[currentRowNum]->moreButton = new QPushButton(mainLayoutWidget);
		cableRows[currentRowNum]->moreButton->setObjectName("moreButton");

		QWidget* moreCellWidget = new QWidget(mainLayoutWidget);
		moreCellWidget->setObjectName("deleteCellWidget");
		QHBoxLayout* moreCellLayout = new QHBoxLayout(moreCellWidget);
		moreCellLayout->setObjectName("deleteCellWidget");
		moreCellLayout->addWidget(cableRows[currentRowNum]->moreButton);
		moreCellLayout->setContentsMargins(0, 0, 0, 0);
		moreCellWidget->setLayout(moreCellLayout);

		mainTableWidget->setCellWidget(currentRowNum, COLOUMN_MORE, moreCellWidget);
	}
}

void TestWindow::on_fullTestManualStandSortButton_clicked()
{

}