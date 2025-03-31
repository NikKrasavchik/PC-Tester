#include "MainWindow.h"

#define IND_CFG             0
#define IND_STAND_TYPE      1

#define COLUMN_COUNT		12
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

		case Warnings::MainWindow::SIZE_CABLE_NUL:
			QMessageBox::warning(this, QString::fromLocal8Bit("Внимание"), QString::fromLocal8Bit("Кол-во тестируемых проводов равно нулю"));
			break;

		case Warnings::MainWindow::NOT_SELECTED_BLOCK:
			QMessageBox::warning(this, QString::fromLocal8Bit("Внимание"), QString::fromLocal8Bit("Выберите блок для тестирования"));
			break;

		case Warnings::MainWindow::FILE_NOT_FOUND:
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

		case Warnings::MainWindow::NOT_SELECTED_BLOCK:
			QMessageBox::warning(this, QString("Warning"), QString("Select a block to test"));
			break;

		case Warnings::MainWindow::FILE_NOT_FOUND:
			break;

		default:
			break;
		}
		break;
	}
}