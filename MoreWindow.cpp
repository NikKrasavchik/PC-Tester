#include "MoreWindow.h"

#define BUTTON_SIZE						100, 40

#define MEASUREMENT_COLUMN_POSITION		5

#define IND_COLUMN_BASE_COMMENT			5

#define CELL_MIN_CURRENT				6
#define CELL_MAX_CURRENT				7
#define CELL_MIN_VOLTAGE				8
#define CELL_MAX_VOLTAGE				9

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
#ifdef QT5
		startTestButton->setText(QString::fromLocal8Bit	("Тест"));
		saveChangesButton->setText(QString::fromLocal8Bit("Сохранить"));
#elif QT6

#endif // QT5
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

// ------------------------------------
// Name: prepareItem
//		Подготовка ячейки для записи в него данных
// Variables: 
//			int row: индекс строчки ячейки
//			int column: индекс столбца ячйки
//			int rowSpan: количество ячейек для горизонтального соединения
//			int columnSpan: количество ячейук для вертикального соединения
// ------------------------------------
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

// ------------------------------------
// Name: setValues
//		Запись значений кабеля в таблицу
// ------------------------------------
void MoreWindow::setValues()
{
	mainTableWidget->item(CELL_VALUE_BASE_CONNECTOR)->setText(row->connectorStr);
	mainTableWidget->item(CELL_VALUE_BASE_PIN)->setText(row->pin);
	mainTableWidget->item(CELL_VALUE_BASE_DIRECTION)->setText(row->direction);
	mainTableWidget->item(CELL_VALUE_BASE_TYPE)->setText(row->typeStr);
	mainTableWidget->item(CELL_VALUE_BASE_NAME)->setText(row->name);
}

// ------------------------------------
// Name: resetLanguage
//		Обновление данных в ячейках под актуальный язык
// Variables: 
//			int offset: Значение смещения. По умолчанию OFFSET_NULL
// ------------------------------------
void MoreWindow::resetLanguage(int offset)
{
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
#ifdef QT5
		mainTableWidget->item(CELL_SIGN_BASE_CONNECTOR)->setText(QString::fromLocal8Bit("Колодка"));
		mainTableWidget->item(CELL_SIGN_BASE_PIN)->setText(QString::fromLocal8Bit("Пин"));
		mainTableWidget->item(CELL_SIGN_BASE_DIRECTION)->setText(QString::fromLocal8Bit("Направление"));
		mainTableWidget->item(CELL_SIGN_BASE_TYPE)->setText(QString::fromLocal8Bit("Тип"));
		mainTableWidget->item(CELL_SIGN_BASE_NAME)->setText(QString::fromLocal8Bit("Название"));
		mainTableWidget->item(CELL_SIGN_FILLED_COMMENT(offset))->setText(QString::fromLocal8Bit("Комментарий"));
#elif QT6

#endif // QT5
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

MoreWindowOut::MoreWindowOut(TestTableRowProperties* row) : MoreWindow(row)
{
	int measuredWidth = ((int)row->thresholds.size() <= 3 ? (int)row->thresholds.size() : 3);
	this->resize(EMPTY_WINDOW_WIDTH + (measuredWidth * WINDOW_MEASURED_WIDTH), WINDOW_OUT_HEIGHT);
	this->setMinimumSize(EMPTY_WINDOW_WIDTH + (measuredWidth * WINDOW_MEASURED_WIDTH), WINDOW_OUT_HEIGHT);

	this->row = row;

	MoreWindow::setValues();
	generateSigns();
	setValues();

	connect(mainTableWidget, &QTableWidget::cellChanged, this, &MoreWindow::on_mainTableWidget_cellChanged);
	isAllInit = true;
}

void MoreWindowOut::generateSigns()
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
	
	for (int i = 0; i < row->thresholds.size(); i++)
	{
		for (int j = 0; j < MEASURED_OFFSET_SEXTUPLE; j++)
		{
			mainTableWidget->insertColumn(MEASUREMENT_COLUMN_POSITION);
			mainTableWidget->setColumnWidth(MEASUREMENT_COLUMN_POSITION, 50);
		}

		prepareItem(CELL_OUT_SIGN_MEASURED, SPAN_HORIZONTAL_SEXTUPLE);
		prepareItem(CELL_OUT_SIGN_MEASURED_VALUES, SPAN_SQUAD_DOUBLE);
		prepareItem(CELL_OUT_SIGN_MEASURED_CURRENT, SPAN_NONE);
		prepareItem(CELL_OUT_SIGN_MEASURED_VOLTAGE, SPAN_NONE);
		prepareItem(CELL_OUT_SIGN_THRESHOLDS, SPAN_HORIZONTAL_QUADRUPLE);
		prepareItem(CELL_OUT_SIGN_THRESHOLDS_CURRENT, SPAN_HORIZONTAL_DOUBLE);
		prepareItem(CELL_OUT_SIGN_THRESHOLDS_VOLTAGE, SPAN_HORIZONTAL_DOUBLE);
		prepareItem(CELL_OUT_SIGN_MIN_CURRENT, SPAN_NONE);
		prepareItem(CELL_OUT_SIGN_MAX_CURRENT, SPAN_NONE);
		prepareItem(CELL_OUT_SING_MIN_VOLTAGE, SPAN_NONE);
		prepareItem(CELL_OUT_SING_MAX_VOLTAGE, SPAN_NONE);
		prepareItem(CELL_OUT_VALUES_MEASURED_CURRENT, SPAN_NONE);
		prepareItem(CELL_OUT_VALUES_MEASURED_VOLTAGE, SPAN_NONE);
		prepareItem(CELL_OUT_VALUES_MIN_CURRENT, SPAN_NONE);
		prepareItem(CELL_OUT_VALUES_MAX_CURRENT, SPAN_NONE);
		prepareItem(CELL_OUT_VALUES_MIN_VOLTAGE, SPAN_NONE);
		prepareItem(CELL_OUT_VALUES_MAX_VOLTAGE, SPAN_NONE);

		resetBlockLanguage((int)row->thresholds.size() - i);
	}
}

