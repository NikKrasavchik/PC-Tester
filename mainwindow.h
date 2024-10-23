#pragma once

#include <QtWidgets/QMainWindow>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QFileDialog>

#include "ui_mainwindow.h"
#include "TestWindow.h"
#include "qsliderbutton.h"
#include "StyleSheets.h"
#include "can.h"

#include <QDebug>

#define MIN_SCREEN_WIDTH    800
#define MIN_SCREEN_HEIGHT   600

#define BORDER_INDENT   25
#define TOOLBAR_SIZE    20

#define GRID_COLOUMN_0  0
#define GRID_COLOUMN_1  1
#define GRID_ROW_0      0
#define GRID_ROW_1      1

#define MIN_STAND_BUTTON_WIDTH			144
#define MIN_STAND_BUTTON_HEIGHT			62
#define MIN_STAND_SWITCH_SLIDER_WIDTH	88
#define MIN_STAND_SWITCH_SLIDER_HEIGHT	46
#define MIN_THEME_LANG_BUTTON			30
#define MIN_ADAPTER_COMBO_HEIGHT		35
#define MIN_ADAPTER_BUTTON_SIZE			35
#define MIN_FREQUENCY_COMBO_HEIGHT		35
#define MIN_CONFIGURATOR_BUTTON_WIDTH	160
#define MIN_CONFIGURATOR_BUTTON_HEIGHT	66
#define MIN_FILE_SEL_BUTTON_WIDTH		166
#define MIN_FILE_SEL_BUTTON_HEIGHT		66
#define MIN_MAIN_IN_OUT_BUTTON_WIDTH	145
#define MIN_MAIN_IN_OUT_BUTTON_HEIGHT	46
#define MIN_MAIN_FUL_BUTTON_WIDTH		200
#define MIN_MAIN_FUL_BUTTON_HEIGHT		60

#define MAX_ADAPTER_COMBO_WIDTH			263
#define MAX_FREQUENCY_COMBO_WIDTH		300

#define COEF_STAND_BUTTON				0.1
#define COEF_STAND_SLIDER				0.06
#define COEF_THEME_LANG_BUTTON			0.03
#define COEF_ADAPTER_GROUP				0.05
#define COEF_FREQUENC_COMBO				0.05
#define COEF_CONFIGURATOR_BUTTON		0.05
#define COEF_FILE_SEL_BUTTON			0.05
#define COEF_MAIN_BUTTON				0.05

#define LIGHT_THEME		0
#define DARK_THEME		1

#define RUSSIAN_LANG	0
#define ENGLISH_LANG	1

#define OVERCROWDED_SEL_FILE_LABEL		26
#define CFG_EXTENSION_LETTERS_COUNT		4

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

