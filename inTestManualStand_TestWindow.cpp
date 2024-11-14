#include "TestWindow.h"

#define COLOUMN_TYPE		3

void TestWindow::initUiInTestManualStand()
{
	initUiTableInTestManualStand();
}

void TestWindow::initUiTableInTestManualStand()
{
	mainTableWidget->setRowCount(cables.size());
	mainTableWidget->setColumnCount(6);
	mainTableWidget->setHorizontalHeaderLabels(QStringList() << QString::fromLocal8Bit("Разъём")
															<< QString::fromLocal8Bit("Пин")
															<< QString::fromLocal8Bit("Название")
															<< QString::fromLocal8Bit("Тип")
															<< QString::fromLocal8Bit("Статус")
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