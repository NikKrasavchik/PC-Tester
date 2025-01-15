#include "ReportWindow.h"


ReportWindow::ReportWindow(std::vector<Cable> cables, WindowType testType)
{
	this->cables = cables;
	this->testType = testType;
	measured.current = -1;
	measured.voltage = -1;
	for (int i = 0; i < sizeof(changedThresholds) / sizeof(changedThresholds[0]); i++)
		changedThresholds[i] = -1;

	initUi();

	QMetaObject::connectSlotsByName(this);
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
	//mainTableWidget->setSelectionMode(QAbstractItemView::NoSelection);
	mainTableWidget->horizontalHeader()->hide();
	mainTableWidget->verticalHeader()->hide();
	mainTableWidget->setRowCount(3);
	mainTableWidget->setColumnCount(9);

	QFont font = QFont();
	font.setBold(true);
	font.setPointSizeF(10);
	// Pad
	mainTableWidget->setSpan(CEll_PAD_TABLE, SPAN_COUNT_3, SPAN_COUNT_1);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CEll_PAD_TABLE), "");
	mainTableWidget->item(CEll_PAD_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CEll_PAD_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(0, 0)->setFont(font);
	// Pin
	mainTableWidget->setSpan(CELL_PIN_TABLE, SPAN_COUNT_3, SPAN_COUNT_1);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_PIN_TABLE), "");
	mainTableWidget->item(CELL_PIN_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_PIN_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_PIN_TABLE)->setFont(font);
	// Id
	mainTableWidget->setSpan(CELL_ID_TABLE, SPAN_COUNT_3, SPAN_COUNT_1);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_ID_TABLE), "");
	mainTableWidget->item(CELL_ID_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_ID_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_ID_TABLE)->setFont(font);
	// Type
	mainTableWidget->setSpan(CELL_TYPE_TABLE, SPAN_COUNT_3, SPAN_COUNT_1);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_TYPE_TABLE), "");
	mainTableWidget->item(CELL_TYPE_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_TYPE_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_TYPE_TABLE)->setFont(font);
	// Name
	mainTableWidget->setSpan(CELL_NAME_TABLE, SPAN_COUNT_3, SPAN_COUNT_1);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_NAME_TABLE), "");
	mainTableWidget->item(CELL_NAME_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_NAME_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_NAME_TABLE)->setFont(font);
	// Progs
	mainTableWidget->setSpan(CELL_TRESHHOLDERS_TABLE, SPAN_COUNT_1, SPAN_COUNT_4);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_TRESHHOLDERS_TABLE), "");
	mainTableWidget->item(CELL_TRESHHOLDERS_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_TRESHHOLDERS_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_TRESHHOLDERS_TABLE)->setFont(font);
	// Progs U
	mainTableWidget->setSpan(CELL_TRESHHOLDERS_U_TABLE, SPAN_COUNT_1, SPAN_COUNT_2);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_TRESHHOLDERS_U_TABLE), "");
	mainTableWidget->item(CELL_TRESHHOLDERS_U_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_TRESHHOLDERS_U_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_TRESHHOLDERS_U_TABLE)->setFont(font);
	// Progs I
	mainTableWidget->setSpan(CELL_TRESHHOLDERS_I_TABLE, SPAN_COUNT_1, SPAN_COUNT_2);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_TRESHHOLDERS_I_TABLE), "");
	mainTableWidget->item(CELL_TRESHHOLDERS_I_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_TRESHHOLDERS_I_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_TRESHHOLDERS_I_TABLE)->setFont(font);
	// Progs U min
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_TRESHHOLDERS_U_MIN_TABLE), "");
	mainTableWidget->item(CELL_TRESHHOLDERS_U_MIN_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_TRESHHOLDERS_U_MIN_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_TRESHHOLDERS_U_MIN_TABLE)->setFont(font);
	// Progs U max
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_TRESHHOLDERS_U_MAX_TABLE), "");
	mainTableWidget->item(CELL_TRESHHOLDERS_U_MAX_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_TRESHHOLDERS_U_MAX_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_TRESHHOLDERS_U_MAX_TABLE)->setFont(font);
	// Progs I min
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_TRESHHOLDERS_I_MIN_TABLE), "");
	mainTableWidget->item(CELL_TRESHHOLDERS_I_MIN_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_TRESHHOLDERS_I_MIN_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_TRESHHOLDERS_I_MIN_TABLE)->setFont(font);
	// Progs I max
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_TRESHHOLDERS_I_MAX_TABLE), "");
	mainTableWidget->item(CELL_TRESHHOLDERS_I_MAX_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_TRESHHOLDERS_I_MAX_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_TRESHHOLDERS_I_MAX_TABLE)->setFont(font);

	mainTableWidget->setColumnWidth(COLUMN_ID, 75);
	mainTableWidget->setColumnWidth(COLUMN_PAD, 75);
	mainTableWidget->setColumnWidth(COLUMN_PIN, 75);
	mainTableWidget->setColumnWidth(COLUMN_TYPE, 75);
	mainTableWidget->setColumnWidth(COLUMN_NAME, 75);
	mainTableWidget->setColumnWidth(COLUMN_TRESHHOLDERS_U_MIN, 50);
	mainTableWidget->setColumnWidth(COLUMN_TRESHHOLDERS_U_MAX, 50);
	mainTableWidget->setColumnWidth(COLUMN_TRESHHOLDERS_I_MIN, 50);
	mainTableWidget->setColumnWidth(COLUMN_TRESHHOLDERS_I_MAX, 50);

	mainTableWidget->setRowHeight(0, 40);
	mainTableWidget->setRowHeight(1, 40);
	mainTableWidget->setRowHeight(2, 40);

	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_ID, QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_PAD, QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_PIN, QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_TYPE, QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_NAME, QHeaderView::Stretch);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_TRESHHOLDERS, QHeaderView::Fixed);

	mainTableWidget->verticalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
	mainTableWidget->verticalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
	mainTableWidget->verticalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
}

