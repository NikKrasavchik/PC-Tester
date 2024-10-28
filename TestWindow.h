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

class TestWindow : public QDialog
{
	Q_OBJECT

public:
	TestWindow(TestWindowType testType, QWidget* parent = nullptr);
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
	void sortRows();
	void fillTestTimeComboBoxes();

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
};