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

#define CEll_PAD_TABLE					0, 0
#define CELL_PIN_TABLE					0, 1
#define CELL_ID_TABLE					0, 2
#define CELL_TYPE_TABLE					0, 3
#define CELL_NAME_TABLE					0, 4
#define CELL_MEASURED_VALUE_TABLE		0, 5
#define CELL_MEASURED_VALUE_U_TABLE		2, 5
#define CELL_MEASURED_VALUE_I_TABLE		2, 6
#define CELL_PROGS_TABLE				0, 7
#define CELL_PROGS_U_TABLE				1, 7
#define CELL_PROGS_I_TABLE				1, 9
#define CELL_PROGS_U_MIN_TABLE			2, 7
#define CELL_PROGS_U_MAX_TABLE			2, 8
#define CELL_PROGS_I_MIN_TABLE			2, 9
#define CELL_PROGS_I_MAX_TABLE			2, 10
#define CEll_VALUE_PAD_TABLE					3, 0
#define CELL_VALUE_PIN_TABLE					3, 1
#define CELL_VALUE_ID_TABLE						3, 2
#define CELL_VALUE_TYPE_TABLE					3, 3
#define CELL_VALUE_NAME_TABLE					3, 4
#define CELL_VALUE_MEASURED_VALUE_U_TABLE		3, 5
#define CELL_VALUE_MEASURED_VALUE_I_TABLE		3, 6
#define CELL_VALUE_PROGS_U_MIN_TABLE			3, 7
#define CELL_VALUE_PROGS_U_MAX_TABLE			3, 8
#define CELL_VALUE_PROGS_I_MIN_TABLE			3, 9
#define CELL_VALUE_PROGS_I_MAX_TABLE			3, 10

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
	QTableWidget* mainTableWidget;
	QCheckBox* measuredCheckBox;
	QCheckBox* checkManualStandCheckBox;
	QCheckBox* checkPCAutoStandCheckBox;
	QCheckBox* checkStandAutoStandCheckBox;
	QSpacerItem* bottomSpacer;
	QPushButton* saveButton;
	
	std::vector<Cable> cables;
	Measured measured;
	float changedThresholds[4];

	ActiveColumn activeColumn;

public slots:
	void on_mainTableWidget_cellChanged(int row, int column);
	void on_saveChangesButton_clicked();
	void on_startTestButton_clicked();
};