void MoreWindowOut::resetBlockLanguage(int measuredNum)
{
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
#ifdef QT5
		mainTableWidget->item(CELL_OUT_SIGN_MEASURED)->setText(QString::fromLocal8Bit("Измерение ") + QString::number(measuredNum));
		mainTableWidget->item(CELL_OUT_SIGN_MEASURED_VALUES)->setText(QString::fromLocal8Bit("Измеренное значение"));
		mainTableWidget->item(CELL_OUT_SIGN_MEASURED_CURRENT)->setText(QString::fromLocal8Bit("U, В"));
		mainTableWidget->item(CELL_OUT_SIGN_MEASURED_VOLTAGE)->setText(QString::fromLocal8Bit("I, А"));
		mainTableWidget->item(CELL_OUT_SIGN_THRESHOLDS)->setText(QString::fromLocal8Bit("Пороги"));
		mainTableWidget->item(CELL_OUT_SIGN_THRESHOLDS_CURRENT)->setText(QString::fromLocal8Bit("U, В"));
		mainTableWidget->item(CELL_OUT_SIGN_THRESHOLDS_VOLTAGE)->setText(QString::fromLocal8Bit("I, А"));
		mainTableWidget->item(CELL_OUT_SIGN_MIN_CURRENT)->setText(QString::fromLocal8Bit("Мин."));
		mainTableWidget->item(CELL_OUT_SIGN_MAX_CURRENT)->setText(QString::fromLocal8Bit("Макс."));
		mainTableWidget->item(CELL_OUT_SING_MIN_VOLTAGE)->setText(QString::fromLocal8Bit("Мин."));
		mainTableWidget->item(CELL_OUT_SING_MAX_VOLTAGE)->setText(QString::fromLocal8Bit("Макс."));
#elif QT6

#endif // QT5
		break;

	case ENGLISH_LANG:
		mainTableWidget->item(CELL_OUT_SIGN_MEASURED)->setText(QString("Measure ") + QString::number(measuredNum));
		mainTableWidget->item(CELL_OUT_SIGN_MEASURED_VALUES)->setText(QString("Measure values"));
		mainTableWidget->item(CELL_OUT_SIGN_MEASURED_CURRENT)->setText(QString("U, V"));
		mainTableWidget->item(CELL_OUT_SIGN_MEASURED_VOLTAGE)->setText(QString("I, A"));
		mainTableWidget->item(CELL_OUT_SIGN_THRESHOLDS)->setText(QString("Thresholds"));
		mainTableWidget->item(CELL_OUT_SIGN_THRESHOLDS_CURRENT)->setText(QString("U, V"));
		mainTableWidget->item(CELL_OUT_SIGN_THRESHOLDS_VOLTAGE)->setText(QString("I, A"));
		mainTableWidget->item(CELL_OUT_SIGN_MIN_CURRENT)->setText(QString("Min"));
		mainTableWidget->item(CELL_OUT_SIGN_MAX_CURRENT)->setText(QString("Max"));
		mainTableWidget->item(CELL_OUT_SING_MIN_VOLTAGE)->setText(QString("Min"));
		mainTableWidget->item(CELL_OUT_SING_MAX_VOLTAGE)->setText(QString("Max"));
		break;
	}
}

