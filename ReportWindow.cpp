#include "ReportWindow.h"


ReportWindow::ReportWindow(std::vector<Cable> cables, std::vector<void*> additionalValues, WindowType testType)
{
	this->cables = cables;
	this->testType = testType;
	this->additionalValues = additionalValues;

	measuredCheckBox = nullptr;
	checkManualStandCheckBox = nullptr;
	checkPCAutoStandCheckBox = nullptr;
	checkStandAutoStandCheckBox = nullptr;

	for (int i = 0; i < sizeof(changedThresholds) / sizeof(changedThresholds[0]); i++)
		changedThresholds[i] = -1;

	initUi();

	QMetaObject::connectSlotsByName(this);

	QStringList* mainTableHeaderLabels = new QStringList();
	mainTableHeaderLabels->push_back("");
	mainTableHeaderLabels->push_back("");
	mainTableHeaderLabels->push_back("");

	for (int row = ROW_3; row < mainTableWidget->rowCount(); row++)
	{
		mainTableHeaderLabels->push_back(QString::number(row - ROW_3));
		mainTableWidget->verticalHeader()->setSectionResizeMode(row, QHeaderView::Interactive);
	}

	mainTableWidget->setVerticalHeaderLabels(*mainTableHeaderLabels);
}
ReportWindow::~ReportWindow()
{}

void ReportWindow::initUi()
{
	this->resize(START_ReportWindow_SIZE);
	this->setMinimumSize(725, 250);
	mainWidget = new QWidget(this);
	mainWidget->setObjectName("mainLayoutWidget");
	mainWidget->setGeometry(LEFT_PADDING_MAIN_WIDGET, UP_PADDING_MAIN_WIDGET, START_ReportWindow_WIDTH - (LEFT_PADDING_MAIN_WIDGET * 2), START_ReportWindow_HEIGHT - (UP_PADDING_MAIN_WIDGET * 2));

	mainVLayout = new QVBoxLayout(mainWidget);
	mainVLayout->setObjectName("mainVLayout");

	bodyHLayout = new QHBoxLayout(mainWidget);
	bodyHLayout->setObjectName("mainHLayout");
	mainVLayout->addLayout(bodyHLayout);

	initUiTable();
	bodyHLayout->addWidget(mainTableWidget);

	initUiColumnSetters();
	bodyHLayout->addWidget(columnSetterWidget);

	resetUiAdditionalColumns();
	resetUiFillColumns();

	initUiFooter();
	mainVLayout->addWidget(footerWidget);
}

void ReportWindow::initUiTable()
{
	initUiGenerateTable();
	initUiSetValueTable();
}

