#pragma once

#include <QDialog>
#include <QFormLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QDebug>
#include <vector>

#include "ui_ConfiguratorWindow.h"
#include "Components.h"
#include "WindowFrame.h"

#define NO_OPTION			-2

#define STAND_NOT_SET		0
#define STAND_MANUAL		1
#define STAND_AUTO			2

#define TYPE_NOT_SET		0
#define TYPE_MANUAL			1
#define TYPE_AUTO			2

#define DIRECTION_NOT_SET	0
#define DIRECTION_OUT		1
#define DIRECTION_IN		2

#define CONNECTOR_NOT_SET	-1

#define NO_OPTION_RU			QString::fromLocal8Bit("Нет доступных вариантов")
#define NO_OPTION_EN			QString("No options available")

#define STAND_NOT_SET_RU		QString::fromLocal8Bit("Тип стенда")
#define STAND_NOT_SET_EN		QString("Stand type")

#define STAND_MANUAL_RU			QString::fromLocal8Bit("Ручной")
#define STAND_MANUAL_EN			QString("Manual")

#define STAND_AUTO_RU			QString::fromLocal8Bit("Автоматический")
#define STAND_AUTO_EN			QString("Auto")

#define TYPE_NOT_SET_RU			QString::fromLocal8Bit("Тип теста")
#define TYPE_NOT_SET_EN			QString("Test type")

#define TYPE_MANUAL_RU			QString::fromLocal8Bit("Ручной")
#define TYPE_MANUAL_EN			QString("Manual")

#define TYPE_AUTO_RU			QString::fromLocal8Bit("Автоматический")
#define TYPE_AUTO_EN			QString("Auto")

#define DIRECTION_NOT_SET_RU	QString::fromLocal8Bit("Направленность")
#define DIRECTION_NOT_SET_EN	QString("Direction")

#define DIRECTION_OUT_RU		QString::fromLocal8Bit("Выход")
#define DIRECTION_OUT_EN		QString("Out")

#define DIRECTION_IN_RU			QString::fromLocal8Bit("Вход")
#define DIRECTION_IN_EN			QString("In")

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

enum class RowName
{
	CONNECTOR,
	PIN,
	DIRECTION,
	CAN_ID,
	BYTE,
	TYPE,
	MIN_A,
	MAX_A,
	MIN_V,
	MAX_V,
	NAME,
	DELETE
};

class TableRowProperties : public QObject
{
	Q_OBJECT

public:
	struct PresetSettings
	{
		int direction;
		int type;
	};

	TableRowProperties(QObject* parent = nullptr);

	int id;
	ConnectorId connectorId;
	int pin;
	QComboBox* direction;
	QComboBox* type;
	QString name;
	PresetSettings* presetSettings;

public slots:
	void on_direction_activated(int index);
	void on_type_activated(int index);
};

class ConfiguratorWindow : public QDialog
{
	Q_OBJECT

public:
	ConfiguratorWindow(QWidget *parent = nullptr);
	~ConfiguratorWindow();

	void setParentFrame(WindowFrame* parentFrame);

private:
	Ui::ConfiguratorWindowClass ui;

	QWidget* mainLayoutWidget;
	QWidget* headerLayoutWidget;
	QWidget* usefulSpaceWidget;
	QWidget* footerLayoutWidget;
	QHBoxLayout* headerMainHLayout;
	QHBoxLayout* usefulSpaceHLayout;
	QHBoxLayout* tripleButtonsHLayout;
	QHBoxLayout* footerMainHLayout;
	QHBoxLayout* footerCombosHLayout;
	QVBoxLayout* switchThemeLanguageVLayout;
	QVBoxLayout* mainVLayout;
	QPushButton* clearPresetButton;
	QPushButton* saveButton;
	QPushButton* addRowButton;
	QPushButton* backButton;
	QPushButton* switchThemeButton;
	QPushButton* switchLanguageButton;
	QSpacerItem* tripleButtonsSpacer;
	QSpacerItem* footerSpacer;
	QTableWidget* mainTableWidget;
	QLineEdit* fileNameLineEdit;
	QComboBox* selectStandTypeComboBox;
	QComboBox* selectTestTypeComboBox;
	QComboBox* selectDirectionComboBox;
	QLabel* logoLabel;
	QFrame* headerLine;
	QFrame* footerLine;
	QPixmap* logoLightPixmap;
	QPixmap* logoDarkPixmap;
	QPixmap* themeLightPixmap;
	QPixmap* themeDarkPixmap;
	QPixmap* languageLightPixmap;
	QPixmap* languageDarkPixmap;
	QPixmap* backButtonLightPixmap;
	QPixmap* backButtonDarkPixmap;

	WindowFrame* parentFrame;

	bool isAllInit;
	bool canReselectStandType;

	std::vector<TableRowProperties*> tableRowPropertiesVector;

	void initUi();
	void initUiHeader();
	void initUiLogo();
	void initUiUsefulSpace();
	void initUiTripleButtons();
	void initUiTable();
	void initUiFooter();

	void initRecources();
	void initConnections();

	void resetTheme();
	void resetLanguage();
	void resetPreset();

	void resizeEvent(QResizeEvent* event);

private slots:
	void on_saveButton_clicked();
	void on_backButton_clicked();
	void on_switchThemeButton_clicked();
	void on_switchLanguageButton_clicked();
	void on_addRowButton_clicked();

	//void on_selectStandTypeComboBox_activated(int index);
	//void on_selectTestTypeComboBox_activated(int index);
	//void on_selectDirectionComboBox_activated(int index);
};

class TableQComboBox : QComboBox
{
	Q_OBJECT

public:
	TableQComboBox(QWidget* parent = nullptr);
	~TableQComboBox();

private:

};

class DeleteRowTableQPushButton : QPushButton
{
	Q_OBJECT

public:
	DeleteRowTableQPushButton(QWidget* parent = nullptr);
	~DeleteRowTableQPushButton();

private:

};