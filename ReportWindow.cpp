#include "ReportWindow.h"

ReportWindow::ReportWindow(std::vector<TestTableRowProperties*> cableRows)
{
	this->cableRows = cableRows;
	int t = 2;
	setMinimumSize(WINDOW_MIN_SIZE);
	resize(WINDOW_MIN_SIZE);

	initUi();
	QMetaObject::connectSlotsByName(this);
}

ReportWindow::~ReportWindow()
{
	
}

void ReportWindow::initUi()
{
	mainWidget = new QWidget(this);
	mainWidget->setObjectName("mainWidget");
	mainWidget->setGeometry(LEFT_PADDING_MAIN_WIDGET, UP_PADDING_MAIN_WIDGET, WINDOW_MIN_SIZE_WIDTH - (LEFT_PADDING_MAIN_WIDGET * 2), WINDOW_MIN_SIZE_HEIGHT - (UP_PADDING_MAIN_WIDGET * 2));

	mainVLayout = new QVBoxLayout(mainWidget);
	mainVLayout->setObjectName("mainVLayout");

	initUiTable();

	mainMiddleSpacer = new QSpacerItem(0, 10, QSizePolicy::Fixed);
	mainVLayout->addItem(mainMiddleSpacer);

	initUiFooter();

	//resetLanguage();
	resetTheme();

	generateTable();
}

void ReportWindow::initUiTable()
{
	tableWidget = new QTableWidget(mainWidget);
	tableWidget->setObjectName("tableWidget");
	tableWidget->horizontalHeader()->hide();
	tableWidget->verticalHeader()->hide();
	mainVLayout->addWidget(tableWidget);
}

void ReportWindow::initUiFooter()
{
	footerHLayout = new QHBoxLayout(mainWidget);
	footerHLayout->setObjectName("footerHLayout");
	mainVLayout->addLayout(footerHLayout);

	footerLeftSpacer = new QSpacerItem(10, 0, QSizePolicy::Expanding);
	footerHLayout->addItem(footerLeftSpacer);

	saveButton = new QPushButton(mainWidget);
	saveButton->setObjectName("saveButton");
	saveButton->setFixedSize(BUTTON_SIZE);
	footerHLayout->addWidget(saveButton);

	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		saveButton->setText(QString::fromLocal8Bit("Сохранить"));
		break;

	case ENGLISH_LANG:
		saveButton->setText(QString("Save"));
		break;
	}
}

void ReportWindow::resetBaseLanguage()
{
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		tableWidget->item(CELL_SIGN_BASE_CONNECTOR)->setText(QString::fromLocal8Bit("Колодка"));
		tableWidget->item(CELL_SIGN_BASE_PIN)->setText(QString::fromLocal8Bit("Пин"));
		tableWidget->item(CELL_SIGN_BASE_DIRECTION)->setText(QString::fromLocal8Bit("Направление"));
		tableWidget->item(CELL_SIGN_BASE_TYPE)->setText(QString::fromLocal8Bit("Тип"));
		tableWidget->item(CELL_SIGN_BASE_NAME)->setText(QString::fromLocal8Bit("Название"));
		tableWidget->item(CELL_SIGN_BASE_EMPTY)->setText(QString::fromLocal8Bit(""));
		tableWidget->item(CELL_SIGN_BASE_COMMENT)->setText(QString::fromLocal8Bit("Комментарий"));
		break;

	case ENGLISH_LANG:
		tableWidget->item(CELL_SIGN_BASE_CONNECTOR)->setText(QString("Connector"));
		tableWidget->item(CELL_SIGN_BASE_PIN)->setText(QString("Pin"));
		tableWidget->item(CELL_SIGN_BASE_DIRECTION)->setText(QString("Direction"));
		tableWidget->item(CELL_SIGN_BASE_TYPE)->setText(QString("Type"));
		tableWidget->item(CELL_SIGN_BASE_NAME)->setText(QString("Name"));
		tableWidget->item(CELL_SIGN_BASE_EMPTY)->setText(QString(""));
		tableWidget->item(CELL_SIGN_BASE_COMMENT)->setText(QString("Comment"));
		break;
	}
}

void ReportWindow::resetTheme()
{
	switch (viewWindowState->appTheme)
	{
	case LIGHT_THEME:
		tableWidget->setStyleSheet(lightStyles.testwindowTableWidget);
		saveButton->setStyleSheet(lightStyles.testwindowButtonStyle);
		break;

	case DARK_THEME:
		tableWidget->setStyleSheet(darkStyles.testwindowTableWidget);
		saveButton->setStyleSheet(darkStyles.testwindowButtonStyle);
		break;
	}
}

void ReportWindow::resizeEvent(QResizeEvent* event)
{
	mainWidget->setGeometry(LEFT_PADDING_MAIN_WIDGET, UP_PADDING_MAIN_WIDGET, geometry().width() - (LEFT_PADDING_MAIN_WIDGET * 2), geometry().height() - (UP_PADDING_MAIN_WIDGET * 2));
}

static int getMaxColumnOffset(std::vector<TestTableRowProperties*> cableRows)
{
	int maxColumnOffset = 0;

	for (int i = 0; i < cableRows.size(); i++)
		switch (cableRows[i]->typeInt)
		{
		case TypeCable::DIG_OUT:
		case TypeCable::PWM_OUT:
		case TypeCable::VNH_OUT:
			if (cableRows[i]->thresholds.size() * MEASUREMENT_OFFSET_OUT > maxColumnOffset)
				maxColumnOffset = cableRows[i]->thresholds.size() * MEASUREMENT_OFFSET_OUT;
			break;

		case TypeCable::DIG_IN:
		case TypeCable::HALL_IN:
			if (MEASUREMENT_OFFSET_IN > maxColumnOffset)
				maxColumnOffset = MEASUREMENT_OFFSET_IN;
			break;

		case TypeCable::ANALOG_IN:
			if (cableRows[i]->thresholds.size() * MEASUREMENT_OFFSET_IN_ANALOG > maxColumnOffset)
				maxColumnOffset = cableRows[i]->thresholds.size() * MEASUREMENT_OFFSET_OUT;
			break;
		}

	return maxColumnOffset;
}

static int getMaxTypeOffset(std::vector<TestTableRowProperties*> cableRows)
{
	int maxTypeOffset = 0;
	for (int i = 0; i < cableRows.size(); i++)
		if (maxTypeOffset < cableRows[i]->thresholds.size())
			maxTypeOffset = cableRows[i]->thresholds.size();
	return maxTypeOffset;
}

static void prepareItem(QTableWidget* tableWidget, int row, int column, int rowSpan, int columnSpan)
{
	QFont font;
	font.setBold(true);
	font.setPointSizeF(10);

	tableWidget->setSpan(row, column, rowSpan, columnSpan);
	tableWidget->model()->setData(tableWidget->model()->index(row, column), "");
	tableWidget->item(row, column)->setTextAlignment(Qt::AlignCenter);
	tableWidget->item(row, column)->setFlags(Qt::ItemIsSelectable);
	tableWidget->item(row, column)->setFont(font);
}

void ReportWindow::generateTable()
{
	generateTableBaseSign();

	int maxColumnOffset = getMaxColumnOffset(cableRows);
	for (int i = 0; i < maxColumnOffset; i++)
	{
		tableWidget->insertColumn(MEASUREMENT_COLUMN_POSITION);
		tableWidget->setColumnWidth(MEASUREMENT_COLUMN_POSITION, 50);
	}
	tableWidget->setSpan(IND_ROW_BASE_SIGN, MEASUREMENT_COLUMN_POSITION, 1, maxColumnOffset);

	std::vector<std::vector<TestTableRowProperties*>> typedCableRows(TYPE_COUNT);
	for (int i = 0; i < cableRows.size(); i++)
		typedCableRows[(int)cableRows[i]->typeInt].push_back(cableRows[i]);

	for (int type = 0; type < TYPE_COUNT; type++)
	{
		if (!typedCableRows[type].size())
			continue;

		int previousRowCount = tableWidget->rowCount();
		int maxTypeOffset = getMaxTypeOffset(typedCableRows[type]);
		generateTableSign((TypeCable)type, maxTypeOffset);
		fillTable((TypeCable)type, typedCableRows[type]);
		int emptySpanRow = tableWidget->rowCount() - previousRowCount;
		int emptySpanColumn = COLUMN_COUNT_BASE_TABLE - 1;
		switch ((TypeCable)type)
		{
		case TypeCable::DIG_IN:
		case TypeCable::HALL_IN:
			emptySpanColumn += MEASUREMENT_OFFSET_IN * MEASUREMENT_OFFSET_DOUBLE;
			break;

		case TypeCable::ANALOG_IN:
			emptySpanColumn += MEASUREMENT_OFFSET_IN_ANALOG * maxTypeOffset;
			break;

		case TypeCable::DIG_OUT:
		case TypeCable::PWM_OUT:
		case TypeCable::VNH_OUT:
			emptySpanColumn += MEASUREMENT_OFFSET_OUT * maxTypeOffset;
			break;
		}
		prepareItem(tableWidget, previousRowCount, emptySpanColumn, emptySpanRow, tableWidget->columnCount() - emptySpanColumn - 1);
	}
}

