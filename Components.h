#pragma once

#define DEBUG

#define MIN_SCREEN_WIDTH    800
#define MIN_SCREEN_HEIGHT   600

#define BORDER_INDENT   25
#define TOOLBAR_SIZE    20

#define LIGHT_THEME		0
#define DARK_THEME		1

#define RUSSIAN_LANG	0
#define ENGLISH_LANG	1

#define KVASER		0
#define MARATHON	1

#define MANUAL_STAND	0
#define AUTO_STAND		1

#include <QString>

struct Styles
{
	// Frame
	//
	// header
	QString headerDefaultStyle;
	QString headerCollapseStyle;
	QString headerMaximizeStyle;
	QString bodyStyle;

	// button
	QString buttonStyle;

	// text title
	QString titleStyle;

	// Mainwindow
	// Header
	//selectStand
	QString standButtons;
	QString alwaysActiveStandButton;
	QString roundSlider;
	QString bgSlider;

	//themeLanguage
	QString themeLangButton;

	// Main
	//
	QString mainButton;
	QString mainButtonNoActive;
	QString mainSubstrateButtons;
	QString mainText;

	// Setting
	//
	QString settingButton;
	QString settingComboBox;
	QString settingSelectText;

	// TestWindow
	//
	//

	//test window
	QString testwindowButtonStyle;
	QString testwindowMoveButtonStyle;
	QString testwindowConnectButtonStyleConnect;
	QString testwindowConnectButtonStyleDisconnected;
	QString testwindowTestTimeComboBox;
	QString testwindowTableWidget;
	QString testwindowNameLineEdit;
};

extern Styles darkStyles;
extern Styles lightStyles;

struct Size
{
	int width;
	int height;
};

struct ViewWindowState
{
	bool appTheme;
	bool appLanguage;
	Size appSize;
};

enum class WindowType
{
	MAINWINDOW,
	CONFIGURATOR,
	IN_TEST_MANUAL_STAND,
	OUT_TEST_MANUAL_STAND,
	FULL_TEST_MANUAL_STAND,
	IN_MANUAL_TEST_AUTO_STAND,
	OUT_MANUAL_TEST_AUTO_STAND,
	IN_AUTO_TEST_AUTO_STAND,
	OUT_AUTO_TEST_AUTO_STAND,
	FULL_TEST_AUTO_STAND
};

enum class ConnectorId
{
	NOT_SET,
	A,
	B,
	C,
	D,
	E,
	F
};

#define TYPE_NOT_SET			-1
#define TYPE_DIGITAL			0
#define TYPE_PWM				1
#define TYPE_VNH				2
#define TYPE_ANALOG				3
#define TYPE_HALL				4

#define DIRECTION_NOT_SET		-1
#define DIRECTION_OUT			0
#define DIRECTION_IN			1

struct Cable
{
	ConnectorId connector;
	int pin;
	int direction;
	int type;
	unsigned char canId;
	char bit;
	double minCurrent;
	double maxCurrent;
	double minVoltage;
	double maxVoltage;
	QString name;
};

extern ViewWindowState* viewWindowState;

#define FIXED_HEADER_HEIGHT			80
#define FIXED_FOOTER_HEIGHT			80
#define FIXED_LOGO_WIDTH			220
#define FIXED_LOGO_HEIGHT			55
#define THEME_LANG_BUTTON_SIZE		30
#define BACK_BUTTON_SIZE			50
#define FIXED_FILE_NAME_WIDTH		150
#define FIXED_FILE_NAME_HEIGHT		50
#define FIXED_TESTER_NAME_WIDTH		300
#define FIXED_TESTER_NAME_HEIGHT	50
#define FIXED_REPORT_BUTTON_WIDTH	100
#define FIXED_REPORT_BUTTON_HEIGHT	50
#define FIXED_HEADER_BUTTON_WIDTH	120
#define FIXED_HEADER_BUTTON_HEIGHT	50
#define FIXED_HEADER_COMBO_WIDTH	200
#define FIXED_HEADER_COMBO_HEIGHT	40
#define FIXED_STAND_COMBO_WIDTH		150
#define FIXED_STAND_COMBO_HEIGHT	50
#define FIXED_FOOTER_BUTTON_WIDTH	100
#define FIXED_FOOTER_BUTTON_HEIGHT	50