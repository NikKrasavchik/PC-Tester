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
#include <utility>

#include "ui_mainwindow.h"
#include "TestWindow.h"
#include "ConfiguratorWindow.h"
#include "qsliderbutton.h"
#include "can.h"
#include "WindowFrame.h"
#include "Components.h"

#include <QDebug>

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

	bool isFileSelected;
	bool isAllInit;
	bool selectedStand;

	QString selectedFileFullName;
	QString fileName;
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
	void resetTheme();
	void resetLanguage();
	void switchStyleMainButtons();

	void resizeEvent(QResizeEvent* event);

	void createTestWindow(WindowType testType);

	void resetWindowView();

private slots:
	// Button
	void on_sliderSwitchStand_click();
	void on_manualStandButton_clicked();
	void on_autoStandButton_clicked();
	void on_selectFileButton_clicked();
	void on_switchThemeButton_clicked();
	void on_switchLanguageButton_clicked();
	void on_checkAdaptersButton_clicked();
	void on_configuratorButton_clicked();
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