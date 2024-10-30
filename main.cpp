#include "MainWindow.h"
#include "Components.h"
#include <QtWidgets/QApplication>
#include <QProxyStyle>
#include <QStyleFactory>
#include <QFile>

ViewWindowState* viewWindowState;

const QString style = "Fusion";
const QString appstylePath = ":/recources/style/appstyles.qss";

int main(int argc, char *argv[])
{
    viewWindowState = new ViewWindowState;
    viewWindowState->appLanguage = RUSSIAN_LANG;
    viewWindowState->appTheme = LIGHT_THEME;
    viewWindowState->appSize.width = MIN_SCREEN_WIDTH;
    viewWindowState->appSize.height = MIN_SCREEN_HEIGHT;

    QApplication a(argc, argv);
    a.setStyle(new QProxyStyle(QStyleFactory::create(style)));

    MainWindow* mainWindow = new MainWindow();
    WindowFrame w(nullptr, mainWindow);
    mainWindow->setParentFrame(&w);
    w.show();
    return a.exec();
}
