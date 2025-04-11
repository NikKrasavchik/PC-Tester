#include "ReportWindow.h"

#define BUTTON_WIDTH					100
#define BUTTON_HEIGHT					40
#define BUTTON_SIZE						BUTTON_WIDTH, BUTTON_HEIGHT
#define FIXED_NAME_LABEL_WIDTH			75
#define FIXED_SERIAL_LABEL_WIDTH		125
#define FIXED_DATA_LINE_EDIT_WIDTH		300
#define FIXED_DATA_LINE_EDIT_HEIGHT		50

#define MEASUREMENT_COLUMN_POSITION		5

#define IND_COLUMN_BASE_COMMENT			5

using namespace QXlsx;

ReportWindow::ReportWindow(std::vector<TestTableRowProperties*> cableRows, TestBlockName testingBlock)
{
	this->cableRows = cableRows;
	this->testingBlock = testingBlock;

	initUi();
	initConnections();
}

ReportWindow::~ReportWindow()
{
	resaveComments();
}

void ReportWindow::initConnections()
{
	connect(saveButton, &QPushButton::clicked, this, &ReportWindow::on_saveButton_clicked);
}

void ReportWindow::initUi()
{
	setMinimumSize(WINDOW_MIN_SIZE);
	resize(WINDOW_MIN_SIZE);
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
	generateTableBaseSign();
	if (cableRows[0]->manualCheckBox != nullptr)
		generateTableManual();
	else
		generateTableAuto();
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
	footerWidget = new QWidget(mainWidget);
	footerWidget->setObjectName("footerWidget");
	mainVLayout->addWidget(footerWidget);

	footerHLayout = new QHBoxLayout(footerWidget);
	footerHLayout->setObjectName("footerHLayout");

	reportDataWidget = new QWidget(footerWidget);
	reportDataWidget->setObjectName("serialNumberWidget");
	footerHLayout->addWidget(reportDataWidget);

	reportDataHLayout = new QHBoxLayout(reportDataWidget);
	reportDataHLayout->setObjectName("serialNumbreVLayout");
	
	QFont font;
	font.setBold(true);
	font.setPointSizeF(10);

	testerNameLabel = new QLabel(reportDataWidget);
	testerNameLabel->setObjectName("testerNameLabel");
	testerNameLabel->setFixedWidth(FIXED_NAME_LABEL_WIDTH);
	testerNameLabel->setFont(font);
	testerNameLabel->setAlignment(Qt::AlignmentFlag::AlignVCenter | Qt::AlignRight);
	reportDataHLayout->addWidget(testerNameLabel);

	testerNameLineEdit = new QLineEdit(reportDataWidget);
	testerNameLineEdit->setObjectName("testerNameLineEdit");
	testerNameLineEdit->setAlignment(Qt::AlignmentFlag::AlignCenter);
	testerNameLineEdit->setFixedSize(FIXED_DATA_LINE_EDIT_WIDTH, FIXED_DATA_LINE_EDIT_HEIGHT);
	reportDataHLayout->addWidget(testerNameLineEdit);

	reportDataSpacer = new QSpacerItem(100, 0, QSizePolicy::Expanding);
	reportDataHLayout->addItem(reportDataSpacer);

	serialNumberLabel = new QLabel(reportDataWidget);
	serialNumberLabel->setObjectName("serialNumberLabel");
	serialNumberLabel->setFixedWidth(FIXED_SERIAL_LABEL_WIDTH);
	serialNumberLabel->setFont(font);
	serialNumberLabel->setAlignment(Qt::AlignmentFlag::AlignVCenter | Qt::AlignRight);
	reportDataHLayout->addWidget(serialNumberLabel);

	serialNumberLineEdit = new QLineEdit(reportDataWidget);
	serialNumberLineEdit->setObjectName("serialNumberLineEdit");
	serialNumberLineEdit->setAlignment(Qt::AlignmentFlag::AlignCenter);
	serialNumberLineEdit->setFixedSize(FIXED_DATA_LINE_EDIT_WIDTH, FIXED_DATA_LINE_EDIT_HEIGHT);
#ifndef FOR_DEVELOPER
	//serialNumberLineEdit->setReadOnly(true);
#endif // FOR_DEVELOPER

	serialNumberLineEdit->setText(Can::getDiagBlock(DiagInformation::Serial_NUMBER, testingBlock));
	reportDataHLayout->addWidget(serialNumberLineEdit);

	footerSpacer = new QSpacerItem(10000, 0, QSizePolicy::Expanding);
	footerHLayout->addItem(footerSpacer);

	saveButton = new QPushButton(footerWidget);
	saveButton->setObjectName("saveButton");
	saveButton->setFixedSize(BUTTON_SIZE);
	footerHLayout->addWidget(saveButton);
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
		tableWidget->item(CELL_SIGN_BASE_COMMENT)->setText(QString::fromLocal8Bit("Комментарий"));

		testerNameLabel->setText(QString::fromLocal8Bit("ФИО: "));
		serialNumberLabel->setText(QString::fromLocal8Bit("Серийный номер: "));

		saveButton->setText(QString::fromLocal8Bit("Сохранить"));
		testerNameLineEdit->setText(QString::fromLocal8Bit("Иванов Иван Иванович"));
		break;

	case ENGLISH_LANG:
		tableWidget->item(CELL_SIGN_BASE_CONNECTOR)->setText(QString("Connector"));
		tableWidget->item(CELL_SIGN_BASE_PIN)->setText(QString("Pin"));
		tableWidget->item(CELL_SIGN_BASE_DIRECTION)->setText(QString("Direction"));
		tableWidget->item(CELL_SIGN_BASE_TYPE)->setText(QString("Type"));
		tableWidget->item(CELL_SIGN_BASE_NAME)->setText(QString("Name"));
		tableWidget->item(CELL_SIGN_BASE_COMMENT)->setText(QString("Comment"));

		testerNameLabel->setText(QString("Initial: "));
		serialNumberLabel->setText(QString("Serial number: "));

		saveButton->setText(QString("Save"));
		testerNameLineEdit->setText(QString("Ivanov Ivan Ivanovich"));
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
		case TypeCable::HLD_OUT:
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

void ReportWindow::generateTableManual()
{
	tableWidget->setColumnWidth(IND_COLUMN_BASE_COMMENT, 250);
	tableWidget->insertColumn(MEASUREMENT_COLUMN_POSITION);
	tableWidget->setColumnWidth(MEASUREMENT_COLUMN_POSITION, 80);
	prepareItem(tableWidget, CELL_SIGN_CORRECT, SPAN_NONE);
	
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		tableWidget->item(CELL_SIGN_CORRECT)->setText(QString::fromLocal8Bit("Статус\nпроверки"));
		break;

	case ENGLISH_LANG:
		tableWidget->item(CELL_SIGN_CORRECT)->setText("Check\nstatus");
		break;
	}

	for (int i = 0; i < cableRows.size(); i++)
	{
		int row = tableWidget->rowCount();
		tableWidget->insertRow(tableWidget->rowCount());
		for (int column = 0; column < COLUMN_COUNT_MANUAL; column++)
			prepareItem(tableWidget, row, column, SPAN_NONE);

		tableWidget->item(row, IND_COLUMN_BASE_CONNECTOR)->setText(cableRows[i]->connectorStr);
		tableWidget->item(row, IND_COLUMN_BASE_PIN)->setText(cableRows[i]->pin);
		tableWidget->item(row, IND_COLUMN_BASE_NAME)->setText(cableRows[i]->name);
		tableWidget->item(row, IND_COLUMN_MANUAL_VALUE)->setBackgroundColor(cableRows[i]->manualCheckBox->isChecked() ? QColor(COLOR_GREEN) : QColor(COLOR_RED));


		commentsTextEdits.push_back(new QTextEdit());
		QWidget* commentWidget = new QWidget();
		QHBoxLayout* commentHLayout = new QHBoxLayout(commentWidget);
		commentHLayout->addWidget(commentsTextEdits[commentsTextEdits.size() - 1]);
		commentHLayout->setAlignment(Qt::AlignCenter);
		commentHLayout->setContentsMargins(0, 0, 0, 0);
		commentWidget->setLayout(commentHLayout);
		tableWidget->setCellWidget(row, tableWidget->columnCount() - 1, commentWidget);
		if (cableRows[i]->comment.size() != 0)
		{
			prepareItem(tableWidget, row, tableWidget->columnCount() - 1, SPAN_NONE);
			commentsTextEdits[commentsTextEdits.size() - 1]->setText(cableRows[i]->comment);
		}

		switch (viewWindowState->appLanguage)
		{
		case RUSSIAN_LANG:
			if (cableRows[i]->direction == "IN")
				tableWidget->item(row, IND_COLUMN_BASE_DIRECTION)->setText(QString::fromLocal8Bit("Вход"));
			else if (cableRows[i]->direction == "OUT")
				tableWidget->item(row, IND_COLUMN_BASE_DIRECTION)->setText(QString::fromLocal8Bit("Выход"));
			
			switch (cableRows[i]->typeInt)
			{
			case TypeCable::DIG_IN:
			case TypeCable::DIG_OUT:
				tableWidget->item(row, IND_COLUMN_BASE_TYPE)->setText(QString::fromLocal8Bit("Цифровой"));
				break;

			case TypeCable::ANALOG_IN:
				tableWidget->item(row, IND_COLUMN_BASE_TYPE)->setText(QString::fromLocal8Bit("Аналоговый"));
				break;

			case TypeCable::PWM_OUT:
				tableWidget->item(row, IND_COLUMN_BASE_TYPE)->setText(QString::fromLocal8Bit("ШИМ"));
				break;

			case TypeCable::HALL_IN:
			case TypeCable::VNH_OUT:
			case TypeCable::HLD_OUT:
				tableWidget->item(row, IND_COLUMN_BASE_TYPE)->setText(cableRows[i]->typeStr);
				break;
			}
			break;

		case ENGLISH_LANG:
			if (cableRows[i]->direction == "IN")
				tableWidget->item(row, IND_COLUMN_BASE_DIRECTION)->setText(QString("In"));
			else if (cableRows[i]->direction == "OUT")
				tableWidget->item(row, IND_COLUMN_BASE_DIRECTION)->setText(QString("Out"));

			switch (cableRows[i]->typeInt)
			{
			case TypeCable::DIG_IN:
			case TypeCable::DIG_OUT:
				tableWidget->item(row, IND_COLUMN_BASE_TYPE)->setText(QString("Digital"));
				break;

			case TypeCable::ANALOG_IN:
				tableWidget->item(row, IND_COLUMN_BASE_TYPE)->setText(QString("Analog"));
				break;

			case TypeCable::PWM_OUT:
			case TypeCable::HALL_IN:
			case TypeCable::VNH_OUT:
			case TypeCable::HLD_OUT:
				tableWidget->item(row, IND_COLUMN_BASE_TYPE)->setText(cableRows[i]->typeStr);
				break;
			}
			break;
		}
	}

}

