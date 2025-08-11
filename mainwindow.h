#pragma once

#include <QtWidgets/QMainWindow>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QFileDialog>
#include <QApplication>
#include <QProxyStyle>
#include <QStyleFactory>
#include <QFile>
#include <qmessagebox.h>
#include <utility>
#include <qtooltip.h>
#include <qpalette.h>

#include "ui_mainwindow.h"
#include "TestWindow.h"
#include "qsliderbutton.h"
#include "can.h"
#include "WindowFrame.h"
#include "Components.h"
#include "mainwindow.h"
#include "verificationtest.h"
#include "Cable.h"

#include <QDebug>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

	// ------------------------------------
	// Name: setParentFrame
	//		Сохранение родительского элемента
	// Variables: 
	//			WindowFrame* parentFrame: родительских элемент
	// ------------------------------------
	void setParentFrame(WindowFrame* parentFrame);

private:
	Ui::MainWindowClass ui;
	WindowFrame* parentFrame;

	QWidget* mainLayoutWidget;
	QWidget* manualStandWidget;
	QWidget* autoStandWidget;
	QWidget* fullTestAutoStandWidget;
	QWidget* manualTestAutoStandWidget;
	QWidget* autoTestAutoStandWidget;
	QWidget* backgroundManualStandWidget;
	QWidget* topWidget;
	QWidget* switchTypeWidget;
	QWidget* switchThemeLanguageWidget;
	QWidget* leftHWidget;
	QWidget* leftVWidget;
	QWidget* leftSwitchBlockWidget;
	QWidget* selectBlockVersionVWidget;
	QWidget* selectBlockVersionHWidget;
	QWidget* leftSettingsWidget;
	QWidget* selectAdapterWidget;
	QWidget* findAdapterWidget;
	QWidget* selectFrequencyWidget;
	QWidget* logoPixmapWidget;
	QWidget* logoWidget;
	QGridLayout* mainGridLayout;
	QHBoxLayout* logoHLayout;
	QHBoxLayout* switchTypeHLayout;
	QHBoxLayout* topHLayout;
	QHBoxLayout* findAdapterHLayout;
	QHBoxLayout* partitionTestAutoStandHLayout;
	QHBoxLayout* manualStandMainHLayout;
	QHBoxLayout* leftHLayout;
	QHBoxLayout* backgroundManualStandHLayout;
	QHBoxLayout* autoStandMainHLayout;
	QHBoxLayout* manualTestAutoStandHLayout;
	QHBoxLayout* autoTestAutoStandHLayout;
	QHBoxLayout* fullTestAutoStandHLayout;
	QHBoxLayout* fullTestAutoStandMiddleHLayout;
	QHBoxLayout* selectBlockVersionHLayout;
	QVBoxLayout* fullTestAutoStandVLayout;
	QVBoxLayout* autoTestAutoStandVLayout;
	QVBoxLayout* manualTestAutoStandVLayout;
	QVBoxLayout* autoStandMainVLayout;
	QVBoxLayout* testManualStandVLayout;
	QVBoxLayout* leftVLayout;
	QVBoxLayout* leftSettingsVLayout;
	QVBoxLayout* selectAdapterVLayout;
	QVBoxLayout* selectFrequencyVLayout;
	QVBoxLayout* mainVLayout;
	QVBoxLayout* switchThemeLanguageVLayout;
	QVBoxLayout* manualStandMainVLayout;
	QVBoxLayout* leftSwitchBlockVLayout;
	QVBoxLayout* selectBlockVersionVLayout;
	QLabel* logoLabel;
	QLabel* selectBlockVersionLabel;
	QLabel* selectAdapterLabel;
	QLabel* selectFrequencyLabel;
	QLabel* manualStandLabel;
	QLabel* manualTestAutoStandLabel;
	QLabel* autoTestAutoStandLabel;
	QSliderButton* switchStandSlider;
	QPushButton* switchThemeButton;
	QPushButton* switchLanguageButton;
	QPushButton* checkAdaptersButton;
	QPushButton* manualStandButton;
	QPushButton* autoStandButton;
	QPushButton* outTestManualStandButton;
	QPushButton* inTestManualStandButton;
	QPushButton* fullTestManualStandButton;
	QPushButton* verificationtestTestManualStandButton;
	QPushButton* outManualTestAutoStandButton;
	QPushButton* inManualTestAutoStandButton;
	QPushButton* outAutoTestAutoStandButton;
	QPushButton* inAutoTestAutoStandButton;
	QPushButton* fullTestAutoStandButton;
	QPushButton* leftBlockBCMButton;
	QPushButton* leftBlockDMButton;
	QComboBox* selectBlockVersionComboBox;
	QComboBox* selectAdapterComboBox;
	QComboBox* selectFrequencyComboBox;
	QSpacerItem* rightAutoStandSpacer;
	QSpacerItem* leftManualStandSpacer;
	QSpacerItem* leftSwitchStandSpacer;
	QSpacerItem* rightSwitchStandSpacer;
	QSpacerItem* topFrequencySpacer;
	QSpacerItem* topSettingsSpacer;
	QSpacerItem* botSettingsSpacer;
	QSpacerItem* findAdapterCenterSpacer;
	QSpacerItem* selectAdapterMiddleSpacer;
	QSpacerItem* manualStandMainLeftSpacer;
	QSpacerItem* manualStandMainRightSpacer;
	QSpacerItem* manualStandMainUpSpacer;
	QSpacerItem* manualStandMainBottomSpacer;
	QSpacerItem* backgroundManualStandMainUpSpacer;
	QSpacerItem* backgroundManualStandMainBottomSpacer;
	QSpacerItem* backgroundManualStandMainBottomSecondSpacer;
	QSpacerItem* backgroundManualStandMainRightSpacer;
	QSpacerItem* backgroundManualStandMainLeftSpacer;
	QSpacerItem* manualTestAutoStandLeftSpacer;
	QSpacerItem* manualTestAutoStandRightSpacer;
	QSpacerItem* frequencyMiddleSpacer;
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
	QSpacerItem* leftStandSwitchSpacer;
	QSpacerItem* leftStandSwitchUpSpacer;
	QSpacerItem* selectBlockVersionUpSpacer;
	QSpacerItem* selectBlockVersionLeftSpacer;
	QSpacerItem* selectBlockVersionRightSpacer;
	QSpacerItem* leftHLayoutLeftSpacer;
	QSpacerItem* leftHLayoutRightSpacer;
	QPixmap* logoLightPixmap;
	QPixmap* logoDarkPixmap;
	QPixmap* themeLightPixmap;
	QPixmap* themeDarkPixmap;
	QPixmap* checkAdapterLightPixmap;
	QPixmap* checkAdapterDarkPixmap;
	QPixmap* languageLightPixmap;
	QPixmap* languageDarkPixmap;
	
	QPalette DarkPalette;
	QTimer* timerCheckAdapter;

	Can* can;
	std::vector<Cable> cables;
	std::vector<QString> blockVersionsDTM;
	std::vector<QString> blockVersionsBCM;

	bool isAllInit;
	TypeStand selectedTypeStand;

	void initRecources();
	void initStyles();
	void initLightStyleSheets();
	void initDarkStyleSheets();
	void initIcons();
	void initTips();

	void initConnections();
	void initBlockVersions();
	void loadCables(TestBlockName block, QString version);

	void initUi();
	void initUiLogo();
	void initUiTopHLayout();
	void initUiLeftHLayout();
	void initUiMainVLayout();
	

	void initUiSwitchType();
	void initUiSwitchThemeLang();
	void initUiSwitchStand();
	void initUiAdapter();
	void initUiFrequency();
	void initUiAutoStand();
	void initUiManualStand();
	void initUiAutoStandManualTest();
	void initUiAutoStandAutoTest();	
	void initUiAutoStandFullTest();

	void initConfig();
	void resetConfig();

	void fillComboBoxes();

	void switchStandButtons();
	void resetTheme();
	void resetLanguage();
	void switchStyleMainButtons();

	void resizeEvent(QResizeEvent* event);

	void createTestWindow(WindowType testType, std::vector<Cable> preparedCables);

	void generateWarning(Warnings::MainWindow warning);

	void resetWindowView();

private slots:
	// Buttons
	void slot_sliderSwitchStand_clicked();
	void slot_manualStandButton_clicked();
	void slot_autoStandButton_clicked();
	void slot_switchThemeButton_clicked();
	void slot_switchLanguageButton_clicked();
	void slot_checkAdaptersButton_clicked();
	void slot_leftBlockBCMButton_clicked();
	void slot_leftBlockDMButton_clicked();

	// ComboBoxes
	void slot_selectBlockVersionComboBox_changed(int index);
	void slot_selectFrequencyComboBox_changed(int index);
	void slot_selectAdapterComboBox_changed(int index);

	// Test buttons
	void slot_outTestManualStandButton_clicked();
	void slot_inTestManualStandButton_clicked();
	void slot_fullTestManualStandButton_clicked();
	void slot_verificationtestTestManualStandButton_clicked();
	void slot_inManualTestAutoStandButton_clicked();
	void slot_outManualTestAutoStandButton_clicked();
	void slot_inAutoTestAutoStandButton_clicked();
	void slot_outAutoTestAutoStandButton_clicked();
	void slot_fullTestAutoStandButton_clicked();



signals:
	void resizeStandSlider(int width, int height);
};
