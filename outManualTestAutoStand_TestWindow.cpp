#include "TestWindow.h"

#define COLOUMN_COUNT		8

#define COLOUMN_TYPE		3
#define COLOUMN_CHECK		4
#define COLOUMN_STAND		5
#define COLOUMN_PC			6
#define COLOUMN_MORE		7

void TestWindow::initUiOutManualTestAutoStand()
{
	AutoStandConnectButton = new QPushButton(usefulSpaceWidget);
	AutoStandConnectButton->setObjectName("AutoStandConnectButton");
	AutoStandConnectButton->setFixedSize(QSize(FIXED_HEADER_BUTTON_WIDTH, FIXED_HEADER_BUTTON_HEIGHT));
	usefulSpaceHLayout->addWidget(AutoStandConnectButton);

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
	mainTableWidget->setColumnCount(COLOUMN_COUNT);

	resetTableHeaderLanguageOutManualTestAutoStand();

	mainTableWidget->setColumnWidth(COLOUMN_CONNECTOR,	COLOUMN_CONNECTOR_WIDTH);
	mainTableWidget->setColumnWidth(COLOUMN_PIN,		COLOUMN_PIN_WIDTH);
	mainTableWidget->setColumnWidth(COLOUMN_TYPE,		COLOUMN_TYPE_WIDTH);
	mainTableWidget->setColumnWidth(COLOUMN_CHECK,		COLOUMN_AUTOCHECK_WIDTH);
	mainTableWidget->setColumnWidth(COLOUMN_STAND,		COLOUMN_STAND_WIDTH);
	mainTableWidget->setColumnWidth(COLOUMN_PC,			COLOUMN_PC_WIDTH);
	mainTableWidget->setColumnWidth(COLOUMN_MORE,		COLOUMN_MORE_WIDTH);

	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLOUMN_CONNECTOR,	QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLOUMN_PIN,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLOUMN_NAME,			QHeaderView::Stretch);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLOUMN_TYPE,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLOUMN_CHECK,		QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLOUMN_STAND,		QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLOUMN_PC,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLOUMN_MORE,			QHeaderView::Fixed);

	QTableWidgetItem* protoitem = new QTableWidgetItem();
	protoitem->setTextAlignment(Qt::AlignCenter);
	for (int row = 0; row < mainTableWidget->rowCount(); row++)
	{
		mainTableWidget->setItem(row, COLOUMN_CONNECTOR, protoitem->clone());
		mainTableWidget->setItem(row, COLOUMN_PIN, protoitem->clone());
		mainTableWidget->setItem(row, COLOUMN_TYPE, protoitem->clone());
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
				model->setData(model->index(currentRowNum, COLOUMN_TYPE), QString::fromLocal8Bit("Цифровой"));
			else if (cableRows[currentRowNum]->type == "PWM")
				model->setData(model->index(currentRowNum, COLOUMN_TYPE), QString::fromLocal8Bit("ШИМ"));
			else if (cableRows[currentRowNum]->type == "VNH")
				model->setData(model->index(currentRowNum, COLOUMN_TYPE), QString::fromLocal8Bit("VNH"));
			else if (cableRows[currentRowNum]->type == "ANALOG")
				model->setData(model->index(currentRowNum, COLOUMN_TYPE), QString::fromLocal8Bit("Аналоговый"));
			else if (cableRows[currentRowNum]->type == "HALL")
				model->setData(model->index(currentRowNum, COLOUMN_TYPE), QString::fromLocal8Bit("HALL"));
			break;

		case ENGLISH_LANG:
			if (cableRows[currentRowNum]->type == "DIGITAL")
				model->setData(model->index(currentRowNum, COLOUMN_TYPE), "Digital");
			else if (cableRows[currentRowNum]->type == "PWM")
				model->setData(model->index(currentRowNum, COLOUMN_TYPE), "PWM");
			else if (cableRows[currentRowNum]->type == "VNH")
				model->setData(model->index(currentRowNum, COLOUMN_TYPE), "VNH");
			else if (cableRows[currentRowNum]->type == "ANALOG")
				model->setData(model->index(currentRowNum, COLOUMN_TYPE), "Analog");
			else if (cableRows[currentRowNum]->type == "HALL")
				model->setData(model->index(currentRowNum, COLOUMN_TYPE), "HALL");
			break;
		}
	}
}

void TestWindow::initUiTableRowsOutManualTestAutoStand()
{
	QAbstractItemModel* model = mainTableWidget->model();
	for (int currentRowNum = 0; currentRowNum < cableRows.size(); currentRowNum++)
	{
		qDebug() << cableRows[currentRowNum]->connector << cableRows[currentRowNum]->pin << cableRows[currentRowNum]->name << cableRows[currentRowNum]->direction << cableRows[currentRowNum]->type;

		model->setData(model->index(currentRowNum, COLOUMN_CONNECTOR), cableRows[currentRowNum]->connector);
		model->setData(model->index(currentRowNum, COLOUMN_PIN), cableRows[currentRowNum]->pin);
		model->setData(model->index(currentRowNum, COLOUMN_NAME), cableRows[currentRowNum]->name);

		QWidget* autoCheckCellWidget = new QWidget(mainLayoutWidget);
		QWidget* moreCellWidget = new QWidget(mainLayoutWidget);

		initAutoCheckButton(currentRowNum, autoCheckCellWidget);
		initMoreButton(currentRowNum, moreCellWidget);

		mainTableWidget->setCellWidget(currentRowNum, COLOUMN_CHECK, autoCheckCellWidget);
		mainTableWidget->setCellWidget(currentRowNum, COLOUMN_MORE, moreCellWidget);
	}
	resetTableTypeLanguageOutManualTestAutoStand();
}


void TestWindow::on_outManualTestAutoStandTestTimeComboBox_changed(int ind)
{
}