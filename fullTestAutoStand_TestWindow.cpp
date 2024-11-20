#include "TestWindow.h"

#define COLOUMN_DIRECTION	3
#define COLOUMN_TYPE		4
#define COLOUMN_STAND		5
#define COLOUMN_MORE		7

void TestWindow::initUiFullTestAutoStand()
{
	fullTestAutoStandConnectButton = new QPushButton(usefulSpaceWidget);
	fullTestAutoStandConnectButton->setObjectName("fullTestAutoStandConnectButton");
	fullTestAutoStandConnectButton->setFixedSize(QSize(FIXED_HEADER_BUTTON_WIDTH, FIXED_HEADER_BUTTON_HEIGHT));
	usefulSpaceHLayout->addWidget(fullTestAutoStandConnectButton);

	fullTestAutoStandStartTestButton = new QPushButton(usefulSpaceWidget);
	fullTestAutoStandStartTestButton->setObjectName("fullTestAutoStandStartTestButton");
	fullTestAutoStandStartTestButton->setFixedSize(QSize(FIXED_HEADER_BUTTON_WIDTH, FIXED_HEADER_BUTTON_HEIGHT));
	usefulSpaceHLayout->addWidget(fullTestAutoStandStartTestButton);

	fullTestAutoStandSortButton = new QPushButton(usefulSpaceWidget);
	fullTestAutoStandSortButton->setObjectName("fullTestAutoStandSortButton");
	fullTestAutoStandSortButton->setFixedSize(QSize(FIXED_HEADER_BUTTON_WIDTH, FIXED_HEADER_BUTTON_HEIGHT));
	usefulSpaceHLayout->addWidget(fullTestAutoStandSortButton);
	fullTestAutoStandTypeSort = false;
	
	initUiTableFullTestAutoStand();
}

void TestWindow::initUiTableFullTestAutoStand()
{
	mainTableWidget->setRowCount(cableRows.size());
	mainTableWidget->setColumnCount(8);
	mainTableWidget->setHorizontalHeaderLabels(QStringList() << QString::fromLocal8Bit("Разъём")
		<< QString::fromLocal8Bit("Пин")
		<< QString::fromLocal8Bit("Название")
		<< QString::fromLocal8Bit("Направленность") // ???
		<< QString::fromLocal8Bit("Тип")
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
		model->setData(model->index(currentRowNum, COLOUMN_DIRECTION), cableRows[currentRowNum]->direction);
		model->setData(model->index(currentRowNum, COLOUMN_TYPE), cableRows[currentRowNum]->type);

		QWidget* interactionButtonsWidget = new QWidget(mainLayoutWidget);
		QWidget* moreCellWidget = new QWidget(mainLayoutWidget);
		initTableRowButtons(currentRowNum, interactionButtonsWidget, moreCellWidget);
		mainTableWidget->setCellWidget(currentRowNum, COLOUMN_STAND, interactionButtonsWidget);
		mainTableWidget->setCellWidget(currentRowNum, COLOUMN_MORE, moreCellWidget);
	}
}

void TestWindow::on_fullTestAutoStandConnectButton_clicked()
{
}

void TestWindow::on_fullTestAutoStandStartTestButton_clicked()
{
}

void TestWindow::on_fullTestAutoStandSortButton_clicked()
{
	fullTestAutoStandTypeSort = !fullTestAutoStandTypeSort;

	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		if (fullTestAutoStandTypeSort)
			fullTestAutoStandSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо типу"));
		else
			fullTestAutoStandSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо нумерации"));
		break;
	case ENGLISH_LANG:
		if (fullTestAutoStandTypeSort)
			fullTestAutoStandSortButton->setText(QString("Sort:\ntype"));
		else
			fullTestAutoStandSortButton->setText(QString("Sort:\nnum"));
		break;
	}
}