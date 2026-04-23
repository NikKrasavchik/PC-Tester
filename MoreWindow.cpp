#include "MoreWindow.h"


#define BUTTON_SIZE						100, 40

#define MEASUREMENT_COLUMN_POSITION		5

#define IND_COLUMN_BASE_COMMENT			5


MoreWindow::MoreWindow(TestTableRowProperties* row)
{
	this->row = row;
	this->coutThresholds = NOT_SET;
	this->isAllInit = false;

	font = new QFont();
	font->setBold(true);
	font->setPointSizeF(10);

	initBaseUi();
	fillBaseTable();
	initConnections();
}

MoreWindow::~MoreWindow() {}

void MoreWindow::initBaseUi()
{
	mainWidget = new QWidget(this);
	mainWidget->setObjectName("mainLayoutWidget");
	mainWidget->setGeometry(PADDING_MAINWIDGET, 0, START_MOREWINDOW_WIDTH - (PADDING_MAINWIDGET * 2), START_MOREWINDOW_HEIGHT - PADDING_MAINWIDGET);

	mainVLayout = new QVBoxLayout(mainWidget);
	mainVLayout->setObjectName("mainVLayout");

	// Table
	mainTableWidget = new QTableWidget(mainWidget);
	mainTableWidget->setObjectName("mainTableWidget");
	mainTableWidget->setStyleSheet(lightStyles.testwindowTableWidget);
	mainTableWidget->setSelectionMode(QAbstractItemView::NoSelection);
	mainTableWidget->horizontalHeader()->hide();
	mainTableWidget->verticalHeader()->hide();
	mainVLayout->addWidget(mainTableWidget);

	// BottomLayout
	bottomHLayout = new QHBoxLayout(mainWidget);
	bottomHLayout->setObjectName("bottomHLayout");

	startTestButton = new QPushButton();
	startTestButton->setMinimumSize(BUTTON_SIZE);
	startTestButton->setObjectName("startTestButton");
	startTestButton->setStyleSheet(lightStyles.testwindowButtonStyle);
	bottomHLayout->addWidget(startTestButton);

	saveChangesButton = new QPushButton();
	saveChangesButton->setMinimumSize(BUTTON_SIZE);
	saveChangesButton->setObjectName("saveChangesButton");
	saveChangesButton->setStyleSheet(lightStyles.testwindowButtonStyle);
	saveChangesButton->hide();
	bottomHLayout->addWidget(saveChangesButton);

	bottomSpacer = new QSpacerItem(40, 0, QSizePolicy::Expanding);
	bottomHLayout->addItem(bottomSpacer);

	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		startTestButton->setText(QString	("Тест"));
		saveChangesButton->setText(QString("Сохранить"));

		break;
	case ENGLISH_LANG:
		startTestButton->setText("Test");
		saveChangesButton->setText("Save");
		break;
	}

	mainVLayout->addLayout(bottomHLayout);
}

void MoreWindow::initConnections()
{
	connect(startTestButton, &QPushButton::clicked, this, &MoreWindow::on_startTestButton_clicked);
}

