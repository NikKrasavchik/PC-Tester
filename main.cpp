#include "MainWindow.h"
#include "Components.h"
#include <QtWidgets/QApplication>
#include <QProxyStyle>
#include <QStyleFactory>
#include <QFile>
#include "Xlsx/xlsxdocument.h"
ViewWindowState* viewWindowState;

int main(int argc, char* argv[])
{

	QXlsx::Document xlsx;
	xlsx.write("A1", "123 Qt!");

	viewWindowState = new ViewWindowState;
	viewWindowState->appLanguage = RUSSIAN_LANG;
	viewWindowState->appTheme = LIGHT_THEME;
	viewWindowState->appSize.width = MIN_SCREEN_WIDTH;
	viewWindowState->appSize.height = MIN_SCREEN_HEIGHT;

	QApplication a(argc, argv);
	a.setStyle(new QProxyStyle(QStyleFactory::create(style)));

	MainWindow* mainWindow = new MainWindow();
	WindowFrame w(WindowType::MAINWINDOW, nullptr, mainWindow);
	w.setWindowIcon(QIcon(QPixmap(appLogoPath)));
	mainWindow->setParentFrame(&w);
	w.show();
	return a.exec();
}