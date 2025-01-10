#include "MoreWindow.h"


MoreWindow::MoreWindow(Cable cable)
{
	this->cable = cable;
	measured.current = -1;
	measured.voltage = -1;

	initUi();
	resetLanguage();
}
MoreWindow::~MoreWindow()
{}

void MoreWindow::resetLanguage()
{
		QString typeTmp;
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		mainTableWidget->item(CEll_PAD_TABLE)->setText(QString::fromLocal8Bit("Колодка"));
		mainTableWidget->item(CELL_PIN_TABLE)->setText(QString::fromLocal8Bit("Пин"));
		mainTableWidget->item(CELL_ID_TABLE)->setText("Id");
		mainTableWidget->item(CELL_TYPE_TABLE)->setText(QString::fromLocal8Bit("Тип"));
		mainTableWidget->item(CELL_NAME_TABLE)->setText(QString::fromLocal8Bit("Название"));
		mainTableWidget->item(CELL_MEASURED_VALUE_TABLE)->setText(QString::fromLocal8Bit("Измеренное\nзначенние"));
		mainTableWidget->item(CELL_MEASURED_VALUE_U_TABLE)->setText(QString::fromLocal8Bit("U, В"));
		mainTableWidget->item(CELL_MEASURED_VALUE_I_TABLE)->setText(QString::fromLocal8Bit("I, А"));
		mainTableWidget->item(CELL_PROGS_TABLE)->setText(QString::fromLocal8Bit("Пороги"));
		mainTableWidget->item(CELL_PROGS_U_TABLE)->setText(QString::fromLocal8Bit("U, В"));
		mainTableWidget->item(CELL_PROGS_I_TABLE)->setText(QString::fromLocal8Bit("I, А"));
		mainTableWidget->item(CELL_PROGS_U_MIN_TABLE)->setText(QString::fromLocal8Bit("Мин."));
		mainTableWidget->item(CELL_PROGS_U_MAX_TABLE)->setText(QString::fromLocal8Bit("Макс."));
		mainTableWidget->item(CELL_PROGS_I_MIN_TABLE)->setText(QString::fromLocal8Bit("Мин."));
		mainTableWidget->item(CELL_PROGS_I_MAX_TABLE)->setText(QString::fromLocal8Bit("Макс."));

		switch (cable.type)
		{
		case TYPE_NOT_SET:
			typeTmp = QString::fromLocal8Bit("Не указан");
			break;
		case TYPE_DIGITAL:
			typeTmp = QString::fromLocal8Bit("Цифровой");
			break;
		case TYPE_ANALOG:
			typeTmp = QString::fromLocal8Bit("Аналоговый");
			break;
		case TYPE_HALL:
			typeTmp = "HALL";
			break;
		case TYPE_PWM:
			typeTmp = "PWM";
			break;
		case TYPE_VNH:
			typeTmp = "VNH";
			break;

		}
		mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_VALUE_TYPE_TABLE), typeTmp);

		startTestButton->setText(QString::fromLocal8Bit("Тест"));
		saveChangesButton->setText(QString::fromLocal8Bit("Сохранить"));	
		break;

	case ENGLISH_LANG:
		mainTableWidget->item(CEll_PAD_TABLE)->setText("Pad");
		mainTableWidget->item(CELL_PIN_TABLE)->setText("Pin");
		mainTableWidget->item(CELL_ID_TABLE)->setText("Id");
		mainTableWidget->item(CELL_TYPE_TABLE)->setText("Type");
		mainTableWidget->item(CELL_NAME_TABLE)->setText("Name");
		mainTableWidget->item(CELL_MEASURED_VALUE_TABLE)->setText("Measured\nvalue");
		mainTableWidget->item(CELL_MEASURED_VALUE_U_TABLE)->setText("U, V");
		mainTableWidget->item(CELL_MEASURED_VALUE_I_TABLE)->setText("I, A");
		mainTableWidget->item(CELL_PROGS_TABLE)->setText("Progs");
		mainTableWidget->item(CELL_PROGS_U_TABLE)->setText("U, V");
		mainTableWidget->item(CELL_PROGS_I_TABLE)->setText("I, A");
		mainTableWidget->item(CELL_PROGS_U_MIN_TABLE)->setText("Min.");
		mainTableWidget->item(CELL_PROGS_U_MAX_TABLE)->setText("Max.");
		mainTableWidget->item(CELL_PROGS_I_MIN_TABLE)->setText("Min.");
		mainTableWidget->item(CELL_PROGS_I_MAX_TABLE)->setText("Max.");


		switch (cable.type)
		{
		case TYPE_NOT_SET:
			typeTmp = "No set";
			break;
		case TYPE_DIGITAL:
			typeTmp = "Digital";
			break;
		case TYPE_ANALOG:
			typeTmp = "Analog";
			break;
		case TYPE_HALL:
			typeTmp = "HALL";
			break;
		case TYPE_PWM:
			typeTmp = "PWM";
			break;
		case TYPE_VNH:
			typeTmp = "VNH";
			break;

		}
		mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_VALUE_TYPE_TABLE), typeTmp);

		startTestButton->setText("Test");
		saveChangesButton->setText("Save");
		break;
	}

	QString padTmp;
	switch (cable.connector)
	{
	case ConnectorId::A:
		padTmp = "A";
		break;
	case ConnectorId::B:
		padTmp = "B";
		break;
	case ConnectorId::C:
		padTmp = "C";
		break;
	case ConnectorId::D:
		padTmp = "D";
		break;
	default:
		padTmp = "Error";
		break;
	}
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CEll_VALUE_PAD_TABLE), padTmp);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_VALUE_PIN_TABLE), cable.pin);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_VALUE_ID_TABLE), cable.id);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_VALUE_NAME_TABLE), cable.name);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_VALUE_MEASURED_VALUE_U_TABLE), measured.voltage);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_VALUE_MEASURED_VALUE_I_TABLE), measured.current);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_VALUE_PROGS_U_MIN_TABLE), cable.minVoltage);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_VALUE_PROGS_U_MAX_TABLE), cable.maxVoltage);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_VALUE_PROGS_I_MIN_TABLE), cable.minCurrent);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_VALUE_PROGS_I_MAX_TABLE), cable.maxCurrent);

}
void MoreWindow::initUi()
{
	this->resize(START_MOREWINDOW_SIZE);
	this->setMinimumSize(725, 250);
	mainWidget = new QWidget(this);
	mainWidget->setObjectName("mainLayoutWidget");
	mainWidget->setGeometry(PADDING_MAIN_WIDGET, 0, START_WINDOW_WIDTH - (PADDING_MAIN_WIDGET *2), START_WINDOW_HEIGHT - PADDING_MAIN_WIDGET);

	mainVLayout = new QVBoxLayout(mainWidget);
	mainVLayout->setObjectName("mainVLayout");
	initUiGenerateTable();
	initUiBottomLayout();
}