void ReportWindow::generateTableAuto()
{
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
		case TypeCable::HLD_OUT:
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
	prepareItem(tableWidget, CELL_SIGN_BASE_COMMENT, SPAN_NONE);

	tableWidget->setColumnWidth(IND_COLUMN_BASE_CONNECTOR, 75);
	tableWidget->setColumnWidth(IND_COLUMN_BASE_PIN, 65);
	tableWidget->setColumnWidth(IND_COLUMN_BASE_DIRECTION, 120);
	tableWidget->setColumnWidth(IND_COLUMN_BASE_TYPE, 90);
	tableWidget->setColumnWidth(IND_COLUMN_BASE_COMMENT, 150);

	tableWidget->setRowHeight(IND_ROW_BASE_SIGN, 40);
	tableWidget->setRowHeight(IND_ROW_BASE_VALUE, 40);

	tableWidget->horizontalHeader()->setSectionResizeMode(IND_COLUMN_BASE_CONNECTOR, QHeaderView::Fixed);
	tableWidget->horizontalHeader()->setSectionResizeMode(IND_COLUMN_BASE_PIN, QHeaderView::Fixed);
	tableWidget->horizontalHeader()->setSectionResizeMode(IND_COLUMN_BASE_DIRECTION, QHeaderView::Fixed);
	tableWidget->horizontalHeader()->setSectionResizeMode(IND_COLUMN_BASE_TYPE, QHeaderView::Fixed);
	tableWidget->horizontalHeader()->setSectionResizeMode(IND_COLUMN_BASE_NAME, QHeaderView::Stretch);
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
	case TypeCable::HLD_OUT:
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

void static fillTableColorOut(TestTableRowProperties* cableRow, int measuredIndex, QTableWidgetItem** tableItems)
{
	if (cableRow->measureds[measuredIndex]->voltage == -1 && cableRow->measureds[measuredIndex]->current == -1)
		return;

	bool disparity = false;

	if (cableRow->measureds[measuredIndex]->voltage < cableRow->thresholds[measuredIndex].minVoltage)
	{
		tableItems[0]->setBackgroundColor(COLOR_RED);
		tableItems[2]->setBackgroundColor(COLOR_RED);
		disparity = true;
	}
	if (cableRow->measureds[measuredIndex]->voltage > cableRow->thresholds[measuredIndex].maxVoltage)
	{
		tableItems[0]->setBackgroundColor(COLOR_RED);
		tableItems[3]->setBackgroundColor(COLOR_RED);
		disparity = true;
	}
	if (!disparity)
		tableItems[0]->setBackgroundColor(COLOR_GREEN);
	
	disparity = false;
	if (cableRow->measureds[measuredIndex]->current < cableRow->thresholds[measuredIndex].minCurrent)
	{
		tableItems[1]->setBackgroundColor(COLOR_RED);
		tableItems[4]->setBackgroundColor(COLOR_RED);
		disparity = true;
	}
	if (cableRow->measureds[measuredIndex]->current > cableRow->thresholds[measuredIndex].maxCurrent)
	{
		tableItems[1]->setBackgroundColor(COLOR_RED);
		tableItems[5]->setBackgroundColor(COLOR_RED);
		disparity = true;
	}
	if (!disparity)
		tableItems[1]->setBackgroundColor(COLOR_GREEN);
}

void static fillTableColorIn(TestTableRowProperties* cableRow, QTableWidgetItem** tableItems)
{
	if (cableRow->measureds[0]->voltage == -1 && cableRow->measureds[0]->current == -1)
		return;

	tableItems[0]->setBackgroundColor(cableRow->measureds[0]->voltage ? COLOR_GREEN : COLOR_RED);
	tableItems[1]->setBackgroundColor(cableRow->measureds[0]->current ? COLOR_GREEN : COLOR_RED);
}

void static fillTableColorInAnalog(TestTableRowProperties* cableRow, int measuredIndex, QTableWidgetItem** tableItems)
{
	if (cableRow->measureds[measuredIndex]->voltage == -1)
		return;

	bool disparity = false;

	if (cableRow->measureds[measuredIndex]->voltage < cableRow->thresholds[measuredIndex].minValue)
	{
		tableItems[0]->setBackgroundColor(COLOR_RED);
		tableItems[1]->setBackgroundColor(COLOR_RED);
		disparity = true;
	}
	if (cableRow->measureds[measuredIndex]->voltage > cableRow->thresholds[measuredIndex].maxValue)
	{
		tableItems[0]->setBackgroundColor(COLOR_RED);
		tableItems[2]->setBackgroundColor(COLOR_RED);
		disparity = true;
	}
	if (!disparity)
		tableItems[0]->setBackgroundColor(COLOR_GREEN);
}

void static fillTableColor(TestTableRowProperties* cableRow, int measuredIndex, QTableWidgetItem** tableItems)
{
	switch (cableRow->typeInt)
	{
	case TypeCable::DIG_OUT:
	case TypeCable::PWM_OUT:
	case TypeCable::VNH_OUT:
	case TypeCable::HLD_OUT:
		fillTableColorOut(cableRow, measuredIndex, tableItems);
		break;
		
	case TypeCable::DIG_IN:
	case TypeCable::HALL_IN:
		fillTableColorIn(cableRow, tableItems);
		break;

	case TypeCable::ANALOG_IN:
		fillTableColorInAnalog(cableRow, measuredIndex, tableItems);
		break;
	}
}

void static fillManualTable(QTableWidgetItem** tableItems, bool checkedState)
{
	tableItems[0]->setText("");
	tableItems[1]->setText("");

	tableItems[0]->setBackgroundColor(checkedState ? COLOR_GREEN : COLOR_RED);
	tableItems[1]->setBackgroundColor(checkedState ? COLOR_GREEN : COLOR_RED);
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
		tableWidget->item(indCurrentRow, IND_COLUMN_BASE_NAME)->setText(cableRows[i]->name);


		switch (viewWindowState->appLanguage)
		{
		case RUSSIAN_LANG:
			if (cableRows[i]->direction == "IN")
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_DIRECTION)->setText(QString::fromLocal8Bit("Вход"));
			else if (cableRows[i]->direction == "OUT")
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_DIRECTION)->setText(QString::fromLocal8Bit("Выход"));

			switch (cableRows[i]->typeInt)
			{
			case TypeCable::DIG_IN:
			case TypeCable::DIG_OUT:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString::fromLocal8Bit("Цифровой"));
				break;

			case TypeCable::ANALOG_IN:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString::fromLocal8Bit("Аналоговый"));
				break;

			case TypeCable::PWM_OUT:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString::fromLocal8Bit("ШИМ"));
				break;

			case TypeCable::HALL_IN:
			case TypeCable::VNH_OUT:
			case TypeCable::HLD_OUT:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(cableRows[i]->typeStr);
				break;
			}
			break;

		case ENGLISH_LANG:
			if (cableRows[i]->direction == "IN")
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_DIRECTION)->setText(QString("In"));
			else if (cableRows[i]->direction == "OUT")
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_DIRECTION)->setText(QString("Out"));

			switch (cableRows[i]->typeInt)
			{
			case TypeCable::DIG_IN:
			case TypeCable::DIG_OUT:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString("Digital"));
				break;

			case TypeCable::ANALOG_IN:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString("Analog"));
				break;

			case TypeCable::PWM_OUT:
			case TypeCable::HALL_IN:
			case TypeCable::VNH_OUT:
			case TypeCable::HLD_OUT:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(cableRows[i]->typeStr);
				break;
			}
			break;
		}

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

			QTableWidgetItem* tableItems[6];
			tableItems[0] = tableWidget->item(indCurrentRow, indColumnMeasuredValuesVoltage);
			tableItems[1] = tableWidget->item(indCurrentRow, indColumnMeasuredValuesCurrent);
			tableItems[2] = tableWidget->item(indCurrentRow, indColumnThresholdsVoltageMin);
			tableItems[3] = tableWidget->item(indCurrentRow, indColumnThresholdsVoltageMax);
			tableItems[4] = tableWidget->item(indCurrentRow, indColumnThresholdsCurrentMin);
			tableItems[5] = tableWidget->item(indCurrentRow, indColumnThresholdsCurrentMax);

			tableItems[0]->setText(cableRows[i]->measureds[j]->voltage != -1 ? QString::number(cableRows[i]->measureds[j]->current) : "-");
			tableItems[1]->setText(cableRows[i]->measureds[j]->current != -1 ? QString::number(cableRows[i]->measureds[j]->voltage) : "-");
			tableItems[2]->setText(cableRows[i]->thresholds[j].minVoltage != -1 ? QString::number(cableRows[i]->thresholds[j].minVoltage) : "-");
			tableItems[3]->setText(cableRows[i]->thresholds[j].maxVoltage != -1 ? QString::number(cableRows[i]->thresholds[j].maxVoltage) : "-");
			tableItems[4]->setText(cableRows[i]->thresholds[j].minCurrent != -1 ? QString::number(cableRows[i]->thresholds[j].minCurrent) : "-");
			tableItems[5]->setText(cableRows[i]->thresholds[j].maxCurrent != -1 ? QString::number(cableRows[i]->thresholds[j].maxCurrent) : "-");

			fillTableColor(cableRows[i], j, tableItems);

			indColumnMeasuredValuesVoltage += MEASUREMENT_OFFSET_OUT;
			indColumnMeasuredValuesCurrent += MEASUREMENT_OFFSET_OUT;
			indColumnThresholdsVoltageMin += MEASUREMENT_OFFSET_OUT;
			indColumnThresholdsVoltageMax += MEASUREMENT_OFFSET_OUT;
			indColumnThresholdsCurrentMin += MEASUREMENT_OFFSET_OUT;
			indColumnThresholdsCurrentMax += MEASUREMENT_OFFSET_OUT;
		}

		commentsTextEdits.push_back(new QTextEdit());
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
		tableWidget->item(indCurrentRow, IND_COLUMN_BASE_NAME)->setText(cableRows[i]->name);

		switch (viewWindowState->appLanguage)
		{
		case RUSSIAN_LANG:
			if (cableRows[i]->direction == "IN")
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_DIRECTION)->setText(QString::fromLocal8Bit("Вход"));
			else if (cableRows[i]->direction == "OUT")
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_DIRECTION)->setText(QString::fromLocal8Bit("Выход"));

			switch (cableRows[i]->typeInt)
			{
			case TypeCable::DIG_IN:
			case TypeCable::DIG_OUT:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString::fromLocal8Bit("Цифровой"));
				break;

			case TypeCable::ANALOG_IN:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString::fromLocal8Bit("Аналоговый"));
				break;

			case TypeCable::PWM_OUT:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString::fromLocal8Bit("ШИМ"));
				break;

			case TypeCable::HALL_IN:
			case TypeCable::VNH_OUT:
			case TypeCable::HLD_OUT:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(cableRows[i]->typeStr);
				break;
			}
			break;

		case ENGLISH_LANG:
			if (cableRows[i]->direction == "IN")
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_DIRECTION)->setText(QString("In"));
			else if (cableRows[i]->direction == "OUT")
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_DIRECTION)->setText(QString("Out"));

			switch (cableRows[i]->typeInt)
			{
			case TypeCable::DIG_IN:
			case TypeCable::DIG_OUT:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString("Digital"));
				break;

			case TypeCable::ANALOG_IN:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString("Analog"));
				break;

			case TypeCable::PWM_OUT:
			case TypeCable::HALL_IN:
			case TypeCable::VNH_OUT:
			case TypeCable::HLD_OUT:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(cableRows[i]->typeStr);
				break;
			}
			break;
		}

		int indColumnMeasuredValue1 = MEASUREMENT_COLUMN_POSITION;
		int indColumnMeasuredValue2 = MEASUREMENT_COLUMN_POSITION + 2;

		prepareItem(tableWidget, indCurrentRow, indColumnMeasuredValue1, SPAN_HORIZONTAL_DOUBLE);
		prepareItem(tableWidget, indCurrentRow, indColumnMeasuredValue2, SPAN_HORIZONTAL_DOUBLE);

		QTableWidgetItem* tableItems[2];
		tableItems[0] = tableWidget->item(indCurrentRow, indColumnMeasuredValue1);
		tableItems[1] = tableWidget->item(indCurrentRow, indColumnMeasuredValue2);

		fillTableColor(cableRows[i], 0, tableItems);

		commentsTextEdits.push_back(new QTextEdit());
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
		tableWidget->item(indCurrentRow, IND_COLUMN_BASE_NAME)->setText(cableRows[i]->name);

		switch (viewWindowState->appLanguage)
		{
		case RUSSIAN_LANG:
			if (cableRows[i]->direction == "IN")
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_DIRECTION)->setText(QString::fromLocal8Bit("Вход"));
			else if (cableRows[i]->direction == "OUT")
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_DIRECTION)->setText(QString::fromLocal8Bit("Выход"));

			switch (cableRows[i]->typeInt)
			{
			case TypeCable::DIG_IN:
			case TypeCable::DIG_OUT:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString::fromLocal8Bit("Цифровой"));
				break;

			case TypeCable::ANALOG_IN:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString::fromLocal8Bit("Аналоговый"));
				break;

			case TypeCable::PWM_OUT:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString::fromLocal8Bit("ШИМ"));
				break;

			case TypeCable::HALL_IN:
			case TypeCable::VNH_OUT:
			case TypeCable::HLD_OUT:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(cableRows[i]->typeStr);
				break;
			}
			break;

		case ENGLISH_LANG:
			if (cableRows[i]->direction == "IN")
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_DIRECTION)->setText(QString("In"));
			else if (cableRows[i]->direction == "OUT")
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_DIRECTION)->setText(QString("Out"));

			switch (cableRows[i]->typeInt)
			{
			case TypeCable::DIG_IN:
			case TypeCable::DIG_OUT:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString("Digital"));
				break;

			case TypeCable::ANALOG_IN:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString("Analog"));
				break;

			case TypeCable::PWM_OUT:
			case TypeCable::HALL_IN:
			case TypeCable::VNH_OUT:
			case TypeCable::HLD_OUT:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(cableRows[i]->typeStr);
				break;
			}
			break;
		}
		
		int indColumnMeasuredValues = MEASUREMENT_COLUMN_POSITION;
		int indColumnThresholdsMin = MEASUREMENT_COLUMN_POSITION + 2;
		int indColumnThresholdsMax = MEASUREMENT_COLUMN_POSITION + 3;

		for (int j = 0; j < cableRows[i]->thresholds.size(); j++)
		{
			prepareItem(tableWidget, indCurrentRow, indColumnMeasuredValues, SPAN_HORIZONTAL_DOUBLE);
			prepareItem(tableWidget, indCurrentRow, indColumnThresholdsMin, SPAN_NONE);
			prepareItem(tableWidget, indCurrentRow, indColumnThresholdsMax, SPAN_NONE);

			QTableWidgetItem* tableItems[3];
			tableItems[0] = tableWidget->item(indCurrentRow, indColumnMeasuredValues);
			tableItems[1] = tableWidget->item(indCurrentRow, indColumnThresholdsMin);
			tableItems[2] = tableWidget->item(indCurrentRow, indColumnThresholdsMax);

			tableItems[0]->setText(cableRows[i]->measureds[j]->voltage != -1 ? QString::number(cableRows[i]->measureds[j]->voltage) : "-");
			tableItems[1]->setText(cableRows[i]->thresholds[j].minValue != -1 ? QString::number(cableRows[i]->thresholds[j].minValue) : "-");
			tableItems[2]->setText(cableRows[i]->thresholds[j].maxValue != -1 ? QString::number(cableRows[i]->thresholds[j].maxValue) : "-");

			fillTableColor(cableRows[i], j, tableItems);

			indColumnMeasuredValues += MEASUREMENT_OFFSET_IN_ANALOG;
			indColumnThresholdsMin += MEASUREMENT_OFFSET_IN_ANALOG;
			indColumnThresholdsMax += MEASUREMENT_OFFSET_IN_ANALOG;
		}

		commentsTextEdits.push_back(new QTextEdit());
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
	case TypeCable::HLD_OUT:
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

