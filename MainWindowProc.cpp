#include "MainWindow.h"

#define IND_CFG             0
#define IND_STAND_TYPE      1

#define COLUMN_COUNT       12
#define IND_CONNECTOR_ID    0
#define IND_PIN             1
#define IND_DIRECTION       2
#define IND_TYPE            3
#define IND_CAN_ID          4
#define IND_BIT             5
#define IND_MIN_CURRENT     6
#define IND_MAX_CURRENT     7
#define IND_MIN_VOLTAGE     8
#define IND_MAX_VOLTAGE     9
#define IND_NAME            10
#define IND_COMPONENT       11

#define MASK_COUNT			2
#define MASK_CFG            "CFG"
#define MASK_STAND_MANUAL   "MANUAL"
#define MASK_STAND_AUTO     "AUTO"

static bool verifyData(int column, QString data)
{
	switch (column)
	{
	case IND_CONNECTOR_ID:
		if (false)
			return false;
		break;

	case IND_PIN:
		if (false)
			return false;
		break;

	case IND_DIRECTION:
		if (false)
			return false;
		break;

	case IND_TYPE:
		if (false)
			return false;
		break;

	case IND_CAN_ID:
		if (false)
			return false;
		break;

	case IND_BIT:
		if (false)
			return false;
		break;

	case IND_MIN_CURRENT:
		if (false)
			return false;
		break;

	case IND_MAX_CURRENT:
		if (false)
			return false;
		break;

	case IND_MIN_VOLTAGE:
		if (false)
			return false;
		break;

	case IND_MAX_VOLTAGE:
		if (false)
			return false;
		break;

	case IND_NAME:
		if (false)
			return false;
		break;
	}

	return true;
}

