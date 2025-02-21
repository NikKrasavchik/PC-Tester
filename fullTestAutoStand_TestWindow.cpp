#include "TestWindow.h"

#define COLUMN_COUNT		8

#define COLUMN_DIRECTION	3
#define COLUMN_TYPE		4
#define COLUMN_CHECK		5
#define COLUMN_STATUS		6
#define COLUMN_MORE		7

void TestWindow::initUiFullTestAutoStand()
{
	autoStandConnectButton = new QPushButton(usefulSpaceWidget);
	autoStandConnectButton->setObjectName("AutoStandConnectButton");
	autoStandConnectButton->setFixedSize(QSize(FIXED_HEADER_BUTTON_WIDTH, FIXED_HEADER_BUTTON_HEIGHT));
	usefulSpaceHLayout->addWidget(autoStandConnectButton);

	autoStandStartTestButton = new QPushButton(usefulSpaceWidget);
	autoStandStartTestButton->setObjectName("AutoStandStartTestButton");
	autoStandStartTestButton->setFixedSize(QSize(FIXED_HEADER_BUTTON_WIDTH, FIXED_HEADER_BUTTON_HEIGHT));
	usefulSpaceHLayout->addWidget(autoStandStartTestButton);

	fullTestSortButton = new QPushButton(usefulSpaceWidget);
	fullTestSortButton->setObjectName("fullTestSortButton");
	fullTestSortButton->setFixedSize(QSize(FIXED_HEADER_BUTTON_WIDTH, FIXED_HEADER_BUTTON_HEIGHT));
	usefulSpaceHLayout->addWidget(fullTestSortButton);
	fullTestSortType = SORT_TYPE_INDEX;
	
	initUiTableFullTestAutoStand();
}

void TestWindow::initUiTableFullTestAutoStand()
{
	resetTableHeaderFullTestAutoStand();
	resetTableRowsFullTestAutoStand();
}

void TestWindow::resetTableHeaderFullTestAutoStand()
{
	mainTableWidget->setRowCount((int)cableRows.size());
	mainTableWidget->setColumnCount(COLUMN_COUNT);

	resetTableHeaderLanguageFullTestAutoStand();

	mainTableWidget->setColumnWidth(COLUMN_CONNECTOR,	COLUMN_CONNECTOR_WIDTH);
	mainTableWidget->setColumnWidth(COLUMN_PIN,		COLUMN_PIN_WIDTH);
	mainTableWidget->setColumnWidth(COLUMN_DIRECTION,	COLUMN_DIRECTION_WIDTH);
	mainTableWidget->setColumnWidth(COLUMN_TYPE,		COLUMN_TYPE_WIDTH);
	mainTableWidget->setColumnWidth(COLUMN_CHECK,		COLUMN_AUTOCHECK_WIDTH);
	mainTableWidget->setColumnWidth(COLUMN_STATUS,		COLUMN_STATUS_WIDTH);
	mainTableWidget->setColumnWidth(COLUMN_MORE,		COLUMN_MORE_WIDTH);

	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_CONNECTOR,		QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_PIN,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_NAME,			QHeaderView::Stretch);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_DIRECTION,		QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_TYPE,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_CHECK,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_STATUS,		QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_MORE,			QHeaderView::Fixed);

	QTableWidgetItem* protoitem = new QTableWidgetItem();
	protoitem->setTextAlignment(Qt::AlignCenter);
	for (int row = 0; row < mainTableWidget->rowCount(); row++)
	{
		mainTableWidget->setItem(row, COLUMN_CONNECTOR, protoitem->clone());
		mainTableWidget->setItem(row, COLUMN_PIN, protoitem->clone());
		mainTableWidget->setItem(row, COLUMN_DIRECTION, protoitem->clone());
		mainTableWidget->setItem(row, COLUMN_TYPE, protoitem->clone());
		mainTableWidget->setItem(row, COLUMN_STATUS, protoitem->clone());
	}
}

