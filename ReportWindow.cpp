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
	if (viewWindowState->appLanguage == RUSSIAN_LANG)
		testerNameLineEdit->setToolTip(QString("Фамилия Имя Отчесто оператора"));
	else if (viewWindowState->appLanguage == ENGLISH_LANG)
		testerNameLineEdit->setToolTip(QString("Surname Name Operator's patronymic"));
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
	if (viewWindowState->appLanguage == RUSSIAN_LANG)
	{
		if(testingBlock == TestBlockName::DTM)
			serialNumberLineEdit->setToolTip(QString("<p>  Серийный номер блока</p><p><img src = \":/app/icons/DTM_Block.png\"/></p>"));
		else if(testingBlock == TestBlockName::BCM)
			serialNumberLineEdit->setToolTip(QString("<p>  Серийный номер блока</p><p><img src = \":/app/icons/BCM_Block.png\"/></p>"));
	}
	else if (viewWindowState->appLanguage == ENGLISH_LANG)
	{
		if(testingBlock == TestBlockName::DTM)
			serialNumberLineEdit->setToolTip(QString("<p>  Block serial number</p><p><img src = \":/app/icons/DTM_Block.png\"/></p>"));
		else if(testingBlock == TestBlockName::BCM)
			serialNumberLineEdit->setToolTip(QString("<p>  Block serial number</p><p><img src = \":/app/icons/BCM_Block.png\"/></p>"));
	}

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
	if (viewWindowState->appLanguage == RUSSIAN_LANG)
		saveButton->setToolTip(QString("Сохранить отчет о проверхе в формате Excel"));
	else if (viewWindowState->appLanguage == ENGLISH_LANG)
		saveButton->setToolTip(QString("Save the report on the top in Excel format"));
	footerHLayout->addWidget(saveButton);
}

void ReportWindow::resetBaseLanguage()
{
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		tableWidget->item(CELL_SIGN_BASE_CONNECTOR)->setText(QString("Колодка"));
		tableWidget->item(CELL_SIGN_BASE_PIN)->setText(QString("Пин"));
		tableWidget->item(CELL_SIGN_BASE_DIRECTION)->setText(QString("Направление"));
		tableWidget->item(CELL_SIGN_BASE_TYPE)->setText(QString("Тип"));
		tableWidget->item(CELL_SIGN_BASE_NAME)->setText(QString("Название"));
		tableWidget->item(CELL_SIGN_BASE_COMMENT)->setText(QString("Комментарий"));

		testerNameLabel->setText(QString("ФИО: "));
		serialNumberLabel->setText(QString("Серийный номер: "));
		saveButton->setText(QString("Сохранить"));
		testerNameLineEdit->setText(QString("Иванов Иван Иванович"));
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
		testerNameLabel->setStyleSheet(lightStyles.settingSelectText);
		serialNumberLabel->setStyleSheet(lightStyles.settingSelectText);
		testerNameLineEdit->setStyleSheet(lightStyles.reportwindowNameLineEdit);
		serialNumberLineEdit->setStyleSheet(lightStyles.reportwindowSerialLineEdit);
		break;

	case DARK_THEME:
		tableWidget->setStyleSheet(darkStyles.testwindowTableWidget);
		saveButton->setStyleSheet(darkStyles.testwindowButtonStyle);
		testerNameLabel->setStyleSheet(darkStyles.settingSelectText);
		serialNumberLabel->setStyleSheet(darkStyles.settingSelectText);
		testerNameLineEdit->setStyleSheet(darkStyles.reportwindowNameLineEdit);
		serialNumberLineEdit->setStyleSheet(darkStyles.reportwindowSerialLineEdit);
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
			if ((int)cableRows[i]->thresholdsAuto.size() * MEASUREMENT_OFFSET_OUT > maxColumnOffset)
				maxColumnOffset = (int)cableRows[i]->thresholdsAuto.size() * MEASUREMENT_OFFSET_OUT;
			break;

		case TypeCable::DIG_IN:
		case TypeCable::HALL_IN:
			if (MEASUREMENT_OFFSET_IN > maxColumnOffset)
				maxColumnOffset = MEASUREMENT_OFFSET_IN;
			break;

		case TypeCable::ANALOG_IN:
			if ((int)cableRows[i]->thresholdsAuto.size() * MEASUREMENT_OFFSET_IN_ANALOG > maxColumnOffset)
				maxColumnOffset = (int)cableRows[i]->thresholdsAuto.size() * MEASUREMENT_OFFSET_OUT;
			break;
		}

	return maxColumnOffset;
}