void ReportWindow::initUiGenerateTable()
{
	mainTableWidget = new QTableWidget(mainWidget);
	mainTableWidget->setObjectName("mainTableWidget");
	mainTableWidget->setStyleSheet(lightStyles.testwindowTableWidget);
	mainTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	mainTableWidget->horizontalHeader()->hide();
	mainTableWidget->setRowCount(3);
	mainTableWidget->setColumnCount(CLEAR_COLUMN_COUNT);

	QFont font = QFont();
	font.setBold(true);
	font.setPointSizeF(10);

	mainTableWidget->setSpan(CELL_PAD_TABLE, SPAN_COUNT_3, SPAN_COUNT_1);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_PAD_TABLE), "");
	mainTableWidget->item(CELL_PAD_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_PAD_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_PAD_TABLE)->setFont(font);

	mainTableWidget->setSpan(CELL_PIN_TABLE, SPAN_COUNT_3, SPAN_COUNT_1);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_PIN_TABLE), "");
	mainTableWidget->item(CELL_PIN_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_PIN_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_PIN_TABLE)->setFont(font);

	mainTableWidget->setSpan(CELL_TYPE_TABLE, SPAN_COUNT_3, SPAN_COUNT_1);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_TYPE_TABLE), "");
	mainTableWidget->item(CELL_TYPE_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_TYPE_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_TYPE_TABLE)->setFont(font);

	mainTableWidget->setSpan(CELL_NAME_TABLE, SPAN_COUNT_3, SPAN_COUNT_1);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_NAME_TABLE), "");
	mainTableWidget->item(CELL_NAME_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_NAME_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_NAME_TABLE)->setFont(font);

	mainTableWidget->setSpan(CELL_COMPONENT_TABLE, SPAN_COUNT_3, SPAN_COUNT_1);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_COMPONENT_TABLE), "");
	mainTableWidget->item(CELL_COMPONENT_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_COMPONENT_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_COMPONENT_TABLE)->setFont(font);

	mainTableWidget->setSpan(CELL_TRESHHOLDERS_TABLE, SPAN_COUNT_1, SPAN_COUNT_4);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_TRESHHOLDERS_TABLE), "");
	mainTableWidget->item(CELL_TRESHHOLDERS_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_TRESHHOLDERS_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_TRESHHOLDERS_TABLE)->setFont(font);

	mainTableWidget->setSpan(CELL_TRESHHOLDERS_U_TABLE, SPAN_COUNT_1, SPAN_COUNT_2);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_TRESHHOLDERS_U_TABLE), "");
	mainTableWidget->item(CELL_TRESHHOLDERS_U_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_TRESHHOLDERS_U_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_TRESHHOLDERS_U_TABLE)->setFont(font);

	mainTableWidget->setSpan(CELL_TRESHHOLDERS_I_TABLE, SPAN_COUNT_1, SPAN_COUNT_2);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_TRESHHOLDERS_I_TABLE), "");
	mainTableWidget->item(CELL_TRESHHOLDERS_I_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_TRESHHOLDERS_I_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_TRESHHOLDERS_I_TABLE)->setFont(font);

	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_TRESHHOLDERS_U_MIN_TABLE), "");
	mainTableWidget->item(CELL_TRESHHOLDERS_U_MIN_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_TRESHHOLDERS_U_MIN_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_TRESHHOLDERS_U_MIN_TABLE)->setFont(font);

	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_TRESHHOLDERS_U_MAX_TABLE), "");
	mainTableWidget->item(CELL_TRESHHOLDERS_U_MAX_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_TRESHHOLDERS_U_MAX_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_TRESHHOLDERS_U_MAX_TABLE)->setFont(font);

	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_TRESHHOLDERS_I_MIN_TABLE), "");
	mainTableWidget->item(CELL_TRESHHOLDERS_I_MIN_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_TRESHHOLDERS_I_MIN_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_TRESHHOLDERS_I_MIN_TABLE)->setFont(font);

	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_TRESHHOLDERS_I_MAX_TABLE), "");
	mainTableWidget->item(CELL_TRESHHOLDERS_I_MAX_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_TRESHHOLDERS_I_MAX_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_TRESHHOLDERS_I_MAX_TABLE)->setFont(font);

	mainTableWidget->setColumnWidth(COLUMN_PAD, 75);
	mainTableWidget->setColumnWidth(COLUMN_PIN, 75);
	mainTableWidget->setColumnWidth(COLUMN_TYPE, 75);
	mainTableWidget->setColumnWidth(COLUMN_NAME, 75);
	mainTableWidget->setColumnWidth(COLUMN_COMPONENT, 75);
	mainTableWidget->setColumnWidth(COLUMN_TRESHHOLDERS_U_MIN, 50);
	mainTableWidget->setColumnWidth(COLUMN_TRESHHOLDERS_U_MAX, 50);
	mainTableWidget->setColumnWidth(COLUMN_TRESHHOLDERS_I_MIN, 50);
	mainTableWidget->setColumnWidth(COLUMN_TRESHHOLDERS_I_MAX, 50);

	mainTableWidget->setRowHeight(ROW_0, 40);
	mainTableWidget->setRowHeight(ROW_1, 40);
	mainTableWidget->setRowHeight(ROW_2, 40);

	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_PAD, QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_PIN, QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_TYPE, QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_NAME, QHeaderView::Stretch);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_COMPONENT, QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_TRESHHOLDERS, QHeaderView::Fixed);

	mainTableWidget->verticalHeader()->setSectionResizeMode(ROW_0, QHeaderView::Fixed);
	mainTableWidget->verticalHeader()->setSectionResizeMode(ROW_1, QHeaderView::Fixed);
	mainTableWidget->verticalHeader()->setSectionResizeMode(ROW_2, QHeaderView::Fixed);
}

