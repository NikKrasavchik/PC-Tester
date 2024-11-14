#include "TestWindow.h"

#define COLOUMN_TYPE	3

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
	mainTableWidget->setRowCount(cables.size());
	mainTableWidget->setColumnCount(7);
	mainTableWidget->setHorizontalHeaderLabels(QStringList() << QString::fromLocal8Bit("Разъём")
															<< QString::fromLocal8Bit("Пин")
															<< QString::fromLocal8Bit("Название")
															<< QString::fromLocal8Bit("Тип")
															<< QString::fromLocal8Bit("Стенд")
															<< QString::fromLocal8Bit("ПК")
															<< QString::fromLocal8Bit("Подробнее"));

	QAbstractItemModel* model = mainTableWidget->model();
	for (int i = 0; i < cables.size(); i++)
	{
		model->setData(model->index(i, COLOUMN_CONNECTOR), QString((char)(PRIMARY_CONNECTOR_SYMBOL + (int)cables[i].connector)));
		model->setData(model->index(i, COLOUMN_PIN), QString::number(cables[i].pin));
		model->setData(model->index(i, COLOUMN_NAME), cables[i].name);

		QString type;
		switch (cables[i].type)
		{
		case TYPE_DIGITAL:
			type = "DIGITAL";
			break;

		case TYPE_ANALOG:
			type = "ANALOG";
			break;

		case TYPE_HALL:
			type = "HALL";
			break;
		}
		model->setData(model->index(i, COLOUMN_TYPE), type);
	}
}

void TestWindow::on_inManualTestAutoStandConnectButton_clicked()
{

}

void TestWindow::on_inManualTestAutoStandTestTimeComboBox_changed(int ind)
{

}