static int getMaxTypeOffset(std::vector<TestTableRowProperties*> cableRows)
{
	int maxTypeOffset = 0;
	for (int i = 0; i < cableRows.size(); i++)
		if (maxTypeOffset < (int)cableRows[i]->thresholdsAuto.size())
			maxTypeOffset = (int)cableRows[i]->thresholdsAuto.size();
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
		tableWidget->item(CELL_SIGN_CORRECT)->setText(QString("Статус\nпроверки"));
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
		if (cableRows[i]->pin != "-1")
			tableWidget->item(row, IND_COLUMN_BASE_PIN)->setText(cableRows[i]->pin);
		else
			tableWidget->item(row, IND_COLUMN_BASE_PIN)->setText(cableRows[i]->component);
		tableWidget->item(row, IND_COLUMN_BASE_NAME)->setText(cableRows[i]->name);
		tableWidget->item(row, IND_COLUMN_MANUAL_VALUE)->setBackground(cableRows[i]->manualCheckBox->isChecked() ? QColor(COLOR_GREEN) : QColor(COLOR_RED));


		commentsTextEdits.push_back(new QTextEdit());
		commentsTextEdits[commentsTextEdits.size() - 1]->setStyleSheet(viewWindowState->appTheme ? darkStyles.reportwindowNameLineEdit : lightStyles.reportwindowNameLineEdit);

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
				tableWidget->item(row, IND_COLUMN_BASE_DIRECTION)->setText(QString("Вход"));
			else if (cableRows[i]->direction == "OUT")
				tableWidget->item(row, IND_COLUMN_BASE_DIRECTION)->setText(QString("Выход"));
			
			switch (cableRows[i]->typeInt)
			{
			case TypeCable::DIG_IN:
			case TypeCable::DIG_OUT:
				tableWidget->item(row, IND_COLUMN_BASE_TYPE)->setText(QString("Цифровой"));
				break;

			case TypeCable::ANALOG_IN:
				tableWidget->item(row, IND_COLUMN_BASE_TYPE)->setText(QString("Аналоговый"));
				break;

			case TypeCable::PWM_OUT:
				tableWidget->item(row, IND_COLUMN_BASE_TYPE)->setText(QString("ШИМ"));
				break;

			case TypeCable::HALL_IN:
				tableWidget->item(row, IND_COLUMN_BASE_TYPE)->setText(QString("HALL"));
				break;

			case TypeCable::VNH_OUT:
				tableWidget->item(row, IND_COLUMN_BASE_TYPE)->setText(QString("VNH"));
				break;

			case TypeCable::HLD_OUT:
				tableWidget->item(row, IND_COLUMN_BASE_TYPE)->setText(QString("HLD"));
				break;
			case TypeCable::CAN_OUT:
				tableWidget->item(row, IND_COLUMN_BASE_TYPE)->setText(QString("CAN"));
				break;
			case TypeCable::LIN_OUT:
				tableWidget->item(row, IND_COLUMN_BASE_TYPE)->setText(QString("LIN"));
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
				tableWidget->item(row, IND_COLUMN_BASE_TYPE)->setText(QString("PWM"));
				break;
			case TypeCable::HALL_IN:
				tableWidget->item(row, IND_COLUMN_BASE_TYPE)->setText(QString("HALL"));
				break;

			case TypeCable::VNH_OUT:
				tableWidget->item(row, IND_COLUMN_BASE_TYPE)->setText(QString("VNH"));
				break;

			case TypeCable::HLD_OUT:
				tableWidget->item(row, IND_COLUMN_BASE_TYPE)->setText(QString("HLD"));
				break;
				break;
			case TypeCable::CAN_OUT:
				tableWidget->item(row, IND_COLUMN_BASE_TYPE)->setText(QString("CAN"));
				break;
			case TypeCable::LIN_OUT:
				tableWidget->item(row, IND_COLUMN_BASE_TYPE)->setText(QString("LIN"));
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

	for (int type = 0; type < TYPE_COUNT -1; type++)
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
			//emptySpanColumn += MEASUREMENT_OFFSET_IN * MEASUREMENT_OFFSET_DOUBLE;
			//break;

		case TypeCable::HALL_IN:
		case TypeCable::ANALOG_IN:
		case TypeCable::DIG_IN:
			emptySpanColumn += MEASUREMENT_OFFSET_IN_ANALOG * maxTypeOffset;
			break;

		case TypeCable::DIG_OUT:
		case TypeCable::PWM_OUT:
		case TypeCable::VNH_OUT:
		case TypeCable::HLD_OUT:
			emptySpanColumn += MEASUREMENT_OFFSET_OUT * maxTypeOffset;
			break;
		case TypeCable::CAN_OUT:
		case TypeCable::LIN_OUT:
			emptySpanColumn += MEASUREMENT_OFFSET_INFORMATION * maxTypeOffset;
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
	int indRowMeasured				 = tableWidget->rowCount();
	int indRowMeasuredValues		 = tableWidget->rowCount() + 1;
	int indRowMeasuredVoltage		 = tableWidget->rowCount() + 3;
	int indRowMeasuredCurrent		 = tableWidget->rowCount() + 3;
	int indRowMeasuredADC			 = tableWidget->rowCount() + 3;
	int indRowThresholds			 = tableWidget->rowCount() + 1;
	int indRowThresholdsVoltage		 = tableWidget->rowCount() + 2;
	int indRowThresholdsCurrent		 = tableWidget->rowCount() + 2;
	int indRowThresholdsADC			 = tableWidget->rowCount() + 2;
	int indRowThresholdsVoltageMin	 = tableWidget->rowCount() + 3;
	int indRowThresholdsVoltageMax	 = tableWidget->rowCount() + 3;
	int indRowThresholdsCurrentMin	 = tableWidget->rowCount() + 3;
	int indRowThresholdsCurrentMax	 = tableWidget->rowCount() + 3;
	int indRowThresholdsADCMin		 = tableWidget->rowCount() + 3;
	int indRowThresholdsADCMax		 = tableWidget->rowCount() + 3;

	int indColumnMeasured				 = MEASUREMENT_COLUMN_POSITION;
	int indColumnMeasuredValues			 = MEASUREMENT_COLUMN_POSITION;
	int indColumnMeasuredValuesVoltage	 = MEASUREMENT_COLUMN_POSITION;
	int indColumnMeasuredValuesCurrent	 = MEASUREMENT_COLUMN_POSITION + 1;
	int indColumnMeasuredValuesADC		 = MEASUREMENT_COLUMN_POSITION + 2;
	int indColumnThresholds				 = MEASUREMENT_COLUMN_POSITION + 3;
	int indColumnThresholdsVoltage		 = MEASUREMENT_COLUMN_POSITION + 3;
	int indColumnThresholdsCurrent		 = MEASUREMENT_COLUMN_POSITION + 5;
	int indColumnThresholdsADC			 = MEASUREMENT_COLUMN_POSITION + 7;
	int indColumnThresholdsVoltageMin	 = MEASUREMENT_COLUMN_POSITION + 3;
	int indColumnThresholdsVoltageMax	 = MEASUREMENT_COLUMN_POSITION + 4;
	int indColumnThresholdsCurrentMin	 = MEASUREMENT_COLUMN_POSITION + 5;
	int indColumnThresholdsCurrentMax	 = MEASUREMENT_COLUMN_POSITION + 6;
	int indColumnThresholdsADCMin		 = MEASUREMENT_COLUMN_POSITION + 7;
	int indColumnThresholdsADCMax		 = MEASUREMENT_COLUMN_POSITION + 8;

	for (int i = 0; i < ROW_COUNT_SIGN_OUT; i++)
		tableWidget->insertRow(tableWidget->rowCount());

	tableWidget->setSpan(indRowMeasured, 0, SPAN_TYPE_OUT);
	tableWidget->setSpan(indRowMeasured, tableWidget->columnCount() - 1, SPAN_TYPE_COMMENT_OUT);

	for (int i = 0; i < maxTypeOffset; i++)
	{
		prepareItem(tableWidget, indRowMeasured, indColumnMeasured, SPAN_HORIZONTAL_NINETUPEL);
		prepareItem(tableWidget, indRowMeasuredValues, indColumnMeasuredValues, SPAN_SQUAD_TRIPL);
		prepareItem(tableWidget, indRowMeasuredVoltage, indColumnMeasuredValuesVoltage, SPAN_NONE);
		prepareItem(tableWidget, indRowMeasuredCurrent, indColumnMeasuredValuesCurrent, SPAN_NONE);
		prepareItem(tableWidget, indRowMeasuredADC, indColumnMeasuredValuesADC, SPAN_NONE);
		prepareItem(tableWidget, indRowThresholds, indColumnThresholds, SPAN_HORIZONTAL_SEXTUPLE);
		prepareItem(tableWidget, indRowThresholdsVoltage, indColumnThresholdsVoltage, SPAN_HORIZONTAL_DOUBLE);
		prepareItem(tableWidget, indRowThresholdsCurrent, indColumnThresholdsCurrent, SPAN_HORIZONTAL_DOUBLE);
		prepareItem(tableWidget, indRowThresholdsADC, indColumnThresholdsADC, SPAN_HORIZONTAL_DOUBLE);
		prepareItem(tableWidget, indRowThresholdsVoltageMin, indColumnThresholdsVoltageMin, SPAN_NONE);
		prepareItem(tableWidget, indRowThresholdsVoltageMax, indColumnThresholdsVoltageMax, SPAN_NONE);
		prepareItem(tableWidget, indRowThresholdsCurrentMin, indColumnThresholdsCurrentMin, SPAN_NONE);
		prepareItem(tableWidget, indRowThresholdsCurrentMax, indColumnThresholdsCurrentMax, SPAN_NONE);
		prepareItem(tableWidget, indRowThresholdsADCMin, indColumnThresholdsADCMin, SPAN_NONE);
		prepareItem(tableWidget, indRowThresholdsADCMax, indColumnThresholdsADCMax, SPAN_NONE);

		//tableWidget->setColumnWidth(indColumnMeasuredValuesVoltage, 60);
		//tableWidget->setColumnWidth(indColumnMeasuredValuesCurrent, 60);
		//tableWidget->setColumnWidth(indColumnMeasuredValuesADC, 60);

		switch (viewWindowState->appLanguage)
		{
		case RUSSIAN_LANG:
			tableWidget->item(indRowMeasured, indColumnMeasured)->setText(QString("Измерение ") + QString::number(i + 1));
			tableWidget->item(indRowMeasuredValues, indColumnMeasuredValues)->setText(QString("Измереное значение"));
			tableWidget->item(indRowMeasuredVoltage, indColumnMeasuredValuesVoltage)->setText(QString("U, В"));
			tableWidget->item(indRowMeasuredCurrent, indColumnMeasuredValuesCurrent)->setText(QString("I, A"));
			tableWidget->item(indRowMeasuredCurrent, indColumnMeasuredValuesADC)->setText(QString("АЦП"));
			tableWidget->item(indRowThresholds, indColumnThresholds)->setText(QString("Пороги"));
			tableWidget->item(indRowThresholdsVoltage, indColumnThresholdsVoltage)->setText(QString("U, A"));
			tableWidget->item(indRowThresholdsCurrent, indColumnThresholdsCurrent)->setText(QString("I, A"));
			tableWidget->item(indRowThresholdsCurrent, indColumnThresholdsADC)->setText(QString("АЦП"));
			tableWidget->item(indRowThresholdsVoltageMin, indColumnThresholdsVoltageMin)->setText(QString("Мин"));
			tableWidget->item(indRowThresholdsVoltageMax, indColumnThresholdsVoltageMax)->setText(QString("Макс"));
			tableWidget->item(indRowThresholdsCurrentMin, indColumnThresholdsCurrentMin)->setText(QString("Мин"));
			tableWidget->item(indRowThresholdsCurrentMax, indColumnThresholdsCurrentMax)->setText(QString("Макс"));
			tableWidget->item(indRowThresholdsADCMin, indColumnThresholdsADCMin)->setText(QString("Мин"));
			tableWidget->item(indRowThresholdsADCMax, indColumnThresholdsADCMax)->setText(QString("Макс"));
			break;

		case ENGLISH_LANG:
			tableWidget->item(indRowMeasured, indColumnMeasured)->setText(QString("Measured ") + QString::number(i + 1));
			tableWidget->item(indRowMeasuredValues, indColumnMeasuredValues)->setText(QString("Measured values"));
			tableWidget->item(indRowMeasuredVoltage, indColumnMeasuredValuesVoltage)->setText(QString("U, V"));
			tableWidget->item(indRowMeasuredCurrent, indColumnMeasuredValuesCurrent)->setText(QString("I, A"));
			tableWidget->item(indRowMeasuredCurrent, indColumnMeasuredValuesADC)->setText(QString("ADC"));
			tableWidget->item(indRowThresholds, indColumnThresholds)->setText(QString("Thresholds"));
			tableWidget->item(indRowThresholdsVoltage, indColumnThresholdsVoltage)->setText(QString("U, V"));
			tableWidget->item(indRowThresholdsCurrent, indColumnThresholdsCurrent)->setText(QString("I, A"));
			tableWidget->item(indRowThresholdsCurrent, indColumnThresholdsADC)->setText(QString("ADC"));
			tableWidget->item(indRowThresholdsVoltageMin, indColumnThresholdsVoltageMin)->setText(QString("Min"));
			tableWidget->item(indRowThresholdsVoltageMax, indColumnThresholdsVoltageMax)->setText(QString("Max"));
			tableWidget->item(indRowThresholdsCurrentMin, indColumnThresholdsCurrentMin)->setText(QString("Min"));
			tableWidget->item(indRowThresholdsCurrentMax, indColumnThresholdsCurrentMax)->setText(QString("Max"));
			tableWidget->item(indRowThresholdsADCMin, indColumnThresholdsADCMin)->setText(QString("Min"));
			tableWidget->item(indRowThresholdsADCMax, indColumnThresholdsADCMax)->setText(QString("Max"));
			break;
		}

		indColumnMeasured += MEASUREMENT_OFFSET_OUT;
		indColumnMeasuredValues += MEASUREMENT_OFFSET_OUT;
		indColumnMeasuredValuesVoltage += MEASUREMENT_OFFSET_OUT;
		indColumnMeasuredValuesCurrent += MEASUREMENT_OFFSET_OUT;
		indColumnMeasuredValuesADC += MEASUREMENT_OFFSET_OUT;
		indColumnThresholds += MEASUREMENT_OFFSET_OUT;
		indColumnThresholdsVoltage += MEASUREMENT_OFFSET_OUT;
		indColumnThresholdsCurrent += MEASUREMENT_OFFSET_OUT;
		indColumnThresholdsADC += MEASUREMENT_OFFSET_OUT;
		indColumnThresholdsVoltageMin += MEASUREMENT_OFFSET_OUT;
		indColumnThresholdsVoltageMax += MEASUREMENT_OFFSET_OUT;
		indColumnThresholdsCurrentMin += MEASUREMENT_OFFSET_OUT;
		indColumnThresholdsCurrentMax += MEASUREMENT_OFFSET_OUT;
		indColumnThresholdsADCMin += MEASUREMENT_OFFSET_OUT;
		indColumnThresholdsADCMax += MEASUREMENT_OFFSET_OUT;
	}
}

static void generateTableSignIn(QTableWidget* tableWidget, int maxTypeOffset)
{
	int indRowMeasured = tableWidget->rowCount();
	int indRowMeasuredValues = tableWidget->rowCount() + 1;
	int indRowSetValue = tableWidget->rowCount() + 1;
	//int indRowThresholdsMin = tableWidget->rowCount() + 2;
	//int indRowThresholdsMax = tableWidget->rowCount() + 2;

	int indColumnMeasured = MEASUREMENT_COLUMN_POSITION;
	int indColumnMeasuredValues = MEASUREMENT_COLUMN_POSITION;
	int indColumnSetValue = MEASUREMENT_COLUMN_POSITION + 2;
	//int indColumnThresholds = MEASUREMENT_COLUMN_POSITION + 2;
	//int indColumnThresholdsMin = MEASUREMENT_COLUMN_POSITION + 2;
	//int indColumnThresholdsMax = MEASUREMENT_COLUMN_POSITION + 3;

	for (int i = 0; i < ROW_COUNT_SIGN_IN_ANALOG; i++)
		tableWidget->insertRow(tableWidget->rowCount());

	tableWidget->setSpan(indRowMeasured, 0, SPAN_TYPE_IN_ANALOG);
	tableWidget->setSpan(indRowMeasured, tableWidget->columnCount() - 1, SPAN_TYPE_COMMENT_IN_ANALOG);

	for (int i = 0; i < maxTypeOffset; i++)
	{
		prepareItem(tableWidget, indRowMeasured, indColumnMeasured, SPAN_HORIZONTAL_QUADRUPLE);
		prepareItem(tableWidget, indRowMeasuredValues, indColumnMeasuredValues, SPAN_SQUAD_DOUBLE);
		prepareItem(tableWidget, indRowSetValue, indColumnSetValue, SPAN_SQUAD_DOUBLE);

		//prepareItem(tableWidget, indRowThresholds, indColumnThresholds, SPAN_HORIZONTAL_DOUBLE);
		//prepareItem(tableWidget, indRowThresholdsMin, indColumnThresholdsMin, SPAN_NONE);
		//prepareItem(tableWidget, indRowThresholdsMax, indColumnThresholdsMax, SPAN_NONE);

		switch (viewWindowState->appLanguage)
		{
		case RUSSIAN_LANG:
			tableWidget->item(indRowMeasured, indColumnMeasured)->setText(QString("Измерение ") + QString::number(i + 1));
			tableWidget->item(indRowMeasuredValues, indColumnMeasuredValues)->setText(QString("Измереное значение"));
			tableWidget->item(indRowSetValue, indColumnSetValue)->setText(QString("Заданое значение"));
			//tableWidget->item(indRowThresholds, indColumnThresholds)->setText(QString("Пороги"));
			//tableWidget->item(indRowThresholdsMin, indColumnThresholdsMin)->setText(QString("Мин"));
			//tableWidget->item(indRowThresholdsMax, indColumnThresholdsMax)->setText(QString("Макс"));
			break;

		case ENGLISH_LANG:
			tableWidget->item(indRowMeasured, indColumnMeasured)->setText(QString("Measured ") + QString::number(i + 1));
			tableWidget->item(indRowMeasuredValues, indColumnMeasuredValues)->setText(QString("Measured values"));
			tableWidget->item(indRowSetValue, indColumnSetValue)->setText(QString("Set value"));
			//tableWidget->item(indRowThresholds, indColumnThresholds)->setText(QString("Thresholds"));
			//tableWidget->item(indRowThresholdsMin, indColumnThresholdsMin)->setText(QString("Min"));
			//tableWidget->item(indRowThresholdsMax, indColumnThresholdsMax)->setText(QString("Max"));
			break;
		}

		indColumnMeasured += MEASUREMENT_OFFSET_IN_ANALOG;
		indColumnMeasuredValues += MEASUREMENT_OFFSET_IN_ANALOG;
		indColumnSetValue += MEASUREMENT_OFFSET_IN_ANALOG;
		//indColumnThresholds += MEASUREMENT_OFFSET_IN_ANALOG;
		//indColumnThresholdsMin += MEASUREMENT_OFFSET_IN_ANALOG;
		//indColumnThresholdsMax += MEASUREMENT_OFFSET_IN_ANALOG;
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
			tableWidget->item(indRowMeasured, indColumnMeasured)->setText(QString("Измерение ") + QString::number(i + 1));
			tableWidget->item(indRowMeasuredValues, indColumnMeasuredValues)->setText(QString("Измереное значение"));
			tableWidget->item(indRowThresholds, indColumnThresholds)->setText(QString("Пороги"));
			tableWidget->item(indRowThresholdsMin, indColumnThresholdsMin)->setText(QString("Мин"));
			tableWidget->item(indRowThresholdsMax, indColumnThresholdsMax)->setText(QString("Макс"));
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

static void generateTableSignInformation(QTableWidget* tableWidget)
{
	int indRowMeasured = tableWidget->rowCount();

	int indColumnMeasured1 = MEASUREMENT_COLUMN_POSITION;
	int indColumnMeasured2 = MEASUREMENT_COLUMN_POSITION + 2;

	tableWidget->insertRow(tableWidget->rowCount());

	tableWidget->setSpan(indRowMeasured, 0, SPAN_TYPE_IN);
	tableWidget->setSpan(indRowMeasured, tableWidget->columnCount() - 1, SPAN_TYPE_COMMENT_IN);

	prepareItem(tableWidget, indRowMeasured, indColumnMeasured1, SPAN_HORIZONTAL_DOUBLE);
	//prepareItem(tableWidget, indRowMeasured, indColumnMeasured2, SPAN_HORIZONTAL_DOUBLE);

	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		tableWidget->item(indRowMeasured, indColumnMeasured1)->setText(QString("Статус"));
		//tableWidget->item(indRowMeasured, indColumnMeasured2)->setText(QString("Значение 2"));
		break;

	case ENGLISH_LANG:
		tableWidget->item(indRowMeasured, indColumnMeasured1)->setText(QString("Status"));
		//tableWidget->item(indRowMeasured, indColumnMeasured2)->setText(QString("Value 2"));
		break;
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
		generateTableSignIn(tableWidget, maxTypeOffset);
		break;

	case TypeCable::ANALOG_IN:
		generateTableSignInAnalog(tableWidget, maxTypeOffset);
		break;
	case TypeCable::CAN_OUT:
	case TypeCable::LIN_OUT:
		generateTableSignInformation(tableWidget);
		break;
	case TypeCable::SLEEP:
		break;
	}
}

void static fillTableColorOut(TestTableRowProperties* cableRow, int measuredIndex, QTableWidgetItem** tableItems)
{
	if (cableRow->measureds.size() == 0)
		return;

	bool disparity = false;

	if (cableRow->measureds[measuredIndex]->voltage < cableRow->thresholdsAuto[measuredIndex].minVoltage)
	{
		tableItems[0]->setBackground(QBrush(COLOR_RED));
		tableItems[3]->setBackground(QBrush(COLOR_RED));
		disparity = true;
	}
	if (cableRow->measureds[measuredIndex]->voltage > cableRow->thresholdsAuto[measuredIndex].maxVoltage)
	{
		tableItems[0]->setBackground(QBrush(COLOR_RED));
		tableItems[4]->setBackground(QBrush(COLOR_RED));
		disparity = true;
	}
	if (!disparity)
		tableItems[0]->setBackground(QBrush(COLOR_GREEN));
	
	disparity = false;
	if (cableRow->measureds[measuredIndex]->current < cableRow->thresholdsAuto[measuredIndex].minCurrent)
	{
		tableItems[1]->setBackground(QBrush(COLOR_RED));
		tableItems[5]->setBackground(QBrush(COLOR_RED));
		disparity = true;
	}
	if (cableRow->measureds[measuredIndex]->current > cableRow->thresholdsAuto[measuredIndex].maxCurrent)
	{
		tableItems[1]->setBackground(QBrush(COLOR_RED));
		tableItems[6]->setBackground(QBrush(COLOR_RED));
		disparity = true;
	}
	if (!disparity)
		tableItems[1]->setBackground(QBrush(COLOR_GREEN));
	disparity = false;
	if (cableRow->measureds[measuredIndex]->digValue < cableRow->thresholdsAuto[measuredIndex].minValue)
	{
		tableItems[2]->setBackground(QBrush(COLOR_RED));
		tableItems[7]->setBackground(QBrush(COLOR_RED));
		disparity = true;
	}
	if (cableRow->measureds[measuredIndex]->digValue > cableRow->thresholdsAuto[measuredIndex].maxValue)
	{
		tableItems[2]->setBackground(QBrush(COLOR_RED));
		tableItems[8]->setBackground(QBrush(COLOR_RED));
		disparity = true;
	}
	if (!disparity)
		tableItems[2]->setBackground(QBrush(COLOR_GREEN));
}

void static fillTableColorIn(TestTableRowProperties* cableRow, QTableWidgetItem** tableItems)
{
	if (cableRow->measureds.size() == 0)
	{

		tableItems[0]->setBackground(QBrush(COLOR_RED));
		return;
	}

	tableItems[0]->setBackground(QBrush(cableRow->measureds[0]->digValue ? COLOR_GREEN : COLOR_RED));
	//tableItems[1]->setBackground(QBrush(cableRow->measureds[0]->current ? COLOR_GREEN : COLOR_RED));
}

void static fillTableColorInAnalog(TestTableRowProperties* cableRow, int measuredIndex, QTableWidgetItem** tableItems)
{
	if(cableRow->measureds.size() == 0)
		return;

	bool disparity = false;

	if (cableRow->measureds[measuredIndex]->digValue < cableRow->thresholdsAuto[measuredIndex].minValue)
	{
		tableItems[0]->setBackground(QBrush(COLOR_RED));
		tableItems[1]->setBackground(QBrush(COLOR_RED));
		disparity = true;
	}
	if (cableRow->measureds[measuredIndex]->digValue > cableRow->thresholdsAuto[measuredIndex].maxValue)
	{               
		tableItems[0]->setBackground(QBrush(COLOR_RED));
		tableItems[2]->setBackground(QBrush(COLOR_RED));
		disparity = true;
	}
	if (!disparity)
		tableItems[0]->setBackground(QBrush(COLOR_GREEN));
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
		
	case TypeCable::CAN_OUT:
	case TypeCable::LIN_OUT:
		fillTableColorIn(cableRow, tableItems);
		break;

	case TypeCable::HALL_IN:
	case TypeCable::ANALOG_IN:
	case TypeCable::DIG_IN:
		fillTableColorInAnalog(cableRow, measuredIndex, tableItems);
		break;
	}
}

void static fillManualTable(QTableWidgetItem** tableItems, bool checkedState)
{
	tableItems[0]->setText("");
	tableItems[1]->setText("");

	tableItems[0]->setBackground(QBrush(checkedState ? COLOR_GREEN : COLOR_RED));
	tableItems[1]->setBackground(QBrush(checkedState ? COLOR_GREEN : COLOR_RED));
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
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_DIRECTION)->setText(QString("Вход"));
			else if (cableRows[i]->direction == "OUT")
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_DIRECTION)->setText(QString("Выход"));

			switch (cableRows[i]->typeInt)
			{
			case TypeCable::DIG_IN:
			case TypeCable::DIG_OUT:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString("Цифровой"));
				break;

			case TypeCable::ANALOG_IN:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString("Аналоговый"));
				break;

			case TypeCable::PWM_OUT:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString("ШИМ"));
				break;

			case TypeCable::HALL_IN:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString("HALL"));
				break;

			case TypeCable::VNH_OUT:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString("VNH"));
				break;

			case TypeCable::HLD_OUT:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString("HLD"));
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
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString("PWM"));
				break;
			case TypeCable::HALL_IN:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString("HALL"));
				break;

			case TypeCable::VNH_OUT:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString("VNH"));
				break;

			case TypeCable::HLD_OUT:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString("HLD"));
				break;
			}
			break;
		}

		int indColumnMeasuredValuesVoltage	= MEASUREMENT_COLUMN_POSITION;
		int indColumnMeasuredValuesCurrent	= MEASUREMENT_COLUMN_POSITION + 1;
		int indColumnMeasuredValuesADC		= MEASUREMENT_COLUMN_POSITION + 2;
		int indColumnThresholdsVoltageMin	= MEASUREMENT_COLUMN_POSITION + 3;
		int indColumnThresholdsVoltageMax	= MEASUREMENT_COLUMN_POSITION + 4;
		int indColumnThresholdsCurrentMin	= MEASUREMENT_COLUMN_POSITION + 5;
		int indColumnThresholdsCurrentMax	= MEASUREMENT_COLUMN_POSITION + 6;
		int indColumnThresholdsADCMin		= MEASUREMENT_COLUMN_POSITION + 7;
		int indColumnThresholdsADCMax		= MEASUREMENT_COLUMN_POSITION + 8;

		for (int j = 0; j < cableRows[i]->thresholdsAuto.size(); j++)
		{
			prepareItem(tableWidget, indCurrentRow, indColumnMeasuredValuesVoltage, SPAN_NONE);
			prepareItem(tableWidget, indCurrentRow, indColumnMeasuredValuesCurrent, SPAN_NONE);
			prepareItem(tableWidget, indCurrentRow, indColumnMeasuredValuesADC, SPAN_NONE);
			prepareItem(tableWidget, indCurrentRow, indColumnThresholdsVoltageMin, SPAN_NONE);
			prepareItem(tableWidget, indCurrentRow, indColumnThresholdsVoltageMax, SPAN_NONE);
			prepareItem(tableWidget, indCurrentRow, indColumnThresholdsCurrentMin, SPAN_NONE);
			prepareItem(tableWidget, indCurrentRow, indColumnThresholdsCurrentMax, SPAN_NONE);
			prepareItem(tableWidget, indCurrentRow, indColumnThresholdsADCMin, SPAN_NONE);
			prepareItem(tableWidget, indCurrentRow, indColumnThresholdsADCMax, SPAN_NONE);

			QTableWidgetItem* tableItems[9];
			tableItems[0] = tableWidget->item(indCurrentRow, indColumnMeasuredValuesVoltage);
			tableItems[1] = tableWidget->item(indCurrentRow, indColumnMeasuredValuesCurrent);
			tableItems[2] = tableWidget->item(indCurrentRow, indColumnMeasuredValuesADC);
			tableItems[3] = tableWidget->item(indCurrentRow, indColumnThresholdsVoltageMin);
			tableItems[4] = tableWidget->item(indCurrentRow, indColumnThresholdsVoltageMax);
			tableItems[5] = tableWidget->item(indCurrentRow, indColumnThresholdsCurrentMin);
			tableItems[6] = tableWidget->item(indCurrentRow, indColumnThresholdsCurrentMax);
			tableItems[7] = tableWidget->item(indCurrentRow, indColumnThresholdsADCMin);
			tableItems[8] = tableWidget->item(indCurrentRow, indColumnThresholdsADCMax);

			if (cableRows[i]->measureds.size() != 0)
			{
				tableItems[0]->setText(cableRows[i]->measureds[j]->voltage != -1 ? QString::number(cableRows[i]->measureds[j]->voltage, 'f', 2) : "-");
				tableItems[1]->setText(cableRows[i]->measureds[j]->current != -1 ? QString::number(cableRows[i]->measureds[j]->current, 'f', 2) : "-");
				tableItems[2]->setText(cableRows[i]->measureds[j]->current != -1 ? QString::number(cableRows[i]->measureds[j]->digValue, 'f', 2) : "-");
			}
			else
			{
				tableItems[0]->setText("-");
				tableItems[1]->setText("-");
				tableItems[2]->setText("-");
			}
			tableItems[3]->setText(cableRows[i]->thresholdsAuto[j].minVoltage != -1 ? QString::number(cableRows[i]->thresholdsAuto[j].minVoltage) : "-");
			tableItems[4]->setText(cableRows[i]->thresholdsAuto[j].maxVoltage != -1 ? QString::number(cableRows[i]->thresholdsAuto[j].maxVoltage) : "-");
			tableItems[5]->setText(cableRows[i]->thresholdsAuto[j].minCurrent != -1 ? QString::number(cableRows[i]->thresholdsAuto[j].minCurrent) : "-");
			tableItems[6]->setText(cableRows[i]->thresholdsAuto[j].maxCurrent != -1 ? QString::number(cableRows[i]->thresholdsAuto[j].maxCurrent) : "-");
			tableItems[7]->setText(cableRows[i]->thresholdsAuto[j].minCurrent != -1 ? QString::number(cableRows[i]->thresholdsAuto[j].minValue) : "-");
			tableItems[8]->setText(cableRows[i]->thresholdsAuto[j].maxCurrent != -1 ? QString::number(cableRows[i]->thresholdsAuto[j].maxValue) : "-");

			fillTableColor(cableRows[i], j, tableItems);

			indColumnMeasuredValuesVoltage += MEASUREMENT_OFFSET_OUT;
			indColumnMeasuredValuesCurrent += MEASUREMENT_OFFSET_OUT;
			indColumnMeasuredValuesADC += MEASUREMENT_OFFSET_OUT;
			indColumnThresholdsVoltageMin += MEASUREMENT_OFFSET_OUT;
			indColumnThresholdsVoltageMax += MEASUREMENT_OFFSET_OUT;
			indColumnThresholdsCurrentMin += MEASUREMENT_OFFSET_OUT;
			indColumnThresholdsCurrentMax += MEASUREMENT_OFFSET_OUT;
			indColumnThresholdsADCMin += MEASUREMENT_OFFSET_OUT;
			indColumnThresholdsADCMax += MEASUREMENT_OFFSET_OUT;
		}

		commentsTextEdits.push_back(new QTextEdit());
		commentsTextEdits[commentsTextEdits.size() - 1]->setStyleSheet(viewWindowState->appTheme ? darkStyles.reportwindowNameLineEdit : lightStyles.reportwindowNameLineEdit);

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
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_DIRECTION)->setText(QString("Вход"));
			else if (cableRows[i]->direction == "OUT")
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_DIRECTION)->setText(QString("Выход"));

			switch (cableRows[i]->typeInt)
			{
			case TypeCable::DIG_IN:
			case TypeCable::DIG_OUT:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString("Цифровой"));
				break;

			case TypeCable::ANALOG_IN:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString("Аналоговый"));
				break;

			case TypeCable::PWM_OUT:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString("ШИМ"));
				break;

			case TypeCable::HALL_IN:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString("HALL"));
				break;

			case TypeCable::VNH_OUT:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString("VNH"));
				break;

			case TypeCable::HLD_OUT:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString("HLD"));
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
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString("PWM"));
				break;

			case TypeCable::HALL_IN:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString("HAL"));
				break;

			case TypeCable::VNH_OUT:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString("VNH"));
				break;

			case TypeCable::HLD_OUT:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString("HLD"));
				break;
			}
			break;
		}

		int indColumnMeasuredValue = MEASUREMENT_COLUMN_POSITION;
		int indColumnSetValue = MEASUREMENT_COLUMN_POSITION + 2;

		//int indColumnMeasuredValues = MEASUREMENT_COLUMN_POSITION;
		//int indColumnThresholdsMin = MEASUREMENT_COLUMN_POSITION + 2;
		//int indColumnThresholdsMax = MEASUREMENT_COLUMN_POSITION + 3;

		//for (int j = 0; j < cableRows[i]->thresholdsAuto.size(); j++)
		//{
		//	prepareItem(tableWidget, indCurrentRow, indColumnMeasuredValues, SPAN_HORIZONTAL_DOUBLE);
		//	prepareItem(tableWidget, indCurrentRow, indColumnThresholdsMin, SPAN_NONE);
		//	prepareItem(tableWidget, indCurrentRow, indColumnThresholdsMax, SPAN_NONE);

		//	QTableWidgetItem* tableItems[3];
		//	tableItems[0] = tableWidget->item(indCurrentRow, indColumnMeasuredValues);
		//	tableItems[1] = tableWidget->item(indCurrentRow, indColumnThresholdsMin);
		//	tableItems[2] = tableWidget->item(indCurrentRow, indColumnThresholdsMax);

		//	if (cableRows[i]->measureds.size() != 0)
		//		tableItems[0]->setText(cableRows[i]->measureds[j]->digValue != -1 ? QString::number(cableRows[i]->measureds[j]->digValue) : "-");
		//	else
		//		tableItems[0]->setText("-");

		//	tableItems[1]->setText(cableRows[i]->thresholdsAuto[j].minValue != -1 ? QString::number(cableRows[i]->thresholdsAuto[j].minValue) : "-");
		//	tableItems[2]->setText(cableRows[i]->thresholdsAuto[j].maxValue != -1 ? QString::number(cableRows[i]->thresholdsAuto[j].maxValue) : "-");

		//	fillTableColor(cableRows[i], j, tableItems);

		//	indColumnMeasuredValues += MEASUREMENT_OFFSET_IN_ANALOG;
		//	indColumnThresholdsMin += MEASUREMENT_OFFSET_IN_ANALOG;
		//	indColumnThresholdsMax += MEASUREMENT_OFFSET_IN_ANALOG;
		//}

		for (int j = 0; j < cableRows[i]->thresholdsAuto.size(); j++)
		{
			prepareItem(tableWidget, indCurrentRow, indColumnMeasuredValue, SPAN_HORIZONTAL_DOUBLE);
			prepareItem(tableWidget, indCurrentRow, indColumnSetValue, SPAN_HORIZONTAL_DOUBLE);

			QTableWidgetItem* tableItems[2];
			tableItems[0] = tableWidget->item(indCurrentRow, indColumnMeasuredValue);
			tableItems[1] = tableWidget->item(indCurrentRow, indColumnSetValue);

			if (cableRows[i]->measureds.size() != 0)
				tableItems[0]->setText(cableRows[i]->measureds[j]->digValue != -1 ? QString::number(cableRows[i]->measureds[j]->digValue) : "-");
			else
				tableItems[0]->setText("-");

			tableItems[1]->setText(cableRows[i]->thresholdsAuto[j].maxValue != -1 ? QString::number((cableRows[i]->thresholdsAuto[j].minValue + cableRows[i]->thresholdsAuto[j].maxValue) / 2) : "-");

			fillTableColor(cableRows[i], 0, tableItems);

			indColumnMeasuredValue += MEASUREMENT_OFFSET_IN_ANALOG;
			indColumnSetValue += MEASUREMENT_OFFSET_IN_ANALOG;
		}

		commentsTextEdits.push_back(new QTextEdit());
		commentsTextEdits[commentsTextEdits.size() - 1]->setStyleSheet(viewWindowState->appTheme ? darkStyles.reportwindowNameLineEdit : lightStyles.reportwindowNameLineEdit);

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
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_DIRECTION)->setText(QString("Вход"));
			else if (cableRows[i]->direction == "OUT")
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_DIRECTION)->setText(QString("Выход"));

			switch (cableRows[i]->typeInt)
			{
			case TypeCable::DIG_IN:
			case TypeCable::DIG_OUT:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString("Цифровой"));
				break;

			case TypeCable::ANALOG_IN:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString("Аналоговый"));
				break;

			case TypeCable::PWM_OUT:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString("ШИМ"));
				break;

			case TypeCable::HALL_IN:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString("HALL"));
				break;

			case TypeCable::VNH_OUT:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString("VNH"));
				break;

			case TypeCable::HLD_OUT:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString("HLD"));
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
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString("PWM"));
				break;

			case TypeCable::HALL_IN:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString("HALL"));
				break;

			case TypeCable::VNH_OUT:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString("VNH"));
				break;

			case TypeCable::HLD_OUT:
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString("HLD"));
				break;
			}
			break;
		}
		
		int indColumnMeasuredValues = MEASUREMENT_COLUMN_POSITION;
		int indColumnThresholdsMin = MEASUREMENT_COLUMN_POSITION + 2;
		int indColumnThresholdsMax = MEASUREMENT_COLUMN_POSITION + 3;

		for (int j = 0; j < cableRows[i]->thresholdsAuto.size(); j++)
		{
			prepareItem(tableWidget, indCurrentRow, indColumnMeasuredValues, SPAN_HORIZONTAL_DOUBLE);
			prepareItem(tableWidget, indCurrentRow, indColumnThresholdsMin, SPAN_NONE);
			prepareItem(tableWidget, indCurrentRow, indColumnThresholdsMax, SPAN_NONE);

			QTableWidgetItem* tableItems[3];
			tableItems[0] = tableWidget->item(indCurrentRow, indColumnMeasuredValues);
			tableItems[1] = tableWidget->item(indCurrentRow, indColumnThresholdsMin);
			tableItems[2] = tableWidget->item(indCurrentRow, indColumnThresholdsMax);

			if(cableRows[i]->measureds.size() != 0)
				tableItems[0]->setText(cableRows[i]->measureds[j]->digValue != -1 ? QString::number(cableRows[i]->measureds[j]->digValue) : "-");
			else
				tableItems[0]->setText("-");

			tableItems[1]->setText(cableRows[i]->thresholdsAuto[j].minValue != -1 ? QString::number(cableRows[i]->thresholdsAuto[j].minValue) : "-");
			tableItems[2]->setText(cableRows[i]->thresholdsAuto[j].maxValue != -1 ? QString::number(cableRows[i]->thresholdsAuto[j].maxValue) : "-");

			fillTableColor(cableRows[i], j, tableItems);

			indColumnMeasuredValues += MEASUREMENT_OFFSET_IN_ANALOG;
			indColumnThresholdsMin += MEASUREMENT_OFFSET_IN_ANALOG;
			indColumnThresholdsMax += MEASUREMENT_OFFSET_IN_ANALOG;
		}

		commentsTextEdits.push_back(new QTextEdit());
		commentsTextEdits[commentsTextEdits.size() - 1]->setStyleSheet(viewWindowState->appTheme ? darkStyles.reportwindowNameLineEdit :  lightStyles.reportwindowNameLineEdit);

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