void ReportWindow::initUiSetValueTable()
{
	QString typeTmp;
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		mainTableWidget->item(CELL_PAD_TABLE)->setText(QString::fromLocal8Bit("Колодка"));
		mainTableWidget->item(CELL_PIN_TABLE)->setText(QString::fromLocal8Bit("Пин"));
		mainTableWidget->item(CELL_TYPE_TABLE)->setText(QString::fromLocal8Bit("Тип"));
		mainTableWidget->item(CELL_NAME_TABLE)->setText(QString::fromLocal8Bit("Название"));
		mainTableWidget->item(CELL_COMPONENT_TABLE)->setText(QString::fromLocal8Bit("Компонент"));
		mainTableWidget->item(CELL_TRESHHOLDERS_TABLE)->setText(QString::fromLocal8Bit("Пороги"));
		mainTableWidget->item(CELL_TRESHHOLDERS_U_TABLE)->setText(QString::fromLocal8Bit("U, В"));
		mainTableWidget->item(CELL_TRESHHOLDERS_I_TABLE)->setText(QString::fromLocal8Bit("I, А"));
		mainTableWidget->item(CELL_TRESHHOLDERS_U_MIN_TABLE)->setText(QString::fromLocal8Bit("Мин"));
		mainTableWidget->item(CELL_TRESHHOLDERS_U_MAX_TABLE)->setText(QString::fromLocal8Bit("Макс"));
		mainTableWidget->item(CELL_TRESHHOLDERS_I_MIN_TABLE)->setText(QString::fromLocal8Bit("Мин"));
		mainTableWidget->item(CELL_TRESHHOLDERS_I_MAX_TABLE)->setText(QString::fromLocal8Bit("Макс"));
		break;

	case ENGLISH_LANG:
		mainTableWidget->item(CELL_PAD_TABLE)->setText("Pad");
		mainTableWidget->item(CELL_PIN_TABLE)->setText("Pin");
		mainTableWidget->item(CELL_TYPE_TABLE)->setText("Type");
		mainTableWidget->item(CELL_NAME_TABLE)->setText("Name");
		mainTableWidget->item(CELL_COMPONENT_TABLE)->setText("Component");
		mainTableWidget->item(CELL_TRESHHOLDERS_TABLE)->setText("Treshholders");
		mainTableWidget->item(CELL_TRESHHOLDERS_U_TABLE)->setText("U, V");
		mainTableWidget->item(CELL_TRESHHOLDERS_I_TABLE)->setText("I, A");
		mainTableWidget->item(CELL_TRESHHOLDERS_U_MIN_TABLE)->setText("Min");
		mainTableWidget->item(CELL_TRESHHOLDERS_U_MAX_TABLE)->setText("Max");
		mainTableWidget->item(CELL_TRESHHOLDERS_I_MIN_TABLE)->setText("Min");
		mainTableWidget->item(CELL_TRESHHOLDERS_I_MAX_TABLE)->setText("Max");
		break;
	}

	for (int row = ROW_3; row < cables.size() + ROW_3; row++)
	{
		mainTableWidget->insertRow(mainTableWidget->rowCount());
		for (int column = 0; column < CLEAR_COLUMN_COUNT; column++)
		{
			QAbstractItemModel* model = mainTableWidget->model();
			switch (column)
			{
			case COLUMN_PAD:
				model->setData(model->index(row, column), QString((char)(PRIMARY_CONNECTOR_SYMBOL + (int)cables[row - ROW_3].connector)));
				break;

			case COLUMN_PIN:
				model->setData(model->index(row, column), QString::number(cables[row - ROW_3].pin));
				break;

			case COLUMN_NAME:
				model->setData(model->index(row, column), QString(cables[row - ROW_3].name));
				break;

			case COLUMN_COMPONENT:
				model->setData(model->index(row, column), QString(cables[row - ROW_3].component));
				break;

			case COLUMN_TYPE:
				switch (viewWindowState->appLanguage)
				{
				case RUSSIAN_LANG:
					switch (cables[row - ROW_3].type)
					{
					case TYPE_NOT_SET:
						model->setData(model->index(row, column), QString("No set"));
						break;
					case TYPE_DIGITAL:
						model->setData(model->index(row, column), QString("Digital"));
						break;
					case TYPE_ANALOG:
						model->setData(model->index(row, column), QString("Analog"));
						break;
					case TYPE_HALL:
						model->setData(model->index(row, column), QString("HALL"));
						break;
					case TYPE_PWM:
						model->setData(model->index(row, column), QString("PWM"));
						break;
					case TYPE_VNH:
						model->setData(model->index(row, column), QString("VNH"));
						break;
					}
					break;
				case ENGLISH_LANG:
					switch (cables[row - ROW_3].type)
					{
					case TYPE_NOT_SET:
						model->setData(model->index(row, column), QString::fromLocal8Bit("Не указан"));
						break;
					case TYPE_DIGITAL:
						model->setData(model->index(row, column), QString::fromLocal8Bit("Цифровой"));
						break;
					case TYPE_ANALOG:
						model->setData(model->index(row, column), QString::fromLocal8Bit("Аналоговый"));
						break;
					case TYPE_HALL:
						model->setData(model->index(row, column), QString("HALL"));
						break;
					case TYPE_PWM:
						model->setData(model->index(row, column), QString::fromLocal8Bit("ШИМ"));
						break;
					case TYPE_VNH:
						model->setData(model->index(row, column), QString::fromLocal8Bit("VNH"));
						break;
						break;
					}
					break;
				}
				break;

			case COLUMN_TRESHHOLDERS_U_MIN:
				model->setData(model->index(row, column), QString::number(cables[row - ROW_3].minVoltage));
				break;

			case COLUMN_TRESHHOLDERS_U_MAX:
				model->setData(model->index(row, column), QString::number(cables[row - ROW_3].maxVoltage));
				break;

			case COLUMN_TRESHHOLDERS_I_MIN:
				model->setData(model->index(row, column), QString::number(cables[row - ROW_3].minCurrent));
				break;

			case COLUMN_TRESHHOLDERS_I_MAX:
				model->setData(model->index(row, column), QString::number(cables[row - ROW_3].maxCurrent));
				break;
			}

			if (column != COLUMN_NAME)
				mainTableWidget->item(row, column)->setTextAlignment(Qt::AlignCenter);
		}
	}
}