void MainWindow::proccessSelectedFile(QString fileName)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		// ERROR
		return;
	}

	cables.clear();
	bool isFileCorrect = true;
	int id = 0;
	while (!file.atEnd())
	{
		QString dataLine = file.readLine();
		dataLine.remove("\n");
		QStringList dataList = dataLine.split(";");

		if (dataList.size() == MASK_COUNT)
		{
			if (dataList[IND_CFG] != MASK_CFG)
			{
				isFileCorrect = false;
				generateError(Errors::MainWindow::FILE_MASK_CFG);
				return;
			}
			if (dataList[IND_STAND_TYPE] == MASK_STAND_MANUAL)
			{
				selectedFileStandType = CFG_STAND_MANUAL;
				manualStandButton->click();
			}
			else if (dataList[IND_STAND_TYPE] == MASK_STAND_AUTO)
			{
				selectedFileStandType = CFG_STAND_AUTO;
				autoStandButton->click();
			}
			else
			{
				selectedFileStandType = CFG_STAND_NOT_SET;
				isFileCorrect = false;
				generateError(Errors::MainWindow::FILE_MASK_STAND_TYPE);
				return;
			}
		}
		else if (dataList.size() == COLUMN_COUNT)
		{
			Cable cable;
			cable.id = id++;
			for (int column = 0; column < COLUMN_COUNT && isFileCorrect; column++)
			{
				QString currentProperty = dataList[column];

				switch (column)
				{
				case IND_CONNECTOR_ID:
					if (verifyData(column, currentProperty))
						cable.connector = (ConnectorId)currentProperty.toInt();
					else
					{
						isFileCorrect = false;
						generateError(Errors::MainWindow::FILE_DATA_CONNECTOR_INCORRECT);
					}
					break;

				case IND_PIN:
					if (verifyData(column, currentProperty))
						cable.pin = currentProperty.toInt();
					else
					{
						isFileCorrect = false;
						generateError(Errors::MainWindow::FILE_DATA_PIN_INCORRECT);
					}
					break;

				case IND_DIRECTION:
					if (verifyData(column, currentProperty))
						cable.direction = currentProperty.toInt();
					else
					{
						isFileCorrect = false;
						generateError(Errors::MainWindow::FILE_DATA_DIRECTION_INCORRECT);
					}
					break;

				case IND_TYPE:
					if (verifyData(column, currentProperty))
						cable.type = currentProperty.toInt();
					else
					{
						isFileCorrect = false;
						generateError(Errors::MainWindow::FILE_DATA_TYPE_INCORRECT);
					}
					break;

				case IND_CAN_ID:
					if (verifyData(column, currentProperty))
						cable.canId = currentProperty.toInt(nullptr, 16);
					else
					{
						isFileCorrect = false;
						generateError(Errors::MainWindow::FILE_DATA_CAN_ID_INCORRECT_HEX);
					}
					break;

				case IND_BIT:
					if (verifyData(column, currentProperty))
						cable.bit = currentProperty.toInt();
					else
					{
						isFileCorrect = false;
						generateError(Errors::MainWindow::FILE_DATA_BIT_INCORRECT);
					}
					break;

				case IND_MIN_CURRENT:
					if (verifyData(column, currentProperty))
						cable.minCurrent = currentProperty.toDouble();
					else
					{
						isFileCorrect = false;
						generateError(Errors::MainWindow::FILE_DATA_MIN_CURRENT_INCORRECT);
					}
					break;

				case IND_MAX_CURRENT:
					if (verifyData(column, currentProperty))
						cable.maxCurrent = currentProperty.toDouble();
					else
					{
						isFileCorrect = false;
						generateError(Errors::MainWindow::FILE_DATA_MAX_CURRENT_INCORRECT);
					}
					break;

				case IND_MIN_VOLTAGE:
					if (verifyData(column, currentProperty))
						cable.minVoltage = currentProperty.toDouble();
					else
					{
						isFileCorrect = false;
						generateError(Errors::MainWindow::FILE_DATA_MIN_VOLTAGE_INCORRECT);
					}
					break;

				case IND_MAX_VOLTAGE:
					if (verifyData(column, currentProperty))
						cable.maxVoltage = currentProperty.toDouble();
					else
					{
						isFileCorrect = false;
						generateError(Errors::MainWindow::FILE_DATA_MAX_VOLTAGE_INCORRECT);
					}
					break;

				case IND_NAME:
					if (verifyData(column, currentProperty))
						cable.name = currentProperty;
					else
					{
						isFileCorrect = false;
						generateError(Errors::MainWindow::FILE_DATA_NAME_EMPTY);
					}
					break;

				case IND_COMPONENT:
					if (verifyData(column, currentProperty))
						cable.component = currentProperty;
					else
					{
						isFileCorrect = false;
						generateError(Errors::MainWindow::FILE_DATA_COMPONENT_EMPTY);
					}
					break;
				}
			}
			cables.push_back(cable);
		}
		else
		{
			isFileCorrect = false;
			generateError(Errors::MainWindow::FILE_DATA_AMOUNT);
		}
	}
	if (!isFileCorrect)
		cables.clear();

	file.close();
}