void TestWindow::resetLanguageFullTestAutoStand()
{
	resetTableHeaderLanguageFullTestAutoStand();
	resetTableTypeLanguageFullTestAutoStand();
	resetTableDirectionLanguageFullTestAutoStand();
}

void TestWindow::resetTableHeaderLanguageFullTestAutoStand()
{
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		delete mainTableHeaderLabels;
		mainTableHeaderLabels = new QStringList();
		mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Разъём"));
		mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Пин"));
		mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Название"));
		mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Направление"));
		mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Тип"));
		mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Стенд"));
		mainTableHeaderLabels->push_back(QString::fromLocal8Bit("ПК"));
		mainTableHeaderLabels->push_back("");
		break;

	case ENGLISH_LANG:
		delete mainTableHeaderLabels;
		mainTableHeaderLabels = new QStringList();
		mainTableHeaderLabels->push_back("Connector");
		mainTableHeaderLabels->push_back("Pin");
		mainTableHeaderLabels->push_back("Name");
		mainTableHeaderLabels->push_back("Direction");
		mainTableHeaderLabels->push_back("Type");
		mainTableHeaderLabels->push_back("Stand");
		mainTableHeaderLabels->push_back("PC");
		mainTableHeaderLabels->push_back("");
		break;
	}
	mainTableWidget->setHorizontalHeaderLabels(*mainTableHeaderLabels);
}

void TestWindow::resetTableDirectionLanguageFullTestAutoStand()
{
	QAbstractItemModel* model = mainTableWidget->model();
	for (int currentRowNum = 0; currentRowNum < cableRows.size(); currentRowNum++)
	{
		switch (viewWindowState->appLanguage)
		{
		case RUSSIAN_LANG:
			if (cableRows[currentRowNum]->direction == "OUT")
				model->setData(model->index(currentRowNum, COLUMN_DIRECTION), QString::fromLocal8Bit("Выход"));
			else if (cableRows[currentRowNum]->direction == "IN")
				model->setData(model->index(currentRowNum, COLUMN_DIRECTION), QString::fromLocal8Bit("Вход"));
			break;

		case ENGLISH_LANG:
			if (cableRows[currentRowNum]->direction == "OUT")
				model->setData(model->index(currentRowNum, COLUMN_DIRECTION), "Out");
			else if (cableRows[currentRowNum]->direction == "IN")
				model->setData(model->index(currentRowNum, COLUMN_DIRECTION), "In");
			break;
		}
	}
}

void TestWindow::resetTableTypeLanguageFullTestAutoStand()
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

void TestWindow::resetTableRowsFullTestAutoStand()
{
	QAbstractItemModel* model = mainTableWidget->model();
	for (int currentRowNum = 0; currentRowNum < cableRows.size(); currentRowNum++)
	{
		qDebug() << cableRows[currentRowNum]->connectorStr << cableRows[currentRowNum]->pin << cableRows[currentRowNum]->name << cableRows[currentRowNum]->direction << cableRows[currentRowNum]->typeStr;

		model->setData(model->index(currentRowNum, COLUMN_CONNECTOR), cableRows[currentRowNum]->connectorStr);
		model->setData(model->index(currentRowNum, COLUMN_PIN), cableRows[currentRowNum]->pin);
		model->setData(model->index(currentRowNum, COLUMN_NAME), cableRows[currentRowNum]->name);

		QWidget* autoCheckCellWidget = new QWidget(mainLayoutWidget);
		QWidget* moreCellWidget = new QWidget(mainLayoutWidget);

		initMoreButton(currentRowNum, moreCellWidget);

		mainTableWidget->setCellWidget(currentRowNum, COLUMN_CHECK, autoCheckCellWidget);
		mainTableWidget->setCellWidget(currentRowNum, COLUMN_MORE, moreCellWidget);
	}
	resetTableDirectionLanguageFullTestAutoStand();
	resetTableTypeLanguageFullTestAutoStand();
}

void TestWindow::on_AutoStandConnectButton_clicked()
{
}
