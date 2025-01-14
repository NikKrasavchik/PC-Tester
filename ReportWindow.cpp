#include "ReportWindow.h"


ReportWindow::ReportWindow(std::vector<Cable> cables, WindowType testType)
{
	this->cables = cables;
	this->testType = testType;
	measured.current = -1;
	measured.voltage = -1;
	for (int i = 0; i < sizeof(changedThresholds) / sizeof(changedThresholds[0]); i++)
		changedThresholds[i] = -1;

	initUi();

	QMetaObject::connectSlotsByName(this);
}
ReportWindow::~ReportWindow()
{}

void ReportWindow::initUi()
{
	this->resize(START_ReportWindow_SIZE);
	this->setMinimumSize(725, 250);
	mainWidget = new QWidget(this);
	mainWidget->setObjectName("mainLayoutWidget");
	mainWidget->setGeometry(LEFT_PADDING_MAIN_WIDGET, UP_PADDING_MAIN_WIDGET, START_ReportWindow_WIDTH - (LEFT_PADDING_MAIN_WIDGET * 2), START_ReportWindow_HEIGHT - (UP_PADDING_MAIN_WIDGET * 2));

	mainVLayout = new QVBoxLayout(mainWidget);
	mainVLayout->setObjectName("mainVLayout");

	bodyHLayout = new QHBoxLayout(mainWidget);
	bodyHLayout->setObjectName("mainHLayout");
	mainVLayout->addLayout(bodyHLayout);

	initUiTable();
	bodyHLayout->addWidget(mainTableWidget);

	initUiColumnSetters();
	bodyHLayout->addWidget(columnSetterWidget);

	initUiFooter();
	mainVLayout->addWidget(footerWidget);
}

void ReportWindow::initUiTable()
{
	initUiGenerateTable();

	initUiSetValueTable();

	connect(mainTableWidget, &QTableWidget::cellChanged, this, &ReportWindow::on_mainTableWidget_cellChanged);
}

void ReportWindow::initUiGenerateTable()
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
	mainTableWidget->item(CEll_PAD_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CEll_PAD_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(0, 0)->setFont(font);
	// Pin
	mainTableWidget->setSpan(CELL_PIN_TABLE, 3, 1);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_PIN_TABLE), "");
	mainTableWidget->item(CELL_PIN_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_PIN_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_PIN_TABLE)->setFont(font);
	// Id
	mainTableWidget->setSpan(CELL_ID_TABLE, 3, 1);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_ID_TABLE), "");
	mainTableWidget->item(CELL_ID_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_ID_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_ID_TABLE)->setFont(font);
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
	mainTableWidget->setSpan(CELL_PROGS_TABLE, 1, 4);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_PROGS_TABLE), "");
	mainTableWidget->item(CELL_PROGS_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_PROGS_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_PROGS_TABLE)->setFont(font);
	// Progs U
	mainTableWidget->setSpan(CELL_PROGS_U_TABLE, 1, 2);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_PROGS_U_TABLE), "");
	mainTableWidget->item(CELL_PROGS_U_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_PROGS_U_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_PROGS_U_TABLE)->setFont(font);
	// Progs I
	mainTableWidget->setSpan(CELL_PROGS_I_TABLE, 1, 2);
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_PROGS_I_TABLE), "");
	mainTableWidget->item(CELL_PROGS_I_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_PROGS_I_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_PROGS_I_TABLE)->setFont(font);
	// Progs U min
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_PROGS_U_MIN_TABLE), "");
	mainTableWidget->item(CELL_PROGS_U_MIN_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_PROGS_U_MIN_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_PROGS_U_MIN_TABLE)->setFont(font);
	// Progs U max
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_PROGS_U_MAX_TABLE), "");
	mainTableWidget->item(CELL_PROGS_U_MAX_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_PROGS_U_MAX_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_PROGS_U_MAX_TABLE)->setFont(font);
	// Progs I min
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_PROGS_I_MIN_TABLE), "");
	mainTableWidget->item(CELL_PROGS_I_MIN_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_PROGS_I_MIN_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_PROGS_I_MIN_TABLE)->setFont(font);
	// Progs I max
	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_PROGS_I_MAX_TABLE), "");
	mainTableWidget->item(CELL_PROGS_I_MAX_TABLE)->setTextAlignment(Qt::AlignCenter);
	mainTableWidget->item(CELL_PROGS_I_MAX_TABLE)->setFlags(Qt::ItemIsSelectable);
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

	for (int column = 0; column < mainTableWidget->columnCount(); column++)
	{
		mainTableWidget->model()->setData(mainTableWidget->model()->index(mainTableWidget->rowCount() - 1, column), "");
		mainTableWidget->item(mainTableWidget->rowCount() - 1, column)->setTextAlignment(Qt::AlignCenter);
		mainTableWidget->item(mainTableWidget->rowCount() - 1, column)->setFont(font);
	}

	mainTableWidget->item(CEll_VALUE_PAD_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_VALUE_PIN_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_VALUE_ID_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_VALUE_ID_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_VALUE_TYPE_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_VALUE_NAME_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_VALUE_MEASURED_VALUE_U_TABLE)->setFlags(Qt::ItemIsSelectable);
	mainTableWidget->item(CELL_VALUE_MEASURED_VALUE_I_TABLE)->setFlags(Qt::ItemIsSelectable);
}

