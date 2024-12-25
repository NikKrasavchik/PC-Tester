#include "ConfiguratorWindow.h"

#define VERIFIABLE				true
#define UNVERIFIABLE			false	

#define FILE_CFG_COUNT			2
#define FILE_DATA_COUNT			12

#define COLOUMN_COUNT_FULL		13

#define IND_CFG					0
#define IND_STAND_TYPE			1

#define MASK_CFG				"CFG"
#define MASK_STAND_MIXED		"MIXED"
#define MASK_STAND_MANUAL		"MANUAL"
#define MASK_STAND_AUTO			"AUTO"

#define EMPTY_FILLING			-1

TableRowProperties::TableRowProperties(QObject* parent)
{
	presetSettings = new PresetSettings();

	presetSettings->direction = DIRECTION_NOT_SET;
	presetSettings->type = TYPE_NOT_SET;
	presetSettings->connector = ConnectorId::NOT_SET;
}

TableRowProperties::~TableRowProperties()
{
	delete connectorComboBox;
	delete directionComboBox;
	delete typeComboBox;
	delete deleteButton;
	delete presetSettings;
}

void TableRowProperties::initComboBoxes()
{
	connectorComboBox = new QComboBox();
	directionComboBox = new QComboBox();
	typeComboBox = new QComboBox();
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

	connect(connectorComboBox, SIGNAL(activated(int)), this, SLOT(on_connector_activated(int)));
	connect(directionComboBox, SIGNAL(activated(int)), this, SLOT(on_direction_activated(int)));
	connect(typeComboBox, SIGNAL(activated(int)), this, SLOT(on_type_activated(int)));
	connect(deleteButton, &QPushButton::clicked, this, &TableRowProperties::on_deleteButton_clicked);
}

void ConfiguratorWindow::createNewRowProperties()
{
	int currentRowNum = mainTableWidget->rowCount();
	tableRowPropertiesVector.push_back(new TableRowProperties());
	tableRowPropertiesVector[currentRowNum]->id = currentRowNum;
	connect(tableRowPropertiesVector[currentRowNum], &TableRowProperties::resetRowPreset, this, &ConfiguratorWindow::resetRowPreset);
	connect(tableRowPropertiesVector[currentRowNum], &TableRowProperties::deleteRow, this, &ConfiguratorWindow::deleteRow);
}

void ConfiguratorWindow::createNewRow()
{
	int currentRowNum = mainTableWidget->rowCount();
	mainTableWidget->insertRow(currentRowNum);

	TableRowProperties* currentRowProperties = tableRowPropertiesVector[currentRowNum];
	currentRowProperties->initComboBoxes();

	currentRowProperties->connectorCellWidget = new QWidget(mainLayoutWidget);
	currentRowProperties->connectorCellWidget->setObjectName("connectorIdCellWidget");
	currentRowProperties->connectorCellLayout = new QHBoxLayout(currentRowProperties->connectorCellWidget);
	currentRowProperties->connectorCellLayout->setObjectName("connectorCellLayout");
	currentRowProperties->connectorCellLayout->addWidget(currentRowProperties->connectorComboBox);
	currentRowProperties->connectorCellLayout->setContentsMargins(0, 0, 0, 0);
	currentRowProperties->connectorCellWidget->setLayout(currentRowProperties->connectorCellLayout);

	currentRowProperties->directionCellWidget = new QWidget(mainLayoutWidget);
	currentRowProperties->directionCellWidget->setObjectName("directionCellWidget");
	currentRowProperties->directionCellLayout = new QHBoxLayout(currentRowProperties->directionCellWidget);
	currentRowProperties->directionCellLayout->setObjectName("directionCellLayout");
	currentRowProperties->directionCellLayout->addWidget(currentRowProperties->directionComboBox);
	currentRowProperties->directionCellLayout->setContentsMargins(0, 0, 0, 0);
	currentRowProperties->directionCellWidget->setLayout(currentRowProperties->directionCellLayout);

	currentRowProperties->typeCellWidget = new QWidget(mainLayoutWidget);
	currentRowProperties->typeCellWidget->setObjectName("typeCellWidget");
	currentRowProperties->typeCellLayout = new QHBoxLayout(currentRowProperties->typeCellWidget);
	currentRowProperties->typeCellLayout->setObjectName("typeCellLayout");
	currentRowProperties->typeCellLayout->addWidget(currentRowProperties->typeComboBox);
	currentRowProperties->typeCellLayout->setContentsMargins(0, 0, 0, 0);
	currentRowProperties->typeCellWidget->setLayout(currentRowProperties->typeCellLayout);

	currentRowProperties->deleteCellWidget = new QWidget(mainLayoutWidget);
	currentRowProperties->deleteCellWidget->setObjectName("deleteCellWidget");
	currentRowProperties->deleteCellLayout = new QHBoxLayout(currentRowProperties->deleteCellWidget);
	currentRowProperties->deleteCellLayout->setObjectName("deleteCellLayout");
	currentRowProperties->deleteCellLayout->addWidget(currentRowProperties->deleteButton);
	currentRowProperties->deleteCellLayout->setContentsMargins(0, 0, 0, 0);
	currentRowProperties->deleteCellWidget->setLayout(currentRowProperties->deleteCellLayout);

	mainTableWidget->setCellWidget(currentRowNum, (int)FullColoumnName::CONNECTOR, currentRowProperties->connectorCellWidget);
	mainTableWidget->setCellWidget(currentRowNum, (int)FullColoumnName::DIRECTION, currentRowProperties->directionCellWidget);
	mainTableWidget->setCellWidget(currentRowNum, (int)FullColoumnName::TYPE, currentRowProperties->typeCellWidget);

	currentRowProperties->connectorComboBox->setCurrentIndex((int)(currentRowProperties->presetSettings->connector) + 1);
	currentRowProperties->directionComboBox->setCurrentIndex(currentRowProperties->presetSettings->direction + 1);
	currentRowProperties->typeComboBox->setCurrentIndex(currentRowProperties->presetSettings->type + 1);

	switch (selectStandTypeComboBox->currentIndex())
	{
	case STAND_NOT_SET:
		mainTableWidget->setCellWidget(currentRowNum, (int)FullColoumnName::DEL, currentRowProperties->deleteCellWidget);
		break;

	case STAND_MANUAL:
		mainTableWidget->setCellWidget(currentRowNum, (int)ManualColoumnName::DEL, currentRowProperties->deleteCellWidget);
		break;

	case STAND_AUTO:
		mainTableWidget->setCellWidget(currentRowNum, (int)AutoColoumnName::DEL, currentRowProperties->deleteCellWidget);
		break;
	}

	resetRowPreset(currentRowProperties);
}

void ConfiguratorWindow::on_addRowButton_clicked()
{
	createNewRowProperties();
	createNewRow();
}

void TableRowProperties::on_connector_activated(int index)
{
	presetSettings->connector = (ConnectorId)index;
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


void ConfiguratorWindow::resetRowPreset(TableRowProperties* currentRowProperties)
{
	currentRowProperties->connectorComboBox->setCurrentIndex((int)currentRowProperties->presetSettings->connector);

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
		case TYPE_ANALOG:
			currentRowProperties->typeComboBox->setCurrentIndex(1);
			break;

		case TYPE_VNH:
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
		case TYPE_ANALOG:
			currentRowProperties->typeComboBox->setCurrentIndex(1);
			break;

		case TYPE_VNH:
		case TYPE_HALL:
			currentRowProperties->typeComboBox->setCurrentIndex(2);
			break;
		}
		break;
	}
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
	switch (selectStandTypeComboBox->currentIndex())
	{
	case STAND_NOT_SET:
		mainTableWidget->removeCellWidget(index, (int)FullColoumnName::CONNECTOR);
		mainTableWidget->removeCellWidget(index, (int)FullColoumnName::DIRECTION);
		mainTableWidget->removeCellWidget(index, (int)FullColoumnName::TYPE);
		mainTableWidget->removeCellWidget(index, (int)FullColoumnName::DEL);
		break;

	case STAND_MANUAL:
		mainTableWidget->removeCellWidget(index, (int)ManualColoumnName::CONNECTOR);
		mainTableWidget->removeCellWidget(index, (int)ManualColoumnName::DIRECTION);
		mainTableWidget->removeCellWidget(index, (int)ManualColoumnName::TYPE);
		mainTableWidget->removeCellWidget(index, (int)ManualColoumnName::DEL);
		break;

	case STAND_AUTO:
		mainTableWidget->removeCellWidget(index, (int)AutoColoumnName::CONNECTOR);
		mainTableWidget->removeCellWidget(index, (int)AutoColoumnName::DIRECTION);
		mainTableWidget->removeCellWidget(index, (int)AutoColoumnName::TYPE);
		mainTableWidget->removeCellWidget(index, (int)AutoColoumnName::DEL);
		break;
	}

	mainTableWidget->removeRow(index);
	tableRowPropertiesVector.erase(tableRowPropertiesVector.begin() + index);

	for (int i = index; i < tableRowPropertiesVector.size(); i++)
		tableRowPropertiesVector[i]->id--;
}

