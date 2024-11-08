#include "ConfiguratorWindow.h"

TableRowProperties::TableRowProperties(QObject* parent)
{
	connectorComboBox = new QComboBox();
	directionComboBox = new QComboBox();
	typeComboBox = new QComboBox();
	presetSettings = new PresetSettings();
	deleteButton = new QPushButton();

	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		connectorComboBox->addItem(QString::fromLocal8Bit("Не выбрано"));
		break;

	case ENGLISH_LANG:
		connectorComboBox->addItem("Not set");
		break;
	}
	connectorComboBox->addItem("A");
	connectorComboBox->addItem("B");
	connectorComboBox->addItem("C");
	connectorComboBox->addItem("D");
	connectorComboBox->addItem("E");
	connectorComboBox->addItem("F");

	presetSettings->direction = DIRECTION_NOT_SET;
	presetSettings->type = TYPE_NOT_SET;
	presetSettings->connector = ConnectorId::NOT_SET;

	connect(directionComboBox, SIGNAL(activated(int)), this, SLOT(on_direction_activated(int)));
	connect(typeComboBox, SIGNAL(activated(int)), this, SLOT(on_type_activated(int)));
	connect(deleteButton, &QPushButton::clicked, this, &TableRowProperties::on_deleteButton_clicked);
}

void ConfiguratorWindow::on_addRowButton_clicked()
{
	int currentRowNum = mainTableWidget->rowCount();
	mainTableWidget->insertRow(currentRowNum);

	tableRowPropertiesVector.push_back(new TableRowProperties());
	TableRowProperties* currentRowProperties = tableRowPropertiesVector[currentRowNum];
	currentRowProperties->id = currentRowNum;
	connect(currentRowProperties, &TableRowProperties::resetRowPreset, this, &ConfiguratorWindow::resetRowPreset);
	connect(currentRowProperties, &TableRowProperties::deleteRow, this, &ConfiguratorWindow::deleteRow);

	QWidget* connectorCellWidget = new QWidget(mainLayoutWidget);
	connectorCellWidget->setObjectName("connectorIdCellWidget");
	QHBoxLayout* connectorCellLayout = new QHBoxLayout(connectorCellWidget);
	connectorCellLayout->setObjectName("connectorCellLayout");
	connectorCellLayout->addWidget(currentRowProperties->connectorComboBox);
	connectorCellLayout->setContentsMargins(0, 0, 0, 0);
	connectorCellWidget->setLayout(connectorCellLayout);

	QWidget* directionCellWidget = new QWidget(mainLayoutWidget);
	directionCellWidget->setObjectName("directionCellWidget");
	QHBoxLayout* directionCellLayout = new QHBoxLayout(directionCellWidget);
	directionCellLayout->setObjectName("directionCellLayout");
	directionCellLayout->addWidget(currentRowProperties->directionComboBox);
	directionCellLayout->setContentsMargins(0, 0, 0, 0);
	directionCellWidget->setLayout(directionCellLayout);

	QWidget* typeCellWidget = new QWidget(mainLayoutWidget);
	typeCellWidget->setObjectName("typeCellWidget");
	QHBoxLayout* typeCellLayout = new QHBoxLayout(typeCellWidget);
	typeCellLayout->setObjectName("typeCellLayout");
	typeCellLayout->addWidget(currentRowProperties->typeComboBox);
	typeCellLayout->setContentsMargins(0, 0, 0, 0);
	typeCellWidget->setLayout(typeCellLayout);

	QWidget* deleteCellWidget = new QWidget(mainLayoutWidget);
	deleteCellWidget->setObjectName("deleteCellWidget");
	QHBoxLayout* deleteCellLayout = new QHBoxLayout(deleteCellWidget);
	deleteCellLayout->setObjectName("deleteCellWidget");
	deleteCellLayout->addWidget(currentRowProperties->deleteButton);
	deleteCellLayout->setContentsMargins(0, 0, 0, 0);
	deleteCellWidget->setLayout(typeCellLayout);

	mainTableWidget->setCellWidget(currentRowNum, (int)RowName::CONNECTOR,	connectorCellWidget);
	mainTableWidget->setCellWidget(currentRowNum, (int)RowName::DIRECTION,	directionCellWidget);
	mainTableWidget->setCellWidget(currentRowNum, (int)RowName::TYPE,		typeCellWidget);
	mainTableWidget->setCellWidget(currentRowNum, (int)RowName::DELETE,		deleteCellWidget);

	resetPresets();
}

void TableRowProperties::on_direction_activated(int index)
{
	switch (directionComboBox->count())
	{
	case DIRECTION_SET_NUM:
		presetSettings->direction = index;
		presetSettings->type = TYPE_NOT_SET_DIR_SET;
		break;
		
	case DIRECTION_NOT_SET_NUM:
		presetSettings->direction = index - 1;

		if (presetSettings->direction != DIRECTION_NOT_SET)
			if (presetSettings->type == TYPE_NOT_SET)
				presetSettings->type = TYPE_NOT_SET_DIR_SET;

		break;
	}
		
	resetRowPreset(this);
}

