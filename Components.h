#pragma once

//#define DEBUG
//#define FOR_DEVELOPER
//#define DEBUG_OUTPUT


#define MIN_SCREEN_WIDTH    800
#define MIN_SCREEN_HEIGHT   600

#define FIXED_LOGO_WIDTH	220
#define FIXED_LOGO_HEIGHT	55

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
	QString testwindowManualCheckBox;

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

enum class TestBlockName
{
	EMPTY = NOT_SET,
	DTM,
	BCM
};

struct ViewWindowState
{
	bool appTheme;
	bool appLanguage;
	Size appSize;
	QString actualVersion;
	TestBlockName selectedBlock;
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

enum class SortType
{
	SortIndex,
	SortComponents,
	SortType
};

enum class DiagInformation
{
	Application_NAME,
	Calibration_NAME,
	Equipment_NAME,
	Manufacture_DATE,
	Hardware_NUMBER,
	Part_NUMBER,
	Serial_NUMBER
};

enum class TypeResetTableButtonsTheme
{
	STAND_DISCONNECTED,
	STAND_CONNECTED,
	STAND_TEST
};

#define PRIMARY_CONNECTOR_SYMBOL	64

#define TYPE_COUNT					9

#define TYPE_DIGITAL				0
#define TYPE_PWM					1
#define TYPE_VNH					2
#define TYPE_ANALOG					3
#define TYPE_HALL					4
#define TYPE_HLD					5	
#define TYPE_CAN					6	
#define TYPE_LIN					7	

enum class TypeCable
{
	EMPTY = NOT_SET,
	DIG_IN,
	ANALOG_IN,
	HALL_IN,
	DIG_OUT,
	PWM_OUT,
	VNH_OUT,
	HLD_OUT,
	CAN_OUT,
	LIN_OUT
};

#define DIRECTION_OUT			0
#define DIRECTION_IN			1

extern ViewWindowState* viewWindowState;

namespace Warnings
{
	enum class MainWindow
	{
		TEST_ACCESS_FILE_SEL,		// warning 0x001
		TEST_ACCESS_FREQUENCY_SEL,	// warning 0x002
		TEST_ACCESS_ADAPTER_SEL,	// warning 0x003
		ADAPTERS_CHANGED,			// warning 0x004
		SIZE_CABLE_NUL,				// warning 0x005
		NOT_SELECTED_BLOCK,			// warning 0x006
		FILE_NOT_FOUND				// warning 0x007
	};

	enum class ReportWindow
	{
		XLSX_SAVE_ERROR,			// warning 0x101
		TYPE_CHANGE_ERROR,			// warning 0x102
		INCORRECT_TEST_TYPE,		// warning 0x103
		EMPTY_INITIALS,				// warning 0x104
		EMPTY_SERIAL				// warning 0x105
	};

	enum class MoreWindow
	{
		FILE_NOT_FOUND,				// warning 0x201
		OPEN_FILE_ERROR				// warning 0x202
	};

	enum class TestWindow
	{
		OPEN_MORE_WINDOW,			// warning 0x301
		INCORRECT_TEST_TYPE			// warning 0x302
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