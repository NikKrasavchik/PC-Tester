#pragma once

//#define DEBUG
//#define FOR_DEVELOPER


#define MIN_SCREEN_WIDTH    800
#define MIN_SCREEN_HEIGHT   600

#define BORDER_INDENT		25
#define TOOLBAR_SIZE		20

#define LIGHT_THEME			0
#define DARK_THEME			1

#define RUSSIAN_LANG		0
#define ENGLISH_LANG		1

#define KVASER				0
#define MARATHON			1

#define FREQUENCY_50K		50000
#define FREQUENCY_100K		100000
#define FREQUENCY_125K		125000
#define FREQUENCY_250K		250000
#define FREQUENCY_500K		500000
#define FREQUENCY_1000K		1000000

#define MANUAL_STAND		0
#define AUTO_STAND			1

#define NOT_SET				-1

#include <QString>
#include <vector>

struct Styles
{
	// FRAME
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

	// MAINWINDOW
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
	QString settingComboBox;
	QString settingSelectText;

	// TESTWINDOW
	//
	//
	QString testwindowButtonStyle;
	QString testwindowMoveButtonStyle;
	QString testwindowConnectButtonStyleConnect;
	QString testwindowConnectButtonStyleDisconnected;
	QString testwindowTestTimeComboBox;
	QString testwindowTableWidget;
	QString testwindowNameLineEdit;
	QString testwindowLableBlock;

	// TABLEBUTTON
	//
	//
	QString activeTableButton;
	QString inactiveTableButton;
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

struct StandStatusFlags
{
	bool StatusConnected; // false - стенд отключён			true - стенд подключён
	bool StatusTest;	  // false - тест не выполняеться	false - тест выполняется
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
	FULL_TEST_AUTO_STAND,
	MOREWINDOW,
	REPORTWINDOW
};

enum class TypeStand
{
	EMPTY = NOT_SET,
	MANUAL,
	AUTO
};

enum class TestBlockName
{
	EMPTY = NOT_SET,
	DM,
	BCM
};

enum class ConnectorId
{
	EMPTY,
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K
};

enum class TypeResetTableButtonsTheme
{
	STAND_DISCONNECTED,
	STAND_CONNECTED,
	STAND_TEST
};

#define PRIMARY_CONNECTOR_SYMBOL	64

#define TYPE_COUNT					7

#define TYPE_DIGITAL				0
#define TYPE_PWM					1
#define TYPE_VNH					2
#define TYPE_ANALOG					3
#define TYPE_HALL					4
#define TYPE_HLD					5

enum class TypeCable
{
	EMPTY = NOT_SET,
	DIG_IN,
	ANALOG_IN,
	HALL_IN,
	DIG_OUT,
	PWM_OUT,
	VNH_OUT,
	HLD_OUT
};

#define DIRECTION_OUT			0
#define DIRECTION_IN			1

extern ViewWindowState* viewWindowState;

namespace Warnings
{
	enum class MainWindow
	{
		TEST_ACCESS_FILE_SEL,
		TEST_ACCESS_FREQUENCY_SEL,
		TEST_ACCESS_ADAPTER_SEL,
		ADAPTERS_CHANGED,
		SIZE_CABLE_NUL,
		NOT_SELECTED_BLOCK,
		FILE_NOT_FOUND
	};

	enum class ReportWindow
	{
		XLSX_SAVE_SUCCESS,
		XLSX_SAVE_ERROR,
		TYPE_CHANGE_ERROR
	};

	enum class MoreWindow
	{
		OPEN_FILE_ERROR,
		FILE_NOT_FOUND
	};

	enum class TestWindow
	{
		OPEN_MORE_WINDOW
	};
}

const QString style = "Fusion";
const QString appstylePath = ":/recources/style/appstyles.qss";
const QString appLogoPath = ":/Dark/icons/App_Logo_White.png";

#define COLOR_RED				"#FF8686"
#define COLOR_GREEN				"#7CC770"
#define COLOR_WHITE				"#FFFFFF"
#define COLOR_GREY				"#979797"
#define COLOR_LIGHT_GREY		"#E8E8E8"
#define COLOR_DIRTY_LIGHT_GREY	"#95B3D7"
#define COLOR_LIGHT_BLUE		"#8DB4E2"
#define	COLOR_DIRTY_WHITE		"#DCE6F1"
#define COLOR_LIGHT_RED			"#FF8686"
#define COLOR_LIGHT_GREEN		"#7CC770"