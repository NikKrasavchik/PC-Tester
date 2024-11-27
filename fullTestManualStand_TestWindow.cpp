#include "TestWindow.h"

#define COLOUMN_COUNT		7

#define COLOUMN_DIRECTION	3
#define COLOUMN_TYPE		4
#define COLOUMN_CHECK		5
#define COLOUMN_STATUS		6

void TestWindow::initUiFullTestManualStand()
{
	fullTestManualStandConnectButton = new QPushButton(usefulSpaceWidget);
	fullTestManualStandConnectButton->setObjectName("fullTestManualStandConnectButton");
	fullTestManualStandConnectButton->setFixedSize(QSize(FIXED_HEADER_BUTTON_WIDTH, FIXED_HEADER_BUTTON_HEIGHT));
	usefulSpaceHLayout->addWidget(fullTestManualStandConnectButton);

	fullTestManualStandSortButton = new QPushButton(usefulSpaceWidget);
	fullTestManualStandSortButton->setObjectName("fullTestManualStandSortButton");
	fullTestManualStandSortButton->setFixedSize(QSize(FIXED_HEADER_BUTTON_WIDTH, FIXED_HEADER_BUTTON_HEIGHT));
	usefulSpaceHLayout->addWidget(fullTestManualStandSortButton);

	fullTestManualStandSortType = SORT_TYPE_INDEX;

	initUiTableFullTestManualStand();
}

void TestWindow::initUiTableFullTestManualStand()
{
	resetTableHeaderFullTestManualStand();
	resetTableRowsFullTestManualStand();
}

void TestWindow::resetTableHeaderFullTestManualStand()
{
	mainTableWidget->setRowCount(cableRows.size());
	mainTableWidget->setColumnCount(COLOUMN_COUNT);
	mainTableWidget->setHorizontalHeaderLabels(QStringList() 
		<< QString::fromLocal8Bit("Разъём")
		<< QString::fromLocal8Bit("Пин")
		<< QString::fromLocal8Bit("Название")
		<< QString::fromLocal8Bit("Направление") // ???
		<< QString::fromLocal8Bit("Тип")
		<< QString::fromLocal8Bit("Проверка")
		<< QString::fromLocal8Bit("Статус")
		<< "");
	 
	mainTableWidget->setColumnWidth(COLOUMN_CONNECTOR,	COLOUMN_CONNECTOR_WIDTH);
	mainTableWidget->setColumnWidth(COLOUMN_PIN,		COLOUMN_PIN_WIDTH);
	mainTableWidget->setColumnWidth(COLOUMN_DIRECTION,	COLOUMN_DIRECTION_WIDTH);
	mainTableWidget->setColumnWidth(COLOUMN_TYPE,		COLOUMN_TYPE_WIDTH);
	mainTableWidget->setColumnWidth(COLOUMN_CHECK,		COLOUMN_CHECK_WIDTH);
	mainTableWidget->setColumnWidth(COLOUMN_STATUS,		COLOUMN_STATUS_WIDTH);

	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLOUMN_CONNECTOR,	QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLOUMN_PIN,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLOUMN_NAME,			QHeaderView::Stretch);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLOUMN_DIRECTION,	QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLOUMN_TYPE,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLOUMN_CHECK,		QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLOUMN_STATUS,		QHeaderView::Fixed);
}

void TestWindow::resetTableRowsFullTestManualStand()
{
	QAbstractItemModel* model = mainTableWidget->model();
	for (int currentRowNum = 0; currentRowNum < cableRows.size(); currentRowNum++)
	{
		model->setData(model->index(currentRowNum, COLOUMN_CONNECTOR), cableRows[currentRowNum]->connector);
		model->setData(model->index(currentRowNum, COLOUMN_PIN), cableRows[currentRowNum]->pin);
		model->setData(model->index(currentRowNum, COLOUMN_NAME), cableRows[currentRowNum]->name);
		model->setData(model->index(currentRowNum, COLOUMN_DIRECTION), cableRows[currentRowNum]->direction);
		model->setData(model->index(currentRowNum, COLOUMN_TYPE), cableRows[currentRowNum]->type);

		QWidget* interactionButtonsWidget = new QWidget(mainLayoutWidget);
		initTableRowButtons(currentRowNum, interactionButtonsWidget);
		mainTableWidget->setCellWidget(currentRowNum, COLOUMN_CHECK, interactionButtonsWidget);
	}
}