void ConfiguratorWindow::resetPresets()
{
	for (int row = 0; row < tableRowPropertiesVector.size(); row++)
		resetRowPreset(tableRowPropertiesVector[row]);
}

void ConfiguratorWindow::on_saveButton_clicked()
{
	std::vector<std::vector<QString>> parsedData = parseData();
	if (!parsedData.size())
		return;

	QString configString = "CFG" + CFG_SPLIT;
	switch (selectStandTypeComboBox->currentIndex())
	{
	case 0:
		configString += "MIXED";
		break;

	case 1:
		configString += "MANUAL";
		break;

	case 2:
		configString += "AUTO";
		break;
	}
	configString += CFG_ENDING;

	for (int row = 0; row < mainTableWidget->rowCount(); row++)
	{
		for (int coloumn = 0; coloumn < COLOUMN_COUNT_FULL - 1; coloumn++)
			configString += parsedData[row][coloumn] + (coloumn == COLOUMN_COUNT_FULL - 2 ? "" : CFG_SPLIT);
		configString += (row == mainTableWidget->rowCount() - 1 ? "" : CFG_ENDING);
	}

	QString tempPath = QDir::currentPath() + "./Config files/";

	QString defaultFilter("");
	QString filters("Config files (*.csv)");

	QString filePath = QFileDialog::getSaveFileName(0, "Save file", tempPath, filters, &defaultFilter);

	std::ofstream fout;
	fout.open(filePath.toStdString());

	fout << configString.toStdString();


	qDebug() << configString;
}

std::vector<std::vector<QString>> ConfiguratorWindow::parseData()
{
	std::vector<std::vector<QString>> data;
	if (!updateTableData(VERIFIABLE))
		return data;

	for (int row = 0; row < mainTableWidget->rowCount(); row++)
	{
		std::vector<QString> rowData;
		for (int coloumn = 0; coloumn < COLOUMN_COUNT_FULL; coloumn++)
		{
			TableRowProperties* currentRowProperties = tableRowPropertiesVector[row];
			switch ((FullColoumnName)coloumn)
			{
			case FullColoumnName::CONNECTOR:
				rowData.push_back(QString::number((int)(currentRowProperties->presetSettings->connector)));
				break;

			case FullColoumnName::PIN:
				rowData.push_back(QString::number(currentRowProperties->pin));
				break;

			case FullColoumnName::DIRECTION:
				rowData.push_back(QString::number(currentRowProperties->presetSettings->direction));
				break;

			case FullColoumnName::TYPE:
				rowData.push_back(QString::number(currentRowProperties->presetSettings->type));
				break;

			case FullColoumnName::CAN_ID:
				if (currentRowProperties->canId == -1 || currentRowProperties->canId == "-")
					rowData.push_back("");
				else
					rowData.push_back(currentRowProperties->canId);
				break;

			case FullColoumnName::BYTE:
				rowData.push_back(currentRowProperties->byte != -1 ? QString::number(currentRowProperties->byte) : "");
				break;

			case FullColoumnName::MIN_CURRENT:
				rowData.push_back(QString::number(currentRowProperties->minCurrent));
				break;

			case FullColoumnName::MAX_CURRENT:
				rowData.push_back(QString::number(currentRowProperties->maxCurrent));
				break;

			case FullColoumnName::MIN_VOLTAGE:
				rowData.push_back(QString::number(currentRowProperties->minVoltage));
				break;

			case FullColoumnName::MAX_VOLTAGE:
				rowData.push_back(QString::number(currentRowProperties->maxVoltage));
				break;

			case FullColoumnName::NAME:
				rowData.push_back(currentRowProperties->name);
				break;

			case FullColoumnName::COMPONENT:
				rowData.push_back(currentRowProperties->component);
				break;

			case FullColoumnName::DEL:
				break;
			}
		}
		data.push_back(rowData);
	}
	return data;
}

