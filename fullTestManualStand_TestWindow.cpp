 #include "TestWindow.h"

#define COLUMN_COUNT			9

#define COLUMN_COMPONENT		3
#define COLUMN_DIRECTION		4
#define COLUMN_TYPE				5
#define COLUMN_CHECK			6
#define COLUMN_STATUS			7
#define COLUMN_MANUAL_CHECK		8

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

	fullTestSortType = SortType::SortIndex;

	initUiTableFullTestManualStand();
}

void TestWindow::initUiTableFullTestManualStand()
{
	resetTableHeaderFullTestManualStand();
	resetTableRowsFullTestManualStand();
}

void TestWindow::resetTableHeaderFullTestManualStand()
{
	mainTableWidget->setRowCount((int)cableRows.size());
	mainTableWidget->setColumnCount(COLUMN_COUNT);
	
	resetTableHeaderLanguageFullTestManualStand();
	 
	mainTableWidget->setColumnWidth(COLUMN_CONNECTOR,		COLUMN_CONNECTOR_WIDTH);
	mainTableWidget->setColumnWidth(COLUMN_PIN,				COLUMN_PIN_WIDTH);
	mainTableWidget->setColumnWidth(COLUMN_COMPONENT,		COLUMN_COMPONENT_WIDTH);
	mainTableWidget->setColumnWidth(COLUMN_DIRECTION,		COLUMN_DIRECTION_WIDTH);
	mainTableWidget->setColumnWidth(COLUMN_TYPE,			COLUMN_TYPE_WIDTH);
	mainTableWidget->setColumnWidth(COLUMN_CHECK,			COLUMN_CHECK_WIDTH);
	mainTableWidget->setColumnWidth(COLUMN_STATUS,			COLUMN_STATUS_WIDTH);
	mainTableWidget->setColumnWidth(COLUMN_MANUAL_CHECK,	COLUMN_MANUAL_CHECK_WIDTH);

	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_CONNECTOR,		QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_PIN,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_NAME,			QHeaderView::Stretch);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_COMPONENT,		QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_DIRECTION,		QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_TYPE,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_CHECK,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_STATUS,		QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_MANUAL_CHECK,	QHeaderView::Fixed);

	QTableWidgetItem* protoitem = new QTableWidgetItem();
	protoitem->setTextAlignment(Qt::AlignCenter);
	for (int row = 0; row < mainTableWidget->rowCount(); row++)
	{
		mainTableWidget->setItem(row, COLUMN_CONNECTOR, protoitem->clone());
		mainTableWidget->setItem(row, COLUMN_PIN, protoitem->clone());
		mainTableWidget->setItem(row, COLUMN_COMPONENT, protoitem->clone());
		mainTableWidget->setItem(row, COLUMN_DIRECTION, protoitem->clone());
		mainTableWidget->setItem(row, COLUMN_TYPE, protoitem->clone());
		if (mainTableWidget->item(row, COLUMN_STATUS) == NULL)
			mainTableWidget->setItem(row, COLUMN_STATUS, protoitem->clone());
		mainTableWidget->setItem(row, COLUMN_MANUAL_CHECK, protoitem->clone());
	}
}

void TestWindow::resetLanguageFullTestManualStand()
{
	resetTableHeaderFullTestManualStand();
	resetTableTypeLanguageFullTestManualStand();
	resetTableDirectionLanguageFullTestManualStand();
	resetTableRowsFullTestManualStand();
}

void TestWindow::resetTableHeaderLanguageFullTestManualStand()
{
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		delete mainTableHeaderLabels;
		mainTableHeaderLabels = new QStringList();
		mainTableHeaderLabels->push_back(QString("Колодка"));
		mainTableHeaderLabels->push_back(QString("Пин"));
		mainTableHeaderLabels->push_back(QString("Название"));
		mainTableHeaderLabels->push_back(QString("Компонент"));
		mainTableHeaderLabels->push_back(QString("Направление"));
		mainTableHeaderLabels->push_back(QString("Тип"));
		mainTableHeaderLabels->push_back(QString("Проверка"));
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
		mainTableHeaderLabels->push_back("Component");
		mainTableHeaderLabels->push_back("Direction");
		mainTableHeaderLabels->push_back("Type");
		mainTableHeaderLabels->push_back("Check");
		mainTableHeaderLabels->push_back("Status");
		mainTableHeaderLabels->push_back("Correctly");
		mainTableHeaderLabels->push_back("");
		break;
	}
	mainTableWidget->setHorizontalHeaderLabels(*mainTableHeaderLabels);
}

