#pragma once

#include <QDialog>
#include <QFormLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>

#include "ui_TestWindow.h"

#define MIN_SCREEN_WIDTH    800
#define MIN_SCREEN_HEIGHT   600

#define BORDER_INDENT		25
#define TOOLBAR_SIZE		20

#define FIXED_HEADER_HEIGHT			80
#define FIXED_FOOTER_HEIGHT			80
#define FIXED_LOGO_WIDTH			220
#define FIXED_LOGO_HEIGHT			55
#define THEME_LANG_BUTTON_SIZE		30
#define BACK_BUTTON_SIZE			50
#define FIXED_FILE_NAME_WIDTH		200
#define FIXED_FILE_NAME_HEIGHT		50
#define FIXED_TESTER_NAME_WIDTH		300
#define FIXED_TESTER_NAME_HEIGHT	50
#define FIXED_REPORT_BUTTON_WIDTH	100
#define FIXED_REPORT_BUTTON_HEIGHT	50
#define FIXED_HEADER_BUTTON_WIDTH	100
#define FIXED_HEADER_BUTTON_HEIGHT	50
#define FIXED_HEADER_COMBO_WIDTH	200
#define FIXED_HEADER_COMBO_HEIGHT	30

#define LIGHT_THEME		0
#define DARK_THEME		1

#define RUSSIAN_LANG	0
#define ENGLISH_LANG	1

enum class TestWindowType {
	IN_TEST_MANUAL_STAND,
	OUT_TEST_MANUAL_STAND,
	FULL_TEST_MANUAL_STAND,
	IN_MANUAL_TEST_AUTO_STAND,
	OUT_MANUAL_TEST_AUTO_STAND,
	IN_AUTO_TEST_AUTO_STAND,
	OUT_AUTO_TEST_AUTO_STAND,
	FULL_TEST_AUTO_STAND
};

struct Size {
	int width;
	int height;
};

struct WindowState {
	bool appTheme;
	bool appLanguage;
	Size appSize;
};

class TestWindow : public QDialog
{
	Q_OBJECT

public:
	TestWindow(TestWindowType testType, WindowState *windowState, QWidget* parent = nullptr);
	~TestWindow();

	void setFileName(QString fileName);

private:
	Ui::TestWindowClass ui;

	QWidget* mainLayoutWidget;
	QWidget* headerLayoutWidget;
	QWidget* footerLayoutWidget;
	QWidget* usefulSpaceWidget;
	QHBoxLayout* headerMainHLayout;
	QHBoxLayout* tripleButtonsHLayout;
	QHBoxLayout* usefulSpaceHLayout;
	QHBoxLayout* footerMainHLayout;
	QHBoxLayout* reportHLayout;
	QVBoxLayout* mainVlayout;
	QVBoxLayout* switchThemeLanguageVLayout;
	QPushButton* switchThemeButton;
	QPushButton* switchLanguageButton;
	QPushButton* backButton;
	QPushButton* reportButton;
	QPushButton* fullTestManualStandSortButton;
	QPushButton* fullTestAutoStandSortButton;
	QPushButton* inManualTestAutoStandConnectButton;
	QPushButton* outManualTestAutoStandConnectButton;
	QPushButton* inAutoTestAutoStandConnectButton;
	QPushButton* outAutoTestAutoStandConnectButton;
	QPushButton* fullTestAutoStandConnectButton;
	QPushButton* inAutoTestAutoStandStartTestButton;
	QPushButton* outAutoTestAutoStandStartTestButton;
	QPushButton* fullTestAutoStandStartTestButton;
	QComboBox* inManualTestAutoStandTestTimeComboBox;
	QComboBox* outManualTestAutoStandTestTimeComboBox;
	QSpacerItem* tripleButtonsSpacer;
	QSpacerItem* reportSpacer;
	QSpacerItem* footerSpacer;
	QLabel* logoLabel;
	QLabel* fileNameLabel;
	QFrame* headerLine;
	QFrame* footerLine;
	QLineEdit* testerNameLineEdit;
	QTableWidget* mainTableWidget;
	QPixmap* logoLightPixmap;
	QPixmap* logoDarkPixmap;
	QPixmap* themeLightPixmap;
	QPixmap* themeDarkPixmap;
	QPixmap* languageLightPixmap;
	QPixmap* languageDarkPixmap;
	QPixmap* backButtonLightPixmap;
	QPixmap* backButtonDarkPixmap;

	WindowState *windowState;
	QString fileName;
	TestWindowType testType;

	void initUiMain();
	void initUiMainHeader();
	void initUiLogo();
	void initUiTripleButtons();
	void initUiTable();
	void initUiMainFooter();

	void initStyles();
	void initRecources();
	void initTexts();
	void initIcons();
	void initConnections();

	void initUiInTestManualStand();
	void initUiTableInTestManualStand();

	void initUiOutTestManualStand();
	void initUiTableOutTestManualStand();

	void initUiFullTestManualStand();
	void initUiTableFullTestManualStand();
	
	void initUiInManualTestAutoStand();
	void initUiTableInManualTestAutoStand();
	
	void initUiOutManualTestAutoStand();
	void initUiTableOutManualTestAutoStand();
	
	void initUiInAutoTestAutoStand();
	void initUiTableInAutoTestAutoStand();
	
	void initUiOutAutoTestAutoStand();
	void initUiTableOutAutoTestAutoStand();
	
	void initUiFullTestAutoStand();
	void initUiTableFullTestAutoStand();

	void switchTheme();
	void switchLanguage();

	void resizeEvent(QResizeEvent* event);

private slots:
	void on_backButton_clicked();
	void on_switchThemeButton_clicked();
	void on_switchLanguageButton_clicked();
	void on_reportButton_clicked();
};