bool ConfiguratorWindow::updateTableData(bool needVerify)
{
	for (int coloumn = 0; coloumn < mainTableWidget->columnCount(); coloumn++)
	{
		switch ((FullColoumnName)coloumn)
		{
		case FullColoumnName::CONNECTOR:
			for (int row = 0; row < mainTableWidget->rowCount(); row++)
				if (needVerify)
					if (!generateError(row, verifyTableData(coloumn, mainTableWidget->item(row, coloumn), tableRowPropertiesVector[row]->connectorComboBox)))
						return false;
				
			break;

		case FullColoumnName::PIN:
			for (int row = 0; row < mainTableWidget->rowCount(); row++)
				if (needVerify)
					if (generateError(row, verifyTableData(coloumn, mainTableWidget->item(row, coloumn))))
						tableRowPropertiesVector[row]->pin = (mainTableWidget->item(row, coloumn) != NULL ? mainTableWidget->item(row, coloumn)->text().toInt() : -1);
					else
						return false;
			break;

		case FullColoumnName::DIRECTION:
			for (int row = 0; row < mainTableWidget->rowCount(); row++)
				if (needVerify)
					if (!generateError(row, verifyTableData(coloumn, mainTableWidget->item(row, coloumn), tableRowPropertiesVector[row]->directionComboBox)))
						return false;
			break;

		case FullColoumnName::TYPE:
			for (int row = 0; row < mainTableWidget->rowCount(); row++)
				if (needVerify)
					if (!generateError(row, verifyTableData(coloumn, mainTableWidget->item(row, coloumn), tableRowPropertiesVector[row]->typeComboBox)))
						return false;
			break;
		}
		switch (standTypeSelected)
		{
		case STAND_NOT_SET:
			switch (coloumn)
			{
			case (int)FullColoumnName::CAN_ID:
				for (int row = 0; row < mainTableWidget->rowCount(); row++)
					if (needVerify)
					{
						if (generateError(row, verifyTableData(coloumn, mainTableWidget->item(row, coloumn))))
							tableRowPropertiesVector[row]->canId = (mainTableWidget->item(row, coloumn) != NULL ? mainTableWidget->item(row, coloumn)->text() : "");
						else
							return false;
					}
					else
						tableRowPropertiesVector[row]->canId = (mainTableWidget->item(row, coloumn) != NULL ? mainTableWidget->item(row, coloumn)->text() : "");
				break;

			case (int)FullColoumnName::BYTE:
				for (int row = 0; row < mainTableWidget->rowCount(); row++)
					if (needVerify)
					{
						if (generateError(row, verifyTableData(coloumn, mainTableWidget->item(row, coloumn))))
							if (mainTableWidget->item(row, coloumn) == NULL || mainTableWidget->item(row, coloumn)->text() == "-")
								tableRowPropertiesVector[row]->byte = -1;
							else
								tableRowPropertiesVector[row]->byte = mainTableWidget->item(row, coloumn)->text().toInt();

						else
							return false;
					}
					else
						tableRowPropertiesVector[row]->byte = (mainTableWidget->item(row, coloumn) != NULL ? mainTableWidget->item(row, coloumn)->text().toInt() : -1);
				break;

			case (int)FullColoumnName::MIN_CURRENT:
				for (int row = 0; row < mainTableWidget->rowCount(); row++)
					if (needVerify)
					{
						if (generateError(row, verifyTableData(coloumn, mainTableWidget->item(row, coloumn))))
							tableRowPropertiesVector[row]->minCurrent = mainTableWidget->item(row, coloumn)->text().toFloat();
						else
							return false;
					}
					else
						tableRowPropertiesVector[row]->minCurrent = (mainTableWidget->item(row, coloumn) != NULL ? mainTableWidget->item(row, coloumn)->text().toFloat() : -1);
				break;

			case (int)FullColoumnName::MAX_CURRENT:
				for (int row = 0; row < mainTableWidget->rowCount(); row++)
					if (needVerify)
					{
						if (generateError(row, verifyTableData(coloumn, mainTableWidget->item(row, coloumn))))
							tableRowPropertiesVector[row]->maxCurrent = mainTableWidget->item(row, coloumn)->text().toFloat();
						else
							return false;
					}
					else
						tableRowPropertiesVector[row]->maxCurrent = (mainTableWidget->item(row, coloumn) != NULL ? mainTableWidget->item(row, coloumn)->text().toFloat() : -1);
				break;

			case (int)FullColoumnName::MIN_VOLTAGE:
				for (int row = 0; row < mainTableWidget->rowCount(); row++)
					if (needVerify)
					{
						if (generateError(row, verifyTableData(coloumn, mainTableWidget->item(row, coloumn))))
							tableRowPropertiesVector[row]->minVoltage = mainTableWidget->item(row, coloumn)->text().toFloat();
						else
							return false;
					}
					else
						tableRowPropertiesVector[row]->minVoltage = (mainTableWidget->item(row, coloumn) != NULL ? mainTableWidget->item(row, coloumn)->text().toFloat() : -1);
				break;

			case (int)FullColoumnName::MAX_VOLTAGE:
				for (int row = 0; row < mainTableWidget->rowCount(); row++)
					if (needVerify)
					{
						if (generateError(row, verifyTableData(coloumn, mainTableWidget->item(row, coloumn))))
							tableRowPropertiesVector[row]->maxVoltage = mainTableWidget->item(row, coloumn)->text().toFloat();
						else
							return false;
					}
					else
						tableRowPropertiesVector[row]->maxVoltage = (mainTableWidget->item(row, coloumn) != NULL ? mainTableWidget->item(row, coloumn)->text().toFloat() : -1);
				break;

			case (int)FullColoumnName::NAME:
				for (int row = 0; row < mainTableWidget->rowCount(); row++)
					if (needVerify)
					{
						if (generateError(row, verifyTableData(coloumn, mainTableWidget->item(row, coloumn))))
							tableRowPropertiesVector[row]->name = mainTableWidget->item(row, coloumn)->text();
						else
							return false;
					}
					else
						tableRowPropertiesVector[row]->name = (mainTableWidget->item(row, coloumn) != NULL ? mainTableWidget->item(row, coloumn)->text() : "");
				break;

			case (int)FullColoumnName::COMPONENT:
				for (int row = 0; row < mainTableWidget->rowCount(); row++)
					if (needVerify)
					{
						if (generateError(row, verifyTableData(coloumn, mainTableWidget->item(row, coloumn))))
							tableRowPropertiesVector[row]->component = mainTableWidget->item(row, coloumn)->text();
						else
							return false;
					}
					else
						tableRowPropertiesVector[row]->component = (mainTableWidget->item(row, coloumn) != NULL ? mainTableWidget->item(row, coloumn)->text() : "");
				break;

			case (int)FullColoumnName::DEL:
				break;
			}
			break;

		case STAND_MANUAL:
			switch (coloumn)
			{
			case (int)ManualColoumnName::CAN_ID:
				for (int row = 0; row < mainTableWidget->rowCount(); row++)
					if (needVerify)
					{
						if (generateError(row, verifyTableData(coloumn, mainTableWidget->item(row, coloumn))))
							if (mainTableWidget->item(row, coloumn) == NULL || mainTableWidget->item(row, coloumn)->text() == "-")
								tableRowPropertiesVector[row]->byte = -1;
							else
								tableRowPropertiesVector[row]->byte = mainTableWidget->item(row, coloumn)->text().toInt();
						else
							return false;
					}
					else
						tableRowPropertiesVector[row]->canId = (mainTableWidget->item(row, coloumn) != NULL ? mainTableWidget->item(row, coloumn)->text() : "");
				break;

			case (int)ManualColoumnName::BYTE:
				for (int row = 0; row < mainTableWidget->rowCount(); row++)
					if (needVerify)
					{
						if (generateError(row, verifyTableData(coloumn, mainTableWidget->item(row, coloumn))))
							if (mainTableWidget->item(row, coloumn) == NULL || mainTableWidget->item(row, coloumn)->text() == "-")
								tableRowPropertiesVector[row]->byte = -1;
							else
								tableRowPropertiesVector[row]->byte = mainTableWidget->item(row, coloumn)->text().toInt();
						else
							return false;
					}
					else
						tableRowPropertiesVector[row]->byte = (mainTableWidget->item(row, coloumn) != NULL ? mainTableWidget->item(row, coloumn)->text().toInt() : -1);
				break;

			case (int)ManualColoumnName::NAME:
				for (int row = 0; row < mainTableWidget->rowCount(); row++)
					if (needVerify)
					{
						if (generateError(row, verifyTableData(coloumn, mainTableWidget->item(row, coloumn))))
							tableRowPropertiesVector[row]->name = mainTableWidget->item(row, coloumn)->text();
						else
							return false;
					}
					else
						tableRowPropertiesVector[row]->name = (mainTableWidget->item(row, coloumn) != NULL ? mainTableWidget->item(row, coloumn)->text() : "");
				break;
				
			case (int)ManualColoumnName::COMPONENT:
				for (int row = 0; row < mainTableWidget->rowCount(); row++)
					if (needVerify)
					{
						if (generateError(row, verifyTableData(coloumn, mainTableWidget->item(row, coloumn))))
							tableRowPropertiesVector[row]->component = mainTableWidget->item(row, coloumn)->text();
						else
							return false;
					}
					else
						tableRowPropertiesVector[row]->component = (mainTableWidget->item(row, coloumn) != NULL ? mainTableWidget->item(row, coloumn)->text() : "");
				break;

			case (int)ManualColoumnName::DEL:
				break;
			}
			break;

		case STAND_AUTO:
			switch (coloumn)
			{
			case (int)AutoColoumnName::MIN_CURRENT:
				for (int row = 0; row < mainTableWidget->rowCount(); row++)
					if (needVerify)
					{
						if (generateError(row, verifyTableData(coloumn, mainTableWidget->item(row, coloumn))))
							tableRowPropertiesVector[row]->minCurrent = mainTableWidget->item(row, coloumn)->text().toFloat();
						else
							return false;
					}
					else
						tableRowPropertiesVector[row]->minCurrent = (mainTableWidget->item(row, coloumn) != NULL ? mainTableWidget->item(row, coloumn)->text().toFloat() : -1);
				break;

			case (int)AutoColoumnName::MAX_CURRENT:
				for (int row = 0; row < mainTableWidget->rowCount(); row++)
					if (needVerify)
					{
						if (generateError(row, verifyTableData(coloumn, mainTableWidget->item(row, coloumn))))
							tableRowPropertiesVector[row]->maxCurrent = mainTableWidget->item(row, coloumn)->text().toFloat();
						else
							return false;
					}
					else
						tableRowPropertiesVector[row]->maxCurrent = (mainTableWidget->item(row, coloumn) != NULL ? mainTableWidget->item(row, coloumn)->text().toFloat() : -1);
				break;

			case (int)AutoColoumnName::MIN_VOLTAGE:
				for (int row = 0; row < mainTableWidget->rowCount(); row++)
					if (needVerify)
					{
						if (generateError(row, verifyTableData(coloumn, mainTableWidget->item(row, coloumn))))
							tableRowPropertiesVector[row]->minVoltage = mainTableWidget->item(row, coloumn)->text().toFloat();
						else
							return false;
					}
					else
						tableRowPropertiesVector[row]->minVoltage = (mainTableWidget->item(row, coloumn) != NULL ? mainTableWidget->item(row, coloumn)->text().toFloat() : -1);
				break;

			case (int)AutoColoumnName::MAX_VOLTAGE:
				for (int row = 0; row < mainTableWidget->rowCount(); row++)
					if (needVerify)
					{
						if (generateError(row, verifyTableData(coloumn, mainTableWidget->item(row, coloumn))))
							tableRowPropertiesVector[row]->maxVoltage = mainTableWidget->item(row, coloumn)->text().toFloat();
						else
							return false;
					}
					else
						tableRowPropertiesVector[row]->maxVoltage = (mainTableWidget->item(row, coloumn) != NULL ? mainTableWidget->item(row, coloumn)->text().toFloat() : -1);
				break;

			case (int)AutoColoumnName::NAME:
				for (int row = 0; row < mainTableWidget->rowCount(); row++)
					if (needVerify)
					{
						if (generateError(row, verifyTableData(coloumn, mainTableWidget->item(row, coloumn))))
							tableRowPropertiesVector[row]->name = mainTableWidget->item(row, coloumn)->text();
						else
							return false;
					}
					else
						tableRowPropertiesVector[row]->name = (mainTableWidget->item(row, coloumn) != NULL ? mainTableWidget->item(row, coloumn)->text() : "");
				break;

			case (int)AutoColoumnName::DEL:
				break;
			}
		}
	}
	return true;
}

