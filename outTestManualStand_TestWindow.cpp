#include "TestWindow.h"

#define COLUMN_COUNT		7

#define COLUMN_COMPONENT	3
#define COLUMN_TYPE			4
#define COLUMN_CHECK		5
#define COLUMN_MANUAL_CHECK	6

void TestWindow::initUiOutTestManualStand()
{
	outTestManualStandConnectButton = new QPushButton(usefulSpaceWidget);
	outTestManualStandConnectButton->setObjectName("outTestManualStandConnectButton");
	outTestManualStandConnectButton->setFixedSize(QSize(FIXED_HEADER_BUTTON_WIDTH, FIXED_HEADER_BUTTON_HEIGHT));
	usefulSpaceHLayout->addWidget(outTestManualStandConnectButton);

	initUiTableOutTestManualStand();
}

void TestWindow::initUiTableOutTestManualStand()
{
	initUiTableHeaderOutTestManualStand();
	initUiTableRowsOutTestManualStand();
}

void TestWindow::initUiTableHeaderOutTestManualStand()
{
	mainTableWidget->setRowCount((int)cableRows.size());
	mainTableWidget->setColumnCount(COLUMN_COUNT);

	resetTableHeaderLanguageOutTestManualStand();

	mainTableWidget->setColumnWidth(COLUMN_CONNECTOR,		COLUMN_CONNECTOR_WIDTH);
	mainTableWidget->setColumnWidth(COLUMN_PIN,				COLUMN_PIN_WIDTH);
	mainTableWidget->setColumnWidth(COLUMN_COMPONENT,		COLUMN_COMPONENT_WIDTH);
	mainTableWidget->setColumnWidth(COLUMN_TYPE,			COLUMN_TYPE_WIDTH);
	mainTableWidget->setColumnWidth(COLUMN_CHECK,			COLUMN_CHECK_WIDTH);
	mainTableWidget->setColumnWidth(COLUMN_MANUAL_CHECK,	COLUMN_MANUAL_CHECK_WIDTH);

	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_CONNECTOR,		QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_PIN,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_NAME,			QHeaderView::Stretch);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_COMPONENT,		QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_TYPE,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_CHECK,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_MANUAL_CHECK,	QHeaderView::Fixed);

	QTableWidgetItem* protoitem = new QTableWidgetItem();
	protoitem->setTextAlignment(Qt::AlignCenter);
	for (int row = 0; row < mainTableWidget->rowCount(); row++)
	{
		mainTableWidget->setItem(row, COLUMN_CONNECTOR,		protoitem->clone());
		mainTableWidget->setItem(row, COLUMN_PIN,			protoitem->clone());
		mainTableWidget->setItem(row, COLUMN_COMPONENT,		protoitem->clone());
		mainTableWidget->setItem(row, COLUMN_TYPE,			protoitem->clone());
		mainTableWidget->setItem(row, COLUMN_MANUAL_CHECK,	protoitem->clone());
	}
}

void TestWindow::resetLanguageOutTestManualStand()
{
	resetTableHeaderLanguageOutTestManualStand();
	resetTableTypeLanguageOutTestManualStand();
	initUiTableRowsInTestManualStand();
}

void TestWindow::resetTableHeaderLanguageOutTestManualStand()
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
		mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Проверка"));
		mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Исправно"));
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
		mainTableHeaderLabels->push_back("Check");
		mainTableHeaderLabels->push_back("Correctly");
		mainTableHeaderLabels->push_back("");
		break;
	}
	mainTableWidget->setHorizontalHeaderLabels(*mainTableHeaderLabels);
}

