#include "TestWindow.h"

#define COLOUMN_TYPE		3

void TestWindow::initUiOutTestManualStand()
{
	initUiTableOutTestManualStand();
}

void TestWindow::initUiTableOutTestManualStand()
{
	mainTableWidget->setRowCount(cables.size());
	mainTableWidget->setColumnCount(8);
	mainTableWidget->setHorizontalHeaderLabels(QStringList() << QString::fromLocal8Bit("Разъём") 
															<< QString::fromLocal8Bit("Пин") 
															<< QString::fromLocal8Bit("Название") 
															<< QString::fromLocal8Bit("Тип") 
															<< "" 
															<< QString::fromLocal8Bit("Отправка (hex)")
															<< ""
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