void MoreWindow::fillBaseTable()
{
	mainTableWidget->setColumnCount(BASE_COLUMN_COUNT);
	mainTableWidget->setRowCount(BASE_ROW_COUNT);

	prepareItem(CELL_SIGN_BASE_CONNECTOR,	SPAN_NONE);
	prepareItem(CELL_SIGN_BASE_PIN,			SPAN_NONE);
	prepareItem(CELL_SIGN_BASE_DIRECTION,	SPAN_NONE);
	prepareItem(CELL_SIGN_BASE_TYPE,		SPAN_NONE);
	prepareItem(CELL_SIGN_BASE_NAME,		SPAN_NONE);
	prepareItem(CELL_SIGN_BASE_COMMENT,		SPAN_NONE);

	prepareItem(CELL_VALUE_BASE_CONNECTOR,	SPAN_NONE);
	prepareItem(CELL_VALUE_BASE_PIN,		SPAN_NONE);
	prepareItem(CELL_VALUE_BASE_DIRECTION,	SPAN_NONE);
	prepareItem(CELL_VALUE_BASE_TYPE,		SPAN_NONE);
	prepareItem(CELL_VALUE_BASE_NAME,		SPAN_NONE);
	prepareItem(CELL_VALUE_BASE_COMMENT,	SPAN_NONE);

	commentTextEdit = new QTextEdit(mainTableWidget);
	commentTextEdit->setText(row->comment);
	connect(commentTextEdit, &QTextEdit::textChanged, this, &MoreWindow::on_commentTextEdit_textChanged);

	QWidget* commentWidget = new QWidget();
	QHBoxLayout* commentHLayout = new QHBoxLayout(commentWidget);
	commentHLayout->addWidget(commentTextEdit);
	commentHLayout->setAlignment(Qt::AlignCenter);
	commentHLayout->setContentsMargins(0, 0, 0, 0);
	commentWidget->setLayout(commentHLayout);
	mainTableWidget->setCellWidget(CELL_VALUE_BASE_COMMENT, commentWidget);

	resetLanguage(OFFSET_NULL);

	mainTableWidget->setColumnWidth(IND_COLUMN_BASE_CONNECTOR, 65);
	mainTableWidget->setColumnWidth(IND_COLUMN_BASE_PIN, 65);
	mainTableWidget->setColumnWidth(IND_COLUMN_BASE_DIRECTION, 120);
	mainTableWidget->setColumnWidth(IND_COLUMN_BASE_TYPE, 75);
	mainTableWidget->setColumnWidth(IND_COLUMN_BASE_COMMENT, 150);

	mainTableWidget->setRowHeight(IND_ROW_BASE_SIGN, 40);
	mainTableWidget->setRowHeight(IND_ROW_BASE_VALUE, 40);

	mainTableWidget->horizontalHeader()->setSectionResizeMode(IND_COLUMN_BASE_CONNECTOR, QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(IND_COLUMN_BASE_PIN, QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(IND_COLUMN_BASE_DIRECTION, QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(IND_COLUMN_BASE_TYPE, QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(IND_COLUMN_BASE_NAME, QHeaderView::Stretch);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(IND_COLUMN_BASE_COMMENT, QHeaderView::Fixed);

	mainTableWidget->verticalHeader()->setSectionResizeMode(IND_ROW_BASE_SIGN, QHeaderView::Fixed);
	mainTableWidget->verticalHeader()->setSectionResizeMode(IND_ROW_BASE_VALUE, QHeaderView::Stretch);
}

void MoreWindow::prepareItem(int row, int column, int rowSpan, int columnSpan)
{
	QFont font;
	font.setBold(true);
	font.setPointSizeF(10);

	if (!(rowSpan == 1 && columnSpan == 1))
		mainTableWidget->setSpan(row, column, rowSpan, columnSpan);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(row, column), "");
	mainTableWidget->item(row, column)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(row, column)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(row, column)->setFont(font);
}

void MoreWindow::setValues()
{
	mainTableWidget->item(CELL_VALUE_BASE_CONNECTOR)->setText(row->connectorStr);
	mainTableWidget->item(CELL_VALUE_BASE_PIN)->setText(row->pin);
	mainTableWidget->item(CELL_VALUE_BASE_DIRECTION)->setText(row->direction);
	mainTableWidget->item(CELL_VALUE_BASE_NAME)->setText(row->name);
	switch (row->typeInt)
	{
	case TypeCable::EMPTY:
		break;
	case TypeCable::DIG_IN:
		mainTableWidget->item(CELL_VALUE_BASE_TYPE)->setText(QString("DIG"));
		break;
	case TypeCable::ANALOG_IN:
		mainTableWidget->item(CELL_VALUE_BASE_TYPE)->setText(QString("ANALOG"));
		break;
	case TypeCable::HALL_IN:
		mainTableWidget->item(CELL_VALUE_BASE_TYPE)->setText(QString("HALL"));
		break;
	case TypeCable::DIG_OUT:
		mainTableWidget->item(CELL_VALUE_BASE_TYPE)->setText(QString("DIG"));
		break;
	case TypeCable::PWM_OUT:
		mainTableWidget->item(CELL_VALUE_BASE_TYPE)->setText(QString("PWM"));
		break;
	case TypeCable::VNH_OUT:
		mainTableWidget->item(CELL_VALUE_BASE_TYPE)->setText(QString("VNH"));
		break;
	case TypeCable::HLD_OUT:
		mainTableWidget->item(CELL_VALUE_BASE_TYPE)->setText(QString("HLD"));
		break;
	case TypeCable::CAN_OUT:
		mainTableWidget->item(CELL_VALUE_BASE_TYPE)->setText(QString("CAN"));
		break;
	case TypeCable::LIN_OUT:
		mainTableWidget->item(CELL_VALUE_BASE_TYPE)->setText(QString("LIN"));
		break;
	default:
		break;
	}
}

void MoreWindow::resetLanguage(int offset)
{
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		mainTableWidget->item(CELL_SIGN_BASE_CONNECTOR)->setText(QString("Колодка"));
		mainTableWidget->item(CELL_SIGN_BASE_PIN)->setText(QString("Пин"));
		mainTableWidget->item(CELL_SIGN_BASE_DIRECTION)->setText(QString("Направление"));
		mainTableWidget->item(CELL_SIGN_BASE_TYPE)->setText(QString("Тип"));
		mainTableWidget->item(CELL_SIGN_BASE_NAME)->setText(QString("Название"));
		mainTableWidget->item(CELL_SIGN_FILLED_COMMENT(offset))->setText(QString("Комментарий"));
		break;

	case ENGLISH_LANG:
		mainTableWidget->item(CELL_SIGN_BASE_CONNECTOR)->setText("Pad");
		mainTableWidget->item(CELL_SIGN_BASE_PIN)->setText("Pin");
		mainTableWidget->item(CELL_SIGN_BASE_DIRECTION)->setText("Direcrion");
		mainTableWidget->item(CELL_SIGN_BASE_TYPE)->setText("Type");
		mainTableWidget->item(CELL_SIGN_BASE_NAME)->setText("Name");
		mainTableWidget->item(CELL_SIGN_FILLED_COMMENT(offset))->setText("Comments");
		break;
	}
}

MoreWindowOut_AutoStend::MoreWindowOut_AutoStend(TestTableRowProperties* row) : MoreWindow(row)
{
	//int measuredWidth = ((int)row->thresholds.size() <= 3 ? (int)row->thresholds.size() : 3);
	int measuredWidth = (int)row->thresholdsAuto.size();
	this->resize(EMPTY_WINDOW_WIDTH + (measuredWidth * (WINDOW_MEASURED_WIDTH + 250)), WINDOW_OUT_HEIGHT);
	this->setMinimumSize(EMPTY_WINDOW_WIDTH + (measuredWidth * (WINDOW_MEASURED_WIDTH + 250)), WINDOW_OUT_HEIGHT);

	this->row = row;

	MoreWindow::setValues();
	generateSigns();
	setValues();

	connect(mainTableWidget, &QTableWidget::cellChanged, this, &MoreWindow::on_mainTableWidget_cellChanged);
	isAllInit = true;
}

void MoreWindowOut_AutoStend::generateSigns()
{
	const int rowAddedPosition = 1;

	for (int i = 0; i < ROW_OFFSET_TRIPPLE; i++)
		mainTableWidget->insertRow(rowAddedPosition);

	prepareItem(CELL_SIGN_BASE_CONNECTOR, SPAN_VERTICAL_QUADRUPLE);
	prepareItem(CELL_SIGN_BASE_PIN, SPAN_VERTICAL_QUADRUPLE);
	prepareItem(CELL_SIGN_BASE_DIRECTION, SPAN_VERTICAL_QUADRUPLE);
	prepareItem(CELL_SIGN_BASE_TYPE, SPAN_VERTICAL_QUADRUPLE);
	prepareItem(CELL_SIGN_BASE_NAME, SPAN_VERTICAL_QUADRUPLE);
	prepareItem(CELL_SIGN_BASE_COMMENT, SPAN_VERTICAL_QUADRUPLE);

	resetLanguage(OFFSET_NULL);
	
	for (int i = 0; i < row->thresholdsAuto.size(); i++)
	{
		for (int j = 0; j < MEASURED_OFFSET_NINETUPLE; j++)
		{
			mainTableWidget->insertColumn(MEASUREMENT_COLUMN_POSITION);
			mainTableWidget->setColumnWidth(MEASUREMENT_COLUMN_POSITION, 60);
		}

		prepareItem(CELL_OUT_SIGN_MEASURED, SPAN_HORIZONTAL_NINETUPLE);
		prepareItem(CELL_OUT_SIGN_MEASURED_VALUES, SPAN_SQUAD_DOUBLE);
		prepareItem(CELL_OUT_SIGN_MEASURED_CURRENT, SPAN_NONE);
		prepareItem(CELL_OUT_SIGN_MEASURED_VOLTAGE, SPAN_NONE);
		prepareItem(CELL_OUT_SIGN_MEASURED_ADC, SPAN_NONE);
		prepareItem(CELL_OUT_SIGN_THRESHOLDS, SPAN_HORIZONTAL_SEXTUPLE);
		prepareItem(CELL_OUT_SIGN_THRESHOLDS_CURRENT, SPAN_HORIZONTAL_DOUBLE);
		prepareItem(CELL_OUT_SIGN_THRESHOLDS_VOLTAGE, SPAN_HORIZONTAL_DOUBLE);
		prepareItem(CELL_OUT_SIGN_THRESHOLDS_ADC, SPAN_HORIZONTAL_DOUBLE);
		prepareItem(CELL_OUT_SIGN_MIN_CURRENT, SPAN_NONE);
		prepareItem(CELL_OUT_SIGN_MAX_CURRENT, SPAN_NONE);
		prepareItem(CELL_OUT_SING_MIN_VOLTAGE, SPAN_NONE);
		prepareItem(CELL_OUT_SING_MAX_VOLTAGE, SPAN_NONE);
		prepareItem(CELL_OUT_SING_MIN_ADC, SPAN_NONE);
		prepareItem(CELL_OUT_SING_MAX_ADC, SPAN_NONE);
		prepareItem(CELL_OUT_VALUES_MEASURED_CURRENT, SPAN_NONE);
		prepareItem(CELL_OUT_VALUES_MEASURED_VOLTAGE, SPAN_NONE);
		prepareItem(CELL_OUT_VALUES_MEASURED_ADC, SPAN_NONE);
		prepareItem(CELL_OUT_VALUES_MIN_CURRENT, SPAN_NONE);
		prepareItem(CELL_OUT_VALUES_MAX_CURRENT, SPAN_NONE);
		prepareItem(CELL_OUT_VALUES_MIN_VOLTAGE, SPAN_NONE);
		prepareItem(CELL_OUT_VALUES_MAX_VOLTAGE, SPAN_NONE);
		prepareItem(CELL_OUT_VALUES_MIN_ADC, SPAN_NONE);
		prepareItem(CELL_OUT_VALUES_MAX_ADC, SPAN_NONE);

		resetBlockLanguage((int)row->thresholdsAuto.size() - i);
	}
}

void MoreWindowOut_AutoStend::resetBlockLanguage(int measuredNum)
{
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		mainTableWidget->item(CELL_OUT_SIGN_MEASURED)->setText(QString("Измерение ") + QString::number(measuredNum));
		mainTableWidget->item(CELL_OUT_SIGN_MEASURED_VALUES)->setText(QString("Измеренное значение"));
		mainTableWidget->item(CELL_OUT_SIGN_MEASURED_VOLTAGE)->setText(QString("U, В"));
		mainTableWidget->item(CELL_OUT_SIGN_MEASURED_CURRENT)->setText(QString("I, A"));
		mainTableWidget->item(CELL_OUT_SIGN_MEASURED_ADC)->setText(QString("АЦП"));
		mainTableWidget->item(CELL_OUT_SIGN_THRESHOLDS)->setText(QString("Пороги"));
		mainTableWidget->item(CELL_OUT_SIGN_THRESHOLDS_CURRENT)->setText(QString("U, A"));
		mainTableWidget->item(CELL_OUT_SIGN_THRESHOLDS_VOLTAGE)->setText(QString("I, A"));
		mainTableWidget->item(CELL_OUT_SIGN_THRESHOLDS_ADC)->setText(QString("АЦП"));
		mainTableWidget->item(CELL_OUT_SIGN_MIN_CURRENT)->setText(QString("Мин"));
		mainTableWidget->item(CELL_OUT_SIGN_MAX_CURRENT)->setText(QString("Макс"));
		mainTableWidget->item(CELL_OUT_SING_MIN_VOLTAGE)->setText(QString("Мин"));
		mainTableWidget->item(CELL_OUT_SING_MAX_VOLTAGE)->setText(QString("Макс"));
		mainTableWidget->item(CELL_OUT_SING_MIN_ADC)->setText(QString("Мин"));
		mainTableWidget->item(CELL_OUT_SING_MAX_ADC)->setText(QString("Макс"));
		break;

	case ENGLISH_LANG:
		mainTableWidget->item(CELL_OUT_SIGN_MEASURED)->setText(QString("Measure ") + QString::number(measuredNum));
		mainTableWidget->item(CELL_OUT_SIGN_MEASURED_VALUES)->setText(QString("Measure values"));
		mainTableWidget->item(CELL_OUT_SIGN_MEASURED_VOLTAGE)->setText(QString("U, V"));
		mainTableWidget->item(CELL_OUT_SIGN_MEASURED_CURRENT)->setText(QString("I, A"));
		mainTableWidget->item(CELL_OUT_SIGN_MEASURED_ADC)->setText(QString("ADC"));
		mainTableWidget->item(CELL_OUT_SIGN_THRESHOLDS)->setText(QString("Thresholds"));
		mainTableWidget->item(CELL_OUT_SIGN_THRESHOLDS_CURRENT)->setText(QString("U, V"));
		mainTableWidget->item(CELL_OUT_SIGN_THRESHOLDS_VOLTAGE)->setText(QString("I, A"));
		mainTableWidget->item(CELL_OUT_SIGN_THRESHOLDS_ADC)->setText(QString("ADC"));
		mainTableWidget->item(CELL_OUT_SIGN_MIN_CURRENT)->setText(QString("Min"));
		mainTableWidget->item(CELL_OUT_SIGN_MAX_CURRENT)->setText(QString("Max"));
		mainTableWidget->item(CELL_OUT_SING_MIN_VOLTAGE)->setText(QString("Min"));
		mainTableWidget->item(CELL_OUT_SING_MAX_VOLTAGE)->setText(QString("Max"));
		mainTableWidget->item(CELL_OUT_SING_MIN_ADC)->setText(QString("Min"));
		mainTableWidget->item(CELL_OUT_SING_MAX_ADC)->setText(QString("Max"));
		break;
	}
}

void MoreWindowOut_AutoStend::setValues()
{
	mainTableWidget->blockSignals(true);
	for (int i = 0; i < row->thresholdsAuto.size(); i++)
	{
		for(int j = BASE_COLUMN_COUNT + (i * MEASURED_OFFSET_NINETUPLE); j < (BASE_COLUMN_COUNT + (i * MEASURED_OFFSET_NINETUPLE)) + MEASURED_OFFSET_NINETUPLE; j++)
			mainTableWidget->item(IND_OUT_ROW_VALUES, j)->setBackground(QBrush(Qt::transparent));

		if (row->measureds.size() != 0)
		{
			mainTableWidget->item(CELL_OUT_VALUES_MEASURED_CURRENT + (i * MEASURED_OFFSET_NINETUPLE))->setText(QString::number(row->measureds[i]->current, 'f', 3));
			mainTableWidget->item(CELL_OUT_VALUES_MEASURED_VOLTAGE + (i * MEASURED_OFFSET_NINETUPLE))->setText(QString::number(row->measureds[i]->voltage, 'f', 3));
			mainTableWidget->item(CELL_OUT_VALUES_MEASURED_ADC + (i * MEASURED_OFFSET_NINETUPLE))->setText(QString::number(row->measureds[i]->digValue));
		}
		else
		{
			mainTableWidget->item(CELL_OUT_VALUES_MEASURED_CURRENT + (i * MEASURED_OFFSET_NINETUPLE))->setText("-");
			mainTableWidget->item(CELL_OUT_VALUES_MEASURED_VOLTAGE + (i * MEASURED_OFFSET_NINETUPLE))->setText("-");
			mainTableWidget->item(CELL_OUT_VALUES_MEASURED_ADC + (i * MEASURED_OFFSET_NINETUPLE))->setText("-");
		}

		mainTableWidget->item(CELL_OUT_VALUES_MIN_VOLTAGE + (i * MEASURED_OFFSET_NINETUPLE))->setText(row->thresholdsAuto[i].minVoltage != -1 ? QString::number(row->thresholdsAuto[i].minVoltage) : "-");
		mainTableWidget->item(CELL_OUT_VALUES_MAX_VOLTAGE + (i * MEASURED_OFFSET_NINETUPLE))->setText(row->thresholdsAuto[i].maxVoltage != -1 ? QString::number(row->thresholdsAuto[i].maxVoltage) : "-");
		mainTableWidget->item(CELL_OUT_VALUES_MIN_CURRENT + (i * MEASURED_OFFSET_NINETUPLE))->setText(row->thresholdsAuto[i].minCurrent != -1 ? QString::number(row->thresholdsAuto[i].minCurrent) : "-");
		mainTableWidget->item(CELL_OUT_VALUES_MAX_CURRENT + (i * MEASURED_OFFSET_NINETUPLE))->setText(row->thresholdsAuto[i].maxCurrent != -1 ? QString::number(row->thresholdsAuto[i].maxCurrent) : "-");
		mainTableWidget->item(CELL_OUT_VALUES_MIN_ADC + (i * MEASURED_OFFSET_NINETUPLE))->setText(row->thresholdsAuto[i].minValue != -1 ? QString::number(row->thresholdsAuto[i].minValue) : "-");
		mainTableWidget->item(CELL_OUT_VALUES_MAX_ADC + (i * MEASURED_OFFSET_NINETUPLE))->setText(row->thresholdsAuto[i].maxValue != -1 ? QString::number(row->thresholdsAuto[i].maxValue) : "-");

		mainTableWidget->item(CELL_OUT_VALUES_MIN_CURRENT + (i * MEASURED_OFFSET_NINETUPLE))->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
		mainTableWidget->item(CELL_OUT_VALUES_MAX_CURRENT + (i * MEASURED_OFFSET_NINETUPLE))->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
		mainTableWidget->item(CELL_OUT_VALUES_MIN_VOLTAGE + (i * MEASURED_OFFSET_NINETUPLE))->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
		mainTableWidget->item(CELL_OUT_VALUES_MAX_VOLTAGE + (i * MEASURED_OFFSET_NINETUPLE))->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
		mainTableWidget->item(CELL_OUT_VALUES_MIN_ADC + (i * MEASURED_OFFSET_NINETUPLE))->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
		mainTableWidget->item(CELL_OUT_VALUES_MAX_ADC + (i * MEASURED_OFFSET_NINETUPLE))->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
		
		if (row->measureds.size() == 0)
			continue;
		if (row->measureds[i]->current != -1)
			if (row->measureds[i]->current < row->thresholdsAuto[i].minCurrent)
			{
				mainTableWidget->item(CELL_OUT_VALUES_MEASURED_CURRENT + (i * MEASURED_OFFSET_NINETUPLE))->setBackground(QBrush(COLOR_RED));
				mainTableWidget->item(CELL_OUT_VALUES_MIN_CURRENT + (i * MEASURED_OFFSET_NINETUPLE))->setBackground(QBrush(COLOR_RED));
			}
			else if (row->measureds[i]->current > row->thresholdsAuto[i].maxCurrent)
			{
				mainTableWidget->item(CELL_OUT_VALUES_MEASURED_CURRENT + (i * MEASURED_OFFSET_NINETUPLE))->setBackground(QBrush(COLOR_RED));
				mainTableWidget->item(CELL_OUT_VALUES_MAX_CURRENT + (i * MEASURED_OFFSET_NINETUPLE))->setBackground(QBrush(COLOR_RED));
			}
			else
				mainTableWidget->item(CELL_OUT_VALUES_MEASURED_CURRENT + (i * MEASURED_OFFSET_NINETUPLE))->setBackground(QBrush(COLOR_GREEN));

		if (row->measureds[i]->voltage != -1)
			if (row->measureds[i]->voltage < row->thresholdsAuto[i].minVoltage)
			{
				mainTableWidget->item(CELL_OUT_VALUES_MEASURED_VOLTAGE + (i * MEASURED_OFFSET_NINETUPLE))->setBackground(QBrush(COLOR_RED));
				mainTableWidget->item(CELL_OUT_VALUES_MIN_VOLTAGE + (i * MEASURED_OFFSET_NINETUPLE))->setBackground(QBrush(COLOR_RED));
			}
			else if (row->measureds[i]->voltage > row->thresholdsAuto[i].maxVoltage)
			{
				mainTableWidget->item(CELL_OUT_VALUES_MEASURED_VOLTAGE + (i * MEASURED_OFFSET_NINETUPLE))->setBackground(QBrush(COLOR_RED));
				mainTableWidget->item(CELL_OUT_VALUES_MAX_VOLTAGE + (i * MEASURED_OFFSET_NINETUPLE))->setBackground(QBrush(COLOR_RED));
			}
			else
				mainTableWidget->item(CELL_OUT_VALUES_MEASURED_VOLTAGE + (i * MEASURED_OFFSET_NINETUPLE))->setBackground(QBrush(COLOR_GREEN));

		if (row->measureds[i]->digValue != -1)
			if (row->measureds[i]->digValue < row->thresholdsAuto[i].minValue)
			{
				mainTableWidget->item(CELL_OUT_VALUES_MEASURED_ADC + (i * MEASURED_OFFSET_NINETUPLE))->setBackground(QBrush(COLOR_RED));
				mainTableWidget->item(CELL_OUT_VALUES_MIN_ADC + (i * MEASURED_OFFSET_NINETUPLE))->setBackground(QBrush(COLOR_RED));
			}
			else if (row->measureds[i]->digValue > row->thresholdsAuto[i].maxValue)
			{
				mainTableWidget->item(CELL_OUT_VALUES_MEASURED_ADC + (i * MEASURED_OFFSET_NINETUPLE))->setBackground(QBrush(COLOR_RED));
				mainTableWidget->item(CELL_OUT_VALUES_MAX_ADC + (i * MEASURED_OFFSET_NINETUPLE))->setBackground(QBrush(COLOR_RED));
			}
			else
				mainTableWidget->item(CELL_OUT_VALUES_MEASURED_ADC + (i * MEASURED_OFFSET_NINETUPLE))->setBackground(QBrush(COLOR_GREEN));
	}
	mainTableWidget->blockSignals(false);

}

MoreWindowIn_AutoStend::MoreWindowIn_AutoStend(TestTableRowProperties* row) : MoreWindow(row)
{
	this->resize(EMPTY_WINDOW_WIDTH + WINDOW_MEASURED_WIDTH, WINDOW_IN_HEIGHT);
	this->setMinimumSize(EMPTY_WINDOW_WIDTH + WINDOW_MEASURED_WIDTH, WINDOW_IN_HEIGHT);

	this->row = row;

	MoreWindow::setValues();
	generateSigns();
	setValues();

	connect(mainTableWidget, &QTableWidget::cellChanged, this, &MoreWindow::on_mainTableWidget_cellChanged);
	isAllInit = true;
}

void MoreWindowIn_AutoStend::generateSigns()
{
	resetLanguage(OFFSET_NULL);

	for (int i = 0; i < row->thresholdsAuto.size(); i++)
	{
		for (int j = 0; j < MEASURED_OFFSET_DOUBLE; j++)
		{
			mainTableWidget->insertColumn(MEASUREMENT_COLUMN_POSITION);
			mainTableWidget->setColumnWidth(MEASUREMENT_COLUMN_POSITION, 90);
		}

		prepareItem(CELL_IN_SIGN_MEASURED_1, SPAN_NONE);
		prepareItem(CELL_IN_SIGN_MEASURED_2, SPAN_NONE);
		prepareItem(CELL_IN_VALUE_MEASURED_1, SPAN_NONE);
		prepareItem(CELL_IN_VALUE_MEASURED_2, SPAN_NONE);
		
		resetBlockLanguage((int)row->thresholdsAuto.size() - i);
	}
}

void MoreWindowIn_AutoStend::setValues()
{
	if (row->measureds.size() == 0)
		return;

	if (row->measureds[0]->voltage == 1)
		mainTableWidget->item(CELL_IN_VALUE_MEASURED_1)->setBackground(QBrush(QColor(COLOR_GREEN)));
	else if (row->measureds[0]->voltage == 0)
		mainTableWidget->item(CELL_IN_VALUE_MEASURED_1)->setBackground(QBrush(QColor(COLOR_RED)));
	else
		mainTableWidget->item(CELL_IN_VALUE_MEASURED_1)->setText("-");

	if (row->measureds[0]->current == 1)
		mainTableWidget->item(CELL_IN_VALUE_MEASURED_2)->setBackground(QBrush(QColor(COLOR_GREEN)));
	else if (row->measureds[0]->current == 0)
		mainTableWidget->item(CELL_IN_VALUE_MEASURED_2)->setBackground(QBrush(QColor(COLOR_RED)));
	else
		mainTableWidget->item(CELL_IN_VALUE_MEASURED_2)->setText("-");
}

void MoreWindowIn_AutoStend::resetBlockLanguage(int measuredNum)
{
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		mainTableWidget->item(CELL_IN_SIGN_MEASURED_1)->setText(QString("Значенеи 1"));
		mainTableWidget->item(CELL_IN_SIGN_MEASURED_2)->setText(QString("Значение 2"));
		break;

	case ENGLISH_LANG:
		mainTableWidget->item(CELL_IN_SIGN_MEASURED_1)->setText(QString("Value 1"));
		mainTableWidget->item(CELL_IN_SIGN_MEASURED_2)->setText(QString("Value 2"));
		break;
	}
}