static void rewriteCableRows(std::vector<TestTableRowProperties*>* cableRows, int sortType)
{
	std::vector<TestTableRowProperties*> tmpCableRows(*cableRows);

	switch (sortType)
	{
	case SORT_TYPE_INDEX:
		for (int i = 0; i < cableRows->size(); i++)
		{
			bool flag = true;
			for (int j = 0; j < cableRows->size() - (i + 1); j++) {
				if ((*cableRows)[j]->id > (*cableRows)[j + 1]->id) {
					flag = false;
					std::swap((*cableRows)[j], (*cableRows)[j + 1]);
				}
			}
			if (flag) {
				break;
			}
		}
		break;

	case SORT_TYPE_DIRECTION_OUT:
		cableRows->clear();
		for (int i = 0; i < tmpCableRows.size(); i++)
			if (tmpCableRows[i]->direction == "OUT")
			{
				cableRows->push_back(new TestTableRowProperties());
				(*cableRows)[cableRows->size() - 1] = tmpCableRows[i];
			}
		for (int i = 0; i < tmpCableRows.size(); i++)
			if (tmpCableRows[i]->direction == "IN")
			{
				cableRows->push_back(new TestTableRowProperties());
				(*cableRows)[cableRows->size() - 1] = tmpCableRows[i];
			}
		break;

	case SORT_TYPE_DIRECTION_IN:
		cableRows->clear();
		for (int i = 0; i < tmpCableRows.size(); i++)
			if (tmpCableRows[i]->direction == "IN")
			{
				cableRows->push_back(new TestTableRowProperties());
				(*cableRows)[cableRows->size() - 1] = tmpCableRows[i];
			}
		for (int i = 0; i < tmpCableRows.size(); i++)
			if (tmpCableRows[i]->direction == "OUT")
			{
				cableRows->push_back(new TestTableRowProperties());
				(*cableRows)[cableRows->size() - 1] = tmpCableRows[i];
			}
		break;
	}
}

void TestWindow::on_fullTestManualStandSortButton_clicked()
{
	switch (fullTestManualStandSortType)
	{
	case SORT_TYPE_INDEX:
		fullTestManualStandSortType = SORT_TYPE_DIRECTION_OUT;

		switch (viewWindowState->appLanguage)
		{
		case RUSSIAN_LANG:
			fullTestManualStandSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо выходам"));
			break;

		case ENGLISH_LANG:
			fullTestManualStandSortButton->setText(QString("Sort:\nout first"));
			break;
		}
		break;

	case SORT_TYPE_DIRECTION_OUT:
		fullTestManualStandSortType = SORT_TYPE_DIRECTION_IN;

		switch (viewWindowState->appLanguage)
		{
		case RUSSIAN_LANG:
			fullTestManualStandSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо входам"));
			break;

		case ENGLISH_LANG:
			fullTestManualStandSortButton->setText(QString("Sort:\nin first"));
			break;
		}
		break;

	case SORT_TYPE_DIRECTION_IN:
		fullTestManualStandSortType = SORT_TYPE_INDEX;

		switch (viewWindowState->appLanguage)
		{
		case RUSSIAN_LANG:
			fullTestManualStandSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо нумерации"));
			break;

		case ENGLISH_LANG:
			fullTestManualStandSortButton->setText(QString("Sort:\nnum"));
			break;
		}
		break;
	}

	rewriteCableRows(&cableRows, fullTestManualStandSortType);

	mainTableWidget->clear();
	resetTableHeaderFullTestManualStand();
	resetTableRowsFullTestManualStand();

}