void ReportWindow::fillColumnsSetters()
{
	switch (testType)
	{
	case WindowType::IN_TEST_MANUAL_STAND:
	case WindowType::OUT_TEST_MANUAL_STAND:
	case WindowType::FULL_TEST_MANUAL_STAND:
		activeColumn.measured				= false;
		activeColumn.checkManualStand		= true;
		activeColumn.checkPCAutoStand		= false;
		activeColumn.checkStandAutoStand	= false;
		break;

	case WindowType::IN_MANUAL_TEST_AUTO_STAND:
	case WindowType::OUT_MANUAL_TEST_AUTO_STAND:
	case WindowType::IN_AUTO_TEST_AUTO_STAND:
	case WindowType::OUT_AUTO_TEST_AUTO_STAND:
	case WindowType::FULL_TEST_AUTO_STAND:
		activeColumn.measured				= true;
		activeColumn.checkManualStand		= false;
		activeColumn.checkPCAutoStand		= true;
		activeColumn.checkStandAutoStand	= true;
		break;
	}
}

void ReportWindow::initUiColumnSetters()
{
	columnSetterWidget = new QWidget(mainWidget);
	columnSetterWidget->setObjectName("columnSetterWidget");
	columnSetterWidget->setFixedWidth(COLUMN_SETTER_WIDTH);

	columnSetterVLayout = new QVBoxLayout(columnSetterWidget);
	columnSetterVLayout->setObjectName("columnSetterVLayout");

	fillColumnsSetters();

	if (activeColumn.measured)
	{
		measuredHLayout = new QHBoxLayout(columnSetterWidget);
		measuredHLayout->setObjectName("measuredHLayout");
		columnSetterVLayout->addLayout(measuredHLayout);

		measuredLabel = new QLabel(columnSetterWidget);
		measuredLabel->setObjectName("measuredLabel");
		measuredLabel->setText("Measured column");
		measuredHLayout->addWidget(measuredLabel);

		measuredCheckBox = new QCheckBox(columnSetterWidget);
		measuredCheckBox->setObjectName("measuredCheckBox");
		measuredCheckBox->setChecked(true);
		measuredHLayout->addWidget(measuredCheckBox, Qt::AlignRight);
	}

	if (activeColumn.checkManualStand)
	{
		checkManualStandHLayout = new QHBoxLayout(columnSetterWidget);
		checkManualStandHLayout->setObjectName("checkManualStandHLayout");
		columnSetterVLayout->addLayout(checkManualStandHLayout);

		checkManualStandLabel = new QLabel(columnSetterWidget);
		checkManualStandLabel->setObjectName("checkManualStandLabel");
		checkManualStandLabel->setText("Manual stand check column");
		checkManualStandHLayout->addWidget(checkManualStandLabel);

		checkManualStandCheckBox = new QCheckBox(columnSetterWidget);
		checkManualStandCheckBox->setObjectName("checkManualStandCheckBox");
		checkManualStandCheckBox->setChecked(false);
		columnSetterVLayout->addWidget(checkManualStandCheckBox, Qt::AlignRight);
	}

	if (activeColumn.checkPCAutoStand)
	{
		checkPCAutoStandHLayout = new QHBoxLayout(columnSetterWidget);
		checkPCAutoStandHLayout->setObjectName("checkPCAutoStandHLayout");
		columnSetterVLayout->addLayout(checkPCAutoStandHLayout);

		checkPCAutoStandLabel = new QLabel(columnSetterWidget);
		checkPCAutoStandLabel->setObjectName("checkPCAutoStandLabel");
		checkPCAutoStandLabel->setText("PC check in auto stand column");
		checkPCAutoStandHLayout->addWidget(checkPCAutoStandLabel);

		checkPCAutoStandCheckBox = new QCheckBox(columnSetterWidget);
		checkPCAutoStandCheckBox->setObjectName("checkPCAutoStandCheckBox");
		checkPCAutoStandCheckBox->setChecked(false);
		columnSetterVLayout->addWidget(checkPCAutoStandCheckBox, Qt::AlignRight);
	}

	if (activeColumn.checkStandAutoStand)
	{
		checkStandAutoStandHLayout = new QHBoxLayout(columnSetterWidget);
		checkStandAutoStandHLayout->setObjectName("checkStandAutoStandHLayout");
		columnSetterVLayout->addLayout(checkStandAutoStandHLayout);

		checkStandAutoStandLabel = new QLabel(columnSetterWidget);
		checkStandAutoStandLabel->setObjectName("checkStandAutoStandLabel");
		checkStandAutoStandLabel->setText("Stand check in auto stand column");
		checkStandAutoStandHLayout->addWidget(checkStandAutoStandLabel);

		checkStandAutoStandCheckBox = new QCheckBox(columnSetterWidget);
		checkStandAutoStandCheckBox->setObjectName("checkStandAutoStandCheckBox");
		checkStandAutoStandCheckBox->setChecked(false);
		columnSetterVLayout->addWidget(checkStandAutoStandCheckBox, Qt::AlignRight);
	}
}

