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
#include "WindowFrame.h"
#include "Components.h"
#include "can.h"
#include "twoThread.h"

#include <QDebug>

#define COLOUMN_CONNECTOR	0
#define COLOUMN_PIN			1
#define COLOUMN_NAME		2

#define PRIMARY_CONNECTOR_SYMBOL	64

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


class TestTableRowProperties : public QObject
{
	Q_OBJECT

public:
	QString connector;
	QString pin;
	QString name;
	QString direction;
	QString type;
	void* buttons;
	QPushButton* moreButton;

	void generateInteractionButtons(int type);

public slots:
	void on_onButton_clicked();
	void on_offButton_clicked();
	void on_load0Button_clicked();
	void on_load25Button_clicked();
	void on_load50Button_clicked();
	void on_load75Button_clicked();
	void on_load100Button_clicked();
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

	bool fullTestManualStandTypeSort; // false - сортировка по нумерации / true - сортировка по типу
	bool fullTestAutoStandTypeSort; // false - сортировка по нумерации / true - сортировка по типу

	bool standConected; // ПОМЕНЯТЬ  Переменная хранящая в информацию о том подключен ли стенд к пк. перезаписываться она будет по сигналу со второго потока. Сейчас для отладки меняеться при нажатии на смену стиля;

	QString fileName;
	WindowType testType;
	Can* can;
	std::vector<TestTableRowProperties*> cableRows;
	autoStandTwoThread* th;

	void initUiMain();
	void initUiMainHeader();
	void initUiLogo();
	void initUiTripleButtons();
	void initUiTable();
	void initUiMainFooter();

	void initLightStyleSheets();
	void initDarkStyleSheets();
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

	void resetTheme();
	void resetLanguage();
	void createItemManualTestAutoStandTestTimeComboBox(QComboBox* comboBox);
	void resetLanguageRowsTable(); // Надо как то менять язык в таблице
	void sortRows();
	void fillTestTimeComboBoxes();
	void generateCableRows(WindowType testType, std::vector<Cable> cables);
	void generateRowsInteractionButtons(TestTableRowProperties* rowTable);
	void initTableRowButtons(int currentRowNum, QWidget* interactionButtonsWidget, QWidget* moreCellWidget);

	void resizeEvent(QResizeEvent* event);

private slots:
	void on_backButton_clicked();
	void on_switchThemeButton_clicked();
	void on_switchLanguageButton_clicked();
	void on_reportButton_clicked();

	void on_fullTestManualStandSortButton_clicked();
	void on_inManualTestAutoStandConnectButton_clicked();
	void on_inManualTestAutoStandTestTimeComboBox_changed(int ind);
	void on_outManualTestAutoStandConnectButton_clicked();
	void on_outManualTestAutoStandTestTimeComboBox_changed(int ind);
	void on_inAutoTestAutoStandConnectButton_clicked();
	void on_inAutoTestAutoStandStartTestButton_clicked();
	void on_outAutoTestAutoStandConnectButton_clicked();
	void on_outAutoTestAutoStandStartTestButton_clicked();
	void on_fullTestAutoStandConnectButton_clicked();
	void on_fullTestAutoStandStartTestButton_clicked();
	void on_fullTestAutoStandSortButton_clicked();

	// twoThread
	void msgToTestWindowStatusConnect(bool statusConnect);
};