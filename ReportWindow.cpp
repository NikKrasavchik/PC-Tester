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
	
	if (!(rowSpan == 1 && columnSpan == 1))
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

	typedCableRows.resize(TYPE_COUNT);
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

void ReportWindow::fillTableOut(std::vector<TestTableRowProperties*> cableRows)
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

		commentsTextEdits.push_back(new QTextEdit());
		connect(commentsTextEdits[commentsTextEdits.size() - 1], &QTextEdit::textChanged, this, &ReportWindow::on_commentTextEdit_textChanged);
		QWidget* commentWidget = new QWidget();
		QHBoxLayout* commentHLayout = new QHBoxLayout(commentWidget);
		commentHLayout->addWidget(commentsTextEdits[commentsTextEdits.size() - 1]);
		commentHLayout->setAlignment(Qt::AlignCenter);
		commentHLayout->setContentsMargins(0, 0, 0, 0);
		commentWidget->setLayout(commentHLayout);
		tableWidget->setCellWidget(indCurrentRow, tableWidget->columnCount() - 1, commentWidget);
		if (cableRows[i]->comment.size() != 0)
		{
			prepareItem(tableWidget, indCurrentRow, tableWidget->columnCount() - 1, SPAN_NONE);
			commentsTextEdits[commentsTextEdits.size() - 1]->setText(cableRows[i]->comment);
		}
	}
}

void ReportWindow::fillTableIn(std::vector<TestTableRowProperties*> cableRows)
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

		commentsTextEdits.push_back(new QTextEdit());
		connect(commentsTextEdits[commentsTextEdits.size() - 1], &QTextEdit::textChanged, this, &ReportWindow::on_commentTextEdit_textChanged);
		QWidget* commentWidget = new QWidget();
		QHBoxLayout* commentHLayout = new QHBoxLayout(commentWidget);
		commentHLayout->addWidget(commentsTextEdits[commentsTextEdits.size() - 1]);
		commentHLayout->setAlignment(Qt::AlignCenter);
		commentHLayout->setContentsMargins(0, 0, 0, 0);
		commentWidget->setLayout(commentHLayout);
		tableWidget->setCellWidget(indCurrentRow, tableWidget->columnCount() - 1, commentWidget);
		if (cableRows[i]->comment.size() != 0)
		{
			prepareItem(tableWidget, indCurrentRow, tableWidget->columnCount() - 1, SPAN_NONE);
			commentsTextEdits[commentsTextEdits.size() - 1]->setText(cableRows[i]->comment);
		}
	}
}

void ReportWindow::fillTableInAnalog(std::vector<TestTableRowProperties*> cableRows)
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

		commentsTextEdits.push_back(new QTextEdit());
		connect(commentsTextEdits[commentsTextEdits.size() - 1], &QTextEdit::textChanged, this, &ReportWindow::on_commentTextEdit_textChanged);
		QWidget* commentWidget = new QWidget();
		QHBoxLayout* commentHLayout = new QHBoxLayout(commentWidget);
		commentHLayout->addWidget(commentsTextEdits[commentsTextEdits.size() - 1]);
		commentHLayout->setAlignment(Qt::AlignCenter);
		commentHLayout->setContentsMargins(0, 0, 0, 0);
		commentWidget->setLayout(commentHLayout);
		tableWidget->setCellWidget(indCurrentRow, tableWidget->columnCount() - 1, commentWidget);
		if (cableRows[i]->comment.size() != 0)
		{
			prepareItem(tableWidget, indCurrentRow, tableWidget->columnCount() - 1, SPAN_NONE);
			commentsTextEdits[commentsTextEdits.size() - 1]->setText(cableRows[i]->comment);
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
		fillTableOut(cableRows);
		break;

	case TypeCable::DIG_IN:
	case TypeCable::HALL_IN:
		fillTableIn(cableRows);
		break;

	case TypeCable::ANALOG_IN:
		fillTableInAnalog(cableRows);
		break;
	}
}

void ReportWindow::on_saveButton_clicked()
{

}

void ReportWindow::on_commentTextEdit_textChanged()
{
	
}
