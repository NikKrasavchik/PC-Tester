#pragma once

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

#include "ui_TestWindow.h"
#include "WindowFrame.h"
#include "Components.h"
#include "can.h"
#include "twoThread.h"

#include <QDebug>

#define COLOUMN_CONNECTOR	0
#define COLOUMN_PIN			1
#define COLOUMN_NAME		2

#define COLOUMN_DIGITAL_HEIGHT	50
#define COLOUMN_PWM_HEIGHT		128
#define COLOUMN_VNH_HEIGHT		169

#define COLOUMN_CONNECTOR_WIDTH	60
#define COLOUMN_PIN_WIDTH		40
#define COLOUMN_DIRECTION_WIDTH	90
#define COLOUMN_TYPE_WIDTH		60
#define COLOUMN_CHECK_WIDTH		160
#define COLOUMN_AUTOCHECK_WIDTH	60
#define COLOUMN_STATUS_WIDTH	60
#define COLOUMN_STAND_WIDTH		60
#define COLOUMN_PC_WIDTH		60
#define COLOUMN_MORE_WIDTH		25

#define BUTTON_NOT_SET			-1
#define OFF_BUTTON_PRESSED		0
#define ON_BUTTON_PRESSED		1
#define LOAD0_BUTTON_PRESSED	0
#define LOAD25_BUTTON_PRESSED	64
#define LOAD50_BUTTON_PRESSED	128
#define LOAD75_BUTTON_PRESSED	192
#define LOAD100_BUTTON_PRESSED	255

#define SORT_TYPE_INDEX			0
#define SORT_TYPE_DIRECTION_OUT	1
#define SORT_TYPE_DIRECTION_IN	2

#define PRIMARY_CONNECTOR_SYMBOL	64

enum class TestButtons
{
	BUTTON_ON,
	BUTTON_OFF,
	BUTTON_LOAD_0,
	BUTTON_LOAD_25,
	BUTTON_LOAD_50,
	BUTTON_LOAD_75,
	BUTTON_LOAD_100
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

struct CheckButton
{
	QPushButton* checkButton;
};

class TestTableRowProperties : public QObject
{
	Q_OBJECT

public:
	int id;
	QString connector;
	QString pin;
	QString name;
	QString direction;
	QString type;

	void* buttons;
	QPushButton* moreButton;

	int stateDigital;
	int statePWM;

	void generateInteractionButtons(WindowType testType, int type);
	void sendSignal();

private:
	void switchButtonState(TestButtons testButton);

public slots:
	void on_onButton_clicked();
	void on_offButton_clicked();
	void on_load0Button_clicked();
	void on_load25Button_clicked();
	void on_load50Button_clicked();
	void on_load75Button_clicked();
	void on_load100Button_clicked();

	void msgFromTwoThreadAfterTest_AutoTwothread(int pad, int pin, float voltage, float curent);

signals:
	void msgToTwoThreadStartTest_ManualTwoThread(int pad, int pin, int digValue, int pwmValue);
	void switchActiveTableButton(void* activeButton, void* inactiveButton);
};

class TestWindow : public QDialog
{
	Q_OBJECT

public:
	TestWindow(WindowType testType, std::vector<Cable> cables, Can* can, QWidget* parent = nullptr);
	~TestWindow();

	void setFileName(QString fileName);
	void setParentFrame(WindowFrame* parentFrame);

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
	QPushButton* reportButton;
	QPushButton* fullTestSortButton;
	QPushButton* fullTestAutoStandSortButton;
	QPushButton* inTestManualStandConnectButton;
	QPushButton* outTestManualStandConnectButton;
	QPushButton* fullTestManualStandConnectButton;
	QPushButton* inManualTestAutoStandConnectButton;
	QPushButton* outManualTestAutoStandConnectButton;
	QPushButton* inAutoTestAutoStandConnectButton;
	QPushButton* outAutoTestAutoStandConnectButton;
	QPushButton* fullTestAutoStandConnectButton;
	QPushButton* AutoStandStartTestButton;
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
	QPixmap* moreButtonLightPixmap;
	QPixmap* moreButtonDarkPixmap;