// ------------------------------------
// Name: setValues
//		Запись значений кабеля в таблицу
// ------------------------------------
void MoreWindowOut::setValues()
{
	for (int i = 0; i < row->thresholds.size(); i++)
	{
		mainTableWidget->item(CELL_OUT_VALUES_MEASURED_CURRENT + (i * MEASURED_OFFSET_SEXTUPLE))->setText(row->measureds[i]->current != -1 ? QString::number(row->measureds[i]->voltage) : "-");
		mainTableWidget->item(CELL_OUT_VALUES_MEASURED_VOLTAGE + (i * MEASURED_OFFSET_SEXTUPLE))->setText(row->measureds[i]->voltage != -1 ? QString::number(row->measureds[i]->current) : "-");
		mainTableWidget->item(CELL_OUT_VALUES_MIN_CURRENT + (i * MEASURED_OFFSET_SEXTUPLE))->setText(row->thresholds[i].minVoltage != -1 ? QString::number(row->thresholds[i].minVoltage) : "-");
		mainTableWidget->item(CELL_OUT_VALUES_MAX_CURRENT + (i * MEASURED_OFFSET_SEXTUPLE))->setText(row->thresholds[i].maxVoltage != -1 ? QString::number(row->thresholds[i].maxVoltage) : "-");
		mainTableWidget->item(CELL_OUT_VALUES_MIN_VOLTAGE + (i * MEASURED_OFFSET_SEXTUPLE))->setText(row->thresholds[i].minCurrent != -1 ? QString::number(row->thresholds[i].minCurrent) : "-");
		mainTableWidget->item(CELL_OUT_VALUES_MAX_VOLTAGE + (i * MEASURED_OFFSET_SEXTUPLE))->setText(row->thresholds[i].maxCurrent != -1 ? QString::number(row->thresholds[i].maxCurrent) : "-");

		mainTableWidget->item(CELL_OUT_VALUES_MIN_CURRENT + (i * MEASURED_OFFSET_SEXTUPLE))->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
		mainTableWidget->item(CELL_OUT_VALUES_MAX_CURRENT + (i * MEASURED_OFFSET_SEXTUPLE))->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
		mainTableWidget->item(CELL_OUT_VALUES_MIN_VOLTAGE + (i * MEASURED_OFFSET_SEXTUPLE))->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
		mainTableWidget->item(CELL_OUT_VALUES_MAX_VOLTAGE + (i * MEASURED_OFFSET_SEXTUPLE))->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
		
		if (row->measureds[i]->current != -1)
			if (row->measureds[i]->current < row->thresholds[i].minCurrent)
			{
				mainTableWidget->item(CELL_OUT_VALUES_MEASURED_CURRENT + (i * MEASURED_OFFSET_SEXTUPLE))->setBackgroundColor(COLOR_RED);
				mainTableWidget->item(CELL_OUT_VALUES_MIN_CURRENT + (i * MEASURED_OFFSET_SEXTUPLE))->setBackgroundColor(COLOR_RED);
			}
			else if (row->measureds[i]->current > row->thresholds[i].maxCurrent)
			{
				mainTableWidget->item(CELL_OUT_VALUES_MEASURED_CURRENT + (i * MEASURED_OFFSET_SEXTUPLE))->setBackgroundColor(COLOR_RED);
				mainTableWidget->item(CELL_OUT_VALUES_MAX_CURRENT + (i * MEASURED_OFFSET_SEXTUPLE))->setBackgroundColor(COLOR_RED);
			}
			else
				mainTableWidget->item(CELL_OUT_VALUES_MEASURED_CURRENT + (i * MEASURED_OFFSET_SEXTUPLE))->setBackgroundColor(COLOR_GREEN);

		if (row->measureds[i]->voltage != -1)
			if (row->measureds[i]->voltage < row->thresholds[i].minVoltage)
			{
				mainTableWidget->item(CELL_OUT_VALUES_MEASURED_VOLTAGE + (i * MEASURED_OFFSET_SEXTUPLE))->setBackgroundColor(COLOR_RED);
				mainTableWidget->item(CELL_OUT_VALUES_MIN_VOLTAGE + (i * MEASURED_OFFSET_SEXTUPLE))->setBackgroundColor(COLOR_RED);
			}
			else if (row->measureds[i]->voltage > row->thresholds[i].maxVoltage)
			{
				mainTableWidget->item(CELL_OUT_VALUES_MEASURED_VOLTAGE + (i * MEASURED_OFFSET_SEXTUPLE))->setBackgroundColor(COLOR_RED);
				mainTableWidget->item(CELL_OUT_VALUES_MAX_VOLTAGE + (i * MEASURED_OFFSET_SEXTUPLE))->setBackgroundColor(COLOR_RED);
			}
			else
				mainTableWidget->item(CELL_OUT_VALUES_MEASURED_VOLTAGE + (i * MEASURED_OFFSET_SEXTUPLE))->setBackgroundColor(COLOR_GREEN);
	}
}

