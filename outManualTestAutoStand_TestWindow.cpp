#include "TestWindow.h"

#define COLOUMN_TYPE		3

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

	initUiTableOutManualTestAutoStand();
}

void TestWindow::initUiTableOutManualTestAutoStand()
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

		case TYPE_PWM:
			type = "PWM";
			break;

		case TYPE_VNH:
			type = "VNH";
			break;
		}
		model->setData(model->index(i, COLOUMN_TYPE), type);
	}
}

void TestWindow::on_outManualTestAutoStandConnectButton_clicked()
{

}

void TestWindow::on_outManualTestAutoStandTestTimeComboBox_changed(int ind)
{

}