void ReportWindow::resetUiAdditionalColumns()
{
	while (mainTableWidget->columnCount() != CLEAR_COLUMN_COUNT)
		mainTableWidget->removeColumn(mainTableWidget->columnCount() - 1);

	QFont font = QFont();
	font.setBold(true);
	font.setPointSizeF(10);

	if (measuredCheckBox != nullptr && measuredCheckBox->isChecked())
	{
		int currentColumn = mainTableWidget->columnCount();
		int currentColumn_U = mainTableWidget->columnCount();
		int currentColumn_I = mainTableWidget->columnCount() + 1;

		mainTableWidget->insertColumn(currentColumn_U);
		mainTableWidget->insertColumn(currentColumn_I);

		mainTableWidget->setSpan(ROW_0, currentColumn, SPAN_COUNT_2, SPAN_COUNT_2);
		mainTableWidget->model()->setData(mainTableWidget->model()->index(ROW_0, currentColumn), "");
		mainTableWidget->item(ROW_0, currentColumn)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(ROW_0, currentColumn)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(ROW_0, currentColumn)->setFont(font);

		mainTableWidget->model()->setData(mainTableWidget->model()->index(ROW_2, currentColumn_U), "U, V");
		mainTableWidget->item(ROW_2, currentColumn_U)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(ROW_2, currentColumn_U)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(ROW_2, currentColumn_U)->setFont(font);

		mainTableWidget->model()->setData(mainTableWidget->model()->index(ROW_2, currentColumn_I), "I, A");
		mainTableWidget->item(ROW_2, currentColumn_I)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(ROW_2, currentColumn_I)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(ROW_2, currentColumn_I)->setFont(font);

		switch (viewWindowState->appLanguage)
		{
		case RUSSIAN_LANG:
			mainTableWidget->item(ROW_0, currentColumn)->setText(QString::fromLocal8Bit("Измеренное значение"));
			break;

		case ENGLISH_LANG:
			mainTableWidget->item(ROW_0, currentColumn)->setText(QString("Measured value"));
			break;
		}
	}

	if (checkManualStandCheckBox != nullptr && checkManualStandCheckBox->isChecked())
	{
		int currentColumn = mainTableWidget->columnCount();
		mainTableWidget->insertColumn(mainTableWidget->columnCount());
		mainTableWidget->setSpan(ROW_0, currentColumn, SPAN_COUNT_3, SPAN_COUNT_1);
		mainTableWidget->model()->setData(mainTableWidget->model()->index(ROW_0, currentColumn), "");
		mainTableWidget->item(ROW_0, currentColumn)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(ROW_0, currentColumn)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(ROW_0, currentColumn)->setFont(font);
		mainTableWidget->setColumnWidth(currentColumn, 75);


		switch (viewWindowState->appLanguage)
		{
		case RUSSIAN_LANG:
			mainTableWidget->item(ROW_0, currentColumn)->setText(QString::fromLocal8Bit("Статус ручной проверки"));
			break;

		case ENGLISH_LANG:
			mainTableWidget->item(ROW_0, currentColumn)->setText(QString("Manual status"));
			break;
		}
	}

	if (checkPCAutoStandCheckBox != nullptr && checkPCAutoStandCheckBox->isChecked())
	{
		int currentColumn = mainTableWidget->columnCount();
		mainTableWidget->insertColumn(mainTableWidget->columnCount());
		mainTableWidget->setSpan(ROW_0, currentColumn, SPAN_COUNT_3, SPAN_COUNT_1);
		mainTableWidget->model()->setData(mainTableWidget->model()->index(ROW_0, currentColumn), "");
		mainTableWidget->item(ROW_0, currentColumn)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(ROW_0, currentColumn)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(ROW_0, currentColumn)->setFont(font);
		mainTableWidget->setColumnWidth(currentColumn, 75);

		switch (viewWindowState->appLanguage)
		{
		case RUSSIAN_LANG:
			mainTableWidget->item(ROW_0, currentColumn)->setText(QString::fromLocal8Bit("Статус проверки ПК"));
			break;

		case ENGLISH_LANG:
			mainTableWidget->item(ROW_0, currentColumn)->setText(QString("PC status"));
			break;
		}
	}

	if (checkStandAutoStandCheckBox != nullptr && checkStandAutoStandCheckBox->isChecked())
	{
		int currentColumn = mainTableWidget->columnCount();
		mainTableWidget->insertColumn(mainTableWidget->columnCount());
		mainTableWidget->setSpan(ROW_0, currentColumn, SPAN_COUNT_3, SPAN_COUNT_1);
		mainTableWidget->model()->setData(mainTableWidget->model()->index(ROW_0, currentColumn), "");
		mainTableWidget->item(ROW_0, currentColumn)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(ROW_0, currentColumn)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(ROW_0, currentColumn)->setFont(font);
		mainTableWidget->setColumnWidth(currentColumn, 75);

		switch (viewWindowState->appLanguage)
		{
		case RUSSIAN_LANG:
			mainTableWidget->item(ROW_0, currentColumn)->setText(QString::fromLocal8Bit("Статус проверки стенда"));
			break;

		case ENGLISH_LANG:
			mainTableWidget->item(ROW_0, currentColumn)->setText(QString("Stand status"));
			break;
		}
	}
}

