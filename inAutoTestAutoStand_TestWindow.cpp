#include "TestWindow.h"

#define COLUMN_COUNT	7

#define COLUMN_TYPE		3
#define COLUMN_STAND	4
#define COLUMN_PC		5
#define COLUMN_MORE		6

void TestWindow::initUiInAutoTestAutoStand()
{
	autoStandConnectButton = new QPushButton(usefulSpaceWidget);
	autoStandConnectButton->setObjectName("AutoStandConnectButton");
	autoStandConnectButton->setFixedSize(QSize(FIXED_HEADER_BUTTON_WIDTH, FIXED_HEADER_BUTTON_HEIGHT));
	usefulSpaceHLayout->addWidget(autoStandConnectButton);

	autoStandStartTestButton = new QPushButton(usefulSpaceWidget);
	autoStandStartTestButton->setObjectName("AutoStandStartTestButton");
	autoStandStartTestButton->setFixedSize(QSize(FIXED_HEADER_BUTTON_WIDTH, FIXED_HEADER_BUTTON_HEIGHT));
	connect(autoStandStartTestButton, &QPushButton::clicked, this, &TestWindow::slot_autoStandStartTestButton_clicked);
	usefulSpaceHLayout->addWidget(autoStandStartTestButton);

	initUiTableInAutoTestAutoStand();
}

void TestWindow::initUiTableInAutoTestAutoStand()
{
	initUiTableHeaderInAutoTestAutoStand();
	initUiTableRowsInAutoTestAutoStand();
}

void TestWindow::initUiTableHeaderInAutoTestAutoStand()
{
	mainTableWidget->setRowCount((int)cableRows.size());
	mainTableWidget->setColumnCount(COLUMN_COUNT);

	resetTableHeaderLanguageInAutoTestAutoStand();

	mainTableWidget->setColumnWidth(COLUMN_CONNECTOR,	COLUMN_CONNECTOR_WIDTH);
	mainTableWidget->setColumnWidth(COLUMN_PIN,			COLUMN_PIN_WIDTH);
	mainTableWidget->setColumnWidth(COLUMN_TYPE,		COLUMN_TYPE_WIDTH);
	mainTableWidget->setColumnWidth(COLUMN_STAND,		COLUMN_STAND_WIDTH);
	mainTableWidget->setColumnWidth(COLUMN_PC,			COLUMN_PC_WIDTH);
	mainTableWidget->setColumnWidth(COLUMN_MORE,		COLUMN_MORE_WIDTH);

	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_CONNECTOR,		QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_PIN,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_NAME,			QHeaderView::Stretch);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_TYPE,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_STAND,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_PC,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_MORE,			QHeaderView::Fixed);

	QTableWidgetItem* protoitem = new QTableWidgetItem();
	protoitem->setTextAlignment(Qt::AlignCenter);
	for (int row = 0; row < mainTableWidget->rowCount(); row++)
	{
		mainTableWidget->setItem(row, COLUMN_CONNECTOR, protoitem->clone());
		mainTableWidget->setItem(row, COLUMN_PIN, protoitem->clone());
		mainTableWidget->setItem(row, COLUMN_TYPE, protoitem->clone());
	}
}

void TestWindow::resetLanguageInAutoTestAutoStand()
{
	resetTableHeaderLanguageInAutoTestAutoStand();
	resetTableTypeLanguageInAutoTestAutoStand();
}

void TestWindow::resetTableHeaderLanguageInAutoTestAutoStand()
{
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		delete mainTableHeaderLabels;
		mainTableHeaderLabels = new QStringList();

		mainTableHeaderLabels->push_back(QString("Колодка"));
		mainTableHeaderLabels->push_back(QString("Пин"));
		mainTableHeaderLabels->push_back(QString("Название"));
		mainTableHeaderLabels->push_back(QString("Тип"));
		mainTableHeaderLabels->push_back(QString("Стенд"));
		mainTableHeaderLabels->push_back(QString("ПК"));
		mainTableHeaderLabels->push_back("");
		break;

	case ENGLISH_LANG:
		delete mainTableHeaderLabels;
		mainTableHeaderLabels = new QStringList();
		mainTableHeaderLabels->push_back("Connector");
		mainTableHeaderLabels->push_back("Pin");
		mainTableHeaderLabels->push_back("Name");
		mainTableHeaderLabels->push_back("Type");
		mainTableHeaderLabels->push_back("Stand");
		mainTableHeaderLabels->push_back("PC");
		mainTableHeaderLabels->push_back("");
		break;
	}
	mainTableWidget->setHorizontalHeaderLabels(*mainTableHeaderLabels);
}