MoreWindowInAnalog_AutoStend::MoreWindowInAnalog_AutoStend(TestTableRowProperties* row) : MoreWindow(row)
{
	int measuredWidth = ((int)row->thresholdsAuto.size() <= 3 ? (int)row->thresholdsAuto.size() : 3);
	this->resize(EMPTY_WINDOW_WIDTH + (measuredWidth * WINDOW_MEASURED_WIDTH), WINDOW_OUT_HEIGHT);
	this->setMinimumSize(EMPTY_WINDOW_WIDTH + (measuredWidth * WINDOW_MEASURED_WIDTH), WINDOW_OUT_HEIGHT);

	this->row = row;

	MoreWindow::setValues();
	generateSigns();
	setValues();

	isAllInit = true;
	connect(mainTableWidget, &QTableWidget::cellChanged, this, &MoreWindow::on_mainTableWidget_cellChanged);
}

void MoreWindowInAnalog_AutoStend::generateSigns()
{
	const int rowAddedPosition = 1;

	for (int i = 0; i < ROW_OFFSET_DOUBLE; i++)
		mainTableWidget->insertRow(rowAddedPosition);

	prepareItem(CELL_SIGN_BASE_CONNECTOR, SPAN_VERTICAL_TRIPPLE);
	prepareItem(CELL_SIGN_BASE_PIN, SPAN_VERTICAL_TRIPPLE);
	prepareItem(CELL_SIGN_BASE_DIRECTION, SPAN_VERTICAL_TRIPPLE);
	prepareItem(CELL_SIGN_BASE_TYPE, SPAN_VERTICAL_TRIPPLE);
	prepareItem(CELL_SIGN_BASE_NAME, SPAN_VERTICAL_TRIPPLE);
	prepareItem(CELL_SIGN_BASE_COMMENT, SPAN_VERTICAL_TRIPPLE);

	resetLanguage(OFFSET_NULL);

	for (int i = 0; i < row->thresholdsAuto.size(); i++)
	{
		for (int j = 0; j < MEASURED_OFFSET_TRIPPLE; j++)
		{
			mainTableWidget->insertColumn(MEASUREMENT_COLUMN_POSITION);
			mainTableWidget->setColumnWidth(MEASUREMENT_COLUMN_POSITION, 50);
		}
		mainTableWidget->setColumnWidth(MEASUREMENT_COLUMN_POSITION, 100);

		prepareItem(CELL_SIGN_IN_ANALOG_MEASURED, SPAN_HORIZONTAL_TRIPPLE);
		prepareItem(CELL_SIGN_IN_ANALOG_MEASURED_VALUES, SPAN_VERTICAL_DOUBLE);
		prepareItem(CELL_SIGN_IN_ANALOG_THRESHOLDS, SPAN_HORIZONTAL_DOUBLE);
		prepareItem(CELL_SIGN_IN_ANALOG_THRESHOLDS_MIN, SPAN_NONE);
		prepareItem(CELL_SIGN_IN_ANALOG_THRESHOLDS_MAX, SPAN_NONE);
		prepareItem(CELL_VALUE_IN_ANALOG_MEASURED_VALUES, SPAN_NONE);
		prepareItem(CELL_VALUE_IN_ANALOG_THRESHOLDS_MIN, SPAN_NONE);
		prepareItem(CELL_VALUE_IN_ANALOG_THRESHOLDS_MAX, SPAN_NONE);

		resetBlockLanguage((int)row->thresholdsAuto.size() - i);
	}
}