void TableRowProperties::on_type_activated(int index)
{
	switch (typeComboBox->count())
	{
	case TYPE_SET_DIR_SET_NUM:
		switch (presetSettings->direction)
		{
		case DIRECTION_OUT:
			switch (index)
			{
			case 0:
				presetSettings->type = TYPE_DIGITAL;
				break;

			case 1:
				presetSettings->type = TYPE_PWM;
				break;

			case 2:
				presetSettings->type = TYPE_VNH;
				break;
			}
			break;

		case DIRECTION_IN:
			switch (index)
			{
			case 0:
				presetSettings->type = TYPE_DIGITAL;
				break;

			case 1:
				presetSettings->type = TYPE_ANALOG;
				break;

			case 2:
				presetSettings->type = TYPE_HALL;
				break;
			}
			break;
		}
		break;

	case TYPE_NOTSET_DIR_SET_NUM:
		switch (presetSettings->direction)
		{
		case DIRECTION_OUT:
			switch (index)
			{
			case 0:
				presetSettings->type = TYPE_NOT_SET_DIR_SET;
				break;

			case 1:
				presetSettings->type = TYPE_DIGITAL;
				break;

			case 2:
				presetSettings->type = TYPE_PWM;
				break;

			case 3:
				presetSettings->type = TYPE_VNH;
				break;
			}
			break;

		case DIRECTION_IN:
			switch (index)
			{
			case 0:
				presetSettings->type = TYPE_NOT_SET_DIR_SET;
				break;

			case 1:
				presetSettings->type = TYPE_DIGITAL;
				break;

			case 2:
				presetSettings->type = TYPE_ANALOG;
				break;

			case 3:
				presetSettings->type = TYPE_HALL;
				break;
			}
			break;
		}
		break;

	case TYPE_SET_NUM:
		switch (index)
		{
		case 0:
			presetSettings->type = TYPE_DIGITAL;
			break;

		case 1:
			presetSettings->type = TYPE_PWM;
			presetSettings->direction = DIRECTION_OUT;
			break;

		case 2:
			presetSettings->type = TYPE_VNH;
			presetSettings->direction = DIRECTION_OUT;
			break;

		case 3:
			presetSettings->type = TYPE_ANALOG;
			presetSettings->direction = DIRECTION_IN;
			break;

		case 4:
			presetSettings->type = TYPE_HALL;
			presetSettings->direction = DIRECTION_IN;
			break;
		}
		break;

	case TYPE_NOT_SET_NUM:
		switch (index)
		{
		case 0:
			presetSettings->type = TYPE_NOT_SET;
			break;

		case 1:
			presetSettings->type = TYPE_DIGITAL;
			break;

		case 2:
			presetSettings->type = TYPE_PWM;
			presetSettings->direction = DIRECTION_OUT;
			break;

		case 3:
			presetSettings->type = TYPE_VNH;
			presetSettings->direction = DIRECTION_OUT;
			break;

		case 4:
			presetSettings->type = TYPE_ANALOG;
			presetSettings->direction = DIRECTION_IN;
			break;

		case 5:
			presetSettings->type = TYPE_HALL;
			presetSettings->direction = DIRECTION_IN;
			break;
		}
		break;
	}

	resetRowPreset(this);
}

void TableRowProperties::on_deleteButton_clicked()
{
	delete directionComboBox;
	delete typeComboBox;
	delete presetSettings;
	delete deleteButton;

	deleteRow(id);
}

void ConfiguratorWindow::deleteRow(int index)
{
	mainTableWidget->removeCellWidget(index, (int)RowName::CONNECTOR);
	mainTableWidget->removeCellWidget(index, (int)RowName::DIRECTION);
	mainTableWidget->removeCellWidget(index, (int)RowName::TYPE);
	mainTableWidget->removeCellWidget(index, (int)RowName::DELETE);

	mainTableWidget->removeRow(index);
	tableRowPropertiesVector.erase(tableRowPropertiesVector.begin() + index);

	for (int i = index; i < tableRowPropertiesVector.size(); i++)
		tableRowPropertiesVector[i]->id--;
}