void MoreWindow::initUiGenerateTable()
{
	mainTableWidget = new QTableWidget(mainWidget);
	mainTableWidget->setObjectName("mainTableWidget");
	mainTableWidget->setStyleSheet(lightStyles.testwindowTableWidget);
	mainTableWidget->setSelectionMode(QAbstractItemView::NoSelection);
	mainTableWidget->horizontalHeader()->hide();
	mainTableWidget->verticalHeader()->hide();
	mainTableWidget->setRowCount(4);
	mainTableWidget->setColumnCount(11);

	QFont font = QFont();
	font.setBold(true);
	font.setPointSizeF(10);
	// Pad
	mainTableWidget->setSpan(CEll_PAD_TABLE, 3, 1);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CEll_PAD_TABLE), "");
	mainTableWidget->item(0, 0)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(0, 0)->setFont(font);
	// Pin
	mainTableWidget->setSpan(CELL_PIN_TABLE, 3, 1);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_PIN_TABLE), "");
	mainTableWidget->item(CELL_PIN_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_PIN_TABLE)->setFont(font);
	// Id
	mainTableWidget->setSpan(CELL_ID_TABLE, 3, 1);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_ID_TABLE), "");
	mainTableWidget->item(CELL_ID_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_ID_TABLE)->setFont(font);
	// Type
	mainTableWidget->setSpan(CELL_TYPE_TABLE, 3, 1);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_TYPE_TABLE), "");
	mainTableWidget->item(CELL_TYPE_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_TYPE_TABLE)->setFont(font);
	// Name
	mainTableWidget->setSpan(CELL_NAME_TABLE, 3, 1);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_NAME_TABLE), "");
	mainTableWidget->item(CELL_NAME_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_NAME_TABLE)->setFont(font);
	// Measured value
	mainTableWidget->setSpan(CELL_MEASURED_VALUE_TABLE, 2, 2);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_MEASURED_VALUE_TABLE), "");
	mainTableWidget->item(CELL_MEASURED_VALUE_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_MEASURED_VALUE_TABLE)->setFont(font);
	// Measured value U
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_MEASURED_VALUE_U_TABLE), "");
	mainTableWidget->item(CELL_MEASURED_VALUE_U_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_MEASURED_VALUE_U_TABLE)->setFont(font);
	// Measured value I
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_MEASURED_VALUE_I_TABLE), "");
	mainTableWidget->item(CELL_MEASURED_VALUE_I_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_MEASURED_VALUE_I_TABLE)->setFont(font);
	// Progs
	mainTableWidget->setSpan(CELL_PROGS_TABLE, 1, 4);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_PROGS_TABLE), "");
	mainTableWidget->item(CELL_PROGS_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_PROGS_TABLE)->setFont(font);
	// Progs U
	mainTableWidget->setSpan(CELL_PROGS_U_TABLE, 1, 2);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_PROGS_U_TABLE), "");
	mainTableWidget->item(CELL_PROGS_U_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_PROGS_U_TABLE)->setFont(font);
	// Progs I
	mainTableWidget->setSpan(CELL_PROGS_I_TABLE, 1, 2);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_PROGS_I_TABLE), "");
	mainTableWidget->item(CELL_PROGS_I_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_PROGS_I_TABLE)->setFont(font);
	// Progs U min
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_PROGS_U_MIN_TABLE), "");
	mainTableWidget->item(CELL_PROGS_U_MIN_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_PROGS_U_MIN_TABLE)->setFont(font);
	// Progs U max
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_PROGS_U_MAX_TABLE), "");
	mainTableWidget->item(CELL_PROGS_U_MAX_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_PROGS_U_MAX_TABLE)->setFont(font);
	// Progs I min
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_PROGS_I_MIN_TABLE), "");
	mainTableWidget->item(CELL_PROGS_I_MIN_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_PROGS_I_MIN_TABLE)->setFont(font);
	// Progs I max
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_PROGS_I_MAX_TABLE), "");
	mainTableWidget->item(CELL_PROGS_I_MAX_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_PROGS_I_MAX_TABLE)->setFont(font);

	mainTableWidget->setColumnWidth(0, 75);
	mainTableWidget->setColumnWidth(1, 75);
	mainTableWidget->setColumnWidth(2, 75);
	mainTableWidget->setColumnWidth(3, 75);
	mainTableWidget->setColumnWidth(4, 75);
	mainTableWidget->setColumnWidth(5, 50);
	mainTableWidget->setColumnWidth(6, 50);
	mainTableWidget->setColumnWidth(7, 50);
	mainTableWidget->setColumnWidth(8, 50);
	mainTableWidget->setColumnWidth(9, 50);
	mainTableWidget->setColumnWidth(10, 50);

	mainTableWidget->setRowHeight(0, 40);
	mainTableWidget->setRowHeight(1, 40);
	mainTableWidget->setRowHeight(2, 40);

	mainTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Fixed);

	mainTableWidget->verticalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
	mainTableWidget->verticalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
	mainTableWidget->verticalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
	mainTableWidget->verticalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);

	for (int column = 0; column < mainTableWidget->columnCount() - 1; column++)
	{
		auto lol = mainTableWidget->rowCount();
		auto lol1 = mainTableWidget->columnCount();
		mainTableWidget->model()->setData(mainTableWidget->model()->index(mainTableWidget->rowCount() - 1, column), "fsdfsd");
		mainTableWidget->item(mainTableWidget->rowCount() - 1, column)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(mainTableWidget->rowCount() - 1, column)->setFont(font);
	}

	mainVLayout->addWidget(mainTableWidget);
}

void MoreWindow::initUiBottomLayout()
{
	bottomHLayout = new QHBoxLayout(mainWidget);
	bottomHLayout->setObjectName("bottomHLayout");
	// test
	startTestButton = new QPushButton();
	startTestButton->setMinimumSize(BUTTON_SIZE);
	startTestButton->setStyleSheet(lightStyles.testwindowButtonStyle);
	bottomHLayout->addWidget(startTestButton);

	saveChangesButton = new QPushButton();
	saveChangesButton->setMinimumSize(BUTTON_SIZE);
	saveChangesButton->setStyleSheet(lightStyles.testwindowButtonStyle);
	saveChangesButton->hide();
	bottomHLayout->addWidget(saveChangesButton);

	bottomSpacer = new QSpacerItem(40, 0, QSizePolicy::Expanding);
	bottomHLayout->addItem(bottomSpacer);


	// test
	
	mainVLayout->addLayout(bottomHLayout);

}


void MoreWindow::resizeEvent(QResizeEvent* event1)
{
	mainWidget->resize(geometry().width() - (PADDING_MAIN_WIDGET * 2), geometry().height() - PADDING_MAIN_WIDGET);
}