void ReportWindow::initUiSetValueTable()
{
	QString typeTmp;
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		mainTableWidget->item(CELL_ID_TABLE)->setText("Id");
		mainTableWidget->item(CEll_PAD_TABLE)->setText(QString::fromLocal8Bit("Колодка"));
		mainTableWidget->item(CELL_PIN_TABLE)->setText(QString::fromLocal8Bit("Пин"));
		mainTableWidget->item(CELL_TYPE_TABLE)->setText(QString::fromLocal8Bit("Тип"));
		mainTableWidget->item(CELL_NAME_TABLE)->setText(QString::fromLocal8Bit("Название"));
		mainTableWidget->item(CELL_TRESHHOLDERS_TABLE)->setText(QString::fromLocal8Bit("Пороги"));
		mainTableWidget->item(CELL_TRESHHOLDERS_U_TABLE)->setText(QString::fromLocal8Bit("U, В"));
		mainTableWidget->item(CELL_TRESHHOLDERS_I_TABLE)->setText(QString::fromLocal8Bit("I, А"));
		mainTableWidget->item(CELL_TRESHHOLDERS_U_MIN_TABLE)->setText(QString::fromLocal8Bit("Мин"));
		mainTableWidget->item(CELL_TRESHHOLDERS_U_MAX_TABLE)->setText(QString::fromLocal8Bit("Макс"));
		mainTableWidget->item(CELL_TRESHHOLDERS_I_MIN_TABLE)->setText(QString::fromLocal8Bit("Мин"));
		mainTableWidget->item(CELL_TRESHHOLDERS_I_MAX_TABLE)->setText(QString::fromLocal8Bit("Макс"));
		break;

	case ENGLISH_LANG:
		mainTableWidget->item(CELL_ID_TABLE)->setText("Id");
		mainTableWidget->item(CEll_PAD_TABLE)->setText("Pad");
		mainTableWidget->item(CELL_PIN_TABLE)->setText("Pin");
		mainTableWidget->item(CELL_TYPE_TABLE)->setText("Type");
		mainTableWidget->item(CELL_NAME_TABLE)->setText("Name");
		mainTableWidget->item(CELL_TRESHHOLDERS_TABLE)->setText("TRESHHOLDERS");
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
			case COLUMN_ID:
				model->setData(model->index(row, column), QString::number(cables[row - ROW_3].id));
				break;

			case COLUMN_PAD:
				model->setData(model->index(row, column), QString::number((int)cables[row - ROW_3].connector));
				break;

			case COLUMN_PIN:
				model->setData(model->index(row, column), QString::number(cables[row - ROW_3].pin));
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

			case COLUMN_NAME:
				model->setData(model->index(row, column), QString(cables[row - ROW_3].name));
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
		}
	}
}