void MoreWindowInAnalog_AutoStend::resetBlockLanguage(int measuredNum)
{
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		mainTableWidget->item(CELL_SIGN_IN_ANALOG_MEASURED)->setText(QString("Измерение ") + QString::number(measuredNum));
		mainTableWidget->item(CELL_SIGN_IN_ANALOG_MEASURED_VALUES)->setText(QString("Измеренное значение"));
		mainTableWidget->item(CELL_SIGN_IN_ANALOG_THRESHOLDS)->setText(QString("Пороги"));
		mainTableWidget->item(CELL_SIGN_IN_ANALOG_THRESHOLDS_MIN)->setText(QString("Мин"));
		mainTableWidget->item(CELL_SIGN_IN_ANALOG_THRESHOLDS_MAX)->setText(QString("Макс"));
		break;

	case ENGLISH_LANG:
		mainTableWidget->item(CELL_SIGN_IN_ANALOG_MEASURED)->setText(QString("Measure ") + QString::number(measuredNum));
		mainTableWidget->item(CELL_SIGN_IN_ANALOG_MEASURED_VALUES)->setText(QString("Measure values"));
		mainTableWidget->item(CELL_SIGN_IN_ANALOG_THRESHOLDS)->setText(QString("Thresholds"));
		mainTableWidget->item(CELL_SIGN_IN_ANALOG_THRESHOLDS_MIN)->setText(QString("Min"));
		mainTableWidget->item(CELL_SIGN_IN_ANALOG_THRESHOLDS_MAX)->setText(QString("Max"));
		break;
	}
}

