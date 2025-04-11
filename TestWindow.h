#pragma once

#include "ui_TestWindow.h"

#include <QDialog>
#include <QFormLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QHeaderView>
#include <qmessagebox.h>
#include <qlist.h>

#include "WindowFrame.h"
#include "can.h"
#include "MoreWindow.h"
#include "ReportWindow.h"

#include <QDebug>

#define COLUMN_CONNECTOR			0
#define COLUMN_PIN					1
#define COLUMN_NAME					2

#define COLUMN_STANDART_HEIGHT		40
#define COLUMN_DIGITAL_HEIGHT		50
#define COLUMN_PWM_HEIGHT			128
#define COLUMN_VNH_HEIGHT			169
#define COLUMN_HLD_HEIGHT			128

#define COLUMN_CONNECTOR_WIDTH		70
#define COLUMN_PIN_WIDTH			40
#define COLUMN_DIRECTION_WIDTH		90
#define COLUMN_COMPONENT_WIDTH		80
#define COLUMN_TYPE_WIDTH			90
#define COLUMN_CHECK_WIDTH			160
#define COLUMN_AUTOCHECK_WIDTH		70
#define COLUMN_STATUS_WIDTH			60
#define COLUMN_STAND_WIDTH			60
#define COLUMN_PC_WIDTH				60
#define COLUMN_MORE_WIDTH			25
#define COLUMN_MANUAL_CHECK_WIDTH	75

#define MIN_ROW_HEIGHT				40

#define BUTTON_NOT_SET				NOT_SET
#define OFF_BUTTON_PRESSED			0
#define ON_BUTTON_PRESSED			1
#define LOAD0_BUTTON_PRESSED		0
#define LOAD25_BUTTON_PRESSED		64
#define LOAD50_BUTTON_PRESSED		128
#define LOAD75_BUTTON_PRESSED		192
#define LOAD100_BUTTON_PRESSED		255
#define HIGH_BUTTON_PRESSED			0
#define LOW_BUTTON_PRESSED			1
#define ZERO_BUTTON_PRESSED			2

#define FIXED_HEADER_BUTTON_WIDTH	130
#define FIXED_HEADER_BUTTON_HEIGHT	50
#define FIXED_HEADER_COMBO_WIDTH	200
#define FIXED_HEADER_COMBO_HEIGHT	40
#define FIXED_MORE_BUTTON_SIZE		25
#define FIXED_ACHECK_BUTTON_WIDTH	60
#define FIXED_ACHECK_BUTTON_HEIGHT	25
#define FIXED_CHECK_BUTTON_WIDTH	65
#define FIXED_CHECK_WBUTTON_WIDTH	148
#define	FIXED_CHECK_BUTTON_HEIGHT	30

enum class TestButtons
{
	BUTTON_ON,
	BUTTON_OFF,
	BUTTON_LOAD_0,
	BUTTON_LOAD_25,
	BUTTON_LOAD_50,
	BUTTON_LOAD_75,
	BUTTON_LOAD_100,
	BUTTON_HIGH,
	BUTTON_LOW,
	BUTTON_ZERO
};

struct DigitalButtons
{
	QPushButton* onButton;
	QPushButton* offButton;
};

struct PWMButtons
{
	QPushButton* load0Button;
	QPushButton* load25Button;
	QPushButton* load50Button;
	QPushButton* load75Button;
	QPushButton* load100Button;
};

struct VNHButtons
{
	QPushButton* onButton;
	QPushButton* offButton;
	QPushButton* load0Button;
	QPushButton* load25Button;
	QPushButton* load50Button;
	QPushButton* load75Button;
	QPushButton* load100Button;
};

struct HLDButtons
{
	QPushButton* highButton;
	QPushButton* lowButton;
	QPushButton* zeroButton;
};

struct CheckButton
{
	QPushButton* checkButton;
};

class MoreWindow;
class ReportWindow;
class TestTableRowProperties : public QObject
{
	Q_OBJECT

public:
	TestTableRowProperties();

	int id;
	int canId;
	int bit;
	QString connectorStr;
	ConnectorId connectorInt;
	QString pin;
	QString name;
	QString component;
	QString direction;
	QString typeStr;
	TypeCable typeInt;
	QString comment;
	bool manualChecked;

	std::vector<Measureds*> measureds;
	std::vector<Thresholds> thresholds;

	void* buttons;
	QPushButton* moreButton;
	QCheckBox* manualCheckBox = nullptr;

	int stateDigital;
	int statePWM;
	int stateHLD;

