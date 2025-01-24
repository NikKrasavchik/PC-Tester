#include "MoreWindow.h"

#define CELL_MIN_CURRENT	6
#define CELL_MAX_CURRENT	7
#define CELL_MIN_VOLTAGE	8
#define CELL_MAX_VOLTAGE	9

MoreWindow::MoreWindow(TestTableRowProperties* row, int startWidth, int startHeight)
{
	this->row = row;
	this->coutTrehholders = NOT_SET;
	this->isAllInit = false;

	this->resize(startWidth, startHeight);
	this->setMinimumSize(startWidth, startHeight);

	font = new QFont();
	font->setBold(true);
	font->setPointSizeF(10);

	initBaseUi();
	QMetaObject::connectSlotsByName(this);
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
	connect(mainTableWidget, &QTableWidget::cellChanged, this, &MoreWindow::on_mainTableWidget_cellChanged);
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
		startTestButton->setText(QString::fromLocal8Bit("Тест"));
		saveChangesButton->setText(QString::fromLocal8Bit("Сохранить"));
		break;
	case ENGLISH_LANG:
		startTestButton->setText("Test");
		saveChangesButton->setText("Save");
		break;
	}

	mainVLayout->addLayout(bottomHLayout);
}

void MoreWindowOut::initUiTableOut()
{
	// Pad
	mainTableWidget->setSpan(OUTALL_CELL_PAD_TABLE, 4, 1);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(OUTALL_CELL_PAD_TABLE), "");
	mainTableWidget->item(OUTALL_CELL_PAD_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(OUTALL_CELL_PAD_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(OUTALL_CELL_PAD_TABLE)->setFont(*font);
	// Pin
	mainTableWidget->setSpan(OUTALL_CELL_PIN_TABLE, 4, 1);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(OUTALL_CELL_PIN_TABLE), "");
	mainTableWidget->item(OUTALL_CELL_PIN_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(OUTALL_CELL_PIN_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(OUTALL_CELL_PIN_TABLE)->setFont(*font);
	// Type
	mainTableWidget->setSpan(OUTALL_CELL_TYPE_TABLE, 4, 1);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(OUTALL_CELL_TYPE_TABLE), "");
	mainTableWidget->item(OUTALL_CELL_TYPE_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(OUTALL_CELL_TYPE_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(OUTALL_CELL_TYPE_TABLE)->setFont(*font);
	// Name
	mainTableWidget->setSpan(OUTALL_CELL_NAME_TABLE, 4, 1);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(OUTALL_CELL_NAME_TABLE), "");
	mainTableWidget->item(OUTALL_CELL_NAME_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(OUTALL_CELL_NAME_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(OUTALL_CELL_NAME_TABLE)->setFont(*font);
	// Comment
	mainTableWidget->setSpan(OUTALL_ROW_COMMENT_TABLE, OUTALL_COLUMN_COMMENT_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders), 4, 1);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(OUTALL_ROW_COMMENT_TABLE, OUTALL_COLUMN_COMMENT_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders)), "");
	mainTableWidget->item(OUTALL_ROW_COMMENT_TABLE, OUTALL_COLUMN_COMMENT_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders))->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(OUTALL_ROW_COMMENT_TABLE, OUTALL_COLUMN_COMMENT_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders))->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(OUTALL_ROW_COMMENT_TABLE, OUTALL_COLUMN_COMMENT_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders))->setFont(*font);

	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		mainTableWidget->item(OUTALL_CELL_PAD_TABLE)->setText(QString::fromLocal8Bit("Колодка"));
		mainTableWidget->item(OUTALL_CELL_PIN_TABLE)->setText(QString::fromLocal8Bit("Пин"));
		mainTableWidget->item(OUTALL_CELL_TYPE_TABLE)->setText(QString::fromLocal8Bit("Тип"));
		mainTableWidget->item(OUTALL_CELL_NAME_TABLE)->setText(QString::fromLocal8Bit("Название"));
		mainTableWidget->item(OUTALL_ROW_COMMENT_TABLE, OUTALL_COLUMN_COMMENT_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders))->setText(QString::fromLocal8Bit("Коментарий"));
		break;
	case ENGLISH_LANG:
		mainTableWidget->item(OUTALL_CELL_PAD_TABLE)->setText("Pad");
		mainTableWidget->item(OUTALL_CELL_PIN_TABLE)->setText("Pin");
		mainTableWidget->item(OUTALL_CELL_TYPE_TABLE)->setText("Type");
		mainTableWidget->item(OUTALL_CELL_NAME_TABLE)->setText("Name");
		mainTableWidget->item(OUTALL_ROW_COMMENT_TABLE, OUTALL_COLUMN_COMMENT_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders))->setText("Comments");
		break;
	}
	// Pad
	mainTableWidget->model()->setData(mainTableWidget->model()->index(OUTALL_CEll_VALUE_PAD_TABLE), row->connectorStr);
	mainTableWidget->item(OUTALL_CEll_VALUE_PAD_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(OUTALL_CEll_VALUE_PAD_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(OUTALL_CEll_VALUE_PAD_TABLE)->setFont(*font);
	// Pin
	mainTableWidget->model()->setData(mainTableWidget->model()->index(OUTALL_CELL_VALUE_PIN_TABLE), row->pin);
	mainTableWidget->item(OUTALL_CELL_VALUE_PIN_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(OUTALL_CELL_VALUE_PIN_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(OUTALL_CELL_VALUE_PIN_TABLE)->setFont(*font);
	// Type
	mainTableWidget->model()->setData(mainTableWidget->model()->index(OUTALL_CELL_VALUE_TYPE_TABLE), row->type);
	mainTableWidget->item(OUTALL_CELL_VALUE_TYPE_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(OUTALL_CELL_VALUE_TYPE_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(OUTALL_CELL_VALUE_TYPE_TABLE)->setFont(*font);
	// Name
	mainTableWidget->model()->setData(mainTableWidget->model()->index(OUTALL_CELL_VALUE_NAME_TABLE), row->name);
	mainTableWidget->item(OUTALL_CELL_VALUE_NAME_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(OUTALL_CELL_VALUE_NAME_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(OUTALL_CELL_VALUE_NAME_TABLE)->setFont(*font);
	// Comment
	commentTextEdit = new QTextEdit();
	commentTextEdit->setText(row->comment);
	connect(commentTextEdit, &QTextEdit::textChanged, this, &MoreWindow::on_commentTextEdit_textChanged);
	mainTableWidget->setCellWidget(OUTALL_ROW_VALUE_COMMENT_TABLE, OUTALL_COLUMN_VALUE_COMMENT_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders), commentTextEdit);

	mainTableWidget->setColumnWidth(OUTALL_COLUMN_PAD_TABLE, 65);
	mainTableWidget->setColumnWidth(OUTALL_COLUMN_PIN_TABLE, 65);
	mainTableWidget->setColumnWidth(OUTALL_COLUMN_TYPE_TABLE, 75);
	mainTableWidget->setColumnWidth(OUTALL_COLUMN_NAME_TABLE, 75);
	mainTableWidget->setColumnWidth(OUTALL_COLUMN_COMMENT_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders), 150);

	mainTableWidget->setRowHeight(0, 40);
	mainTableWidget->setRowHeight(1, 40);
	mainTableWidget->setRowHeight(2, 40);
	mainTableWidget->setRowHeight(3, 40);

	mainTableWidget->horizontalHeader()->setSectionResizeMode(OUTALL_COLUMN_PAD_TABLE, QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(OUTALL_COLUMN_PIN_TABLE, QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(OUTALL_COLUMN_TYPE_TABLE, QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(OUTALL_COLUMN_NAME_TABLE, QHeaderView::Stretch);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(OUTALL_COLUMN_COMMENT_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders), QHeaderView::Fixed);

	mainTableWidget->verticalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
	mainTableWidget->verticalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
	mainTableWidget->verticalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
	mainTableWidget->verticalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
	mainTableWidget->verticalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);

	// Error
	if (coutTrehholders > row->measureds.size())
	{
		mainTableWidget->model()->setData(mainTableWidget->model()->index(0, 0), "Error");
		return;
	}

	for (int i = 0; i < coutTrehholders; i++)
	{
		// Name measured
		mainTableWidget->setSpan(OUTALL_ROW_NAME_MEASURED_TABLE, OUTALL_COLUMN_NAME_MEASURED_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i), 1, 6);
		mainTableWidget->model()->setData(mainTableWidget->model()->index(OUTALL_ROW_NAME_MEASURED_TABLE, OUTALL_COLUMN_NAME_MEASURED_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i)), "");
		mainTableWidget->item(OUTALL_ROW_NAME_MEASURED_TABLE, OUTALL_COLUMN_NAME_MEASURED_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(OUTALL_ROW_NAME_MEASURED_TABLE, OUTALL_COLUMN_NAME_MEASURED_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(OUTALL_ROW_NAME_MEASURED_TABLE, OUTALL_COLUMN_NAME_MEASURED_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
		// Measured value
		mainTableWidget->setSpan(OUTALL_ROW_MEASURED_VALUE_TABLE, OUTALL_COLUMN_MEASURED_VALUE_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i), 2, 2);
		mainTableWidget->model()->setData(mainTableWidget->model()->index(OUTALL_ROW_MEASURED_VALUE_TABLE, OUTALL_COLUMN_MEASURED_VALUE_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i)), "");
		mainTableWidget->item(OUTALL_ROW_MEASURED_VALUE_TABLE, OUTALL_COLUMN_MEASURED_VALUE_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(OUTALL_ROW_MEASURED_VALUE_TABLE, OUTALL_COLUMN_MEASURED_VALUE_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(OUTALL_ROW_MEASURED_VALUE_TABLE, OUTALL_COLUMN_MEASURED_VALUE_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
		// Measured value U
		mainTableWidget->model()->setData(mainTableWidget->model()->index(OUTALL_ROW_MEASURED_VALUE_U_TABLE, OUTALL_COLUMN_MEASURED_VALUE_U_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i)), "");
		mainTableWidget->item(OUTALL_ROW_MEASURED_VALUE_U_TABLE, OUTALL_COLUMN_MEASURED_VALUE_U_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(OUTALL_ROW_MEASURED_VALUE_U_TABLE, OUTALL_COLUMN_MEASURED_VALUE_U_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(OUTALL_ROW_MEASURED_VALUE_U_TABLE, OUTALL_COLUMN_MEASURED_VALUE_U_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
		// Measured value I
		mainTableWidget->model()->setData(mainTableWidget->model()->index(OUTALL_ROW_MEASURED_VALUE_I_TABLE, OUTALL_COLUMN_MEASURED_VALUE_I_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i)), "");
		mainTableWidget->item(OUTALL_ROW_MEASURED_VALUE_I_TABLE, OUTALL_COLUMN_MEASURED_VALUE_I_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(OUTALL_ROW_MEASURED_VALUE_I_TABLE, OUTALL_COLUMN_MEASURED_VALUE_I_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(OUTALL_ROW_MEASURED_VALUE_I_TABLE, OUTALL_COLUMN_MEASURED_VALUE_I_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
		// Progs
		mainTableWidget->setSpan(OUTALL_ROW_TRESHHOLDERS_TABLE, OUTALL_COLUMN_TRESHHOLDERS_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i), 1, 4);
		mainTableWidget->model()->setData(mainTableWidget->model()->index(OUTALL_ROW_TRESHHOLDERS_TABLE, OUTALL_COLUMN_TRESHHOLDERS_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i)), "");
		mainTableWidget->item(OUTALL_ROW_TRESHHOLDERS_TABLE, OUTALL_COLUMN_TRESHHOLDERS_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(OUTALL_ROW_TRESHHOLDERS_TABLE, OUTALL_COLUMN_TRESHHOLDERS_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(OUTALL_ROW_TRESHHOLDERS_TABLE, OUTALL_COLUMN_TRESHHOLDERS_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
		// Progs U
		mainTableWidget->setSpan(OUTALL_ROW_TRESHHOLDERS_U_TABLE, OUTALL_COLUMN_TRESHHOLDERS_U_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i), 1, 2);
		mainTableWidget->model()->setData(mainTableWidget->model()->index(OUTALL_ROW_TRESHHOLDERS_U_TABLE, OUTALL_COLUMN_TRESHHOLDERS_U_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i)), "");
		mainTableWidget->item(OUTALL_ROW_TRESHHOLDERS_U_TABLE, OUTALL_COLUMN_TRESHHOLDERS_U_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(OUTALL_ROW_TRESHHOLDERS_U_TABLE, OUTALL_COLUMN_TRESHHOLDERS_U_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(OUTALL_ROW_TRESHHOLDERS_U_TABLE, OUTALL_COLUMN_TRESHHOLDERS_U_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
		// Progs I
		mainTableWidget->setSpan(OUTALL_ROW_TRESHHOLDERS_I_TABLE, OUTALL_COLUMN_TRESHHOLDERS_I_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i), 1, 2);
		mainTableWidget->model()->setData(mainTableWidget->model()->index(OUTALL_ROW_TRESHHOLDERS_I_TABLE, OUTALL_COLUMN_TRESHHOLDERS_I_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i)), "");
		mainTableWidget->item(OUTALL_ROW_TRESHHOLDERS_I_TABLE, OUTALL_COLUMN_TRESHHOLDERS_I_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(OUTALL_ROW_TRESHHOLDERS_I_TABLE, OUTALL_COLUMN_TRESHHOLDERS_I_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(OUTALL_ROW_TRESHHOLDERS_I_TABLE, OUTALL_COLUMN_TRESHHOLDERS_I_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
		// Progs U min
		mainTableWidget->model()->setData(mainTableWidget->model()->index(OUTALL_ROW_TRESHHOLDERS_U_MIN_TABLE, OUTALL_COLUMN_TRESHHOLDERS_U_MIN_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i)), "");
		mainTableWidget->item(OUTALL_ROW_TRESHHOLDERS_U_MIN_TABLE, OUTALL_COLUMN_TRESHHOLDERS_U_MIN_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(OUTALL_ROW_TRESHHOLDERS_U_MIN_TABLE, OUTALL_COLUMN_TRESHHOLDERS_U_MIN_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(OUTALL_ROW_TRESHHOLDERS_U_MIN_TABLE, OUTALL_COLUMN_TRESHHOLDERS_U_MIN_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
		// Progs U max
		mainTableWidget->model()->setData(mainTableWidget->model()->index(OUTALL_ROW_TRESHHOLDERS_U_MAX_TABLE, OUTALL_COLUMN_TRESHHOLDERS_U_MAX_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i)), "");
		mainTableWidget->item(OUTALL_ROW_TRESHHOLDERS_U_MAX_TABLE, OUTALL_COLUMN_TRESHHOLDERS_U_MAX_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(OUTALL_ROW_TRESHHOLDERS_U_MAX_TABLE, OUTALL_COLUMN_TRESHHOLDERS_U_MAX_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(OUTALL_ROW_TRESHHOLDERS_U_MAX_TABLE, OUTALL_COLUMN_TRESHHOLDERS_U_MAX_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
		// Progs I min
		mainTableWidget->model()->setData(mainTableWidget->model()->index(OUTALL_ROW_TRESHHOLDERS_I_MIN_TABLE, OUTALL_COLUMN_TRESHHOLDERS_I_MIN_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i)), "");
		mainTableWidget->item(OUTALL_ROW_TRESHHOLDERS_I_MIN_TABLE, OUTALL_COLUMN_TRESHHOLDERS_I_MIN_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(OUTALL_ROW_TRESHHOLDERS_I_MIN_TABLE, OUTALL_COLUMN_TRESHHOLDERS_I_MIN_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(OUTALL_ROW_TRESHHOLDERS_I_MIN_TABLE, OUTALL_COLUMN_TRESHHOLDERS_I_MIN_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
		// Progs I max
		mainTableWidget->model()->setData(mainTableWidget->model()->index(OUTALL_ROW_TRESHHOLDERS_I_MAX_TABLE, OUTALL_COLUMN_TRESHHOLDERS_I_MAX_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i)), "");
		mainTableWidget->item(OUTALL_ROW_TRESHHOLDERS_I_MAX_TABLE, OUTALL_COLUMN_TRESHHOLDERS_I_MAX_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(OUTALL_ROW_TRESHHOLDERS_I_MAX_TABLE, OUTALL_COLUMN_TRESHHOLDERS_I_MAX_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(OUTALL_ROW_TRESHHOLDERS_I_MAX_TABLE, OUTALL_COLUMN_TRESHHOLDERS_I_MAX_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);

		mainTableWidget->setColumnWidth(OUTALL_COLUMN_MEASURED_VALUE_U_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i), 50);
		mainTableWidget->setColumnWidth(OUTALL_COLUMN_MEASURED_VALUE_I_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i), 50);
		mainTableWidget->setColumnWidth(OUTALL_COLUMN_TRESHHOLDERS_U_MIN_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i), 50);
		mainTableWidget->setColumnWidth(OUTALL_COLUMN_TRESHHOLDERS_U_MAX_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i), 50);
		mainTableWidget->setColumnWidth(OUTALL_COLUMN_TRESHHOLDERS_I_MIN_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i), 50);
		mainTableWidget->setColumnWidth(OUTALL_COLUMN_TRESHHOLDERS_I_MAX_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i), 50);

		mainTableWidget->horizontalHeader()->setSectionResizeMode(OUTALL_COLUMN_TRESHHOLDERS_U_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i), QHeaderView::Fixed);
		mainTableWidget->horizontalHeader()->setSectionResizeMode(OUTALL_COLUMN_TRESHHOLDERS_I_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i), QHeaderView::Fixed);
		mainTableWidget->horizontalHeader()->setSectionResizeMode(OUTALL_COLUMN_TRESHHOLDERS_U_MIN_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i), QHeaderView::Fixed);
		mainTableWidget->horizontalHeader()->setSectionResizeMode(OUTALL_COLUMN_TRESHHOLDERS_U_MAX_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i), QHeaderView::Fixed);
		mainTableWidget->horizontalHeader()->setSectionResizeMode(OUTALL_COLUMN_TRESHHOLDERS_I_MIN_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i), QHeaderView::Fixed);
		mainTableWidget->horizontalHeader()->setSectionResizeMode(OUTALL_COLUMN_TRESHHOLDERS_I_MAX_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i), QHeaderView::Fixed);

		switch (viewWindowState->appLanguage)
		{
		case RUSSIAN_LANG:
			mainTableWidget->item(OUTALL_ROW_NAME_MEASURED_TABLE, OUTALL_COLUMN_NAME_MEASURED_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setText(QString::fromLocal8Bit("Измерение ") + QString::number(i + 1));
			mainTableWidget->item(OUTALL_ROW_MEASURED_VALUE_TABLE, OUTALL_COLUMN_MEASURED_VALUE_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setText(QString::fromLocal8Bit("Измеренное\nзначенние"));
			mainTableWidget->item(OUTALL_ROW_MEASURED_VALUE_U_TABLE, OUTALL_COLUMN_MEASURED_VALUE_U_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setText(QString::fromLocal8Bit("U, В"));
			mainTableWidget->item(OUTALL_ROW_MEASURED_VALUE_I_TABLE, OUTALL_COLUMN_MEASURED_VALUE_I_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setText(QString::fromLocal8Bit("I, А"));
			mainTableWidget->item(OUTALL_ROW_TRESHHOLDERS_TABLE, OUTALL_COLUMN_TRESHHOLDERS_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setText(QString::fromLocal8Bit("Пороги"));
			mainTableWidget->item(OUTALL_ROW_TRESHHOLDERS_U_TABLE, OUTALL_COLUMN_TRESHHOLDERS_U_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setText(QString::fromLocal8Bit("U, В"));
			mainTableWidget->item(OUTALL_ROW_TRESHHOLDERS_I_TABLE, OUTALL_COLUMN_TRESHHOLDERS_I_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setText(QString::fromLocal8Bit("I, А"));
			mainTableWidget->item(OUTALL_ROW_TRESHHOLDERS_U_MIN_TABLE, OUTALL_COLUMN_TRESHHOLDERS_U_MIN_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setText(QString::fromLocal8Bit("Мин."));
			mainTableWidget->item(OUTALL_ROW_TRESHHOLDERS_U_MAX_TABLE, OUTALL_COLUMN_TRESHHOLDERS_U_MAX_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setText(QString::fromLocal8Bit("Макс."));
			mainTableWidget->item(OUTALL_ROW_TRESHHOLDERS_I_MIN_TABLE, OUTALL_COLUMN_TRESHHOLDERS_I_MIN_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setText(QString::fromLocal8Bit("Мин."));
			mainTableWidget->item(OUTALL_ROW_TRESHHOLDERS_I_MAX_TABLE, OUTALL_COLUMN_TRESHHOLDERS_I_MAX_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setText(QString::fromLocal8Bit("Макс."));
			break;
		case ENGLISH_LANG:
			mainTableWidget->item(OUTALL_ROW_NAME_MEASURED_TABLE, OUTALL_COLUMN_NAME_MEASURED_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setText("Measured " + QString::number(i + 1));
			mainTableWidget->item(OUTALL_ROW_MEASURED_VALUE_TABLE, OUTALL_COLUMN_MEASURED_VALUE_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setText("Measured\nvalue");
			mainTableWidget->item(OUTALL_ROW_MEASURED_VALUE_U_TABLE, OUTALL_COLUMN_MEASURED_VALUE_U_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setText("U, V");
			mainTableWidget->item(OUTALL_ROW_MEASURED_VALUE_I_TABLE, OUTALL_COLUMN_MEASURED_VALUE_I_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setText("I, A");
			mainTableWidget->item(OUTALL_ROW_TRESHHOLDERS_TABLE, OUTALL_COLUMN_TRESHHOLDERS_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setText("Progs");
			mainTableWidget->item(OUTALL_ROW_TRESHHOLDERS_U_TABLE, OUTALL_COLUMN_TRESHHOLDERS_U_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setText("U, V");
			mainTableWidget->item(OUTALL_ROW_TRESHHOLDERS_I_TABLE, OUTALL_COLUMN_TRESHHOLDERS_I_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setText("I, A");
			mainTableWidget->item(OUTALL_ROW_TRESHHOLDERS_U_MIN_TABLE, OUTALL_COLUMN_TRESHHOLDERS_U_MIN_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setText("Min.");
			mainTableWidget->item(OUTALL_ROW_TRESHHOLDERS_U_MAX_TABLE, OUTALL_COLUMN_TRESHHOLDERS_U_MAX_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setText("Max.");
			mainTableWidget->item(OUTALL_ROW_TRESHHOLDERS_I_MIN_TABLE, OUTALL_COLUMN_TRESHHOLDERS_I_MIN_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setText("Min.");
			mainTableWidget->item(OUTALL_ROW_TRESHHOLDERS_I_MAX_TABLE, OUTALL_COLUMN_TRESHHOLDERS_I_MAX_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setText("Max.");
			break;
		}
		Measured* measured = row->measureds[i];
		// Measured value U
		mainTableWidget->model()->setData(mainTableWidget->model()->index(OUTALL_ROW_VALUE_MEASURED_VALUE_U_TABLE, OUTALL_COLUMN_VALUE_MEASURED_VALUE_U_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i)), measured->voltage == NOT_SET ? -1 : measured->voltage);
		mainTableWidget->item(OUTALL_ROW_VALUE_MEASURED_VALUE_U_TABLE, OUTALL_COLUMN_VALUE_MEASURED_VALUE_U_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(OUTALL_ROW_VALUE_MEASURED_VALUE_U_TABLE, OUTALL_COLUMN_VALUE_MEASURED_VALUE_U_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(OUTALL_ROW_VALUE_MEASURED_VALUE_U_TABLE, OUTALL_COLUMN_VALUE_MEASURED_VALUE_U_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
		// Measured value I
		mainTableWidget->model()->setData(mainTableWidget->model()->index(OUTALL_ROW_VALUE_MEASURED_VALUE_I_TABLE, OUTALL_COLUMN_VALUE_MEASURED_VALUE_I_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i)), measured->current == NOT_SET ? -1 : measured->current);
		mainTableWidget->item(OUTALL_ROW_VALUE_MEASURED_VALUE_I_TABLE, OUTALL_COLUMN_VALUE_MEASURED_VALUE_I_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(OUTALL_ROW_VALUE_MEASURED_VALUE_I_TABLE, OUTALL_COLUMN_VALUE_MEASURED_VALUE_I_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(OUTALL_ROW_VALUE_MEASURED_VALUE_I_TABLE, OUTALL_COLUMN_VALUE_MEASURED_VALUE_I_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
		// Progs U min
		mainTableWidget->model()->setData(mainTableWidget->model()->index(OUTALL_ROW_VALUE_TRESHHOLDERS_U_MIN_TABLE, OUTALL_COLUMN_VALUE_TRESHHOLDERS_U_MIN_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i)), measured->minVoltage == NOT_SET ? -1 : measured->minVoltage);
		mainTableWidget->item(OUTALL_ROW_VALUE_TRESHHOLDERS_U_MIN_TABLE, OUTALL_COLUMN_VALUE_TRESHHOLDERS_U_MIN_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(OUTALL_ROW_VALUE_TRESHHOLDERS_U_MIN_TABLE, OUTALL_COLUMN_VALUE_TRESHHOLDERS_U_MIN_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
		// Progs U max
		mainTableWidget->model()->setData(mainTableWidget->model()->index(OUTALL_ROW_VALUE_TRESHHOLDERS_U_MAX_TABLE, OUTALL_COLUMN_VALUE_TRESHHOLDERS_U_MAX_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i)), measured->maxVoltage == NOT_SET ? -1 : measured->maxVoltage);
		mainTableWidget->item(OUTALL_ROW_VALUE_TRESHHOLDERS_U_MAX_TABLE, OUTALL_COLUMN_VALUE_TRESHHOLDERS_U_MAX_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(OUTALL_ROW_VALUE_TRESHHOLDERS_U_MAX_TABLE, OUTALL_COLUMN_VALUE_TRESHHOLDERS_U_MAX_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
		// Progs I min
		mainTableWidget->model()->setData(mainTableWidget->model()->index(OUTALL_ROW_VALUE_TRESHHOLDERS_I_MIN_TABLE, OUTALL_COLUMN_VALUE_TRESHHOLDERS_I_MIN_TABL + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i)), measured->minCurrent == NOT_SET ? -1 : measured->minCurrent);
		mainTableWidget->item(OUTALL_ROW_VALUE_TRESHHOLDERS_I_MIN_TABLE, OUTALL_COLUMN_VALUE_TRESHHOLDERS_I_MIN_TABL + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(OUTALL_ROW_VALUE_TRESHHOLDERS_I_MIN_TABLE, OUTALL_COLUMN_VALUE_TRESHHOLDERS_I_MIN_TABL + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
		// Progs I max
		mainTableWidget->model()->setData(mainTableWidget->model()->index(OUTALL_ROW_VALUE_TRESHHOLDERS_I_MAX_TABLE, OUTALL_COLUMN_VALUE_TRESHHOLDERS_I_MAX_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i)), measured->maxCurrent == NOT_SET ? -1 : measured->maxCurrent);
		mainTableWidget->item(OUTALL_ROW_VALUE_TRESHHOLDERS_I_MAX_TABLE, OUTALL_COLUMN_VALUE_TRESHHOLDERS_I_MAX_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(OUTALL_ROW_VALUE_TRESHHOLDERS_I_MAX_TABLE, OUTALL_COLUMN_VALUE_TRESHHOLDERS_I_MAX_TABLE + OUTALL_OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
	}
	isAllInit = true;
}

MoreWindowOutDig::MoreWindowOutDig(TestTableRowProperties* row) : MoreWindowOut(row, SIZE_MOREWINDOW_OUT_DIG)
{
	coutTrehholders = 2;
	mainTableWidget->setRowCount(5);
	mainTableWidget->setColumnCount(5 + coutTrehholders * OUTALL_WIDTH_BLOCK_TRESHHOLDERS);

	initUiTableOut();
}

MoreWindowOutPwm::MoreWindowOutPwm(TestTableRowProperties* row) : MoreWindowOut(row, SIZE_MOREWINDOW_OUT_PWM)
{
	coutTrehholders = 2;
	mainTableWidget->setRowCount(5);
	mainTableWidget->setColumnCount(5 + coutTrehholders * OUTALL_WIDTH_BLOCK_TRESHHOLDERS);

	initUiTableOut();
}

MoreWindowOutVnh::MoreWindowOutVnh(TestTableRowProperties* row) : MoreWindowOut(row, SIZE_MOREWINDOW_OUT_VNH)
{
	coutTrehholders = 3;
	mainTableWidget->setRowCount(5);
	mainTableWidget->setColumnCount(5 + coutTrehholders * OUTALL_WIDTH_BLOCK_TRESHHOLDERS);

	initUiTableOut();
}

//void MoreWindowOut::on_mainTableWidget_cellChanged(int row, int column)
//{
//	if (row == 4 && isAllInit)
//	{
//		saveChangesButton->show();
//	}
//}

void MoreWindowIn::initUiTableIn(TypeMoreWindow typeWindow)
{
	switch (typeWindow)
	{
	case TypeMoreWindow::MoreWindowInDig:
		// Pad
		mainTableWidget->model()->setData(mainTableWidget->model()->index(INDIG_CELL_PAD_TABLE), "");
		mainTableWidget->item(INDIG_CELL_PAD_TABLE)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(INDIG_CELL_PAD_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(INDIG_CELL_PAD_TABLE)->setFont(*font);
		// Pin
		mainTableWidget->model()->setData(mainTableWidget->model()->index(INDIG_CELL_PIN_TABLE), "");
		mainTableWidget->item(INDIG_CELL_PIN_TABLE)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(INDIG_CELL_PIN_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(INDIG_CELL_PIN_TABLE)->setFont(*font);
		// Type
		mainTableWidget->model()->setData(mainTableWidget->model()->index(INDIG_CELL_TYPE_TABLE), "");
		mainTableWidget->item(INDIG_CELL_TYPE_TABLE)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(INDIG_CELL_TYPE_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(INDIG_CELL_TYPE_TABLE)->setFont(*font);
		// Name
		mainTableWidget->model()->setData(mainTableWidget->model()->index(INDIG_CELL_NAME_TABLE), "");
		mainTableWidget->item(INDIG_CELL_NAME_TABLE)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(INDIG_CELL_NAME_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(INDIG_CELL_NAME_TABLE)->setFont(*font);
		// First measured
		mainTableWidget->model()->setData(mainTableWidget->model()->index(INDIG_CELL_FIRST_MEASURED_TABLE), "");
		mainTableWidget->item(INDIG_CELL_FIRST_MEASURED_TABLE)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(INDIG_CELL_FIRST_MEASURED_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(INDIG_CELL_FIRST_MEASURED_TABLE)->setFont(*font);
		// Second measured
		mainTableWidget->model()->setData(mainTableWidget->model()->index(INDIG_CELL_SECOND_MEASURED_TABLE), "");
		mainTableWidget->item(INDIG_CELL_SECOND_MEASURED_TABLE)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(INDIG_CELL_SECOND_MEASURED_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(INDIG_CELL_SECOND_MEASURED_TABLE)->setFont(*font);
		// Comment
		mainTableWidget->model()->setData(mainTableWidget->model()->index(INDIG_CELL_COMMENT_TABLE), "");
		mainTableWidget->item(INDIG_CELL_COMMENT_TABLE)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(INDIG_CELL_COMMENT_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(INDIG_CELL_COMMENT_TABLE)->setFont(*font);

		switch (viewWindowState->appLanguage)
		{
		case RUSSIAN_LANG:
			mainTableWidget->item(INDIG_CELL_PAD_TABLE)->setText(QString::fromLocal8Bit("Колодка"));
			mainTableWidget->item(INDIG_CELL_PIN_TABLE)->setText(QString::fromLocal8Bit("Пин"));
			mainTableWidget->item(INDIG_CELL_TYPE_TABLE)->setText(QString::fromLocal8Bit("Тип"));
			mainTableWidget->item(INDIG_CELL_NAME_TABLE)->setText(QString::fromLocal8Bit("Название"));
			mainTableWidget->item(INDIG_CELL_FIRST_MEASURED_TABLE)->setText(QString::fromLocal8Bit("Измерение 1"));
			mainTableWidget->item(INDIG_CELL_SECOND_MEASURED_TABLE)->setText(QString::fromLocal8Bit("Измерение 2"));
			mainTableWidget->item(INDIG_CELL_COMMENT_TABLE)->setText(QString::fromLocal8Bit("Коментарий"));
			break;
		case ENGLISH_LANG:
			mainTableWidget->item(INDIG_CELL_PAD_TABLE)->setText("Pad");
			mainTableWidget->item(INDIG_CELL_PIN_TABLE)->setText("Pin");
			mainTableWidget->item(INDIG_CELL_TYPE_TABLE)->setText("Type");
			mainTableWidget->item(INDIG_CELL_NAME_TABLE)->setText("Name");
			mainTableWidget->item(INDIG_CELL_FIRST_MEASURED_TABLE)->setText("Measured 1");
			mainTableWidget->item(INDIG_CELL_SECOND_MEASURED_TABLE)->setText("Measured 2");
			mainTableWidget->item(INDIG_CELL_COMMENT_TABLE)->setText("Comments");
			break;
		}
		// Pad
		mainTableWidget->model()->setData(mainTableWidget->model()->index(INDIG_CEll_VALUE_PAD_TABLE), row->connectorStr);
		mainTableWidget->item(INDIG_CEll_VALUE_PAD_TABLE)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(INDIG_CEll_VALUE_PAD_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(INDIG_CEll_VALUE_PAD_TABLE)->setFont(*font);
		// Pin
		mainTableWidget->model()->setData(mainTableWidget->model()->index(INDIG_CELL_VALUE_PIN_TABLE), row->pin);
		mainTableWidget->item(INDIG_CELL_VALUE_PIN_TABLE)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(INDIG_CELL_VALUE_PIN_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(INDIG_CELL_VALUE_PIN_TABLE)->setFont(*font);
		// Type
		mainTableWidget->model()->setData(mainTableWidget->model()->index(INDIG_CELL_VALUE_TYPE_TABLE), row->type);
		mainTableWidget->item(INDIG_CELL_VALUE_TYPE_TABLE)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(INDIG_CELL_VALUE_TYPE_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(INDIG_CELL_VALUE_TYPE_TABLE)->setFont(*font);
		// Name
		mainTableWidget->model()->setData(mainTableWidget->model()->index(INDIG_CELL_VALUE_NAME_TABLE), row->name);
		mainTableWidget->item(INDIG_CELL_VALUE_NAME_TABLE)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(INDIG_CELL_VALUE_NAME_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(INDIG_CELL_VALUE_NAME_TABLE)->setFont(*font);
		// Comment
		commentTextEdit = new QTextEdit();
		commentTextEdit->setText(row->comment);
		connect(commentTextEdit, &QTextEdit::textChanged, this, &MoreWindow::on_commentTextEdit_textChanged);
		mainTableWidget->setCellWidget(INDIG_CELL_VALUE_COMMENT_TABLE, commentTextEdit);

		mainTableWidget->setColumnWidth(INDIG_COLUMN_PAD_TABLE, 65);
		mainTableWidget->setColumnWidth(INDIG_COLUMN_PIN_TABLE, 65);
		mainTableWidget->setColumnWidth(INDIG_COLUMN_TYPE_TABLE, 75);
		mainTableWidget->setColumnWidth(INDIG_COLUMN_NAME_TABLE, 150);
		mainTableWidget->setColumnWidth(INDIG_COLUMN_COMMENT_TABLE, 200);

		mainTableWidget->setRowHeight(0, 80);
		mainTableWidget->setRowHeight(1, 80);

		mainTableWidget->horizontalHeader()->setSectionResizeMode(INDIG_COLUMN_PAD_TABLE, QHeaderView::Fixed);
		mainTableWidget->horizontalHeader()->setSectionResizeMode(INDIG_COLUMN_PIN_TABLE, QHeaderView::Fixed);
		mainTableWidget->horizontalHeader()->setSectionResizeMode(INDIG_COLUMN_TYPE_TABLE, QHeaderView::Fixed);
		mainTableWidget->horizontalHeader()->setSectionResizeMode(INDIG_COLUMN_NAME_TABLE, QHeaderView::Stretch);
		mainTableWidget->horizontalHeader()->setSectionResizeMode(INDIG_COLUMN_COMMENT_TABLE, QHeaderView::Fixed);

		mainTableWidget->verticalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
		mainTableWidget->verticalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
		break;
	case TypeMoreWindow::MoreWindowInAnalog:
		// Pad
		mainTableWidget->setSpan(INANALOG_CELL_PAD_TABLE, 3, 1);
		mainTableWidget->model()->setData(mainTableWidget->model()->index(INANALOG_CELL_PAD_TABLE), "");
		mainTableWidget->item(INANALOG_CELL_PAD_TABLE)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(INANALOG_CELL_PAD_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(INANALOG_CELL_PAD_TABLE)->setFont(*font);
		// Pin
		mainTableWidget->setSpan(INANALOG_CELL_PIN_TABLE, 3, 1);
		mainTableWidget->model()->setData(mainTableWidget->model()->index(INANALOG_CELL_PIN_TABLE), "");
		mainTableWidget->item(INANALOG_CELL_PIN_TABLE)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(INANALOG_CELL_PIN_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(INANALOG_CELL_PIN_TABLE)->setFont(*font);
		// Type
		mainTableWidget->setSpan(INANALOG_CELL_TYPE_TABLE, 3, 1);
		mainTableWidget->model()->setData(mainTableWidget->model()->index(INANALOG_CELL_TYPE_TABLE), "");
		mainTableWidget->item(INANALOG_CELL_TYPE_TABLE)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(INANALOG_CELL_TYPE_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(INANALOG_CELL_TYPE_TABLE)->setFont(*font);
		// Name
		mainTableWidget->setSpan(INANALOG_CELL_NAME_TABLE, 3, 1);
		mainTableWidget->model()->setData(mainTableWidget->model()->index(INANALOG_CELL_NAME_TABLE), "");
		mainTableWidget->item(INANALOG_CELL_NAME_TABLE)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(INANALOG_CELL_NAME_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(INANALOG_CELL_NAME_TABLE)->setFont(*font);
		// Comment
		mainTableWidget->setSpan(INANALOG_ROW_COMMENT_TABLE, INANALOG_COLUMN_COMMENT_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders), 3, 1);
		mainTableWidget->model()->setData(mainTableWidget->model()->index(INANALOG_ROW_COMMENT_TABLE, INANALOG_COLUMN_COMMENT_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders)), "");
		mainTableWidget->item(INANALOG_ROW_COMMENT_TABLE, INANALOG_COLUMN_COMMENT_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders))->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(INANALOG_ROW_COMMENT_TABLE, INANALOG_COLUMN_COMMENT_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders))->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(INANALOG_ROW_COMMENT_TABLE, INANALOG_COLUMN_COMMENT_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders))->setFont(*font);

		switch (viewWindowState->appLanguage)
		{
		case RUSSIAN_LANG:
			mainTableWidget->item(INANALOG_CELL_PAD_TABLE)->setText(QString::fromLocal8Bit("Колодка"));
			mainTableWidget->item(INANALOG_CELL_PIN_TABLE)->setText(QString::fromLocal8Bit("Пин"));
			mainTableWidget->item(INANALOG_CELL_TYPE_TABLE)->setText(QString::fromLocal8Bit("Тип"));
			mainTableWidget->item(INANALOG_CELL_NAME_TABLE)->setText(QString::fromLocal8Bit("Название"));
			mainTableWidget->item(INANALOG_ROW_COMMENT_TABLE, INANALOG_COLUMN_COMMENT_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders))->setText(QString::fromLocal8Bit("Коментарий"));
			break;
		case ENGLISH_LANG:
			mainTableWidget->item(INANALOG_CELL_PAD_TABLE)->setText("Pad");
			mainTableWidget->item(INANALOG_CELL_PIN_TABLE)->setText("Pin");
			mainTableWidget->item(INANALOG_CELL_TYPE_TABLE)->setText("Type");
			mainTableWidget->item(INANALOG_CELL_NAME_TABLE)->setText("Name");
			mainTableWidget->item(INANALOG_ROW_COMMENT_TABLE, INANALOG_COLUMN_COMMENT_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders))->setText("Comments");
			break;
		}
		// Pad
		mainTableWidget->model()->setData(mainTableWidget->model()->index(INANALOG_CEll_VALUE_PAD_TABLE), row->connectorStr);
		mainTableWidget->item(INANALOG_CEll_VALUE_PAD_TABLE)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(INANALOG_CEll_VALUE_PAD_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(INANALOG_CEll_VALUE_PAD_TABLE)->setFont(*font);
		// Pin
		mainTableWidget->model()->setData(mainTableWidget->model()->index(INANALOG_CELL_VALUE_PIN_TABLE), row->pin);
		mainTableWidget->item(INANALOG_CELL_VALUE_PIN_TABLE)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(INANALOG_CELL_VALUE_PIN_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(INANALOG_CELL_VALUE_PIN_TABLE)->setFont(*font);
		// Type
		mainTableWidget->model()->setData(mainTableWidget->model()->index(INANALOG_CELL_VALUE_TYPE_TABLE), row->type);
		mainTableWidget->item(INANALOG_CELL_VALUE_TYPE_TABLE)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(INANALOG_CELL_VALUE_TYPE_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(INANALOG_CELL_VALUE_TYPE_TABLE)->setFont(*font);
		// Name
		mainTableWidget->model()->setData(mainTableWidget->model()->index(INANALOG_CELL_VALUE_NAME_TABLE), row->name);
		mainTableWidget->item(INANALOG_CELL_VALUE_NAME_TABLE)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(INANALOG_CELL_VALUE_NAME_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(INANALOG_CELL_VALUE_NAME_TABLE)->setFont(*font);
		// Comment
		commentTextEdit = new QTextEdit();
		commentTextEdit->setText(row->comment);
		connect(commentTextEdit, &QTextEdit::textChanged, this, &MoreWindow::on_commentTextEdit_textChanged);
		mainTableWidget->setCellWidget(INANALOG_ROW_VALUE_COMMENT_TABLE, INANALOG_COLUMN_VALUE_COMMENT_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders), commentTextEdit);

		mainTableWidget->setColumnWidth(INANALOG_COLUMN_PAD_TABLE, 65);
		mainTableWidget->setColumnWidth(INANALOG_COLUMN_PIN_TABLE, 65);
		mainTableWidget->setColumnWidth(INANALOG_COLUMN_TYPE_TABLE, 75);
		mainTableWidget->setColumnWidth(INANALOG_COLUMN_NAME_TABLE, 75);
		mainTableWidget->setColumnWidth(INANALOG_COLUMN_COMMENT_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders), 150);

		mainTableWidget->setRowHeight(0, 40);
		mainTableWidget->setRowHeight(1, 40);
		mainTableWidget->setRowHeight(2, 40);
		mainTableWidget->setRowHeight(3, 40);

		mainTableWidget->horizontalHeader()->setSectionResizeMode(INANALOG_COLUMN_PAD_TABLE, QHeaderView::Fixed);
		mainTableWidget->horizontalHeader()->setSectionResizeMode(INANALOG_COLUMN_PIN_TABLE, QHeaderView::Fixed);
		mainTableWidget->horizontalHeader()->setSectionResizeMode(INANALOG_COLUMN_TYPE_TABLE, QHeaderView::Fixed);
		mainTableWidget->horizontalHeader()->setSectionResizeMode(INANALOG_COLUMN_NAME_TABLE, QHeaderView::Stretch);
		mainTableWidget->horizontalHeader()->setSectionResizeMode(INANALOG_COLUMN_COMMENT_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders), QHeaderView::Fixed);

		mainTableWidget->verticalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
		mainTableWidget->verticalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
		mainTableWidget->verticalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
		mainTableWidget->verticalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);

		//Error
		if (coutTrehholders > row->measureds.size())
		{
			mainTableWidget->model()->setData(mainTableWidget->model()->index(0, 0), "Error");
			return;
		}

		for (int i = 0; i < coutTrehholders; i++)
		{
			// Name measured
			mainTableWidget->setSpan(INANALOG_ROW_NAME_MEASURED_TABLE, INANALOG_COLUMN_NAME_MEASURED_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i), 1, 3);
			mainTableWidget->model()->setData(mainTableWidget->model()->index(INANALOG_ROW_NAME_MEASURED_TABLE, INANALOG_COLUMN_NAME_MEASURED_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i)), "");
			mainTableWidget->item(INANALOG_ROW_NAME_MEASURED_TABLE, INANALOG_COLUMN_NAME_MEASURED_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
			mainTableWidget->item(INANALOG_ROW_NAME_MEASURED_TABLE, INANALOG_COLUMN_NAME_MEASURED_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
			mainTableWidget->item(INANALOG_ROW_NAME_MEASURED_TABLE, INANALOG_COLUMN_NAME_MEASURED_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
			// Measured value
			mainTableWidget->setSpan(INANALOG_ROW_MEASURED_VALUE_TABLE, INANALOG_COLUMN_MEASURED_VALUE_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i), 2, 1);
			mainTableWidget->model()->setData(mainTableWidget->model()->index(INANALOG_ROW_MEASURED_VALUE_TABLE, INANALOG_COLUMN_MEASURED_VALUE_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i)), "");
			mainTableWidget->item(INANALOG_ROW_MEASURED_VALUE_TABLE, INANALOG_COLUMN_MEASURED_VALUE_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
			mainTableWidget->item(INANALOG_ROW_MEASURED_VALUE_TABLE, INANALOG_COLUMN_MEASURED_VALUE_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
			mainTableWidget->item(INANALOG_ROW_MEASURED_VALUE_TABLE, INANALOG_COLUMN_MEASURED_VALUE_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
			// Progs
			mainTableWidget->setSpan(INANALOG_ROW_TRESHHOLDERS_TABLE, INANALOG_COLUMN_TRESHHOLDERS_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i), 1, 2);
			mainTableWidget->model()->setData(mainTableWidget->model()->index(INANALOG_ROW_TRESHHOLDERS_TABLE, INANALOG_COLUMN_TRESHHOLDERS_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i)), "");
			mainTableWidget->item(INANALOG_ROW_TRESHHOLDERS_TABLE, INANALOG_COLUMN_TRESHHOLDERS_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
			mainTableWidget->item(INANALOG_ROW_TRESHHOLDERS_TABLE, INANALOG_COLUMN_TRESHHOLDERS_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
			mainTableWidget->item(INANALOG_ROW_TRESHHOLDERS_TABLE, INANALOG_COLUMN_TRESHHOLDERS_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
			// Progs min
			mainTableWidget->model()->setData(mainTableWidget->model()->index(INANALOG_ROW_TRESHHOLDERS_MIN_TABLE, INANALOG_COLUMN_TRESHHOLDERS_MIN_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i)), "");
			mainTableWidget->item(INANALOG_ROW_TRESHHOLDERS_MIN_TABLE, INANALOG_COLUMN_TRESHHOLDERS_MIN_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
			mainTableWidget->item(INANALOG_ROW_TRESHHOLDERS_MIN_TABLE, INANALOG_COLUMN_TRESHHOLDERS_MIN_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
			mainTableWidget->item(INANALOG_ROW_TRESHHOLDERS_MIN_TABLE, INANALOG_COLUMN_TRESHHOLDERS_MIN_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
			// Progs max
			mainTableWidget->model()->setData(mainTableWidget->model()->index(INANALOG_ROW_TRESHHOLDERS_MAX_TABLE, INANALOG_COLUMN_TRESHHOLDERS_MAX_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i)), "");
			mainTableWidget->item(INANALOG_ROW_TRESHHOLDERS_MAX_TABLE, INANALOG_COLUMN_TRESHHOLDERS_MAX_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
			mainTableWidget->item(INANALOG_ROW_TRESHHOLDERS_MAX_TABLE, INANALOG_COLUMN_TRESHHOLDERS_MAX_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
			mainTableWidget->item(INANALOG_ROW_TRESHHOLDERS_MAX_TABLE, INANALOG_COLUMN_TRESHHOLDERS_MAX_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);

			mainTableWidget->setColumnWidth(INANALOG_COLUMN_MEASURED_VALUE_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i), 100);
			mainTableWidget->setColumnWidth(INANALOG_COLUMN_TRESHHOLDERS_MIN_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i), 50);
			mainTableWidget->setColumnWidth(INANALOG_COLUMN_TRESHHOLDERS_MAX_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i), 50);

			mainTableWidget->horizontalHeader()->setSectionResizeMode(INANALOG_COLUMN_MEASURED_VALUE_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i), QHeaderView::Fixed);
			mainTableWidget->horizontalHeader()->setSectionResizeMode(INANALOG_COLUMN_TRESHHOLDERS_MIN_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i), QHeaderView::Fixed);
			mainTableWidget->horizontalHeader()->setSectionResizeMode(INANALOG_COLUMN_TRESHHOLDERS_MAX_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i), QHeaderView::Fixed);

			switch (viewWindowState->appLanguage)
			{
			case RUSSIAN_LANG:
				mainTableWidget->item(INANALOG_ROW_NAME_MEASURED_TABLE, INANALOG_COLUMN_NAME_MEASURED_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i))->setText(QString::fromLocal8Bit("Измерение ") + QString::number(i + 1));
				mainTableWidget->item(INANALOG_ROW_MEASURED_VALUE_TABLE, INANALOG_COLUMN_MEASURED_VALUE_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i))->setText(QString::fromLocal8Bit("Измеренное\nзначенние"));
				mainTableWidget->item(INANALOG_ROW_TRESHHOLDERS_TABLE, INANALOG_COLUMN_TRESHHOLDERS_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i))->setText(QString::fromLocal8Bit("Пороги"));
				mainTableWidget->item(INANALOG_ROW_TRESHHOLDERS_MIN_TABLE, INANALOG_COLUMN_TRESHHOLDERS_MIN_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i))->setText(QString::fromLocal8Bit("Min"));
				mainTableWidget->item(INANALOG_ROW_TRESHHOLDERS_MAX_TABLE, INANALOG_COLUMN_TRESHHOLDERS_MAX_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i))->setText(QString::fromLocal8Bit("Max"));
				break;
			case ENGLISH_LANG:
				mainTableWidget->item(INANALOG_ROW_NAME_MEASURED_TABLE, INANALOG_COLUMN_NAME_MEASURED_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i))->setText("Measured " + QString::number(i + 1));
				mainTableWidget->item(INANALOG_ROW_MEASURED_VALUE_TABLE, INANALOG_COLUMN_MEASURED_VALUE_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i))->setText("Measured\nvalue");
				mainTableWidget->item(INANALOG_ROW_TRESHHOLDERS_TABLE, INANALOG_COLUMN_TRESHHOLDERS_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i))->setText("Progs");
				mainTableWidget->item(INANALOG_ROW_TRESHHOLDERS_MIN_TABLE, INANALOG_COLUMN_TRESHHOLDERS_MIN_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i))->setText("Min");
				mainTableWidget->item(INANALOG_ROW_TRESHHOLDERS_MAX_TABLE, INANALOG_COLUMN_TRESHHOLDERS_MAX_TABLE + INANALOG_OFFSET_BLOCK_TRESHHOLDERS(i))->setText("Max");
				break;
			}
		}
		break;
	}
	isAllInit = true;
}

MoreWindowInDig::MoreWindowInDig(TestTableRowProperties* row) : MoreWindowIn(row, SIZE_MOREWINDOW_IN_DIG)
{
	mainTableWidget->setRowCount(2);
	mainTableWidget->setColumnCount(7);
	initUiTableIn(TypeMoreWindow::MoreWindowInDig);
}

MoreWindowInAnalog::MoreWindowInAnalog(TestTableRowProperties* row) : MoreWindowIn(row, SIZE_MOREWINDOW_IN_ANALOG)
{
	coutTrehholders = 5;
	mainTableWidget->setColumnCount(11);
	mainTableWidget->setRowCount(4);

	initUiTableIn(TypeMoreWindow::MoreWindowInAnalog);
}

void MoreWindow::on_mainTableWidget_cellChanged(int row, int column)
{
	if (row == 4 && isAllInit)
	{
		saveChangesButton->show();
	}
}
void MoreWindow::on_commentTextEdit_textChanged()
{
	row->comment = commentTextEdit->toPlainText();
}

void MoreWindow::on_saveChangesButton_clicked()
{
	QMessageBox msgBox;
	msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Save);
	msgBox.setIcon(QMessageBox::Question);
	msgBox.setMinimumSize(200, 100);
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		msgBox.setText(QString::fromLocal8Bit("Сохранить изменения?"));
		msgBox.setInformativeText(QString::fromLocal8Bit("Изменения будут внесены в конфигурационный файл."));
		break;
	case ENGLISH_LANG:

		break;
	}
	if (msgBox.exec() == QMessageBox::Save)
	{
		Cable cableTmp;

		cableTmp.setId(row->id);
		cableTmp.setConnector(row->connectorInt);
		cableTmp.setPin(row->pin.toInt());

		if (row->direction == "OUT")
			cableTmp.setDirection(DIRECTION_OUT);
		else if (row->direction == "IN")
			cableTmp.setDirection(DIRECTION_IN);
		else
			cableTmp.setDirection(DIRECTION_NOT_SET);

		if (row->type == "DIGITAL")
			cableTmp.setType(TYPE_DIGITAL);
		else if (row->type == "ANALOG")
			cableTmp.setType(TYPE_ANALOG);
		else if (row->type == "HALL")
			cableTmp.setType(TYPE_HALL);
		else if (row->type == "PWM")
			cableTmp.setType(TYPE_PWM);
		else if (row->type == "VNH")
			cableTmp.setType(TYPE_VNH);
		else
			cableTmp.setType(TYPE_NOT_SET);

		cableTmp.setCanId(row->canId);
		cableTmp.setBit(row->bit);
		cableTmp.setName(row->name);
		cableTmp.setComponent(row->component);
		//cableTmp.minVoltage = (changedThresholds[0] != NOT_SET ? changedThresholds[0] : row->minVoltage);
		//cableTmp.maxVoltage = (changedThresholds[1] != NOT_SET ? changedThresholds[1] : row->maxVoltage);
		//cableTmp.minCurrent = (changedThresholds[2] != NOT_SET ? changedThresholds[2] : row->minCurrent);
		//cableTmp.maxCurrent = (changedThresholds[3] != NOT_SET ? changedThresholds[3] : row->maxCurrent);

		//row->minVoltage = (changedThresholds[0] != NOT_SET ? changedThresholds[0] : row->minVoltage);
		//row->maxVoltage = (changedThresholds[1] != NOT_SET ? changedThresholds[1] : row->maxVoltage);
		//row->minCurrent = (changedThresholds[2] != NOT_SET ? changedThresholds[2] : row->minCurrent);
		//row->maxCurrent = (changedThresholds[3] != NOT_SET ? changedThresholds[3] : row->maxCurrent);

		//changedThresholds[0] = NOT_SET;
		//changedThresholds[1] = NOT_SET;
		//changedThresholds[2] = NOT_SET;
		//changedThresholds[3] = NOT_SET;

		//resaveFile(row->testwindow->getFileName(), cableTmp);

		saveChangesButton->hide();
	}
}
/*
void MoreWindow::resaveFile(QString fileName, Cable newCable)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		//generateError(EMPTY_FILLING, Errors::Configurator::FILE_OPEN);
		return;
	}

	QString outputString = "";
	while (!file.atEnd())
	{
		QString dataLine = file.readLine();
		dataLine.remove("\n");
		QStringList dataList = dataLine.split(";");

		for (int i = CELL_MIN_CURRENT; i < dataList.size(); i++)
			if (dataList[0].toInt() == (int)newCable.connector && dataList[1].toInt() == newCable.pin)
			{
				switch (i)
				{
				case CELL_MIN_CURRENT:
					if (dataList[i].toInt() != newCable.minCurrent)
						dataList[i] = QString::number(newCable.minCurrent);
					break;

				case CELL_MAX_CURRENT:
					if (dataList[i].toInt() != newCable.maxCurrent)
						dataList[i] = QString::number(newCable.maxCurrent);
					break;

				case CELL_MIN_VOLTAGE:
					if (dataList[i].toInt() != newCable.minVoltage)
						dataList[i] = QString::number(newCable.minVoltage);
					break;

				case CELL_MAX_VOLTAGE:
					if (dataList[i].toInt() != newCable.maxVoltage)
						dataList[i] = QString::number(newCable.maxVoltage);
					break;
				}
			}

		QString outputLine = "";
		for (int i = 0; i < dataList.size(); i++)
			outputLine += dataList[i] + ";";
		outputString += outputLine.left(outputLine.lastIndexOf(";")) + "\n";
	}

	std::ofstream fout;
	fout.open(fileName.toStdString());
	fout << outputString.toStdString();
	fout.close();
}
*/
void MoreWindow::on_startTestButton_clicked()
{
	if (!row->testwindow->statusFlags->StatusTest)
		row->testwindow->ProcAutoTest((int)row->connectorInt, row->pin.toInt());
}