MoreWindowIn::MoreWindowIn(TestTableRowProperties* row) : MoreWindow(row)
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

void MoreWindowIn::generateSigns()
{
	resetLanguage(OFFSET_NULL);

	for (int i = 0; i < row->thresholds.size(); i++)
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
		
		resetBlockLanguage((int)row->thresholds.size() - i);
	}
}

// ------------------------------------
// Name: setValues
//		Запись значений кабеля в таблицу
// ------------------------------------
void MoreWindowIn::setValues()
{
	if (row->measureds[0]->voltage == 1)
		mainTableWidget->item(CELL_IN_VALUE_MEASURED_1)->setBackgroundColor(QColor(COLOR_GREEN));
	else if (row->measureds[0]->voltage == 0)
		mainTableWidget->item(CELL_IN_VALUE_MEASURED_1)->setBackgroundColor(QColor(COLOR_RED));
	else
		mainTableWidget->item(CELL_IN_VALUE_MEASURED_1)->setText("-");

	if (row->measureds[0]->current == 1)
		mainTableWidget->item(CELL_IN_VALUE_MEASURED_2)->setBackgroundColor(QColor(COLOR_GREEN));
	else if (row->measureds[0]->current == 0)
		mainTableWidget->item(CELL_IN_VALUE_MEASURED_2)->setBackgroundColor(QColor(COLOR_RED));
	else
		mainTableWidget->item(CELL_IN_VALUE_MEASURED_2)->setText("-");
}