void ReportWindow::fillColumnsSetters()
{
	switch (testType)
	{
	case WindowType::IN_TEST_MANUAL_STAND:
		activeColumn.measured				= true;
		activeColumn.checkManualStand		= true;
		activeColumn.checkPCAutoStand		= true;
		activeColumn.checkStandAutoStand	= true;
		break;

	case WindowType::OUT_TEST_MANUAL_STAND:
		activeColumn.measured				= true;
		activeColumn.checkManualStand		= true;
		activeColumn.checkPCAutoStand		= true;
		activeColumn.checkStandAutoStand	= true;
		break;

	case WindowType::FULL_TEST_MANUAL_STAND:
		activeColumn.measured				= true;
		activeColumn.checkManualStand		= true;
		activeColumn.checkPCAutoStand		= true;
		activeColumn.checkStandAutoStand	= true;
		break;

	case WindowType::IN_MANUAL_TEST_AUTO_STAND:
		activeColumn.measured				= true;
		activeColumn.checkManualStand		= true;
		activeColumn.checkPCAutoStand		= true;
		activeColumn.checkStandAutoStand	= true;
		break;

	case WindowType::OUT_MANUAL_TEST_AUTO_STAND:
		activeColumn.measured				= true;
		activeColumn.checkManualStand		= true;
		activeColumn.checkPCAutoStand		= true;
		activeColumn.checkStandAutoStand	= true;
		break;

	case WindowType::IN_AUTO_TEST_AUTO_STAND:
		activeColumn.measured				= true;
		activeColumn.checkManualStand		= true;
		activeColumn.checkPCAutoStand		= true;
		activeColumn.checkStandAutoStand	= true;
		break;

	case WindowType::OUT_AUTO_TEST_AUTO_STAND:
		activeColumn.measured				= true;
		activeColumn.checkManualStand		= true;
		activeColumn.checkPCAutoStand		= true;
		activeColumn.checkStandAutoStand	= true;
		break;

	case WindowType::FULL_TEST_AUTO_STAND:
		activeColumn.measured				= true;
		activeColumn.checkManualStand		= true;
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
		measuredLabel->setText("Measured column"); // move to reset func
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
		checkManualStandLabel->setText("Manual stand check column"); // move to reset func
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
		checkPCAutoStandLabel->setText("PC check in auto stand column"); // move to reset func
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
		checkStandAutoStandLabel->setText("Stand check in auto stand column"); // move to reset func
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

	if (measuredCheckBox->isChecked())
	{
		int currentColumn = mainTableWidget->columnCount();
		int currentColumn_U = mainTableWidget->columnCount();
		int currentColumn_I = mainTableWidget->columnCount() + 1;

		mainTableWidget->insertColumn(currentColumn_U);
		mainTableWidget->insertColumn(currentColumn_I);

		// Measured value
		mainTableWidget->setSpan(ROW_0, currentColumn, SPAN_COUNT_2, SPAN_COUNT_2);
		mainTableWidget->model()->setData(mainTableWidget->model()->index(ROW_0, currentColumn), "");
		mainTableWidget->item(ROW_0, currentColumn)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(ROW_0, currentColumn)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(ROW_0, currentColumn)->setFont(font);
		// Measured value U
		mainTableWidget->model()->setData(mainTableWidget->model()->index(ROW_2, currentColumn_U), "U, V");
		mainTableWidget->item(ROW_2, currentColumn_U)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(ROW_2, currentColumn_U)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(ROW_2, currentColumn_U)->setFont(font);
		// Measured value I
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

	if (checkManualStandCheckBox->isChecked())
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

	if (checkPCAutoStandCheckBox->isChecked())
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

	if (checkStandAutoStandCheckBox->isChecked())
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
	int measuredAdditionalInd = -1;
	int checkManualStandAdditionalInd = -1;
	int checkPCAutoStandAdditionalInd = -1;
	int checkStandAutoStandAdditionalInd = -1;

	int currentAdditionalInd = 9;
	if (measuredCheckBox->isChecked())
	{
		measuredAdditionalInd = currentAdditionalInd;
		currentAdditionalInd += 2;
	}
	if (checkManualStandCheckBox->isChecked())
	{
		checkManualStandAdditionalInd = currentAdditionalInd;
		currentAdditionalInd++;
	}
	if (checkPCAutoStandCheckBox->isChecked())
	{
		checkPCAutoStandAdditionalInd = currentAdditionalInd;
		currentAdditionalInd++;
	}
	if (checkStandAutoStandCheckBox->isChecked())
	{
		checkStandAutoStandAdditionalInd = currentAdditionalInd;
		currentAdditionalInd++;
	}

	QAbstractItemModel* model = mainTableWidget->model();
	for (int row = ROW_3; row < cables.size() + ROW_3; row++)
		for (int column = CLEAR_COLUMN_COUNT - 1; column < mainTableWidget->columnCount(); column++)
		{
			if (measuredAdditionalInd != -1 && measuredAdditionalInd == column)
			{
				model->setData(model->index(row, column), 1);
				model->setData(model->index(row, column + 1), 1);
				column++;
			}
			if (checkManualStandAdditionalInd != -1 && checkManualStandAdditionalInd == column)
			{
				model->setData(model->index(row, column), 2);
			}
			if (checkPCAutoStandAdditionalInd != -1 && checkPCAutoStandAdditionalInd == column)
			{
				model->setData(model->index(row, column), 3);
			}
			if (checkStandAutoStandAdditionalInd != -1 && checkStandAutoStandAdditionalInd == column)
			{
				model->setData(model->index(row, column), "4");
			}
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

void ReportWindow::on_saveChangesButton_clicked()
{
	//QMessageBox msgBox;
	//msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
	//msgBox.setDefaultButton(QMessageBox::Save);
	//msgBox.setIcon(QMessageBox::Question);
	//msgBox.setMinimumSize(200, 100);
	//switch (viewWindowState->appLanguage)
	//{
	//case RUSSIAN_LANG:
	//	msgBox.setText(QString::fromLocal8Bit("Сохранить изменения?"));
	//	msgBox.setInformativeText(QString::fromLocal8Bit("Изменения будут внесены в конфигурационный файл."));
	//	break;
	//case ENGLISH_LANG:

	//	break;
	//}
	//if (msgBox.exec() == QMessageBox::Save)
	//{
	//	std::vector<Cable> cablesTmp;
	//	for (int i = 0; i < sizeof(changedThresholds) / sizeof(changedThresholds[0]); i++)
	//	{
	//		if (changedThresholds[i] != -1)
	//		{
	//			Cable cableTmp;
	//			cableTmp.id = cable.id;
	//			cableTmp.connector = cable.connector;
	//			cableTmp.pin = cable.pin;
	//			cableTmp.direction = cable.direction;
	//			cableTmp.type = cable.type;
	//			cableTmp.canId = cable.canId;
	//			cableTmp.bit = cable.bit;
	//			cableTmp.name = cable.name;
	//			cableTmp.component = cable.component;

	//			switch (i)
	//			{
	//			case 0:
	//				cableTmp.minVoltage = changedThresholds[i];
	//				break;
	//			case 1:
	//				cableTmp.maxVoltage = changedThresholds[i];
	//				break;
	//			case 2:
	//				cableTmp.minCurrent = changedThresholds[i];
	//				break;
	//			case 3:
	//				cableTmp.maxCurrent = changedThresholds[i];
	//				break;
	//			default:
	//				QMessageBox::critical(this, "error", "ReportWindow.cpp 357 on_saveChangesButton_clicked");
	//				return;
	//				break;
	//			}
	//			cablesTmp.push_back(cableTmp);
	//			changedThresholds[i] = -1;
	//		}
	//	}
	//	// Надо внести изменения в конфиг файл этих кабелей cablesTmp
	//	saveChangesButton->hide();
	//}
}

void ReportWindow::on_startTestButton_clicked()
{


}