void ReportWindow::generateTableBaseSign()
{
	tableWidget->setRowCount(ROW_COUNT_BASE_TABLE);
	tableWidget->setColumnCount(COLUMN_COUNT_BASE_TABLE);

	prepareItem(tableWidget, CELL_SIGN_BASE_CONNECTOR, SPAN_NONE);
	prepareItem(tableWidget, CELL_SIGN_BASE_PIN, SPAN_NONE);
	prepareItem(tableWidget, CELL_SIGN_BASE_DIRECTION, SPAN_NONE);
	prepareItem(tableWidget, CELL_SIGN_BASE_TYPE, SPAN_NONE);
	prepareItem(tableWidget, CELL_SIGN_BASE_NAME, SPAN_NONE);
	prepareItem(tableWidget, CELL_SIGN_BASE_EMPTY, SPAN_NONE);
	prepareItem(tableWidget, CELL_SIGN_BASE_COMMENT, SPAN_NONE);

	tableWidget->setColumnWidth(IND_COLUMN_BASE_CONNECTOR, 65);
	tableWidget->setColumnWidth(IND_COLUMN_BASE_PIN, 65);
	tableWidget->setColumnWidth(IND_COLUMN_BASE_DIRECTION, 120);
	tableWidget->setColumnWidth(IND_COLUMN_BASE_TYPE, 75);
	tableWidget->setColumnWidth(IND_COLUMN_BASE_EMPTY, 65);
	tableWidget->setColumnWidth(IND_COLUMN_BASE_COMMENT, 150);

	tableWidget->setRowHeight(IND_ROW_BASE_SIGN, 40);
	tableWidget->setRowHeight(IND_ROW_BASE_VALUE, 40);

	tableWidget->horizontalHeader()->setSectionResizeMode(IND_COLUMN_BASE_CONNECTOR, QHeaderView::Fixed);
	tableWidget->horizontalHeader()->setSectionResizeMode(IND_COLUMN_BASE_PIN, QHeaderView::Fixed);
	tableWidget->horizontalHeader()->setSectionResizeMode(IND_COLUMN_BASE_DIRECTION, QHeaderView::Fixed);
	tableWidget->horizontalHeader()->setSectionResizeMode(IND_COLUMN_BASE_TYPE, QHeaderView::Fixed);
	tableWidget->horizontalHeader()->setSectionResizeMode(IND_COLUMN_BASE_NAME, QHeaderView::Stretch);
	tableWidget->horizontalHeader()->setSectionResizeMode(IND_COLUMN_BASE_EMPTY, QHeaderView::Fixed);
	tableWidget->horizontalHeader()->setSectionResizeMode(IND_COLUMN_BASE_COMMENT, QHeaderView::Fixed);

	tableWidget->verticalHeader()->setSectionResizeMode(IND_ROW_BASE_SIGN, QHeaderView::Fixed);

	resetBaseLanguage();
}

static void generateTableSignOut(QTableWidget* tableWidget, int maxTypeOffset)
{
	int indRowMeasured = tableWidget->rowCount();
	int indRowMeasuredValues = tableWidget->rowCount() + 1;
	int indRowMeasuredVoltage = tableWidget->rowCount() + 3;
	int indRowMeasuredCurrent = tableWidget->rowCount() + 3;
	int indRowThresholds = tableWidget->rowCount() + 1;
	int indRowThresholdsVoltage = tableWidget->rowCount() + 2;
	int indRowThresholdsCurrent = tableWidget->rowCount() + 2;
	int indRowThresholdsVoltageMin = tableWidget->rowCount() + 3;
	int indRowThresholdsVoltageMax = tableWidget->rowCount() + 3;
	int indRowThresholdsCurrentMin = tableWidget->rowCount() + 3;
	int indRowThresholdsCurrentMax = tableWidget->rowCount() + 3;

	int indColumnMeasured = MEASUREMENT_COLUMN_POSITION;
	int indColumnMeasuredValues = MEASUREMENT_COLUMN_POSITION;
	int indColumnMeasuredValuesVoltage = MEASUREMENT_COLUMN_POSITION;
	int indColumnMeasuredValuesCurrent = MEASUREMENT_COLUMN_POSITION + 1;
	int indColumnThresholds = MEASUREMENT_COLUMN_POSITION + 2;
	int indColumnThresholdsVoltage = MEASUREMENT_COLUMN_POSITION + 2;
	int indColumnThresholdsCurrent = MEASUREMENT_COLUMN_POSITION + 4;
	int indColumnThresholdsVoltageMin = MEASUREMENT_COLUMN_POSITION + 2;
	int indColumnThresholdsVoltageMax = MEASUREMENT_COLUMN_POSITION + 3;
	int indColumnThresholdsCurrentMin = MEASUREMENT_COLUMN_POSITION + 4;
	int indColumnThresholdsCurrentMax = MEASUREMENT_COLUMN_POSITION + 5;

	for (int i = 0; i < ROW_COUNT_SIGN_OUT; i++)
		tableWidget->insertRow(tableWidget->rowCount());

	tableWidget->setSpan(indRowMeasured, 0, SPAN_TYPE_OUT);
	tableWidget->setSpan(indRowMeasured, tableWidget->columnCount() - 1, SPAN_TYPE_COMMENT_OUT);

	for (int i = 0; i < maxTypeOffset; i++)
	{
		prepareItem(tableWidget, indRowMeasured, indColumnMeasured, SPAN_HORIZONTAL_SEXTUPLE);
		prepareItem(tableWidget, indRowMeasuredValues, indColumnMeasuredValues, SPAN_SQUAD_DOUBLE);
		prepareItem(tableWidget, indRowMeasuredVoltage, indColumnMeasuredValuesVoltage, SPAN_NONE);
		prepareItem(tableWidget, indRowMeasuredCurrent, indColumnMeasuredValuesCurrent, SPAN_NONE);
		prepareItem(tableWidget, indRowThresholds, indColumnThresholds, SPAN_HORIZONTAL_QUADRUPLE);
		prepareItem(tableWidget, indRowThresholdsVoltage, indColumnThresholdsVoltage, SPAN_HORIZONTAL_DOUBLE);
		prepareItem(tableWidget, indRowThresholdsCurrent, indColumnThresholdsCurrent, SPAN_HORIZONTAL_DOUBLE);
		prepareItem(tableWidget, indRowThresholdsVoltageMin, indColumnThresholdsVoltageMin, SPAN_NONE);
		prepareItem(tableWidget, indRowThresholdsVoltageMax, indColumnThresholdsVoltageMax, SPAN_NONE);
		prepareItem(tableWidget, indRowThresholdsCurrentMin, indColumnThresholdsCurrentMin, SPAN_NONE);
		prepareItem(tableWidget, indRowThresholdsCurrentMax, indColumnThresholdsCurrentMax, SPAN_NONE);

		switch (viewWindowState->appLanguage)
		{
		case RUSSIAN_LANG:
			tableWidget->item(indRowMeasured, indColumnMeasured)->setText(QString(QString::fromLocal8Bit("Измерение ") + QString::number(i + 1)));
			tableWidget->item(indRowMeasuredValues, indColumnMeasuredValues)->setText(QString(QString::fromLocal8Bit("Измеренное значение")));
			tableWidget->item(indRowMeasuredVoltage, indColumnMeasuredValuesVoltage)->setText(QString(QString::fromLocal8Bit("U, В")));
			tableWidget->item(indRowMeasuredCurrent, indColumnMeasuredValuesCurrent)->setText(QString(QString::fromLocal8Bit("I, А")));
			tableWidget->item(indRowThresholds, indColumnThresholds)->setText(QString(QString::fromLocal8Bit("Пороги")));
			tableWidget->item(indRowThresholdsVoltage, indColumnThresholdsVoltage)->setText(QString(QString::fromLocal8Bit("U, В")));
			tableWidget->item(indRowThresholdsCurrent, indColumnThresholdsCurrent)->setText(QString(QString::fromLocal8Bit("I, А")));
			tableWidget->item(indRowThresholdsVoltageMin, indColumnThresholdsVoltageMin)->setText(QString(QString::fromLocal8Bit("Мин")));
			tableWidget->item(indRowThresholdsVoltageMax, indColumnThresholdsVoltageMax)->setText(QString(QString::fromLocal8Bit("Макс")));
			tableWidget->item(indRowThresholdsCurrentMin, indColumnThresholdsCurrentMin)->setText(QString(QString::fromLocal8Bit("Мин")));
			tableWidget->item(indRowThresholdsCurrentMax, indColumnThresholdsCurrentMax)->setText(QString(QString::fromLocal8Bit("Макс")));
			break;

		case ENGLISH_LANG:
			tableWidget->item(indRowMeasured, indColumnMeasured)->setText(QString("Measured ") + QString::number(i + 1));
			tableWidget->item(indRowMeasuredValues, indColumnMeasuredValues)->setText(QString("Measured values"));
			tableWidget->item(indRowMeasuredVoltage, indColumnMeasuredValuesVoltage)->setText(QString("U, V"));
			tableWidget->item(indRowMeasuredCurrent, indColumnMeasuredValuesCurrent)->setText(QString("I, А"));
			tableWidget->item(indRowThresholds, indColumnThresholds)->setText(QString("Thresholds"));
			tableWidget->item(indRowThresholdsVoltage, indColumnThresholdsVoltage)->setText(QString("U, V"));
			tableWidget->item(indRowThresholdsCurrent, indColumnThresholdsCurrent)->setText(QString("I, А"));
			tableWidget->item(indRowThresholdsVoltageMin, indColumnThresholdsVoltageMin)->setText(QString("Min"));
			tableWidget->item(indRowThresholdsVoltageMax, indColumnThresholdsVoltageMax)->setText(QString("Max"));
			tableWidget->item(indRowThresholdsCurrentMin, indColumnThresholdsCurrentMin)->setText(QString("Min"));
			tableWidget->item(indRowThresholdsCurrentMax, indColumnThresholdsCurrentMax)->setText(QString("Max"));
			break;
		}

		indColumnMeasured += MEASUREMENT_OFFSET_OUT;
		indColumnMeasuredValues += MEASUREMENT_OFFSET_OUT;
		indColumnMeasuredValuesVoltage += MEASUREMENT_OFFSET_OUT;
		indColumnMeasuredValuesCurrent += MEASUREMENT_OFFSET_OUT;
		indColumnThresholds += MEASUREMENT_OFFSET_OUT;
		indColumnThresholdsVoltage += MEASUREMENT_OFFSET_OUT;
		indColumnThresholdsCurrent += MEASUREMENT_OFFSET_OUT;
		indColumnThresholdsVoltageMin += MEASUREMENT_OFFSET_OUT;
		indColumnThresholdsVoltageMax += MEASUREMENT_OFFSET_OUT;
		indColumnThresholdsCurrentMin += MEASUREMENT_OFFSET_OUT;
		indColumnThresholdsCurrentMax += MEASUREMENT_OFFSET_OUT;
	}
}

