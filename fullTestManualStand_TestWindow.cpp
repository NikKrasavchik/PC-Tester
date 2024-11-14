#include "TestWindow.h"

#define COLOUMN_DIRECTION	3
#define COLOUMN_TYPE		4

void TestWindow::initUiFullTestManualStand()
{
	fullTestManualStandSortButton = new QPushButton(usefulSpaceWidget);
	fullTestManualStandSortButton->setObjectName("fullTestManualStandSortButton");
	fullTestManualStandSortButton->setFixedSize(QSize(FIXED_HEADER_BUTTON_WIDTH, FIXED_HEADER_BUTTON_HEIGHT));
	usefulSpaceHLayout->addWidget(fullTestManualStandSortButton);

	initUiTableFullTestManualStand();
}

void TestWindow::initUiTableFullTestManualStand()
{
	mainTableWidget->setRowCount(cables.size());
	mainTableWidget->setColumnCount(8);
	mainTableWidget->setHorizontalHeaderLabels(QStringList() << QString::fromLocal8Bit("Разъём")
															<< QString::fromLocal8Bit("Пин")
															<< QString::fromLocal8Bit("Название")
															<< QString::fromLocal8Bit("Направленность") // ???
															<< QString::fromLocal8Bit("Тип")
															<< ""
															<< QString::fromLocal8Bit("Статус")
															<< QString::fromLocal8Bit("Подробнее"));

	QAbstractItemModel* model = mainTableWidget->model();
	for (int i = 0; i < cables.size(); i++)
	{
		model->setData(model->index(i, COLOUMN_CONNECTOR), QString((char)(PRIMARY_CONNECTOR_SYMBOL + (int)cables[i].connector)));
		model->setData(model->index(i, COLOUMN_PIN), QString::number(cables[i].pin));
		model->setData(model->index(i, COLOUMN_NAME), cables[i].name);

		QString direction;
		switch (cables[i].direction)
		{
		case DIRECTION_OUT:
			direction = "OUT";
			break;

		case DIRECTION_IN:
			direction = "IN";
			break;
		}
		model->setData(model->index(i, COLOUMN_NAME), direction);

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

void TestWindow::on_fullTestManualStandSortButton_clicked()
{

}