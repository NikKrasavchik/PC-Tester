#include "TestWindow.h"

#define COLOUMN_COUNT		8

#define COLOUMN_COMPONENT	3
#define COLOUMN_DIRECTION	4
#define COLOUMN_TYPE		5
#define COLOUMN_CHECK		6
#define COLOUMN_STATUS		7

void TestWindow::initUiFullTestManualStand()
{
	fullTestManualStandConnectButton = new QPushButton(usefulSpaceWidget);
	fullTestManualStandConnectButton->setObjectName("fullTestManualStandConnectButton");
	fullTestManualStandConnectButton->setFixedSize(QSize(FIXED_HEADER_BUTTON_WIDTH, FIXED_HEADER_BUTTON_HEIGHT));
	usefulSpaceHLayout->addWidget(fullTestManualStandConnectButton);

	fullTestSortButton = new QPushButton(usefulSpaceWidget);
	fullTestSortButton->setObjectName("fullTestSortButton");
	fullTestSortButton->setFixedSize(QSize(FIXED_HEADER_BUTTON_WIDTH, FIXED_HEADER_BUTTON_HEIGHT));
	usefulSpaceHLayout->addWidget(fullTestSortButton);

	fullTestSortType = SORT_TYPE_INDEX;

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
	
	resetTableHeaderLanguageFullTestManualStand();
	 
	mainTableWidget->setColumnWidth(COLOUMN_CONNECTOR,	COLOUMN_CONNECTOR_WIDTH);
	mainTableWidget->setColumnWidth(COLOUMN_PIN,		COLOUMN_PIN_WIDTH);
	mainTableWidget->setColumnWidth(COLOUMN_COMPONENT,	COLOUMN_COMPONENT_WIDTH);
	mainTableWidget->setColumnWidth(COLOUMN_DIRECTION,	COLOUMN_DIRECTION_WIDTH);
	mainTableWidget->setColumnWidth(COLOUMN_TYPE,		COLOUMN_TYPE_WIDTH);
	mainTableWidget->setColumnWidth(COLOUMN_CHECK,		COLOUMN_CHECK_WIDTH);
	mainTableWidget->setColumnWidth(COLOUMN_STATUS,		COLOUMN_STATUS_WIDTH);

	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLOUMN_CONNECTOR,	QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLOUMN_PIN,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLOUMN_NAME,			QHeaderView::Stretch);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLOUMN_COMPONENT,	QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLOUMN_DIRECTION,	QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLOUMN_TYPE,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLOUMN_CHECK,		QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLOUMN_STATUS,		QHeaderView::Fixed);
}

void TestWindow::resetTableHeaderLanguageFullTestManualStand()
{
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		delete mainTableHeaderLabels;
		mainTableHeaderLabels = new QStringList();
		mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Разъём"));
		mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Пин"));
		mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Название"));
		mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Компонент"));
		mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Направление"));
		mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Тип"));
		mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Проверка"));
		mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Статус"));
		mainTableHeaderLabels->push_back("");
		break;

	case ENGLISH_LANG:
		delete mainTableHeaderLabels;
		mainTableHeaderLabels = new QStringList();
		mainTableHeaderLabels->push_back("Connector");
		mainTableHeaderLabels->push_back("Pin");
		mainTableHeaderLabels->push_back("Name");
		mainTableHeaderLabels->push_back("Component");
		mainTableHeaderLabels->push_back("Direction");
		mainTableHeaderLabels->push_back("Type");
		mainTableHeaderLabels->push_back("Check");
		mainTableHeaderLabels->push_back("Status");
		mainTableHeaderLabels->push_back("");
		break;
	}
	mainTableWidget->setHorizontalHeaderLabels(*mainTableHeaderLabels);
}

void TestWindow::resetTableRowsFullTestManualStand()
{
	QAbstractItemModel* model = mainTableWidget->model();
	for (int currentRowNum = 0; currentRowNum < cableRows.size(); currentRowNum++)
	{
		model->setData(model->index(currentRowNum, COLOUMN_CONNECTOR), cableRows[currentRowNum]->connector);
		model->setData(model->index(currentRowNum, COLOUMN_PIN), cableRows[currentRowNum]->pin);
		model->setData(model->index(currentRowNum, COLOUMN_NAME), cableRows[currentRowNum]->name);
		model->setData(model->index(currentRowNum, COLOUMN_COMPONENT), cableRows[currentRowNum]->component);
		model->setData(model->index(currentRowNum, COLOUMN_DIRECTION), cableRows[currentRowNum]->direction);
		model->setData(model->index(currentRowNum, COLOUMN_TYPE), cableRows[currentRowNum]->type);

		QWidget* interactionButtonsWidget = new QWidget(mainLayoutWidget);
		initTableRowButtons(currentRowNum, interactionButtonsWidget);
		mainTableWidget->setCellWidget(currentRowNum, COLOUMN_CHECK, interactionButtonsWidget);
	}
}

