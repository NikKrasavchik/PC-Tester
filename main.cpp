
//    ДЛЯ СБОРКИ УБЕДИТСЯ:
//    1. Настроен ли Qt в вашей VS
//    2. Правильный ди путь до SDK
//		2.1 Kvaser = "C:\Program Files (x86)\Kvaser\Canlib" в этой папке должны находится папки "INC", "lib" и все остальные ресурсы SDK.
//		2.2 Marathon = "C:\Program Files (x86)\CHAI-2.14.0" в этой папке должны находится папки "include", "x64" и все остальные ресурсы SDK.
//	  ПОСЛЕ ЭТОГО ПРОЕКТ ДОЛЖЕН СОБРАТЬСЯ ДЛЯ ПЛАТФОРМЫ x64
// 
// 
//    Приложение написано на языке C++ в редакторе Visual Studio c со стандартом ISO C++ 14, использованием графики QT и SDK: canlib, marathon
//    Стандарт C++:                 ISO C++ 14
//    Версия Visual Studio:         Visual Studio 2019 (v142)
//    Версия пакета SDK на Windows: 10.0.19041.0
//    Версия QT:                    5.12.10
//    Версия SDK Kvaser:		    5.24
//    Версия SDK Marathon:		    CHAI 2.14.0

#include "MainWindow.h"
#include "Components.h"
#include <QtWidgets/QApplication>
#include <QProxyStyle>
#include <QStyleFactory>
#include <QFile>
ViewWindowState* viewWindowState;


int main(int argc, char* argv[])
{

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