void MoreWindowIn::resetBlockLanguage(int measuredNum)
{
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
#ifdef QT5
		mainTableWidget->item(CELL_IN_SIGN_MEASURED_1)->setText(QString::fromLocal8Bit("Значение 1"));
		mainTableWidget->item(CELL_IN_SIGN_MEASURED_2)->setText(QString::fromLocal8Bit("Значение 2"));
#elif QT6

#endif // QT5
		break;

	case ENGLISH_LANG:
		mainTableWidget->item(CELL_IN_SIGN_MEASURED_1)->setText(QString("Value 1"));
		mainTableWidget->item(CELL_IN_SIGN_MEASURED_2)->setText(QString("Value 2"));
		break;
	}
}

MoreWindowInAnalog::MoreWindowInAnalog(TestTableRowProperties* row) : MoreWindow(row)
{
	int measuredWidth = ((int)row->thresholds.size() <= 3 ? (int)row->thresholds.size() : 3);
	this->resize(EMPTY_WINDOW_WIDTH + (measuredWidth * WINDOW_MEASURED_WIDTH), WINDOW_OUT_HEIGHT);
	this->setMinimumSize(EMPTY_WINDOW_WIDTH + (measuredWidth * WINDOW_MEASURED_WIDTH), WINDOW_OUT_HEIGHT);

	this->row = row;

	MoreWindow::setValues();
	generateSigns();
	setValues();

	connect(mainTableWidget, &QTableWidget::cellChanged, this, &MoreWindow::on_mainTableWidget_cellChanged);
	isAllInit = true;
}

void MoreWindowInAnalog::generateSigns()
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

	for (int i = 0; i < row->thresholds.size(); i++)
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

		resetBlockLanguage((int)row->thresholds.size() - i);
	}
}

