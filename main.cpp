
//    ��� ������ ��������:
//    1. �������� �� Qt � ����� VS
//    2. ���������� �� ���� �� SDK
//		2.1 Kvaser = "C:\Program Files (x86)\Kvaser\Canlib" � ���� ����� ������ ��������� ����� "INC", "lib" � ��� ��������� ������� SDK.
//		2.2 Marathon = "C:\Program Files (x86)\CHAI-2.14.0" � ���� ����� ������ ��������� ����� "include", "x64" � ��� ��������� ������� SDK.
//	  ����� ����� ������ ������ ��������� ��� ��������� x64
// 
// 
//    ���������� �������� �� ����� C++ � ��������� Visual Studio c �� ���������� ISO C++ 14, �������������� ������� QT � SDK: canlib, marathon
//    �������� C++:                 ISO C++ 14
//    ������ Visual Studio:         Visual Studio 2019 (v142)
//    ������ ������ SDK �� Windows: 10.0.19041.0
//    ������ QT:                    5.12.10
//    ������ SDK Kvaser:		    5.24
//    ������ SDK Marathon:		    CHAI 2.14.0

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