void ReportWindow::fillColumnsSetters()
{
	switch (testType)
	{
	case WindowType::IN_TEST_MANUAL_STAND:
		activeColumn.measured				= true;
		activeColumn.checkManualStand		= true;
		activeColumn.checkPCAutoStand		= true;
		activeColumn.checkStandAutoStand	= true;
		break;

	case WindowType::OUT_TEST_MANUAL_STAND:
		activeColumn.measured				= true;
		activeColumn.checkManualStand		= true;
		activeColumn.checkPCAutoStand		= true;
		activeColumn.checkStandAutoStand	= true;
		break;

	case WindowType::FULL_TEST_MANUAL_STAND:
		activeColumn.measured				= true;
		activeColumn.checkManualStand		= true;
		activeColumn.checkPCAutoStand		= true;
		activeColumn.checkStandAutoStand	= true;
		break;

	case WindowType::IN_MANUAL_TEST_AUTO_STAND:
		activeColumn.measured				= true;
		activeColumn.checkManualStand		= true;
		activeColumn.checkPCAutoStand		= true;
		activeColumn.checkStandAutoStand	= true;
		break;

	case WindowType::OUT_MANUAL_TEST_AUTO_STAND:
		activeColumn.measured				= true;
		activeColumn.checkManualStand		= true;
		activeColumn.checkPCAutoStand		= true;
		activeColumn.checkStandAutoStand	= true;
		break;

	case WindowType::IN_AUTO_TEST_AUTO_STAND:
		activeColumn.measured				= true;
		activeColumn.checkManualStand		= true;
		activeColumn.checkPCAutoStand		= true;
		activeColumn.checkStandAutoStand	= true;
		break;

	case WindowType::OUT_AUTO_TEST_AUTO_STAND:
		activeColumn.measured				= true;
		activeColumn.checkManualStand		= true;
		activeColumn.checkPCAutoStand		= true;
		activeColumn.checkStandAutoStand	= true;
		break;

	case WindowType::FULL_TEST_AUTO_STAND:
		activeColumn.measured				= true;
		activeColumn.checkManualStand		= true;
		activeColumn.checkPCAutoStand		= true;
		activeColumn.checkStandAutoStand	= true;
		break;
	}
}