void MoreWindowInAnalog_AutoStend::setValues()
{
	mainTableWidget->blockSignals(true);
	for (int i = 0; i < row->thresholdsAuto.size(); i++)
	{ 
		for (int j = BASE_COLUMN_COUNT + (i * MEASURED_OFFSET_TRIPPLE); j < (BASE_COLUMN_COUNT + (i * MEASURED_OFFSET_TRIPPLE)) + MEASURED_OFFSET_TRIPPLE; j++)
			mainTableWidget->item(IND_OUT_ROW_VALUES - 1, j)->setBackground(QBrush(Qt::transparent));
		
		if (row->typeInt == TypeCable::SLEEP)
		{
			mainTableWidget->item(CELL_VALUE_IN_ANALOG_THRESHOLDS_MIN + (i * MEASURED_OFFSET_TRIPPLE))->setText(QString::number(row->thresholdsAuto[i].minCurrent) != "-1" ? QString::number(row->thresholdsAuto[i].minCurrent) : "-");
			mainTableWidget->item(CELL_VALUE_IN_ANALOG_THRESHOLDS_MAX + (i * MEASURED_OFFSET_TRIPPLE))->setText(QString::number(row->thresholdsAuto[i].maxCurrent) != "-1" ? QString::number(row->thresholdsAuto[i].maxCurrent) : "-");
		}
		else
		{
			mainTableWidget->item(CELL_VALUE_IN_ANALOG_THRESHOLDS_MIN + (i * MEASURED_OFFSET_TRIPPLE))->setText(QString::number(row->thresholdsAuto[i].minValue) != "-1" ? QString::number(row->thresholdsAuto[i].minValue) : "-");
			mainTableWidget->item(CELL_VALUE_IN_ANALOG_THRESHOLDS_MAX + (i * MEASURED_OFFSET_TRIPPLE))->setText(QString::number(row->thresholdsAuto[i].maxValue) != "-1" ? QString::number(row->thresholdsAuto[i].maxValue) : "-");
		}

		mainTableWidget->item(CELL_VALUE_IN_ANALOG_THRESHOLDS_MIN + (i * MEASURED_OFFSET_TRIPPLE))->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
		mainTableWidget->item(CELL_VALUE_IN_ANALOG_THRESHOLDS_MAX + (i * MEASURED_OFFSET_TRIPPLE))->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);

		if (row->measureds.size())
		{
			if (row->typeInt == TypeCable::SLEEP)
				mainTableWidget->item(CELL_VALUE_IN_ANALOG_MEASURED_VALUES + (i * MEASURED_OFFSET_TRIPPLE))->setText(QString::number(row->measureds[i]->current));
			else
				mainTableWidget->item(CELL_VALUE_IN_ANALOG_MEASURED_VALUES + (i * MEASURED_OFFSET_TRIPPLE))->setText(QString::number(row->measureds[i]->digValue));
		}
		else
		{
			mainTableWidget->item(CELL_VALUE_IN_ANALOG_MEASURED_VALUES + (i * MEASURED_OFFSET_TRIPPLE))->setText("-");
			continue;
		}

		if (row->typeInt == TypeCable::SLEEP)
			if (row->measureds[i]->current < row->thresholdsAuto[i].minCurrent)
			{
				mainTableWidget->item(CELL_VALUE_IN_ANALOG_MEASURED_VALUES + (i * MEASURED_OFFSET_TRIPPLE))->setBackground(QBrush(COLOR_RED));
				mainTableWidget->item(CELL_VALUE_IN_ANALOG_THRESHOLDS_MIN + (i * MEASURED_OFFSET_TRIPPLE))->setBackground(QBrush(COLOR_RED));
			}
			else if (row->measureds[i]->current > row->thresholdsAuto[i].maxCurrent)
			{
				mainTableWidget->item(CELL_VALUE_IN_ANALOG_MEASURED_VALUES + (i * MEASURED_OFFSET_TRIPPLE))->setBackground(QBrush(COLOR_RED));
				mainTableWidget->item(CELL_VALUE_IN_ANALOG_THRESHOLDS_MAX + (i * MEASURED_OFFSET_TRIPPLE))->setBackground(QBrush(COLOR_RED));
			}
			else
				mainTableWidget->item(CELL_VALUE_IN_ANALOG_MEASURED_VALUES + (i * MEASURED_OFFSET_TRIPPLE))->setBackground(QBrush(COLOR_GREEN));

		else
			if (row->measureds[i]->digValue < row->thresholdsAuto[i].minValue)
			{
				mainTableWidget->item(CELL_VALUE_IN_ANALOG_MEASURED_VALUES + (i * MEASURED_OFFSET_TRIPPLE))->setBackground(QBrush(COLOR_RED));
				mainTableWidget->item(CELL_VALUE_IN_ANALOG_THRESHOLDS_MIN + (i * MEASURED_OFFSET_TRIPPLE))->setBackground(QBrush(COLOR_RED));
			}
			else if (row->measureds[i]->digValue > row->thresholdsAuto[i].maxValue)
			{
				mainTableWidget->item(CELL_VALUE_IN_ANALOG_MEASURED_VALUES + (i * MEASURED_OFFSET_TRIPPLE))->setBackground(QBrush(COLOR_RED));
				mainTableWidget->item(CELL_VALUE_IN_ANALOG_THRESHOLDS_MAX + (i * MEASURED_OFFSET_TRIPPLE))->setBackground(QBrush(COLOR_RED));
			}
			else
				mainTableWidget->item(CELL_VALUE_IN_ANALOG_MEASURED_VALUES + (i * MEASURED_OFFSET_TRIPPLE))->setBackground(QBrush(COLOR_GREEN));
	}
	mainTableWidget->blockSignals(false);
}