static void generateTableSignIn(QTableWidget* tableWidget)
{
	int indRowMeasured = tableWidget->rowCount();

	int indColumnMeasured1 = MEASUREMENT_COLUMN_POSITION;
	int indColumnMeasured2 = MEASUREMENT_COLUMN_POSITION + 2;

	tableWidget->insertRow(tableWidget->rowCount());

	tableWidget->setSpan(indRowMeasured, 0, SPAN_TYPE_IN);
	tableWidget->setSpan(indRowMeasured, tableWidget->columnCount() - 1, SPAN_TYPE_COMMENT_IN);

	prepareItem(tableWidget, indRowMeasured, indColumnMeasured1, SPAN_HORIZONTAL_DOUBLE);
	prepareItem(tableWidget, indRowMeasured, indColumnMeasured2, SPAN_HORIZONTAL_DOUBLE);

	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		tableWidget->item(indRowMeasured, indColumnMeasured1)->setText(QString::fromLocal8Bit("Значение 1"));
		tableWidget->item(indRowMeasured, indColumnMeasured2)->setText(QString::fromLocal8Bit("Значение 2"));
		break;

	case ENGLISH_LANG:
		tableWidget->item(indRowMeasured, indColumnMeasured1)->setText(QString("Value 1"));
		tableWidget->item(indRowMeasured, indColumnMeasured2)->setText(QString("Value 2"));
		break;
	}
}

static void generateTableSignInAnalog(QTableWidget* tableWidget, int maxTypeOffset)
{
	int indRowMeasured = tableWidget->rowCount();
	int indRowMeasuredValues = tableWidget->rowCount() + 1;
	int indRowThresholds = tableWidget->rowCount() + 1;
	int indRowThresholdsMin = tableWidget->rowCount() + 2;
	int indRowThresholdsMax = tableWidget->rowCount() + 2;

	int indColumnMeasured = MEASUREMENT_COLUMN_POSITION;
	int indColumnMeasuredValues = MEASUREMENT_COLUMN_POSITION;
	int indColumnThresholds = MEASUREMENT_COLUMN_POSITION + 2;
	int indColumnThresholdsMin = MEASUREMENT_COLUMN_POSITION + 2;
	int indColumnThresholdsMax = MEASUREMENT_COLUMN_POSITION + 3;

	for (int i = 0; i < ROW_COUNT_SIGN_IN_ANALOG; i++)
		tableWidget->insertRow(tableWidget->rowCount());

	tableWidget->setSpan(indRowMeasured, 0, SPAN_TYPE_IN_ANALOG);
	tableWidget->setSpan(indRowMeasured, tableWidget->columnCount() - 1, SPAN_TYPE_COMMENT_IN_ANALOG);

	for (int i = 0; i < maxTypeOffset; i++)
	{
		prepareItem(tableWidget, indRowMeasured, indColumnMeasured, SPAN_HORIZONTAL_QUADRUPLE);
		prepareItem(tableWidget, indRowMeasuredValues, indColumnMeasuredValues, SPAN_SQUAD_DOUBLE);
		prepareItem(tableWidget, indRowThresholds, indColumnThresholds, SPAN_HORIZONTAL_DOUBLE);
		prepareItem(tableWidget, indRowThresholdsMin, indColumnThresholdsMin, SPAN_NONE);
		prepareItem(tableWidget, indRowThresholdsMax, indColumnThresholdsMax, SPAN_NONE);

		switch (viewWindowState->appLanguage)
		{
		case RUSSIAN_LANG:
			tableWidget->item(indRowMeasured, indColumnMeasured)->setText(QString(QString::fromLocal8Bit("Измерение ") + QString::number(i + 1)));
			tableWidget->item(indRowMeasuredValues, indColumnMeasuredValues)->setText(QString(QString::fromLocal8Bit("Измеренное значение")));
			tableWidget->item(indRowThresholds, indColumnThresholds)->setText(QString(QString::fromLocal8Bit("Пороги")));
			tableWidget->item(indRowThresholdsMin, indColumnThresholdsMin)->setText(QString(QString::fromLocal8Bit("Мин")));
			tableWidget->item(indRowThresholdsMax, indColumnThresholdsMax)->setText(QString(QString::fromLocal8Bit("Макс")));
			break;

		case ENGLISH_LANG:
			tableWidget->item(indRowMeasured, indColumnMeasured)->setText(QString("Measured ") + QString::number(i + 1));
			tableWidget->item(indRowMeasuredValues, indColumnMeasuredValues)->setText(QString("Measured values"));
			tableWidget->item(indRowThresholds, indColumnThresholds)->setText(QString("Thresholds"));
			tableWidget->item(indRowThresholdsMin, indColumnThresholdsMin)->setText(QString("Min"));
			tableWidget->item(indRowThresholdsMax, indColumnThresholdsMax)->setText(QString("Max"));
			break;
		}

		indColumnMeasured += MEASUREMENT_OFFSET_IN_ANALOG;
		indColumnMeasuredValues += MEASUREMENT_OFFSET_IN_ANALOG;
		indColumnThresholds += MEASUREMENT_OFFSET_IN_ANALOG;
		indColumnThresholdsMin += MEASUREMENT_OFFSET_IN_ANALOG;
		indColumnThresholdsMax += MEASUREMENT_OFFSET_IN_ANALOG;
	}
}

void ReportWindow::generateTableSign(TypeCable type, int maxTypeOffset)
{
	switch (type)
	{
	case TypeCable::DIG_OUT:
	case TypeCable::PWM_OUT:
	case TypeCable::VNH_OUT:
		generateTableSignOut(tableWidget, maxTypeOffset);
		break;

	case TypeCable::DIG_IN:
	case TypeCable::HALL_IN:
		generateTableSignIn(tableWidget);
		break;

	case TypeCable::ANALOG_IN:
		generateTableSignInAnalog(tableWidget, maxTypeOffset);
		break;
	}
}