void ReportWindow::initUiColumnSetters()
{
	columnSetterWidget = new QWidget(mainWidget);
	columnSetterWidget->setObjectName("columnSetterWidget");
	columnSetterWidget->setFixedWidth(COLUMN_SETTER_WIDTH);

	columnSetterVLayout = new QVBoxLayout(columnSetterWidget);
	columnSetterVLayout->setObjectName("columnSetterVLayout");

	fillColumnsSetters();

	if (activeColumn.measured)
	{
		measuredCheckBox = new QCheckBox(columnSetterWidget);
		measuredCheckBox->setObjectName("measuredCheckBox");
		measuredCheckBox->setChecked(true);
		columnSetterVLayout->addWidget(measuredCheckBox, Qt::AlignRight);
	}

	if (activeColumn.checkManualStand)
	{
		checkManualStandCheckBox = new QCheckBox(columnSetterWidget);
		checkManualStandCheckBox->setObjectName("checkManualStandCheckBox");
		checkManualStandCheckBox->setChecked(false);
		columnSetterVLayout->addWidget(checkManualStandCheckBox, Qt::AlignRight);
	}

	if (activeColumn.checkPCAutoStand)
	{
		checkPCAutoStandCheckBox = new QCheckBox(columnSetterWidget);
		checkPCAutoStandCheckBox->setObjectName("checkPCAutoStandCheckBox");
		checkPCAutoStandCheckBox->setChecked(false);
		columnSetterVLayout->addWidget(checkPCAutoStandCheckBox, Qt::AlignRight);
	}

	if (activeColumn.checkStandAutoStand)
	{
		checkStandAutoStandCheckBox = new QCheckBox(columnSetterWidget);
		checkStandAutoStandCheckBox->setObjectName("checkStandAutoStandCheckBox");
		checkStandAutoStandCheckBox->setChecked(false);
		columnSetterVLayout->addWidget(checkStandAutoStandCheckBox, Qt::AlignRight);
	}
}

