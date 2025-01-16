#include "TestWindow.h"

#define COLUMN_COUNT		8

#define COLUMN_TYPE		3
#define COLUMN_CHECK		4
#define COLUMN_STAND		5
#define COLUMN_PC			6
#define COLUMN_MORE		7

void TestWindow::initUiOutManualTestAutoStand()
{
	autoStandConnectButton = new QPushButton(usefulSpaceWidget);
	autoStandConnectButton->setObjectName("AutoStandConnectButton");
	autoStandConnectButton->setFixedSize(QSize(FIXED_HEADER_BUTTON_WIDTH, FIXED_HEADER_BUTTON_HEIGHT));
	usefulSpaceHLayout->addWidget(autoStandConnectButton);

	outManualTestAutoStandTestTimeComboBox = new QComboBox(usefulSpaceWidget);
	outManualTestAutoStandTestTimeComboBox->setObjectName("outManualTestAutoStandTestTimeComboBox");
	outManualTestAutoStandTestTimeComboBox->setFixedSize(QSize(FIXED_HEADER_COMBO_WIDTH, FIXED_HEADER_COMBO_HEIGHT));
	usefulSpaceHLayout->addWidget(outManualTestAutoStandTestTimeComboBox);
	createItemManualTestAutoStandTestTimeComboBox(outManualTestAutoStandTestTimeComboBox);

	initUiTableOutManualTestAutoStand();
}

void TestWindow::initUiTableOutManualTestAutoStand()
{
	initUiTableHeaderOutManualTestAutoStand();
	initUiTableRowsOutManualTestAutoStand();
}

void TestWindow::initUiTableHeaderOutManualTestAutoStand()
{
	mainTableWidget->setRowCount(cableRows.size());
	mainTableWidget->setColumnCount(COLUMN_COUNT);

	resetTableHeaderLanguageOutManualTestAutoStand();

	mainTableWidget->setColumnWidth(COLUMN_CONNECTOR,	COLUMN_CONNECTOR_WIDTH);
	mainTableWidget->setColumnWidth(COLUMN_PIN,			COLUMN_PIN_WIDTH);
	mainTableWidget->setColumnWidth(COLUMN_TYPE,		COLUMN_TYPE_WIDTH);
	mainTableWidget->setColumnWidth(COLUMN_CHECK,		COLUMN_AUTOCHECK_WIDTH);
	mainTableWidget->setColumnWidth(COLUMN_STAND,		COLUMN_STAND_WIDTH);
	mainTableWidget->setColumnWidth(COLUMN_PC,			COLUMN_PC_WIDTH);
	mainTableWidget->setColumnWidth(COLUMN_MORE,		COLUMN_MORE_WIDTH);

	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_CONNECTOR,		QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_PIN,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_NAME,			QHeaderView::Stretch);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_TYPE,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_CHECK,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_STAND,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_PC,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLUMN_MORE,			QHeaderView::Fixed);

	QTableWidgetItem* protoitem = new QTableWidgetItem();
	protoitem->setTextAlignment(Qt::AlignCenter);
	for (int row = 0; row < mainTableWidget->rowCount(); row++)
	{
		mainTableWidget->setItem(row, COLUMN_CONNECTOR, protoitem->clone());
		mainTableWidget->setItem(row, COLUMN_PIN, protoitem->clone());
		mainTableWidget->setItem(row, COLUMN_TYPE, protoitem->clone());
	}
}

void TestWindow::resetLanguageOutManualTestAutoStand()
{
	resetTableHeaderLanguageOutManualTestAutoStand();
	resetTableTypeLanguageOutManualTestAutoStand();
}