void TestWindow::resetTableTypeLanguageInAutoTestAutoStand()
{
	QAbstractItemModel* model = mainTableWidget->model();
	for (int currentRowNum = 0; currentRowNum < cableRows.size(); currentRowNum++)
	{
		switch (viewWindowState->appLanguage)
		{
		case RUSSIAN_LANG:
			switch (cableRows[currentRowNum]->typeInt)
			{
			case TypeCable::EMPTY:
				break;
			case TypeCable::DIG_IN:
				model->setData(model->index(currentRowNum, COLUMN_TYPE), QString("Цифровой"));
				break;
			case TypeCable::ANALOG_IN:
				model->setData(model->index(currentRowNum, COLUMN_TYPE), QString("Аналоговоый"));
				break;
			case TypeCable::HALL_IN:
				model->setData(model->index(currentRowNum, COLUMN_TYPE), QString("HALL"));
				break;
			case TypeCable::DIG_OUT:
				model->setData(model->index(currentRowNum, COLUMN_TYPE), QString("Цифровой"));
				break;
			case TypeCable::PWM_OUT:
				model->setData(model->index(currentRowNum, COLUMN_TYPE), QString("ШИМ"));
				break;
			case TypeCable::VNH_OUT:
				model->setData(model->index(currentRowNum, COLUMN_TYPE), QString("VNH"));
				break;
			case TypeCable::HLD_OUT:
				model->setData(model->index(currentRowNum, COLUMN_TYPE), QString("HLD"));
				break;
			case TypeCable::CAN_OUT:
				model->setData(model->index(currentRowNum, COLUMN_TYPE), "CAN");
				break;
			case TypeCable::LIN_OUT:
				model->setData(model->index(currentRowNum, COLUMN_TYPE), "LIN");
				break;
			default:
				break;
			}
			break;

		case ENGLISH_LANG:
			switch (cableRows[currentRowNum]->typeInt)
			{
			case TypeCable::EMPTY:
				break;
			case TypeCable::DIG_IN:
				model->setData(model->index(currentRowNum, COLUMN_TYPE), "Digital");
				break;
			case TypeCable::ANALOG_IN:
				model->setData(model->index(currentRowNum, COLUMN_TYPE), "Analog");
				break;
			case TypeCable::HALL_IN:
				model->setData(model->index(currentRowNum, COLUMN_TYPE), "HALL");
				break;
			case TypeCable::DIG_OUT:
				model->setData(model->index(currentRowNum, COLUMN_TYPE), "Digital");
				break;
			case TypeCable::PWM_OUT:
				model->setData(model->index(currentRowNum, COLUMN_TYPE), "PWM");
				break;
			case TypeCable::VNH_OUT:
				model->setData(model->index(currentRowNum, COLUMN_TYPE), "VNH");
				break;
			case TypeCable::HLD_OUT:
				model->setData(model->index(currentRowNum, COLUMN_TYPE), "HLD");
				break;
			case TypeCable::CAN_OUT:
				model->setData(model->index(currentRowNum, COLUMN_TYPE), "CAN");
				break;
			case TypeCable::LIN_OUT:
				model->setData(model->index(currentRowNum, COLUMN_TYPE), "LIN");
				break;
			default:
				break;
			}
			break;
		}
	}
}

void TestWindow::initUiTableRowsInAutoTestAutoStand()
{
	QAbstractItemModel* model = mainTableWidget->model();
	for (int currentRowNum = 0; currentRowNum < cableRows.size(); currentRowNum++)
	{
		if (mainTableWidget->rowHeight(currentRowNum) < MIN_ROW_HEIGHT)
			mainTableWidget->setRowHeight(currentRowNum, MIN_ROW_HEIGHT);
		if (cableRows[currentRowNum]->connectorInt != ConnectorId::EMPTY)
			model->setData(model->index(currentRowNum, COLUMN_CONNECTOR), cableRows[currentRowNum]->connectorStr + "\nXP" + QString::number((int)cableRows[currentRowNum]->connectorInt));
		model->setData(model->index(currentRowNum, COLUMN_PIN), cableRows[currentRowNum]->pin);
		model->setData(model->index(currentRowNum, COLUMN_NAME), cableRows[currentRowNum]->name);

		QWidget* moreCellWidget = new QWidget(mainLayoutWidget);
		if (cableRows[currentRowNum]->typeInt != TypeCable::CAN_OUT &&
			cableRows[currentRowNum]->typeInt != TypeCable::LIN_OUT)
			initMoreButton(currentRowNum, moreCellWidget);
		mainTableWidget->setCellWidget(currentRowNum, COLUMN_MORE, moreCellWidget);
	}
	resetTableTypeLanguageInAutoTestAutoStand();
}


