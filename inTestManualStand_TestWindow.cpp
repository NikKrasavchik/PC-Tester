#include "TestWindow.h"

#define COLOUMN_COUNT		6

#define COLOUMN_COMPONENT	3
#define COLOUMN_TYPE		4
#define COLOUMN_STATUS		5

void TestWindow::initUiInTestManualStand()
{
	inTestManualStandConnectButton = new QPushButton(usefulSpaceWidget);
	inTestManualStandConnectButton->setObjectName("inTestManualStandConnectButton");
	inTestManualStandConnectButton->setFixedSize(QSize(FIXED_HEADER_BUTTON_WIDTH, FIXED_HEADER_BUTTON_HEIGHT));
	usefulSpaceHLayout->addWidget(inTestManualStandConnectButton);

	initUiTableInTestManualStand();
}

void TestWindow::initUiTableInTestManualStand()
{
	initUiTableHeaderInTestManualStand();
	initUiTableRowsInTestManualStand();
}

void TestWindow::initUiTableHeaderInTestManualStand()
{
	mainTableWidget->setRowCount(cableRows.size());
	mainTableWidget->setColumnCount(COLOUMN_COUNT);

	resetTableHeaderLanguageInTestManualStand();

	mainTableWidget->setColumnWidth(COLOUMN_CONNECTOR,	COLOUMN_CONNECTOR_WIDTH);
	mainTableWidget->setColumnWidth(COLOUMN_PIN,		COLOUMN_PIN_WIDTH);
	mainTableWidget->setColumnWidth(COLOUMN_COMPONENT,	COLOUMN_COMPONENT_WIDTH);
	mainTableWidget->setColumnWidth(COLOUMN_TYPE,		COLOUMN_TYPE_WIDTH);
	mainTableWidget->setColumnWidth(COLOUMN_STATUS,		COLOUMN_STATUS_WIDTH);

	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLOUMN_CONNECTOR,	QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLOUMN_PIN,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLOUMN_NAME,			QHeaderView::Stretch);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLOUMN_COMPONENT,	QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLOUMN_TYPE,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLOUMN_STATUS,		QHeaderView::Fixed);
}

void TestWindow::resetTableHeaderLanguageInTestManualStand()
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
		mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Тип"));
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
		mainTableHeaderLabels->push_back("Type");
		mainTableHeaderLabels->push_back("Status");
		mainTableHeaderLabels->push_back("");
		break;
	}
	mainTableWidget->setHorizontalHeaderLabels(*mainTableHeaderLabels);
}

void TestWindow::initUiTableRowsInTestManualStand()
{
	QAbstractItemModel* model = mainTableWidget->model();
	for (int currentRowNum = 0; currentRowNum < cableRows.size(); currentRowNum++)
	{
		model->setData(model->index(currentRowNum, COLOUMN_CONNECTOR), cableRows[currentRowNum]->connector);
		model->setData(model->index(currentRowNum, COLOUMN_PIN), cableRows[currentRowNum]->pin);
		model->setData(model->index(currentRowNum, COLOUMN_NAME), cableRows[currentRowNum]->name);
		model->setData(model->index(currentRowNum, COLOUMN_COMPONENT), cableRows[currentRowNum]->component);
		model->setData(model->index(currentRowNum, COLOUMN_TYPE), cableRows[currentRowNum]->type);
	}
}
