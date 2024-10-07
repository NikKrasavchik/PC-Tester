#pragma once

#include <QtWidgets/QMainWindow>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>

#include "ui_mainwindow.h"
//#include "qsliderbutton.h"
#include "stylesheets.h"

#include <QDebug>

#define MIN_SCREEN_WIDTH    800
#define MIN_SCREEN_HEIGHT   600

#define LOGO_LIGHT_QRC  ":/MainWindow/Recources/Logo_Light.png"
#define LOGO_DARK_QRC  ":/MainWindow/Recources/Logo_Light.png"

#define GRID_COLOUMN_0  0
#define GRID_COLOUMN_1  1
#define GRID_ROW_0      0
#define GRID_ROW_1      1

#define BORDER_INDENT   25
#define TOOLBAR_SIZE    20

#define COEFFICIENT_WIDTH_LOGO              220 / MINIMUM_SCREEN_WIDTH
#define COEFFICIENT_HEIGHT_LOGO             55 / MINIMUM_SCREEN_HEIGHT
#define COEFFICIENT_GRID_0_ROW_HEIGHT       80 / MINIMUM_SCREEN_HEIGHT

#define COEFFICIENT_STAND_BUTTONS_WIDTH     0.1
#define COEFFICIENT_STAND_BUTTONS_HEIGHT    0.05

#define COEFFICIENT_STAND_SWITCHER_WIDTH    0.11
#define COEFFICIENT_STAND_SWITCHER_HEIGHT   0.07

#define COEFFICIENT_THEME_BUTTON_SIZE        0.08

#define MIN_STAND_BUTTON_WIDTH			144
#define MIN_STAND_BUTTON_HEIGHT			62
#define MIN_STAND_SWITCH_BUTTON_WIDTH	88
#define MIN_STAND_SWITCH_BUTTON_HEIGHT	46
#define MIN_THEME_LANG_BUTTON_WIDTH		
#define MIN_THEME_LANG_BUTTON_HEIGHT	
#define MIN_ADAPTER_COMBO_WIDTH			
#define MIN_ADAPTER_COMBO_HEIGHT		
#define MIN_ADAPTER_BUTTON_WIDTH		
#define MIN_ADAPTER_BUTTON_HEIGHT		
#define MIN_FREQUENCY_COMBO_WIDTH		
#define MIN_FREQUENCY_COMBO_HEIGHT		
#define MIN_CONFIGURATOR_BUTTON_WIDTH	
#define MIN_CONFIGURATOR_BUTTON_HEIGHT	
#define MIN_FILE_SEL_BUTTON_WIDTH		
#define MIN_FILE_SEL_BUTTON_HEIGHT		
#define MIN_FILE_SEL_LABEL_WIDTH		
#define MIN_FILE_SEL_LABEL_HEIGHT		

#define COEF_STAND_BUTTON				0.103


#define MANUAL_STAND    0
#define AUTO_STAND      1

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

private:
	Ui::MainWindowClass ui;

	QWidget* mainLayoutWidget;
	QGridLayout* mainGridLayout;
	QHBoxLayout* logoHLayout;
	QHBoxLayout* topHLayout;
	QHBoxLayout* switchHLayout;
	QHBoxLayout* findAdapterHLayout;
	QHBoxLayout* autoTestManualStandHLayout;
	QHBoxLayout* manualTestManualStandHLayout;
	QVBoxLayout* leftVLayout;
	QVBoxLayout* selectAdapterVLayout;
	QVBoxLayout* selectFrequencyVLayout;
	QVBoxLayout* selectFileVLayout;
	QVBoxLayout* mainVLayout;
	QVBoxLayout* switchThemeLanguageVLayout;
	QLabel* logoLabel;
	QLabel* manualStandLabel;
	QLabel* autoStandLabel;
	QLabel* selectAdapterLabel;
	QLabel* selectFrequencyLabel;
	QLabel* selectFileLabel;
	QPushButton* switchStandButton;
	QPushButton* switchThemeButton;
	QPushButton* switchLanguageButton;
	QPushButton* checkAdaptersButton;
	QPushButton* configuratorButton;
	QPushButton* selectFileButton;
	QPushButton* manualStandButton;
	QPushButton* autoStandButton;
	QPushButton* outManualTestManualStandButton;
	QPushButton* outAutoTestManualStandButton;
	QPushButton* inManualTestManualStandButton;
	QPushButton* inAutoTestManualStandButton;
	QPushButton* fullTestManualStandButton;
	QPushButton* outTestAutoStandButton;
	QPushButton* inTestAutoStandButton;
	QComboBox* selectAdapterComboBox;
	QComboBox* selectFrequencyComboBox;
	QSpacerItem* rightAutoStandSpacer;
	QSpacerItem* leftManualStandSpacer;
	QSpacerItem* leftManualStandSpacer1;
	QSpacerItem* leftSwitchStandSpacer;
	QSpacerItem* rightSwitchStandSpacer;
	QSpacerItem* topAdapterSpacer;
	QSpacerItem* topFrequencySpacer;
	QSpacerItem* topConfiguratorSpacer;
	QSpacerItem* topSelectFileSpacer;
	QSpacerItem* bottomSpacer;

	QPixmap* logoPixmap;

	bool switchStandState;

	void initStyles();
	void initUi();
	void initUiLogo();
	void initUiTopHLayout();
	void initUiLeftVLayout();
	void initUiMainVLayout();

	void switchStandButtons();

	void resizeEvent(QResizeEvent* event);

private slots:
	void on_switchStandButton_clicked();
	void on_manualStandButton_clicked();
	void on_autoStandButton_clicked();
};