MoreWindowOut_ManualStend::MoreWindowOut_ManualStend(TestTableRowProperties* row) : MoreWindow(row)
{
	int measuredWidth = (int)row->thresholdsManual.size();
	this->resize(EMPTY_WINDOW_WIDTH + (measuredWidth * (WINDOW_MEASURED_WIDTH)), WINDOW_OUT_HEIGHT);
	this->setMinimumSize(EMPTY_WINDOW_WIDTH + (measuredWidth * (WINDOW_MEASURED_WIDTH)), WINDOW_OUT_HEIGHT);

	this->row = row;

	MoreWindow::setValues();
	generateSigns();
	setValues();

	startTestButton->hide();

	isAllInit = true;
	connect(mainTableWidget, &QTableWidget::cellChanged, this, &MoreWindow::on_mainTableWidget_cellChanged);
}

void MoreWindowOut_ManualStend::generateSigns()
{
	const int rowAddedPosition = 1;

	for (int i = 0; i < ROW_OFFSET_DOUBLE; i++)
		mainTableWidget->insertRow(rowAddedPosition);

	prepareItem(CELL_SIGN_BASE_CONNECTOR, SPAN_VERTICAL_TRIPPLE);
	prepareItem(CELL_SIGN_BASE_PIN, SPAN_VERTICAL_TRIPPLE);
	prepareItem(CELL_SIGN_BASE_DIRECTION, SPAN_VERTICAL_TRIPPLE);
	prepareItem(CELL_SIGN_BASE_TYPE, SPAN_VERTICAL_TRIPPLE);
	prepareItem(CELL_SIGN_BASE_NAME, SPAN_VERTICAL_TRIPPLE);
	prepareItem(CELL_SIGN_BASE_COMMENT, SPAN_VERTICAL_TRIPPLE);

	resetLanguage(OFFSET_NULL);

	for (int i = 0; i < row->thresholdsManual.size(); i++)
	{
		for (int j = 0; j < MEASURED_OFFSET_DOUBLE; j++)
		{
			mainTableWidget->insertColumn(MEASUREMENT_COLUMN_POSITION);
			mainTableWidget->setColumnWidth(MEASUREMENT_COLUMN_POSITION, 60);
		}

		prepareItem(CELL_OUT_SIGN_THRESHOLDS_MANUALSTEND, SPAN_HORIZONTAL_DOUBLE);
		prepareItem(CELL_OUT_SING_MIN_THRESHOLDS_MANUALSTAND, SPAN_NONE);
		prepareItem(CELL_OUT_SING_MAX_THRESHOLDS_MANUALSTAND, SPAN_NONE);
		prepareItem(CELL_OUT_VALUES_MIN_THRESHOLDS_MANUALSTEND, SPAN_NONE);
		prepareItem(CELL_OUT_VALUES_MAX_THRESHOLDS_MANUALSTEND, SPAN_NONE);

		resetBlockLanguage((int)row->thresholdsManual.size() - i);
	}

	prepareItem(CELL_OUT_SIGN_ADC_MANUALSTEND, 1, 2 * row->thresholdsManual.size());
	mainTableWidget->item(CELL_OUT_SIGN_ADC_MANUALSTEND)->setText(viewWindowState->appLanguage ? QString("АЦП") : QString("ADC"));

}

void MoreWindowOut_ManualStend::resetBlockLanguage(int measuredNum)
{
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		mainTableWidget->item(CELL_OUT_SIGN_THRESHOLDS_MANUALSTEND)->setText(QString("Порог ") + QString::number(measuredNum));
		mainTableWidget->item(CELL_OUT_SING_MIN_THRESHOLDS_MANUALSTAND)->setText(QString("Мин"));
		mainTableWidget->item(CELL_OUT_SING_MAX_THRESHOLDS_MANUALSTAND)->setText(QString("Макс"));
		break;

	case ENGLISH_LANG:
		mainTableWidget->item(CELL_OUT_SIGN_THRESHOLDS_MANUALSTEND)->setText(QString("Threshold ") + QString::number(measuredNum));
		mainTableWidget->item(CELL_OUT_SING_MIN_THRESHOLDS_MANUALSTAND)->setText(QString("Min"));
		mainTableWidget->item(CELL_OUT_SING_MAX_THRESHOLDS_MANUALSTAND)->setText(QString("Max"));


		break;
	}
}