Errors::Configurator ConfiguratorWindow::verifyTableData(int coloumnName, QTableWidgetItem* data, QComboBox* comboBox)
{
	bool isOk;
	QString hexPrefix = "0x";
	switch ((FullColoumnName)coloumnName)
	{
	case FullColoumnName::CONNECTOR:
		if (comboBox->currentIndex() == 0)
			return Errors::Configurator::SAVE_CONNECTOR_NOT_SET;
		break;

	case FullColoumnName::PIN:
		if (data == NULL)
			return Errors::Configurator::SAVE_PIN_NULL;
		bool isOk;
		data->text().toInt(&isOk);
		if (!isOk)
			return Errors::Configurator::SAVE_PIN_INCORRECT;
		break;

	case FullColoumnName::DIRECTION:
		if (comboBox->count() == 3)
			return Errors::Configurator::SAVE_DIRECTION_NOT_SET;
		break;

	case FullColoumnName::TYPE:
		if (comboBox->count() != 3)
			return Errors::Configurator::SAVE_TYPE_NOT_SET;
		break;
	}
	switch (standTypeSelected)
	{
	case STAND_NOT_SET:
		switch ((FullColoumnName)coloumnName)
		{
		case FullColoumnName::CAN_ID:
			if (data == NULL || data->text() == "-1" || data->text() == "-")
				break;
			else if (data->text()[0] == hexPrefix[0] && data->text()[1] == hexPrefix[1])
			{
				QString id = "";
				for (int i = 2; i < data->text().size(); i++)
					id += data->text()[i];

				data->text().toInt(&isOk, 16);
				if (!isOk)
					return Errors::Configurator::SAVE_CAN_ID_INCORRECT_HEX;
			}
			else
			{
				data->text().toInt(&isOk, 16);
				if (!isOk) 
				{
					data->text().toInt(&isOk, 10);
					if (!isOk)
						return Errors::Configurator::SAVE_CAN_ID_INCORRECT_DEC;
				}
			}
			break;

		case FullColoumnName::BYTE:
			if (data == NULL || data->text() == "-")
				break;
			data->text().toInt(&isOk);
			if (!isOk)
				return Errors::Configurator::SAVE_BYTE_INCORRECT;
			break;

		case FullColoumnName::MIN_CURRENT:
			if (data == NULL)
				return Errors::Configurator::SAVE_MIN_CURRENT_NULL;
			data->text().toFloat(&isOk);
			if (!isOk)
				return Errors::Configurator::SAVE_MIN_CURRENT_INCORRECT;
			break;

		case FullColoumnName::MAX_CURRENT:
			if (data == NULL)
				return Errors::Configurator::SAVE_MAX_CURRENT_NULL;
			data->text().toFloat(&isOk);
			if (!isOk)
				return Errors::Configurator::SAVE_MAX_CURRENT_INCORRECT;
			break;

		case FullColoumnName::MIN_VOLTAGE:
			if (data == NULL)
				return Errors::Configurator::SAVE_MIN_VOLTAGE_NULL;
			data->text().toFloat(&isOk);
			if (!isOk)
				return Errors::Configurator::SAVE_MIN_VOLTAGE_INCORRECT;
			break;

		case FullColoumnName::MAX_VOLTAGE:
			if (data == NULL)
				return Errors::Configurator::SAVE_MAX_VOLTAGE_NULL;
			data->text().toFloat(&isOk);
			if (!isOk)
				return Errors::Configurator::SAVE_MAX_VOLTAGE_INCORRECT;
			break;

		case FullColoumnName::NAME:
			if (data == NULL)
				return Errors::Configurator::SAVE_NAME_NULL;
			break;
			
		case FullColoumnName::COMPONENT:
			if (data == NULL)
				return Errors::Configurator::SAVE_COMPONENT_NULL;
			break;

		case FullColoumnName::DEL:
			break;
		}
		break;

	case STAND_MANUAL:
		switch ((ManualColoumnName)coloumnName)
		{
		case ManualColoumnName::CAN_ID:
			if (data == NULL || data->text() == "-1" || data->text() == "-")
				break;
			else if (data->text()[0] == hexPrefix[0] && data->text()[1] == hexPrefix[1])
			{
				QString id = "";
				for (int i = 2; i < data->text().size(); i++)
					id += data->text()[i];

				data->text().toInt(&isOk, 16);
				if (!isOk)
					return Errors::Configurator::SAVE_CAN_ID_INCORRECT_HEX;
			}
			else
			{
				data->text().toInt(&isOk, 16);
				if (!isOk)
				{
					data->text().toInt(&isOk, 10);
					if (!isOk)
						return Errors::Configurator::SAVE_CAN_ID_INCORRECT_DEC;
				}
			}
			break;

		case ManualColoumnName::BYTE:
			if (data == NULL || data->text() == "-")
				break;
			data->text().toInt(&isOk);
			if (!isOk)
				return Errors::Configurator::SAVE_BYTE_INCORRECT;
			break;

		case ManualColoumnName::NAME:
			if (data == NULL)
				return Errors::Configurator::SAVE_NAME_NULL;
			break;
			
		case ManualColoumnName::COMPONENT:
			if (data == NULL)
				return Errors::Configurator::SAVE_COMPONENT_NULL;
			break;

		case ManualColoumnName::DEL:
			break;
		}
		break;

	case STAND_AUTO:
		switch ((AutoColoumnName)coloumnName)
		{
		case AutoColoumnName::MIN_CURRENT:
			if (data == NULL)
				return Errors::Configurator::SAVE_MIN_CURRENT_NULL;
			data->text().toFloat(&isOk);
			if (!isOk)
				return Errors::Configurator::SAVE_MIN_CURRENT_INCORRECT;
			break;

		case AutoColoumnName::MAX_CURRENT:
			if (data == NULL)
				return Errors::Configurator::SAVE_MAX_CURRENT_NULL;
			data->text().toFloat(&isOk);
			if (!isOk)
				return Errors::Configurator::SAVE_MAX_CURRENT_INCORRECT;
			break;

		case AutoColoumnName::MIN_VOLTAGE:
			if (data == NULL)
				return Errors::Configurator::SAVE_MIN_VOLTAGE_NULL;
			data->text().toFloat(&isOk);
			if (!isOk)
				return Errors::Configurator::SAVE_MIN_VOLTAGE_INCORRECT;
			break;

		case AutoColoumnName::MAX_VOLTAGE:
			if (data == NULL)
				return Errors::Configurator::SAVE_MAX_VOLTAGE_NULL;
			data->text().toFloat(&isOk);
			if (!isOk)
				return Errors::Configurator::SAVE_MAX_VOLTAGE_INCORRECT;
			break;

		case AutoColoumnName::NAME:
			if (data == NULL)
				return Errors::Configurator::SAVE_NAME_NULL;
			break;

		case AutoColoumnName::DEL:
			break;
		}
	}
	return Errors::Configurator::CORRECT;
}

