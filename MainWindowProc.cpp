#include "MainWindow.h"

#define IND_CFG             0
#define IND_STAND_TYPE      1

#define COLOUMN_COUNT       12
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

#define MASK_CFG            "CFG"
#define MASK_STAND_MANUAL   "MANUAL"
#define MASK_STAND_AUTO     "AUTO"

#define PIN_NOT_SET			-1
#define MIN_CURRENT_NOT_SET	-1
#define MAX_CURRENT_NOT_SET	-1
#define MIN_VOLTAGE_NOT_SET	-1
#define MAX_VOLTAGE_NOT_SET -1
#define NAME_NOT_SET		""

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

		if (dataList.size() == 2)
		{
			if (dataList[IND_CFG] != MASK_CFG)
			{
				isFileCorrect = false;
				// ERROR
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
				// ERROR
				return;
			}
		}
		else
		{
			Cable cable;
			cable.id = id++;
			for (int column = 0; column < COLOUMN_COUNT && isFileCorrect; column++)
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
						// ERROR
					}
					break;

				case IND_PIN:
					if (verifyData(column, currentProperty))
						cable.pin = currentProperty.toInt();
					else
					{
						isFileCorrect = false;
						// ERROR
					}
					break;

				case IND_DIRECTION:
					if (verifyData(column, currentProperty))
						cable.direction = currentProperty.toInt();
					else
					{
						isFileCorrect = false;
						// ERROR
					}
					break;

				case IND_TYPE:
					if (verifyData(column, currentProperty))
						cable.type = currentProperty.toInt();
					else
					{
						isFileCorrect = false;
						// ERROR
					}
					break;

				case IND_CAN_ID:
					if (verifyData(column, currentProperty))
						cable.canId = currentProperty.toInt(nullptr, 16);
					else
					{
						isFileCorrect = false;
						// ERROR
					}
					break;

				case IND_BIT:
					if (verifyData(column, currentProperty))
						cable.bit = currentProperty.toInt();
					else
					{
						isFileCorrect = false;
						// ERROR
					}
					break;

				case IND_MIN_CURRENT:
					if (verifyData(column, currentProperty))
						cable.minCurrent = currentProperty.toDouble();
					else
					{
						isFileCorrect = false;
						// ERROR
					}
					break;

				case IND_MAX_CURRENT:
					if (verifyData(column, currentProperty))
						cable.maxCurrent = currentProperty.toDouble();
					else
					{
						isFileCorrect = false;
						// ERROR
					}
					break;

				case IND_MIN_VOLTAGE:
					if (verifyData(column, currentProperty))
						cable.minVoltage = currentProperty.toDouble();
					else
					{
						isFileCorrect = false;
						// ERROR
					}
					break;

				case IND_MAX_VOLTAGE:
					if (verifyData(column, currentProperty))
						cable.maxVoltage = currentProperty.toDouble();
					else
					{
						isFileCorrect = false;
						// ERROR
					}
					break;

				case IND_NAME:
					if (verifyData(column, currentProperty))
						cable.name = currentProperty;
					else
					{
						isFileCorrect = false;
						// ERROR
					}
					break;

				case IND_COMPONENT:
					if (verifyData(column, currentProperty))
						cable.component = currentProperty;
					else
					{
						isFileCorrect = false;
						// ERROR
					}
					break;
				}
			}
			cables.push_back(cable);
		}
	}
	if (!isFileCorrect)
		cables.clear();

	file.close();
}