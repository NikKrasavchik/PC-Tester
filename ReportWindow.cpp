#include "ReportWindow.h"

#define BUTTON_WIDTH				100
#define BUTTON_HEIGHT				40
#define BUTTON_SIZE					BUTTON_WIDTH, BUTTON_HEIGHT

#define MEASUREMENT_COLUMN_POSITION		6

#define IND_COLUMN_BASE_COMMENT				6

using namespace QXlsx;

ReportWindow::ReportWindow(std::vector<TestTableRowProperties*> cableRows, QString testerName)
{
	this->cableRows = cableRows;
	this->testerName = testerName;
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
			if ((int)cableRows[i]->thresholds.size() * MEASUREMENT_OFFSET_OUT > maxColumnOffset)
				maxColumnOffset = (int)cableRows[i]->thresholds.size() * MEASUREMENT_OFFSET_OUT;
			break;

		case TypeCable::DIG_IN:
		case TypeCable::HALL_IN:
			if (MEASUREMENT_OFFSET_IN > maxColumnOffset)
				maxColumnOffset = MEASUREMENT_OFFSET_IN;
			break;

		case TypeCable::ANALOG_IN:
			if ((int)cableRows[i]->thresholds.size() * MEASUREMENT_OFFSET_IN_ANALOG > maxColumnOffset)
				maxColumnOffset = (int)cableRows[i]->thresholds.size() * MEASUREMENT_OFFSET_OUT;
			break;
		}

	return maxColumnOffset;
}

