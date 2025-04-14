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
#ifdef QT5
		case Warnings::MainWindow::TEST_ACCESS_FILE_SEL: // Warning 0x001
			QMessageBox::warning(this, QString::fromLocal8Bit("Внимание"), QString::fromLocal8Bit("Выберите конфигурационный файл перед началом работы\nWarning 0x001"));
			break;

		case Warnings::MainWindow::TEST_ACCESS_FREQUENCY_SEL: // Warning 0x002
			QMessageBox::warning(this, QString::fromLocal8Bit("Внимание"), QString::fromLocal8Bit("Выберите частоту Can-шины перед началом работы\nWarning 0x002"));
			break;

		case Warnings::MainWindow::TEST_ACCESS_ADAPTER_SEL: // Warning 0x003
			QMessageBox::warning(this, QString::fromLocal8Bit("Внимание"), QString::fromLocal8Bit("Выберите Can-адаптер перед началом работы\nWarning 0x003"));
			break;

		case Warnings::MainWindow::ADAPTERS_CHANGED: // Warning 0x004
			QMessageBox::warning(this, QString::fromLocal8Bit("Внимание"), QString::fromLocal8Bit("Изменился список активных адаптеров\nWarning 0x004"));
			break;

		case Warnings::MainWindow::SIZE_CABLE_NUL: // Warning 0x005
			QMessageBox::warning(this, QString::fromLocal8Bit("Внимание"), QString::fromLocal8Bit("Кол-во тестируемых проводов равно нулю\nWarning 0x005"));
			break;

		case Warnings::MainWindow::NOT_SELECTED_BLOCK: // Warning 0x006
			QMessageBox::warning(this, QString::fromLocal8Bit("Внимание"), QString::fromLocal8Bit("Выберите блок для тестирования\nWarning 0x006"));
			break;

		case Warnings::MainWindow::FILE_NOT_FOUND: // Warning 0x007
			QMessageBox::warning(this, QString::fromLocal8Bit("Внимание"), QString::fromLocal8Bit("Нет конфигурационного файла. Проверьте наличие в папке и перезапустите приложение\nWarning 0x007"));
			break;

		default:
			break;
#elif QT6

#endif // QT5
		}
		break;

	case ENGLISH_LANG:
		switch (warning)
		{
		case Warnings::MainWindow::TEST_ACCESS_FILE_SEL: // Warning 0x001
			QMessageBox::warning(this, QString("Warning"), QString("Select a configuration file before you begin\nWarning 0x001"));
			break;

		case Warnings::MainWindow::TEST_ACCESS_FREQUENCY_SEL: // Warning 0x002
			QMessageBox::warning(this, QString("Warning"), QString("Select Can Bus frequency before starting work\nWarning 0x002"));
			break;

		case Warnings::MainWindow::TEST_ACCESS_ADAPTER_SEL: // Warning 0x003
			QMessageBox::warning(this, QString("Warning"), QString("Select Can Adapter before starting\nWarning 0x003"));
			break;

		case Warnings::MainWindow::ADAPTERS_CHANGED: // Warning 0x004
			QMessageBox::warning(this, QString("Warning"), QString("The list of active adapter adapters has changed\nWarning 0x004"));
			break;

		case Warnings::MainWindow::SIZE_CABLE_NUL: // Warning 0x005
			QMessageBox::warning(this, QString("Warning"), QString("The number of wires tested is zero\nWarning 0x005"));
			break;

		case Warnings::MainWindow::NOT_SELECTED_BLOCK: // Warning 0x006
			QMessageBox::warning(this, QString("Warning"), QString("Select a block to test\nWarning 0x006"));
			break;

		case Warnings::MainWindow::FILE_NOT_FOUND: // Warning 0x007
			QMessageBox::warning(this, QString("Warning"), QString::fromLocal8Bit("No configuration file. Check for presence in the folder and restart the application.\nWarning 0x007"));

			break;

		default:
			break;
		}
		break;
	}
}