void MainWindow::generateError(Errors::MainWindow error)
{
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		switch (error)
		{
		case Errors::MainWindow::FILE_OPEN:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при загрузке"), QString::fromLocal8Bit("Невозможно открыть файл"), QString::fromLocal8Bit("Ок"));
			break;

		case Errors::MainWindow::FILE_MASK_CFG:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при загрузке"), QString::fromLocal8Bit("Отсутствует ключевое слово CFG"), QString::fromLocal8Bit("Ок"));
			break;

		case Errors::MainWindow::FILE_MASK_STAND_TYPE:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при загрузке"), QString::fromLocal8Bit("Некорректная маска типа стенда"), QString::fromLocal8Bit("Ок"));
			break;

		case Errors::MainWindow::FILE_DATA_AMOUNT:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при загрузке"), QString::fromLocal8Bit("Количество даных в строке файла неверно"), QString::fromLocal8Bit("Ок"));
			break;

		case Errors::MainWindow::FILE_DATA_CONNECTOR_INCORRECT:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при загрузке"), QString::fromLocal8Bit("Один из коннекторов не корректен"), QString::fromLocal8Bit("Ок"));
			break;

		case Errors::MainWindow::FILE_DATA_PIN_INCORRECT:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при загрузке"), QString::fromLocal8Bit("один из пинов не корректен"), QString::fromLocal8Bit("Ок"));
			break;

		case Errors::MainWindow::FILE_DATA_DIRECTION_INCORRECT:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при загрузке"), QString::fromLocal8Bit("Одно из направлений не корректено"), QString::fromLocal8Bit("Ок"));
			break;

		case Errors::MainWindow::FILE_DATA_TYPE_INCORRECT:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при загрузке"), QString::fromLocal8Bit("Один из типов не корректен"), QString::fromLocal8Bit("Ок"));
			break;

		case Errors::MainWindow::FILE_DATA_CAN_ID_INCORRECT_HEX:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при загрузке"), QString::fromLocal8Bit("Один из can_id не корректен"), QString::fromLocal8Bit("Ок"));
			break;

		case Errors::MainWindow::FILE_DATA_BIT_INCORRECT:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при загрузке"), QString::fromLocal8Bit("Один из байтов не корректен"), QString::fromLocal8Bit("Ок"));
			break;

		case Errors::MainWindow::FILE_DATA_MIN_CURRENT_INCORRECT:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при загрузке"), QString::fromLocal8Bit("Одно из минимальных значений силы тока не корректно"), QString::fromLocal8Bit("Ок"));
			break;

		case Errors::MainWindow::FILE_DATA_MAX_CURRENT_INCORRECT:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при загрузке"), QString::fromLocal8Bit("Одно из максимальных значений силы тока не корректно"), QString::fromLocal8Bit("Ок"));
			break;

		case Errors::MainWindow::FILE_DATA_MIN_VOLTAGE_INCORRECT:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при загрузке"), QString::fromLocal8Bit("Одно из минимальных значений напряжения не корректено"), QString::fromLocal8Bit("Ок"));
			break;

		case Errors::MainWindow::FILE_DATA_MAX_VOLTAGE_INCORRECT:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при загрузке"), QString::fromLocal8Bit("Одно из максимальных значений напряжения не корректено"), QString::fromLocal8Bit("Ок"));
			break;

		case Errors::MainWindow::FILE_DATA_NAME_EMPTY:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при загрузке"), QString::fromLocal8Bit("Одно из названий не заполнено"), QString::fromLocal8Bit("Ок"));
			break;

		case Errors::MainWindow::FILE_DATA_COMPONENT_EMPTY:
			QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка при загрузке"), QString::fromLocal8Bit("Один из компонентов не заполнен"), QString::fromLocal8Bit("Ок"));
			break;
		}
		break;

	case ENGLISH_LANG:
		switch (error)
		{
		case Errors::MainWindow::FILE_OPEN:
			QMessageBox::critical(this, "Error loading file", "Unable to open file", "Ok");
			break;

		case Errors::MainWindow::FILE_MASK_CFG:
			QMessageBox::critical(this, "Error loading file", "Keyword CFG not found", "Ok");
			break;

		case Errors::MainWindow::FILE_MASK_STAND_TYPE:
			QMessageBox::critical(this, "Error loading file", "Stand type incorrect", "Ok");
			break;

		case Errors::MainWindow::FILE_DATA_AMOUNT:
			QMessageBox::critical(this, "Error loading file", "The amount of data in the row is incorrect", "Ok");
			break;

		case Errors::MainWindow::FILE_DATA_CONNECTOR_INCORRECT:
			QMessageBox::critical(this, "Error loading file", "One of the connectors is incorrect", "Ok");
			break;

		case Errors::MainWindow::FILE_DATA_PIN_INCORRECT:
			QMessageBox::critical(this, "Error loading file", "One of the pins is incorrect", "Ok");
			break;

		case Errors::MainWindow::FILE_DATA_DIRECTION_INCORRECT:
			QMessageBox::critical(this, "Error loading file", "One of the direction data is incorrect", "Ok");
			break;

		case Errors::MainWindow::FILE_DATA_TYPE_INCORRECT:
			QMessageBox::critical(this, "Error loading file", "One of the type data is incorrect", "Ok");
			break;

		case Errors::MainWindow::FILE_DATA_CAN_ID_INCORRECT_HEX:
			QMessageBox::critical(this, "Error loading file", "One of the can_id is incorrect", "Ok");
			break;

		case Errors::MainWindow::FILE_DATA_BIT_INCORRECT:
			QMessageBox::critical(this, "Error loading file", "One of the byte data is incorrect", "Ok");
			break;

		case Errors::MainWindow::FILE_DATA_MIN_CURRENT_INCORRECT:
			QMessageBox::critical(this, "Error loading file", "One of the min current value is incorrect", "Ok");
			break;

		case Errors::MainWindow::FILE_DATA_MAX_CURRENT_INCORRECT:
			QMessageBox::critical(this, "Error loading file", "One of the max current value is incorrect", "Ok");
			break;

		case Errors::MainWindow::FILE_DATA_MIN_VOLTAGE_INCORRECT:
			QMessageBox::critical(this, "Error loading file", "One of the min voltage value is incorrect", "Ok");
			break;

		case Errors::MainWindow::FILE_DATA_MAX_VOLTAGE_INCORRECT:
			QMessageBox::critical(this, "Error loading file", "One of the max voltage value is incorrect", "Ok");
			break;

		case Errors::MainWindow::FILE_DATA_NAME_EMPTY:
			QMessageBox::critical(this, "Error loading file", "One of the name data is empty", "Ok");
			break;

		case Errors::MainWindow::FILE_DATA_COMPONENT_EMPTY:
			QMessageBox::critical(this, "Error loading file", "One of the component data is empty", "Ok");
			break;
		}
		break;

	default:
		break;
	}
}

