#include "TestWindow.h"

#define COLUMN_COUNT		7

#define COLUMN_TYPE			3
#define COLUMN_COMPONENT	4
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
	resetTableTypeLanguageOutTestManualStand();
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
	initUiTableRowsOutTestManualStand();
}

void TestWindow::resetTableHeaderLanguageOutTestManualStand()
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
		mainTableHeaderLabels->push_back(QString("Компонент"));
		mainTableHeaderLabels->push_back(QString("Статус"));
		mainTableHeaderLabels->push_back(QString("Исправно"));
		mainTableHeaderLabels->push_back("");
		break;

	case ENGLISH_LANG:
		delete mainTableHeaderLabels;
		mainTableHeaderLabels = new QStringList();
		mainTableHeaderLabels->push_back("Connector");
		mainTableHeaderLabels->push_back("Pin");
		mainTableHeaderLabels->push_back("Name");
		mainTableHeaderLabels->push_back("Type");
		mainTableHeaderLabels->push_back("Component");
		mainTableHeaderLabels->push_back("Status");
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

		resetLanguageToolTipButtonTable(currentRowNum, NOT_SET);
	}
}
