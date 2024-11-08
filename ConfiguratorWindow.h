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
#include <fstream>

#include "ui_ConfiguratorWindow.h"
#include "Components.h"
#include "WindowFrame.h"

#define NO_OPTION				-2

#define STAND_NOT_SET			0
#define STAND_MANUAL			1
#define STAND_AUTO				2

#define TYPE_SET_DIR_SET_NUM	3
#define TYPE_NOTSET_DIR_SET_NUM	4
#define TYPE_SET_NUM			5
#define TYPE_NOT_SET_NUM		6

#define TYPE_NOT_SET_DIR_SET	-2
#define TYPE_NOT_SET			-1
#define TYPE_DIGITAL			0
#define TYPE_PWM				1
#define TYPE_VNH				2
#define TYPE_ANALOG				3
#define TYPE_HALL				4

#define DIRECTION_SET_NUM		2
#define DIRECTION_NOT_SET_NUM	3

#define DIRECTION_NOT_SET		-1
#define DIRECTION_OUT			0
#define DIRECTION_IN			1

#define CONNECTOR_NOT_SET		-1

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

#define TYPE_DIGITAL_RU			QString::fromLocal8Bit("Цифровой")
#define TYPE_DIGITAL_EN			QString("Digital")

#define TYPE_PWM_RU				QString::fromLocal8Bit("Шим")
#define TYPE_PWM_EN				QString("PWM")

#define TYPE_VNH_RU				QString("VNH")
#define TYPE_VNH_EN				QString("VNH")

#define TYPE_ANALOG_RU			QString::fromLocal8Bit("Аналоговый")
#define TYPE_ANALOG_EN			QString("Analog")

#define TYPE_HALL_RU			QString::fromLocal8Bit("Hall")
#define TYPE_HALL_EN			QString("Hall")

#define DIRECTION_NOT_SET_RU	QString::fromLocal8Bit("Направленность")
#define DIRECTION_NOT_SET_EN	QString("Direction")

#define DIRECTION_OUT_RU		QString::fromLocal8Bit("Выход")
#define DIRECTION_OUT_EN		QString("Out")

#define DIRECTION_IN_RU			QString::fromLocal8Bit("Вход")
#define DIRECTION_IN_EN			QString("In")

#define CFG_SPLIT				QString(",")
#define CFG_ENDING				QString("\n")

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

enum class ColoumnName
{
	CONNECTOR,
	PIN,
	DIRECTION,
	CAN_ID,
	BIT,
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
		ConnectorId connector;
		int direction;
		int type;
		QString name;
	};

	TableRowProperties(QObject* parent = nullptr);

	int id;
	QComboBox* connectorComboBox;
	int pin;
	QComboBox* directionComboBox;
	QString canId;
	int bit;
	QComboBox* typeComboBox;
	float minA;
	float maxA;
	float minV;
	float maxV;
	QString name;
	QPushButton* deleteButton;
	PresetSettings* presetSettings;

public slots:
	void on_direction_activated(int index);
	void on_type_activated(int index);
	void on_deleteButton_clicked();

signals:
	void resetRowPreset(TableRowProperties* currentRowProperties);
	void deleteRow(int index);
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
	void resetPresets();

	std::vector<std::vector<QString>> parseData();
	void deParseData();
	void updateTableData();

	bool verifyData(std::vector<std::vector<QString>> data);
	bool verifyRow(ColoumnName coloumnName, QTableWidgetItem* data);

	void resizeEvent(QResizeEvent* event);

private slots:
	void on_saveButton_clicked();
	void on_backButton_clicked();
	void on_switchThemeButton_clicked();
	void on_switchLanguageButton_clicked();
	void on_addRowButton_clicked();
	
	void resetRowPreset(TableRowProperties* currentRowProperties);
	void deleteRow(int index);
	//void on_selectStandTypeComboBox_activated(int index);
};

// Вопрос о нужности id в котором хранится индекс в tableRowProperties