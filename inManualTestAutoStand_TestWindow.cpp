#include "TestWindow.h"

#define COLOUMN_TYPE	3
#define COLOUMN_MORE	6

void TestWindow::initUiInManualTestAutoStand()
{
	inManualTestAutoStandConnectButton = new QPushButton(usefulSpaceWidget);
	inManualTestAutoStandConnectButton->setObjectName("inManualTestAutoStandConnectButton");
	inManualTestAutoStandConnectButton->setFixedSize(QSize(FIXED_HEADER_BUTTON_WIDTH, FIXED_HEADER_BUTTON_HEIGHT));
	usefulSpaceHLayout->addWidget(inManualTestAutoStandConnectButton);

	inManualTestAutoStandTestTimeComboBox = new QComboBox(usefulSpaceWidget);
	inManualTestAutoStandTestTimeComboBox->setObjectName("inManualTestAutoStandTestTimeComboBox");
	inManualTestAutoStandTestTimeComboBox->setFixedSize(QSize(FIXED_HEADER_COMBO_WIDTH, FIXED_HEADER_COMBO_HEIGHT));
	usefulSpaceHLayout->addWidget(inManualTestAutoStandTestTimeComboBox);

	initUiTableInManualTestAutoStand();
}

void TestWindow::initUiTableInManualTestAutoStand()
{
	mainTableWidget->setRowCount(cableRows.size());
	mainTableWidget->setColumnCount(7);
	mainTableWidget->setHorizontalHeaderLabels(QStringList() << QString::fromLocal8Bit("Разъём")
															<< QString::fromLocal8Bit("Пин")
															<< QString::fromLocal8Bit("Название")
															<< QString::fromLocal8Bit("Тип")
															<< QString::fromLocal8Bit("Стенд")
															<< QString::fromLocal8Bit("ПК")
															<< QString::fromLocal8Bit("Подробнее"));

	QAbstractItemModel* model = mainTableWidget->model();
	for (int currentRowNum = 0; currentRowNum < cableRows.size(); currentRowNum++)
	{
		model->setData(model->index(currentRowNum, COLOUMN_CONNECTOR), cableRows[currentRowNum]->connector);
		model->setData(model->index(currentRowNum, COLOUMN_PIN), cableRows[currentRowNum]->pin);
		model->setData(model->index(currentRowNum, COLOUMN_NAME), cableRows[currentRowNum]->name);
		model->setData(model->index(currentRowNum, COLOUMN_TYPE), cableRows[currentRowNum]->type);

		cableRows[currentRowNum]->moreButton = new QPushButton(mainLayoutWidget);
		cableRows[currentRowNum]->moreButton->setObjectName("moreButton");

		QWidget* moreCellWidget = new QWidget(mainLayoutWidget);
		moreCellWidget->setObjectName("deleteCellWidget");
		QHBoxLayout* moreCellLayout = new QHBoxLayout(moreCellWidget);
		moreCellLayout->setObjectName("deleteCellWidget");
		moreCellLayout->addWidget(cableRows[currentRowNum]->moreButton);
		moreCellLayout->setContentsMargins(0, 0, 0, 0);
		moreCellWidget->setLayout(moreCellLayout);

		mainTableWidget->setCellWidget(currentRowNum, COLOUMN_MORE, moreCellWidget);
	}
}

void TestWindow::on_inManualTestAutoStandConnectButton_clicked()
{

}

void TestWindow::on_inManualTestAutoStandTestTimeComboBox_changed(int ind)
{

}