void MoreWindowOut_ManualStend::setValues()
{
	mainTableWidget->blockSignals(true);
	for (int i = 0; i < row->thresholdsManual.size(); i++)
	{
		for (int j = BASE_COLUMN_COUNT + (i * MEASURED_OFFSET_DOUBLE); j < (BASE_COLUMN_COUNT + (i * MEASURED_OFFSET_DOUBLE)) + MEASURED_OFFSET_DOUBLE; j++)
			mainTableWidget->item(IND_OUT_ROW_VALUES_MANUALSTAND, j)->setBackground(QBrush(Qt::transparent));
		mainTableWidget->item(CELL_OUT_VALUES_MIN_THRESHOLDS_MANUALSTEND + (i * MEASURED_OFFSET_DOUBLE))->setText(row->thresholdsManual[i].minValue != -1 ? QString::number(row->thresholdsManual[i].minValue) : "-");
		mainTableWidget->item(CELL_OUT_VALUES_MAX_THRESHOLDS_MANUALSTEND + (i * MEASURED_OFFSET_DOUBLE))->setText(row->thresholdsManual[i].maxValue != -1 ? QString::number(row->thresholdsManual[i].maxValue) : "-");

		mainTableWidget->item(CELL_OUT_VALUES_MIN_THRESHOLDS_MANUALSTEND + (i * MEASURED_OFFSET_DOUBLE))->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
		mainTableWidget->item(CELL_OUT_VALUES_MAX_THRESHOLDS_MANUALSTEND + (i * MEASURED_OFFSET_DOUBLE))->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
	}
	mainTableWidget->blockSignals(false);
}