void ConfiguratorWindow::resetRowPreset(TableRowProperties* currentRowProperties)
{
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		currentRowProperties->connectorComboBox->setItemText(0, QString::fromLocal8Bit("Не выбрано"));

		currentRowProperties->directionComboBox->clear();
		if (currentRowProperties->presetSettings->direction == DIRECTION_NOT_SET)
			currentRowProperties->directionComboBox->addItem(DIRECTION_NOT_SET_RU);
		currentRowProperties->directionComboBox->addItem(DIRECTION_OUT_RU);
		currentRowProperties->directionComboBox->addItem(DIRECTION_IN_RU);

		switch (currentRowProperties->presetSettings->direction)
		{
		case DIRECTION_NOT_SET:
		case DIRECTION_OUT:
			currentRowProperties->directionComboBox->setCurrentIndex(0);
			break;
		case DIRECTION_IN:
			currentRowProperties->directionComboBox->setCurrentIndex(1);
			break;
		}

		currentRowProperties->typeComboBox->clear();
		if (currentRowProperties->presetSettings->type == TYPE_NOT_SET || currentRowProperties->presetSettings->type == TYPE_NOT_SET_DIR_SET)
			currentRowProperties->typeComboBox->addItem(TYPE_NOT_SET_RU);
		currentRowProperties->typeComboBox->addItem(TYPE_DIGITAL_RU);
		if (currentRowProperties->presetSettings->direction != DIRECTION_IN)
		{
			currentRowProperties->typeComboBox->addItem(TYPE_PWM_RU);
			currentRowProperties->typeComboBox->addItem(TYPE_VNH_RU);
		}
		if (currentRowProperties->presetSettings->direction != DIRECTION_OUT)
		{
			currentRowProperties->typeComboBox->addItem(TYPE_ANALOG_RU);
			currentRowProperties->typeComboBox->addItem(TYPE_HALL_RU);
		}

		switch (currentRowProperties->presetSettings->type)
		{
		case TYPE_NOT_SET:
		case TYPE_DIGITAL:
			currentRowProperties->typeComboBox->setCurrentIndex(0);
			break;

		case TYPE_PWM:
			currentRowProperties->typeComboBox->setCurrentIndex(1);
			break;

		case TYPE_VNH:
			currentRowProperties->typeComboBox->setCurrentIndex(2);
			break;

		case TYPE_ANALOG:
			currentRowProperties->typeComboBox->setCurrentIndex(1);
			break;

		case TYPE_HALL:
			currentRowProperties->typeComboBox->setCurrentIndex(2);
			break;
		}
		break;

	case ENGLISH_LANG:
		currentRowProperties->connectorComboBox->setItemText(0, QString("Not set"));

		currentRowProperties->directionComboBox->clear();
		if (currentRowProperties->presetSettings->direction == DIRECTION_NOT_SET)
			currentRowProperties->directionComboBox->addItem(DIRECTION_NOT_SET_EN);
		currentRowProperties->directionComboBox->addItem(DIRECTION_OUT_EN);
		currentRowProperties->directionComboBox->addItem(DIRECTION_IN_EN);

		switch (currentRowProperties->presetSettings->direction)
		{
		case DIRECTION_NOT_SET:
		case DIRECTION_OUT:
			currentRowProperties->directionComboBox->setCurrentIndex(0);
			break;
		case DIRECTION_IN:
			currentRowProperties->directionComboBox->setCurrentIndex(1);
			break;
		}

		currentRowProperties->typeComboBox->clear();
		if (currentRowProperties->presetSettings->type == TYPE_NOT_SET || currentRowProperties->presetSettings->type == TYPE_NOT_SET_DIR_SET)
			currentRowProperties->typeComboBox->addItem(TYPE_NOT_SET_EN);
		currentRowProperties->typeComboBox->addItem(TYPE_DIGITAL_EN);
		if (currentRowProperties->presetSettings->direction != DIRECTION_IN)
		{
			currentRowProperties->typeComboBox->addItem(TYPE_PWM_EN);
			currentRowProperties->typeComboBox->addItem(TYPE_VNH_EN);
		}
		if (currentRowProperties->presetSettings->direction != DIRECTION_OUT)
		{
			currentRowProperties->typeComboBox->addItem(TYPE_ANALOG_EN);
			currentRowProperties->typeComboBox->addItem(TYPE_HALL_EN);
		}

		switch (currentRowProperties->presetSettings->type)
		{
		case TYPE_NOT_SET:
		case TYPE_DIGITAL:
			currentRowProperties->typeComboBox->setCurrentIndex(0);
			break;

		case TYPE_PWM:
			currentRowProperties->typeComboBox->setCurrentIndex(1);
			break;

		case TYPE_VNH:
			currentRowProperties->typeComboBox->setCurrentIndex(2);
			break;

		case TYPE_ANALOG:
			currentRowProperties->typeComboBox->setCurrentIndex(1);
			break;

		case TYPE_HALL:
			currentRowProperties->typeComboBox->setCurrentIndex(2);
			break;
		}
		break;
	}
}

void ConfiguratorWindow::resetPresets()
{
	for (int row = 0; row < tableRowPropertiesVector.size(); row++)
	{
		
		resetRowPreset(tableRowPropertiesVector[row]);
	}
}

void ConfiguratorWindow::on_saveButton_clicked()
{

}