#pragma once

#include <QDialog>
#include <QFormLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QLineEdit>

#include "ui_TestWindow.h"

#define MIN_SCREEN_WIDTH    800
#define MIN_SCREEN_HEIGHT   600

#define BORDER_INDENT   25
#define TOOLBAR_SIZE    20

#define FIXED_HEADER_HEIGHT		80
#define FIXED_FOOTER_HEIGHT		80
#define FIXED_LOGO_WIDTH		220
#define FIXED_LOGO_HEIGHT		55
#define THEME_LANG_BUTTON_SIZE	30
#define BACK_BUTTON_SIZE		50
#define FIXED_FILE_NAME_WIDTH		200
#define FIXED_FILE_NAME_HEIGHT		50
#define FIXED_TESTER_NAME_WIDTH		300
#define FIXED_TESTER_NAME_HEIGHT	50
#define FIXED_REPORT_BUTTON_WIDTH	100
#define FIXED_REPORT_BUTTON_HEIGHT	50

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

class TestWindow : public QDialog
{
	Q_OBJECT

public:
	TestWindow(TestWindowType testType, QWidget* parent = nullptr);
	~TestWindow();

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
	QSpacerItem* tripleButtonsSpacer;
	QSpacerItem* reportSpacer;
	QSpacerItem* footerSpacer;
	QLabel* logoLabel;
	QLabel* fileNameLabel;
	QFrame* headerLine;
	QFrame* footerLine;
	QPixmap* logoLightPixmap;
	QLineEdit* testerNameLineEdit;
	QTableWidget* mainTableWidget;

	void initUiMain();
	void initUiMainHeader();
	void initUiLogo();
	void initUiTripleButtons();
	void initUiTable();
	void initUiMainFooter();

	void initUiInTestManualStand();
	void initUiOutTestManualStand();
	void initUiFullTestManualStand();
	void initUiInManualTestAutoStand();
	void initUiOutManualTestAutoStand();
	void initUiInAutoTestAutoStand();
	void initUiOutAutoTestAutoStand();
	void initUiFullTestAutoStand();

	void resizeEvent(QResizeEvent* event);
};