#pragma once

#include <QtWidgets/QMainWindow>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>

#include "ui_mainwindow.h"
#include "qsliderbutton.h"
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

#define MIN_STAND_BUTTON_WIDTH			144
#define MIN_STAND_BUTTON_HEIGHT			62
#define MIN_STAND_SWITCH_SLIDER_WIDTH	88
#define MIN_STAND_SWITCH_SLIDER_HEIGHT	46
#define MIN_THEME_LANG_BUTTON			30
#define MIN_ADAPTER_COMBO_WIDTH			120
#define MIN_ADAPTER_COMBO_HEIGHT		30
#define MIN_ADAPTER_BUTTON_SIZE			30
#define MIN_FREQUENCY_COMBO_WIDTH		170
#define MIN_FREQUENCY_COMBO_HEIGHT		30
#define MIN_CONFIGURATOR_BUTTON_WIDTH	160
#define MIN_CONFIGURATOR_BUTTON_HEIGHT	66
#define MIN_FILE_SEL_BUTTON_WIDTH		166
#define MIN_FILE_SEL_BUTTON_HEIGHT		66
#define MIN_MAIN_IN_OUT_BUTTON_WIDTH	145
#define MIN_MAIN_IN_OUT_BUTTON_HEIGHT	46
#define MIN_MAIN_FUL_BUTTON_WIDTH		200
#define MIN_MAIN_FUL_BUTTON_HEIGHT		60

#define COEF_STAND_BUTTON				0.1
#define COEF_STAND_SLIDER				0.06
#define COEF_THEME_LANG_BUTTON			0.1
#define COEF_ADAPTER_GROUP				0.05
#define COEF_FREQUENC_COMBO				0.05
#define COEF_CONFIGURATOR_BUTTON		0.05
#define COEF_FILE_SEL_BUTTON			0.05
#define COEF_MAIN_BUTTON				0.05

#define MANUAL_STAND    0
#define AUTO_STAND      1

#define LIGHT_THEME		0
#define DARK_THEME		1

#define RUSSIAN_LANG	0
#define ENGLISH_LANG	1

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

private:
	Ui::MainWindowClass ui;

	QWidget* mainLayoutWidget;
	QWidget* manualTestAutoStandWidget;
	QWidget* autoTestAutoStandWidget;
	QWidget* manualStandWidget;
	QGridLayout* mainGridLayout;
	QHBoxLayout* logoHLayout;
	QHBoxLayout* topHLayout;
	QHBoxLayout* switchHLayout;
	QHBoxLayout* selectAdapterHLayout;
	QHBoxLayout* findAdapterHLayout;
	QHBoxLayout* selectFileHLayout;
	QHBoxLayout* configuratorHLayout;
	QHBoxLayout* partitionTestAutoStandHLayout;
	QHBoxLayout* manualStandMainHLayout;
	QHBoxLayout* fullAutoStandMainHLayout;
	QVBoxLayout* leftVLayout;
	QVBoxLayout* selectAdapterVLayout;
	QVBoxLayout* selectFrequencyVLayout;
	QVBoxLayout* selectFileVLayout;
	QVBoxLayout* mainVLayout;
	QVBoxLayout* switchThemeLanguageVLayout;
	QVBoxLayout* manualTestAutoStandVLayout;
	QVBoxLayout* autoTestAutoStandVLayout;
	QVBoxLayout* manualStandMainVLayout;
	QVBoxLayout* manualStandVLayout;
	QLabel* logoLabel;
	QLabel* selectAdapterLabel;
	QLabel* selectFrequencyLabel;
	QLabel* selectFileLabel;
	QLabel* manualStandLabel;
	QLabel* manualTestAutoStandLabel;
	QLabel* autoTestAutoStandLabel;
	QSliderButton* switchStandSlider;
	QPushButton* switchThemeButton;
	QPushButton* switchLanguageButton;
	QPushButton* checkAdaptersButton;
	QPushButton* configuratorButton;
	QPushButton* selectFileButton;
	QPushButton* manualStandButton;
	QPushButton* autoStandButton;
	QPushButton* outTestManualStandButton;
	QPushButton* inTestManualStandButton;
	QPushButton* outManualTestAutoStandButton;
	QPushButton* inManualTestAutoStandButton;
	QPushButton* outAutoTestAutoStandButton;
	QPushButton* inAutoTestAutoStandButton;
	QPushButton* fullTestAutoStandButton;
	QComboBox* selectAdapterComboBox;
	QComboBox* selectFrequencyComboBox;
	QSpacerItem* rightAutoStandSpacer;
	QSpacerItem* leftManualStandSpacer;
	QSpacerItem* leftSwitchStandSpacer;
	QSpacerItem* rightSwitchStandSpacer;
	QSpacerItem* topAdapterSpacer;
	QSpacerItem* topFrequencySpacer;
	QSpacerItem* topConfiguratorSpacer;
	QSpacerItem* topSelectFileSpacer;
	QSpacerItem* bottomSpacer;
	QSpacerItem* adapterLeftSpacer;
	QSpacerItem* adapterRightSpacer;
	QSpacerItem* findAdapterCenterSpacer;
	QSpacerItem* selectAdapterMiddleSpacer;
	QSpacerItem* configuratorLeftSpacer;
	QSpacerItem* configuratorRightSpacer;
	QSpacerItem* selectFileLeftSpacer;
	QSpacerItem* selectFileRightSpacer;
	QSpacerItem* manualTestAutoStandSpacer;
	QSpacerItem* autoTestAutoStandSpacer;
	QSpacerItem* manualStandMainLeftSpacer;
	QSpacerItem* manualStandMainRightSpacer;
	QSpacerItem* manualStaneMainUpSpacer;
	QSpacerItem* manualStandMainMiddleSpacer;
	QSpacerItem* manualStaneMainBottomSpacer;
	QSpacerItem* fullAutoStandMainLeftSpacer;
	QSpacerItem* fullAutoStandMainRightSpacer;
	QSpacerItem* manualTestAutoStandLeftSpacer;
	QSpacerItem* manualTestAutoStandRightSpacer;
	QSpacerItem* manualTestAutoStandMiddleSpacer;
	QSpacerItem* manualTestAutoStandFooterSpacer;

	QPixmap* logoLightPixmap;
	QPixmap* logoDarkPixmap;
	QPixmap* themeLightPixmap;
	QPixmap* themeDarkPixmap;
	QPixmap* checkAdapterLightPixmap;
	QPixmap* checkAdapterDarkPixmap;
	QPixmap* languageLightPixmap;
	QPixmap* languageDarkPixmap;

	bool switchStandState;
	bool appTheme;
	bool appLanguage;

	void initStyles();
	void initRecources();
	void initUi();
	void initUiLogo();
	void initUiTopHLayout();
	void initUiLeftVLayout();
	void initUiMainVLayout();

	void switchStandButtons();
	void switchTheme();
	void switchLanguage();

	void resizeEvent(QResizeEvent* event);

private slots:
	void on_sliderSwitchStand_click();
	void on_manualStandButton_clicked();
	void on_autoStandButton_clicked();
	void on_selectFileButton_clicked();
	void on_switchThemeButton_clicked();
	void on_switchLanguageButton_clicked();

signals:
	void resizeStandSlider(int width, int height);
};