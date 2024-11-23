#include "TestWindow.h"

#define COLOUMN_COUNT		8

#define COLOUMN_TYPE		3
#define COLOUMN_CHECK		4
#define COLOUMN_STAND		5
#define COLOUMN_PC			6
#define COLOUMN_MORE		7

void TestWindow::initUiOutManualTestAutoStand()
{
	outManualTestAutoStandConnectButton = new QPushButton(usefulSpaceWidget);
	outManualTestAutoStandConnectButton->setObjectName("outManualTestAutoStandConnectButton");
	outManualTestAutoStandConnectButton->setFixedSize(QSize(FIXED_HEADER_BUTTON_WIDTH, FIXED_HEADER_BUTTON_HEIGHT));
	usefulSpaceHLayout->addWidget(outManualTestAutoStandConnectButton);

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
	mainTableWidget->setHorizontalHeaderLabels(QStringList()
		<< QString::fromLocal8Bit("Разъём")
		<< QString::fromLocal8Bit("Пин")
		<< QString::fromLocal8Bit("Название")
		<< QString::fromLocal8Bit("Тип")
		<< QString::fromLocal8Bit("Проверка")
		<< QString::fromLocal8Bit("Стенд")
		<< QString::fromLocal8Bit("ПК")
		<< "");

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
		model->setData(model->index(currentRowNum, COLOUMN_TYPE), cableRows[currentRowNum]->type);

		QWidget* autoCheckCellWidget = new QWidget(mainLayoutWidget);
		QWidget* moreCellWidget = new QWidget(mainLayoutWidget);

		initAutoCheckButton(currentRowNum, autoCheckCellWidget);
		initMoreButton(currentRowNum, moreCellWidget);

		mainTableWidget->setCellWidget(currentRowNum, COLOUMN_CHECK, autoCheckCellWidget);
		mainTableWidget->setCellWidget(currentRowNum, COLOUMN_MORE, moreCellWidget);
	}
}

void TestWindow::on_outManualTestAutoStandConnectButton_clicked()
{
}

void TestWindow::on_outManualTestAutoStandTestTimeComboBox_changed(int ind)
{
}