void ReportWindow::initUiSetValueTable()
{
	//QString typeTmp;
	//switch (viewWindowState->appLanguage)
	//{
	//case RUSSIAN_LANG:
	//	mainTableWidget->item(CEll_PAD_TABLE)->setText(QString::fromLocal8Bit("Колодка"));
	//	mainTableWidget->item(CELL_PIN_TABLE)->setText(QString::fromLocal8Bit("Пин"));
	//	mainTableWidget->item(CELL_ID_TABLE)->setText("Id");
	//	mainTableWidget->item(CELL_TYPE_TABLE)->setText(QString::fromLocal8Bit("Тип"));
	//	mainTableWidget->item(CELL_NAME_TABLE)->setText(QString::fromLocal8Bit("Название"));
	//	mainTableWidget->item(CELL_MEASURED_VALUE_TABLE)->setText(QString::fromLocal8Bit("Измеренное\nзначенние"));
	//	mainTableWidget->item(CELL_MEASURED_VALUE_U_TABLE)->setText(QString::fromLocal8Bit("U, В"));
	//	mainTableWidget->item(CELL_MEASURED_VALUE_I_TABLE)->setText(QString::fromLocal8Bit("I, А"));
	//	mainTableWidget->item(CELL_PROGS_TABLE)->setText(QString::fromLocal8Bit("Пороги"));
	//	mainTableWidget->item(CELL_PROGS_U_TABLE)->setText(QString::fromLocal8Bit("U, В"));
	//	mainTableWidget->item(CELL_PROGS_I_TABLE)->setText(QString::fromLocal8Bit("I, А"));
	//	mainTableWidget->item(CELL_PROGS_U_MIN_TABLE)->setText(QString::fromLocal8Bit("Мин."));
	//	mainTableWidget->item(CELL_PROGS_U_MAX_TABLE)->setText(QString::fromLocal8Bit("Макс."));
	//	mainTableWidget->item(CELL_PROGS_I_MIN_TABLE)->setText(QString::fromLocal8Bit("Мин."));
	//	mainTableWidget->item(CELL_PROGS_I_MAX_TABLE)->setText(QString::fromLocal8Bit("Макс."));

	//	switch (cable.type)
	//	{
	//	case TYPE_NOT_SET:
	//		typeTmp = QString::fromLocal8Bit("Не указан");
	//		break;
	//	case TYPE_DIGITAL:
	//		typeTmp = QString::fromLocal8Bit("Цифровой");
	//		break;
	//	case TYPE_ANALOG:
	//		typeTmp = QString::fromLocal8Bit("Аналоговый");
	//		break;
	//	case TYPE_HALL:
	//		typeTmp = "HALL";
	//		break;
	//	case TYPE_PWM:
	//		typeTmp = "PWM";
	//		break;
	//	case TYPE_VNH:
	//		typeTmp = "VNH";
	//		break;

	//	}
	//	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_VALUE_TYPE_TABLE), typeTmp);	
	//	break;

	//case ENGLISH_LANG:
	//	mainTableWidget->item(CEll_PAD_TABLE)->setText("Pad");
	//	mainTableWidget->item(CELL_PIN_TABLE)->setText("Pin");
	//	mainTableWidget->item(CELL_ID_TABLE)->setText("Id");
	//	mainTableWidget->item(CELL_TYPE_TABLE)->setText("Type");
	//	mainTableWidget->item(CELL_NAME_TABLE)->setText("Name");
	//	mainTableWidget->item(CELL_MEASURED_VALUE_TABLE)->setText("Measured\nvalue");
	//	mainTableWidget->item(CELL_MEASURED_VALUE_U_TABLE)->setText("U, V");
	//	mainTableWidget->item(CELL_MEASURED_VALUE_I_TABLE)->setText("I, A");
	//	mainTableWidget->item(CELL_PROGS_TABLE)->setText("Progs");
	//	mainTableWidget->item(CELL_PROGS_U_TABLE)->setText("U, V");
	//	mainTableWidget->item(CELL_PROGS_I_TABLE)->setText("I, A");
	//	mainTableWidget->item(CELL_PROGS_U_MIN_TABLE)->setText("Min.");
	//	mainTableWidget->item(CELL_PROGS_U_MAX_TABLE)->setText("Max.");
	//	mainTableWidget->item(CELL_PROGS_I_MIN_TABLE)->setText("Min.");
	//	mainTableWidget->item(CELL_PROGS_I_MAX_TABLE)->setText("Max.");


	//	switch (cable.type)
	//	{
	//	case TYPE_NOT_SET:
	//		typeTmp = "No set";
	//		break;
	//	case TYPE_DIGITAL:
	//		typeTmp = "Digital";
	//		break;
	//	case TYPE_ANALOG:
	//		typeTmp = "Analog";
	//		break;
	//	case TYPE_HALL:
	//		typeTmp = "HALL";
	//		break;
	//	case TYPE_PWM:
	//		typeTmp = "PWM";
	//		break;
	//	case TYPE_VNH:
	//		typeTmp = "VNH";
	//		break;

	//	}
	//	mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_VALUE_TYPE_TABLE), typeTmp);
	//	break;
	//}

	//QString padTmp;
	//switch (cable.connector)
	//{
	//case ConnectorId::A:
	//	padTmp = "A";
	//	break;
	//case ConnectorId::B:
	//	padTmp = "B";
	//	break;
	//case ConnectorId::C:
	//	padTmp = "C";
	//	break;
	//case ConnectorId::D:
	//	padTmp = "D";
	//	break;
	//default:
	//	padTmp = "Error";
	//	break;
	//}
	//mainTableWidget->model()->setData(mainTableWidget->model()->index(CEll_VALUE_PAD_TABLE), padTmp);
	//mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_VALUE_PIN_TABLE), cable.pin);
	//mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_VALUE_ID_TABLE), cable.id);
	//mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_VALUE_NAME_TABLE), cable.name);
	//mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_VALUE_MEASURED_VALUE_U_TABLE), measured.voltage);
	//mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_VALUE_MEASURED_VALUE_I_TABLE), measured.current);
	//mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_VALUE_PROGS_U_MIN_TABLE), cable.minVoltage);
	//mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_VALUE_PROGS_U_MAX_TABLE), cable.maxVoltage);
	//mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_VALUE_PROGS_I_MIN_TABLE), cable.minCurrent);
	//mainTableWidget->model()->setData(mainTableWidget->model()->index(CELL_VALUE_PROGS_I_MAX_TABLE), cable.maxCurrent);

}