static Errors::Configurator verifyFileData(FullColoumnName coloumn, QString data)
{
	bool isOk;
	const char* hexPrefix = "0x";
	int connector;
	int direction;
	int type;
	switch (coloumn)
	{
	case FullColoumnName::CONNECTOR:
		connector = data.toInt(&isOk, 10);
		if (!isOk || !((int)ConnectorId::A <= connector && connector <= (int)ConnectorId::F))
			return Errors::Configurator::FILE_DATA_CONNECTOR_INCORRECT;
		break;

	case FullColoumnName::PIN:
		data.toInt(&isOk, 10);
		if (!isOk)
			return Errors::Configurator::FILE_DATA_PIN_INCORRECT;
		break;

	case FullColoumnName::DIRECTION:
		direction = data.toInt(&isOk, 10);
		if (!isOk || (direction != DIRECTION_OUT && direction != DIRECTION_IN))
			return Errors::Configurator::FILE_DATA_DIRECTION_INCORRECT;
		break;

	case FullColoumnName::TYPE:
		type = data.toInt(&isOk, 10);
		if (!isOk || (type != TYPE_DIGITAL && type != TYPE_PWM && type != TYPE_VNH && type != TYPE_ANALOG && type != TYPE_HALL))
			return Errors::Configurator::FILE_DATA_TYPE_INCORRECT;
		break;

	case FullColoumnName::CAN_ID:
		if (data == "" || data == "-1" || data == "-")
			break;
		else if (data[0] == hexPrefix[0] && data[1] == hexPrefix[1])
		{
			QString id = "";
			for (int i = 2; i < data.size(); i++)
				id += data[i];

			data.toInt(&isOk, 16);
			if (!isOk)
				return Errors::Configurator::FILE_DATA_CAN_ID_INCORRECT_HEX;
		}
		else
		{
			data.toInt(&isOk, 16);
			if (!isOk)
			{
				data.toInt(&isOk, 10);
				if (!isOk)
					return Errors::Configurator::FILE_DATA_CAN_ID_INCORRECT_DEC;
			}
		}
		break;

	case FullColoumnName::BYTE:
		if (data == "")
			break;
		data.toInt(&isOk, 10);
		if (!isOk)
			return Errors::Configurator::FILE_DATA_BYTE_INCORRECT;
		break;

	case FullColoumnName::MIN_CURRENT:
		data.toFloat(&isOk);
		if (!isOk)
			return Errors::Configurator::FILE_DATA_MIN_CURRENT_INCORRECT;
		break;

	case FullColoumnName::MAX_CURRENT:
		data.toFloat(&isOk);
		if (!isOk)
			return Errors::Configurator::FILE_DATA_MAX_CURRENT_INCORRECT;
		break;

	case FullColoumnName::MIN_VOLTAGE:
		data.toFloat(&isOk);
		if (!isOk)
			return Errors::Configurator::FILE_DATA_MIN_VOLTAGE_INCORRECT;
		break;

	case FullColoumnName::MAX_VOLTAGE:
		data.toFloat(&isOk);
		if (!isOk)
			return Errors::Configurator::FILE_DATA_MAX_VOLTAGE_INCORRECT;
		break;

	case FullColoumnName::NAME:
		if (!data.size())
			return Errors::Configurator::FILE_DATA_NAME_EMPTY;
		break;
	
	case FullColoumnName::COMPONENT:
		if (!data.size())
			return Errors::Configurator::FILE_DATA_COMPONENT_EMPTY;
		break;
	}

	return Errors::Configurator::CORRECT;
}

void ConfiguratorWindow::on_loadButton_clicked()
{
	selectedFileFullName = QFileDialog::getOpenFileName(this, "Open File", "", "CSV (*.csv)");
	int localFileNameInd = selectedFileFullName.lastIndexOf("/");

	QString partialPrintedFileName = "";
	fileName = "";

	bool isTypeStart = false;
	for (int i = localFileNameInd + 1; i < selectedFileFullName.size(); i++)
	{
		if (selectedFileFullName[i] == '.')
			isTypeStart = true;

		if (!isTypeStart)
			partialPrintedFileName += selectedFileFullName[i];

		fileName += selectedFileFullName[i];
	}

	fileNameLineEdit->setText(partialPrintedFileName);

	proccessSelectedFile(selectedFileFullName);
}

void ConfiguratorWindow::proccessSelectedFile(QString fileName)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		generateError(EMPTY_FILLING, Errors::Configurator::FILE_OPEN);
		mainTableWidget->clear();
		cleanRowProperties();
		tableRowPropertiesVector.clear();
		mainTableWidget->setRowCount(0);
		fileNameLineEdit->setText("");
		return;
	}

	while (mainTableWidget->rowCount())
		deleteRow(0);

	bool isFileCorrect = true;
	int row = 1;
	while (!file.atEnd())
	{
		QString dataLine = file.readLine();
		dataLine.remove("\n");
		QStringList dataList = dataLine.split(";");

		if (dataList.size() == FILE_CFG_COUNT)
		{
			if (dataList[IND_CFG] != MASK_CFG)
			{
				isFileCorrect = false;
				generateError(EMPTY_FILLING, Errors::Configurator::FILE_MASK_CFG);
			}
			if (dataList[IND_STAND_TYPE] == MASK_STAND_MIXED)
				selectStandTypeComboBox->setCurrentIndex(STAND_NOT_SET);
			else if (dataList[IND_STAND_TYPE] == MASK_STAND_MANUAL)
				selectStandTypeComboBox->setCurrentIndex(STAND_MANUAL);
			else if (dataList[IND_STAND_TYPE] == MASK_STAND_AUTO)
				selectStandTypeComboBox->setCurrentIndex(STAND_AUTO);
			else
			{
				isFileCorrect = false;
				generateError(EMPTY_FILLING, Errors::Configurator::FILE_MASK_STAND_TYPE);
				break;
			}
		}
		else if (dataList.size() == FILE_DATA_COUNT)
		{
			QAbstractItemModel* model = mainTableWidget->model();

			int currentRowNum = mainTableWidget->rowCount();
			
			createNewRowProperties();
			createNewRow();

			for (int currentColoumnNum = 0; currentColoumnNum < FILE_DATA_COUNT && isFileCorrect; currentColoumnNum++)
			{
				QString currentData = dataList[currentColoumnNum];

				switch (currentColoumnNum)
				{
				case (int)FullColoumnName::CONNECTOR:
					if (generateError(row, verifyFileData((FullColoumnName)currentColoumnNum, currentData)))
						tableRowPropertiesVector[currentRowNum]->presetSettings->connector = (ConnectorId)(currentData.toInt());
					else
						isFileCorrect = false;
					break;

				case (int)FullColoumnName::PIN:
					if (generateError(row, verifyFileData((FullColoumnName)currentColoumnNum, currentData)))
						tableRowPropertiesVector[currentRowNum]->pin = currentData.toInt();
					else
						isFileCorrect = false;
					break;

				case (int)FullColoumnName::DIRECTION:
					if (generateError(row, verifyFileData((FullColoumnName)currentColoumnNum, currentData)))
						tableRowPropertiesVector[currentRowNum]->presetSettings->direction = currentData.toInt();
					else
						isFileCorrect = false;
					break;

				case (int)FullColoumnName::TYPE:
					if (generateError(row, verifyFileData((FullColoumnName)currentColoumnNum, currentData)))
						tableRowPropertiesVector[currentRowNum]->presetSettings->type = currentData.toInt();
					else
						isFileCorrect = false;
					break;

				case (int)FullColoumnName::CAN_ID:
					if (generateError(row, verifyFileData((FullColoumnName)currentColoumnNum, currentData)))
						tableRowPropertiesVector[currentRowNum]->canId = currentData;
					else
						isFileCorrect = false;
					break;

				case (int)FullColoumnName::BYTE:
					if (generateError(row, verifyFileData((FullColoumnName)currentColoumnNum, currentData)))
						tableRowPropertiesVector[currentRowNum]->byte = (currentData != "" ? currentData.toInt() : -1);
					else
						isFileCorrect = false;
					break;

				case (int)FullColoumnName::MIN_CURRENT:
					if (generateError(row, verifyFileData((FullColoumnName)currentColoumnNum, currentData)))
						tableRowPropertiesVector[currentRowNum]->minCurrent = currentData.toFloat();
					else
						isFileCorrect = false;
					break;

				case (int)FullColoumnName::MAX_CURRENT:
					if (generateError(row, verifyFileData((FullColoumnName)currentColoumnNum, currentData)))
						tableRowPropertiesVector[currentRowNum]->maxCurrent = currentData.toFloat();
					else
						isFileCorrect = false;
					break;

				case (int)FullColoumnName::MIN_VOLTAGE:
					if (generateError(row, verifyFileData((FullColoumnName)currentColoumnNum, currentData)))
						tableRowPropertiesVector[currentRowNum]->minVoltage = currentData.toFloat();
					else
						isFileCorrect = false;
					break;

				case (int)FullColoumnName::MAX_VOLTAGE:
					if (generateError(row, verifyFileData((FullColoumnName)currentColoumnNum, currentData)))
						tableRowPropertiesVector[currentRowNum]->maxVoltage = currentData.toFloat();
					else
						isFileCorrect = false;
					break;

				case (int)FullColoumnName::NAME:
					if (generateError(row, verifyFileData((FullColoumnName)currentColoumnNum, currentData)))
						tableRowPropertiesVector[currentRowNum]->name = currentData;
					else
						isFileCorrect = false;
					break;

				case (int)FullColoumnName::COMPONENT:
					if (generateError(row, verifyFileData((FullColoumnName)currentColoumnNum, currentData)))
						tableRowPropertiesVector[currentRowNum]->component = currentData;
					else
						isFileCorrect = false;
					break;
				}
			}
		}
		else
		{ 
			generateError(EMPTY_FILLING, Errors::Configurator::FILE_DATA_AMOUNT);
			break;
		}

		row++;
	}
	if (!isFileCorrect)
	{
		mainTableWidget->clear();
		cleanRowProperties();
		tableRowPropertiesVector.clear();
		mainTableWidget->setRowCount(0);
		fileNameLineEdit->setText("");
	}

	file.close();
	resetedFill(selectStandTypeComboBox->currentIndex());
}

