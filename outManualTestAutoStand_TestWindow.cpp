#include "TestWindow.h"

#define COLOUMN_TYPE		3
#define COLOUMN_CHECK		4
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
	mainTableWidget->setRowCount(cableRows.size());
	mainTableWidget->setColumnCount(8);
	mainTableWidget->setHorizontalHeaderLabels(QStringList() << QString::fromLocal8Bit("Разъём")
		<< QString::fromLocal8Bit("Пин")
		<< QString::fromLocal8Bit("Название")
		<< QString::fromLocal8Bit("Тип")
		<< QString::fromLocal8Bit("Проверка")
		<< QString::fromLocal8Bit("Стенд")
		<< QString::fromLocal8Bit("ПК")
		<< "");

	mainTableWidget->setColumnWidth(COLOUMN_MORE, 25);

	QAbstractItemModel* model = mainTableWidget->model();
	for (int currentRowNum = 0; currentRowNum < cableRows.size(); currentRowNum++)
	{
		qDebug() << cableRows[currentRowNum]->connector << cableRows[currentRowNum]->pin << cableRows[currentRowNum]->name << cableRows[currentRowNum]->direction << cableRows[currentRowNum]->type;

		model->setData(model->index(currentRowNum, COLOUMN_CONNECTOR), cableRows[currentRowNum]->connector);
		model->setData(model->index(currentRowNum, COLOUMN_PIN), cableRows[currentRowNum]->pin);
		model->setData(model->index(currentRowNum, COLOUMN_NAME), cableRows[currentRowNum]->name);
		model->setData(model->index(currentRowNum, COLOUMN_TYPE), cableRows[currentRowNum]->type);

		QWidget* interactionButtonsWidget = new QWidget(mainLayoutWidget);
		QWidget* moreCellWidget = new QWidget(mainLayoutWidget);
		initTableRowButtons(currentRowNum, interactionButtonsWidget, moreCellWidget);
		mainTableWidget->setCellWidget(currentRowNum, COLOUMN_CHECK, interactionButtonsWidget);
		mainTableWidget->setCellWidget(currentRowNum, COLOUMN_MORE, moreCellWidget);
	}
}

void TestWindow::on_outManualTestAutoStandConnectButton_clicked()
{
}

void TestWindow::on_outManualTestAutoStandTestTimeComboBox_changed(int ind)
{
}