void ReportWindow::initUiFooter()
{
	footerWidget = new QWidget(mainWidget);
	footerWidget->setObjectName("footerWidget");

	bottomHLayout = new QHBoxLayout(footerWidget);
	bottomHLayout->setObjectName("bottomHLayout");

	bottomSpacer = new QSpacerItem(40, 0, QSizePolicy::Expanding);
	bottomHLayout->addItem(bottomSpacer);

	saveButton = new QPushButton(footerWidget);
	saveButton->setMinimumSize(BUTTON_SIZE);
	saveButton->setObjectName("saveButton");
	saveButton->setStyleSheet(lightStyles.testwindowButtonStyle);
	bottomHLayout->addWidget(saveButton);

	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		saveButton->setText(QString::fromLocal8Bit("Сохранить"));
		break;
	case ENGLISH_LANG:
		saveButton->setText("Save");
		break;
	}
}


void ReportWindow::resizeEvent(QResizeEvent* event1)
{
	mainWidget->resize(geometry().width() - (LEFT_PADDING_MAIN_WIDGET * 2), geometry().height() - (UP_PADDING_MAIN_WIDGET * 2));
}

void ReportWindow::on_mainTableWidget_cellChanged(int row, int column)
{
	//if (row == 3 && column >= 7)
	//{
	//	saveChangesButton->show();
	//	changedThresholds[column - 7] = mainTableWidget->item(row, column)->text().toFloat();
	//}
}

void ReportWindow::on_saveChangesButton_clicked()
{
	//QMessageBox msgBox;
	//msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
	//msgBox.setDefaultButton(QMessageBox::Save);
	//msgBox.setIcon(QMessageBox::Question);
	//msgBox.setMinimumSize(200, 100);
	//switch (viewWindowState->appLanguage)
	//{
	//case RUSSIAN_LANG:
	//	msgBox.setText(QString::fromLocal8Bit("Сохранить изменения?"));
	//	msgBox.setInformativeText(QString::fromLocal8Bit("Изменения будут внесены в конфигурационный файл."));
	//	break;
	//case ENGLISH_LANG:

	//	break;
	//}
	//if (msgBox.exec() == QMessageBox::Save)
	//{
	//	std::vector<Cable> cablesTmp;
	//	for (int i = 0; i < sizeof(changedThresholds) / sizeof(changedThresholds[0]); i++)
	//	{
	//		if (changedThresholds[i] != -1)
	//		{
	//			Cable cableTmp;
	//			cableTmp.id = cable.id;
	//			cableTmp.connector = cable.connector;
	//			cableTmp.pin = cable.pin;
	//			cableTmp.direction = cable.direction;
	//			cableTmp.type = cable.type;
	//			cableTmp.canId = cable.canId;
	//			cableTmp.bit = cable.bit;
	//			cableTmp.name = cable.name;
	//			cableTmp.component = cable.component;

	//			switch (i)
	//			{
	//			case 0:
	//				cableTmp.minVoltage = changedThresholds[i];
	//				break;
	//			case 1:
	//				cableTmp.maxVoltage = changedThresholds[i];
	//				break;
	//			case 2:
	//				cableTmp.minCurrent = changedThresholds[i];
	//				break;
	//			case 3:
	//				cableTmp.maxCurrent = changedThresholds[i];
	//				break;
	//			default:
	//				QMessageBox::critical(this, "error", "ReportWindow.cpp 357 on_saveChangesButton_clicked");
	//				return;
	//				break;
	//			}
	//			cablesTmp.push_back(cableTmp);
	//			changedThresholds[i] = -1;
	//		}
	//	}
	//	// Надо внести изменения в конфиг файл этих кабелей cablesTmp
	//	saveChangesButton->hide();
	//}
}

void ReportWindow::on_startTestButton_clicked()
{


}
