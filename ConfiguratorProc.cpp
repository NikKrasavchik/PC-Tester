#include "ConfiguratorWindow.h"

TableRowProperties::TableRowProperties(QObject* parent)
{
	direction = new QComboBox();
	type = new QComboBox();
	presetSettings = new PresetSettings();
	presetSettings->direction = DIRECTION_NOT_SET;
	presetSettings->type = TYPE_NOT_SET;

	connect(direction, SIGNAL(activated(int)), this, SLOT(on_direction_activated(int)));
	connect(type, SIGNAL(activated(int)), this, SLOT(on_type_activated(int)));
}

void ConfiguratorWindow::on_addRowButton_clicked()
{
	int currentRowNum = mainTableWidget->rowCount();
	mainTableWidget->insertRow(currentRowNum);

	tableRowPropertiesVector.push_back(new TableRowProperties());
	TableRowProperties* currentRowProperties = tableRowPropertiesVector[currentRowNum];

	currentRowProperties->id = currentRowNum;
	//connect(currentRowProperties->direction, &QComboBox::activated, currentRowProperties, &TableRowProperties::on_direction_activated);

	QWidget* directionCellWidget = new QWidget(mainLayoutWidget);
	QHBoxLayout* directionCellLayout = new QHBoxLayout(directionCellWidget);
	directionCellLayout->addWidget(currentRowProperties->direction);
	directionCellLayout->setContentsMargins(0, 0, 0, 0);
	directionCellWidget->setLayout(directionCellLayout);

	QWidget* typeCellWidget = new QWidget(mainLayoutWidget);
	QHBoxLayout* typeCellLayout = new QHBoxLayout(typeCellWidget);
	typeCellLayout->addWidget(currentRowProperties->type);
	typeCellLayout->setContentsMargins(0, 0, 0, 0);
	typeCellWidget->setLayout(typeCellLayout);

	mainTableWidget->setCellWidget(currentRowNum, (int)RowName::DIRECTION,	directionCellWidget);
	mainTableWidget->setCellWidget(currentRowNum, (int)RowName::TYPE,		typeCellWidget);

	resetPreset();
}

void TableRowProperties::on_direction_activated(int index)
{
	presetSettings->direction = index;
}

void TableRowProperties::on_type_activated(int index)
{
	presetSettings->type = index;
}

void ConfiguratorWindow::resetPreset()
{
	for (int i = 0; i < tableRowPropertiesVector.size(); i++)
	{
		TableRowProperties* currentRowProperties = tableRowPropertiesVector[i];
		switch (viewWindowState->appLanguage)
		{
		case RUSSIAN_LANG:
			currentRowProperties->direction->clear();
			currentRowProperties->direction->addItem(DIRECTION_NOT_SET_RU);
			currentRowProperties->direction->addItem(DIRECTION_OUT_RU);
			currentRowProperties->direction->addItem(DIRECTION_IN_RU);

			currentRowProperties->type->clear();
			currentRowProperties->type->addItem(TYPE_NOT_SET_RU);
			currentRowProperties->type->addItem(TYPE_MANUAL_RU);
			currentRowProperties->type->addItem(TYPE_AUTO_RU);
			break;

		case ENGLISH_LANG:
			currentRowProperties->direction->clear();
			currentRowProperties->direction->addItem(DIRECTION_NOT_SET_EN);
			currentRowProperties->direction->addItem(DIRECTION_OUT_EN);
			currentRowProperties->direction->addItem(DIRECTION_IN_EN);

			currentRowProperties->type->clear();
			currentRowProperties->type->addItem(TYPE_NOT_SET_EN);
			currentRowProperties->type->addItem(TYPE_MANUAL_EN);
			currentRowProperties->type->addItem(TYPE_AUTO_EN);
			break;
		}
		currentRowProperties->direction->setCurrentIndex(currentRowProperties->presetSettings->direction);
		currentRowProperties->type->setCurrentIndex(currentRowProperties->presetSettings->type);
	}
}