void ReportWindow::resaveComments()
{
	if (cableRows[0]->manualCheckBox != nullptr)
		for (int i = 0; i < cableRows.size(); i++)
			cableRows[i]->comment = commentsTextEdits[i]->toPlainText();
	else
	{
		int counter = 0;
		for (int i = 0; i < typedCableRows.size(); i++)
			for (int j = 0; j < typedCableRows[i].size(); j++)
			{
				typedCableRows[i][j]->comment = commentsTextEdits[counter]->toPlainText();
				counter++;
			}
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

void genereateHeaderFile(Document& xlsx, QString testerName, TestBlockName testingBlock, QString actualVersion)
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
	writeHorizontalAlignCell(xlsx, 5, 1, 2, viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Название программы") : QString("App name"), Format::AlignLeft, format);
	writeHorizontalAlignCell(xlsx, 6, 1, 2, viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Комплектация") : QString("Equipment"), Format::AlignLeft, format);
	writeHorizontalAlignCell(xlsx, 7, 1, 2, viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("PART номер") : QString("Part number"), Format::AlignLeft, format);
	writeHorizontalAlignCell(xlsx, 8, 1, 2, viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Серийный номер") : QString("Serial number"), Format::AlignLeft, format);
	writeHorizontalAlignCell(xlsx, 9, 1, 2, viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Дата производства") : QString("Manufacture date"), Format::AlignLeft, format);

	QDateTime time = QDateTime::currentDateTime();

	writeHorizontalAlignCell(xlsx, 1, 3, 6, time.date(), Format::AlignLeft, dateFormat);
	writeHorizontalAlignCell(xlsx, 2, 3, 6, time.time(), Format::AlignLeft, timeFormat);
	writeHorizontalAlignCell(xlsx, 3, 3, 6, testerName, Format::AlignLeft, format);
	writeHorizontalAlignCell(xlsx, 4, 3, 6, testingBlock == TestBlockName::DTM ? QString("DTM ") + actualVersion : QString("BCM ") + actualVersion, Format::AlignLeft, format);
	writeHorizontalAlignCell(xlsx, 5, 3, 6, Can::getDiagBlock(DiagInformation::Calibration_NAME, testingBlock), Format::AlignLeft, format);
	writeHorizontalAlignCell(xlsx, 6, 3, 6, Can::getDiagBlock(DiagInformation::Equipment_NAME, testingBlock), Format::AlignLeft, format);
	writeHorizontalAlignCell(xlsx, 7, 3, 6, Can::getDiagBlock(DiagInformation::Part_NUMBER, testingBlock), Format::AlignLeft, format);
	writeHorizontalAlignCell(xlsx, 8, 3, 6, Can::getDiagBlock(DiagInformation::Serial_NUMBER, testingBlock), Format::AlignLeft, format);
	writeHorizontalAlignCell(xlsx, 9, 3, 6, Can::getDiagBlock(DiagInformation::Manufacture_DATE, testingBlock), Format::AlignLeft, format);


}

void genereateHeaderTable(Document& xlsx, int maxOffset, bool isAutoStand)
{
	Format format;
	format.setHorizontalAlignment(Format::AlignHCenter);
	format.setFontBold(true);
	format.setBorderStyle(Format::BorderThin);
	format.setPatternBackgroundColor(QColor(COLOR_LIGHT_BLUE));

	xlsx.write(HEIGHT_HEADERFILE + 1, 1, viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Колодка") : QString("Pad"), format);
	xlsx.write(HEIGHT_HEADERFILE + 1, 2, viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Пин") : QString("Pin"), format);
	xlsx.write(HEIGHT_HEADERFILE + 1, 3, viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Направление") : QString("Direction"), format);
	xlsx.write(HEIGHT_HEADERFILE + 1, 4, viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Тип") : QString("Type"), format);
	xlsx.write(HEIGHT_HEADERFILE + 1, 5, viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Название") : QString("Name"), format);

	xlsx.setColumnWidth(5, 25);

	Format formatLeftAlg(format);
	formatLeftAlg.setHorizontalAlignment(Format::AlignLeft);
	if (isAutoStand)
	{
		xlsx.write(HEIGHT_HEADERFILE + 1, 6 + maxOffset, viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Комментарий") : QString("Comment"), formatLeftAlg);
		xlsx.setColumnWidth(6 + maxOffset, 50);


		CellRange range(HEIGHT_HEADERFILE + 1, 6, HEIGHT_HEADERFILE + 1, 5 + maxOffset);
		xlsx.mergeCells(range, format);
	}
	else
	{
		xlsx.write(HEIGHT_HEADERFILE + 1, 8, viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Комментарий") : QString("Comment"), formatLeftAlg);
		xlsx.setColumnWidth(8, 50);

		CellRange range(HEIGHT_HEADERFILE + 1, 6, HEIGHT_HEADERFILE + 1, 7);
		xlsx.mergeCells(range, format);
	}
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

QString ReportWindow::getStrType(TypeCable type)
{
	QString str;
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		switch (type)
		{
		case TypeCable::EMPTY:
			generateWarning(Warnings::ReportWindow::TYPE_CHANGE_ERROR);
			str = "Error";
			break;

		case TypeCable::DIG_IN:
		case TypeCable::DIG_OUT:
			str = QString::fromLocal8Bit("Цифровой");
			break;

		case TypeCable::ANALOG_IN:
			str = QString::fromLocal8Bit("Аналоговый");
			break;

		case TypeCable::HALL_IN:
			str = "HAll";
			break;

		case TypeCable::PWM_OUT:
			str = "PWM";
			break;

		case TypeCable::VNH_OUT:
			str = "VNH";
			break;

		case TypeCable::HLD_OUT:
			str = "HLD";
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
		case TypeCable::DIG_OUT:
			str = "Digital";
			break;

		case TypeCable::ANALOG_IN:
			str = "Analog";
			break;

		case TypeCable::HALL_IN:
			str = "HAll";
			break;

		case TypeCable::PWM_OUT:
			str = "PWM";
			break;

		case TypeCable::VNH_OUT:
			str = "VNH";
			break;

		case TypeCable::HLD_OUT:
			str = "HLD";
		}
	}
	return str;
}

void ReportWindow::on_saveButton_clicked()
{
	if (testerNameLineEdit->text() != "")
		testerName = testerNameLineEdit->text();
	else
	{
		generateWarning(Warnings::ReportWindow::EMPTY_INITIALS);
		return;
	}

	if (serialNumberLineEdit->text() != "")
		serialNumber = serialNumberLineEdit->text();
	else
	{
		generateWarning(Warnings::ReportWindow::EMPTY_SERIAL);
		return;
	}

	resaveComments();

	generateXlsx();
}

void ReportWindow::generateXlsx()
{

	typedCableRows.clear();
		typedCableRows.resize(TYPE_COUNT);
	for (int i = 0; i < cableRows.size(); i++)
		typedCableRows[(int)cableRows[i]->typeInt].push_back(cableRows[i]);
	try
	{
		int maxOffset = getMaxColumnOffset(cableRows);

		Document xlsx;
		xlsx.addSheet(viewWindowState->appLanguage == RUSSIAN_LANG ? QString::fromLocal8Bit("Отчёт") : QString("Report"));
		xlsx.currentWorksheet()->setGridLinesVisible(false);
		xlsx.setColumnWidth(1, 5, 13);



		Format format;
		format.setHorizontalAlignment(Format::AlignHCenter);
		format.setFontBold(true);
		format.setBorderStyle(Format::BorderThin);

		//for (int i = 0; i < cableRows.size(); i++)
		//{
		//	xlsx.write(i + 5, 30, cableRows[i]->connectorStr, format);
		//	xlsx.write(i + 5, 31, cableRows[i]->pin, format);
		//	if(checkedState[i])
		//		xlsx.write(i + 5, 32, "Work", format);
		//	else
		//		xlsx.write(i + 5, 32, "No", format);

		//}
		int numRow = START_ROW_TABLE;
		bool color = false;

		genereateHeaderFile(xlsx, testerName, testingBlock, viewWindowState->actualVersion);
		if (cableRows[0]->manualCheckBox != nullptr)
		{
			genereateHeaderTable(xlsx, maxOffset, false);
			for (int i = 0; i < cableRows.size(); i++)
			{
				Format tmpRowFormat(format);
				if (i % 2)
					tmpRowFormat.setPatternBackgroundColor(QColor(COLOR_DIRTY_LIGHT_GREY));
				else
					tmpRowFormat.setPatternBackgroundColor(QColor(COLOR_DIRTY_WHITE));
				Format formatLeftAlg(tmpRowFormat);
				formatLeftAlg.setHorizontalAlignment(Format::AlignLeft);
				xlsx.write(numRow, 1, cableRows[i]->connectorStr + "  (XP" + QString::number((int)cableRows[i]->connectorInt) + ")", tmpRowFormat);
				xlsx.write(numRow, 2, cableRows[i]->pin, tmpRowFormat);
				xlsx.write(numRow, 3, getStrDirection(cableRows[i]->direction), tmpRowFormat);
				xlsx.write(numRow, 4, getStrType(cableRows[i]->typeInt), tmpRowFormat);
				xlsx.write(numRow, 5, cableRows[i]->name, tmpRowFormat);
				xlsx.write(numRow, 8, cableRows[i]->comment, formatLeftAlg);

				Format tmpManualStandFormat(format);
				if (cableRows[i]->manualCheckBox->isChecked())
					tmpManualStandFormat.setPatternBackgroundColor(QColor(COLOR_LIGHT_GREEN));
				else
					tmpManualStandFormat.setPatternBackgroundColor(QColor(COLOR_LIGHT_RED));

				CellRange range(numRow, 6, numRow, 7);
				xlsx.mergeCells(range, tmpManualStandFormat);
				numRow++;
			}
		}
		else
		{
			genereateHeaderTable(xlsx, maxOffset, true);

			for (int type = 0; type < typedCableRows.size(); type++)
			{
				for (int i = 0; i < typedCableRows[type].size(); i++)
				{
					Format tmpHeaderFormat(format);
					tmpHeaderFormat.setPatternBackgroundColor(QColor(COLOR_LIGHT_BLUE)); 
					Format tmpRowFormat(format);
					if (color)
					{
						tmpRowFormat.setPatternBackgroundColor(QColor(COLOR_DIRTY_LIGHT_GREY));
						color = false;
					}
					else
					{
						tmpRowFormat.setPatternBackgroundColor(QColor(COLOR_DIRTY_WHITE));
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
						xlsx.write(numRow, 1, typedCableRows[type][i]->connectorStr + " XP" + QString::number((int)typedCableRows[type][i]->connectorInt), tmpRowFormat);
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
							writeHorizontalAlignCell(xlsx, numRow, 6, 7, "", Format::AlignHCenter, Format(), (typedCableRows[type][i]->measureds[0]->voltage == 0 ? QColor(COLOR_LIGHT_RED) : QColor(COLOR_LIGHT_GREEN)));
							writeHorizontalAlignCell(xlsx, numRow, 8, 9, "", Format::AlignHCenter, Format(), (typedCableRows[type][i]->measureds[0]->current == 0 ? QColor(COLOR_LIGHT_RED) : QColor(COLOR_LIGHT_GREEN)));
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

						xlsx.write(numRow, 1, typedCableRows[type][i]->connectorStr + " XP" + QString::number((int)typedCableRows[type][i]->connectorInt), tmpRowFormat);
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
								tmpRedFormat.setPatternBackgroundColor(COLOR_LIGHT_RED);
								Format tmpGreenFormat(format);
								tmpGreenFormat.setPatternBackgroundColor(COLOR_LIGHT_GREEN);

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

						numRow++;
						break;

					case TypeCable::DIG_OUT:
					case TypeCable::PWM_OUT:
					case TypeCable::VNH_OUT:
					case TypeCable::HLD_OUT:
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

						xlsx.write(numRow, 1, typedCableRows[type][i]->connectorStr + " XP" + QString::number((int)typedCableRows[type][i]->connectorInt), tmpRowFormat);
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
								tmpRedFormat.setPatternBackgroundColor(COLOR_RED);
								Format tmpGreenFormat(format);
								tmpGreenFormat.setPatternBackgroundColor(COLOR_GREEN);
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
		}
		QDir dir;
		QDateTime time = QDateTime::currentDateTime();

		dir.mkdir("Reports");
		QString nameFile = "Reports/";
		if (testingBlock == TestBlockName::DTM)
			nameFile += "DTM_";
		else
			nameFile += "BCM_";
		nameFile += serialNumber.remove("0") + "-";
		nameFile += time.date().toString("dd.MM.yy").replace(".", "_");
		for (int i = 1;; i++)
		{
			QString tmpNameFile = nameFile + "-test" + QString::number(i) + ".xlsx";
			if (!QFile::exists(tmpNameFile))
			{
				nameFile = tmpNameFile;
				break;
			}
		}
		xlsx.saveAs(nameFile);

		QMessageBox::warning(this, QString::fromLocal8Bit("Сохранено"), QString::fromLocal8Bit("Отчёт \"" + nameFile.toLocal8Bit() + "\" сохранён в папку Reports"));
	}
	catch (...)
	{
		generateWarning(Warnings::ReportWindow::XLSX_SAVE_ERROR);
	}
}

void ReportWindow::generateWarning(Warnings::ReportWindow warning)
{
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		switch (warning)
		{
		case Warnings::ReportWindow::XLSX_SAVE_ERROR:
			break;
		}
		break;

	case ENGLISH_LANG:
		switch (warning)
		{
		case Warnings::ReportWindow::XLSX_SAVE_ERROR:
			break;
		}
		break;
	}
}