private:
	Ui::MainWindowClass ui;

	QWidget* mainLayoutWidget;
	QWidget* manualStandWidget;
	QWidget* autoStandWidget;
	QWidget* fullTestAutoStandWidget;
	QWidget* manualTestAutoStandWidget;
	QWidget* autoTestAutoStandWidget;
	QWidget* backgroundManualStandWidget;
	QGridLayout* mainGridLayout;
	QHBoxLayout* topHLayout;
	QHBoxLayout* switchStandHLayout;
	QHBoxLayout* findAdapterHLayout;
	QHBoxLayout* configuratorHLayout;
	QHBoxLayout* partitionTestAutoStandHLayout;
	QHBoxLayout* manualStandMainHLayout;
	QHBoxLayout* leftHLayout;
	QHBoxLayout* selectFileHLayout;
	QHBoxLayout* backgroundManualStandHLayout;
	QHBoxLayout* autoStandMainHLayout;
	QHBoxLayout* manualTestAutoStandHLayout;
	QHBoxLayout* autoTestAutoStandHLayout;
	QHBoxLayout* fullTestAutoStandHLayout;
	QHBoxLayout* fullTestAutoStandMiddleHLayout;
	QVBoxLayout* fullTestAutoStandVLayout;
	QVBoxLayout* autoTestAutoStandVLayout;
	QVBoxLayout* manualTestAutoStandVLayout;
	QVBoxLayout* autoStandMainVLayout;
	QVBoxLayout* testManualStandVLayout;
	QVBoxLayout* leftVLayout;
	QVBoxLayout* leftSettingsVLayout;
	QVBoxLayout* selectAdapterVLayout;
	QVBoxLayout* selectFrequencyVLayout;
	QVBoxLayout* selectFileVLayout;
	QVBoxLayout* mainVLayout;
	QVBoxLayout* switchThemeLanguageVLayout;
	QVBoxLayout* manualStandMainVLayout;
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
	QPushButton* fullTestManualStandButton;
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
	QSpacerItem* topFrequencySpacer;
	QSpacerItem* topSettingsSpacer;
	QSpacerItem* botSettingsSpacer;
	QSpacerItem* topSelectFileSpacer;
	QSpacerItem* topConfiguratorSpacer;
	QSpacerItem* findAdapterCenterSpacer;
	QSpacerItem* selectAdapterMiddleSpacer;
	QSpacerItem* configuratorLeftSpacer;
	QSpacerItem* manualStandMainLeftSpacer;
	QSpacerItem* manualStandMainRightSpacer;
	QSpacerItem* manualStandMainUpSpacer;
	QSpacerItem* manualStandMainBottomSpacer;
	QSpacerItem* backgroundManualStandMainUpSpacer;
	QSpacerItem* backgroundManualStandMainBottomSpacer;
	QSpacerItem* backgroundManualStandMainRightSpacer;
	QSpacerItem* backgroundManualStandMainLeftSpacer;
	QSpacerItem* manualTestAutoStandLeftSpacer;
	QSpacerItem* manualTestAutoStandRightSpacer;
	QSpacerItem* frequencyMiddleSpacer;
	QSpacerItem* selectFileMiddleSpacer;
	QSpacerItem* selectFileLeftSpacer;
	QSpacerItem* selectFileRightSpacer;
	QSpacerItem* testManualStandMiddleUpSpacer;
	QSpacerItem* testManualStandMiddleBottomSpacer;
	QSpacerItem* testManualStandMiddleFooterSpacer;
	QSpacerItem* autoStandMainLeftSpacer;
	QSpacerItem* autoStandMainRightSpacer;
	QSpacerItem* autoStandMainUpSpacer;
	QSpacerItem* autoStandMainMiddleSpacer;
	QSpacerItem* autoStandMainBottomSpacer;
	QSpacerItem* partitionTestAutoStandSpacer;
	QSpacerItem* manualTestAutoStandUpSpacer;
	QSpacerItem* manualTestAutoStandBottomSpacer;
	QSpacerItem* manualTestAutoStandMiddleUpSpacer;
	QSpacerItem* manualTestAutoStandMiddleBottomSpacer;
	QSpacerItem* autoTestAutoStandLeftSpacer;
	QSpacerItem* autoTestAutoStandRightSpacer;
	QSpacerItem* autoTestAutoStandUpSpacer;
	QSpacerItem* autoTestAutoStandBottomSpacer;
	QSpacerItem* autoTestAutoStandMiddleUpSpacer;
	QSpacerItem* autoTestAutoStandMiddleBottomSpacer;
	QSpacerItem* fullTestAutoStandOuterLeftSpacer;
	QSpacerItem* fullTestAutoStandOuterRightSpacer;
	QSpacerItem* fullTestAutoStandLeftSpacer;
	QSpacerItem* fullTestAutoStandRightSpacer;
	QSpacerItem* fullTestAutoStandUpSpacer;
	QSpacerItem* fullTestAutoStandBottomSpacer;
	QFrame* selectFileLine;
	QPixmap* logoLightPixmap;
	QPixmap* logoDarkPixmap;
	QPixmap* themeLightPixmap;
	QPixmap* themeDarkPixmap;
	QPixmap* checkAdapterLightPixmap;
	QPixmap* checkAdapterDarkPixmap;
	QPixmap* languageLightPixmap;
	QPixmap* languageDarkPixmap;

	Can* can;

	bool appTheme;
	bool appLanguage;
	bool fileSelected;
	bool isAllInit = false;
	bool selectedStand;

	QString selectedFullFileName;

	void initStyleSheets();
	void initRecources();
	void initStyles();
	void initTexts();
	void initIcons();
	void initConnections();

	void initUi();
	void initUiLogo();
	void initUiTopHLayout();
	void initUiLeftHLayout();
	void initUiMainVLayout();

	void initUiSwitchStand();
	void initUiSwitchThemeLang();
	void initUiAdapter();
	void initUiFrequency();
	void initUiSelectFile();
	void initUiConfigurator();
	void initUiAutoStand();
	void initUiManualStand();
	void initUiAutoStandManualTest();
	void initUiAutoStandAutoTest();
	void initUiAutoStandFullTest();

	void fillComboBoxes();

	void switchStandButtons();
	void switchTheme();
	void switchLanguage();
	void switchStyleMainButtons();

	void resizeEvent(QResizeEvent* event);

	void createTestWindow(TestWindowType testType);

private slots:
	// Button
	void on_sliderSwitchStand_click();
	void on_manualStandButton_clicked();
	void on_autoStandButton_clicked();
	void on_selectFileButton_clicked();
	void on_switchThemeButton_clicked();
	void on_switchLanguageButton_clicked();
	void on_checkAdaptersButton_clicked();
	// ComboBox
	void on_selectFrequencyComboBox_changed(int index);
	void on_selectAdapterComboBox_changed(int index);
	// main
	void on_outTestManualStandButton_clicked();
	void on_inTestManualStandButton_clicked();
	void on_fullTestManualStandButton_clicked();
	void on_inManualTestAutoStandButton_clicked();
	void on_outManualTestAutoStandButton_clicked();
	void on_inAutoTestAutoStandButton_clicked();
	void on_outAutoTestAutoStandButton_clicked();
	void on_fullTestAutoStandButton_clicked();

signals:
	void resizeStandSlider(int width, int height);
};