	// ------------------------------------
	// Name: generateInteractionButtons
	//			Производится генерация кнопок в соответствии с типом кабеля
	// Variables:
	//			WindowType testType: Содержит тип теста. Обрабатываемые аргументы:
	//								OUT_TEST_MANUAL_STAND		
	//								FULL_TEST_MANUAL_STAND
	//								OUT_MANUAL_TEST_AUTO_STAND
	//								IN_MANUAL_TEST_AUTO_STAND
	//								OUT_AUTO_TEST_AUTO_STAND
	//			int type: Содержит тип кабеля. Обрабатываемые аргументы:
	//						TYPE_DIGITAL
	//						TYPE_PWM
	//						TYPE_VNH
	//						TYPE_HLD
	// ------------------------------------
	void generateInteractionButtons(WindowType testType, int type);

	// ------------------------------------
	// Name: switchButtonState
	//			Производится переулючение стиля кнопок в соответствии с нажатием кнопок
	// Variables: 
	//			TestButtons testButton: содержит тип кнопки. Обрабатываемые аргументы:
	//				BUTTON_ON
	//				BUTTON_OFF
	//				BUTTON_LOAD_0
	//				BUTTON_LOAD_25
	//				BUTTON_LOAD_50
	//				BUTTON_LOAD_75
	//				BUTTON_LOAD_100
	//				BUTTON_HIGH
	//				BUTTON_LOW
	//				BUTTON_ZERO
	// ------------------------------------
	void switchButtonState(TestButtons testButton);

	// ------------------------------------
	// Name: sendSignal
	//			Отправляется сигнал на can
	// ------------------------------------
	void sendSignal();

	// ------------------------------------
	// Name: generateWarning
	//			Вызывается окно сообщения при неожиданных исходах
	// Variables: 
	//			Warnings::TestWindow warning: Идентификатор вызываемой ошибки
	//											OPEN_MORE_WINDOW
	// ------------------------------------
	void generateWarning(Warnings::TestWindow warning);

public slots:
	void on_onButton_clicked();
	void on_offButton_clicked();
	void on_load0Button_clicked();
	void on_load25Button_clicked();
	void on_load50Button_clicked();
	void on_load75Button_clicked();
	void on_load100Button_clicked();
	void on_high_clicked();
	void on_low_clicked();
	void on_zero_clicked();
	void on_checkButton_clicked();
	void on_moreButton_clicked();

signals:
	void msgToTwoThreadStartTest_ManualTwoThread(int pad, int pin, int digValue, int pwmValue);

	void selectCurrentCell(QString connector, QString pin);
};

class TestWindow : public QDialog
{
	Q_OBJECT

public:
	TestWindow(WindowType testType, std::vector<Cable> cables, TestBlockName testingBlock, QWidget* parent = nullptr);
	~TestWindow();

	// ------------------------------------
	// Name: setParentFrame
	//		Сохранение родительского элемента
	// Variables: 
	//			WindowFrame* parentFrame: Родительский элемент
	// ------------------------------------
	void setParentFrame(WindowFrame* parentFrame);

	// ------------------------------------
	// Name: ProcAutoTest
	//		Отправка сообщения на can
	// Variables: 
	//			int pad: Коннектор кабеля для отправки
	//			int pin: Пин кабеля для отправки
	// ------------------------------------
	void ProcAutoTest(int pad, int pin);

	StandStatusFlags* statusFlags;
private:

	Ui::TestWindowClass ui;
	WindowFrame* parentFrame;

