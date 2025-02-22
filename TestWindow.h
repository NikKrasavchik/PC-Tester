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

#include "WindowFrame.h"
#include "can.h"
#include "MoreWindow.h"
#include "ReportWindow.h"

#include <QDebug>

#define COLUMN_CONNECTOR			0
#define COLUMN_PIN					1
#define COLUMN_NAME					2

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
#define COLUMN_MANUAL_CHECK_WIDTH	100

#define BUTTON_NOT_SET			NOT_SET
#define OFF_BUTTON_PRESSED		0
#define ON_BUTTON_PRESSED		1
#define LOAD0_BUTTON_PRESSED	0
#define LOAD25_BUTTON_PRESSED	64
#define LOAD50_BUTTON_PRESSED	128
#define LOAD75_BUTTON_PRESSED	192
#define LOAD100_BUTTON_PRESSED	255
#define HIGH_BUTTON_PRESSED		0
#define LOW_BUTTON_PRESSED	1
#define ZERO_BUTTON_PRESSED		2

#define SORT_TYPE_INDEX			0
#define SORT_TYPE_DIRECTION_OUT	1
#define SORT_TYPE_DIRECTION_IN	2

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
	TestTableRowProperties(TestWindow* testwindow);

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

	std::vector<Measureds*> measureds;
	std::vector<Thresholds> thresholds;

	void* buttons;
	QPushButton* moreButton;

	int stateDigital;
	int statePWM;
	int stateHLD;

	//TestWindow* testwindow;

	TestTableRowProperties* getThis() { return this; }; // Ужасное название. Не вникал, но если это родительский какой то, то назови что то по типу getParent
	void generateInteractionButtons(WindowType testType, int type);
	void switchButtonState(TestButtons testButton);
	void sendSignal();

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

	//void msgFromTwoThreadAfterTest_AutoTwothread(int pad, int pin, float voltage, float curent);

signals:
	void msgToTwoThreadStartTest_ManualTwoThread(int pad, int pin, int digValue, int pwmValue);
	void switchActiveTableButton(void* activeButton, void* inactiveButton);

	void selectCurrentCell(QString connector, QString pin);
};

class TestWindow : public QDialog
{
	Q_OBJECT

public:
	TestWindow(WindowType testType, std::vector<Cable> cables, NameTestingBlock testingBlock, QWidget* parent = nullptr);
	~TestWindow();

	void setFileName(QString fileName);
	void setParentFrame(WindowFrame* parentFrame);
	void ProcAutoTest(int pad, int pin);
	QString getFileName() { return fileName; }

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
	QPushButton* reportButton;
	QPushButton* fullTestSortButton;
	QPushButton* fullTestAutoStandSortButton;
	QPushButton* inTestManualStandConnectButton;
	QPushButton* outTestManualStandConnectButton;
	QPushButton* fullTestManualStandConnectButton;
	QPushButton* autoStandConnectButton;
	QPushButton* autoStandStartTestButton;
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

	int fullTestSortType; // false - сортировка по нумерации / true - сортировка по типу
	
	bool isFullTestEnabled; 

	QString fileName;
	WindowType testType;
	NameTestingBlock testingBlock;
	Can* can;
	std::vector<TestTableRowProperties*> cableRows;
	std::vector<QCheckBox*> manualChecks;
	std::vector<Measureds*> measuredValues;
	//QThread* th;
	Cable *nextCheckCable;

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
	void resetLanguageRowsTable(); // Надо как то менять язык в таблице
	void resetIconMoreButton(bool theme);
	void sortRows();
	void fillTestTimeComboBoxes();

	void generateCableRows(WindowType testType, std::vector<Cable> cables);
	void initTableRowButtons(int currentRowNum, QWidget* interactionButtonsWidget);
	void initTableAdditionalManualChecks(int currentRowNum, QWidget* manualChecksWidget);
	void initAutoCheckButton(int currentRowNum, QWidget* autoChechButtonWidget);
	void initMoreButton(int currentRowNum, QWidget* moreCellWidget);
	void setStatusTableButtons(bool statusButton);
	void resizeEvent(QResizeEvent* event);
	void rewriteCableRows(std::vector<TestTableRowProperties*>* cableRows, int sortType);

public slots:
	void on_backButton_clicked();
	void on_switchThemeButton_clicked();
	void on_switchLanguageButton_clicked();
	void on_reportButton_clicked();

	void on_AutoStandConnectButton_clicked();
	void on_inManualTestAutoStandTestTimeComboBox_changed(int ind);
	void on_outManualTestAutoStandTestTimeComboBox_changed(int ind);
	void on_AutoStandStartTestButton_clicked();

	void on_fullTestSortButton_clicked();

	void switchActiveTableButton(void* activeButton, void* inactiveButton);

public slots:
	void Slot_ChangedStatusStandConnect(bool statusConnect);
	void Slot_AfterTest(int connector, int pin, std::vector<Measureds*> measureds);
	void selectCurrentCell(QString conector, QString pin);

signals:

};