void TestWindow::resetTableTypeLanguageOutTestManualStand()
{
	QAbstractItemModel* model = mainTableWidget->model();
	for (int currentRowNum = 0; currentRowNum < cableRows.size(); currentRowNum++)
	{
		switch (viewWindowState->appLanguage)
		{
		case RUSSIAN_LANG:
#ifdef QT5
			if (cableRows[currentRowNum]->typeStr == "DIGITAL")
				model->setData(model->index(currentRowNum, COLUMN_TYPE), QString::fromLocal8Bit("Цифровой"));
			else if (cableRows[currentRowNum]->typeStr == "PWM")
				model->setData(model->index(currentRowNum, COLUMN_TYPE), QString::fromLocal8Bit("ШИМ"));
			else if (cableRows[currentRowNum]->typeStr == "VNH")
				model->setData(model->index(currentRowNum, COLUMN_TYPE), QString::fromLocal8Bit("VNH"));
			else if (cableRows[currentRowNum]->typeStr == "ANALOG")
				model->setData(model->index(currentRowNum, COLUMN_TYPE), QString::fromLocal8Bit("Аналоговый"));
			else if (cableRows[currentRowNum]->typeStr == "HALL")
				model->setData(model->index(currentRowNum, COLUMN_TYPE), QString::fromLocal8Bit("HALL"));
			else if (cableRows[currentRowNum]->typeStr == "HLD")
				model->setData(model->index(currentRowNum, COLUMN_TYPE), QString::fromLocal8Bit("HLD"));
#elif QT6

#endif // QT5
			break;

		case ENGLISH_LANG:
			if (cableRows[currentRowNum]->typeStr == "DIGITAL")
				model->setData(model->index(currentRowNum, COLUMN_TYPE), "Digital");
			else if (cableRows[currentRowNum]->typeStr == "PWM")
				model->setData(model->index(currentRowNum, COLUMN_TYPE), "PWM");
			else if (cableRows[currentRowNum]->typeStr == "VNH")
				model->setData(model->index(currentRowNum, COLUMN_TYPE), "VNH");
			else if (cableRows[currentRowNum]->typeStr == "ANALOG")
				model->setData(model->index(currentRowNum, COLUMN_TYPE), "Analog");
			else if (cableRows[currentRowNum]->typeStr == "HALL")
				model->setData(model->index(currentRowNum, COLUMN_TYPE), "HALL");
			else if (cableRows[currentRowNum]->typeStr == "HLD")
				model->setData(model->index(currentRowNum, COLUMN_TYPE), "HLD");
			break;
		}
	}
}

void TestWindow::initUiTableRowsOutTestManualStand()
{
	QAbstractItemModel* model = mainTableWidget->model();
	for (int currentRowNum = 0; currentRowNum < cableRows.size(); currentRowNum++)
	{
		if (mainTableWidget->rowHeight(currentRowNum) < MIN_ROW_HEIGHT)
			mainTableWidget->setRowHeight(currentRowNum, MIN_ROW_HEIGHT);
		if (cableRows[currentRowNum]->connectorInt != ConnectorId::EMPTY)
			model->setData(model->index(currentRowNum, COLUMN_CONNECTOR), cableRows[currentRowNum]->connectorStr + "\nXP" + QString::number((int)cableRows[currentRowNum]->connectorInt));
		model->setData(model->index(currentRowNum, COLUMN_PIN),				cableRows[currentRowNum]->pin);
		model->setData(model->index(currentRowNum, COLUMN_NAME),			cableRows[currentRowNum]->name);
		model->setData(model->index(currentRowNum, COLUMN_COMPONENT),		cableRows[currentRowNum]->component);

		QWidget* interactionButtonsWidget = new QWidget(mainLayoutWidget);
		QWidget* manualChecksWidget = new QWidget(mainLayoutWidget);

		initTableRowButtons(currentRowNum, interactionButtonsWidget);
		initTableAdditionalManualChecks(currentRowNum, manualChecksWidget);

		mainTableWidget->setCellWidget(currentRowNum, COLUMN_CHECK, interactionButtonsWidget);
		mainTableWidget->setCellWidget(currentRowNum, COLUMN_MANUAL_CHECK, manualChecksWidget);
	}
	resetTableTypeLanguageOutTestManualStand();
}