	QWidget* mainLayoutWidget;
	QWidget* headerLayoutWidget;
	QWidget* footerLayoutWidget;
	QWidget* usefulSpaceWidget;
	QHBoxLayout* headerMainHLayout;
	QHBoxLayout* tripleButtonsHLayout;
	QHBoxLayout* usefulSpaceHLayout;
	QHBoxLayout* footerMainHLayout;
	QHBoxLayout* reportHLayout;
	QVBoxLayout* mainVLayout;
	QVBoxLayout* switchThemeLanguageVLayout;
	QPushButton* switchThemeButton;
	QPushButton* switchLanguageButton;
	QPushButton* backButton;
	QPushButton* sleepButton;
	QPushButton* reportButton;
	QPushButton* fullTestSortButton;
	QPushButton* inTestManualStandConnectButton;
	QPushButton* outTestManualStandConnectButton;
	QPushButton* fullTestManualStandConnectButton;
	QPushButton* autoStandConnectButton;
	QPushButton* autoStandStartTestButton;
	QComboBox* inManualTestAutoStandTestTimeComboBox;
	QComboBox* outManualTestAutoStandTestTimeComboBox;
	QSpacerItem* tripleButtonsSpacer;
	QSpacerItem* reportSpacer;
	QSpacerItem* reportSpacerTwo;
	QSpacerItem* footerSpacer;
	QLabel* logoLabel;
	QLabel* fileNameLabel;
	QFrame* headerLine;
	QFrame* footerLine;
	QTableWidget* mainTableWidget;
	QStringList* mainTableHeaderLabels;
	QPixmap* logoLightPixmap;
	QPixmap* logoDarkPixmap;
	QPixmap* themeLightPixmap;
	QPixmap* themeDarkPixmap;
	QPixmap* languageLightPixmap;
	QPixmap* languageDarkPixmap;
	QPixmap* backButtonLightPixmap;
	QPixmap* backButtonDarkPixmap;
	QPixmap* moreButtonLightPixmap;
	QPixmap* moreButtonDarkPixmap;
	QPixmap* clockwiseLightPixmap;
	QPixmap* clockwiseDarkPixmap;
	QPixmap* counterclockwiseLightPixmap;
	QPixmap* counterclockwiseDarkPixmap;
	QPixmap* noClockwiseLightPixmap;
	QPixmap* noClockwiseDarkPixmap;

	SortType fullTestSortType;

	WindowType testType;
	bool isFullTestEnabled;

	std::vector<TestTableRowProperties*> cableRows;
	QMap <int, int> offsetMap;
	Cable *nextCheckCable;
	QTimer* rotateTimer;
	int timerCounter;
	std::vector<std::pair<int, QLabel*>> hallLabels;

	void initUiMain();
	void initUiMainHeader();
	void initUiLogo();
	void initUiTripleButtons();
	void initUiTable();
	void initUiMainFooter();

	void initLightStyleSheets();
	void initDarkStyleSheets();
	void initStyles();
	void resetTableButtonsTheme(TypeResetTableButtonsTheme typeResetTheme, int pad, int pin);
	void initRecources();
	void initTexts();
	void initIcons();
	void initConnections();

	void initUiInTestManualStand();
	void initUiTableInTestManualStand();
	void initUiTableHeaderInTestManualStand();
	void resetLanguageInTestManualStand();
	void resetTableHeaderLanguageInTestManualStand();
	void resetTableTypeLanguageInTestManualStand();
	void initUiTableRowsInTestManualStand();

	void initUiOutTestManualStand();
	void initUiTableOutTestManualStand();
	void initUiTableHeaderOutTestManualStand();
	void resetLanguageOutTestManualStand();
	void resetTableHeaderLanguageOutTestManualStand();
	void resetTableTypeLanguageOutTestManualStand();
	void initUiTableRowsOutTestManualStand();

	void initUiFullTestManualStand();
	void initUiTableFullTestManualStand();
	void resetTableHeaderFullTestManualStand();
	void resetLanguageFullTestManualStand();
	void resetTableHeaderLanguageFullTestManualStand();
	void resetTableTypeLanguageFullTestManualStand();
	void resetTableDirectionLanguageFullTestManualStand();
	void resetTableRowsFullTestManualStand();

	void initUiInManualTestAutoStand();
	void initUiTableInManualTestAutoStand();
	void initUiTableHeaderInManualTestAutoStand();
	void resetLanguageInManualTestAutoStand();
	void resetTableHeaderLanguageInManualTestAutoStand();
	void resetTableTypeLanguageInManualTestAutoStand();
	void initUiTableRowsInManualTestAutoStand();

	void initUiOutManualTestAutoStand();
	void initUiTableOutManualTestAutoStand();
	void initUiTableHeaderOutManualTestAutoStand();
	void resetLanguageOutManualTestAutoStand();
	void resetTableHeaderLanguageOutManualTestAutoStand();
	void resetTableTypeLanguageOutManualTestAutoStand();
	void initUiTableRowsOutManualTestAutoStand();

	void initUiInAutoTestAutoStand();
	void initUiTableInAutoTestAutoStand();
	void initUiTableHeaderInAutoTestAutoStand();
	void resetLanguageInAutoTestAutoStand();
	void resetTableHeaderLanguageInAutoTestAutoStand();
	void resetTableTypeLanguageInAutoTestAutoStand();
	void initUiTableRowsInAutoTestAutoStand();

	void initUiOutAutoTestAutoStand();
	void initUiTableOutAutoTestAutoStand();
	void initUiTableHeaderOutAutoTestAutoStand();
	void resetLanguageOutAutoTestAutoStand();
	void resetTableHeaderLanguageOutAutoTestAutoStand();
	void resetTableTypeLanguageOutAutoTestAutoStand();
	void initUiTableRowsOutAutoTestAutoStand();

