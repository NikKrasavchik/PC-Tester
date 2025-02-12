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

#include "ui_mainwindow.h"
#include "TestWindow.h"
#include "qsliderbutton.h"
#include "can.h"
#include "WindowFrame.h"
#include "Components.h"
#include "mainwindow.h"
#include "Cable.h"

#include <QDebug>

#define TYPE_NOT_SET	NOT_SET
#define TYPE_MANUAL		0
#define TYPE_AUTO		1

#define STAND_NOT_SET	NOT_SET
#define STAND_BCM		0
#define STAND_DM		1

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

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
	QGridLayout* mainGridLayout;
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
	QVBoxLayout* leftSwitchStandVLayout;
	QVBoxLayout* leftSwitchStandButtonsVLayout;
	QHBoxLayout* leftSwitchStandHLayout;
	QLabel* logoLabel;
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
	QPushButton* outManualTestAutoStandButton;
	QPushButton* inManualTestAutoStandButton;
	QPushButton* outAutoTestAutoStandButton;
	QPushButton* inAutoTestAutoStandButton;
	QPushButton* fullTestAutoStandButton;
	QPushButton* leftStandBCMButton;
	QPushButton* leftStandDMButton;
	QComboBox* selectAdapterComboBox;
	QComboBox* selectFrequencyComboBox;
	QSpacerItem* rightAutoStandSpacer;
	QSpacerItem* leftManualStandSpacer;
	QSpacerItem* leftSwitchStandSpacer;
	QSpacerItem* rightSwitchStandSpacer;
	QSpacerItem* topSwitchStandSpacer;
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
	QSpacerItem* leftStandSwitchSpacer;
	QSpacerItem* leftStandSwitchUpSpacer;
	QPixmap* logoLightPixmap;
	QPixmap* logoDarkPixmap;
	QPixmap* themeLightPixmap;
	QPixmap* themeDarkPixmap;
	QPixmap* checkAdapterLightPixmap;
	QPixmap* checkAdapterDarkPixmap;
	QPixmap* languageLightPixmap;
	QPixmap* languageDarkPixmap;

	Can* can;
	std::vector<Cable> cables;

	bool isAllInit;
	int selectedStand;
	int selectedType;

	QString appstylePath;
	QString darkStylePath;
	QString lightStylePath;

	void initRecources();
	void initStyles();
	void initLightStyleSheets();
	void initDarkStyleSheets();
	void initTexts();
	void initIcons();
	void initConnections();
	void initCables();

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

	void fillComboBoxes();

	void switchStandButtons();
	void resetTheme();
	void resetLanguage();
	void switchStyleMainButtons();

	void resizeEvent(QResizeEvent* event);

	std::vector<Cable> prepareArguments(WindowType testType);
	void createTestWindow(WindowType testType, std::vector<Cable> preparedCables);

	void generateWarning(Warnings::MainWindow warning);

	void resetWindowView();

private slots:
	// Button
	void on_sliderSwitchStand_click();
	void on_manualStandButton_clicked();
	void on_autoStandButton_clicked();
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
	void on_leftStandBCMButton_clicked();
	void on_leftStandDMButton_clicked();

signals:
	void resizeStandSlider(int width, int height);
};