void TestWindow::resetTableHeaderLanguageOutManualTestAutoStand()
{
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		delete mainTableHeaderLabels;
		mainTableHeaderLabels = new QStringList();
		mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Разъём"));
		mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Пин"));
		mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Название"));
		mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Тип"));
		mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Проверка"));
		mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Стенд"));
		mainTableHeaderLabels->push_back(QString::fromLocal8Bit("ПК"));
		mainTableHeaderLabels->push_back("");
		break;

	case ENGLISH_LANG:
		delete mainTableHeaderLabels;
		mainTableHeaderLabels = new QStringList();
		mainTableHeaderLabels->push_back("Connector");
		mainTableHeaderLabels->push_back("Pin");
		mainTableHeaderLabels->push_back("Name");
		mainTableHeaderLabels->push_back("Direction");
		mainTableHeaderLabels->push_back("Type");
		mainTableHeaderLabels->push_back("Check");
		mainTableHeaderLabels->push_back("Stand");
		mainTableHeaderLabels->push_back("PC");
		mainTableHeaderLabels->push_back("");
		break;
	}
	mainTableWidget->setHorizontalHeaderLabels(*mainTableHeaderLabels);
}

void TestWindow::resetTableTypeLanguageOutManualTestAutoStand()
{
	QAbstractItemModel* model = mainTableWidget->model();
	for (int currentRowNum = 0; currentRowNum < cableRows.size(); currentRowNum++)
	{
		switch (viewWindowState->appLanguage)
		{
		case RUSSIAN_LANG:
			if (cableRows[currentRowNum]->type == "DIGITAL")
				model->setData(model->index(currentRowNum, COLUMN_TYPE), QString::fromLocal8Bit("Цифровой"));
			else if (cableRows[currentRowNum]->type == "PWM")
				model->setData(model->index(currentRowNum, COLUMN_TYPE), QString::fromLocal8Bit("ШИМ"));
			else if (cableRows[currentRowNum]->type == "VNH")
				model->setData(model->index(currentRowNum, COLUMN_TYPE), QString::fromLocal8Bit("VNH"));
			else if (cableRows[currentRowNum]->type == "ANALOG")
				model->setData(model->index(currentRowNum, COLUMN_TYPE), QString::fromLocal8Bit("Аналоговый"));
			else if (cableRows[currentRowNum]->type == "HALL")
				model->setData(model->index(currentRowNum, COLUMN_TYPE), QString::fromLocal8Bit("HALL"));
			break;

		case ENGLISH_LANG:
			if (cableRows[currentRowNum]->type == "DIGITAL")
				model->setData(model->index(currentRowNum, COLUMN_TYPE), "Digital");
			else if (cableRows[currentRowNum]->type == "PWM")
				model->setData(model->index(currentRowNum, COLUMN_TYPE), "PWM");
			else if (cableRows[currentRowNum]->type == "VNH")
				model->setData(model->index(currentRowNum, COLUMN_TYPE), "VNH");
			else if (cableRows[currentRowNum]->type == "ANALOG")
				model->setData(model->index(currentRowNum, COLUMN_TYPE), "Analog");
			else if (cableRows[currentRowNum]->type == "HALL")
				model->setData(model->index(currentRowNum, COLUMN_TYPE), "HALL");
			break;
		}
	}
}

void TestWindow::initUiTableRowsOutManualTestAutoStand()
{
	QAbstractItemModel* model = mainTableWidget->model();
	for (int currentRowNum = 0; currentRowNum < cableRows.size(); currentRowNum++)
	{
		model->setData(model->index(currentRowNum, COLUMN_CONNECTOR), cableRows[currentRowNum]->connector);
		model->setData(model->index(currentRowNum, COLUMN_PIN), cableRows[currentRowNum]->pin);
		model->setData(model->index(currentRowNum, COLUMN_NAME), cableRows[currentRowNum]->name);

		QWidget* autoCheckCellWidget = new QWidget(mainLayoutWidget);
		QWidget* moreCellWidget = new QWidget(mainLayoutWidget);

		initAutoCheckButton(currentRowNum, autoCheckCellWidget);
		initMoreButton(currentRowNum, moreCellWidget);

		mainTableWidget->setCellWidget(currentRowNum, COLUMN_CHECK, autoCheckCellWidget);
		mainTableWidget->setCellWidget(currentRowNum, COLUMN_MORE, moreCellWidget);
	}
	resetTableTypeLanguageOutManualTestAutoStand();
}


void TestWindow::on_outManualTestAutoStandTestTimeComboBox_changed(int ind)
{
}