void TestWindow::resetTableDirectionLanguageFullTestManualStand()
{
	QAbstractItemModel* model = mainTableWidget->model();
	for (int currentRowNum = 0; currentRowNum < cableRows.size(); currentRowNum++)
	{
		switch (viewWindowState->appLanguage)
		{
		case RUSSIAN_LANG:
			if (cableRows[currentRowNum]->direction == "OUT")
				model->setData(model->index(currentRowNum, COLUMN_DIRECTION), QString("Выход"));
			else if (cableRows[currentRowNum]->direction == "IN")
				model->setData(model->index(currentRowNum, COLUMN_DIRECTION), QString("Вход"));
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

void TestWindow::resetTableTypeLanguageFullTestManualStand()
{
	QAbstractItemModel* model = mainTableWidget->model();
	for (int currentRowNum = 0; currentRowNum < cableRows.size(); currentRowNum++)
	{
		switch (viewWindowState->appLanguage)
		{
			switch (cableRows[currentRowNum]->typeInt)
			{
			case TypeCable::EMPTY:
				break;
			case TypeCable::DIG_IN:
				model->setData(model->index(currentRowNum, COLUMN_TYPE), QString("Цифровой"));
				break;
			case TypeCable::ANALOG_IN:
				model->setData(model->index(currentRowNum, COLUMN_TYPE), QString("Аналоговый"));
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
				model->setData(model->index(currentRowNum, COLUMN_TYPE), QString("CAN"));
				break;
			case TypeCable::LIN_OUT:
				model->setData(model->index(currentRowNum, COLUMN_TYPE), QString("LIN"));
				break;
			default:
				break;
			}
		case RUSSIAN_LANG:
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


void TestWindow::resetTableRowsFullTestManualStand()
{
	int hallId = -1;
	QAbstractItemModel* model = mainTableWidget->model();
	for (int currentRowNum = 0; currentRowNum < cableRows.size(); currentRowNum++)
	{
		if (mainTableWidget->rowHeight(currentRowNum) < MIN_ROW_HEIGHT)
			mainTableWidget->setRowHeight(currentRowNum, MIN_ROW_HEIGHT);
		if(cableRows[currentRowNum]->connectorInt != ConnectorId::EMPTY)
			model->setData(model->index(currentRowNum, COLUMN_CONNECTOR), cableRows[currentRowNum]->connectorStr + "\nXP" + QString::number((int)cableRows[currentRowNum]->connectorInt));
		model->setData(model->index(currentRowNum, COLUMN_PIN), cableRows[currentRowNum]->pin);
		model->setData(model->index(currentRowNum, COLUMN_NAME), cableRows[currentRowNum]->name);
		if (cableRows[currentRowNum]->typeInt == TypeCable::CAN_OUT || cableRows[currentRowNum]->typeInt == TypeCable::LIN_OUT)
			model->setData(model->index(currentRowNum, COLUMN_PIN), cableRows[currentRowNum]->component);
		else
			model->setData(model->index(currentRowNum, COLUMN_COMPONENT), cableRows[currentRowNum]->component);

		QWidget* interactionButtonsWidget = new QWidget(mainLayoutWidget);
		QWidget* manualChecksWidget = new QWidget(mainLayoutWidget);

		initTableRowButtons(currentRowNum, interactionButtonsWidget);
		initTableAdditionalManualChecks(currentRowNum, manualChecksWidget);

		mainTableWidget->setCellWidget(currentRowNum, COLUMN_CHECK, interactionButtonsWidget);
		mainTableWidget->setCellWidget(currentRowNum, COLUMN_MANUAL_CHECK, manualChecksWidget);

		if (cableRows[currentRowNum]->typeInt == TypeCable::HALL_IN)
		{
			hallId++;

			QWidget* wiseWidget = new QWidget(mainTableWidget);
			QLabel* wiseLabel = hallLabels[hallId].second;
			hallLabels[hallId].first = 0;
			wiseLabel->setObjectName("wiseLabel");
			wiseLabel->setText("");

			QHBoxLayout* wiseCellHLayout = new QHBoxLayout(wiseWidget);
			wiseCellHLayout->setObjectName("wiseCellHLayout");
			QSpacerItem* leftWiseSpacer = new QSpacerItem(18, 0, QSizePolicy::Fixed);
			wiseCellHLayout->addItem(leftWiseSpacer);
			wiseCellHLayout->addWidget(wiseLabel);
			wiseCellHLayout->setContentsMargins(0, 0, 0, 0);
			wiseWidget->setLayout(wiseCellHLayout);
			if (mainTableWidget->cellWidget(currentRowNum, COLUMN_STATUS) != nullptr)
				mainTableWidget->removeCellWidget(currentRowNum, COLUMN_STATUS);
			mainTableWidget->setCellWidget(currentRowNum, COLUMN_STATUS, wiseWidget);
		}
	}

	resetTableDirectionLanguageFullTestManualStand();
	resetTableTypeLanguageFullTestManualStand();
}