	int fullTestSortType; // false - сортировка по нумерации / true - сортировка по типу
	
	bool isFullTestEnabled; 

	QString fileName;
	WindowType testType;
	Can* can;
	StandStatusFlags* statusFlags;
	std::vector<TestTableRowProperties*> cableRows;
	QThread* th;

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
	void initUiTableRowsInTestManualStand();

	void initUiOutTestManualStand();
	void initUiTableOutTestManualStand();
	void initUiTableHeaderOutTestManualStand();
	void initUiTableRowsOutTestManualStand();

	void initUiFullTestManualStand();
	void initUiTableFullTestManualStand();
	void resetTableHeaderFullTestManualStand();
	void resetTableRowsFullTestManualStand();

	void initUiInManualTestAutoStand();
	void initUiTableInManualTestAutoStand();
	void initUiTableHeaderInManualTestAutoStand();
	void initUiTableRowsInManualTestAutoStand();

	void initUiOutManualTestAutoStand();
	void initUiTableOutManualTestAutoStand();
	void initUiTableHeaderOutManualTestAutoStand();
	void initUiTableRowsOutManualTestAutoStand();

	void initUiInAutoTestAutoStand();
	void initUiTableInAutoTestAutoStand();
	void initUiTableHeaderInAutoTestAutoStand();
	void initUiTableRowsInAutoTestAutoStand();

	void initUiOutAutoTestAutoStand();
	void initUiTableOutAutoTestAutoStand();
	void initUiTableHeaderOutAutoTestAutoStand();
	void initUiTableRowsOutAutoTestAutoStand();

	void initUiFullTestAutoStand();
	void initUiTableFullTestAutoStand();
	void initUiTableHeaderFullTestAutoStand();
	void initUiTableRowsFullTestAutoStand();

	void resetTheme();
	void resetLanguage();
	void createItemManualTestAutoStandTestTimeComboBox(QComboBox* comboBox);
	void resetLanguageRowsTable(); // Надо как то менять язык в таблице
	void resetIconMoreButton(bool theme);
	void sortRows();
	void fillTestTimeComboBoxes();
	void generateCableRows(WindowType testType, std::vector<Cable> cables);
	void initTableRowButtons(int currentRowNum, QWidget* interactionButtonsWidget);
	void initAutoCheckButton(int currentRowNum, QWidget* autoChechButtonWidget);
	void initMoreButton(int currentRowNum, QWidget* moreCellWidget);
	void setStatusTableButtons(bool statusButton);
	void resizeEvent(QResizeEvent* event);

private slots:
	void on_backButton_clicked();
	void on_switchThemeButton_clicked();
	void on_switchLanguageButton_clicked();
	void on_reportButton_clicked();

	void on_inManualTestAutoStandConnectButton_clicked();
	void on_inManualTestAutoStandTestTimeComboBox_changed(int ind);
	void on_outManualTestAutoStandConnectButton_clicked();
	void on_outManualTestAutoStandTestTimeComboBox_changed(int ind);
	void on_inAutoTestAutoStandConnectButton_clicked();
	void on_AutoStandStartTestButton_clicked();
	void on_outAutoTestAutoStandConnectButton_clicked();
	void on_fullTestAutoStandConnectButton_clicked();
	void on_fullTestSortButton_clicked();

	void switchActiveTableButton(void* activeButton, void* inactiveButton);

	// manualtwoThread
	void msgToTestWindowStatusConnect_ManualTwoThread(bool statusConnect);
	void msgToTestWindowChangeValue_ManualTwoThread(int pad, int pin, int newValue);

	//void msgToTestWindowBeforeTest_AutoTwoThread(int pad, int pin);
	//void msgToTestWindowAfterTest_AutoTwoThread(int pad, int pin, float voltage, float current, int value);
};