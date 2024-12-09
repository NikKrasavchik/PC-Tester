#include "TestWindow.h"

#define COLOUMN_COUNT	7

#define COLOUMN_TYPE	3
#define COLOUMN_STAND	4
#define COLOUMN_PC		5
#define COLOUMN_MORE	6

void TestWindow::initUiInManualTestAutoStand()
{
	AutoStandConnectButton = new QPushButton(usefulSpaceWidget);
	AutoStandConnectButton->setObjectName("AutoStandConnectButton");
	AutoStandConnectButton->setFixedSize(QSize(FIXED_HEADER_BUTTON_WIDTH, FIXED_HEADER_BUTTON_HEIGHT));
	usefulSpaceHLayout->addWidget(AutoStandConnectButton);

	inManualTestAutoStandTestTimeComboBox = new QComboBox(usefulSpaceWidget);
	inManualTestAutoStandTestTimeComboBox->setObjectName("inManualTestAutoStandTestTimeComboBox");
	inManualTestAutoStandTestTimeComboBox->setFixedSize(QSize(FIXED_HEADER_COMBO_WIDTH, FIXED_HEADER_COMBO_HEIGHT));
	usefulSpaceHLayout->addWidget(inManualTestAutoStandTestTimeComboBox);
	createItemManualTestAutoStandTestTimeComboBox(inManualTestAutoStandTestTimeComboBox);

	initUiTableInManualTestAutoStand();
}

void TestWindow::initUiTableInManualTestAutoStand()
{
	initUiTableHeaderInManualTestAutoStand();
	initUiTableRowsInManualTestAutoStand();

}

void TestWindow::initUiTableHeaderInManualTestAutoStand()
{
	mainTableWidget->setRowCount(cableRows.size());
	mainTableWidget->setColumnCount(COLOUMN_COUNT);

	resetTableHeaderLanguageInManualTestAutoStand();

	mainTableWidget->setColumnWidth(COLOUMN_CONNECTOR,	COLOUMN_CONNECTOR_WIDTH);
	mainTableWidget->setColumnWidth(COLOUMN_PIN,		COLOUMN_PIN_WIDTH);
	mainTableWidget->setColumnWidth(COLOUMN_TYPE,		COLOUMN_TYPE_WIDTH);
	mainTableWidget->setColumnWidth(COLOUMN_STAND,		COLOUMN_STAND_WIDTH);
	mainTableWidget->setColumnWidth(COLOUMN_PC,			COLOUMN_PC_WIDTH);
	mainTableWidget->setColumnWidth(COLOUMN_MORE,		COLOUMN_MORE_WIDTH);

	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLOUMN_CONNECTOR,	QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLOUMN_PIN,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLOUMN_NAME,			QHeaderView::Stretch);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLOUMN_TYPE,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLOUMN_STAND,		QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLOUMN_PC,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(COLOUMN_MORE,			QHeaderView::Fixed);
}

void TestWindow::resetTableHeaderLanguageInManualTestAutoStand()
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
		mainTableHeaderLabels->push_back("Stand");
		mainTableHeaderLabels->push_back("PC");
		mainTableHeaderLabels->push_back("");
		break;
	}
	mainTableWidget->setHorizontalHeaderLabels(*mainTableHeaderLabels);
}

void TestWindow::initUiTableRowsInManualTestAutoStand()
{
	QAbstractItemModel* model = mainTableWidget->model();
	for (int currentRowNum = 0; currentRowNum < cableRows.size(); currentRowNum++)
	{
		model->setData(model->index(currentRowNum, COLOUMN_CONNECTOR), cableRows[currentRowNum]->connector);
		model->setData(model->index(currentRowNum, COLOUMN_PIN), cableRows[currentRowNum]->pin);
		model->setData(model->index(currentRowNum, COLOUMN_NAME), cableRows[currentRowNum]->name);
		model->setData(model->index(currentRowNum, COLOUMN_TYPE), cableRows[currentRowNum]->type);

		QWidget* moreCellWidget = new QWidget(mainLayoutWidget);
		initMoreButton(currentRowNum, moreCellWidget);
		mainTableWidget->setCellWidget(currentRowNum, COLOUMN_MORE, moreCellWidget);
	}
}


void TestWindow::on_inManualTestAutoStandTestTimeComboBox_changed(int ind)
{
}