	void initUiFullTestAutoStand();
	void initUiTableFullTestAutoStand();
	void resetTableHeaderFullTestAutoStand();
	void resetLanguageFullTestAutoStand();
	void resetTableHeaderLanguageFullTestAutoStand();
	void resetTableTypeLanguageFullTestAutoStand();
	void resetTableDirectionLanguageFullTestAutoStand();
	void resetTableRowsFullTestAutoStand();

	void resetTheme();
	void resetLanguage();
	void createItemManualTestAutoStandTestTimeComboBox(QComboBox* comboBox);
	void resetIconMoreButton(bool theme);

	void generateCableRows(WindowType testType, std::vector<Cable> cables);
	void initTableRowButtons(int currentRowNum, QWidget* interactionButtonsWidget);
	void initTableAdditionalManualChecks(int currentRowNum, QWidget* manualChecksWidget);
	void initAutoCheckButton(int currentRowNum, QWidget* autoChechButtonWidget);
	void initMoreButton(int currentRowNum, QWidget* moreCellWidget);
	void setStatusTableButtons(bool statusButton);
	void resizeEvent(QResizeEvent* event);
	void rewriteCableRows();

public slots:
	void slot_backButton_clicked();
	void slot_switchThemeButton_clicked();
	void slot_switchLanguageButton_clicked();
	void slot_reportButton_clicked();
	void slot_sleepButton_clicked();

	void slot_autoStandConnectButton_clicked();
	void slot_inManualTestAutoStandTestTimeComboBox_changed(int ind);
	void slot_outManualTestAutoStandTestTimeComboBox_changed(int ind);
	void slot_autoStandStartTestButton_clicked();

	void slot_fullTestSortButton_clicked();

	void slot_mainTableWidget_cellClicked(int row, int column);

	void Slot_ChangedStatusStandConnect(bool statusConnect);
	void Slot_AfterTest(int connector, int pin, std::vector<Measureds*> measureds);
	void Slot_ChangedByte(int idCable, int newValue);
	void selectCurrentCell(QString conector, QString pin);

