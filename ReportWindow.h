#pragma once

#include <QDialog>
#include <QFormLayout>
#include <qtablewidget.h>
#include <qpushbutton.h>
#include <QHeaderView>
#include <qmessagebox.h>
#include <QString>
#include <QCheckBox>

#include "ui_ReportWindow.h"
#include "Components.h"
#include "TestWindow.h"

#define START_ReportWindow_WIDTH			1000
#define START_ReportWindow_HEIGHT			250
#define START_ReportWindow_SIZE			START_ReportWindow_WIDTH, START_ReportWindow_HEIGHT

#define BUTTON_WIDTH					100
#define BUTTON_HEIGHT					40
#define BUTTON_SIZE						BUTTON_WIDTH, BUTTON_HEIGHT

#define LEFT_PADDING_MAIN_WIDGET		10
#define UP_PADDING_MAIN_WIDGET			10
#define COLUMN_SETTER_WIDTH				150

#define SPAN_COUNT_1	1
#define SPAN_COUNT_2	2
#define SPAN_COUNT_3	3
#define SPAN_COUNT_4	4

#define ROW_0			0
#define ROW_1			1
#define ROW_2			2
#define ROW_3			3

#define COLUMN_ID					0
#define COLUMN_PAD					1
#define COLUMN_PIN					2
#define COLUMN_TYPE					3
#define COLUMN_NAME					4
#define COLUMN_TRESHHOLDERS			5
#define COLUMN_TRESHHOLDERS_U		5
#define COLUMN_TRESHHOLDERS_U_MIN	5
#define COLUMN_TRESHHOLDERS_U_MAX	6
#define COLUMN_TRESHHOLDERS_I		7
#define COLUMN_TRESHHOLDERS_I_MIN	7
#define COLUMN_TRESHHOLDERS_I_MAX	8

#define CEll_PAD_TABLE							ROW_0, COLUMN_ID
#define CELL_PIN_TABLE							ROW_0, COLUMN_PAD
#define CELL_ID_TABLE							ROW_0, COLUMN_PIN
#define CELL_TYPE_TABLE							ROW_0, COLUMN_TYPE
#define CELL_NAME_TABLE							ROW_0, COLUMN_NAME
#define CELL_TRESHHOLDERS_TABLE					ROW_0, COLUMN_TRESHHOLDERS
#define CELL_TRESHHOLDERS_U_TABLE				ROW_1, COLUMN_TRESHHOLDERS_U
#define CELL_TRESHHOLDERS_I_TABLE				ROW_1, COLUMN_TRESHHOLDERS_I
#define CELL_TRESHHOLDERS_U_MIN_TABLE			ROW_2, COLUMN_TRESHHOLDERS_U_MIN
#define CELL_TRESHHOLDERS_U_MAX_TABLE			ROW_2, COLUMN_TRESHHOLDERS_U_MAX
#define CELL_TRESHHOLDERS_I_MIN_TABLE			ROW_2, COLUMN_TRESHHOLDERS_I_MIN
#define CELL_TRESHHOLDERS_I_MAX_TABLE			ROW_2, COLUMN_TRESHHOLDERS_I_MAX

#define CLEAR_COLUMN_COUNT		9

#define SET_INVISIBLE			0
#define SET_VISIBLE				1

#define MEASURED_COLUMN			0
#define CHECK_MANUAL_STAND		1
#define CHECK_PC_AUTO_STAND		2
#define CHECK_STAND_AUTO_STAND	3

struct ActiveColumn
{
	bool measured;
	bool checkManualStand;
	bool checkPCAutoStand;
	bool checkStandAutoStand;
};

class ReportWindow : public QDialog
{
	Q_OBJECT

public:
	ReportWindow(std::vector<Cable> cables, WindowType testType);
	~ReportWindow();

private:
	void initUi();
	void initUiTable();
	void initUiGenerateTable();
	void initUiSetValueTable();
	void initUiColumnSetters();
	void initUiFooter();
	void resetUiAdditionalColumns();
	void resetUiFillColumns();
	
	void fillColumnsSetters();

	WindowType testType;


	void resizeEvent(QResizeEvent* event);

	QWidget* mainWidget;
	QWidget* columnSetterWidget;
	QWidget* footerWidget;
	QVBoxLayout* mainVLayout;
	QVBoxLayout* columnSetterVLayout;
	QHBoxLayout* bodyHLayout;
	QHBoxLayout* bottomHLayout;
	QHBoxLayout* measuredHLayout;
	QHBoxLayout* checkManualStandHLayout;
	QHBoxLayout* checkPCAutoStandHLayout;
	QHBoxLayout* checkStandAutoStandHLayout;
	QTableWidget* mainTableWidget;
	QCheckBox* measuredCheckBox;
	QCheckBox* checkManualStandCheckBox;
	QCheckBox* checkPCAutoStandCheckBox;
	QCheckBox* checkStandAutoStandCheckBox;
	QSpacerItem* bottomSpacer;
	QLabel* measuredLabel;
	QLabel* checkManualStandLabel;
	QLabel* checkPCAutoStandLabel;
	QLabel* checkStandAutoStandLabel;
	QPushButton* saveButton;
	
	std::vector<Cable> cables;
	Measured measured;
	float changedThresholds[4];

	ActiveColumn activeColumn;

public slots:
	void on_saveChangesButton_clicked();
	void on_startTestButton_clicked();

	void on_measuredCheckBox_stateChanged(int state);
	void on_checkManualStandCheckBox_stateChanged(int state);
	void on_checkPCAutoStandCheckBox_stateChanged(int state);
	void on_checkStandAutoStandCheckBox_stateChanged(int state);
};