void MainWindow::generateWarning(Warnings::MainWindow warning)
{
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		switch (warning)
		{
		case Warnings::MainWindow::TEST_ACCESS_ADAPTER_SEL:
			QMessageBox::warning(this, QString::fromLocal8Bit("Внимание"), QString::fromLocal8Bit("Выберите Can-адаптер перед началом работы"));
			break;

		case Warnings::MainWindow::TEST_ACCESS_FREQUENCY_SEL:
			QMessageBox::warning(this, QString::fromLocal8Bit("Внимание"), QString::fromLocal8Bit("Выберите частоту Can-шины перед началом работы"));
			break;

		case Warnings::MainWindow::TEST_ACCESS_FILE_SEL:
			QMessageBox::warning(this, QString::fromLocal8Bit("Внимание"), QString::fromLocal8Bit("Выберите конфигурационный файл перед началом работы"));
			break;

		case Warnings::MainWindow::ADAPTERS_CHANGED:
			QMessageBox::warning(this, QString::fromLocal8Bit("Внимание"), QString::fromLocal8Bit("Изменился список активных адаптеров"));
			break;

		default:
			break;
		}
		break;

	case ENGLISH_LANG:
		switch (warning)
		{
		case Warnings::MainWindow::TEST_ACCESS_ADAPTER_SEL:
			QMessageBox::warning(this, QString("Warning"), QString("Select Can Adapter before starting"));
			break;

		case Warnings::MainWindow::TEST_ACCESS_FREQUENCY_SEL:
			QMessageBox::warning(this, QString("Warning"), QString("Select Can Bus frequency before starting work"));
			break;

		case Warnings::MainWindow::TEST_ACCESS_FILE_SEL:
			QMessageBox::warning(this, QString("Warning"), QString("Select a configuration file before you begin"));
			break;

		case Warnings::MainWindow::ADAPTERS_CHANGED:
			QMessageBox::warning(this, QString("Warning"), QString("The list of active adapter adapters has changed"));
			break;

		default:
			break;
		}
		break;
	}
}