static void fillTableOut(QTableWidget* tableWidget, std::vector<TestTableRowProperties*> cableRows)
{
	for (int i = 0; i < cableRows.size(); i++)
	{
		int indCurrentRow = tableWidget->rowCount();
		tableWidget->insertRow(indCurrentRow);

		prepareItem(tableWidget, indCurrentRow, IND_COLUMN_BASE_CONNECTOR, SPAN_NONE);
		prepareItem(tableWidget, indCurrentRow, IND_COLUMN_BASE_PIN, SPAN_NONE);
		prepareItem(tableWidget, indCurrentRow, IND_COLUMN_BASE_DIRECTION, SPAN_NONE);
		prepareItem(tableWidget, indCurrentRow, IND_COLUMN_BASE_TYPE, SPAN_NONE);
		prepareItem(tableWidget, indCurrentRow, IND_COLUMN_BASE_NAME, SPAN_NONE);

		tableWidget->item(indCurrentRow, IND_COLUMN_BASE_CONNECTOR)->setText(cableRows[i]->connectorStr);
		tableWidget->item(indCurrentRow, IND_COLUMN_BASE_PIN)->setText(cableRows[i]->pin);
		tableWidget->item(indCurrentRow, IND_COLUMN_BASE_DIRECTION)->setText(cableRows[i]->direction);
		tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(cableRows[i]->typeStr);
		tableWidget->item(indCurrentRow, IND_COLUMN_BASE_NAME)->setText(cableRows[i]->name);

		int indColumnMeasuredValuesVoltage = MEASUREMENT_COLUMN_POSITION;
		int indColumnMeasuredValuesCurrent = MEASUREMENT_COLUMN_POSITION + 1;
		int indColumnThresholdsVoltageMin = MEASUREMENT_COLUMN_POSITION + 2;
		int indColumnThresholdsVoltageMax = MEASUREMENT_COLUMN_POSITION + 3;
		int indColumnThresholdsCurrentMin = MEASUREMENT_COLUMN_POSITION + 4;
		int indColumnThresholdsCurrentMax = MEASUREMENT_COLUMN_POSITION + 5;


		for (int j = 0; j < cableRows[i]->thresholds.size(); j++)
		{
			prepareItem(tableWidget, indCurrentRow, indColumnMeasuredValuesVoltage, SPAN_NONE);
			prepareItem(tableWidget, indCurrentRow, indColumnMeasuredValuesCurrent, SPAN_NONE);
			prepareItem(tableWidget, indCurrentRow, indColumnThresholdsVoltageMin, SPAN_NONE);
			prepareItem(tableWidget, indCurrentRow, indColumnThresholdsVoltageMax, SPAN_NONE);
			prepareItem(tableWidget, indCurrentRow, indColumnThresholdsCurrentMin, SPAN_NONE);
			prepareItem(tableWidget, indCurrentRow, indColumnThresholdsCurrentMax, SPAN_NONE);

			tableWidget->item(indCurrentRow, indColumnMeasuredValuesVoltage)->setText(cableRows[i]->measureds[j]->voltage != -1 ? QString::number(cableRows[i]->measureds[j]->current) : "-");
			tableWidget->item(indCurrentRow, indColumnMeasuredValuesCurrent)->setText(cableRows[i]->measureds[j]->current != -1 ? QString::number(cableRows[i]->measureds[j]->voltage) : "-");
			tableWidget->item(indCurrentRow, indColumnThresholdsVoltageMin)->setText(cableRows[i]->thresholds[j].minVoltage != -1 ? QString::number(cableRows[i]->thresholds[j].minVoltage) : "-");
			tableWidget->item(indCurrentRow, indColumnThresholdsVoltageMax)->setText(cableRows[i]->thresholds[j].maxVoltage != -1 ? QString::number(cableRows[i]->thresholds[j].maxVoltage) : "-");
			tableWidget->item(indCurrentRow, indColumnThresholdsCurrentMin)->setText(cableRows[i]->thresholds[j].minCurrent != -1 ? QString::number(cableRows[i]->thresholds[j].minCurrent) : "-");
			tableWidget->item(indCurrentRow, indColumnThresholdsCurrentMax)->setText(cableRows[i]->thresholds[j].maxCurrent != -1 ? QString::number(cableRows[i]->thresholds[j].maxCurrent) : "-");

			indColumnMeasuredValuesVoltage += MEASUREMENT_OFFSET_OUT;
			indColumnMeasuredValuesCurrent += MEASUREMENT_OFFSET_OUT;
			indColumnThresholdsVoltageMin += MEASUREMENT_OFFSET_OUT;
			indColumnThresholdsVoltageMax += MEASUREMENT_OFFSET_OUT;
			indColumnThresholdsCurrentMin += MEASUREMENT_OFFSET_OUT;
			indColumnThresholdsCurrentMax += MEASUREMENT_OFFSET_OUT;
		}
	}
}

static void fillTableIn(QTableWidget* tableWidget, std::vector<TestTableRowProperties*> cableRows)
{
	for (int i = 0; i < cableRows.size(); i++)
	{
		int indCurrentRow = tableWidget->rowCount();
		tableWidget->insertRow(indCurrentRow);

		prepareItem(tableWidget, indCurrentRow, IND_COLUMN_BASE_CONNECTOR, SPAN_NONE);
		prepareItem(tableWidget, indCurrentRow, IND_COLUMN_BASE_PIN, SPAN_NONE);
		prepareItem(tableWidget, indCurrentRow, IND_COLUMN_BASE_DIRECTION, SPAN_NONE);
		prepareItem(tableWidget, indCurrentRow, IND_COLUMN_BASE_TYPE, SPAN_NONE);
		prepareItem(tableWidget, indCurrentRow, IND_COLUMN_BASE_NAME, SPAN_NONE);

		tableWidget->item(indCurrentRow, IND_COLUMN_BASE_CONNECTOR)->setText(cableRows[i]->connectorStr);
		tableWidget->item(indCurrentRow, IND_COLUMN_BASE_PIN)->setText(cableRows[i]->pin);
		tableWidget->item(indCurrentRow, IND_COLUMN_BASE_DIRECTION)->setText(cableRows[i]->direction);
		tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(cableRows[i]->typeStr);
		tableWidget->item(indCurrentRow, IND_COLUMN_BASE_NAME)->setText(cableRows[i]->name);

		int indColumnMeasuredValue1 = MEASUREMENT_COLUMN_POSITION;
		int indColumnMeasuredValue2 = MEASUREMENT_COLUMN_POSITION + 2;

		prepareItem(tableWidget, indCurrentRow, indColumnMeasuredValue1, SPAN_HORIZONTAL_DOUBLE);
		prepareItem(tableWidget, indCurrentRow, indColumnMeasuredValue2, SPAN_HORIZONTAL_DOUBLE);

		tableWidget->item(indCurrentRow, indColumnMeasuredValue1)->setText(QString::number(cableRows[i]->measureds[0]->current) != "-1" ? QString::number(cableRows[i]->measureds[0]->current) : "-");
		tableWidget->item(indCurrentRow, indColumnMeasuredValue2)->setText(QString::number(cableRows[i]->measureds[0]->voltage) != "-1" ? QString::number(cableRows[i]->measureds[0]->voltage) : "-");
	}
}

static void fillTableInAnalog(QTableWidget* tableWidget, std::vector<TestTableRowProperties*> cableRows)
{
	for (int i = 0; i < cableRows.size(); i++)
	{
		int indCurrentRow = tableWidget->rowCount();
		tableWidget->insertRow(indCurrentRow);

		prepareItem(tableWidget, indCurrentRow, IND_COLUMN_BASE_CONNECTOR, SPAN_NONE);
		prepareItem(tableWidget, indCurrentRow, IND_COLUMN_BASE_PIN, SPAN_NONE);
		prepareItem(tableWidget, indCurrentRow, IND_COLUMN_BASE_DIRECTION, SPAN_NONE);
		prepareItem(tableWidget, indCurrentRow, IND_COLUMN_BASE_TYPE, SPAN_NONE);
		prepareItem(tableWidget, indCurrentRow, IND_COLUMN_BASE_NAME, SPAN_NONE);

		tableWidget->item(indCurrentRow, IND_COLUMN_BASE_CONNECTOR)->setText(cableRows[i]->connectorStr);
		tableWidget->item(indCurrentRow, IND_COLUMN_BASE_PIN)->setText(cableRows[i]->pin);
		tableWidget->item(indCurrentRow, IND_COLUMN_BASE_DIRECTION)->setText(cableRows[i]->direction);
		tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(cableRows[i]->typeStr);
		tableWidget->item(indCurrentRow, IND_COLUMN_BASE_NAME)->setText(cableRows[i]->name);

		int indColumnMeasuredValues = MEASUREMENT_COLUMN_POSITION;
		int indColumnThresholdsMin = MEASUREMENT_COLUMN_POSITION + 2;
		int indColumnThresholdsMax = MEASUREMENT_COLUMN_POSITION + 3;


		for (int j = 0; j < cableRows[i]->thresholds.size(); j++)
		{
			prepareItem(tableWidget, indCurrentRow, indColumnMeasuredValues, SPAN_HORIZONTAL_DOUBLE);
			prepareItem(tableWidget, indCurrentRow, indColumnThresholdsMin, SPAN_NONE);
			prepareItem(tableWidget, indCurrentRow, indColumnThresholdsMax, SPAN_NONE);

			tableWidget->item(indCurrentRow, indColumnMeasuredValues)->setText(cableRows[i]->measureds[j]->voltage != -1 ? QString::number(cableRows[i]->measureds[j]->voltage) : "-");
			tableWidget->item(indCurrentRow, indColumnThresholdsMin)->setText(cableRows[i]->thresholds[j].minValue != -1 ? QString::number(cableRows[i]->thresholds[j].minValue) : "-");
			tableWidget->item(indCurrentRow, indColumnThresholdsMax)->setText(cableRows[i]->thresholds[j].maxValue != -1 ? QString::number(cableRows[i]->thresholds[j].maxValue) : "-");

			indColumnMeasuredValues += MEASUREMENT_OFFSET_IN_ANALOG;
			indColumnThresholdsMin += MEASUREMENT_OFFSET_IN_ANALOG;
			indColumnThresholdsMax += MEASUREMENT_OFFSET_IN_ANALOG;
		}
	}
}

void ReportWindow::fillTable(TypeCable type, std::vector<TestTableRowProperties*> cableRows)
{
	switch (type)
	{
	case TypeCable::DIG_OUT:
	case TypeCable::PWM_OUT:
	case TypeCable::VNH_OUT:
		fillTableOut(tableWidget, cableRows);
		break;

	case TypeCable::DIG_IN:
	case TypeCable::HALL_IN:
		fillTableIn(tableWidget, cableRows);
		break;

	case TypeCable::ANALOG_IN:
		fillTableInAnalog(tableWidget, cableRows);
		break;
	}
}








































































