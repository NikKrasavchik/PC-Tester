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

void MainWindow::generateWarning(Warnings::MainWindow warning)
{
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		switch (warning)
		{
		case Warnings::MainWindow::TEST_ACCESS_ADAPTER_SEL:
			QMessageBox::warning(this, QString::fromLocal8Bit("��������"), QString::fromLocal8Bit("�������� Can-������� ����� ������� ������"));
			break;

		case Warnings::MainWindow::TEST_ACCESS_FREQUENCY_SEL:
			QMessageBox::warning(this, QString::fromLocal8Bit("��������"), QString::fromLocal8Bit("�������� ������� Can-���� ����� ������� ������"));
			break;

		case Warnings::MainWindow::TEST_ACCESS_FILE_SEL:
			QMessageBox::warning(this, QString::fromLocal8Bit("��������"), QString::fromLocal8Bit("�������� ���������������� ���� ����� ������� ������"));
			break;

		case Warnings::MainWindow::ADAPTERS_CHANGED:
			QMessageBox::warning(this, QString::fromLocal8Bit("��������"), QString::fromLocal8Bit("��������� ������ �������� ���������"));
			break;
		case Warnings::MainWindow::SIZE_CABLE_NUL:
			QMessageBox::warning(this, QString::fromLocal8Bit("��������"), QString::fromLocal8Bit("���-�� ����������� �������� ����� ����"));
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

		case Warnings::MainWindow::SIZE_CABLE_NUL:
			QMessageBox::warning(this, QString("Warning"), QString("The number of wires tested is zero"));
			break;

		default:
			break;
		}
		break;
	}
}