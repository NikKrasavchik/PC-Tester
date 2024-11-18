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
		<< QString::fromLocal8Bit("Подробнее"));

	QAbstractItemModel* model = mainTableWidget->model();
	for (int currentRowNum = 0; currentRowNum < cableRows.size(); currentRowNum++)
	{
		model->setData(model->index(currentRowNum, COLOUMN_CONNECTOR), cableRows[currentRowNum]->connector);
		model->setData(model->index(currentRowNum, COLOUMN_PIN), cableRows[currentRowNum]->pin);
		model->setData(model->index(currentRowNum, COLOUMN_NAME), cableRows[currentRowNum]->name);
		model->setData(model->index(currentRowNum, COLOUMN_DIRECTION), cableRows[currentRowNum]->direction);
		model->setData(model->index(currentRowNum, COLOUMN_TYPE), cableRows[currentRowNum]->type);

		QWidget* interactionButtonsWidget = new QWidget(mainLayoutWidget);
		interactionButtonsWidget->setObjectName("interactionButtonsWidget");
		QVBoxLayout* interactionButtonsCellVLayout = new QVBoxLayout(interactionButtonsWidget);
		interactionButtonsCellVLayout->setObjectName("interactionButtonsCellVLayout");

		if (cableRows[currentRowNum]->type == "DIGITAL")
		{
			interactionButtonsCellVLayout->addWidget(((DigitalButtons*)cableRows[currentRowNum]->buttons)->onButton);
			interactionButtonsCellVLayout->addWidget(((DigitalButtons*)cableRows[currentRowNum]->buttons)->offButton);

			connect(((DigitalButtons*)cableRows[currentRowNum]->buttons)->onButton,		&QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_onButton_clicked);
			connect(((DigitalButtons*)cableRows[currentRowNum]->buttons)->offButton,	&QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_offButton_clicked);

			mainTableWidget->setRowHeight(currentRowNum, 50);
		}
		else if (cableRows[currentRowNum]->type == "PWM")
		{
			interactionButtonsCellVLayout->addWidget(((PWMButtons*)cableRows[currentRowNum]->buttons)->load0Button);
			interactionButtonsCellVLayout->addWidget(((PWMButtons*)cableRows[currentRowNum]->buttons)->load25Button);
			interactionButtonsCellVLayout->addWidget(((PWMButtons*)cableRows[currentRowNum]->buttons)->load50Button);
			interactionButtonsCellVLayout->addWidget(((PWMButtons*)cableRows[currentRowNum]->buttons)->load75Button);
			interactionButtonsCellVLayout->addWidget(((PWMButtons*)cableRows[currentRowNum]->buttons)->load100Button);

			connect(((PWMButtons*)cableRows[currentRowNum]->buttons)->load0Button,		&QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_load0Button_clicked);
			connect(((PWMButtons*)cableRows[currentRowNum]->buttons)->load25Button,		&QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_load25Button_clicked);
			connect(((PWMButtons*)cableRows[currentRowNum]->buttons)->load50Button,		&QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_load50Button_clicked);
			connect(((PWMButtons*)cableRows[currentRowNum]->buttons)->load75Button,		&QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_load75Button_clicked);
			connect(((PWMButtons*)cableRows[currentRowNum]->buttons)->load100Button,	&QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_load100Button_clicked);

			mainTableWidget->setRowHeight(currentRowNum, 100);
		}
		else if (cableRows[currentRowNum]->type == "VNH")
		{
			interactionButtonsCellVLayout->addWidget(((VNHButtons*)cableRows[currentRowNum]->buttons)->onButton);
			interactionButtonsCellVLayout->addWidget(((VNHButtons*)cableRows[currentRowNum]->buttons)->offButton);
			interactionButtonsCellVLayout->addWidget(((VNHButtons*)cableRows[currentRowNum]->buttons)->load0Button);
			interactionButtonsCellVLayout->addWidget(((VNHButtons*)cableRows[currentRowNum]->buttons)->load25Button);
			interactionButtonsCellVLayout->addWidget(((VNHButtons*)cableRows[currentRowNum]->buttons)->load50Button);
			interactionButtonsCellVLayout->addWidget(((VNHButtons*)cableRows[currentRowNum]->buttons)->load75Button);
			interactionButtonsCellVLayout->addWidget(((VNHButtons*)cableRows[currentRowNum]->buttons)->load100Button);

			connect(((VNHButtons*)cableRows[currentRowNum]->buttons)->onButton,			&QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_onButton_clicked);
			connect(((VNHButtons*)cableRows[currentRowNum]->buttons)->offButton,		&QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_offButton_clicked);
			connect(((VNHButtons*)cableRows[currentRowNum]->buttons)->load0Button,		&QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_load0Button_clicked);
			connect(((VNHButtons*)cableRows[currentRowNum]->buttons)->load25Button,		&QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_load25Button_clicked);
			connect(((VNHButtons*)cableRows[currentRowNum]->buttons)->load50Button,		&QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_load50Button_clicked);
			connect(((VNHButtons*)cableRows[currentRowNum]->buttons)->load75Button,		&QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_load75Button_clicked);
			connect(((VNHButtons*)cableRows[currentRowNum]->buttons)->load100Button,	&QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_load100Button_clicked);

			mainTableWidget->setRowHeight(currentRowNum, 150);
		}

		interactionButtonsCellVLayout->setContentsMargins(0, 0, 0, 0);
		interactionButtonsWidget->setLayout(interactionButtonsCellVLayout);

		cableRows[currentRowNum]->moreButton = new QPushButton(mainLayoutWidget);
		cableRows[currentRowNum]->moreButton->setObjectName("moreButton");

		QWidget* moreCellWidget = new QWidget(mainLayoutWidget);
		moreCellWidget->setObjectName("deleteCellWidget");
		QHBoxLayout* moreCellHLayout = new QHBoxLayout(moreCellWidget);
		moreCellHLayout->setObjectName("deleteCellWidget");
		moreCellHLayout->addWidget(cableRows[currentRowNum]->moreButton);
		moreCellHLayout->setContentsMargins(0, 0, 0, 0);
		moreCellWidget->setLayout(moreCellHLayout);

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