void MoreWindowInAnalog::resetBlockLanguage(int measuredNum)
{
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
#ifdef QT5
		mainTableWidget->item(CELL_SIGN_IN_ANALOG_MEASURED)->setText(QString::fromLocal8Bit("Измерение ") + QString::number(measuredNum));
		mainTableWidget->item(CELL_SIGN_IN_ANALOG_MEASURED_VALUES)->setText(QString::fromLocal8Bit("Измеренное значение"));
		mainTableWidget->item(CELL_SIGN_IN_ANALOG_THRESHOLDS)->setText(QString::fromLocal8Bit("Пороги"));
		mainTableWidget->item(CELL_SIGN_IN_ANALOG_THRESHOLDS_MIN)->setText(QString::fromLocal8Bit("Мин"));
		mainTableWidget->item(CELL_SIGN_IN_ANALOG_THRESHOLDS_MAX)->setText(QString::fromLocal8Bit("Макс"));
#elif QT6

#endif // QT5
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

// ------------------------------------
// Name: setValues
//		Запись значений кабеля в таблицу
// ------------------------------------
void MoreWindowInAnalog::setValues()
{
	for (int i = 0; i < row->thresholds.size(); i++)
	{ 
		mainTableWidget->item(CELL_VALUE_IN_ANALOG_MEASURED_VALUES + (i * MEASURED_OFFSET_TRIPPLE))->setText(QString::number(row->measureds[i]->voltage) != "-1" ? QString::number(row->measureds[i]->voltage) : "-");
		if (row->measureds[i]->voltage == -1)
			mainTableWidget->item(CELL_VALUE_IN_ANALOG_MEASURED_VALUES + (i * MEASURED_OFFSET_TRIPPLE))->setText("-");
		else
			if (row->measureds[i]->voltage < row->thresholds[i].minValue)
			{
				mainTableWidget->item(CELL_VALUE_IN_ANALOG_MEASURED_VALUES + (i * MEASURED_OFFSET_TRIPPLE))->setBackgroundColor(COLOR_RED);
				mainTableWidget->item(CELL_VALUE_IN_ANALOG_THRESHOLDS_MIN + (i * MEASURED_OFFSET_TRIPPLE))->setBackgroundColor(COLOR_RED);
			}
			else if (row->measureds[i]->voltage > row->thresholds[i].maxValue)
			{
				mainTableWidget->item(CELL_VALUE_IN_ANALOG_MEASURED_VALUES + (i * MEASURED_OFFSET_TRIPPLE))->setBackgroundColor(COLOR_RED);
				mainTableWidget->item(CELL_VALUE_IN_ANALOG_THRESHOLDS_MAX + (i * MEASURED_OFFSET_TRIPPLE))->setBackgroundColor(COLOR_RED);
			}
			else
				mainTableWidget->item(CELL_VALUE_IN_ANALOG_MEASURED_VALUES + (i * MEASURED_OFFSET_TRIPPLE))->setBackgroundColor(COLOR_GREEN);

		mainTableWidget->item(CELL_VALUE_IN_ANALOG_THRESHOLDS_MIN + (i * MEASURED_OFFSET_TRIPPLE))->setText(QString::number(row->thresholds[i].minValue) != "-1" ? QString::number(row->thresholds[i].minValue) : "-");
		mainTableWidget->item(CELL_VALUE_IN_ANALOG_THRESHOLDS_MAX + (i * MEASURED_OFFSET_TRIPPLE))->setText(QString::number(row->thresholds[i].maxValue) != "-1" ? QString::number(row->thresholds[i].maxValue) : "-");

		mainTableWidget->item(CELL_VALUE_IN_ANALOG_THRESHOLDS_MIN + (i * MEASURED_OFFSET_TRIPPLE))->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
		mainTableWidget->item(CELL_VALUE_IN_ANALOG_THRESHOLDS_MAX + (i * MEASURED_OFFSET_TRIPPLE))->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
	}
}

void MoreWindow::on_mainTableWidget_cellChanged(int row, int column)
{
	if (isAllInit)
	{
		int value = column - MEASUREMENT_COLUMN_POSITION;
		int currentOffset;
		if (this->row->direction == "OUT")
			currentOffset = MEASURED_OFFSET_SEXTUPLE;
		else if (this->row->typeInt == TypeCable::ANALOG_IN)
			currentOffset = MEASURED_OFFSET_TRIPPLE;
		else
			currentOffset = MEASURED_OFFSET_DOUBLE;

		int index = 0;
		while (value - currentOffset >= 0)
		{
			value -= currentOffset;
			index++;
		}

		QString text = mainTableWidget->item(row, column)->text();
		if (this->row->direction == "OUT")
		{
			switch (value)
			{
			case 2:
				this->row->thresholds[index].minVoltage = text.toFloat();
				break;

			case 3:
				this->row->thresholds[index].maxVoltage = text.toFloat();
				break;

			case 4:
				this->row->thresholds[index].minCurrent = text.toFloat();
				break;

			case 5:
				this->row->thresholds[index].maxCurrent = text.toFloat();
				break;
			}
		}
		else if (this->row->typeInt == TypeCable::ANALOG_IN)
		{
			switch (value)
			{
			case 1:
				this->row->thresholds[index].minValue = text.toFloat();
				break;

			case 2:
				this->row->thresholds[index].maxValue = text.toFloat();
				break;
			}
		}
		resaveFile();
	}
}

void MoreWindow::on_commentTextEdit_textChanged()
{
	row->comment = commentTextEdit->toPlainText();
}

void MoreWindow::resaveFile()
{
	QFile fin("cables.cfg");
	if (!fin.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		MoreWindow::generateWarning(Warnings::MoreWindow::OPEN_FILE_ERROR);
		return;
	}

	QString config = fin.readLine();
	config += fin.readLine();

	QString outputString = "";
	bool isFound = false;
	bool admissionBlock = false;
	bool admissionVersion = false;
	while (!fin.atEnd())
	{
		QString dataLine = fin.readLine();
		
		if (dataLine == "DTM\n" && viewWindowState->selectedBlock == TestBlockName::DTM)
			if (viewWindowState->selectedBlock == TestBlockName::DTM)
				admissionBlock = true;
			else
				admissionBlock = false;

		if (dataLine == "BCM\n")
			if (viewWindowState->selectedBlock == TestBlockName::BCM)
				admissionBlock = true;
			else
				admissionBlock = false;

		QString proccessedDataLine = dataLine;
		proccessedDataLine.remove(":");
		proccessedDataLine.remove("\n");

		if (dataLine[0] == ":")
			if (proccessedDataLine == viewWindowState->actualVersion)
				admissionVersion = true;
			else
				admissionVersion = false;

		if (dataLine == "DTM\n" || dataLine == "BCM\n")
		{
			outputString += dataLine;
			continue;
		}
		if (dataLine[0] == ":")
		{
			outputString += dataLine;
			continue;
		}

		dataLine.remove("\n");
		QStringList dataList = dataLine.split(";");

		bool typeCheck = false;
		if (row->direction == "OUT")
			if (row->typeStr == "DIGITAL")
				typeCheck = (dataList[3].toInt() == TYPE_DIGITAL);
			else if (row->typeStr == "PWM")
				typeCheck = (dataList[3].toInt() == TYPE_PWM);
			else if (row->typeStr == "VNH")
				typeCheck = (dataList[3].toInt() == TYPE_VNH);
			else if (row->typeStr == "HLD")
				typeCheck = (dataList[3].toInt() == TYPE_HLD);
		else
			if (row->typeStr == "DIGITAL")
				typeCheck = (dataList[3].toInt() == TYPE_DIGITAL);
			else if (row->typeStr == "ANALOG")
				typeCheck = (dataList[3].toInt() == TYPE_ANALOG);
			else if (row->typeStr == "HALL")
				typeCheck = (dataList[3].toInt() == TYPE_HALL);

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
				switch (row->typeInt)
				{
				case TypeCable::DIG_OUT:
				case TypeCable::PWM_OUT:
				case TypeCable::VNH_OUT:
				case TypeCable::HLD_OUT:
					for (int i = 0; i < row->thresholds.size(); i++)
					{
						dataList[FILE_MEASUREMENT_OFFSET + i * 4] = QString::number(row->thresholds[i].minVoltage);
						dataList[FILE_MEASUREMENT_OFFSET + i * 4 + 1] = QString::number(row->thresholds[i].maxVoltage);
						dataList[FILE_MEASUREMENT_OFFSET + i * 4 + 2] = QString::number(row->thresholds[i].minCurrent);
						dataList[FILE_MEASUREMENT_OFFSET + i * 4 + 3] = QString::number(row->thresholds[i].maxCurrent);
					}
					break;

				case TypeCable::ANALOG_IN:
					for (int i = 0; i < row->thresholds.size(); i++)
					{
						dataList[FILE_MEASUREMENT_OFFSET + i * 2] = QString::number(row->thresholds[i].minValue);
						dataList[FILE_MEASUREMENT_OFFSET + i * 2 + 1] = QString::number(row->thresholds[i].maxValue);
					}
					break;
				}
			}

		for (int i = 0; i < dataList.size(); i++)
			outputString += dataList[i] + ";";
		outputString[outputString.size() - 1] = '\n';
	}
	outputString.remove(outputString.size() - 1, 1);
	outputString.insert(0, config);
	fin.close();

	if (!isFound)
	{
		generateWarning(Warnings::MoreWindow::FILE_NOT_FOUND);
		return;
	}
#ifdef QT_5

#endif // QT_5

	std::ofstream fout;
	fout.open("cables.cfg");
	fout << outputString.toStdString();
	fout.close();
}

void MoreWindow::on_startTestButton_clicked()
{
	Can::sendTestMsg(row->connectorInt, row->pin.toInt(), row->typeInt, TestBlockName::BCM);
}

void MoreWindow::generateWarning(Warnings::MoreWindow warning)
{
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		switch (warning)
		{
		case Warnings::MoreWindow::OPEN_FILE_ERROR:
			break;

		case Warnings::MoreWindow::FILE_NOT_FOUND:
			break;
		}
		break;

	case ENGLISH_LANG:
		switch (warning)
		{
		case Warnings::MoreWindow::OPEN_FILE_ERROR:
			break;

		case Warnings::MoreWindow::FILE_NOT_FOUND:
			break;
		}
		break;
	}
}