	void on_rotateTimer_timeout();
private:
	std::vector<std::pair<QString, TestTableRowProperties*>> sortComponents{  {QString("ARc5"),nullptr }, // Входы
															{QString("ARc7"),nullptr }, 
															{QString("ARc3"),nullptr }, 
															{QString("ARc2"),nullptr }, 
															{QString("ARc1"),nullptr }, 
															{QString("ARc4"),nullptr }, 
															{QString("Sw18"),nullptr }, 
															{QString("IHall4"),nullptr }, 
															{QString("Sw17"),nullptr }, 
															{QString("ARc8"),nullptr }, 
															{QString("Sw4"),nullptr }, 
															{QString("Sw24"),nullptr }, 
															{QString("Sw2"),nullptr }, 
															{QString("Sw8"),nullptr }, 
															{QString("ARc6"),nullptr }, 
															{QString("Sw16"),nullptr }, 
															{QString("Arv1"),nullptr }, 
															{QString("Sw23"),nullptr }, 
															{QString("Sw20"),nullptr }, 
															{QString("Sw3"),nullptr }, 
															{QString("Sw22"),nullptr }, 
															{QString("Sw12"),nullptr }, 
															{QString("Sw10"),nullptr }, 
															{QString("Sw19"),nullptr }, 
															{QString("Sw1"),nullptr }, 
															{QString("Sw15"),nullptr }, 
															{QString("Sw7"),nullptr }, 
															{QString("Sw14"),nullptr }, 
															{QString("Sw6"),nullptr }, 
															{QString("Sw13"),nullptr }, 
															{QString("Sw5"),nullptr }, 
															{QString("IHall1"),nullptr }, 
															{QString("IHall2"),nullptr }, 
															{QString("IHall3"),nullptr }, 
															{QString("IHall5"),nullptr }, 
															{QString("Enc1"),nullptr }, 
															{QString("Enc1"),nullptr }, 
															{QString("Enc2"),nullptr }, 
															{QString("Enc2"),nullptr }, 
															{QString("Enc3"),nullptr }, 
															{QString("Sw21"),nullptr }, 
															{QString("Sw11"),nullptr }, 
															{QString("Sw9"),nullptr }, 
															{QString("ARc9"),nullptr }, 
															{QString("Arv8"),nullptr }, 
															{QString("Arv3"),nullptr }, 
															{QString("Arv4"),nullptr }, 
															{QString("Arv7"),nullptr }, 
															{QString("Arv2"),nullptr }, 
															{QString("Arv5"),nullptr }, 
															{QString("Arv6"),nullptr }, 
															{QString("LOUT1"),nullptr }, 
															{QString("LOUT2"),nullptr }, 
															{QString("LOUT3"),nullptr }, 
															{QString("LOUT4"),nullptr }, 
															{QString("LOUT5"),nullptr }, 
															{QString("LOUT6"),nullptr }, 
															{QString("LOUT7"),nullptr }, 
															{QString("LOUT8"),nullptr }, 
															{QString("LOUT9"),nullptr }, 
															{QString("LOUT10"),nullptr }, 
															{QString("LOUT11"),nullptr }, 
															{QString("LOUT12"),nullptr }, 
															{QString("LOUT13"),nullptr }, 
															{QString("LOUT14"),nullptr }, 
															{QString("LOUT15"),nullptr }, 
															{QString("LOUT16"),nullptr }, 
															{QString("LOUT17"),nullptr }, 
															{QString("LOUT18"),nullptr }, 
															{QString("LOUT19"),nullptr }, 
															{QString("LOUT20"),nullptr }, 
															{QString("Hsd32"),nullptr }, 
															{QString("Hsd33"),nullptr }, 
															{QString("Hsd34"),nullptr }, 
															{QString("Hsd35"),nullptr }, 
															{QString("Hsd36"),nullptr }, 
															{QString("Hsd37"),nullptr }, 
															{QString("Hsd38"),nullptr }, 
															{QString("Hsd39"),nullptr }, 
															{QString("Hsd40"),nullptr }, 
															{QString("Hsd41"),nullptr }, 
															{QString("Hsd42"),nullptr }, 
															{QString("Hsd43"),nullptr }, 
															{QString("Hsd44"),nullptr }, 
															{QString("Hsd45"),nullptr }, 
															{QString("Hsd46"),nullptr }, 
															{QString("Hsd47"),nullptr }, 
															{QString("Hsd48"),nullptr }, 
															{QString("Hsd49"),nullptr }, 
															{QString("Hsd50"),nullptr }, 
															{QString("Hsd51"),nullptr }, 
															{QString("Hsd12"),nullptr }, 
															{QString("Hsd13"),nullptr }, 
															{QString("Hsd14"),nullptr }, 
															{QString("Hsd15"),nullptr }, 
															{QString("Hsd16"),nullptr }, 
															{QString("Hsd17"),nullptr }, 
															{QString("Hsd18"),nullptr }, 
															{QString("Hsd19"),nullptr }, 
															{QString("Hsd20"),nullptr }, 
															{QString("Hsd21"),nullptr }, 
															{QString("Hsd22"),nullptr }, 
															{QString("Hsd23"),nullptr }, 
															{QString("Hsd24"),nullptr }, 
															{QString("Hsd25"),nullptr }, 
															{QString("Hsd26"),nullptr }, 
															{QString("Hsd27"),nullptr }, 
															{QString("Hsd28"),nullptr }, 
															{QString("Hsd29"),nullptr }, 
															{QString("Hsd30"),nullptr }, 
															{QString("Hsd31"),nullptr }, 
															{QString("Br1"),nullptr }, 
															{QString("Br1"),nullptr }, 
															{QString("Br2"),nullptr }, 
															{QString("Br2"),nullptr }, 
															{QString("Br3"),nullptr }, 
															{QString("Br3"),nullptr }, 
															{QString("Br4"),nullptr }, 
															{QString("Br4"),nullptr }, 
															{QString("Br5"),nullptr }, 
															{QString("Br5"),nullptr }, 
															{QString("Br6"),nullptr }, 
															{QString("Br6"),nullptr }, 
															{QString("Br7"),nullptr }, 
															{QString("Br7"),nullptr }, 
															{QString("Br8"),nullptr }, 
															{QString("Br8"),nullptr }, 
															{QString("Br9"),nullptr }, 
															{QString("Br9"),nullptr }, 
															{QString("Br10"),nullptr }, 
															{QString("Br10"),nullptr }, 
															{QString("Hsd1"),nullptr }, 
															{QString("Hsd2"),nullptr }, 
															{QString("Hsd3"),nullptr }, 
															{QString("Hsd4"),nullptr }, 
															{QString("Hsd5"),nullptr }, 
															{QString("Hsd6"),nullptr }, 
															{QString("Hsd7"),nullptr }, 
															{QString("Hsd8"),nullptr }, 
															{QString("Hsd9"),nullptr }, 
															{QString("Hsd10"),nullptr }, 
															{QString("Hsd11"),nullptr }};

};