void ReportWindow::resetUiFillColumns()
{
	int measuredAdditionalInd = ADDITIONAL_IND_NOT_SET;
	int checkManualStandAdditionalInd = ADDITIONAL_IND_NOT_SET;
	int checkPCAutoStandAdditionalInd = ADDITIONAL_IND_NOT_SET;
	int checkStandAutoStandAdditionalInd = ADDITIONAL_IND_NOT_SET;

	std::vector<QCheckBox*> manualChecked;

	int currentAdditionalInd = CLEAR_COLUMN_COUNT;
	if (measuredCheckBox != nullptr && measuredCheckBox->isChecked())
	{
		measuredAdditionalInd = currentAdditionalInd;
		currentAdditionalInd += 2;
	}
	if (checkManualStandCheckBox != nullptr && checkManualStandCheckBox->isChecked())
	{
		checkManualStandAdditionalInd = currentAdditionalInd;
		currentAdditionalInd++;

		for (int i = 0; i < additionalValues.size(); i++)
			manualChecked.push_back(((QCheckBox*)additionalValues[i]));
	}
	if (checkPCAutoStandCheckBox != nullptr && checkPCAutoStandCheckBox->isChecked())
	{
		checkPCAutoStandAdditionalInd = currentAdditionalInd;
		currentAdditionalInd++;
	}
	if (checkStandAutoStandCheckBox != nullptr && checkStandAutoStandCheckBox->isChecked())
	{
		checkStandAutoStandAdditionalInd = currentAdditionalInd;
		currentAdditionalInd++;
	}

	QAbstractItemModel* model = mainTableWidget->model();
	for (int row = ROW_3; row < cables.size() + ROW_3; row++)
		for (int column = CLEAR_COLUMN_COUNT - 1; column < mainTableWidget->columnCount(); column++)
		{
			if (measuredAdditionalInd != ADDITIONAL_IND_NOT_SET && measuredAdditionalInd == column)
			{
				model->setData(model->index(row, column), 1);
				model->setData(model->index(row, column + 1), 1);
				column++;
			}
			if (checkManualStandAdditionalInd != ADDITIONAL_IND_NOT_SET && checkManualStandAdditionalInd == column)
			{
				switch (viewWindowState->appLanguage)
				{
				case RUSSIAN_LANG:
					model->setData(model->index(row, column), QString::fromLocal8Bit(((QCheckBox*)additionalValues[row - ROW_3])->isChecked() ? "Исправно" : "Неисправно"));
					break;

				case ENGLISH_LANG:
					model->setData(model->index(row, column), QString(((QCheckBox*)additionalValues[row - ROW_3])->isChecked() ? "Correct" : "Incorrect"));
					break;
				}
			}
			if (checkPCAutoStandAdditionalInd != ADDITIONAL_IND_NOT_SET && checkPCAutoStandAdditionalInd == column)
			{
				model->setData(model->index(row, column), 3);
			}
			if (checkStandAutoStandAdditionalInd != ADDITIONAL_IND_NOT_SET && checkStandAutoStandAdditionalInd == column)
			{
				model->setData(model->index(row, column), "4");
			}

			mainTableWidget->item(row, column)->setTextAlignment(Qt::AlignCenter);
			if (measuredAdditionalInd != ADDITIONAL_IND_NOT_SET)
				mainTableWidget->item(row, column - 1)->setTextAlignment(Qt::AlignCenter);
		}
}