static int getMaxTypeOffset(std::vector<TestTableRowProperties*> cableRows)
{
	int maxTypeOffset = 0;
	for (int i = 0; i < cableRows.size(); i++)
		if (maxTypeOffset < (int)cableRows[i]->thresholds.size())
			maxTypeOffset = (int)cableRows[i]->thresholds.size();
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

		tableWidget->item(indCurrentRow, indColumnMeasuredValue1)->setText(QString::number(cableRows[i]->measureds[0]->voltage) != "-1" ? QString::number(cableRows[i]->measureds[0]->voltage) : "-");
		tableWidget->item(indCurrentRow, indColumnMeasuredValue2)->setText(QString::number(cableRows[i]->measureds[0]->current) != "-1" ? QString::number(cableRows[i]->measureds[0]->current) : "-");

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

void writeHorizontalAlignCell(Document& xlsx, int row, int columnStart, int columnEnd, const QVariant& text, QXlsx::Format::HorizontalAlignment align, Format formatText = Format(), const QColor& color = nullptr)
{
	CellRange r(row, columnStart, row, columnEnd);
	if(color != nullptr)
		formatText.setPatternBackgroundColor(color);
	xlsx.write(r.firstRow(), r.firstColumn(), text, formatText);
	formatText.setHorizontalAlignment(align);
	xlsx.mergeCells(r, formatText);
}

void genereateHeaderFile(Document& xlsx, QString testerName)
{
	Format dateFormat;
	dateFormat.setNumberFormatIndex(14);
	dateFormat.setBorderStyle(Format::BorderThin);
	Format timeFormat;
	timeFormat.setNumberFormatIndex(21);
	timeFormat.setBorderStyle(Format::BorderThin);
	Format format;
	format.setBorderStyle(Format::BorderThin);




	writeHorizontalAlignCell(xlsx, 1, 1, 2, viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Дата проверки") : QString("Date of inspection"), Format::AlignLeft, format);
	writeHorizontalAlignCell(xlsx, 2, 1, 2, viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Время проверки") : QString("Time of inspection"), Format::AlignLeft, format);
	writeHorizontalAlignCell(xlsx, 3, 1, 2, viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Имя оператора") : QString("Operator name"), Format::AlignLeft, format);
	writeHorizontalAlignCell(xlsx, 4, 1, 2, viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Название ЭБУ") : QString("Block name"), Format::AlignLeft, format);
	writeHorizontalAlignCell(xlsx, 5, 1, 2, viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Номер детали") : QString("Part number"), Format::AlignLeft, format);
	writeHorizontalAlignCell(xlsx, 6, 1, 2, viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Серийный номер") : QString("Serial number"), Format::AlignLeft, format);

	QDateTime time = QDateTime::currentDateTime();

	writeHorizontalAlignCell(xlsx, 1, 3, 6, time.date(), Format::AlignLeft, dateFormat);
	writeHorizontalAlignCell(xlsx, 2, 3, 6, time.time(), Format::AlignLeft, timeFormat);
	writeHorizontalAlignCell(xlsx, 3, 3, 6, testerName, Format::AlignLeft, format);
}

void genereateHeaderTable(Document& xlsx, int maxOffset)
{
	Format format;
	format.setHorizontalAlignment(Format::AlignHCenter);
	format.setFontBold(true);
	format.setBorderStyle(Format::BorderThin);

	xlsx.write(HEIGHT_HEADERFILE + 1, 1, viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Колодка") : QString("Pad"), format);
	xlsx.write(HEIGHT_HEADERFILE + 1, 2, viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Пин") : QString("Pin"), format);
	xlsx.write(HEIGHT_HEADERFILE + 1, 3, viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Направление") : QString("Direction"), format);
	xlsx.write(HEIGHT_HEADERFILE + 1, 4, viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Тип") : QString("Type"), format);
	xlsx.write(HEIGHT_HEADERFILE + 1, 5, viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Название") : QString("Name"), format);
	xlsx.write(HEIGHT_HEADERFILE + 1, 6 + maxOffset, viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Комментарий") : QString("Comment"), format);

	CellRange range(HEIGHT_HEADERFILE + 1, 6, HEIGHT_HEADERFILE + 1, 5 + maxOffset);
	xlsx.mergeCells(range, format);

}

QString getStrDirection(QString direction)
{
	QString str;
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		if (direction == "OUT")
			str = QString::fromLocal8Bit("Выход");
		else if(direction == "IN")
			str = QString::fromLocal8Bit("Вход");
		break;
	case ENGLISH_LANG:
		if (direction == "OUT")
			str = "Out";
		else if (direction == "IN")
			str = "In";
		break;
	}
	return str;
}

QString getStrType(TypeCable type)
{
	QString str;
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		switch (type)
		{
		case TypeCable::EMPTY:
			str = "Error";
			break;
		case TypeCable::DIG_IN:
			str = QString::fromLocal8Bit("Цифровой");
			break;
		case TypeCable::ANALOG_IN:
			str = QString::fromLocal8Bit("Аналоговый");
			break;
		case TypeCable::HALL_IN:
			str = "HAll";
			break;
		case TypeCable::DIG_OUT:
			str = QString::fromLocal8Bit("Цифровой");
			break;
		case TypeCable::PWM_OUT:
			str = "PWM";
			break;
		case TypeCable::VNH_OUT:
			str = "VNH";
			break;
		default:
			break;
		}
		break;
	case ENGLISH_LANG:
		switch (type)
		{
		case TypeCable::EMPTY:
			str = "Error";
			break;
		case TypeCable::DIG_IN:
			str = "Digital";
			break;
		case TypeCable::ANALOG_IN:
			str = "Analog";
			break;
		case TypeCable::HALL_IN:
			str = "HAll";
			break;
		case TypeCable::DIG_OUT:
			str = "Digital";
			break;
		case TypeCable::PWM_OUT:
			str = "PWM";
			break;
		case TypeCable::VNH_OUT:
			str = "VNH";
			break;
		default:
			break;
		}
	}
	return str;
}

void ReportWindow::on_commentTextEdit_textChanged()
{

}


void ReportWindow::on_saveButton_clicked()
{
	int maxOffset = getMaxColumnOffset(cableRows);


	Document xlsx;
	xlsx.addSheet(viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Отчёт") : QString("Report"));
	xlsx.currentWorksheet()->setGridLinesVisible(false);
	xlsx.setColumnWidth(1,	5, 13);


	genereateHeaderFile(xlsx, testerName);
	genereateHeaderTable(xlsx, maxOffset);




	Format format;
	format.setHorizontalAlignment(Format::AlignHCenter);
	format.setFontBold(true);
	format.setBorderStyle(Format::BorderThin);

	int numRow = START_ROW_TABLE;
	bool color = false;
	for (int type = 0; type < typedCableRows.size(); type++)
	{
		for (int i = 0; i < typedCableRows[type].size(); i++)
		{
			Format tmpHeaderFormat(format);
			tmpHeaderFormat.setPatternBackgroundColor(QColor("#8db4e2"));
			Format tmpRowFormat(format);
			if (color)
			{
				tmpRowFormat.setPatternBackgroundColor(QColor("#95b3d7"));
				color = false;
			}
			else
			{
				tmpRowFormat.setPatternBackgroundColor(QColor("#dce6f1"));
				color = true;
			}
			Format tmpRowCommentFormat(tmpRowFormat);
			tmpRowCommentFormat.setHorizontalAlignment(Format::AlignLeft);

			switch ((TypeCable)type)
			{
			case TypeCable::DIG_IN:
			case TypeCable::HALL_IN:
				if (i == 0) // header type
				{
					writeHorizontalAlignCell(xlsx, numRow, 6, 7, viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Значение 1") : QString("Value 1"), Format::AlignHCenter, tmpHeaderFormat);
					writeHorizontalAlignCell(xlsx, numRow, 8, 9, viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Значение 2") : QString("Value 2"), Format::AlignHCenter, tmpHeaderFormat);
					CellRange* range = new CellRange(numRow, 10, numRow + (int)typedCableRows[type].size(), 9 + maxOffset - 4);
					xlsx.mergeCells(*range, format);
					delete range;
					range = new CellRange(numRow, 1, numRow, 5);
					xlsx.mergeCells(*range);
					delete range;
					numRow++;
				}
				xlsx.write(numRow, 1, typedCableRows[type][i]->connectorStr, tmpRowFormat);
				xlsx.write(numRow, 2, typedCableRows[type][i]->pin, tmpRowFormat);
				xlsx.write(numRow, 3, getStrDirection(typedCableRows[type][i]->direction), tmpRowFormat);
				xlsx.write(numRow, 4, getStrType(typedCableRows[type][i]->typeInt), tmpRowFormat);
				xlsx.write(numRow, 5, typedCableRows[type][i]->name, tmpRowFormat);
				xlsx.write(numRow, 6 + maxOffset, typedCableRows[type][i]->comment, tmpRowCommentFormat);

				if (typedCableRows[type][i]->measureds[0]->voltage == NOT_SET)
				{
					writeHorizontalAlignCell(xlsx, numRow, 6, 7, "-", Format::AlignHCenter, tmpRowFormat);
					writeHorizontalAlignCell(xlsx, numRow, 8, 9, "-", Format::AlignHCenter, tmpRowFormat);

				}
				else
				{
					writeHorizontalAlignCell(xlsx, numRow, 6, 7, "", Format::AlignHCenter, Format(), (typedCableRows[type][i]->measureds[0]->voltage == 0 ? QColor("#FF8686") : QColor("#7CC770")));
					writeHorizontalAlignCell(xlsx, numRow, 8, 9, "", Format::AlignHCenter, Format(), (typedCableRows[type][i]->measureds[0]->current == 0 ? QColor("#FF8686") : QColor("#7CC770")));
				}
				numRow++;
				break;
			case TypeCable::ANALOG_IN:
				if (i == 0) // header type
				{
					for (int j = 0; j < typedCableRows[type][i]->thresholds.size(); j++)
					{
						writeHorizontalAlignCell(xlsx, numRow, 6 + (4 * j), 9 + (4 * j), viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Измерение ") + QString::number(j + 1) : QString("Measured ") + QString::number(j + 1), Format::AlignHCenter, tmpHeaderFormat);
						
						CellRange r(numRow + 1, 6 + (4 * j), numRow + 2, 7 + (4 * j));
						xlsx.write(r.firstRow(), r.firstColumn(), viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Измеренное значение") : QString("Measured values"), tmpHeaderFormat);
						tmpHeaderFormat.setTextWarp(true);
						xlsx.mergeCells(r, tmpHeaderFormat);
						tmpHeaderFormat.setTextWarp(false);

						writeHorizontalAlignCell(xlsx, numRow + 1, 8 + (4 * j), 9 + (4 * j), viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Пороги") : QString("Thresholds "), Format::AlignHCenter, tmpHeaderFormat);
						xlsx.write(numRow + 2, 8 + (4 * j), viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Мин") : QString("Min"), tmpHeaderFormat);
						xlsx.write(numRow + 2, 9 + (4 * j), viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Макс") : QString("Max"), tmpHeaderFormat);
					}	
					CellRange* range = new CellRange(numRow, 6 + ((int)typedCableRows[type][i]->thresholds.size() * 4), numRow + (int)typedCableRows[type].size() + 2, 9 + maxOffset - 4);
					if (typedCableRows[type][i]->thresholds.size() * 6 != maxOffset)
						xlsx.mergeCells(*range, format);
					delete range;
					range = new CellRange(numRow, 1, numRow + 2, 5);
					xlsx.mergeCells(*range);
					delete range;
						numRow += 3;
				}
				xlsx.write(numRow, 1, typedCableRows[type][i]->connectorStr, tmpRowFormat);
				xlsx.write(numRow, 2, typedCableRows[type][i]->pin, tmpRowFormat);
				xlsx.write(numRow, 3, getStrDirection(typedCableRows[type][i]->direction), tmpRowFormat);
				xlsx.write(numRow, 4, getStrType(typedCableRows[type][i]->typeInt), tmpRowFormat);
				xlsx.write(numRow, 5, typedCableRows[type][i]->name, tmpRowFormat);
				xlsx.write(numRow, 6 + maxOffset, typedCableRows[type][i]->comment, tmpRowCommentFormat);


				for (int j = 0; j < typedCableRows[type][i]->thresholds.size(); j++)
				{
					if (typedCableRows[type][i]->measureds[j]->voltage == NOT_SET)
					{
						writeHorizontalAlignCell(xlsx, numRow, 6 + (4 * j), 7 + (4 * j), "-", Format::AlignHCenter, tmpRowFormat);
						xlsx.write(numRow, 8 + (4 * j), "-", tmpRowFormat); // red
						xlsx.write(numRow, 9 + (4 * j), "-", tmpRowFormat);
					}
					else
					{
						Format tmpRedFormat(format);
						tmpRedFormat.setPatternBackgroundColor("#FF8686");
						Format tmpGreenFormat(format);
						tmpGreenFormat.setPatternBackgroundColor("#7CC770");

						if (typedCableRows[type][i]->measureds[j]->voltage > typedCableRows[type][i]->thresholds[j].minValue && typedCableRows[type][i]->measureds[j]->voltage < typedCableRows[type][i]->thresholds[j].maxValue)
						{
							writeHorizontalAlignCell(xlsx, numRow, 6 + (4 * j), 7 + (4 * j), typedCableRows[type][i]->measureds[j]->voltage, Format::AlignHCenter, tmpGreenFormat);
							xlsx.write(numRow, 8 + (4 * j), typedCableRows[type][i]->thresholds[j].minValue, tmpRowFormat);
							xlsx.write(numRow, 9 + (4 * j), typedCableRows[type][i]->thresholds[j].maxValue, tmpRowFormat);
						}
						else if (typedCableRows[type][i]->measureds[j]->voltage < typedCableRows[type][i]->thresholds[j].minValue)
						{
							writeHorizontalAlignCell(xlsx, numRow, 6 + (4 * j), 7 + (4 * j), typedCableRows[type][i]->measureds[j]->voltage, Format::AlignHCenter, tmpRedFormat);
							xlsx.write(numRow, 8 + (4 * j), typedCableRows[type][i]->thresholds[j].minValue, tmpRedFormat); // red
							xlsx.write(numRow, 9 + (4 * j), typedCableRows[type][i]->thresholds[j].maxValue, tmpRowFormat);
						}
						else if (typedCableRows[type][i]->measureds[j]->voltage > typedCableRows[type][i]->thresholds[j].maxValue)
						{
							writeHorizontalAlignCell(xlsx, numRow, 6 + (4 * j), 7 + (4 * j), typedCableRows[type][i]->measureds[j]->voltage, Format::AlignHCenter, tmpRedFormat);
							xlsx.write(numRow, 8 + (4 * j), typedCableRows[type][i]->thresholds[j].minValue, tmpRowFormat);
							xlsx.write(numRow, 9 + (4 * j), typedCableRows[type][i]->thresholds[j].maxValue, tmpRedFormat); // red
						}

					}
				}

				numRow ++;
				break;
			case TypeCable::DIG_OUT:
			case TypeCable::PWM_OUT:
			case TypeCable::VNH_OUT:

				if (i == 0) // header type
				{
					for (int j = 0; j < typedCableRows[type][i]->thresholds.size(); j++)
					{
						writeHorizontalAlignCell(xlsx, numRow, 6 + (6 * j), 11 + (6 * j), viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Измерение ") + QString::number(j + 1) : QString("Measured ") + QString::number(j + 1), Format::AlignHCenter, tmpHeaderFormat);

						CellRange r(numRow + 1, 6 + (6 * j), numRow + 2, 7 + (6 * j));
						xlsx.write(r.firstRow(), r.firstColumn(), viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Измеренное значение") : QString("Measured values"), tmpHeaderFormat);
						tmpHeaderFormat.setTextWarp(true);
						xlsx.mergeCells(r, tmpHeaderFormat);
						tmpHeaderFormat.setTextWarp(false);

						writeHorizontalAlignCell(xlsx, numRow + 1, 8 + (6 * j), 11 + (6 * j), viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Пороги") : QString("Thresholds "), Format::AlignHCenter, tmpHeaderFormat);
						writeHorizontalAlignCell(xlsx, numRow + 2, 8 + (6 * j), 9 + (6 * j), viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("U, В") : QString("U, V"), Format::AlignHCenter, tmpHeaderFormat);
						writeHorizontalAlignCell(xlsx, numRow + 2, 10 + (6 * j), 11 + (6 * j), QString("I, A"), Format::AlignHCenter, tmpHeaderFormat);
						xlsx.write(numRow + 3, 6 + (6 * j), viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("U, В") : QString("U, V"), tmpHeaderFormat);
						xlsx.write(numRow + 3, 7 + (6 * j), QString("I, A"), tmpHeaderFormat);
						xlsx.write(numRow + 3, 8 + (6 * j), viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Мин") : QString("Min"), tmpHeaderFormat);
						xlsx.write(numRow + 3, 9 + (6 * j), viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Макс") : QString("Max"), tmpHeaderFormat);
						xlsx.write(numRow + 3, 10 + (6 * j), viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Мин") : QString("Min"), tmpHeaderFormat);
						xlsx.write(numRow + 3, 11 + (6 * j), viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Макс") : QString("Max"), tmpHeaderFormat);



					}
					CellRange* range = new CellRange(numRow, 6 + ((int)typedCableRows[type][i]->thresholds.size() * 6), numRow + (int)typedCableRows[type].size() + 3, 9 + maxOffset - 4);
					if (typedCableRows[type][i]->thresholds.size() * 6 != maxOffset)
						xlsx.mergeCells(*range, format);
					delete range;
					range = new CellRange(numRow, 1, numRow + 3, 5);
					xlsx.mergeCells(*range);
					delete range;
					numRow += 4;
				}
				xlsx.write(numRow, 1, typedCableRows[type][i]->connectorStr, tmpRowFormat);
				xlsx.write(numRow, 2, typedCableRows[type][i]->pin, tmpRowFormat);
				xlsx.write(numRow, 3, getStrDirection(typedCableRows[type][i]->direction), tmpRowFormat);
				xlsx.write(numRow, 4, getStrType(typedCableRows[type][i]->typeInt), tmpRowFormat);
				xlsx.write(numRow, 5, typedCableRows[type][i]->name, tmpRowFormat);
				xlsx.write(numRow, 6 + maxOffset, typedCableRows[type][i]->comment, tmpRowCommentFormat);

				for (int j = 0; j < typedCableRows[type][i]->thresholds.size(); j++)
				{


					if (typedCableRows[type][i]->measureds[j]->voltage == NOT_SET)
					{
						xlsx.write(numRow, 6 + (6 * j), QString("-"), tmpRowFormat);
						xlsx.write(numRow, 7 + (6 * j), QString("-"), tmpRowFormat);
						xlsx.write(numRow, 8 + (6 * j), typedCableRows[type][i]->thresholds[j].minVoltage, tmpRowFormat);
						xlsx.write(numRow, 9 + (6 * j), typedCableRows[type][i]->thresholds[j].maxVoltage, tmpRowFormat);
						xlsx.write(numRow, 10 + (6 * j), typedCableRows[type][i]->thresholds[j].minCurrent, tmpRowFormat);
						xlsx.write(numRow, 11 + (6 * j), typedCableRows[type][i]->thresholds[j].maxCurrent, tmpRowFormat);

					}
					else
					{
						Format tmpRedFormat(format);
						tmpRedFormat.setPatternBackgroundColor("#FF8686");
						Format tmpGreenFormat(format);
						tmpGreenFormat.setPatternBackgroundColor("#7CC770");
						if (typedCableRows[type][i]->measureds[j]->voltage > typedCableRows[type][i]->thresholds[j].minVoltage && typedCableRows[type][i]->measureds[j]->voltage < typedCableRows[type][i]->thresholds[j].maxVoltage)
						{
							xlsx.write(numRow, 6 + (6 * j), typedCableRows[type][i]->measureds[j]->voltage, tmpGreenFormat);
							xlsx.write(numRow, 8 + (6 * j), typedCableRows[type][i]->thresholds[j].minVoltage, tmpRowFormat);
							xlsx.write(numRow, 9 + (6 * j), typedCableRows[type][i]->thresholds[j].maxVoltage, tmpRowFormat);
						}
						else if (typedCableRows[type][i]->measureds[j]->voltage < typedCableRows[type][i]->thresholds[j].minVoltage)
						{
							xlsx.write(numRow, 6 + (6 * j), typedCableRows[type][i]->measureds[j]->voltage, tmpRedFormat);
							xlsx.write(numRow, 8 + (6 * j), typedCableRows[type][i]->thresholds[j].minVoltage, tmpRedFormat); // red
							xlsx.write(numRow, 9 + (6 * j), typedCableRows[type][i]->thresholds[j].maxVoltage, tmpRowFormat);
						}
						else if (typedCableRows[type][i]->measureds[j]->voltage > typedCableRows[type][i]->thresholds[j].maxVoltage)
						{

							xlsx.write(numRow, 6 + (6 * j), typedCableRows[type][i]->measureds[j]->voltage, tmpRedFormat);
							xlsx.write(numRow, 8 + (6 * j), typedCableRows[type][i]->thresholds[j].minVoltage, tmpRowFormat);
							xlsx.write(numRow, 9 + (6 * j), typedCableRows[type][i]->thresholds[j].maxVoltage, tmpRedFormat); // red
						}

						if (typedCableRows[type][i]->measureds[j]->current > typedCableRows[type][i]->thresholds[j].minCurrent && typedCableRows[type][i]->measureds[j]->current < typedCableRows[type][i]->thresholds[j].maxCurrent)
						{
							xlsx.write(numRow, 7 + (6 * j), typedCableRows[type][i]->measureds[j]->current, tmpGreenFormat);
							xlsx.write(numRow, 10 + (6 * j), typedCableRows[type][i]->thresholds[j].minCurrent, tmpRowFormat);
							xlsx.write(numRow, 11 + (6 * j), typedCableRows[type][i]->thresholds[j].maxCurrent, tmpRowFormat);
						}
						else if (typedCableRows[type][i]->measureds[j]->current < typedCableRows[type][i]->thresholds[j].minCurrent)
						{
							xlsx.write(numRow, 7 + (6 * j), typedCableRows[type][i]->measureds[j]->current, tmpRedFormat);
							xlsx.write(numRow, 10 + (6 * j), typedCableRows[type][i]->thresholds[j].minCurrent, tmpRedFormat); // red
							xlsx.write(numRow, 11 + (6 * j), typedCableRows[type][i]->thresholds[j].maxCurrent, tmpRowFormat);
						}
						else if (typedCableRows[type][i]->measureds[j]->current > typedCableRows[type][i]->thresholds[j].maxCurrent)
						{

							xlsx.write(numRow, 7 + (6 * j), typedCableRows[type][i]->measureds[j]->current, tmpRedFormat);
							xlsx.write(numRow, 10 + (6 * j), typedCableRows[type][i]->thresholds[j].minCurrent, tmpRowFormat);
							xlsx.write(numRow, 11 + (6 * j), typedCableRows[type][i]->thresholds[j].maxCurrent, tmpRedFormat); // red
						}
					}

				}
				numRow++;
				break;
			}
		}
	}
	

	QDir dir;
	dir.mkdir("Reports");
	xlsx.saveAs("Reports/Report.xlsx");


}


/*
//void writeHorizontalAlignCell(Document& xlsx, const QString& cell, const QString& text, QXlsx::Format::HorizontalAlignment align)
//{
//    Format format;
//    format.setHorizontalAlignment(align);
//    format.setBorderStyle(Format::BorderThin);
//    xlsx.write(cell, text, format);
//}

void writeVerticalAlignCell(Document& xlsx, const QString& range, const QString& text,
	Format::VerticalAlignment align)
{
	Format format;
	format.setVerticalAlignment(align);
	format.setBorderStyle(Format::BorderThin);
	CellRange r(range);
	xlsx.write(r.firstRow(), r.firstColumn(), text);
	xlsx.mergeCells(r, format);
}

void writeBorderStyleCell(Document& xlsx, const QString& cell, const QString& text,
	Format::BorderStyle bs)
{
	Format format;
	format.setBorderStyle(bs);
	xlsx.write(cell, text, format);
}

void writeSolidFillCell(Document& xlsx, const QString& cell, const QColor& color)
{
	Format format;
	format.setPatternBackgroundColor(color);
	xlsx.write(cell, QVariant(), format);
}

void writePatternFillCell(Document& xlsx, const QString& cell, Format::FillPattern pattern,
	const QColor& color)
{
	Format format;
	format.setPatternForegroundColor(color);
	format.setFillPattern(pattern);
	xlsx.write(cell, QVariant(), format);
}

void writeBorderAndFontColorCell(Document& xlsx, const QString& cell, const QString& text,
	const QColor& color)
{
	Format format;
	format.setBorderStyle(Format::BorderThin);
	format.setBorderColor(color);
	format.setFontColor(color);
	xlsx.write(cell, text, format);
}

void writeFontNameCell(Document& xlsx, const QString& cell, const QString& text)
{
	Format format;
	format.setFontName(text);
	format.setFontSize(16);
	xlsx.write(cell, text, format);
}

void writeFontSizeCell(Document& xlsx, const QString& cell, int size)
{
	Format format;
	format.setFontSize(size);
	xlsx.write(cell, "Qt Xlsx", format);
}

void writeInternalNumFormatsCell(Document& xlsx, int row, double value, int numFmt)
{
	Format format;
	format.setNumberFormatIndex(numFmt);
	xlsx.write(row, 1, value);
	xlsx.write(row, 2, QString("Builtin NumFmt %1").arg(numFmt));
	xlsx.write(row, 3, value, format);
}

void writeCustomNumFormatsCell(Document& xlsx, int row, double value, const QString& numFmt)
{
	Format format;
	format.setNumberFormat(numFmt);
	xlsx.write(row, 1, value);
	xlsx.write(row, 2, numFmt);
	xlsx.write(row, 3, value, format);
}


	//---------------------------------------------------------------
	// Create the first sheet (Otherwise, default "Sheet1" will be created)
	xlsx.addSheet("Aligns & Borders");
	xlsx.setColumnWidth(2, 20); // Column B
	xlsx.setColumnWidth(8, 12); // Column H
	xlsx.currentWorksheet()->setGridLinesVisible(false);

	// Alignment
	//writeHorizontalAlignCell(xlsx, "B3", "AlignLeft", Format::AlignLeft);
	//writeHorizontalAlignCell(xlsx, "B5", "AlignHCenter", Format::AlignHCenter);
	//writeHorizontalAlignCell(xlsx, "B7", "AlignRight", Format::AlignRight);
	writeVerticalAlignCell(xlsx, "D3:D7", "AlignTop", Format::AlignTop);
	writeVerticalAlignCell(xlsx, "F3:F7", "AlignVCenter", Format::AlignVCenter);
	writeVerticalAlignCell(xlsx, "H3:H7", "AlignBottom", Format::AlignBottom);

	// Border
	writeBorderStyleCell(xlsx, "B13", "BorderMedium", Format::BorderMedium);
	writeBorderStyleCell(xlsx, "B15", "BorderDashed", Format::BorderDashed);
	writeBorderStyleCell(xlsx, "B17", "BorderDotted", Format::BorderDotted);
	writeBorderStyleCell(xlsx, "B19", "BorderThick", Format::BorderThick);
	writeBorderStyleCell(xlsx, "B21", "BorderDouble", Format::BorderDouble);
	writeBorderStyleCell(xlsx, "B23", "BorderDashDot", Format::BorderDashDot);

	// Fill
	writeSolidFillCell(xlsx, "D13", Qt::red);
	writeSolidFillCell(xlsx, "D15", Qt::blue);
	writeSolidFillCell(xlsx, "D17", Qt::yellow);
	writeSolidFillCell(xlsx, "D19", Qt::magenta);
	writeSolidFillCell(xlsx, "D21", Qt::green);
	writeSolidFillCell(xlsx, "D23", Qt::gray);
	writePatternFillCell(xlsx, "F13", Format::PatternMediumGray, Qt::red);
	writePatternFillCell(xlsx, "F15", Format::PatternDarkHorizontal, Qt::blue);
	writePatternFillCell(xlsx, "F17", Format::PatternDarkVertical, Qt::yellow);
	writePatternFillCell(xlsx, "F19", Format::PatternDarkDown, Qt::magenta);
	writePatternFillCell(xlsx, "F21", Format::PatternLightVertical, Qt::green);
	writePatternFillCell(xlsx, "F23", Format::PatternLightTrellis, Qt::gray);

	writeBorderAndFontColorCell(xlsx, "H13", "Qt::red", Qt::red);
	writeBorderAndFontColorCell(xlsx, "H15", "Qt::blue", Qt::blue);
	writeBorderAndFontColorCell(xlsx, "H17", "Qt::yellow", Qt::yellow);
	writeBorderAndFontColorCell(xlsx, "H19", "Qt::magenta", Qt::magenta);
	writeBorderAndFontColorCell(xlsx, "H21", "Qt::green", Qt::green);
	writeBorderAndFontColorCell(xlsx, "H23", "Qt::gray", Qt::gray);

	//---------------------------------------------------------------
	// Create the second sheet.
	xlsx.addSheet("Fonts");

	xlsx.write("B3", "Normal");
	Format font_bold;
	font_bold.setFontBold(true);
	xlsx.write("B4", "Bold", font_bold);
	Format font_italic;
	font_italic.setFontItalic(true);
	xlsx.write("B5", "Italic", font_italic);
	Format font_underline;
	font_underline.setFontUnderline(Format::FontUnderlineSingle);
	xlsx.write("B6", "Underline", font_underline);
	Format font_strikeout;
	font_strikeout.setFontStrikeOut(true);
	xlsx.write("B7", "StrikeOut", font_strikeout);

	writeFontNameCell(xlsx, "D3", "Arial");
	writeFontNameCell(xlsx, "D4", "Arial Black");
	writeFontNameCell(xlsx, "D5", "Comic Sans MS");
	writeFontNameCell(xlsx, "D6", "Courier New");
	writeFontNameCell(xlsx, "D7", "Impact");
	writeFontNameCell(xlsx, "D8", "Times New Roman");
	writeFontNameCell(xlsx, "D9", "Verdana");

	writeFontSizeCell(xlsx, "G3", 10);
	writeFontSizeCell(xlsx, "G4", 12);
	writeFontSizeCell(xlsx, "G5", 14);
	writeFontSizeCell(xlsx, "G6", 16);
	writeFontSizeCell(xlsx, "G7", 18);
	writeFontSizeCell(xlsx, "G8", 20);
	writeFontSizeCell(xlsx, "G9", 25);

	Format font_vertical;
	font_vertical.setRotation(255);
	font_vertical.setFontSize(16);
	xlsx.write("J3", "vertical", font_vertical);
	xlsx.mergeCells("J3:J9");

	//---------------------------------------------------------------
	// Create the third sheet.
	xlsx.addSheet("Formulas");
	xlsx.setColumnWidth(1, 2, 40);
	Format rAlign;
	rAlign.setHorizontalAlignment(Format::AlignRight);
	Format lAlign;
	lAlign.setHorizontalAlignment(Format::AlignLeft);
	xlsx.write("B3", 40, lAlign);
	xlsx.write("B4", 30, lAlign);
	xlsx.write("B5", 50, lAlign);
	xlsx.write("A7", "SUM(B3:B5)=", rAlign);
	xlsx.write("B7", "=SUM(B3:B5)", lAlign);
	xlsx.write("A8", "AVERAGE(B3:B5)=", rAlign);
	xlsx.write("B8", "=AVERAGE(B3:B5)", lAlign);
	xlsx.write("A9", "MAX(B3:B5)=", rAlign);
	xlsx.write("B9", "=MAX(B3:B5)", lAlign);
	xlsx.write("A10", "MIN(B3:B5)=", rAlign);
	xlsx.write("B10", "=MIN(B3:B5)", lAlign);
	xlsx.write("A11", "COUNT(B3:B5)=", rAlign);
	xlsx.write("B11", "=COUNT(B3:B5)", lAlign);

	xlsx.write("A13", "IF(B7>100,\"large\",\"small\")=", rAlign);
	xlsx.write("B13", "=IF(B7>100,\"large\",\"small\")", lAlign);

	xlsx.write("A15", "SQRT(25)=", rAlign);
	xlsx.write("B15", "=SQRT(25)", lAlign);
	xlsx.write("A16", "RAND()=", rAlign);
	xlsx.write("B16", "=RAND()", lAlign);
	xlsx.write("A17", "2*PI()=", rAlign);
	xlsx.write("B17", "=2*PI()", lAlign);

	xlsx.write("A19", "UPPER(\"qtxlsx\")=", rAlign);
	xlsx.write("B19", "=UPPER(\"qtxlsx\")", lAlign);
	xlsx.write("A20", "LEFT(\"ubuntu\",3)=", rAlign);
	xlsx.write("B20", "=LEFT(\"ubuntu\",3)", lAlign);
	xlsx.write("A21", "LEN(\"Hello Qt!\")=", rAlign);
	xlsx.write("B21", "=LEN(\"Hello Qt!\")", lAlign);

	Format dateFormat;
	dateFormat.setHorizontalAlignment(Format::AlignLeft);
	dateFormat.setNumberFormat("yyyy-mm-dd");
	xlsx.write("A23", "DATE(2013,8,13)=", rAlign);
	xlsx.write("B23", "=DATE(2013,8,13)", dateFormat);
	xlsx.write("A24", "DAY(B23)=", rAlign);
	xlsx.write("B24", "=DAY(B23)", lAlign);
	xlsx.write("A25", "MONTH(B23)=", rAlign);
	xlsx.write("B25", "=MONTH(B23)", lAlign);
	xlsx.write("A26", "YEAR(B23)=", rAlign);
	xlsx.write("B26", "=YEAR(B23)", lAlign);
	xlsx.write("A27", "DAYS360(B23,TODAY())=", rAlign);
	xlsx.write("B27", "=DAYS360(B23,TODAY())", lAlign);

	xlsx.write("A29", "B3+100*(2-COS(0)))=", rAlign);
	xlsx.write("B29", "=B3+100*(2-COS(0))", lAlign);
	xlsx.write("A30", "ISNUMBER(B29)=", rAlign);
	xlsx.write("B30", "=ISNUMBER(B29)", lAlign);
	xlsx.write("A31", "AND(1,0)=", rAlign);
	xlsx.write("B31", "=AND(1,0)", lAlign);

	xlsx.write("A33", "HYPERLINK(\"http://qt-project.org\")=", rAlign);
	xlsx.write("B33", "=HYPERLINK(\"http://qt-project.org\")", lAlign);

	//---------------------------------------------------------------
	// Create the fourth sheet.
	xlsx.addSheet("NumFormats");
	xlsx.setColumnWidth(2, 40);
	writeInternalNumFormatsCell(xlsx, 4, 2.5681, 2);
	writeInternalNumFormatsCell(xlsx, 5, 2500000, 3);
	writeInternalNumFormatsCell(xlsx, 6, -500, 5);
	writeInternalNumFormatsCell(xlsx, 7, -0.25, 9);
	writeInternalNumFormatsCell(xlsx, 8, 890, 11);
	writeInternalNumFormatsCell(xlsx, 9, 0.75, 12);
	writeInternalNumFormatsCell(xlsx, 10, 41499, 14);
	writeInternalNumFormatsCell(xlsx, 11, 41499, 17);

	writeCustomNumFormatsCell(xlsx, 13, 20.5627, "#.###");
	writeCustomNumFormatsCell(xlsx, 14, 4.8, "#.00");
	writeCustomNumFormatsCell(xlsx, 15, 1.23, "0.00 \"RMB\"");
	writeCustomNumFormatsCell(xlsx, 16, 60, "[Red][<=100];[Green][>100]");

	//---------------------------------------------------------------
	// Create the fifth sheet.
	xlsx.addSheet("Merging");
	Format centerAlign;
	centerAlign.setHorizontalAlignment(Format::AlignHCenter);
	centerAlign.setVerticalAlignment(Format::AlignVCenter);
	xlsx.write("B4", "Hello Qt!");
	xlsx.mergeCells("B4:F6", centerAlign);
	xlsx.write("B8", 1);
	xlsx.mergeCells("B8:C21", centerAlign);
	xlsx.write("E8", 2);
	xlsx.mergeCells("E8:F21", centerAlign);

	//---------------------------------------------------------------
	// Create the fifth sheet.
	xlsx.addSheet("Grouping");
	qsrand(QDateTime::currentMSecsSinceEpoch());
	for (int row = 2; row < 31; ++row) {
		for (int col = 1; col <= 10; ++col)
			xlsx.write(row, col, qrand() % 100);
	}
	xlsx.groupRows(4, 7);
	xlsx.groupRows(11, 26, false);
	xlsx.groupRows(15, 17, false);
	xlsx.groupRows(20, 22, false);
	xlsx.setColumnWidth(1, 10, 10.0);
	xlsx.groupColumns(1, 2);
	xlsx.groupColumns(5, 8, false);

	xlsx.saveAs("Book1.xlsx");

	// Make sure that read/write works well.
	Document xlsx2("Book1.xlsx");
	xlsx2.saveAs("Book2.xlsx");
*/