void ReportWindow::fillTableInformation(std::vector<TestTableRowProperties*> cableRows)
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
		tableWidget->item(indCurrentRow, IND_COLUMN_BASE_PIN)->setText(cableRows[i]->component);
		tableWidget->item(indCurrentRow, IND_COLUMN_BASE_NAME)->setText(cableRows[i]->name);

		switch (viewWindowState->appLanguage)
		{
		case RUSSIAN_LANG:
			if (cableRows[i]->direction == "IN")
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_DIRECTION)->setText(QString("Вход"));
			else if (cableRows[i]->direction == "OUT")
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_DIRECTION)->setText(QString("Выход"));
			break;

		case ENGLISH_LANG:
			if (cableRows[i]->direction == "IN")
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_DIRECTION)->setText(QString("In"));
			else if (cableRows[i]->direction == "OUT")
				tableWidget->item(indCurrentRow, IND_COLUMN_BASE_DIRECTION)->setText(QString("Out"));
			break;
		}
		if(cableRows[i]->typeInt == TypeCable::CAN_OUT)
			tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString("CAN"));
		else
			tableWidget->item(indCurrentRow, IND_COLUMN_BASE_TYPE)->setText(QString("LIN"));

		int indColumnMeasuredValue1 = MEASUREMENT_COLUMN_POSITION;

		prepareItem(tableWidget, indCurrentRow, indColumnMeasuredValue1, SPAN_HORIZONTAL_DOUBLE);

		QTableWidgetItem* tableItems;
		tableItems = tableWidget->item(indCurrentRow, indColumnMeasuredValue1);

		fillTableColor(cableRows[i], 0, &tableItems);
		if (cableRows[i]->measureds.size() == 0)
			tableItems->setText("-");

		
		commentsTextEdits.push_back(new QTextEdit());
		commentsTextEdits[commentsTextEdits.size() - 1]->setStyleSheet(viewWindowState->appTheme ? darkStyles.reportwindowNameLineEdit : lightStyles.reportwindowNameLineEdit);

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
	case TypeCable::CAN_OUT:
	case TypeCable::LIN_OUT:
		fillTableInformation(cableRows);
		break;
	case TypeCable::SLEEP:
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
				if(typedCableRows[i][j]->typeInt != TypeCable::SLEEP)
					typedCableRows[i][j]->comment = commentsTextEdits[counter]->toPlainText();
				counter++;
			}
	}
}

