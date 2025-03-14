#include "TestWindow.h"

#define COLUMN_COUNT			7

#define COLUMN_COMPONENT		3
#define COLUMN_TYPE				4
#define COLUMN_STATUS			5
#define COLUMN_MANUAL_CHECK		6

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
	mainTableWidget->setRowCount((int)cableRows.size());
	mainTableWidget->setColumnCount(COLUMN_COUNT);

	resetTableHeaderLanguageInTestManualStand();

	mainTableWidget->setColumnWidth(COLUMN_CONNECTOR,	COLUMN_CONNECTOR_WIDTH);
	mainTableWidget->setColumnWidth(COLUMN_PIN,			COLUMN_PIN_WIDTH);
	mainTableWidget->setColumnWidth(COLUMN_COMPONENT,	COLUMN_COMPONENT_WIDTH);
	mainTableWidget->setColumnWidth(COLUMN_TYPE,		COLUMN_TYPE_WIDTH);
	mainTableWidget->setColumnWidth(COLUMN_STATUS,		COLUMN_STATUS_WIDTH);
	mainTableWidget->setColumnWidth(COLUMN_MANUAL_CHECK, COLUMN_MANUAL_CHECK_WIDTH);

	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_CONNECTOR,		QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_PIN,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_NAME,			QHeaderView::Stretch);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_COMPONENT,		QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_TYPE,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_STATUS,		QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_MANUAL_CHECK,	QHeaderView::Fixed);

	QTableWidgetItem* protoitem = new QTableWidgetItem();
	protoitem->setTextAlignment(Qt::AlignCenter);
	for (int row = 0; row < mainTableWidget->rowCount(); row++)
	{
		mainTableWidget->setItem(row, COLUMN_CONNECTOR, protoitem->clone());
		mainTableWidget->setItem(row, COLUMN_PIN, protoitem->clone());
		mainTableWidget->setItem(row, COLUMN_COMPONENT, protoitem->clone());
		mainTableWidget->setItem(row, COLUMN_TYPE, protoitem->clone());
		mainTableWidget->setItem(row, COLUMN_STATUS, protoitem->clone());
		mainTableWidget->setItem(row, COLUMN_MANUAL_CHECK, protoitem->clone());
	}
}

void TestWindow::resetLanguageInTestManualStand()
{
	std::vector<bool> checkedManualChecks;
	for (int i = 0; i < manualChecks.size(); i++)
		checkedManualChecks.push_back(manualChecks[i]->isChecked());
	for (int i = 0; i < manualChecks.size(); i++)
		delete manualChecks[i];
	manualChecks.clear();

	resetTableHeaderLanguageInTestManualStand();
	resetTableTypeLanguageInTestManualStand();
	initUiTableRowsInTestManualStand();

	for (int i = 0; i < checkedManualChecks.size(); i++)
		manualChecks[i]->setChecked(checkedManualChecks[i]);
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
		mainTableHeaderLabels->push_back("Status");
		mainTableHeaderLabels->push_back("Correctly");
		mainTableHeaderLabels->push_back("");
		break;
	}
	mainTableWidget->setHorizontalHeaderLabels(*mainTableHeaderLabels);
}

void TestWindow::resetTableTypeLanguageInTestManualStand()
{
	QAbstractItemModel* model = mainTableWidget->model();
	for (int currentRowNum = 0; currentRowNum < cableRows.size(); currentRowNum++)
	{
		switch (viewWindowState->appLanguage)
		{
		case RUSSIAN_LANG:
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

void TestWindow::initUiTableRowsInTestManualStand()
{
	QAbstractItemModel* model = mainTableWidget->model();
	for (int currentRowNum = 0; currentRowNum < cableRows.size(); currentRowNum++)
	{
		model->setData(model->index(currentRowNum, COLUMN_CONNECTOR), cableRows[currentRowNum]->connectorStr + "\nXP" + QString::number((int)cableRows[currentRowNum]->connectorInt));
		model->setData(model->index(currentRowNum, COLUMN_PIN), cableRows[currentRowNum]->pin);
		model->setData(model->index(currentRowNum, COLUMN_NAME), cableRows[currentRowNum]->name);
		model->setData(model->index(currentRowNum, COLUMN_COMPONENT), cableRows[currentRowNum]->component);

		QWidget* manualChecksWidget = new QWidget(mainLayoutWidget);
		initTableAdditionalManualChecks(currentRowNum, manualChecksWidget);
		mainTableWidget->setCellWidget(currentRowNum, COLUMN_MANUAL_CHECK, manualChecksWidget);
	}
	resetTableTypeLanguageInTestManualStand();
}
