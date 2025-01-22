#include "MoreWindow.h"

#define CELL_MIN_CURRENT	6
#define CELL_MAX_CURRENT	7
#define CELL_MIN_VOLTAGE	8
#define CELL_MAX_VOLTAGE	9

MoreWindow::MoreWindow(TestTableRowProperties* row, int startWidth, int startHeight)
{
	this->row = row;
	this->coutTrehholders = NOT_SET;
	// переделать
	for (int i = 0; i < sizeof(changedThresholds) / sizeof(changedThresholds[0]); i++)
		changedThresholds[i] = NOT_SET;
	// переделать

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
	mainTableWidget->setSpan(CELL_PAD_TABLE, 4, 1);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_PAD_TABLE), "");
	mainTableWidget->item(CELL_PAD_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_PAD_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_PAD_TABLE)->setFont(*font);
	// Pin
	mainTableWidget->setSpan(CELL_PIN_TABLE, 4, 1);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_PIN_TABLE), "");
	mainTableWidget->item(CELL_PIN_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_PIN_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_PIN_TABLE)->setFont(*font);
	// Type
	mainTableWidget->setSpan(CELL_TYPE_TABLE, 4, 1);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_TYPE_TABLE), "");
	mainTableWidget->item(CELL_TYPE_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_TYPE_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_TYPE_TABLE)->setFont(*font);
	// Name
	mainTableWidget->setSpan(CELL_NAME_TABLE, 4, 1);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_NAME_TABLE), "");
	mainTableWidget->item(CELL_NAME_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_NAME_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_NAME_TABLE)->setFont(*font);
	// Comment
	mainTableWidget->setSpan(ROW_COMMENT_TABLE, COLUMN_COMMENT_TABLE + OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders), 4, 1);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(ROW_COMMENT_TABLE, COLUMN_COMMENT_TABLE + OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders)), "");
	mainTableWidget->item(ROW_COMMENT_TABLE, COLUMN_COMMENT_TABLE + OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders))->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(ROW_COMMENT_TABLE, COLUMN_COMMENT_TABLE + OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders))->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(ROW_COMMENT_TABLE, COLUMN_COMMENT_TABLE + OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders))->setFont(*font);

	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		mainTableWidget->item(CELL_PAD_TABLE)->setText(QString::fromLocal8Bit("Колодка"));
		mainTableWidget->item(CELL_PIN_TABLE)->setText(QString::fromLocal8Bit("Пин"));
		mainTableWidget->item(CELL_TYPE_TABLE)->setText(QString::fromLocal8Bit("Тип"));
		mainTableWidget->item(CELL_NAME_TABLE)->setText(QString::fromLocal8Bit("Название"));
		mainTableWidget->item(ROW_COMMENT_TABLE, COLUMN_COMMENT_TABLE + OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders))->setText(QString::fromLocal8Bit("Коментарий"));
		break;
	case ENGLISH_LANG:
		mainTableWidget->item(CELL_PAD_TABLE)->setText("Pad");
		mainTableWidget->item(CELL_PIN_TABLE)->setText("Pin");
		mainTableWidget->item(CELL_TYPE_TABLE)->setText("Type");
		mainTableWidget->item(CELL_NAME_TABLE)->setText("Name");
		mainTableWidget->item(ROW_COMMENT_TABLE, COLUMN_COMMENT_TABLE + OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders))->setText("Comments");
		break;
	}
	// Pad
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CEll_VALUE_PAD_TABLE), row->connectorStr);
	mainTableWidget->item(CEll_VALUE_PAD_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CEll_VALUE_PAD_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CEll_VALUE_PAD_TABLE)->setFont(*font);
	// Pin
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_VALUE_PIN_TABLE), row->pin);
	mainTableWidget->item(CELL_VALUE_PIN_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_VALUE_PIN_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_VALUE_PIN_TABLE)->setFont(*font);
	// Type
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_VALUE_TYPE_TABLE), row->type);
	mainTableWidget->item(CELL_VALUE_TYPE_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_VALUE_TYPE_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_VALUE_TYPE_TABLE)->setFont(*font);
	// Name
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_VALUE_NAME_TABLE), row->name);
	mainTableWidget->item(CELL_VALUE_NAME_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_VALUE_NAME_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_VALUE_NAME_TABLE)->setFont(*font);
	// Comment
	commentTextEdit = new QTextEdit();
	commentTextEdit->setText(row->comment);
	connect(commentTextEdit, &QTextEdit::textChanged, this, &MoreWindow::on_commentTextEdit_textChanged);
	mainTableWidget->setCellWidget(ROW_VALUE_COMMENT_TABLE, COLUMN_VALUE_COMMENT_TABLE + OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders), commentTextEdit);

	mainTableWidget->setColumnWidth(COLUMN_PAD_TABLE,	65);
	mainTableWidget->setColumnWidth(COLUMN_PIN_TABLE,	65);
	mainTableWidget->setColumnWidth(COLUMN_TYPE_TABLE,	75);
	mainTableWidget->setColumnWidth(ROW_NAME_TABLE,		75);
	mainTableWidget->setColumnWidth(COLUMN_COMMENT_TABLE + OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders), 150);

	mainTableWidget->setRowHeight(0, 40);
	mainTableWidget->setRowHeight(1, 40);
	mainTableWidget->setRowHeight(2, 40);
	mainTableWidget->setRowHeight(3, 40);
	
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_PAD_TABLE,	 QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_PIN_TABLE,	 QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_TYPE_TABLE, QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_NAME_TABLE, QHeaderView::Stretch);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_COMMENT_TABLE + OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders), QHeaderView::Fixed);

	mainTableWidget->verticalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
	mainTableWidget->verticalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
	mainTableWidget->verticalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
	mainTableWidget->verticalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
	mainTableWidget->verticalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);

	for (int i = 0; i < coutTrehholders; i++)
	{
		// Name measured
		mainTableWidget->setSpan(ROW_NAME_MEASURED_TABLE, COLUMN_NAME_MEASURED_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i), 1, 6);
		mainTableWidget->model()->setData(mainTableWidget->model()->index(ROW_NAME_MEASURED_TABLE, COLUMN_NAME_MEASURED_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i)), "");
		mainTableWidget->item(ROW_NAME_MEASURED_TABLE, COLUMN_NAME_MEASURED_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(ROW_NAME_MEASURED_TABLE, COLUMN_NAME_MEASURED_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(ROW_NAME_MEASURED_TABLE, COLUMN_NAME_MEASURED_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
		// Measured value
		mainTableWidget->setSpan(ROW_MEASURED_VALUE_TABLE, COLUMN_MEASURED_VALUE_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i), 2, 2);
		mainTableWidget->model()->setData(mainTableWidget->model()->index(ROW_MEASURED_VALUE_TABLE, COLUMN_MEASURED_VALUE_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i)), "");
		mainTableWidget->item(ROW_MEASURED_VALUE_TABLE, COLUMN_MEASURED_VALUE_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(ROW_MEASURED_VALUE_TABLE, COLUMN_MEASURED_VALUE_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(ROW_MEASURED_VALUE_TABLE, COLUMN_MEASURED_VALUE_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
		// Measured value U
		mainTableWidget->model()->setData(mainTableWidget->model()->index(ROW_MEASURED_VALUE_U_TABLE, COLUMN_MEASURED_VALUE_U_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i)), "");
		mainTableWidget->item(ROW_MEASURED_VALUE_U_TABLE, COLUMN_MEASURED_VALUE_U_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(ROW_MEASURED_VALUE_U_TABLE, COLUMN_MEASURED_VALUE_U_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(ROW_MEASURED_VALUE_U_TABLE, COLUMN_MEASURED_VALUE_U_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
		// Measured value I
		mainTableWidget->model()->setData(mainTableWidget->model()->index(ROW_MEASURED_VALUE_I_TABLE, COLUMN_MEASURED_VALUE_I_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i)), "");
		mainTableWidget->item(ROW_MEASURED_VALUE_I_TABLE, COLUMN_MEASURED_VALUE_I_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(ROW_MEASURED_VALUE_I_TABLE, COLUMN_MEASURED_VALUE_I_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(ROW_MEASURED_VALUE_I_TABLE, COLUMN_MEASURED_VALUE_I_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
		// Progs
		mainTableWidget->setSpan(ROW_TRESHHOLDERS_TABLE, COLUMN_TRESHHOLDERS_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i), 1, 4);
		mainTableWidget->model()->setData(mainTableWidget->model()->index(ROW_TRESHHOLDERS_TABLE, COLUMN_TRESHHOLDERS_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i)), "");
		mainTableWidget->item(ROW_TRESHHOLDERS_TABLE, COLUMN_TRESHHOLDERS_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(ROW_TRESHHOLDERS_TABLE, COLUMN_TRESHHOLDERS_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(ROW_TRESHHOLDERS_TABLE, COLUMN_TRESHHOLDERS_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
		// Progs U
		mainTableWidget->setSpan(ROW_TRESHHOLDERS_U_TABLE, COLUMN_TRESHHOLDERS_U_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i), 1, 2);
		mainTableWidget->model()->setData(mainTableWidget->model()->index(ROW_TRESHHOLDERS_U_TABLE, COLUMN_TRESHHOLDERS_U_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i)), "");
		mainTableWidget->item(ROW_TRESHHOLDERS_U_TABLE, COLUMN_TRESHHOLDERS_U_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(ROW_TRESHHOLDERS_U_TABLE, COLUMN_TRESHHOLDERS_U_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(ROW_TRESHHOLDERS_U_TABLE, COLUMN_TRESHHOLDERS_U_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
		// Progs I
		mainTableWidget->setSpan(ROW_TRESHHOLDERS_I_TABLE, COLUMN_TRESHHOLDERS_I_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i), 1, 2);
		mainTableWidget->model()->setData(mainTableWidget->model()->index(ROW_TRESHHOLDERS_I_TABLE, COLUMN_TRESHHOLDERS_I_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i)), "");
		mainTableWidget->item(ROW_TRESHHOLDERS_I_TABLE, COLUMN_TRESHHOLDERS_I_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(ROW_TRESHHOLDERS_I_TABLE, COLUMN_TRESHHOLDERS_I_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(ROW_TRESHHOLDERS_I_TABLE, COLUMN_TRESHHOLDERS_I_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
		// Progs U min
		mainTableWidget->model()->setData(mainTableWidget->model()->index(ROW_TRESHHOLDERS_U_MIN_TABLE, COLUMN_TRESHHOLDERS_U_MIN_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i)), "");
		mainTableWidget->item(ROW_TRESHHOLDERS_U_MIN_TABLE, COLUMN_TRESHHOLDERS_U_MIN_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(ROW_TRESHHOLDERS_U_MIN_TABLE, COLUMN_TRESHHOLDERS_U_MIN_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(ROW_TRESHHOLDERS_U_MIN_TABLE, COLUMN_TRESHHOLDERS_U_MIN_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
		// Progs U max
		mainTableWidget->model()->setData(mainTableWidget->model()->index(ROW_TRESHHOLDERS_U_MAX_TABLE, COLUMN_TRESHHOLDERS_U_MAX_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i)), "");
		mainTableWidget->item(ROW_TRESHHOLDERS_U_MAX_TABLE, COLUMN_TRESHHOLDERS_U_MAX_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(ROW_TRESHHOLDERS_U_MAX_TABLE, COLUMN_TRESHHOLDERS_U_MAX_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(ROW_TRESHHOLDERS_U_MAX_TABLE, COLUMN_TRESHHOLDERS_U_MAX_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
		// Progs I min
		mainTableWidget->model()->setData(mainTableWidget->model()->index(ROW_TRESHHOLDERS_I_MIN_TABLE, COLUMN_TRESHHOLDERS_I_MIN_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i)), "");
		mainTableWidget->item(ROW_TRESHHOLDERS_I_MIN_TABLE, COLUMN_TRESHHOLDERS_I_MIN_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(ROW_TRESHHOLDERS_I_MIN_TABLE, COLUMN_TRESHHOLDERS_I_MIN_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(ROW_TRESHHOLDERS_I_MIN_TABLE, COLUMN_TRESHHOLDERS_I_MIN_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
		// Progs I max
		mainTableWidget->model()->setData(mainTableWidget->model()->index(ROW_TRESHHOLDERS_I_MAX_TABLE, COLUMN_TRESHHOLDERS_I_MAX_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i)), "");
		mainTableWidget->item(ROW_TRESHHOLDERS_I_MAX_TABLE, COLUMN_TRESHHOLDERS_I_MAX_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(ROW_TRESHHOLDERS_I_MAX_TABLE, COLUMN_TRESHHOLDERS_I_MAX_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(ROW_TRESHHOLDERS_I_MAX_TABLE, COLUMN_TRESHHOLDERS_I_MAX_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);

		mainTableWidget->setColumnWidth(COLUMN_MEASURED_VALUE_U_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i),		50);
		mainTableWidget->setColumnWidth(COLUMN_MEASURED_VALUE_I_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i),		50);
		mainTableWidget->setColumnWidth(COLUMN_TRESHHOLDERS_U_MIN_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i),		50);
		mainTableWidget->setColumnWidth(COLUMN_TRESHHOLDERS_U_MAX_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i),		50);
		mainTableWidget->setColumnWidth(COLUMN_TRESHHOLDERS_I_MIN_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i),		50);
		mainTableWidget->setColumnWidth(COLUMN_TRESHHOLDERS_I_MAX_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i),		50);

		mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_TRESHHOLDERS_U_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i),		QHeaderView::Fixed);
		mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_TRESHHOLDERS_I_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i),		QHeaderView::Fixed);
		mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_TRESHHOLDERS_U_MIN_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i),	QHeaderView::Fixed);
		mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_TRESHHOLDERS_U_MAX_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i),	QHeaderView::Fixed);
		mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_TRESHHOLDERS_I_MIN_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i),	QHeaderView::Fixed);
		mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_TRESHHOLDERS_I_MAX_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i),	QHeaderView::Fixed);

		switch (viewWindowState->appLanguage)
		{
		case RUSSIAN_LANG:
			mainTableWidget->item(ROW_NAME_MEASURED_TABLE, COLUMN_NAME_MEASURED_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText(QString::fromLocal8Bit("Измерение ") + QString::number(i + 1));
			mainTableWidget->item(ROW_MEASURED_VALUE_TABLE, COLUMN_MEASURED_VALUE_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText(QString::fromLocal8Bit("Измеренное\nзначенние"));
			mainTableWidget->item(ROW_MEASURED_VALUE_U_TABLE, COLUMN_MEASURED_VALUE_U_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText(QString::fromLocal8Bit("U, В"));
			mainTableWidget->item(ROW_MEASURED_VALUE_I_TABLE, COLUMN_MEASURED_VALUE_I_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText(QString::fromLocal8Bit("I, А"));
			mainTableWidget->item(ROW_TRESHHOLDERS_TABLE, COLUMN_TRESHHOLDERS_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText(QString::fromLocal8Bit("Пороги"));
			mainTableWidget->item(ROW_TRESHHOLDERS_U_TABLE, COLUMN_TRESHHOLDERS_U_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText(QString::fromLocal8Bit("U, В"));
			mainTableWidget->item(ROW_TRESHHOLDERS_I_TABLE, COLUMN_TRESHHOLDERS_I_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText(QString::fromLocal8Bit("I, А"));
			mainTableWidget->item(ROW_TRESHHOLDERS_U_MIN_TABLE, COLUMN_TRESHHOLDERS_U_MIN_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText(QString::fromLocal8Bit("Мин."));
			mainTableWidget->item(ROW_TRESHHOLDERS_U_MAX_TABLE, COLUMN_TRESHHOLDERS_U_MAX_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText(QString::fromLocal8Bit("Макс."));
			mainTableWidget->item(ROW_TRESHHOLDERS_I_MIN_TABLE, COLUMN_TRESHHOLDERS_I_MIN_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText(QString::fromLocal8Bit("Мин."));
			mainTableWidget->item(ROW_TRESHHOLDERS_I_MAX_TABLE, COLUMN_TRESHHOLDERS_I_MAX_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText(QString::fromLocal8Bit("Макс."));
			break;
		case ENGLISH_LANG:
			mainTableWidget->item(ROW_NAME_MEASURED_TABLE, COLUMN_NAME_MEASURED_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText("Measured " + QString::number(i + 1));
			mainTableWidget->item(ROW_MEASURED_VALUE_TABLE, COLUMN_MEASURED_VALUE_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText("Measured\nvalue");
			mainTableWidget->item(ROW_MEASURED_VALUE_U_TABLE, COLUMN_MEASURED_VALUE_U_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText("U, V");
			mainTableWidget->item(ROW_MEASURED_VALUE_I_TABLE, COLUMN_MEASURED_VALUE_I_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText("I, A");
			mainTableWidget->item(ROW_TRESHHOLDERS_TABLE, COLUMN_TRESHHOLDERS_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText("Progs");
			mainTableWidget->item(ROW_TRESHHOLDERS_U_TABLE, COLUMN_TRESHHOLDERS_U_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText("U, V");
			mainTableWidget->item(ROW_TRESHHOLDERS_I_TABLE, COLUMN_TRESHHOLDERS_I_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText("I, A");
			mainTableWidget->item(ROW_TRESHHOLDERS_U_MIN_TABLE, COLUMN_TRESHHOLDERS_U_MIN_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText("Min.");
			mainTableWidget->item(ROW_TRESHHOLDERS_U_MAX_TABLE, COLUMN_TRESHHOLDERS_U_MAX_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText("Max.");
			mainTableWidget->item(ROW_TRESHHOLDERS_I_MIN_TABLE, COLUMN_TRESHHOLDERS_I_MIN_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText("Min.");
			mainTableWidget->item(ROW_TRESHHOLDERS_I_MAX_TABLE, COLUMN_TRESHHOLDERS_I_MAX_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText("Max.");
			break;
		}
	}
	
}

MoreWindowOutDig::MoreWindowOutDig(TestTableRowProperties* row) : MoreWindowOut(row, SIZE_MOREWINDOW_OUT_DIG)
{
	coutTrehholders = 2;
	mainTableWidget->setRowCount(5);
	mainTableWidget->setColumnCount(5 + coutTrehholders * WIDTH_BLOCK_TRESHHOLDERS);

	initUiTableOut();
}

MoreWindowOutPwm::MoreWindowOutPwm(TestTableRowProperties* row) : MoreWindowOut(row, SIZE_MOREWINDOW_OUT_PWM)
{
	coutTrehholders = 2;
	mainTableWidget->setRowCount(5);
	mainTableWidget->setColumnCount(5 + coutTrehholders * WIDTH_BLOCK_TRESHHOLDERS);

	initUiTableOut();
}

MoreWindowOutVnh::MoreWindowOutVnh(TestTableRowProperties* row) : MoreWindowOut(row, SIZE_MOREWINDOW_OUT_VNH)
{
	coutTrehholders = 3;
	mainTableWidget->setRowCount(5);
	mainTableWidget->setColumnCount(5 + coutTrehholders * WIDTH_BLOCK_TRESHHOLDERS);

	initUiTableOut();
}


void MoreWindowIn::initUiTableIn(TypeMoreWindow typeWindow)
{
	switch (typeWindow)
	{
	case TypeMoreWindow::MoreWindowInDig:
	
		break;
	case TypeMoreWindow::MoreWindowInAnalog:
		// Pad
		mainTableWidget->setSpan(CELL_PAD_TABLE, 4, 1);
		mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_PAD_TABLE), "");
		mainTableWidget->item(CELL_PAD_TABLE)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(CELL_PAD_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(CELL_PAD_TABLE)->setFont(*font);
		// Pin
		mainTableWidget->setSpan(CELL_PIN_TABLE, 4, 1);
		mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_PIN_TABLE), "");
		mainTableWidget->item(CELL_PIN_TABLE)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(CELL_PIN_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(CELL_PIN_TABLE)->setFont(*font);
		// Type
		mainTableWidget->setSpan(CELL_TYPE_TABLE, 4, 1);
		mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_TYPE_TABLE), "");
		mainTableWidget->item(CELL_TYPE_TABLE)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(CELL_TYPE_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(CELL_TYPE_TABLE)->setFont(*font);
		// Name
		mainTableWidget->setSpan(CELL_NAME_TABLE, 4, 1);
		mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_NAME_TABLE), "");
		mainTableWidget->item(CELL_NAME_TABLE)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(CELL_NAME_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(CELL_NAME_TABLE)->setFont(*font);
		// Comment
		mainTableWidget->setSpan(ROW_COMMENT_TABLE, COLUMN_COMMENT_TABLE + OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders), 4, 1);
		mainTableWidget->model()->setData(mainTableWidget->model()->index(ROW_COMMENT_TABLE, COLUMN_COMMENT_TABLE + OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders)), "");
		mainTableWidget->item(ROW_COMMENT_TABLE, COLUMN_COMMENT_TABLE + OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders))->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(ROW_COMMENT_TABLE, COLUMN_COMMENT_TABLE + OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders))->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(ROW_COMMENT_TABLE, COLUMN_COMMENT_TABLE + OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders))->setFont(*font);

		switch (viewWindowState->appLanguage)
		{
		case RUSSIAN_LANG:
			mainTableWidget->item(CELL_PAD_TABLE)->setText(QString::fromLocal8Bit("Колодка"));
			mainTableWidget->item(CELL_PIN_TABLE)->setText(QString::fromLocal8Bit("Пин"));
			mainTableWidget->item(CELL_TYPE_TABLE)->setText(QString::fromLocal8Bit("Тип"));
			mainTableWidget->item(CELL_NAME_TABLE)->setText(QString::fromLocal8Bit("Название"));
			mainTableWidget->item(ROW_COMMENT_TABLE, COLUMN_COMMENT_TABLE + OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders))->setText(QString::fromLocal8Bit("Коментарий"));
			break;
		case ENGLISH_LANG:
			mainTableWidget->item(CELL_PAD_TABLE)->setText("Pad");
			mainTableWidget->item(CELL_PIN_TABLE)->setText("Pin");
			mainTableWidget->item(CELL_TYPE_TABLE)->setText("Type");
			mainTableWidget->item(CELL_NAME_TABLE)->setText("Name");
			mainTableWidget->item(ROW_COMMENT_TABLE, COLUMN_COMMENT_TABLE + OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders))->setText("Comments");
			break;
		}
		// Pad
		mainTableWidget->model()->setData(mainTableWidget->model()->index(CEll_VALUE_PAD_TABLE), row->connectorStr);
		mainTableWidget->item(CEll_VALUE_PAD_TABLE)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(CEll_VALUE_PAD_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(CEll_VALUE_PAD_TABLE)->setFont(*font);
		// Pin
		mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_VALUE_PIN_TABLE), row->pin);
		mainTableWidget->item(CELL_VALUE_PIN_TABLE)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(CELL_VALUE_PIN_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(CELL_VALUE_PIN_TABLE)->setFont(*font);
		// Type
		mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_VALUE_TYPE_TABLE), row->type);
		mainTableWidget->item(CELL_VALUE_TYPE_TABLE)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(CELL_VALUE_TYPE_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(CELL_VALUE_TYPE_TABLE)->setFont(*font);
		// Name
		mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_VALUE_NAME_TABLE), row->name);
		mainTableWidget->item(CELL_VALUE_NAME_TABLE)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(CELL_VALUE_NAME_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(CELL_VALUE_NAME_TABLE)->setFont(*font);
		// Comment
		commentTextEdit = new QTextEdit();
		commentTextEdit->setText(row->comment);
		connect(commentTextEdit, &QTextEdit::textChanged, this, &MoreWindow::on_commentTextEdit_textChanged);
		mainTableWidget->setCellWidget(ROW_VALUE_COMMENT_TABLE, COLUMN_VALUE_COMMENT_TABLE + OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders), commentTextEdit);

		mainTableWidget->setColumnWidth(COLUMN_PAD_TABLE, 65);
		mainTableWidget->setColumnWidth(COLUMN_PIN_TABLE, 65);
		mainTableWidget->setColumnWidth(COLUMN_TYPE_TABLE, 75);
		mainTableWidget->setColumnWidth(ROW_NAME_TABLE, 75);
		mainTableWidget->setColumnWidth(COLUMN_COMMENT_TABLE + OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders), 150);

		mainTableWidget->setRowHeight(0, 40);
		mainTableWidget->setRowHeight(1, 40);
		mainTableWidget->setRowHeight(2, 40);
		mainTableWidget->setRowHeight(3, 40);

		mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_PAD_TABLE, QHeaderView::Fixed);
		mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_PIN_TABLE, QHeaderView::Fixed);
		mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_TYPE_TABLE, QHeaderView::Fixed);
		mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_NAME_TABLE, QHeaderView::Stretch);
		mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_COMMENT_TABLE + OFFSET_BLOCK_TRESHHOLDERS(coutTrehholders), QHeaderView::Fixed);

		mainTableWidget->verticalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
		mainTableWidget->verticalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
		mainTableWidget->verticalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
		mainTableWidget->verticalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
		mainTableWidget->verticalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);

		for (int i = 0; i < coutTrehholders; i++)
		{
			// Name measured
			mainTableWidget->setSpan(ROW_NAME_MEASURED_TABLE, COLUMN_NAME_MEASURED_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i), 1, 6);
			mainTableWidget->model()->setData(mainTableWidget->model()->index(ROW_NAME_MEASURED_TABLE, COLUMN_NAME_MEASURED_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i)), "");
			mainTableWidget->item(ROW_NAME_MEASURED_TABLE, COLUMN_NAME_MEASURED_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
			mainTableWidget->item(ROW_NAME_MEASURED_TABLE, COLUMN_NAME_MEASURED_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
			mainTableWidget->item(ROW_NAME_MEASURED_TABLE, COLUMN_NAME_MEASURED_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
			// Measured value
			mainTableWidget->setSpan(ROW_MEASURED_VALUE_TABLE, COLUMN_MEASURED_VALUE_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i), 2, 2);
			mainTableWidget->model()->setData(mainTableWidget->model()->index(ROW_MEASURED_VALUE_TABLE, COLUMN_MEASURED_VALUE_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i)), "");
			mainTableWidget->item(ROW_MEASURED_VALUE_TABLE, COLUMN_MEASURED_VALUE_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
			mainTableWidget->item(ROW_MEASURED_VALUE_TABLE, COLUMN_MEASURED_VALUE_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
			mainTableWidget->item(ROW_MEASURED_VALUE_TABLE, COLUMN_MEASURED_VALUE_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
			// Measured value U
			mainTableWidget->model()->setData(mainTableWidget->model()->index(ROW_MEASURED_VALUE_U_TABLE, COLUMN_MEASURED_VALUE_U_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i)), "");
			mainTableWidget->item(ROW_MEASURED_VALUE_U_TABLE, COLUMN_MEASURED_VALUE_U_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
			mainTableWidget->item(ROW_MEASURED_VALUE_U_TABLE, COLUMN_MEASURED_VALUE_U_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
			mainTableWidget->item(ROW_MEASURED_VALUE_U_TABLE, COLUMN_MEASURED_VALUE_U_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
			// Measured value I
			mainTableWidget->model()->setData(mainTableWidget->model()->index(ROW_MEASURED_VALUE_I_TABLE, COLUMN_MEASURED_VALUE_I_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i)), "");
			mainTableWidget->item(ROW_MEASURED_VALUE_I_TABLE, COLUMN_MEASURED_VALUE_I_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
			mainTableWidget->item(ROW_MEASURED_VALUE_I_TABLE, COLUMN_MEASURED_VALUE_I_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
			mainTableWidget->item(ROW_MEASURED_VALUE_I_TABLE, COLUMN_MEASURED_VALUE_I_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
			// Progs
			mainTableWidget->setSpan(ROW_TRESHHOLDERS_TABLE, COLUMN_TRESHHOLDERS_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i), 1, 4);
			mainTableWidget->model()->setData(mainTableWidget->model()->index(ROW_TRESHHOLDERS_TABLE, COLUMN_TRESHHOLDERS_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i)), "");
			mainTableWidget->item(ROW_TRESHHOLDERS_TABLE, COLUMN_TRESHHOLDERS_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
			mainTableWidget->item(ROW_TRESHHOLDERS_TABLE, COLUMN_TRESHHOLDERS_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
			mainTableWidget->item(ROW_TRESHHOLDERS_TABLE, COLUMN_TRESHHOLDERS_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
			// Progs U
			mainTableWidget->setSpan(ROW_TRESHHOLDERS_U_TABLE, COLUMN_TRESHHOLDERS_U_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i), 1, 2);
			mainTableWidget->model()->setData(mainTableWidget->model()->index(ROW_TRESHHOLDERS_U_TABLE, COLUMN_TRESHHOLDERS_U_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i)), "");
			mainTableWidget->item(ROW_TRESHHOLDERS_U_TABLE, COLUMN_TRESHHOLDERS_U_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
			mainTableWidget->item(ROW_TRESHHOLDERS_U_TABLE, COLUMN_TRESHHOLDERS_U_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
			mainTableWidget->item(ROW_TRESHHOLDERS_U_TABLE, COLUMN_TRESHHOLDERS_U_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
			// Progs I
			mainTableWidget->setSpan(ROW_TRESHHOLDERS_I_TABLE, COLUMN_TRESHHOLDERS_I_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i), 1, 2);
			mainTableWidget->model()->setData(mainTableWidget->model()->index(ROW_TRESHHOLDERS_I_TABLE, COLUMN_TRESHHOLDERS_I_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i)), "");
			mainTableWidget->item(ROW_TRESHHOLDERS_I_TABLE, COLUMN_TRESHHOLDERS_I_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
			mainTableWidget->item(ROW_TRESHHOLDERS_I_TABLE, COLUMN_TRESHHOLDERS_I_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
			mainTableWidget->item(ROW_TRESHHOLDERS_I_TABLE, COLUMN_TRESHHOLDERS_I_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
			// Progs U min
			mainTableWidget->model()->setData(mainTableWidget->model()->index(ROW_TRESHHOLDERS_U_MIN_TABLE, COLUMN_TRESHHOLDERS_U_MIN_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i)), "");
			mainTableWidget->item(ROW_TRESHHOLDERS_U_MIN_TABLE, COLUMN_TRESHHOLDERS_U_MIN_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
			mainTableWidget->item(ROW_TRESHHOLDERS_U_MIN_TABLE, COLUMN_TRESHHOLDERS_U_MIN_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
			mainTableWidget->item(ROW_TRESHHOLDERS_U_MIN_TABLE, COLUMN_TRESHHOLDERS_U_MIN_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
			// Progs U max
			mainTableWidget->model()->setData(mainTableWidget->model()->index(ROW_TRESHHOLDERS_U_MAX_TABLE, COLUMN_TRESHHOLDERS_U_MAX_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i)), "");
			mainTableWidget->item(ROW_TRESHHOLDERS_U_MAX_TABLE, COLUMN_TRESHHOLDERS_U_MAX_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
			mainTableWidget->item(ROW_TRESHHOLDERS_U_MAX_TABLE, COLUMN_TRESHHOLDERS_U_MAX_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
			mainTableWidget->item(ROW_TRESHHOLDERS_U_MAX_TABLE, COLUMN_TRESHHOLDERS_U_MAX_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
			// Progs I min
			mainTableWidget->model()->setData(mainTableWidget->model()->index(ROW_TRESHHOLDERS_I_MIN_TABLE, COLUMN_TRESHHOLDERS_I_MIN_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i)), "");
			mainTableWidget->item(ROW_TRESHHOLDERS_I_MIN_TABLE, COLUMN_TRESHHOLDERS_I_MIN_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
			mainTableWidget->item(ROW_TRESHHOLDERS_I_MIN_TABLE, COLUMN_TRESHHOLDERS_I_MIN_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
			mainTableWidget->item(ROW_TRESHHOLDERS_I_MIN_TABLE, COLUMN_TRESHHOLDERS_I_MIN_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);
			// Progs I max
			mainTableWidget->model()->setData(mainTableWidget->model()->index(ROW_TRESHHOLDERS_I_MAX_TABLE, COLUMN_TRESHHOLDERS_I_MAX_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i)), "");
			mainTableWidget->item(ROW_TRESHHOLDERS_I_MAX_TABLE, COLUMN_TRESHHOLDERS_I_MAX_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setTextAlignment(Qt::AlignCenter);
			mainTableWidget->item(ROW_TRESHHOLDERS_I_MAX_TABLE, COLUMN_TRESHHOLDERS_I_MAX_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFlags(Qt::ItemIsSelectable);
			mainTableWidget->item(ROW_TRESHHOLDERS_I_MAX_TABLE, COLUMN_TRESHHOLDERS_I_MAX_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setFont(*font);

			mainTableWidget->setColumnWidth(COLUMN_MEASURED_VALUE_U_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i), 50);
			mainTableWidget->setColumnWidth(COLUMN_MEASURED_VALUE_I_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i), 50);
			mainTableWidget->setColumnWidth(COLUMN_TRESHHOLDERS_U_MIN_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i), 50);
			mainTableWidget->setColumnWidth(COLUMN_TRESHHOLDERS_U_MAX_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i), 50);
			mainTableWidget->setColumnWidth(COLUMN_TRESHHOLDERS_I_MIN_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i), 50);
			mainTableWidget->setColumnWidth(COLUMN_TRESHHOLDERS_I_MAX_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i), 50);

			mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_TRESHHOLDERS_U_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i), QHeaderView::Fixed);
			mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_TRESHHOLDERS_I_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i), QHeaderView::Fixed);
			mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_TRESHHOLDERS_U_MIN_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i), QHeaderView::Fixed);
			mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_TRESHHOLDERS_U_MAX_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i), QHeaderView::Fixed);
			mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_TRESHHOLDERS_I_MIN_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i), QHeaderView::Fixed);
			mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_TRESHHOLDERS_I_MAX_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i), QHeaderView::Fixed);

			switch (viewWindowState->appLanguage)
			{
			case RUSSIAN_LANG:
				mainTableWidget->item(ROW_NAME_MEASURED_TABLE, COLUMN_NAME_MEASURED_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText(QString::fromLocal8Bit("Измерение ") + QString::number(i + 1));
				mainTableWidget->item(ROW_MEASURED_VALUE_TABLE, COLUMN_MEASURED_VALUE_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText(QString::fromLocal8Bit("Измеренное\nзначенние"));
				mainTableWidget->item(ROW_MEASURED_VALUE_U_TABLE, COLUMN_MEASURED_VALUE_U_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText(QString::fromLocal8Bit("U, В"));
				mainTableWidget->item(ROW_MEASURED_VALUE_I_TABLE, COLUMN_MEASURED_VALUE_I_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText(QString::fromLocal8Bit("I, А"));
				mainTableWidget->item(ROW_TRESHHOLDERS_TABLE, COLUMN_TRESHHOLDERS_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText(QString::fromLocal8Bit("Пороги"));
				mainTableWidget->item(ROW_TRESHHOLDERS_U_TABLE, COLUMN_TRESHHOLDERS_U_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText(QString::fromLocal8Bit("U, В"));
				mainTableWidget->item(ROW_TRESHHOLDERS_I_TABLE, COLUMN_TRESHHOLDERS_I_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText(QString::fromLocal8Bit("I, А"));
				mainTableWidget->item(ROW_TRESHHOLDERS_U_MIN_TABLE, COLUMN_TRESHHOLDERS_U_MIN_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText(QString::fromLocal8Bit("Мин."));
				mainTableWidget->item(ROW_TRESHHOLDERS_U_MAX_TABLE, COLUMN_TRESHHOLDERS_U_MAX_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText(QString::fromLocal8Bit("Макс."));
				mainTableWidget->item(ROW_TRESHHOLDERS_I_MIN_TABLE, COLUMN_TRESHHOLDERS_I_MIN_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText(QString::fromLocal8Bit("Мин."));
				mainTableWidget->item(ROW_TRESHHOLDERS_I_MAX_TABLE, COLUMN_TRESHHOLDERS_I_MAX_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText(QString::fromLocal8Bit("Макс."));
				break;
			case ENGLISH_LANG:
				mainTableWidget->item(ROW_NAME_MEASURED_TABLE, COLUMN_NAME_MEASURED_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText("Measured " + QString::number(i + 1));
				mainTableWidget->item(ROW_MEASURED_VALUE_TABLE, COLUMN_MEASURED_VALUE_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText("Measured\nvalue");
				mainTableWidget->item(ROW_MEASURED_VALUE_U_TABLE, COLUMN_MEASURED_VALUE_U_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText("U, V");
				mainTableWidget->item(ROW_MEASURED_VALUE_I_TABLE, COLUMN_MEASURED_VALUE_I_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText("I, A");
				mainTableWidget->item(ROW_TRESHHOLDERS_TABLE, COLUMN_TRESHHOLDERS_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText("Progs");
				mainTableWidget->item(ROW_TRESHHOLDERS_U_TABLE, COLUMN_TRESHHOLDERS_U_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText("U, V");
				mainTableWidget->item(ROW_TRESHHOLDERS_I_TABLE, COLUMN_TRESHHOLDERS_I_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText("I, A");
				mainTableWidget->item(ROW_TRESHHOLDERS_U_MIN_TABLE, COLUMN_TRESHHOLDERS_U_MIN_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText("Min.");
				mainTableWidget->item(ROW_TRESHHOLDERS_U_MAX_TABLE, COLUMN_TRESHHOLDERS_U_MAX_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText("Max.");
				mainTableWidget->item(ROW_TRESHHOLDERS_I_MIN_TABLE, COLUMN_TRESHHOLDERS_I_MIN_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText("Min.");
				mainTableWidget->item(ROW_TRESHHOLDERS_I_MAX_TABLE, COLUMN_TRESHHOLDERS_I_MAX_TABLE + OFFSET_BLOCK_TRESHHOLDERS(i))->setText("Max.");
				break;
			}
		}
		break;
	}
}

MoreWindowInDig::MoreWindowInDig(TestTableRowProperties* row) : MoreWindowIn(row, SIZE_MOREWINDOW_IN_DIG)
{
	initUiTableIn(TypeMoreWindow::MoreWindowInDig);
}

MoreWindowInAnalog::MoreWindowInAnalog(TestTableRowProperties* row) : MoreWindowIn(row, SIZE_MOREWINDOW_IN_ANALOG)
{
	coutTrehholders = 2;
	mainTableWidget->setRowCount(5);
	mainTableWidget->setColumnCount(5 + coutTrehholders * WIDTH_BLOCK_TRESHHOLDERS);

	initUiTableIn(TypeMoreWindow::MoreWindowInAnalog);

}

/*
void MoreWindow::setValueProgs()
{
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_VALUE_MEASURED_VALUE_U_TABLE), (row->measured.voltage != NOT_SET ? QString::number(row->measured.voltage) : "-"));
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_VALUE_MEASURED_VALUE_I_TABLE), (row->measured.current != NOT_SET ? QString::number(row->measured.current) : "-"));

	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_VALUE_PROGS_U_MIN_TABLE), row->minVoltage);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_VALUE_PROGS_U_MAX_TABLE), row->maxVoltage);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_VALUE_PROGS_I_MIN_TABLE), row->minCurrent);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_VALUE_PROGS_I_MAX_TABLE), row->maxCurrent);

	if (row->measured.voltage != NOT_SET)
	{
		// Красим пороги и измеренное значение
		if (row->minVoltage > row->measured.voltage)
		{
			mainTableWidget->item(CELL_VALUE_PROGS_U_MIN_TABLE)->setBackgroundColor(Qt::red);
			mainTableWidget->item(CELL_VALUE_MEASURED_VALUE_U_TABLE)->setBackgroundColor(Qt::red);
		}
		else if (row->measured.voltage > row->maxVoltage)
		{
			mainTableWidget->item(CELL_VALUE_PROGS_U_MAX_TABLE)->setBackgroundColor(Qt::red);
			mainTableWidget->item(CELL_VALUE_MEASURED_VALUE_U_TABLE)->setBackgroundColor(Qt::red);
		}
		if (row->minCurrent > row->measured.current)
		{
			mainTableWidget->item(CELL_VALUE_PROGS_I_MIN_TABLE)->setBackgroundColor(Qt::red);
			mainTableWidget->item(CELL_VALUE_MEASURED_VALUE_I_TABLE)->setBackgroundColor(Qt::red);
		}
		else if (row->measured.current > row->maxCurrent)
		{
			mainTableWidget->item(CELL_VALUE_PROGS_I_MAX_TABLE)->setBackgroundColor(Qt::red);
			mainTableWidget->item(CELL_VALUE_MEASURED_VALUE_I_TABLE)->setBackgroundColor(Qt::red);
		}
	}
}
*/
/*
void MoreWindow::initUiSetValueTable()
{
	if (row->direction == "IN" && row->type == "DIGITAL")
	{

	}
	else
	{
		QString typeTmp;
		switch (viewWindowState->appLanguage)
		{
		case RUSSIAN_LANG:
			mainTableWidget->item(CELL_PAD_TABLE)->setText(QString::fromLocal8Bit("Колодка"));
			mainTableWidget->item(CELL_PIN_TABLE)->setText(QString::fromLocal8Bit("Пин"));
			mainTableWidget->item(CELL_TYPE_TABLE)->setText(QString::fromLocal8Bit("Тип"));
			mainTableWidget->item(CELL_NAME_TABLE)->setText(QString::fromLocal8Bit("Название"));
			mainTableWidget->item(CELL_MEASURED_VALUE_TABLE)->setText(QString::fromLocal8Bit("Измеренное\nзначенние"));
			mainTableWidget->item(CELL_MEASURED_VALUE_U_TABLE)->setText(QString::fromLocal8Bit("U, В"));
			mainTableWidget->item(CELL_MEASURED_VALUE_I_TABLE)->setText(QString::fromLocal8Bit("I, А"));
			mainTableWidget->item(CELL_TRESHHOLDERS_TABLE)->setText(QString::fromLocal8Bit("Пороги"));
			mainTableWidget->item(CELL_TRESHHOLDERS_U_TABLE)->setText(QString::fromLocal8Bit("U, В"));
			mainTableWidget->item(CELL_TRESHHOLDERS_I_TABLE)->setText(QString::fromLocal8Bit("I, А"));
			mainTableWidget->item(CELL_TRESHHOLDERS_U_MIN_TABLE)->setText(QString::fromLocal8Bit("Мин."));
			mainTableWidget->item(CELL_TRESHHOLDERS_U_MAX_TABLE)->setText(QString::fromLocal8Bit("Макс."));
			mainTableWidget->item(CELL_TRESHHOLDERS_I_MIN_TABLE)->setText(QString::fromLocal8Bit("Мин."));
			mainTableWidget->item(CELL_TRESHHOLDERS_I_MAX_TABLE)->setText(QString::fromLocal8Bit("Макс."));
			mainTableWidget->item(CELL_COMMENT_TABLE)->setText(QString::fromLocal8Bit("Коментарий"));

			if (row->type == "ANALOG")
				typeTmp = QString::fromLocal8Bit("Аналоговый");
			else if (row->type == "DIGITAL")
				typeTmp = QString::fromLocal8Bit("Цифровой");
			else if (row->type == "HALL")
				typeTmp = QString::fromLocal8Bit("HALL");
			else if (row->type == "PWM")
				typeTmp = QString::fromLocal8Bit("ШИМ");
			else if (row->type == "VNH")
				typeTmp = QString::fromLocal8Bit("VNH");
			else
				typeTmp = QString::fromLocal8Bit("Ошибка");

			mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_VALUE_TYPE_TABLE), typeTmp);
			break;

		case ENGLISH_LANG:
			mainTableWidget->item(CELL_PAD_TABLE)->setText("Pad");
			mainTableWidget->item(CELL_PIN_TABLE)->setText("Pin");
			mainTableWidget->item(CELL_TYPE_TABLE)->setText("Type");
			mainTableWidget->item(CELL_NAME_TABLE)->setText("Name");
			mainTableWidget->item(CELL_MEASURED_VALUE_TABLE)->setText("Measured\nvalue");
			mainTableWidget->item(CELL_MEASURED_VALUE_U_TABLE)->setText("U, V");
			mainTableWidget->item(CELL_MEASURED_VALUE_I_TABLE)->setText("I, A");
			mainTableWidget->item(CELL_TRESHHOLDERS_TABLE)->setText("Progs");
			mainTableWidget->item(CELL_TRESHHOLDERS_U_TABLE)->setText("U, V");
			mainTableWidget->item(CELL_TRESHHOLDERS_I_TABLE)->setText("I, A");
			mainTableWidget->item(CELL_TRESHHOLDERS_U_MIN_TABLE)->setText("Min.");
			mainTableWidget->item(CELL_TRESHHOLDERS_U_MAX_TABLE)->setText("Max.");
			mainTableWidget->item(CELL_TRESHHOLDERS_I_MIN_TABLE)->setText("Min.");
			mainTableWidget->item(CELL_TRESHHOLDERS_I_MAX_TABLE)->setText("Max.");
			mainTableWidget->item(CELL_COMMENT_TABLE)->setText("Comments");

			if (row->type == "ANALOG")
				typeTmp = "Analog";
			else if (row->type == "DIGITAL")
				typeTmp = "Digital";
			else if (row->type == "HALL")
				typeTmp = "HALL";
			else if (row->type == "PWM")
				typeTmp = "PWM";
			else if (row->type == "VNH")
				typeTmp = "VNH";
			else
				typeTmp = "Error";

			mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_VALUE_TYPE_TABLE), typeTmp);
			break;
		}

		mainTableWidget->model()->setData(mainTableWidget->model()->index(CEll_VALUE_PAD_TABLE), row->connectorStr);
		mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_VALUE_PIN_TABLE), row->pin);
		mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_VALUE_NAME_TABLE), row->name);
		setValueProgs();



		commentTextEdit = new QTextEdit();
		commentTextEdit->setText(row->comment);
		connect(commentTextEdit, &QTextEdit::textChanged, this, &MoreWindow::on_commentTextEdit_textChanged);
		mainTableWidget->setCellWidget(CELL_VALUE_COMMENT_TABLE, commentTextEdit);
	}
}
*/

/*
void MoreWindow::initUiGenerateTable()
{

	if (row->direction == "IN" && row->type == "DIGITAL")
	{

	}
	else
	{

		mainTableWidget->setRowCount(4);
		mainTableWidget->setColumnCount(11);

		QFont font = QFont();
		font.setBold(true);
		font.setPointSizeF(10);
		// Pad
		mainTableWidget->setSpan(CELL_PAD_TABLE, 3, 1);
		mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_PAD_TABLE), "");
		mainTableWidget->item(CELL_PAD_TABLE)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(CELL_PAD_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(CELL_PAD_TABLE)->setFont(font);
		// Pin
		mainTableWidget->setSpan(CELL_PIN_TABLE, 3, 1);
		mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_PIN_TABLE), "");
		mainTableWidget->item(CELL_PIN_TABLE)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(CELL_PIN_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(CELL_PIN_TABLE)->setFont(font);
		// Type
		mainTableWidget->setSpan(CELL_TYPE_TABLE, 3, 1);
		mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_TYPE_TABLE), "");
		mainTableWidget->item(CELL_TYPE_TABLE)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(CELL_TYPE_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(CELL_TYPE_TABLE)->setFont(font);
		// Name
		mainTableWidget->setSpan(CELL_NAME_TABLE, 3, 1);
		mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_NAME_TABLE), "");
		mainTableWidget->item(CELL_NAME_TABLE)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(CELL_NAME_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(CELL_NAME_TABLE)->setFont(font);
		// Measured value
		mainTableWidget->setSpan(CELL_MEASURED_VALUE_TABLE, 2, 2);
		mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_MEASURED_VALUE_TABLE), "");
		mainTableWidget->item(CELL_MEASURED_VALUE_TABLE)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(CELL_MEASURED_VALUE_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(CELL_MEASURED_VALUE_TABLE)->setFont(font);
		// Measured value U
		mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_MEASURED_VALUE_U_TABLE), "");
		mainTableWidget->item(CELL_MEASURED_VALUE_U_TABLE)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(CELL_MEASURED_VALUE_U_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(CELL_MEASURED_VALUE_U_TABLE)->setFont(font);
		// Measured value I
		mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_MEASURED_VALUE_I_TABLE), "");
		mainTableWidget->item(CELL_MEASURED_VALUE_I_TABLE)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(CELL_MEASURED_VALUE_I_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(CELL_MEASURED_VALUE_I_TABLE)->setFont(font);
		// Progs
		mainTableWidget->setSpan(CELL_TRESHHOLDERS_TABLE, 1, 4);
		mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_TRESHHOLDERS_TABLE), "");
		mainTableWidget->item(CELL_TRESHHOLDERS_TABLE)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(CELL_TRESHHOLDERS_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(CELL_TRESHHOLDERS_TABLE)->setFont(font);
		// Progs U
		mainTableWidget->setSpan(CELL_TRESHHOLDERS_U_TABLE, 1, 2);
		mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_TRESHHOLDERS_U_TABLE), "");
		mainTableWidget->item(CELL_TRESHHOLDERS_U_TABLE)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(CELL_TRESHHOLDERS_U_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(CELL_TRESHHOLDERS_U_TABLE)->setFont(font);
		// Progs I
		mainTableWidget->setSpan(CELL_TRESHHOLDERS_I_TABLE, 1, 2);
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
		// Comment
		mainTableWidget->setSpan(CELL_COMMENT_TABLE, 3, 1);
		mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_COMMENT_TABLE), "");
		mainTableWidget->item(CELL_COMMENT_TABLE)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(CELL_COMMENT_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(CELL_COMMENT_TABLE)->setFont(font);

		mainTableWidget->setColumnWidth(0, 65);
		mainTableWidget->setColumnWidth(1, 65);
		mainTableWidget->setColumnWidth(2, 75);
		mainTableWidget->setColumnWidth(3, 65);
		mainTableWidget->setColumnWidth(4, 65);
		mainTableWidget->setColumnWidth(5, 50);
		mainTableWidget->setColumnWidth(6, 50);
		mainTableWidget->setColumnWidth(7, 50);
		mainTableWidget->setColumnWidth(8, 50);
		mainTableWidget->setColumnWidth(9, 50);
		mainTableWidget->setColumnWidth(10, 150);

		mainTableWidget->setRowHeight(0, 40);
		mainTableWidget->setRowHeight(1, 40);
		mainTableWidget->setRowHeight(2, 40);

		mainTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
		mainTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
		mainTableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
		mainTableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
		mainTableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);
		mainTableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Fixed);
		mainTableWidget->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Fixed);

		mainTableWidget->verticalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
		mainTableWidget->verticalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
		mainTableWidget->verticalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
		mainTableWidget->verticalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);

		for (int column = 0; column < mainTableWidget->columnCount(); column++)
		{
			mainTableWidget->model()->setData(mainTableWidget->model()->index(mainTableWidget->rowCount() - 1, column), "");
			mainTableWidget->item(mainTableWidget->rowCount() - 1, column)->setTextAlignment(Qt::AlignCenter);
			mainTableWidget->item(mainTableWidget->rowCount() - 1, column)->setFont(font);
		}

		mainTableWidget->item(CEll_VALUE_PAD_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(CELL_VALUE_PIN_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(CELL_VALUE_TYPE_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(CELL_VALUE_NAME_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(CELL_VALUE_MEASURED_VALUE_U_TABLE)->setFlags(Qt::ItemIsSelectable);
		mainTableWidget->item(CELL_VALUE_MEASURED_VALUE_I_TABLE)->setFlags(Qt::ItemIsSelectable);
	}

	initUiSetValueTable();


}
*/



void MoreWindow::resizeEvent(QResizeEvent* event1)
{
	mainWidget->resize(geometry().width() - (PADDING_MAINWIDGET * 2), geometry().height() - PADDING_MAINWIDGET);
}

void MoreWindow::on_mainTableWidget_cellChanged(int row, int column)
{
	//if (row == 3 && column >= 7)
	//{
	//	saveChangesButton->show();
	//	changedThresholds[column - 7] = mainTableWidget->item(row, column)->text().toFloat();
	//}
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

		cableTmp.id = row->id;
		cableTmp.connector = row->connectorInt;
		cableTmp.pin = row->pin.toInt();

		if (row->direction == "OUT")
			cableTmp.direction = DIRECTION_OUT;
		else if(row->direction == "IN")
			cableTmp.direction = DIRECTION_IN;
		else
			cableTmp.direction = DIRECTION_NOT_SET;

		if (row->type == "DIGITAL")
			cableTmp.type = TYPE_DIGITAL;
		else if(row->type == "ANALOG")
			cableTmp.type = TYPE_ANALOG;
		else if (row->type == "HALL")
			cableTmp.type = TYPE_HALL;
		else if (row->type == "PWM")
			cableTmp.type = TYPE_PWM;
		else if (row->type == "VNH")
			cableTmp.type = TYPE_VNH;
		else
			cableTmp.type = TYPE_NOT_SET;

		cableTmp.canId = row->canId;
		cableTmp.bit = row->bit;
		cableTmp.name = row->name;
		cableTmp.component = row->component;
		cableTmp.minVoltage = (changedThresholds[0] != NOT_SET ? changedThresholds[0] : row->minVoltage);
		cableTmp.maxVoltage = (changedThresholds[1] != NOT_SET ? changedThresholds[1] : row->maxVoltage);
		cableTmp.minCurrent = (changedThresholds[2] != NOT_SET ? changedThresholds[2] : row->minCurrent);
		cableTmp.maxCurrent = (changedThresholds[3] != NOT_SET ? changedThresholds[3] : row->maxCurrent);

		row->minVoltage = (changedThresholds[0] != NOT_SET ? changedThresholds[0] : row->minVoltage);
		row->maxVoltage = (changedThresholds[1] != NOT_SET ? changedThresholds[1] : row->maxVoltage);
		row->minCurrent = (changedThresholds[2] != NOT_SET ? changedThresholds[2] : row->minCurrent);
		row->maxCurrent = (changedThresholds[3] != NOT_SET ? changedThresholds[3] : row->maxCurrent);

		changedThresholds[0] = NOT_SET;
		changedThresholds[1] = NOT_SET;
		changedThresholds[2] = NOT_SET;
		changedThresholds[3] = NOT_SET;

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