void ConfiguratorWindow::resetedFill(int standType)
{
	for (int currentRowNum = 0; currentRowNum < tableRowPropertiesVector.size(); currentRowNum++)
	{
		TableRowProperties* currentRowProperties = tableRowPropertiesVector[currentRowNum];

		QAbstractItemModel* model = mainTableWidget->model();
		currentRowProperties->connectorComboBox->setCurrentIndex((int)(currentRowProperties->presetSettings->connector));
		if (currentRowProperties->pin != -1)
			model->setData(model->index(currentRowNum, (int)FullColoumnName::PIN), QString::number(currentRowProperties->pin));
		if (currentRowProperties->presetSettings->direction != -1)
			currentRowProperties->on_direction_activated(currentRowProperties->presetSettings->direction + 1);

		switch (currentRowProperties->presetSettings->type)
		{
		case TYPE_DIGITAL:
		case TYPE_PWM:
		case TYPE_VNH:
			currentRowProperties->on_type_activated(currentRowProperties->presetSettings->type);
			break;

		case TYPE_ANALOG:
		case TYPE_HALL:
			currentRowProperties->on_type_activated(currentRowProperties->presetSettings->type - 2);
			break;
		}
		switch (standType)
		{
		case STAND_NOT_SET:
			if (currentRowProperties->canId != -1)
				model->setData(model->index(currentRowNum, (int)FullColoumnName::CAN_ID), currentRowProperties->canId);
			if (currentRowProperties->byte != -1)
				model->setData(model->index(currentRowNum, (int)FullColoumnName::BYTE), QString::number(currentRowProperties->byte));
			if (currentRowProperties->minCurrent != -1)
				model->setData(model->index(currentRowNum, (int)FullColoumnName::MIN_CURRENT), QString::number(currentRowProperties->minCurrent));
			if (currentRowProperties->maxCurrent != -1)
				model->setData(model->index(currentRowNum, (int)FullColoumnName::MAX_CURRENT), QString::number(currentRowProperties->maxCurrent));
			if (currentRowProperties->minVoltage != -1)
				model->setData(model->index(currentRowNum, (int)FullColoumnName::MIN_VOLTAGE), QString::number(currentRowProperties->minVoltage));
			if (currentRowProperties->maxVoltage != -1)
				model->setData(model->index(currentRowNum, (int)FullColoumnName::MAX_VOLTAGE), QString::number(currentRowProperties->maxVoltage));
			model->setData(model->index(currentRowNum, (int)FullColoumnName::NAME), currentRowProperties->name);
			model->setData(model->index(currentRowNum, (int)FullColoumnName::COMPONENT), currentRowProperties->component);
			break;

		case STAND_MANUAL:
			if (currentRowProperties->canId != -1)
				model->setData(model->index(currentRowNum, (int)ManualColoumnName::CAN_ID), currentRowProperties->canId);
			if (currentRowProperties->byte != -1)
				model->setData(model->index(currentRowNum, (int)ManualColoumnName::BYTE), QString::number(currentRowProperties->byte));
			model->setData(model->index(currentRowNum, (int)ManualColoumnName::NAME), currentRowProperties->name);
			model->setData(model->index(currentRowNum, (int)ManualColoumnName::COMPONENT), currentRowProperties->component);
			break;

		case STAND_AUTO:
			if (currentRowProperties->minCurrent != -1)
				model->setData(model->index(currentRowNum, (int)AutoColoumnName::MIN_CURRENT), QString::number(currentRowProperties->minCurrent));
			if (currentRowProperties->maxCurrent != -1)
				model->setData(model->index(currentRowNum, (int)AutoColoumnName::MAX_CURRENT), QString::number(currentRowProperties->maxCurrent));
			if (currentRowProperties->minVoltage != -1)
				model->setData(model->index(currentRowNum, (int)AutoColoumnName::MIN_VOLTAGE), QString::number(currentRowProperties->minVoltage));
			if (currentRowProperties->maxVoltage != -1)
				model->setData(model->index(currentRowNum, (int)AutoColoumnName::MAX_VOLTAGE), QString::number(currentRowProperties->maxVoltage));
			model->setData(model->index(currentRowNum, (int)AutoColoumnName::NAME), currentRowProperties->name);
			break;
		}
	}
}

void ConfiguratorWindow::cleanRowProperties()
{
	for (int i = 0; i < tableRowPropertiesVector.size(); i++)
	{
		tableRowPropertiesVector[i]->connectorCellLayout->removeWidget(tableRowPropertiesVector[i]->connectorComboBox);
		tableRowPropertiesVector[i]->directionCellLayout->removeWidget(tableRowPropertiesVector[i]->directionComboBox);
		tableRowPropertiesVector[i]->typeCellLayout->removeWidget(tableRowPropertiesVector[i]->typeComboBox);
		tableRowPropertiesVector[i]->deleteCellLayout->removeWidget(tableRowPropertiesVector[i]->deleteButton);

		delete tableRowPropertiesVector[i]->connectorComboBox;
		delete tableRowPropertiesVector[i]->directionComboBox;
		delete tableRowPropertiesVector[i]->typeComboBox;
		delete tableRowPropertiesVector[i]->deleteButton;
		delete tableRowPropertiesVector[i]->connectorCellLayout;
		delete tableRowPropertiesVector[i]->directionCellLayout;
		delete tableRowPropertiesVector[i]->typeCellLayout;
		delete tableRowPropertiesVector[i]->deleteCellLayout;
		delete tableRowPropertiesVector[i]->connectorCellWidget;
		delete tableRowPropertiesVector[i]->directionCellWidget;
		delete tableRowPropertiesVector[i]->typeCellWidget;
		delete tableRowPropertiesVector[i]->deleteCellWidget;
	}
}

void ConfiguratorWindow::on_selectStandTypeComboBox_currentIndexChanged(int index)
{
	resetPresets();
	updateTableData(UNVERIFIABLE);

	switch (index)
	{
	case STAND_NOT_SET:
		resetFullTable();
		break;
		
	case STAND_MANUAL:
		resetManualTable();
		break;
		
	case STAND_AUTO:
		resetAutoTable();
		break;
	}
	for (int i = 0; i < tableRowPropertiesVector.size(); i++)
		createNewRow();
	resetedFill(index);

	standTypeSelected = selectStandTypeComboBox->currentIndex();
}