void writeHorizontalAlignCell(Document& xlsx, int rowStart, int columnStart, int rowEnd, int columnEnd, const QVariant& text, Format formatText, const QColor& color = nullptr)
{

	if(color != nullptr)
		formatText.setPatternBackgroundColor(color);
	xlsx.write(rowStart, columnStart, text, formatText);
	xlsx.mergeCells(CellRange(rowStart, columnStart, rowEnd, columnEnd), formatText);
}

void genereateHeaderFile(Document& xlsx, QString testerName, QString serialNumber, TestBlockName testingBlock, QString actualVersion, QString& equipmentName)
{
	// 
	Format format;
	format.setLocked(true);
	format.setHorizontalAlignment(Format::AlignLeft);
	format.setBorderStyle(Format::BorderThin);
	Format dateFormat(format);
	dateFormat.setNumberFormatIndex(14);
	Format timeFormat(format);
	timeFormat.setNumberFormatIndex(21);
	Format commentHeader(format);
	commentHeader.setHorizontalAlignment(Format::AlignHCenter);
	commentHeader.setVerticalAlignment(Format::AlignTop);

	// 
	writeHorizontalAlignCell(xlsx, 1, 1, 1, 2, viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Дата проверки") : QString("Date of inspection"), format);
	writeHorizontalAlignCell(xlsx, 2, 1, 2, 2, viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Время проверки") : QString("Time of inspection"), format);
	writeHorizontalAlignCell(xlsx, 3, 1, 3, 2, viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Имя оператора") : QString("Operator name"), format);
	writeHorizontalAlignCell(xlsx, 4, 1, 4, 2, viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Название блока") : QString("Block name"), format);
	writeHorizontalAlignCell(xlsx, 5, 1, 5, 2, viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Название программы") : QString("App name"), format);
	writeHorizontalAlignCell(xlsx, 6, 1, 6, 2, viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Комплектация") : QString("Equipment"), format);
	writeHorizontalAlignCell(xlsx, 7, 1, 7, 2, viewWindowState->appLanguage == RUSSIAN_LANG ? QString("PART номер") : QString("Part number"), format);
	writeHorizontalAlignCell(xlsx, 8, 1, 8, 2, viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Серийный номер") : QString("Serial number"), format);
	writeHorizontalAlignCell(xlsx, 9, 1, 9, 2, viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Дата производства") : QString("Manufacture date"), format);
	writeHorizontalAlignCell(xlsx, 10, 1, 10, 2, viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Версия ПО") : QString("Software version"), format);
	writeHorizontalAlignCell(xlsx, 1, 7, 1, 8, viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Комментарий") : QString("Comment"), commentHeader);
	commentHeader.setHorizontalAlignment(Format::AlignLeft);
	writeHorizontalAlignCell(xlsx, 2, 7, HEIGHT_HEADERFILE - 1, 8, "", commentHeader);

	QDateTime time = QDateTime::currentDateTime();

	writeHorizontalAlignCell(xlsx, 1, 3, 1, 6, time.date(), dateFormat);
	writeHorizontalAlignCell(xlsx, 2, 3, 2, 6, time.time(), timeFormat);
	writeHorizontalAlignCell(xlsx, 3, 3, 3, 6, testerName, format);
	writeHorizontalAlignCell(xlsx, 4, 3, 4, 6, testingBlock == TestBlockName::SMXX ? QString("SMXX ") + actualVersion : (testingBlock == TestBlockName::DTM ? QString("DTM ") + actualVersion : QString("BCM ") + actualVersion), format);
	writeHorizontalAlignCell(xlsx, 5, 3, 5, 6, Can::getDiagBlock(DiagInformation::Calibration_NAME, testingBlock), format);
	equipmentName = Can::getDiagBlock(DiagInformation::Equipment_NAME, testingBlock);
	writeHorizontalAlignCell(xlsx, 6, 3, 6, 6, equipmentName, format);
	writeHorizontalAlignCell(xlsx, 7, 3, 7, 6, Can::getDiagBlock(DiagInformation::Part_NUMBER, testingBlock), format);
	writeHorizontalAlignCell(xlsx, 8, 3, 8, 6, serialNumber, format);
	writeHorizontalAlignCell(xlsx, 9, 3, 9, 6, Can::getDiagBlock(DiagInformation::Manufacture_DATE, testingBlock), format);
	writeHorizontalAlignCell(xlsx, 10, 3, 10, 6, QString(NAME_PROGRAM), format);



}

void genereateHeaderTable(Document& xlsx, int maxOffset, bool isAutoStand)
{
	Format format;
	format.setHorizontalAlignment(Format::AlignHCenter);
	format.setFontBold(true);
	format.setBorderStyle(Format::BorderThin);
	format.setPatternBackgroundColor(QColor(COLOR_LIGHT_BLUE));

	xlsx.write(HEIGHT_HEADERFILE + 1, 1, viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Колодка") : QString("Pad"), format);
	xlsx.write(HEIGHT_HEADERFILE + 1, 2, viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Пин") : QString("Pin"), format);
	xlsx.write(HEIGHT_HEADERFILE + 1, 3, viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Направление") : QString("Direction"), format);
	xlsx.write(HEIGHT_HEADERFILE + 1, 4, viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Тип") : QString("Type"), format);
	xlsx.write(HEIGHT_HEADERFILE + 1, 5, viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Название") : QString("Name"), format);


	xlsx.setColumnWidth(5, 100);


	if (isAutoStand)
	{

		xlsx.write(HEIGHT_HEADERFILE + 1, 6 + maxOffset, viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Комментарий") : QString("Comment"), format);

		xlsx.setColumnWidth(6 + maxOffset, 50);


		CellRange range(HEIGHT_HEADERFILE + 1, 6, HEIGHT_HEADERFILE + 1, 5 + maxOffset);
		xlsx.mergeCells(range, format);
	}
	else
	{
		xlsx.write(HEIGHT_HEADERFILE + 1, 8, viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Комментарий") : QString("Comment"), format);

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
			str = QString("Выход");
		else if(direction == "IN")
			str = QString("Вход");

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
			str = QString("Цифровой");
			break;

		case TypeCable::ANALOG_IN:
			str = QString("Аналоговый");
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

		case TypeCable::CAN_OUT:
			str = "CAN";
			break;

		case TypeCable::LIN_OUT:
			str = "LIN";
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
			break;

		case TypeCable::CAN_OUT:
			str = "CAN";
			break;

		case TypeCable::LIN_OUT:
			str = "LIN";
			break;
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

	
	if (equipmentName != (viewWindowState->appLanguage == RUSSIAN_LANG ? "Ошибка. Длительная задержка" : "Error. Long delay"))
	{
		dlgErase = new QDialog;
		uiErase.setupUi(dlgErase);
		WindowFrame w(WindowType::ERASEWINDOW, this, dlgErase);
		w.setWindowIcon(QIcon(QPixmap(appLogoPath)));
		dlgErase->setMaximumSize(380, 170);

		connect(uiErase.erasePushButton, &QPushButton::clicked, this, &ReportWindow::on_erasePushButton_clicked);
		connect(uiErase.cancelPushButton, &QPushButton::clicked, this, &ReportWindow::on_cancelPushButton_clicked);

		QString nameBlock = equipmentName;
		if (viewWindowState->appLanguage == RUSSIAN_LANG)
		{
			uiErase.erasePushButton->setText(QString("Стереть"));
			uiErase.cancelPushButton->setText(QString("Выйти"));
			uiErase.headerLabel->setText(QString("Произвести стирание программы\nблока ") + nameBlock.remove("_NAMI") + " ?");

		}
		else
		{
			uiErase.erasePushButton->setText(QString("Erasing"));
			uiErase.cancelPushButton->setText(QString("Exit"));
			uiErase.headerLabel->setText(QString("Erase the program\nblock ") + nameBlock.remove("_NAMI") + " ?");

		}
		if (viewWindowState->appTheme == LIGHT_THEME)
		{
			uiErase.erasePushButton->setStyleSheet(lightStyles.testwindowButtonStyle);
			uiErase.cancelPushButton->setStyleSheet(lightStyles.testwindowButtonStyle);
			uiErase.headerLabel->setStyleSheet(lightStyles.eraseWindowLable);

		}
		else
		{
			uiErase.erasePushButton->setStyleSheet(darkStyles.testwindowButtonStyle);
			uiErase.cancelPushButton->setStyleSheet(darkStyles.testwindowButtonStyle);
			uiErase.headerLabel->setStyleSheet(darkStyles.eraseWindowLable);

		}


		w.show();
		dlgErase->exec();
	}

}

void ReportWindow::on_erasePushButton_clicked()
{
	startErase = true;
	uiErase.progressBar->setValue(0);
	if (viewWindowState->appLanguage == RUSSIAN_LANG)
		uiErase.headerLabel->setText(QString("Стирание программы\nблока ") + equipmentName);
	else
		uiErase.headerLabel->setText(QString("Erasing the program\nblock ") + equipmentName);

	QString result = Can::eraseApp(equipmentName);

	if (result == QString("GOOD"))
	{
		uiErase.progressBar->setValue(100);
		if (viewWindowState->appLanguage == RUSSIAN_LANG)
			uiErase.headerLabel->setText(QString("Программа успешно удалена."));
		else
			uiErase.headerLabel->setText(QString("The program has been successfully removed."));
	}
	else
	{
		uiErase.headerLabel->setText(QString("Error. ") + result);

	}
	startErase = false;
}

void ReportWindow::on_cancelPushButton_clicked()
{
	if (!startErase)
		dlgErase->close();
	else
	{
		if (viewWindowState->appLanguage == RUSSIAN_LANG)
			QMessageBox::warning(this, QString("Внимание"), QString("Во время стирание памяти, невозможно выйти. Дождитесь окончание процесса."));
		else
			QMessageBox::warning(this, QString("Warning"), QString("During the erasing of memory, it is impossible to exit. Wait until the process is completed."));
	}

}

HRESULT AutoWrap(int autoType, VARIANT* pvResult, IDispatch* pDisp, LPOLESTR ptName, int cArgs...) {
	// Begin variable-argument list...
	va_list marker;
	va_start(marker, cArgs);

	if (!pDisp)
		return -1;


	// Variables used...
	DISPPARAMS dp = { NULL, NULL, 0, 0 };
	DISPID dispidNamed = DISPID_PROPERTYPUT;
	DISPID dispID;
	HRESULT hr;
	char szName[200];

	//LPOLESTR rgszNames[] = ;
	// Convert down to ANSI
	WideCharToMultiByte(CP_ACP, 0, ptName, -1, szName, 256, NULL, NULL);

	// Get DISPID for name passed...
	hr = pDisp->GetIDsOfNames(IID_NULL, &ptName, 1, LOCALE_USER_DEFAULT, &dispID);
	if (FAILED(hr))
		return hr;
	

	VARIANT* pArgs = new VARIANT[cArgs + 1];

	for (int i = 0; i < cArgs; i++) {
		pArgs[i] = va_arg(marker, VARIANT);
	}


	dp.cArgs = cArgs;
	dp.rgvarg = pArgs;

	if (autoType & DISPATCH_PROPERTYPUT) 
	{
		dp.cNamedArgs = 1;
		dp.rgdispidNamedArgs = &dispidNamed;
	}

	hr = pDisp->Invoke(dispID, IID_NULL, LOCALE_SYSTEM_DEFAULT, autoType, &dp, pvResult, NULL, NULL);
	if (FAILED(hr)) 
		return hr;


	va_end(marker);

	delete[] pArgs;

	return hr;
}

void ReportWindow::blockComXlsx(QString nameFile)	
{
	CoInitialize(NULL);

	// Get CLSID for our server...
	CLSID clsid;
	HRESULT hr = CLSIDFromProgID(L"Excel.Application", &clsid);

	IDispatch* pXlApp;
	hr = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, IID_IDispatch, (void**)&pXlApp);


	VARIANT x;
	x.vt = VT_I4;
	x.lVal = 0;
	if(FAILED(AutoWrap(DISPATCH_PROPERTYPUT, NULL, pXlApp, (LPOLESTR)L"Visible" , 1, x)))
		if (viewWindowState->appLanguage == RUSSIAN_LANG)
			QMessageBox::warning(this, QString("Критическая ошибка"), QString("Произошла критическая ошибка во время сохранения файла"));
		else
			QMessageBox::warning(this, QString("Critical error"), QString("A critical error occurred while saving the file."));


	IDispatch* pXlBooks;
	VariantInit(&x);
	if (FAILED(AutoWrap(DISPATCH_PROPERTYGET, &x, pXlApp, (LPOLESTR)L"Workbooks", 0)))
		if (viewWindowState->appLanguage == RUSSIAN_LANG)
			QMessageBox::warning(this, QString("Критическая ошибка"), QString("Произошла критическая ошибка во время сохранения файла"));
		else
			QMessageBox::warning(this, QString("Critical error"), QString("A critical error occurred while saving the file."));
	pXlBooks = x.pdispVal;

	VARIANT filePath;
	VariantInit(&filePath);
	filePath.vt = VT_BSTR;
	filePath.bstrVal = SysAllocString(reinterpret_cast<const OLECHAR*>(nameFile.utf16()));
	IDispatch* pXlBook;
	VariantInit(&x);
	if (FAILED(AutoWrap(DISPATCH_METHOD, &x, pXlBooks, (LPOLESTR)L"Open", 1, filePath)))
	if (viewWindowState->appLanguage == RUSSIAN_LANG)
		QMessageBox::warning(this, QString("Критическая ошибка"), QString("Произошла критическая ошибка во время сохранения файла"));
	else
		QMessageBox::warning(this, QString("Critical error"), QString("A critical error occurred while saving the file."));
	pXlBook = x.pdispVal;

	IDispatch* pXlSheets;
	VariantInit(&x);
	if (FAILED(AutoWrap(DISPATCH_PROPERTYGET, &x, pXlBook, (LPOLESTR)L"Worksheets", 0)))
		if (viewWindowState->appLanguage == RUSSIAN_LANG)
			QMessageBox::warning(this, QString("Критическая ошибка"), QString("Произошла критическая ошибка во время сохранения файла"));
		else
			QMessageBox::warning(this, QString("Critical error"), QString("A critical error occurred while saving the file."));
	pXlSheets = x.pdispVal;

	VARIANT sheetName;
	VariantInit(&sheetName);
	sheetName.vt = VT_BSTR;
	sheetName.bstrVal = SysAllocString(viewWindowState->appLanguage == RUSSIAN_LANG ? reinterpret_cast<const OLECHAR*>(QString("Отчёт").utf16()) : reinterpret_cast<const OLECHAR*>(QString("Report").utf16()));

	IDispatch* pXlSheet;
	VariantInit(&x);
	if (FAILED(AutoWrap(DISPATCH_PROPERTYGET, &x, pXlSheets, (LPOLESTR)L"Item", 1, sheetName)))
	if (viewWindowState->appLanguage == RUSSIAN_LANG)
		QMessageBox::warning(this, QString("Критическая ошибка"), QString("Произошла критическая ошибка во время сохранения файла"));
	else
		QMessageBox::warning(this, QString("Critical error"), QString("A critical error occurred while saving the file."));
	pXlSheet = x.pdispVal;

	SysFreeString(sheetName.bstrVal);


	VARIANT password;
	VariantInit(&password);
	password.vt = VT_BSTR;
	QString passwordFile = PASSWORD;
	password.bstrVal = SysAllocString(reinterpret_cast<const OLECHAR*>(passwordFile.utf16()));

	VARIANT contents;
	VariantInit(&contents);
	contents.vt = VT_BOOL;
	contents.boolVal = VARIANT_TRUE;

	VARIANT users;
	VariantInit(&users);
	users.vt = VT_BOOL;
	users.boolVal = VARIANT_TRUE;

	if (FAILED(AutoWrap(DISPATCH_METHOD, NULL, pXlSheet, (LPOLESTR)L"Protect", 1, password)))
		if (viewWindowState->appLanguage == RUSSIAN_LANG)
			QMessageBox::warning(this, QString("Критическая ошибка"), QString("Произошла критическая ошибка во время сохранения файла"));
		else
			QMessageBox::warning(this, QString("Critical error"), QString("A critical error occurred while saving the file."));


	VARIANT l;
	VariantInit(&l);
	AutoWrap(DISPATCH_METHOD, NULL, pXlBook, (LPOLESTR)L"Save", 0);

	l.vt = VT_BOOL;
	l.boolVal = VARIANT_TRUE;
	AutoWrap(DISPATCH_PROPERTYPUT, NULL, pXlBook, (LPOLESTR)L"Saved", 1, l);
	AutoWrap(DISPATCH_METHOD, NULL, pXlBook, (LPOLESTR)L"Close", 0);

	AutoWrap(DISPATCH_METHOD, NULL, pXlApp, (LPOLESTR)L"Quit", 0);

	pXlSheet->Release();
	pXlSheets->Release();
	pXlBook->Release();
	pXlBooks->Release();
	pXlApp->Release();

	SysFreeString(filePath.bstrVal);
	
	CoUninitialize();

	SysFreeString(password.bstrVal);
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
		xlsx.addSheet(viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Отчёт") : QString("Report"));

		xlsx.currentWorksheet()->setGridLinesVisible(false);
		xlsx.setColumnWidth(1, 5, 13);

		genereateHeaderFile(xlsx, testerName, serialNumber, testingBlock, viewWindowState->actualVersion, equipmentName);


		Format format;
		format.setHorizontalAlignment(Format::AlignHCenter);
		format.setVerticalAlignment(Format::AlignVCenter);
		format.setFontBold(true);
		format.setBorderStyle(Format::BorderThin);

		int numRow = START_ROW_TABLE;
		bool color = false;

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
				if (cableRows[i]->pin != "-1")
					xlsx.write(numRow, 2, cableRows[i]->pin, tmpRowFormat);
				else
					xlsx.write(numRow, 2, cableRows[i]->component, tmpRowFormat);
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
						if (i == 0)
						{
							for (int j = 0; j < typedCableRows[type][i]->thresholdsAuto.size(); j++)
							{
								writeHorizontalAlignCell(xlsx, numRow, 6 + (4 * j), numRow, 9 + (4 * j), viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Измерение ") + QString::number(j + 1) : QString("Measured ") + QString::number(j + 1), tmpHeaderFormat);

								tmpHeaderFormat.setTextWrap(true);
								writeHorizontalAlignCell(xlsx, numRow + 1, 6 + (4 * j), numRow + 2, 7 + (4 * j), viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Измереное значени") : QString("Measured values"), tmpHeaderFormat);
								writeHorizontalAlignCell(xlsx, numRow + 1, 8 + (4 * j), numRow + 2, 9 + (4 * j), viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Заданое значение") : QString("Set value "), tmpHeaderFormat);
								tmpHeaderFormat.setTextWrap(false);



							}
							CellRange* range = new CellRange(numRow, 6 + ((int)typedCableRows[type][i]->thresholdsAuto.size() * 4), numRow + (int)typedCableRows[type].size() + 2, 9 + maxOffset - 4);
							if (typedCableRows[type][i]->thresholdsAuto.size() * 6 != maxOffset)
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
						for (int j = 0; j < typedCableRows[type][i]->thresholdsAuto.size(); j++)
						{
							if (typedCableRows[type][i]->measureds.size() == 0)
							{
								writeHorizontalAlignCell(xlsx, numRow, 6 + (4 * j), numRow, 7 + (4 * j), "-", tmpRowFormat);
								writeHorizontalAlignCell(xlsx, numRow, 8 + (4 * j), numRow, 9 + (4 * j), (typedCableRows[type][i]->thresholdsAuto[j].minValue + typedCableRows[type][i]->thresholdsAuto[j].maxValue) / 2, tmpRowFormat);

							}
							else
							{
								Format tmpRedFormat(format);
								tmpRedFormat.setPatternBackgroundColor(COLOR_LIGHT_RED);
								Format tmpGreenFormat(format);
									tmpGreenFormat.setPatternBackgroundColor(COLOR_LIGHT_GREEN);

								if (typedCableRows[type][i]->measureds[j]->digValue >= typedCableRows[type][i]->thresholdsAuto[j].minValue && typedCableRows[type][i]->measureds[j]->digValue <= typedCableRows[type][i]->thresholdsAuto[j].maxValue)
								{
									writeHorizontalAlignCell(xlsx, numRow, 6 + (4 * j), numRow, 7 + (4 * j), typedCableRows[type][i]->measureds[j]->digValue, tmpGreenFormat);
						 			xlsx.write(numRow, 8 + (4 * j), typedCableRows[type][i]->thresholdsAuto[j].minValue, tmpRowFormat);
									xlsx.write(numRow, 9 + (4 * j), typedCableRows[type][i]->thresholdsAuto[j].maxValue, tmpRowFormat);
								}
								else if (typedCableRows[type][i]->measureds[j]->digValue < typedCableRows[type][i]->thresholdsAuto[j].minValue)
								{
									writeHorizontalAlignCell(xlsx, numRow, 6 + (4 * j), numRow, 7 + (4 * j), typedCableRows[type][i]->measureds[j]->digValue, tmpRedFormat);
									xlsx.write(numRow, 8 + (4 * j), typedCableRows[type][i]->thresholdsAuto[j].minValue, tmpRedFormat); // red
									xlsx.write(numRow, 9 + (4 * j), typedCableRows[type][i]->thresholdsAuto[j].maxValue, tmpRowFormat);
								}
								else if (typedCableRows[type][i]->measureds[j]->digValue > typedCableRows[type][i]->thresholdsAuto[j].maxValue)
								{
									writeHorizontalAlignCell(xlsx, numRow, 6 + (4 * j), numRow, 7 + (4 * j), typedCableRows[type][i]->measureds[j]->digValue, tmpRedFormat);
									xlsx.write(numRow, 8 + (4 * j), typedCableRows[type][i]->thresholdsAuto[j].minValue, tmpRowFormat);
									xlsx.write(numRow, 9 + (4 * j), typedCableRows[type][i]->thresholdsAuto[j].maxValue, tmpRedFormat); // red
								}

							}
						}
						numRow++;
						break;


					case TypeCable::ANALOG_IN:
						if (i == 0)
						{
							for (int j = 0; j < typedCableRows[type][i]->thresholdsAuto.size(); j++)
							{
								writeHorizontalAlignCell(xlsx, numRow, 6 + (4 * j), numRow, 9 + (4 * j), viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Измерение ") + QString::number(j + 1) : QString("Measured ") + QString::number(j + 1), tmpHeaderFormat);

								CellRange r(numRow + 1, 6 + (4 * j), numRow + 2, 7 + (4 * j));
								xlsx.write(r.firstRow(), r.firstColumn(), viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Измереное значени") : QString("Measured values"), tmpHeaderFormat);

								tmpHeaderFormat.setTextWrap(true);
								xlsx.mergeCells(r, tmpHeaderFormat);
								tmpHeaderFormat.setTextWrap(false);

								writeHorizontalAlignCell(xlsx, numRow + 1, 8 + (4 * j), numRow + 1, 9 + (4 * j), viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Пороги") : QString("Thresholds "), tmpHeaderFormat);
								xlsx.write(numRow + 2, 8 + (4 * j), viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Мин") : QString("Min"), tmpHeaderFormat);
								xlsx.write(numRow + 2, 9 + (4 * j), viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Макс") : QString("Max"), tmpHeaderFormat);

							}
							CellRange* range = new CellRange(numRow, 6 + ((int)typedCableRows[type][i]->thresholdsAuto.size() * 4), numRow + (int)typedCableRows[type].size() + 2, 9 + maxOffset - 4);
							if (typedCableRows[type][i]->thresholdsAuto.size() * 6 != maxOffset)
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

						for (int j = 0; j < typedCableRows[type][i]->thresholdsAuto.size(); j++)
						{
							if (typedCableRows[type][i]->measureds.size() == 0)
							{
								writeHorizontalAlignCell(xlsx, numRow, 6 + (4 * j), numRow, 7 + (4 * j), "-", tmpRowFormat);
								xlsx.write(numRow, 8 + (4 * j), typedCableRows[type][i]->thresholdsAuto[j].minValue, tmpRowFormat);
								xlsx.write(numRow, 9 + (4 * j), typedCableRows[type][i]->thresholdsAuto[j].maxValue, tmpRowFormat);
							}
							else
							{
								Format tmpRedFormat(format);
								tmpRedFormat.setPatternBackgroundColor(COLOR_LIGHT_RED);
								Format tmpGreenFormat(format);
								tmpGreenFormat.setPatternBackgroundColor(COLOR_LIGHT_GREEN);

								if (typedCableRows[type][i]->measureds[j]->digValue >= typedCableRows[type][i]->thresholdsAuto[j].minValue && typedCableRows[type][i]->measureds[j]->digValue <= typedCableRows[type][i]->thresholdsAuto[j].maxValue)
								{
									writeHorizontalAlignCell(xlsx, numRow, 6 + (4 * j), numRow, 7 + (4 * j), typedCableRows[type][i]->measureds[j]->digValue, tmpGreenFormat);
									xlsx.write(numRow, 8 + (4 * j), typedCableRows[type][i]->thresholdsAuto[j].minValue, tmpRowFormat);
									xlsx.write(numRow, 9 + (4 * j), typedCableRows[type][i]->thresholdsAuto[j].maxValue, tmpRowFormat);
								}
								else if (typedCableRows[type][i]->measureds[j]->digValue < typedCableRows[type][i]->thresholdsAuto[j].minValue)
								{
									writeHorizontalAlignCell(xlsx, numRow, 6 + (4 * j), numRow, 7 + (4 * j), typedCableRows[type][i]->measureds[j]->digValue, tmpRedFormat);
									xlsx.write(numRow, 8 + (4 * j), typedCableRows[type][i]->thresholdsAuto[j].minValue, tmpRedFormat); // red
									xlsx.write(numRow, 9 + (4 * j), typedCableRows[type][i]->thresholdsAuto[j].maxValue, tmpRowFormat);
								}
								else if (typedCableRows[type][i]->measureds[j]->digValue > typedCableRows[type][i]->thresholdsAuto[j].maxValue)
								{
									writeHorizontalAlignCell(xlsx, numRow, 6 + (4 * j), numRow, 7 + (4 * j), typedCableRows[type][i]->measureds[j]->digValue, tmpRedFormat);
									xlsx.write(numRow, 8 + (4 * j), typedCableRows[type][i]->thresholdsAuto[j].minValue, tmpRowFormat);
									xlsx.write(numRow, 9 + (4 * j), typedCableRows[type][i]->thresholdsAuto[j].maxValue, tmpRedFormat); // red
								}

							}
						}

						numRow++;
						break;

					case TypeCable::DIG_OUT:
					case TypeCable::PWM_OUT:
					case TypeCable::VNH_OUT:
					case TypeCable::HLD_OUT:
						if (i == 0)
						{
							for (int j = 0; j < typedCableRows[type][i]->thresholdsAuto.size(); j++)
							{
								CellRange r(numRow + 1, 6 + (9 * j), numRow + 2, 8 + (9 * j));
								writeHorizontalAlignCell(xlsx, numRow, 6 + (9 * j), numRow, 14 + (9 * j), viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Измерение ") + QString::number(j + 1) : QString("Measured ") + QString::number(j + 1), tmpHeaderFormat);

								tmpHeaderFormat.setTextWrap(true);
								writeHorizontalAlignCell(xlsx, numRow + 1, 6 + (9 * j), numRow + 2, 8 + (9 * j), viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Измереное значение") : QString("Measured values"), tmpHeaderFormat);
								tmpHeaderFormat.setTextWrap(false);

								writeHorizontalAlignCell(xlsx, numRow + 1, 9 + (9 * j), numRow + 1, 14 + (9 * j), viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Пороги") : QString("Thresholds"), tmpHeaderFormat);
								writeHorizontalAlignCell(xlsx, numRow + 2, 9 + (9 * j), numRow + 2, 10 + (9 * j), viewWindowState->appLanguage == RUSSIAN_LANG ? QString("U, В") : QString("U, V"), tmpHeaderFormat);
								writeHorizontalAlignCell(xlsx, numRow + 2, 11 + (9 * j), numRow + 2, 12 + (9 * j), QString("I, A"), tmpHeaderFormat);
								writeHorizontalAlignCell(xlsx, numRow + 2, 13 + (9 * j), numRow + 2, 14 + (9 * j), viewWindowState->appLanguage == RUSSIAN_LANG ? QString("АЦП") : QString("ADC"), tmpHeaderFormat);
								xlsx.write(numRow + 3, 6 + (9 * j), viewWindowState->appLanguage == RUSSIAN_LANG ? QString("U, В") : QString("U, V"), tmpHeaderFormat);
								xlsx.write(numRow + 3, 7 + (9 * j), QString("I, A"), tmpHeaderFormat);
								xlsx.write(numRow + 3, 8 + (9 * j), viewWindowState->appLanguage == RUSSIAN_LANG ? QString("АЦП") : QString("ADC"), tmpHeaderFormat);

								xlsx.write(numRow + 3, 9 + (9 * j), viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Мин") : QString("Min"), tmpHeaderFormat);
								xlsx.write(numRow + 3, 10 + (9 * j), viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Макс") : QString("Max"), tmpHeaderFormat);
								xlsx.write(numRow + 3, 11 + (9 * j), viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Мин") : QString("Min"), tmpHeaderFormat);
								xlsx.write(numRow + 3, 12 + (9 * j), viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Макс") : QString("Max"), tmpHeaderFormat);
								xlsx.write(numRow + 3, 13 + (9 * j), viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Мин") : QString("Min"), tmpHeaderFormat);
								xlsx.write(numRow + 3, 14 + (9 * j), viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Макс") : QString("Max"), tmpHeaderFormat);
							}

							CellRange* range = new CellRange(numRow, 6 + ((int)typedCableRows[type][i]->thresholdsAuto.size() * 9), numRow + (int)typedCableRows[type].size() + 3, 5 + maxOffset);
							if (typedCableRows[type][i]->thresholdsAuto.size() * 9 != maxOffset)
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

						for (int j = 0; j < typedCableRows[type][i]->thresholdsAuto.size(); j++)
						{
							if (typedCableRows[type][i]->measureds.size() == 0)
							{
								xlsx.write(numRow, 6 + (9 * j), QString("-"), tmpRowFormat);
								xlsx.write(numRow, 7 + (9 * j), QString("-"), tmpRowFormat);
								xlsx.write(numRow, 8 + (9 * j), QString("-"), tmpRowFormat);
								xlsx.write(numRow, 9 + (9 * j), typedCableRows[type][i]->thresholdsAuto[j].minVoltage, tmpRowFormat);
								xlsx.write(numRow, 10 + (9 * j), typedCableRows[type][i]->thresholdsAuto[j].maxVoltage, tmpRowFormat);
								xlsx.write(numRow, 11 + (9 * j), typedCableRows[type][i]->thresholdsAuto[j].minCurrent, tmpRowFormat);
								xlsx.write(numRow, 12 + (9 * j), typedCableRows[type][i]->thresholdsAuto[j].maxCurrent, tmpRowFormat);
								xlsx.write(numRow, 13 + (9 * j), typedCableRows[type][i]->thresholdsAuto[j].minValue, tmpRowFormat);
								xlsx.write(numRow, 14 + (9 * j), typedCableRows[type][i]->thresholdsAuto[j].maxValue, tmpRowFormat);
							}
							else
							{
								Format tmpRedFormat(format);
								tmpRedFormat.setPatternBackgroundColor(COLOR_RED);
								Format tmpGreenFormat(format);
								tmpGreenFormat.setPatternBackgroundColor(COLOR_GREEN);
								if (typedCableRows[type][i]->measureds[j]->voltage >= typedCableRows[type][i]->thresholdsAuto[j].minVoltage && typedCableRows[type][i]->measureds[j]->voltage <= typedCableRows[type][i]->thresholdsAuto[j].maxVoltage)
								{
									xlsx.write(numRow, 6 + (9 * j), typedCableRows[type][i]->measureds[j]->voltage, tmpGreenFormat);
									xlsx.write(numRow, 9 + (9 * j), typedCableRows[type][i]->thresholdsAuto[j].minVoltage, tmpRowFormat);
									xlsx.write(numRow, 10 + (9 * j), typedCableRows[type][i]->thresholdsAuto[j].maxVoltage, tmpRowFormat);
								}
								else if (typedCableRows[type][i]->measureds[j]->voltage < typedCableRows[type][i]->thresholdsAuto[j].minVoltage)
								{
									xlsx.write(numRow, 6 + (9 * j), typedCableRows[type][i]->measureds[j]->voltage, tmpRedFormat);
									xlsx.write(numRow, 9 + (9 * j), typedCableRows[type][i]->thresholdsAuto[j].minVoltage, tmpRedFormat); // red
									xlsx.write(numRow, 10 + (9 * j), typedCableRows[type][i]->thresholdsAuto[j].maxVoltage, tmpRowFormat);
								}
								else if (typedCableRows[type][i]->measureds[j]->voltage > typedCableRows[type][i]->thresholdsAuto[j].maxVoltage)
								{
									xlsx.write(numRow, 6 + (9 * j), typedCableRows[type][i]->measureds[j]->voltage, tmpRedFormat);
									xlsx.write(numRow, 9 + (9 * j), typedCableRows[type][i]->thresholdsAuto[j].minVoltage, tmpRowFormat);
									xlsx.write(numRow, 10 + (9 * j), typedCableRows[type][i]->thresholdsAuto[j].maxVoltage, tmpRedFormat); // red
								}

								if (typedCableRows[type][i]->measureds[j]->current >= typedCableRows[type][i]->thresholdsAuto[j].minCurrent && typedCableRows[type][i]->measureds[j]->current <= typedCableRows[type][i]->thresholdsAuto[j].maxCurrent)
								{
									xlsx.write(numRow, 7 + (9 * j), typedCableRows[type][i]->measureds[j]->current, tmpGreenFormat);
									xlsx.write(numRow, 11 + (9 * j), typedCableRows[type][i]->thresholdsAuto[j].minCurrent, tmpRowFormat);
									xlsx.write(numRow, 12 + (9 * j), typedCableRows[type][i]->thresholdsAuto[j].maxCurrent, tmpRowFormat);
								}
								else if (typedCableRows[type][i]->measureds[j]->current < typedCableRows[type][i]->thresholdsAuto[j].minCurrent)
								{
									xlsx.write(numRow, 7 + (9 * j), typedCableRows[type][i]->measureds[j]->current, tmpRedFormat);
									xlsx.write(numRow, 11 + (9 * j), typedCableRows[type][i]->thresholdsAuto[j].minCurrent, tmpRedFormat); // red
									xlsx.write(numRow, 12 + (9 * j), typedCableRows[type][i]->thresholdsAuto[j].maxCurrent, tmpRowFormat);
								}
								else if (typedCableRows[type][i]->measureds[j]->current > typedCableRows[type][i]->thresholdsAuto[j].maxCurrent)
								{
									xlsx.write(numRow, 7 + (9 * j), typedCableRows[type][i]->measureds[j]->current, tmpRedFormat);
									xlsx.write(numRow, 11 + (9 * j), typedCableRows[type][i]->thresholdsAuto[j].minCurrent, tmpRowFormat);
									xlsx.write(numRow, 12 + (9 * j), typedCableRows[type][i]->thresholdsAuto[j].maxCurrent, tmpRedFormat); // red
								}


								if (typedCableRows[type][i]->measureds[j]->digValue >= typedCableRows[type][i]->thresholdsAuto[j].minValue && typedCableRows[type][i]->measureds[j]->digValue <= typedCableRows[type][i]->thresholdsAuto[j].maxValue)
								{
									xlsx.write(numRow, 8 + (9 * j), typedCableRows[type][i]->measureds[j]->digValue, tmpGreenFormat);
									xlsx.write(numRow, 13 + (9 * j), typedCableRows[type][i]->thresholdsAuto[j].minValue, tmpRowFormat);
									xlsx.write(numRow, 14 + (9 * j), typedCableRows[type][i]->thresholdsAuto[j].maxValue, tmpRowFormat);
								}
								else if (typedCableRows[type][i]->measureds[j]->digValue < typedCableRows[type][i]->thresholdsAuto[j].minValue)
								{
									xlsx.write(numRow, 8 + (9 * j), typedCableRows[type][i]->measureds[j]->digValue, tmpRedFormat);
									xlsx.write(numRow, 13 + (9 * j), typedCableRows[type][i]->thresholdsAuto[j].minValue, tmpRedFormat); // red
									xlsx.write(numRow, 14 + (9 * j), typedCableRows[type][i]->thresholdsAuto[j].maxValue, tmpRowFormat);
								}
								else if (typedCableRows[type][i]->measureds[j]->digValue > typedCableRows[type][i]->thresholdsAuto[j].maxValue)
								{
									xlsx.write(numRow, 8 + (9 * j), typedCableRows[type][i]->measureds[j]->digValue, tmpRedFormat);
									xlsx.write(numRow, 13 + (9 * j), typedCableRows[type][i]->thresholdsAuto[j].minValue, tmpRowFormat);
									xlsx.write(numRow, 14 + (9 * j), typedCableRows[type][i]->thresholdsAuto[j].maxValue, tmpRedFormat); // red
								}
							}
						}
						numRow++;
						break;

					case TypeCable::LIN_OUT:
					case TypeCable::CAN_OUT:

						if (i == 0 && (TypeCable)type == TypeCable::CAN_OUT)
						{
							writeHorizontalAlignCell(xlsx, numRow, 6, numRow, 7, viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Статус") : QString("Status"), tmpHeaderFormat);


							CellRange* range = new CellRange(numRow, 8, numRow + (int)typedCableRows[type].size(), 5 + maxOffset);
							xlsx.mergeCells(*range, format);
							delete range;

							range = new CellRange(numRow, 1, numRow, 5);
							xlsx.mergeCells(*range);
							delete range;
							numRow++;
						}

						xlsx.write(numRow, 1, typedCableRows[type][i]->connectorStr + " XP" + QString::number((int)typedCableRows[type][i]->connectorInt), tmpRowFormat);
						xlsx.write(numRow, 2, typedCableRows[type][i]->component, tmpRowFormat);
						xlsx.write(numRow, 3, "", tmpRowFormat);
						xlsx.write(numRow, 4, getStrType(typedCableRows[type][i]->typeInt), tmpRowFormat);
						xlsx.write(numRow, 5, typedCableRows[type][i]->name, tmpRowFormat);
						xlsx.write(numRow, 6 + maxOffset, typedCableRows[type][i]->comment, tmpRowCommentFormat);


						if (typedCableRows[type][i]->measureds.size() == 0)
							writeHorizontalAlignCell(xlsx, numRow, 6, numRow, 7, "-", tmpRowFormat);
						else
						{
							Format tmpRedFormat(format);
							tmpRedFormat.setPatternBackgroundColor(COLOR_RED);
							Format tmpGreenFormat(format);
							tmpGreenFormat.setPatternBackgroundColor(COLOR_GREEN);
							if (typedCableRows[type][i]->measureds[0]->digValue == 1)
								writeHorizontalAlignCell(xlsx, numRow, 6, numRow, 7, "", tmpGreenFormat);
							else
								writeHorizontalAlignCell(xlsx, numRow, 6, numRow, 7, "", tmpRedFormat);

						}


						numRow++;
						break;
					case TypeCable::SLEEP:

						if (i == 0)
						{
							tmpHeaderFormat.setTextWrap(true);
							writeHorizontalAlignCell(xlsx, numRow, 6, numRow + 2, 7, viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Статус просыпания") : QString("Wake-up status"), tmpHeaderFormat);
							writeHorizontalAlignCell(xlsx, numRow, 8, numRow + 2, 10, viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Измереноее значение тока в режиме \"SLEEP\" I, A") : QString("Measured current value in \"SLEEP\" mode I, A"), tmpHeaderFormat);
							writeHorizontalAlignCell(xlsx, numRow, 11, numRow, 12, viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Пороги") : QString("Thresholds"), tmpHeaderFormat);
							writeHorizontalAlignCell(xlsx, numRow + 1, 11, numRow + 1, 12, QString("I, A"), tmpHeaderFormat);
							writeHorizontalAlignCell(xlsx, numRow + 2, 11, numRow + 2, 11, viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Мин") : QString("Min"), tmpHeaderFormat);
							writeHorizontalAlignCell(xlsx, numRow + 2, 12, numRow + 2, 12, viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Макс") : QString("Max"), tmpHeaderFormat);

							tmpHeaderFormat.setTextWrap(false);
						}
						numRow++;
						break;
					}
				}
			}
			numRow += 2;

			//format.setPatternBackgroundColor(QColor(COLOR_LIGHT_BLUE));
			//format.setTextWrap(true);

			//writeHorizontalAlignCell(xlsx, numRow - 1, 5, numRow - 1, 8, viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Проверка режима \"SLEEP\"") : QString("Checking the \"SLEEP\" mode"), format);

			//writeHorizontalAlignCell(xlsx, numRow, 6, numRow + 2, 7, viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Измереное значени") : QString("Measured values"), format);
			//writeHorizontalAlignCell(xlsx, numRow, 8, numRow, 9, viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Пороги") : QString("Thresholds"), format);
			//writeHorizontalAlignCell(xlsx, numRow + 1, 8, numRow + 1, 9, QString("I, mA"), format);
			//xlsx.write(numRow + 2, 8, viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Мин") : QString("Min"), format);
			//xlsx.write(numRow + 2, 9, viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Макс") : QString("Max"), format);

			//format.setPatternBackgroundColor(QColor(COLOR_DIRTY_LIGHT_GREY));
			//xlsx.write(numRow + 3, 5, viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Ток потребления в режиме \"RUN\"") : QString("Current consumption in \"RUN\""), format);
			//writeHorizontalAlignCell(xlsx, numRow + 3, 6, numRow + 3, 7, QString("-"), format);
			//xlsx.write(numRow + 3, 8, QString("170"), format);
			//xlsx.write(numRow + 3, 9, QString("230"), format);

			//format.setPatternBackgroundColor(QColor(COLOR_DIRTY_WHITE));
			//xlsx.write(numRow + 4, 5, viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Ток потребления в режиме \"SLEEP\"") : QString("Current consumption in \"SLEEP\""), format);
			//writeHorizontalAlignCell(xlsx, numRow + 4, 6, numRow + 4, 7, QString("-"), format);
			//xlsx.write(numRow + 4, 8, QString("0"), format);
			//xlsx.write(numRow + 4, 9, QString("5"), format);


			//format.setPatternBackgroundColor(QColor(COLOR_LIGHT_BLUE));
			//writeHorizontalAlignCell(xlsx, numRow + 6, 6, numRow + 6, 7, viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Статус") : QString("Status"), format);
			//
			//format.setPatternBackgroundColor(QColor(COLOR_DIRTY_LIGHT_GREY));
			//xlsx.write(numRow + 7, 5, viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Просыпание по BODY-CAN") : QString("Waking up on BODY-CAN"), format);
			//writeHorizontalAlignCell(xlsx, numRow + 7, 6, numRow + 7, 7, QString("-"), format);

			//format.setPatternBackgroundColor(QColor(COLOR_DIRTY_WHITE));
			//xlsx.write(numRow + 8, 5, viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Просыпание по входу") : QString("Waking up by entrance"), format);
			//writeHorizontalAlignCell(xlsx, numRow + 8, 6, numRow + 8, 7, QString("-"), format);
		}
		QDateTime time = QDateTime::currentDateTime();
		QString nameFile = "Reports/";
		if (testingBlock == TestBlockName::DTM)
			nameFile += "DTM_";
		else
			nameFile += "BCM_";
		while (serialNumber[0] == "0")
			serialNumber.remove(0,1);
		nameFile += serialNumber + "-";
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
		QString fullPath = QDir::currentPath() + "/" + nameFile;
		xlsx.saveAs(nameFile);

		blockComXlsx(fullPath);


		if(viewWindowState->appLanguage == RUSSIAN_LANG)
			QMessageBox::warning(this, QString("Внимание"), QString(" \"" + nameFile.remove("Reports/") + "\" файл сохранён в папку Reports\n" + fullPath));
		else
			QMessageBox::warning(this, QString("Warning"), QString(" \"" + nameFile.remove("Reports/") + "\" the file is saved in the Reports folder\n" + fullPath));
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
		case Warnings::ReportWindow::XLSX_SAVE_ERROR: // Warning 0x101
			QMessageBox::warning(this, QString("Внимание"), QString("Неизвестная ошибка при создании документа .xlsx\nWarning 0x101"));
			break;

		case Warnings::ReportWindow::EMPTY_INITIALS: // Warning 0x104
			QMessageBox::warning(this, QString("Внимание"), QString("Пустое поле \"ФИО\". Заполните его\nWarning 0x104"));
			break;
		case Warnings::ReportWindow::EMPTY_SERIAL: // Warning 0x105
			QMessageBox::warning(this, QString("Внимание"), QString("Пустое поле \"Серийный номер\". Заполните его\nWarning 0x105"));
			break;
		}
		break;

	case ENGLISH_LANG:
		switch (warning)
		{
		case Warnings::ReportWindow::XLSX_SAVE_ERROR: // Warning 0x101
			QMessageBox::warning(this, QString("Warning"), QString("Unknown error while creating .xlsx document\nWarning 0x101"));
			break;
		case Warnings::ReportWindow::EMPTY_INITIALS: // Warning 0x104
			QMessageBox::warning(this, QString("Warning"), QString("Empty field \"Initiale\". Fill it in\nWarning 0x104"));
			break;
		case Warnings::ReportWindow::EMPTY_SERIAL: // Warning 0x105
			QMessageBox::warning(this, QString("Warning"), QString("Empty field \"Serial number\". Fill it in\nWarning 0x105"));
			break;
		}
		break;
	}
}