//ReportWindow::ReportWindow(std::vector<Cable> cables, std::vector<void*> additionalValues, WindowType testType)
//{
//	this->cables = cables;
//	this->testType = testType;
//	this->additionalValues = additionalValues;
//
//	measuredCheckBox = nullptr;
//	checkManualStandCheckBox = nullptr;
//	checkPCAutoStandCheckBox = nullptr;
//	checkStandAutoStandCheckBox = nullptr;
//
//	for (int i = 0; i < sizeof(changedThresholds) / sizeof(changedThresholds[0]); i++)
//		changedThresholds[i] = NOT_SET;
//
//	initUi();
//
//	QMetaObject::connectSlotsByName(this);
//
//	//QStringList* mainTableHeaderLabels = new QStringList();
//	//mainTableHeaderLabels->push_back("");
//	//mainTableHeaderLabels->push_back("");
//	//mainTableHeaderLabels->push_back("");
//
//	//for (int row = ROW_3; row < mainTableWidget->rowCount(); row++)
//	//{
//	//	mainTableHeaderLabels->push_back(QString::number(row - ROW_3));
//	//	mainTableWidget->verticalHeader()->setSectionResizeMode(row, QHeaderView::Interactive);
//	//}
//
//	//mainTableWidget->setVerticalHeaderLabels(*mainTableHeaderLabels);
//}
//ReportWindow::~ReportWindow()
//{}
//
//void ReportWindow::initUi()
//{
//	this->resize(START_ReportWindow_SIZE);
//	this->setMinimumSize(725, 250);
//	mainWidget = new QWidget(this);
//	mainWidget->setObjectName("mainLayoutWidget");
//	mainWidget->setGeometry(LEFT_PADDING_MAIN_WIDGET, UP_PADDING_MAIN_WIDGET, START_ReportWindow_WIDTH - (LEFT_PADDING_MAIN_WIDGET * 2), START_ReportWindow_HEIGHT - (UP_PADDING_MAIN_WIDGET * 2));
//
//	mainVLayout = new QVBoxLayout(mainWidget);
//	mainVLayout->setObjectName("mainVLayout");
//
//	bodyHLayout = new QHBoxLayout(mainWidget);
//	bodyHLayout->setObjectName("mainHLayout");
//	mainVLayout->addLayout(bodyHLayout);
//
//	initUiTable();
//	bodyHLayout->addWidget(mainTableWidget);
//
//	initUiColumnSetters();
//	bodyHLayout->addWidget(columnSetterWidget);
//
//	resetUiAdditionalColumns();
//	resetUiFillColumns();
//
//	initUiFooter();
//	mainVLayout->addWidget(footerWidget);
//}
//
//void ReportWindow::initUiTable()
//{
//	initUiGenerateTable();
//	initUiSetValueTable();
//}
//
//void ReportWindow::initUiGenerateTable()
//{
//	mainTableWidget = new QTableWidget(mainWidget);
//	mainTableWidget->setObjectName("mainTableWidget");
//	mainTableWidget->setStyleSheet(lightStyles.testwindowTableWidget);
//	mainTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
//	mainTableWidget->horizontalHeader()->hide();
//	mainTableWidget->verticalHeader()->hide();
//	mainTableWidget->setRowCount(3);
//	mainTableWidget->setColumnCount(CLEAR_COLUMN_COUNT);
//
//	QFont font = QFont();
//	font.setBold(true);
//	font.setPointSizeF(10);
//
//	mainTableWidget->setSpan(OUTALL_CELL_PAD_TABLE, SPAN_COUNT_3, SPAN_COUNT_1);
//	mainTableWidget->model()->setData(mainTableWidget->model()->index(OUTALL_CELL_PAD_TABLE), "");
//	mainTableWidget->item(OUTALL_CELL_PAD_TABLE)->setTextAlignment(Qt::AlignCenter);
//	mainTableWidget->item(OUTALL_CELL_PAD_TABLE)->setFlags(Qt::ItemIsSelectable);
//	mainTableWidget->item(OUTALL_CELL_PAD_TABLE)->setFont(font);
//
//	mainTableWidget->setSpan(OUTALL_CELL_PIN_TABLE, SPAN_COUNT_3, SPAN_COUNT_1);
//	mainTableWidget->model()->setData(mainTableWidget->model()->index(OUTALL_CELL_PIN_TABLE), "");
//	mainTableWidget->item(OUTALL_CELL_PIN_TABLE)->setTextAlignment(Qt::AlignCenter);
//	mainTableWidget->item(OUTALL_CELL_PIN_TABLE)->setFlags(Qt::ItemIsSelectable);
//	mainTableWidget->item(OUTALL_CELL_PIN_TABLE)->setFont(font);
//
//	mainTableWidget->setSpan(OUTALL_CELL_TYPE_TABLE, SPAN_COUNT_3, SPAN_COUNT_1);
//	mainTableWidget->model()->setData(mainTableWidget->model()->index(OUTALL_CELL_TYPE_TABLE), "");
//	mainTableWidget->item(OUTALL_CELL_TYPE_TABLE)->setTextAlignment(Qt::AlignCenter);
//	mainTableWidget->item(OUTALL_CELL_TYPE_TABLE)->setFlags(Qt::ItemIsSelectable);
//	mainTableWidget->item(OUTALL_CELL_TYPE_TABLE)->setFont(font);
//
//	mainTableWidget->setSpan(OUTALL_CELL_NAME_TABLE, SPAN_COUNT_3, SPAN_COUNT_1);
//	mainTableWidget->model()->setData(mainTableWidget->model()->index(OUTALL_CELL_NAME_TABLE), "");
//	mainTableWidget->item(OUTALL_CELL_NAME_TABLE)->setTextAlignment(Qt::AlignCenter);
//	mainTableWidget->item(OUTALL_CELL_NAME_TABLE)->setFlags(Qt::ItemIsSelectable);
//	mainTableWidget->item(OUTALL_CELL_NAME_TABLE)->setFont(font);
//
//	mainTableWidget->setSpan(CELL_COMPONENT_TABLE, SPAN_COUNT_3, SPAN_COUNT_1);
//	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_COMPONENT_TABLE), "");
//	mainTableWidget->item(CELL_COMPONENT_TABLE)->setTextAlignment(Qt::AlignCenter);
//	mainTableWidget->item(CELL_COMPONENT_TABLE)->setFlags(Qt::ItemIsSelectable);
//	mainTableWidget->item(CELL_COMPONENT_TABLE)->setFont(font);
//
//	mainTableWidget->setSpan(CELL_TRESHHOLDERS_TABLE, SPAN_COUNT_1, SPAN_COUNT_4);
//	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_TRESHHOLDERS_TABLE), "");
//	mainTableWidget->item(CELL_TRESHHOLDERS_TABLE)->setTextAlignment(Qt::AlignCenter);
//	mainTableWidget->item(CELL_TRESHHOLDERS_TABLE)->setFlags(Qt::ItemIsSelectable);
//	mainTableWidget->item(CELL_TRESHHOLDERS_TABLE)->setFont(font);
//
//	mainTableWidget->setSpan(CELL_TRESHHOLDERS_U_TABLE, SPAN_COUNT_1, SPAN_COUNT_2);
//	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_TRESHHOLDERS_U_TABLE), "");
//	mainTableWidget->item(CELL_TRESHHOLDERS_U_TABLE)->setTextAlignment(Qt::AlignCenter);
//	mainTableWidget->item(CELL_TRESHHOLDERS_U_TABLE)->setFlags(Qt::ItemIsSelectable);
//	mainTableWidget->item(CELL_TRESHHOLDERS_U_TABLE)->setFont(font);
//
//	mainTableWidget->setSpan(CELL_TRESHHOLDERS_I_TABLE, SPAN_COUNT_1, SPAN_COUNT_2);
//	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_TRESHHOLDERS_I_TABLE), "");
//	mainTableWidget->item(CELL_TRESHHOLDERS_I_TABLE)->setTextAlignment(Qt::AlignCenter);
//	mainTableWidget->item(CELL_TRESHHOLDERS_I_TABLE)->setFlags(Qt::ItemIsSelectable);
//	mainTableWidget->item(CELL_TRESHHOLDERS_I_TABLE)->setFont(font);
//
//	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_TRESHHOLDERS_U_MIN_TABLE), "");
//	mainTableWidget->item(CELL_TRESHHOLDERS_U_MIN_TABLE)->setTextAlignment(Qt::AlignCenter);
//	mainTableWidget->item(CELL_TRESHHOLDERS_U_MIN_TABLE)->setFlags(Qt::ItemIsSelectable);
//	mainTableWidget->item(CELL_TRESHHOLDERS_U_MIN_TABLE)->setFont(font);
//
//	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_TRESHHOLDERS_U_MAX_TABLE), "");
//	mainTableWidget->item(CELL_TRESHHOLDERS_U_MAX_TABLE)->setTextAlignment(Qt::AlignCenter);
//	mainTableWidget->item(CELL_TRESHHOLDERS_U_MAX_TABLE)->setFlags(Qt::ItemIsSelectable);
//	mainTableWidget->item(CELL_TRESHHOLDERS_U_MAX_TABLE)->setFont(font);
//
//	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_TRESHHOLDERS_I_MIN_TABLE), "");
//	mainTableWidget->item(CELL_TRESHHOLDERS_I_MIN_TABLE)->setTextAlignment(Qt::AlignCenter);
//	mainTableWidget->item(CELL_TRESHHOLDERS_I_MIN_TABLE)->setFlags(Qt::ItemIsSelectable);
//	mainTableWidget->item(CELL_TRESHHOLDERS_I_MIN_TABLE)->setFont(font);
//
//	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_TRESHHOLDERS_I_MAX_TABLE), "");
//	mainTableWidget->item(CELL_TRESHHOLDERS_I_MAX_TABLE)->setTextAlignment(Qt::AlignCenter);
//	mainTableWidget->item(CELL_TRESHHOLDERS_I_MAX_TABLE)->setFlags(Qt::ItemIsSelectable);
//	mainTableWidget->item(CELL_TRESHHOLDERS_I_MAX_TABLE)->setFont(font);
//
//	mainTableWidget->setColumnWidth(COLUMN_PAD, 75);
//	mainTableWidget->setColumnWidth(COLUMN_PIN, 75);
//	mainTableWidget->setColumnWidth(COLUMN_TYPE, 75);
//	mainTableWidget->setColumnWidth(COLUMN_NAME, 75);
//	mainTableWidget->setColumnWidth(COLUMN_COMPONENT, 75);
//	mainTableWidget->setColumnWidth(COLUMN_TRESHHOLDERS_U_MIN, 50);
//	mainTableWidget->setColumnWidth(COLUMN_TRESHHOLDERS_U_MAX, 50);
//	mainTableWidget->setColumnWidth(COLUMN_TRESHHOLDERS_I_MIN, 50);
//	mainTableWidget->setColumnWidth(COLUMN_TRESHHOLDERS_I_MAX, 50);
//
//	mainTableWidget->setRowHeight(ROW_0, 40);
//	mainTableWidget->setRowHeight(ROW_1, 40);
//	mainTableWidget->setRowHeight(ROW_2, 40);
//
//	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_PAD, QHeaderView::Fixed);
//	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_PIN, QHeaderView::Fixed);
//	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_TYPE, QHeaderView::Fixed);
//	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_NAME, QHeaderView::Stretch);
//	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_COMPONENT, QHeaderView::Fixed);
//	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_TRESHHOLDERS, QHeaderView::Fixed);
//
//	mainTableWidget->verticalHeader()->setSectionResizeMode(ROW_0, QHeaderView::Fixed);
//	mainTableWidget->verticalHeader()->setSectionResizeMode(ROW_1, QHeaderView::Fixed);
//	mainTableWidget->verticalHeader()->setSectionResizeMode(ROW_2, QHeaderView::Fixed);
//}
//
//void ReportWindow::initUiSetValueTable()
//{
//	QString typeTmp;
//	switch (viewWindowState->appLanguage)
//	{
//	case RUSSIAN_LANG:
//		mainTableWidget->item(OUTALL_CELL_PAD_TABLE)->setText(QString::fromLocal8Bit("Колодка"));
//		mainTableWidget->item(OUTALL_CELL_PIN_TABLE)->setText(QString::fromLocal8Bit("Пин"));
//		mainTableWidget->item(OUTALL_CELL_TYPE_TABLE)->setText(QString::fromLocal8Bit("Тип"));
//		mainTableWidget->item(OUTALL_CELL_NAME_TABLE)->setText(QString::fromLocal8Bit("Название"));
//		mainTableWidget->item(CELL_COMPONENT_TABLE)->setText(QString::fromLocal8Bit("Компонент"));
//		mainTableWidget->item(CELL_TRESHHOLDERS_TABLE)->setText(QString::fromLocal8Bit("Пороги"));
//		mainTableWidget->item(CELL_TRESHHOLDERS_U_TABLE)->setText(QString::fromLocal8Bit("U, В"));
//		mainTableWidget->item(CELL_TRESHHOLDERS_I_TABLE)->setText(QString::fromLocal8Bit("I, А"));
//		mainTableWidget->item(CELL_TRESHHOLDERS_U_MIN_TABLE)->setText(QString::fromLocal8Bit("Мин"));
//		mainTableWidget->item(CELL_TRESHHOLDERS_U_MAX_TABLE)->setText(QString::fromLocal8Bit("Макс"));
//		mainTableWidget->item(CELL_TRESHHOLDERS_I_MIN_TABLE)->setText(QString::fromLocal8Bit("Мин"));
//		mainTableWidget->item(CELL_TRESHHOLDERS_I_MAX_TABLE)->setText(QString::fromLocal8Bit("Макс"));
//		break;
//
//	case ENGLISH_LANG:
//		mainTableWidget->item(OUTALL_CELL_PAD_TABLE)->setText("Pad");
//		mainTableWidget->item(OUTALL_CELL_PIN_TABLE)->setText("Pin");
//		mainTableWidget->item(OUTALL_CELL_TYPE_TABLE)->setText("Type");
//		mainTableWidget->item(OUTALL_CELL_NAME_TABLE)->setText("Name");
//		mainTableWidget->item(CELL_COMPONENT_TABLE)->setText("Component");
//		mainTableWidget->item(CELL_TRESHHOLDERS_TABLE)->setText("Treshholders");
//		mainTableWidget->item(CELL_TRESHHOLDERS_U_TABLE)->setText("U, V");
//		mainTableWidget->item(CELL_TRESHHOLDERS_I_TABLE)->setText("I, A");
//		mainTableWidget->item(CELL_TRESHHOLDERS_U_MIN_TABLE)->setText("Min");
//		mainTableWidget->item(CELL_TRESHHOLDERS_U_MAX_TABLE)->setText("Max");
//		mainTableWidget->item(CELL_TRESHHOLDERS_I_MIN_TABLE)->setText("Min");
//		mainTableWidget->item(CELL_TRESHHOLDERS_I_MAX_TABLE)->setText("Max");
//		break;
//	}
//
//	for (int row = ROW_3; row < cables.size() + ROW_3; row++)
//	{
//		mainTableWidget->insertRow(mainTableWidget->rowCount());
//		for (int column = 0; column < CLEAR_COLUMN_COUNT; column++)
//		{
//			QAbstractItemModel* model = mainTableWidget->model();
//			switch (column)
//			{
//			case COLUMN_PAD:
//				model->setData(model->index(row, column), QString((char)(PRIMARY_CONNECTOR_SYMBOL + (int)cables[row - ROW_3].getConnector())));
//				break;
//
//			case COLUMN_PIN:
//				model->setData(model->index(row, column), QString::number(cables[row - ROW_3].getPin()));
//				break;
//
//			case COLUMN_NAME:
//				model->setData(model->index(row, column), QString(cables[row - ROW_3].getName()));
//				break;
//
//			case COLUMN_COMPONENT:
//				model->setData(model->index(row, column), QString(cables[row - ROW_3].getComponent()));
//				break;
//
//			case COLUMN_TYPE:
//				switch (viewWindowState->appLanguage)
//				{
//				case RUSSIAN_LANG:
//					switch (cables[row - ROW_3].getType())
//					{
//					case TYPE_NOT_SET:
//						model->setData(model->index(row, column), QString("No set"));
//						break;
//					case TYPE_DIGITAL:
//						model->setData(model->index(row, column), QString("Digital"));
//						break;
//					case TYPE_ANALOG:
//						model->setData(model->index(row, column), QString("Analog"));
//						break;
//					case TYPE_HALL:
//						model->setData(model->index(row, column), QString("HALL"));
//						break;
//					case TYPE_PWM:
//						model->setData(model->index(row, column), QString("PWM"));
//						break;
//					case TYPE_VNH:
//						model->setData(model->index(row, column), QString("VNH"));
//						break;
//					}
//					break;
//				case ENGLISH_LANG:
//					switch (cables[row - ROW_3].getType())
//					{
//					case TYPE_NOT_SET:
//						model->setData(model->index(row, column), QString::fromLocal8Bit("Не указан"));
//						break;
//					case TYPE_DIGITAL:
//						model->setData(model->index(row, column), QString::fromLocal8Bit("Цифровой"));
//						break;
//					case TYPE_ANALOG:
//						model->setData(model->index(row, column), QString::fromLocal8Bit("Аналоговый"));
//						break;
//					case TYPE_HALL:
//						model->setData(model->index(row, column), QString("HALL"));
//						break;
//					case TYPE_PWM:
//						model->setData(model->index(row, column), QString::fromLocal8Bit("ШИМ"));
//						break;
//					case TYPE_VNH:
//						model->setData(model->index(row, column), QString::fromLocal8Bit("VNH"));
//						break;
//						break;
//					}
//					break;
//				}
//				break;
//
//			case COLUMN_TRESHHOLDERS_U_MIN:
//				//model->setData(model->index(row, column), QString::number(cables[row - ROW_3].getMinVoltage()));
//				break;
//
//			case COLUMN_TRESHHOLDERS_U_MAX:
//				//model->setData(model->index(row, column), QString::number(cables[row - ROW_3].getMaxVoltage()));
//				break;
//
//			case COLUMN_TRESHHOLDERS_I_MIN:
//				//model->setData(model->index(row, column), QString::number(cables[row - ROW_3].getMinCurrent()));
//				break;
//
//			case COLUMN_TRESHHOLDERS_I_MAX:
//				//model->setData(model->index(row, column), QString::number(cables[row - ROW_3].getMaxCurrent()));
//				break;
//			}
//
//			if (column != COLUMN_NAME)
//				mainTableWidget->item(row, column)->setTextAlignment(Qt::AlignCenter);
//		}
//	}
//}
//
//void ReportWindow::fillColumnsSetters()
//{
//	switch (testType)
//	{
//	case WindowType::IN_TEST_MANUAL_STAND:
//	case WindowType::OUT_TEST_MANUAL_STAND:
//	case WindowType::FULL_TEST_MANUAL_STAND:
//		activeColumn.measured				= false;
//		activeColumn.checkManualStand		= true;
//		activeColumn.checkPCAutoStand		= false;
//		activeColumn.checkStandAutoStand	= false;
//		break;
//
//	case WindowType::IN_MANUAL_TEST_AUTO_STAND:
//	case WindowType::OUT_MANUAL_TEST_AUTO_STAND:
//	case WindowType::IN_AUTO_TEST_AUTO_STAND:
//	case WindowType::OUT_AUTO_TEST_AUTO_STAND:
//	case WindowType::FULL_TEST_AUTO_STAND:
//		activeColumn.measured				= true;
//		activeColumn.checkManualStand		= false;
//		activeColumn.checkPCAutoStand		= true;
//		activeColumn.checkStandAutoStand	= true;
//		break;
//	}
//}
//
//void ReportWindow::initUiColumnSetters()
//{
//	columnSetterWidget = new QWidget(mainWidget);
//	columnSetterWidget->setObjectName("columnSetterWidget");
//	columnSetterWidget->setFixedWidth(COLUMN_SETTER_WIDTH);
//
//	columnSetterVLayout = new QVBoxLayout(columnSetterWidget);
//	columnSetterVLayout->setObjectName("columnSetterVLayout");
//
//	fillColumnsSetters();
//
//	if (activeColumn.measured)
//	{
//		measuredHLayout = new QHBoxLayout(columnSetterWidget);
//		measuredHLayout->setObjectName("measuredHLayout");
//		columnSetterVLayout->addLayout(measuredHLayout);
//
//		measuredLabel = new QLabel(columnSetterWidget);
//		measuredLabel->setObjectName("measuredLabel");
//		measuredLabel->setText("Measured column");
//		measuredHLayout->addWidget(measuredLabel);
//
//		measuredCheckBox = new QCheckBox(columnSetterWidget);
//		measuredCheckBox->setObjectName("measuredCheckBox");
//		measuredCheckBox->setChecked(true);
//		measuredHLayout->addWidget(measuredCheckBox, Qt::AlignRight);
//	}
//
//	if (activeColumn.checkManualStand)
//	{
//		checkManualStandHLayout = new QHBoxLayout(columnSetterWidget);
//		checkManualStandHLayout->setObjectName("checkManualStandHLayout");
//		columnSetterVLayout->addLayout(checkManualStandHLayout);
//
//		checkManualStandLabel = new QLabel(columnSetterWidget);
//		checkManualStandLabel->setObjectName("checkManualStandLabel");
//		checkManualStandLabel->setText("Manual stand check column");
//		checkManualStandHLayout->addWidget(checkManualStandLabel);
//
//		checkManualStandCheckBox = new QCheckBox(columnSetterWidget);
//		checkManualStandCheckBox->setObjectName("checkManualStandCheckBox");
//		checkManualStandCheckBox->setChecked(false);
//		columnSetterVLayout->addWidget(checkManualStandCheckBox, Qt::AlignRight);
//	}
//
//	if (activeColumn.checkPCAutoStand)
//	{
//		checkPCAutoStandHLayout = new QHBoxLayout(columnSetterWidget);
//		checkPCAutoStandHLayout->setObjectName("checkPCAutoStandHLayout");
//		columnSetterVLayout->addLayout(checkPCAutoStandHLayout);
//
//		checkPCAutoStandLabel = new QLabel(columnSetterWidget);
//		checkPCAutoStandLabel->setObjectName("checkPCAutoStandLabel");
//		checkPCAutoStandLabel->setText("PC check in auto stand column");
//		checkPCAutoStandHLayout->addWidget(checkPCAutoStandLabel);
//
//		checkPCAutoStandCheckBox = new QCheckBox(columnSetterWidget);
//		checkPCAutoStandCheckBox->setObjectName("checkPCAutoStandCheckBox");
//		checkPCAutoStandCheckBox->setChecked(false);
//		columnSetterVLayout->addWidget(checkPCAutoStandCheckBox, Qt::AlignRight);
//	}
//
//	if (activeColumn.checkStandAutoStand)
//	{
//		checkStandAutoStandHLayout = new QHBoxLayout(columnSetterWidget);
//		checkStandAutoStandHLayout->setObjectName("checkStandAutoStandHLayout");
//		columnSetterVLayout->addLayout(checkStandAutoStandHLayout);
//
//		checkStandAutoStandLabel = new QLabel(columnSetterWidget);
//		checkStandAutoStandLabel->setObjectName("checkStandAutoStandLabel");
//		checkStandAutoStandLabel->setText("Stand check in auto stand column");
//		checkStandAutoStandHLayout->addWidget(checkStandAutoStandLabel);
//
//		checkStandAutoStandCheckBox = new QCheckBox(columnSetterWidget);
//		checkStandAutoStandCheckBox->setObjectName("checkStandAutoStandCheckBox");
//		checkStandAutoStandCheckBox->setChecked(false);
//		columnSetterVLayout->addWidget(checkStandAutoStandCheckBox, Qt::AlignRight);
//	}
//}
//
//void ReportWindow::resetUiAdditionalColumns()
//{
//	while (mainTableWidget->columnCount() != CLEAR_COLUMN_COUNT)
//		mainTableWidget->removeColumn(mainTableWidget->columnCount() - 1);
//
//	QFont font = QFont();
//	font.setBold(true);
//	font.setPointSizeF(10);
//
//	if (measuredCheckBox != nullptr && measuredCheckBox->isChecked())
//	{
//		int currentColumn = mainTableWidget->columnCount();
//		int currentColumn_U = mainTableWidget->columnCount();
//		int currentColumn_I = mainTableWidget->columnCount() + 1;
//
//		mainTableWidget->insertColumn(currentColumn_U);
//		mainTableWidget->insertColumn(currentColumn_I);
//
//		mainTableWidget->setSpan(ROW_0, currentColumn, SPAN_COUNT_2, SPAN_COUNT_2);
//		mainTableWidget->model()->setData(mainTableWidget->model()->index(ROW_0, currentColumn), "");
//		mainTableWidget->item(ROW_0, currentColumn)->setTextAlignment(Qt::AlignCenter);
//		mainTableWidget->item(ROW_0, currentColumn)->setFlags(Qt::ItemIsSelectable);
//		mainTableWidget->item(ROW_0, currentColumn)->setFont(font);
//
//		mainTableWidget->model()->setData(mainTableWidget->model()->index(ROW_2, currentColumn_U), "U, V");
//		mainTableWidget->item(ROW_2, currentColumn_U)->setTextAlignment(Qt::AlignCenter);
//		mainTableWidget->item(ROW_2, currentColumn_U)->setFlags(Qt::ItemIsSelectable);
//		mainTableWidget->item(ROW_2, currentColumn_U)->setFont(font);
//
//		mainTableWidget->model()->setData(mainTableWidget->model()->index(ROW_2, currentColumn_I), "I, A");
//		mainTableWidget->item(ROW_2, currentColumn_I)->setTextAlignment(Qt::AlignCenter);
//		mainTableWidget->item(ROW_2, currentColumn_I)->setFlags(Qt::ItemIsSelectable);
//		mainTableWidget->item(ROW_2, currentColumn_I)->setFont(font);
//
//		switch (viewWindowState->appLanguage)
//		{
//		case RUSSIAN_LANG:
//			mainTableWidget->item(ROW_0, currentColumn)->setText(QString::fromLocal8Bit("Измеренное значение"));
//			break;
//
//		case ENGLISH_LANG:
//			mainTableWidget->item(ROW_0, currentColumn)->setText(QString("Measured value"));
//			break;
//		}
//	}
//
//	if (checkManualStandCheckBox != nullptr && checkManualStandCheckBox->isChecked())
//	{
//		int currentColumn = mainTableWidget->columnCount();
//		mainTableWidget->insertColumn(mainTableWidget->columnCount());
//		mainTableWidget->setSpan(ROW_0, currentColumn, SPAN_COUNT_3, SPAN_COUNT_1);
//		mainTableWidget->model()->setData(mainTableWidget->model()->index(ROW_0, currentColumn), "");
//		mainTableWidget->item(ROW_0, currentColumn)->setTextAlignment(Qt::AlignCenter);
//		mainTableWidget->item(ROW_0, currentColumn)->setFlags(Qt::ItemIsSelectable);
//		mainTableWidget->item(ROW_0, currentColumn)->setFont(font);
//		mainTableWidget->setColumnWidth(currentColumn, 75);
//
//
//		switch (viewWindowState->appLanguage)
//		{
//		case RUSSIAN_LANG:
//			mainTableWidget->item(ROW_0, currentColumn)->setText(QString::fromLocal8Bit("Статус ручной проверки"));
//			break;
//
//		case ENGLISH_LANG:
//			mainTableWidget->item(ROW_0, currentColumn)->setText(QString("Manual status"));
//			break;
//		}
//	}
//
//	if (checkPCAutoStandCheckBox != nullptr && checkPCAutoStandCheckBox->isChecked())
//	{
//		int currentColumn = mainTableWidget->columnCount();
//		mainTableWidget->insertColumn(mainTableWidget->columnCount());
//		mainTableWidget->setSpan(ROW_0, currentColumn, SPAN_COUNT_3, SPAN_COUNT_1);
//		mainTableWidget->model()->setData(mainTableWidget->model()->index(ROW_0, currentColumn), "");
//		mainTableWidget->item(ROW_0, currentColumn)->setTextAlignment(Qt::AlignCenter);
//		mainTableWidget->item(ROW_0, currentColumn)->setFlags(Qt::ItemIsSelectable);
//		mainTableWidget->item(ROW_0, currentColumn)->setFont(font);
//		mainTableWidget->setColumnWidth(currentColumn, 75);
//
//		switch (viewWindowState->appLanguage)
//		{
//		case RUSSIAN_LANG:
//			mainTableWidget->item(ROW_0, currentColumn)->setText(QString::fromLocal8Bit("Статус проверки ПК"));
//			break;
//
//		case ENGLISH_LANG:
//			mainTableWidget->item(ROW_0, currentColumn)->setText(QString("PC status"));
//			break;
//		}
//	}
//
//	if (checkStandAutoStandCheckBox != nullptr && checkStandAutoStandCheckBox->isChecked())
//	{
//		int currentColumn = mainTableWidget->columnCount();
//		mainTableWidget->insertColumn(mainTableWidget->columnCount());
//		mainTableWidget->setSpan(ROW_0, currentColumn, SPAN_COUNT_3, SPAN_COUNT_1);
//		mainTableWidget->model()->setData(mainTableWidget->model()->index(ROW_0, currentColumn), "");
//		mainTableWidget->item(ROW_0, currentColumn)->setTextAlignment(Qt::AlignCenter);
//		mainTableWidget->item(ROW_0, currentColumn)->setFlags(Qt::ItemIsSelectable);
//		mainTableWidget->item(ROW_0, currentColumn)->setFont(font);
//		mainTableWidget->setColumnWidth(currentColumn, 75);
//
//		switch (viewWindowState->appLanguage)
//		{
//		case RUSSIAN_LANG:
//			mainTableWidget->item(ROW_0, currentColumn)->setText(QString::fromLocal8Bit("Статус проверки стенда"));
//			break;
//
//		case ENGLISH_LANG:
//			mainTableWidget->item(ROW_0, currentColumn)->setText(QString("Stand status"));
//			break;
//		}
//	}
//}
//
//void ReportWindow::resetUiFillColumns()
//{
//	int measuredAdditionalInd = ADDITIONAL_IND_NOT_SET;
//	int checkManualStandAdditionalInd = ADDITIONAL_IND_NOT_SET;
//	int checkPCAutoStandAdditionalInd = ADDITIONAL_IND_NOT_SET;
//	int checkStandAutoStandAdditionalInd = ADDITIONAL_IND_NOT_SET;
//
//	std::vector<QCheckBox*> manualChecked;
//
//	int currentAdditionalInd = CLEAR_COLUMN_COUNT;
//	if (measuredCheckBox != nullptr && measuredCheckBox->isChecked())
//	{
//		measuredAdditionalInd = currentAdditionalInd;
//		currentAdditionalInd += 2;
//	}
//	if (checkManualStandCheckBox != nullptr && checkManualStandCheckBox->isChecked())
//	{
//		checkManualStandAdditionalInd = currentAdditionalInd;
//		currentAdditionalInd++;
//
//		for (int i = 0; i < additionalValues.size(); i++)
//			manualChecked.push_back(((QCheckBox*)additionalValues[i]));
//	}
//	if (checkPCAutoStandCheckBox != nullptr && checkPCAutoStandCheckBox->isChecked())
//	{
//		checkPCAutoStandAdditionalInd = currentAdditionalInd;
//		currentAdditionalInd++;
//	}
//	if (checkStandAutoStandCheckBox != nullptr && checkStandAutoStandCheckBox->isChecked())
//	{
//		checkStandAutoStandAdditionalInd = currentAdditionalInd;
//		currentAdditionalInd++;
//	}
//
//	QAbstractItemModel* model = mainTableWidget->model();
//	for (int row = ROW_3; row < cables.size() + ROW_3; row++)
//		for (int column = CLEAR_COLUMN_COUNT - 1; column < mainTableWidget->columnCount(); column++)
//		{
//			if (measuredAdditionalInd != ADDITIONAL_IND_NOT_SET && measuredAdditionalInd == column)
//			{
//				model->setData(model->index(row, column), 1);
//				model->setData(model->index(row, column + 1), 1);
//				column++;
//			}
//			if (checkManualStandAdditionalInd != ADDITIONAL_IND_NOT_SET && checkManualStandAdditionalInd == column)
//			{
//				switch (viewWindowState->appLanguage)
//				{
//				case RUSSIAN_LANG:
//					model->setData(model->index(row, column), QString::fromLocal8Bit(((QCheckBox*)additionalValues[row - ROW_3])->isChecked() ? "Исправно" : "Неисправно"));
//					break;
//
//				case ENGLISH_LANG:
//					model->setData(model->index(row, column), QString(((QCheckBox*)additionalValues[row - ROW_3])->isChecked() ? "Correct" : "Incorrect"));
//					break;
//				}
//			}
//			if (checkPCAutoStandAdditionalInd != ADDITIONAL_IND_NOT_SET && checkPCAutoStandAdditionalInd == column)
//			{
//				model->setData(model->index(row, column), 3);
//			}
//			if (checkStandAutoStandAdditionalInd != ADDITIONAL_IND_NOT_SET && checkStandAutoStandAdditionalInd == column)
//			{
//				model->setData(model->index(row, column), "4");
//			}
//
//			mainTableWidget->item(row, column)->setTextAlignment(Qt::AlignCenter);
//			if (measuredAdditionalInd != ADDITIONAL_IND_NOT_SET)
//				mainTableWidget->item(row, column - 1)->setTextAlignment(Qt::AlignCenter);
//		}
//}
//
//void ReportWindow::initUiFooter()
//{
//	footerWidget = new QWidget(mainWidget);
//	footerWidget->setObjectName("footerWidget");
//
//	bottomHLayout = new QHBoxLayout(footerWidget);
//	bottomHLayout->setObjectName("bottomHLayout");
//
//	testerNameLineEdit = new QLineEdit(footerWidget);
//	testerNameLineEdit->setObjectName("testerNameLineEdit");
//	testerNameLineEdit->setFixedWidth(FIXED_TESTER_NAME_WIDTH);
//	testerNameLineEdit->setFixedHeight(FIXED_TESTER_NAME_HEIGHT);
//	bottomHLayout->addWidget(testerNameLineEdit);
//
//	switch (viewWindowState->appLanguage)
//	{
//	case RUSSIAN_LANG:
//		testerNameLineEdit->setText(QString::fromLocal8Bit("Иванов Иван Иванович"));
//		break;
//
//	case ENGLISH_LANG:
//		testerNameLineEdit->setText("Ivanov Ivan Ivanovich");
//		break;
//	}
//
//	bottomLeftSpacer = new QSpacerItem(20, 0, QSizePolicy::Expanding);
//	bottomHLayout->addItem(bottomLeftSpacer);
//
//	testerJobLineEdit = new QLineEdit(footerWidget);
//	testerJobLineEdit->setObjectName("testerJobLineEdit");
//	testerJobLineEdit->setFixedWidth(FIXED_TESTER_JOB_WIDTH);
//	testerJobLineEdit->setFixedHeight(FIXED_TESTER_JOB_HEIGHT);
//	bottomHLayout->addWidget(testerJobLineEdit);
//
//	switch (viewWindowState->appLanguage)
//	{
//	case RUSSIAN_LANG:
//		testerJobLineEdit->setText(QString::fromLocal8Bit("Иванов Иван Иванович"));
//		break;
//
//	case ENGLISH_LANG:
//		testerJobLineEdit->setText("Ivanov Ivan Ivanovich");
//		break;
//	}
//
//	bottomRightSpacer = new QSpacerItem(40, 0, QSizePolicy::Expanding);
//	bottomHLayout->addItem(bottomRightSpacer);
//
//	saveButton = new QPushButton(footerWidget);
//	saveButton->setMinimumSize(BUTTON_SIZE);
//	saveButton->setObjectName("saveButton");
//	saveButton->setStyleSheet(lightStyles.testwindowButtonStyle);
//	bottomHLayout->addWidget(saveButton);
//
//	switch (viewWindowState->appLanguage)
//	{
//	case RUSSIAN_LANG:
//		saveButton->setText(QString::fromLocal8Bit("Сохранить"));
//		break;
//	case ENGLISH_LANG:
//		saveButton->setText("Save");
//		break;
//	}
//}
//
//
//void ReportWindow::resizeEvent(QResizeEvent* event1)
//{
//	mainWidget->resize(geometry().width() - (LEFT_PADDING_MAIN_WIDGET * 2), geometry().height() - (UP_PADDING_MAIN_WIDGET * 2));
//}
//
//void ReportWindow::on_measuredCheckBox_stateChanged(int state)
//{
//	resetUiAdditionalColumns();
//	resetUiFillColumns();
//}
//
//void ReportWindow::on_checkManualStandCheckBox_stateChanged(int state)
//{
//	resetUiAdditionalColumns();
//	resetUiFillColumns();
//}
//
//void ReportWindow::on_checkPCAutoStandCheckBox_stateChanged(int state)
//{
//	resetUiAdditionalColumns();
//	resetUiFillColumns();
//}
//
//void ReportWindow::on_checkStandAutoStandCheckBox_stateChanged(int state)
//{
//	resetUiAdditionalColumns();
//	resetUiFillColumns();
//}
//
//void ReportWindow::on_saveButton_clicked()
//{
//
//}