void MoreWindow::on_mainTableWidget_cellChanged(int row, int column)
{
	if (isAllInit)
	{
		bool badTry = false;
		bool ok;
		QString passwordUser = QInputDialog::getText(this, viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Админестрирование") : QString("Administration"), viewWindowState->appLanguage == RUSSIAN_LANG ? QString("Введите пароль:") : QString("Enter password:"), QLineEdit::Normal, "", &ok);
		if (!(ok && passwordUser == PASSWORD))
		{
			QMessageBox::warning(this, QString("Внимание"), QString("Неверный пароль!!!"));
			badTry = true;
		}

		TypeStand typeStand = this->row->manualCheckBox != nullptr ? TypeStand::MANUAL : TypeStand::AUTO;

		int value = column - MEASUREMENT_COLUMN_POSITION;
		if (typeStand == TypeStand::MANUAL)
		{
			value++;
			if(this->row->direction == "OUT")
				value += 6;
		}

		int index = 0;
		int oldValueInt = -1;
		float oldValueFloat = -1;







		int currentOffset = MEASURED_OFFSET_DOUBLE;

		if (this->row->direction == "OUT" && typeStand != TypeStand::MANUAL)
			currentOffset = MEASURED_OFFSET_NINETUPLE;
		else if (typeStand == TypeStand::AUTO)
			currentOffset = MEASURED_OFFSET_TRIPPLE;
		//else
		//	currentOffset = MEASURED_OFFSET_DOUBLE;
		if (typeStand == TypeStand::MANUAL)
		{
			if (this->row->direction == "OUT")
			{

			}
			else
			{
				while (value - currentOffset > 0)
				{
					value -= currentOffset;
					index++;
				}
			}

		}
		else
		{
			while (value - currentOffset > 0)
			{
				value -= currentOffset;
				index++;
			}
			if (this->row->direction == "OUT")
			{

			}
			else
			{

			}

		}
		Thresholds tmpItem(typeStand == TypeStand::MANUAL ? this->row->thresholdsManual[index] : this->row->thresholdsAuto[index]);

			if (!badTry)
			{
			QString text = mainTableWidget->item(row, column)->text();
			if (this->row->direction == "OUT")
			{
				switch (value)
				{
				case 0:
					oldValueInt = tmpItem.minValue;
					tmpItem.minValue = text.toInt();
					break;

				case 1:
					oldValueInt = tmpItem.maxValue;
					tmpItem.maxValue = text.toInt();
					break;
				case 3:
					oldValueFloat = tmpItem.minVoltage;
					tmpItem.minVoltage = text.toFloat();
					break;

				case 4:
					oldValueFloat = tmpItem.maxVoltage;
					tmpItem.maxVoltage = text.toFloat();
					break;

				case 5:
					oldValueFloat = tmpItem.minCurrent;
					tmpItem.minCurrent = text.toFloat();
					break;

				case 6:
					oldValueFloat = tmpItem.maxCurrent;
					tmpItem.maxCurrent = text.toFloat();
					break;

				case 7:
					oldValueInt = tmpItem.minValue;
					tmpItem.minValue = text.toInt();
					break;

				case 8:
					oldValueInt = tmpItem.maxValue;
					tmpItem.maxValue = text.toInt();
					break;
				}
			}
			else
			{
				switch (value)
				{
				case 1:
					oldValueInt = tmpItem.minValue;
					tmpItem.minValue = text.toFloat();
					break;

				case 2:
					oldValueInt = tmpItem.maxValue;
					tmpItem.maxValue = text.toFloat();
					break;
				}
			}
			if (typeStand == TypeStand::MANUAL)
				this->row->thresholdsManual[index] = tmpItem;
			else
				this->row->thresholdsAuto[index] = tmpItem;

			badTry = !resaveFile();
		}
		if (badTry)
		{
			mainTableWidget->blockSignals(true);
			if (this->row->direction == "OUT")
			{
				switch (value)
				{
				case 0:
					mainTableWidget->item(row, column)->setText(QString::number(oldValueFloat));
					tmpItem.minVoltage = oldValueFloat;
					break;

				case 1:
					mainTableWidget->item(row, column)->setText(QString::number(oldValueFloat));
					tmpItem.maxVoltage = oldValueFloat;
					break;
				case 3:
					mainTableWidget->item(row, column)->setText(QString::number(oldValueFloat));
					tmpItem.minVoltage = oldValueFloat;
					break;

				case 4:
					mainTableWidget->item(row, column)->setText(QString::number(oldValueFloat));
					tmpItem.maxVoltage = oldValueFloat;
					break;

				case 5:
					mainTableWidget->item(row, column)->setText(QString::number(oldValueFloat));
					tmpItem.minCurrent = oldValueFloat;
					break;

				case 6:
					mainTableWidget->item(row, column)->setText(QString::number(oldValueFloat));
					tmpItem.maxCurrent = oldValueFloat;
					break;

				case 7:
					mainTableWidget->item(row, column)->setText(QString::number(oldValueInt));
					tmpItem.minValue = oldValueInt;
					break;

				case 8:
					mainTableWidget->item(row, column)->setText(QString::number(oldValueInt));
					tmpItem.maxValue = oldValueInt;
					break;
				}
			}
			else if (this->row->typeInt == TypeCable::ANALOG_IN)
			{
				switch (value)
				{
				case 1:
					mainTableWidget->item(row, column)->setText(QString::number(oldValueInt));
					tmpItem.minValue = oldValueInt;
					break;

				case 2:
					mainTableWidget->item(row, column)->setText(QString::number(oldValueInt));
					tmpItem.maxValue = oldValueInt;
					break;
				}
			}
			if (typeStand == TypeStand::MANUAL)
				this->row->thresholdsManual[index] = tmpItem;
			else
				this->row->thresholdsAuto[index] = tmpItem;
			mainTableWidget->blockSignals(false);
		}
		setValues();
	}
}

void MoreWindow::on_commentTextEdit_textChanged()
{
	row->comment = commentTextEdit->toPlainText();
}

bool MoreWindow::resaveFile()
{
	QStringList data = MainWindow::ReadFileFromArchiv();


	QString outputString = "";
	bool isFound = false;
	bool admissionBlock = false;
	bool admissionVersion = false;

	for(int i = 2; i < data.size(); i++)
	{
		QString dataLine = data[i];// = fin.readLine();
		
		if (dataLine == "DTM")
			if (viewWindowState->selectedBlock == TestBlockName::DTM)
				admissionBlock = true;
			else
				admissionBlock = false;

		if (dataLine == "BCM")
			if (viewWindowState->selectedBlock == TestBlockName::BCM)
				admissionBlock = true;
			else
				admissionBlock = false;
		if (dataLine == "SMXX")
			if (viewWindowState->selectedBlock == TestBlockName::SMXX)
				admissionBlock = true;
			else
				admissionBlock = false;

		QString proccessedDataLine = dataLine;
		proccessedDataLine.remove(":");

		if (dataLine[0] == ":")
			if (proccessedDataLine == viewWindowState->actualVersion && admissionBlock)
				admissionVersion = true;
			else
				admissionVersion = false;

		if (dataLine == "DTM" || dataLine == "BCM" || dataLine == "SMXX")
		{
			outputString += dataLine + "\n";
			continue;
		}
		if (dataLine[0] == ":")
		{
			outputString += dataLine + "\n";
			continue;
		}

		//dataLine.remove("\n");
		QStringList dataList = dataLine.split(";");

		bool typeCheck = false;
		switch (row->typeInt)
		{
		case TypeCable::EMPTY:
			break;
		case TypeCable::DIG_IN:
				typeCheck = (dataList[3].toInt() == TYPE_DIGITAL);
			break;
		case TypeCable::ANALOG_IN:
				typeCheck = (dataList[3].toInt() == TYPE_ANALOG);
			break;
		case TypeCable::HALL_IN:
				typeCheck = (dataList[3].toInt() == TYPE_HALL);
			break;
		case TypeCable::DIG_OUT:
				typeCheck = (dataList[3].toInt() == TYPE_DIGITAL);
			break;
		case TypeCable::PWM_OUT:
				typeCheck = (dataList[3].toInt() == TYPE_PWM);
			break;
		case TypeCable::VNH_OUT:
				typeCheck = (dataList[3].toInt() == TYPE_VNH);
			break;
		case TypeCable::HLD_OUT:
				typeCheck = (dataList[3].toInt() == TYPE_HLD);
			break;
		case TypeCable::CAN_OUT:
				typeCheck = (dataList[3].toInt() == TYPE_CAN);
			break;
		case TypeCable::LIN_OUT:
				typeCheck = (dataList[3].toInt() == TYPE_LIN);
			break;
		default:
			break;
		}

		if (admissionBlock && admissionVersion)
			if ((dataList[0].toInt() == (int)row->connectorInt) &&
				(dataList[1] == row->pin) &&
				((dataList[2].toInt() ? "IN" : "OUT") == row->direction) &&
				typeCheck &&
				(dataList[4].toInt(nullptr, 16) == row->canId) &&
				(dataList[5].toInt() == row->bit) &&
				(dataList[6] == row->name) &&
				(dataList[7] == row->component))
			{
				isFound = true;
				int counter = 0;
				switch (row->typeInt)
				{
				case TypeCable::DIG_OUT:
				case TypeCable::PWM_OUT:
				case TypeCable::VNH_OUT:
				case TypeCable::HLD_OUT:

					for (int i = 0; i < row->thresholdsManual.size(); i++, counter++)
						dataList[FILE_MEASUREMENT_OFFSET + counter] = "M|" + QString::number(row->thresholdsManual[i].minValue) + "|"
																			+ QString::number(row->thresholdsManual[i].maxValue);
					for (int i = 0; i < row->thresholdsAuto.size(); i++, counter++)
						dataList[FILE_MEASUREMENT_OFFSET + counter] = "A|"	+ QString::number(row->thresholdsAuto[i].minVoltage) + "|"
																			+ QString::number(row->thresholdsAuto[i].maxVoltage) + "|"
																			+ QString::number(row->thresholdsAuto[i].minCurrent) + "|"
																			+ QString::number(row->thresholdsAuto[i].maxCurrent) + "|"
																			+ QString::number(row->thresholdsAuto[i].minValue) + "|"
																			+ QString::number(row->thresholdsAuto[i].maxValue);
					break;

				case TypeCable::ANALOG_IN:
				case TypeCable::DIG_IN:
				case TypeCable::HALL_IN:
					for (int i = 0; i < row->thresholdsManual.size(); i++, counter++)
						dataList[FILE_MEASUREMENT_OFFSET + counter] = "M|"	+ QString::number(row->thresholdsManual[i].minValue) + "|"
																			+ QString::number(row->thresholdsManual[i].maxValue);
					for (int i = 0; i < row->thresholdsAuto.size(); i++, counter++)
						dataList[FILE_MEASUREMENT_OFFSET + counter] = "A|"	+ QString::number(row->thresholdsAuto[i].minValue) + "|"
																			+ QString::number(row->thresholdsAuto[i].maxValue);
					break;
				}
			}

		for (int i = 0; i < dataList.size(); i++)
			outputString += dataList[i] + ";";
		outputString[outputString.size() - 1] = '\n';
	}
	outputString.remove(outputString.size() - 1, 1); 
	outputString.insert(0, data[1] + '\n');
	outputString.insert(0, data[0] + '\n');


	if (!isFound)
	{
		generateWarning(Warnings::MoreWindow::OPEN_FILE_ERROR);
		return false;
	}
	if (!MainWindow::GenerateNewArchiv(outputString.split('\n')))
	{
		generateWarning(Warnings::MoreWindow::NO_ACCESS_FILE);
		return false;
	}
	return true;
}

void MoreWindow::on_startTestButton_clicked()
{
	Can::sendTestMsg(row->connectorInt, row->pin.toInt(), row->canId, row->bit, row->typeInt, TestBlockName::BCM);
}

void MoreWindow::generateWarning(Warnings::MoreWindow warning)
{
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		switch (warning)
		{
		case Warnings::MoreWindow::FILE_NOT_FOUND: // Warning 0x201
			QMessageBox::warning(this, QString("Внимание"), QString("Нет конфигурационного файла. Проверьте наличие в папке и перезапустите приложение\nWarning 0x201"));
			break;
		
		case Warnings::MoreWindow::OPEN_FILE_ERROR: // Warning 0x202
			QMessageBox::warning(this, QString("Внимание"), QString("Не обнаружен нужный кабель, попробуйте вернутся к исходному конфигурационному файлу\nWarning 0x202"));
			break;

		case Warnings::MoreWindow::NO_ACCESS_FILE: // Warning 0x203
			QMessageBox::warning(this, QString("Внимание"), QString("Не удалось получить доступ к конфигурационному файлу\nWarning 0x203"));
			break;
		}
		break;

	case ENGLISH_LANG:
		switch (warning)
		{
		case Warnings::MoreWindow::FILE_NOT_FOUND: // Warning 0x201
			QMessageBox::warning(this, QString("Warning"), QString("No configuration file. Check for presence in the folder and restart the application\nWarning 0x201"));
			break;

		case Warnings::MoreWindow::OPEN_FILE_ERROR: // Warning 0x202
			QMessageBox::warning(this, QString("Warning"), QString("The required cable was not found, try to return to the original configuration file\nWarning 0x202"));
			break;

		case Warnings::MoreWindow::NO_ACCESS_FILE: // Warning 0x203
			QMessageBox::warning(this, QString("Внимание"), QString("Failed to access the configuration file\nWarning 0x203"));
			break;
		}
		break;
	}
}