bool ConfiguratorWindow::generateError(int row, Errors::Configurator error)
{
	row++;
	switch (viewWindowState->appLanguage)
	{
	case ENGLISH_LANG:
		switch (error)
		{
		case Errors::Configurator::CORRECT:
			return true;

		case Errors::Configurator::SAVE_CONNECTOR_NOT_SET:
			QMessageBox::critical(this, "Error saving file", "Connector on row " + QString::number(row) + " not selected", "Ok");
			return false;

		case Errors::Configurator::SAVE_PIN_NULL:
			QMessageBox::critical(this, "Error saving file", "Pin on row " + QString::number(row) + " is not filled", "Ok");
			return false;

		case Errors::Configurator::SAVE_PIN_INCORRECT:
			QMessageBox::critical(this, "Error saving file", "Pin on row " + QString::number(row) + " is incorrect\n Write decimal number", "Ok");
			return false;

		case Errors::Configurator::SAVE_DIRECTION_NOT_SET:
			QMessageBox::critical(this, "Error saving file", "Direction on row " + QString::number(row) + " not selected", "Ok");
			return false;

		case Errors::Configurator::SAVE_TYPE_NOT_SET:
			QMessageBox::critical(this, "Error saving file", "Type on row " + QString::number(row) + " not selected", "Ok");
			return false;

		case Errors::Configurator::SAVE_CAN_ID_INCORRECT_HEX:
		case Errors::Configurator::SAVE_CAN_ID_INCORRECT_DEC:
			QMessageBox::critical(this, "Error saving file", "Can_id on row " + QString::number(row) + " is incorrect\n Write heximal or decimal number", "Ok");
			return false;

		case Errors::Configurator::SAVE_BYTE_INCORRECT:
			QMessageBox::critical(this, "Error saving file", "Byte on row " + QString::number(row) + " is incorrect\n Write decimal number from 0 to 7", "Ok");
			return false;

		case Errors::Configurator::SAVE_MIN_CURRENT_NULL:
			QMessageBox::critical(this, "Error saving file", "Min current on row " + QString::number(row) + " is not filled", "Ok");
			return false;

		case Errors::Configurator::SAVE_MIN_CURRENT_INCORRECT:
			QMessageBox::critical(this, "Error saving file", "Min current on row " + QString::number(row) + " is incorrect\n Write decimal float number", "Ok");
			return false;

		case Errors::Configurator::SAVE_MAX_CURRENT_NULL:
			QMessageBox::critical(this, "Error saving file", "Max current on row " + QString::number(row) + " is not filled", "Ok");
			return false;

		case Errors::Configurator::SAVE_MAX_CURRENT_INCORRECT:
			QMessageBox::critical(this, "Error saving file", "Max current on row " + QString::number(row) + " is incorrect\n Write decimal float number", "Ok");
			return false;

		case Errors::Configurator::SAVE_MIN_VOLTAGE_NULL:
			QMessageBox::critical(this, "Error saving file", "Min voltage on row " + QString::number(row) + " is not filled", "Ok");
			return false;

		case Errors::Configurator::SAVE_MIN_VOLTAGE_INCORRECT:
			QMessageBox::critical(this, "Error saving file", "Min voltage on row " + QString::number(row) + " is incorrect\n Write decimal float number", "Ok");
			return false;

		case Errors::Configurator::SAVE_MAX_VOLTAGE_NULL:
			QMessageBox::critical(this, "Error saving file", "Max voltage on row " + QString::number(row) + " is not filled", "Ok");
			return false;

		case Errors::Configurator::SAVE_MAX_VOLTAGE_INCORRECT:
			QMessageBox::critical(this, "Error saving file", "Max voltage on row " + QString::number(row) + " is incorrect\n Write decimal float number", "Ok");
			return false;

		case Errors::Configurator::SAVE_NAME_NULL:
			QMessageBox::critical(this, "Error saving file", "Name on row " + QString::number(row) + " is not filled", "Ok");
			return false;
			
		case Errors::Configurator::SAVE_COMPONENT_NULL:
			QMessageBox::critical(this, "Error saving file", "Component on row " + QString::number(row) + " is not filled", "Ok");
			return false;

		case Errors::Configurator::FILE_OPEN:
			QMessageBox::critical(this, "Error loading file", "Unable to open file", "Ok");
			return false;

		case Errors::Configurator::FILE_MASK_CFG:
			QMessageBox::critical(this, "Error loading file", "Keyword CFG not found", "Ok");
			return false;

		case Errors::Configurator::FILE_MASK_STAND_TYPE:
			QMessageBox::critical(this, "Error loading file", "Stand type incorrect", "Ok");
			return false;

		case Errors::Configurator::FILE_DATA_AMOUNT:
			QMessageBox::critical(this, "Error loading file", "The amount of data in the row is incorrect", "Ok");
			return false;

		case Errors::Configurator::FILE_DATA_CONNECTOR_INCORRECT:
			QMessageBox::critical(this, "Error loading file", "Connector data on row " + QString::number(row) + " is incorrect", "Ok");
			return false;

		case Errors::Configurator::FILE_DATA_PIN_INCORRECT:
			QMessageBox::critical(this, "Error loading file", "Pin data on row " + QString::number(row) + " is incorrect", "Ok");
			return false;

		case Errors::Configurator::FILE_DATA_DIRECTION_INCORRECT:
			QMessageBox::critical(this, "Error loading file", "Direction data on row " + QString::number(row) + " is incorrect", "Ok");
			return false;

		case Errors::Configurator::FILE_DATA_TYPE_INCORRECT:
			QMessageBox::critical(this, "Error loading file", "Type data on row " + QString::number(row) + " is incorrect", "Ok");
			return false;

		case Errors::Configurator::FILE_DATA_CAN_ID_INCORRECT_HEX:
		case Errors::Configurator::FILE_DATA_CAN_ID_INCORRECT_DEC:
			QMessageBox::critical(this, "Error loading file", "Can_id data on row " + QString::number(row) + " is incorrect", "Ok");
			return false;

		case Errors::Configurator::FILE_DATA_BYTE_INCORRECT:
			QMessageBox::critical(this, "Error loading file", "Byte data on row " + QString::number(row) + " is incorrect", "Ok");
			return false;

		case Errors::Configurator::FILE_DATA_MIN_CURRENT_INCORRECT:
			QMessageBox::critical(this, "Error loading file", "Min current data on row " + QString::number(row) + " is incorrect", "Ok");
			return false;

		case Errors::Configurator::FILE_DATA_MAX_CURRENT_INCORRECT:
			QMessageBox::critical(this, "Error loading file", "Max current data on row " + QString::number(row) + " is incorrect", "Ok");
			return false;

		case Errors::Configurator::FILE_DATA_MIN_VOLTAGE_INCORRECT:
			QMessageBox::critical(this, "Error loading file", "Min voltage data on row " + QString::number(row) + " is incorrect", "Ok");
			return false;

		case Errors::Configurator::FILE_DATA_MAX_VOLTAGE_INCORRECT:
			QMessageBox::critical(this, "Error loading file", "Max voltage data on row" + QString::number(row) + " is incorrect", "Ok");
			return false;

		case Errors::Configurator::FILE_DATA_NAME_EMPTY:
			QMessageBox::critical(this, "Error loading file", "Name data on row" + QString::number(row) + " is empty", "Ok");
			return false;

		case Errors::Configurator::FILE_DATA_COMPONENT_EMPTY:
			QMessageBox::critical(this, "Error loading file", "Component data on row" + QString::number(row) + " is empty", "Ok");
			return false;
		}
		break;

	case RUSSIAN_LANG:
		switch (error)
		{
		case Errors::Configurator::CORRECT:
			return true;

		case Errors::Configurator::SAVE_CONNECTOR_NOT_SET:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при сохранении"), QString::fromLocal8Bit("Коннектор в строке ") + QString::number(row) + QString::fromLocal8Bit(" не выбран"), QString::fromLocal8Bit("Ок"));
			return false; 

		case Errors::Configurator::SAVE_PIN_NULL:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при сохранении"), QString::fromLocal8Bit("Пин в строке ") + QString::number(row) + QString::fromLocal8Bit(" не заполнен"), QString::fromLocal8Bit("Ок"));
			return false;

		case Errors::Configurator::SAVE_PIN_INCORRECT:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при сохранении"), QString::fromLocal8Bit("Пин в строке ") + QString::number(row) + QString::fromLocal8Bit(" не корректен\n Введите десятичное число"), QString::fromLocal8Bit("Ок"));
			return false;

		case Errors::Configurator::SAVE_DIRECTION_NOT_SET:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при сохранении"), QString::fromLocal8Bit("Направление в строке ") + QString::number(row) + QString::fromLocal8Bit(" не выбрано"), QString::fromLocal8Bit("Ок"));
			return false;

		case Errors::Configurator::SAVE_TYPE_NOT_SET:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при сохранении"), QString::fromLocal8Bit("Тип в строке ") + QString::number(row) + QString::fromLocal8Bit(" не выбран"), QString::fromLocal8Bit("Ок"));
			return false;

		case Errors::Configurator::SAVE_CAN_ID_INCORRECT_HEX:
		case Errors::Configurator::SAVE_CAN_ID_INCORRECT_DEC:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при сохранении"), QString::fromLocal8Bit("Can_id в строке ") + QString::number(row) + QString::fromLocal8Bit(" не корректен\n Введите шестнадцатеричное или десятичное число"), QString::fromLocal8Bit("Ок"));
			return false;	

		case Errors::Configurator::SAVE_BYTE_INCORRECT:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при сохранении"), QString::fromLocal8Bit("Байт в строке ") + QString::number(row) + QString::fromLocal8Bit(" не корректен\n Введите десятичную цифру от 0 до 7"), QString::fromLocal8Bit("Ок"));
			return false;

		case Errors::Configurator::SAVE_MIN_CURRENT_NULL:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при сохранении"), QString::fromLocal8Bit("Минимальная сила тока в строке ") + QString::number(row) + QString::fromLocal8Bit(" не заполнена"), QString::fromLocal8Bit("Ок"));
			return false;

		case Errors::Configurator::SAVE_MIN_CURRENT_INCORRECT:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при сохранении"), QString::fromLocal8Bit("Минимальная сила тока в строке ") + QString::number(row) + QString::fromLocal8Bit(" не корректна\nВведите десятичное вещественное число"), QString::fromLocal8Bit("Ок"));
			return false;

		case Errors::Configurator::SAVE_MAX_CURRENT_NULL:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при сохранении"), QString::fromLocal8Bit("Максимальная сила тока в строке ") + QString::number(row) + QString::fromLocal8Bit(" не заполнена"), QString::fromLocal8Bit("Ок"));
			return false;

		case Errors::Configurator::SAVE_MAX_CURRENT_INCORRECT:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при сохранении"), QString::fromLocal8Bit("Максимальная сила тока в строке ") + QString::number(row) + QString::fromLocal8Bit(" не корректна\nВведите десятичное вещественное число"), QString::fromLocal8Bit("Ок"));
			return false;

		case Errors::Configurator::SAVE_MIN_VOLTAGE_NULL:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при сохранении"), QString::fromLocal8Bit("Минимальное напряжение в строке ") + QString::number(row) + QString::fromLocal8Bit(" не заполнено"), QString::fromLocal8Bit("Ок"));
			return false;

		case Errors::Configurator::SAVE_MIN_VOLTAGE_INCORRECT:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при сохранении"), QString::fromLocal8Bit("Минимальное напряжение в строке ") + QString::number(row) + QString::fromLocal8Bit(" не корректно\nВведите десятичное вещественное число"), QString::fromLocal8Bit("Ок"));
			return false;

		case Errors::Configurator::SAVE_MAX_VOLTAGE_NULL:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при сохранении"), QString::fromLocal8Bit("Максимальное напряжение в строке ") + QString::number(row) + QString::fromLocal8Bit("  не заполнено"), QString::fromLocal8Bit("Ок"));
			return false;

		case Errors::Configurator::SAVE_MAX_VOLTAGE_INCORRECT:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при сохранении"), QString::fromLocal8Bit("Максимальное напряжение в строке ") + QString::number(row) + QString::fromLocal8Bit(" не корректно\nВведите десятичное вещественное число"), QString::fromLocal8Bit("Ок"));
			return false;

		case Errors::Configurator::SAVE_NAME_NULL:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при сохранении"), QString::fromLocal8Bit("Название в строке ") + QString::number(row) + QString::fromLocal8Bit(" не заполнено"), QString::fromLocal8Bit("Ок"));
			return false;
			
		case Errors::Configurator::SAVE_COMPONENT_NULL:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при сохранении"), QString::fromLocal8Bit("Компонент в строке ") + QString::number(row) + QString::fromLocal8Bit(" не заполнено"), QString::fromLocal8Bit("Ок"));
			return false;

		case Errors::Configurator::FILE_OPEN:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при загрузке"), QString::fromLocal8Bit("Невозможно открыть файл"), QString::fromLocal8Bit("Ок"));
			return false;

		case Errors::Configurator::FILE_MASK_CFG:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при загрузке"), QString::fromLocal8Bit("Отсутствует ключевое слово CFG"), QString::fromLocal8Bit("Ок"));
			return false;

		case Errors::Configurator::FILE_MASK_STAND_TYPE:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при загрузке"), QString::fromLocal8Bit("Некорректная маска типа стенда"), QString::fromLocal8Bit("Ок"));
			return false;

		case Errors::Configurator::FILE_DATA_AMOUNT:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при загрузке"), QString::fromLocal8Bit("Количество даных в строке файла неверно"), QString::fromLocal8Bit("Ок"));
			return false;

		case Errors::Configurator::FILE_DATA_CONNECTOR_INCORRECT:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при загрузке"), QString::fromLocal8Bit("Коннектор в строке ") + QString::number(row) + QString::fromLocal8Bit(" не корректен"), QString::fromLocal8Bit("Ок"));
			return false;

		case Errors::Configurator::FILE_DATA_PIN_INCORRECT:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при загрузке"), QString::fromLocal8Bit("Пин в строке ") + QString::number(row) + QString::fromLocal8Bit(" не корректен"), QString::fromLocal8Bit("Ок"));
			return false;

		case Errors::Configurator::FILE_DATA_DIRECTION_INCORRECT:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при загрузке"), QString::fromLocal8Bit("Направление в строке ") + QString::number(row) + QString::fromLocal8Bit(" не корректено"), QString::fromLocal8Bit("Ок"));
			return false;

		case Errors::Configurator::FILE_DATA_TYPE_INCORRECT:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при загрузке"), QString::fromLocal8Bit("Тип в строке ") + QString::number(row) + QString::fromLocal8Bit(" не корректен"), QString::fromLocal8Bit("Ок"));
			return false;

		case Errors::Configurator::FILE_DATA_CAN_ID_INCORRECT_HEX:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при загрузке"), QString::fromLocal8Bit("Can_id в строке ") + QString::number(row) + QString::fromLocal8Bit(" не корректен"), QString::fromLocal8Bit("Ок"));
			return false;

		case Errors::Configurator::FILE_DATA_CAN_ID_INCORRECT_DEC:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при загрузке"), QString::fromLocal8Bit("Can_id в строке ") + QString::number(row) + QString::fromLocal8Bit(" не корректен"), QString::fromLocal8Bit("Ок"));
			return false;

		case Errors::Configurator::FILE_DATA_BYTE_INCORRECT:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при загрузке"), QString::fromLocal8Bit("Байт в строке ") + QString::number(row) + QString::fromLocal8Bit(" не корректен"), QString::fromLocal8Bit("Ок"));
			return false;

		case Errors::Configurator::FILE_DATA_MIN_CURRENT_INCORRECT:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при загрузке"), QString::fromLocal8Bit("Минимальная сила тока в строке ") + QString::number(row) + QString::fromLocal8Bit(" не корректена"), QString::fromLocal8Bit("Ок"));
			return false;

		case Errors::Configurator::FILE_DATA_MAX_CURRENT_INCORRECT:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при загрузке"), QString::fromLocal8Bit("Максимальная сила тока в строке ") + QString::number(row) + QString::fromLocal8Bit(" не корректена"), QString::fromLocal8Bit("Ок"));
			return false;

		case Errors::Configurator::FILE_DATA_MIN_VOLTAGE_INCORRECT:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при загрузке"), QString::fromLocal8Bit("Минимальное напряжение в строке ") + QString::number(row) + QString::fromLocal8Bit(" не корректено"), QString::fromLocal8Bit("Ок"));
			return false;

		case Errors::Configurator::FILE_DATA_MAX_VOLTAGE_INCORRECT:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при загрузке"), QString::fromLocal8Bit("Максимальное напряжение в строке ") + QString::number(row) + QString::fromLocal8Bit(" не корректено"), QString::fromLocal8Bit("Ок"));
			return false;

		case Errors::Configurator::FILE_DATA_NAME_EMPTY:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при загрузке"), QString::fromLocal8Bit("Название в строке ") + QString::number(row) + QString::fromLocal8Bit(" не заполнено"), QString::fromLocal8Bit("Ок"));
			return false;

		case Errors::Configurator::FILE_DATA_COMPONENT_EMPTY:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при загрузке"), QString::fromLocal8Bit("Компонент в строке ") + QString::number(row) + QString::fromLocal8Bit(" не заполнено"), QString::fromLocal8Bit("Ок"));
			return false;
		}
		break;
	}
}