void ReportWindow::initUiFooter()
{
	footerWidget = new QWidget(mainWidget);
	footerWidget->setObjectName("footerWidget");

	bottomHLayout = new QHBoxLayout(footerWidget);
	bottomHLayout->setObjectName("bottomHLayout");

	bottomSpacer = new QSpacerItem(40, 0, QSizePolicy::Expanding);
	bottomHLayout->addItem(bottomSpacer);

	saveButton = new QPushButton(footerWidget);
	saveButton->setMinimumSize(BUTTON_SIZE);
	saveButton->setObjectName("saveButton");
	saveButton->setStyleSheet(lightStyles.testwindowButtonStyle);
	bottomHLayout->addWidget(saveButton);

	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		saveButton->setText(QString::fromLocal8Bit("Сохранить"));
		break;
	case ENGLISH_LANG:
		saveButton->setText("Save");
		break;
	}
}


void ReportWindow::resizeEvent(QResizeEvent* event1)
{
	mainWidget->resize(geometry().width() - (LEFT_PADDING_MAIN_WIDGET * 2), geometry().height() - (UP_PADDING_MAIN_WIDGET * 2));
}

void ReportWindow::on_measuredCheckBox_stateChanged(int state)
{
	resetUiAdditionalColumns();
	resetUiFillColumns();
}

void ReportWindow::on_checkManualStandCheckBox_stateChanged(int state)
{
	resetUiAdditionalColumns();
	resetUiFillColumns();
}

void ReportWindow::on_checkPCAutoStandCheckBox_stateChanged(int state)
{
	resetUiAdditionalColumns();
	resetUiFillColumns();
}

void ReportWindow::on_checkStandAutoStandCheckBox_stateChanged(int state)
{
	resetUiAdditionalColumns();
	resetUiFillColumns();
}

void ReportWindow::on_saveButton_clicked()
{

}
