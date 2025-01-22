#pragma once

#include <QDialog>
#include <QFormLayout>
#include <qtablewidget.h>
#include <qpushbutton.h>
#include <QHeaderView>
#include <qmessagebox.h>
#include <QTextEdit>
#include <QString>
#include <QFileDialog>
#include <fstream>

#include "ui_MoreWindow.h"
#include "Components.h"
#include "TestWindow.h"

#define START_MOREWINDOW_WIDTH			900
#define START_MOREWINDOW_HEIGHT			250
#define START_MOREWINDOW_SIZE			START_MOREWINDOW_WIDTH,START_MOREWINDOW_HEIGHT

#define BUTTON_SIZE						100,40

#define PADDING_MAINWIDGET				10

// BEGIN | OUT
#define OUTALL_ROW_PAD_TABLE							0
#define OUTALL_COLUMN_PAD_TABLE							0
#define OUTALL_CELL_PAD_TABLE							OUTALL_ROW_PAD_TABLE,OUTALL_COLUMN_PAD_TABLE
#define OUTALL_ROW_PIN_TABLE							0
#define OUTALL_COLUMN_PIN_TABLE							1
#define OUTALL_CELL_PIN_TABLE							OUTALL_ROW_TYPE_TABLE,OUTALL_COLUMN_PIN_TABLE
#define OUTALL_ROW_TYPE_TABLE							0
#define OUTALL_COLUMN_TYPE_TABLE						2
#define OUTALL_CELL_TYPE_TABLE							OUTALL_ROW_TYPE_TABLE,OUTALL_COLUMN_TYPE_TABLE
#define OUTALL_ROW_NAME_TABLE							0
#define OUTALL_COLUMN_NAME_TABLE						3
#define OUTALL_CELL_NAME_TABLE							OUTALL_ROW_NAME_TABLE,OUTALL_COLUMN_NAME_TABLE
#define OUTALL_ROW_NAME_MEASURED_TABLE					0
#define OUTALL_COLUMN_NAME_MEASURED_TABLE				4
#define OUTALL_ROW_MEASURED_VALUE_TABLE					1
#define OUTALL_COLUMN_MEASURED_VALUE_TABLE				4
#define OUTALL_ROW_MEASURED_VALUE_U_TABLE				3
#define OUTALL_COLUMN_MEASURED_VALUE_U_TABLE			4
#define OUTALL_ROW_MEASURED_VALUE_I_TABLE				3
#define OUTALL_COLUMN_MEASURED_VALUE_I_TABLE			5
#define OUTALL_ROW_TRESHHOLDERS_TABLE					1
#define OUTALL_COLUMN_TRESHHOLDERS_TABLE				6
#define OUTALL_ROW_TRESHHOLDERS_U_TABLE					2
#define OUTALL_COLUMN_TRESHHOLDERS_U_TABLE				6
#define OUTALL_ROW_TRESHHOLDERS_I_TABLE					2
#define OUTALL_COLUMN_TRESHHOLDERS_I_TABLE				8
#define OUTALL_ROW_TRESHHOLDERS_U_MIN_TABLE				3
#define OUTALL_COLUMN_TRESHHOLDERS_U_MIN_TABLE			6
#define OUTALL_ROW_TRESHHOLDERS_U_MAX_TABLE				3
#define OUTALL_COLUMN_TRESHHOLDERS_U_MAX_TABLE			7
#define OUTALL_ROW_TRESHHOLDERS_I_MIN_TABLE				3
#define OUTALL_COLUMN_TRESHHOLDERS_I_MIN_TABLE			8
#define OUTALL_ROW_TRESHHOLDERS_I_MAX_TABLE				3
#define OUTALL_COLUMN_TRESHHOLDERS_I_MAX_TABLE			9
#define OUTALL_ROW_COMMENT_TABLE						0
#define OUTALL_COLUMN_COMMENT_TABLE						4
#define OUTALL_CELL_COMMENT_TABLE						OUTALL_ROW_COMMENT_TABLE,OUTALL_COLUMN_COMMENT_TABLE

#define OUTALL_CEll_VALUE_PAD_TABLE						4,0
#define OUTALL_CELL_VALUE_PIN_TABLE						4,1
#define OUTALL_CELL_VALUE_TYPE_TABLE					4,2
#define OUTALL_CELL_VALUE_NAME_TABLE					4,3
#define OUTALL_CELL_VALUE_MEASURED_VALUE_U_TABLE		3,4
#define OUTALL_CELL_VALUE_MEASURED_VALUE_I_TABLE		3,5
#define OUTALL_CELL_VALUE_PROGS_U_MIN_TABLE				3,6
#define OUTALL_CELL_VALUE_PROGS_U_MAX_TABLE				3,7
#define OUTALL_CELL_VALUE_PROGS_I_MIN_TABLE				3,8
#define OUTALL_CELL_VALUE_PROGS_I_MAX_TABLE				3,9
#define OUTALL_ROW_VALUE_COMMENT_TABLE					4
#define OUTALL_COLUMN_VALUE_COMMENT_TABLE				4
#define OUTALL_CELL_VALUE_COMMENT_TABLE					OUTALL_ROW_VALUE_COMMENT_TABLE,OUTALL_COLUMN_VALUE_COMMENT_TABLE

#define OUTALL_WIDTH_BLOCK_TRESHHOLDERS					6
#define OUTALL_OFFSET_BLOCK_TRESHHOLDERS(num)			num * OUTALL_WIDTH_BLOCK_TRESHHOLDERS
// END | OUT
// BEGIN | IN ANALOG
#define INANALOG_ROW_PAD_TABLE							0
#define INANALOG_COLUMN_PAD_TABLE						0
#define INANALOG_CELL_PAD_TABLE							INANALOG_ROW_PAD_TABLE,INANALOG_COLUMN_PAD_TABLE
#define INANALOG_ROW_PIN_TABLE							0
#define INANALOG_COLUMN_PIN_TABLE						1
#define INANALOG_CELL_PIN_TABLE							INANALOG_ROW_TYPE_TABLE,INANALOG_COLUMN_PIN_TABLE
#define INANALOG_ROW_TYPE_TABLE							0
#define INANALOG_COLUMN_TYPE_TABLE						2
#define INANALOG_CELL_TYPE_TABLE						INANALOG_ROW_TYPE_TABLE,INANALOG_COLUMN_TYPE_TABLE
#define INANALOG_ROW_NAME_TABLE							0
#define INANALOG_COLUMN_NAME_TABLE						3
#define INANALOG_CELL_NAME_TABLE						INANALOG_ROW_NAME_TABLE,INANALOG_COLUMN_NAME_TABLE
#define INANALOG_ROW_NAME_MEASURED_TABLE				0
#define INANALOG_COLUMN_NAME_MEASURED_TABLE				4
#define INANALOG_ROW_MEASURED_VALUE_TABLE				1
#define INANALOG_COLUMN_MEASURED_VALUE_TABLE			4
#define INANALOG_ROW_TRESHHOLDERS_TABLE					1
#define INANALOG_COLUMN_TRESHHOLDERS_TABLE				5
#define INANALOG_ROW_TRESHHOLDERS_MIN_TABLE				2
#define INANALOG_COLUMN_TRESHHOLDERS_MIN_TABLE			5
#define INANALOG_ROW_TRESHHOLDERS_MAX_TABLE				2
#define INANALOG_COLUMN_TRESHHOLDERS_MAX_TABLE			6

#define INANALOG_ROW_COMMENT_TABLE						0
#define INANALOG_COLUMN_COMMENT_TABLE					4
#define INANALOG_CELL_COMMENT_TABLE						INANALOG_ROW_COMMENT_TABLE,INANALOG_COLUMN_COMMENT_TABLE

#define INANALOG_CEll_VALUE_PAD_TABLE					3,0
#define INANALOG_CELL_VALUE_PIN_TABLE					3,1
#define INANALOG_CELL_VALUE_TYPE_TABLE					3,2
#define INANALOG_CELL_VALUE_NAME_TABLE					3,3
#define INANALOG_CELL_VALUE_MEASURED_VALUE_TABLE		3,4
#define INANALOG_CELL_VALUE_PROGS_MIN_TABLE				3,6
#define INANALOG_CELL_VALUE_PROGS_MAX_TABLE				3,7
#define INANALOG_ROW_VALUE_COMMENT_TABLE				3
#define INANALOG_COLUMN_VALUE_COMMENT_TABLE				4
#define INANALOG_CELL_VALUE_COMMENT_TABLE				INANALOG_ROW_VALUE_COMMENT_TABLE,INANALOG_COLUMN_VALUE_COMMENT_TABLE

#define INANALOG_WIDTH_BLOCK_TRESHHOLDERS				3
#define INANALOG_OFFSET_BLOCK_TRESHHOLDERS(num)			num * INANALOG_WIDTH_BLOCK_TRESHHOLDERS
// END | IN ANALOG


class TestTableRowProperties;
class MoreWindow : public QDialog
{
	Q_OBJECT

public:
	MoreWindow(TestTableRowProperties* row, int startWidth, int startHeight);
	~MoreWindow();

	enum TypeMoreWindow
	{
		MoreWindowOutDig,
		MoreWindowOutPwm,
		MoreWindowOutVnh,
		MoreWindowInDig,
		MoreWindowInAnalog,
		MoreWindowBuses,
		MoreWindowError
	};

protected:
	QTableWidget* mainTableWidget;
	QTextEdit* commentTextEdit;
	QFont* font;

	TestTableRowProperties* row;
	int coutTrehholders;

private:
	void initBaseUi();
	//void initUiBottomLayout();
	//void initUiGenerateTable();
	//void initUiSetValueTable();
	//void setValueProgs();

	void resizeEvent(QResizeEvent* event);

	//void resaveFile(QString fileName, Cable newCable);

	QWidget* mainWidget;
	QVBoxLayout* mainVLayout;
	QHBoxLayout* bottomHLayout;
	QSpacerItem* bottomSpacer;
	QPushButton* startTestButton;
	QPushButton* saveChangesButton;
	
	float changedThresholds[4];
public slots:
	void on_mainTableWidget_cellChanged(int row, int column);
	void on_commentTextEdit_textChanged();
	void on_saveChangesButton_clicked();
	void on_startTestButton_clicked();
};

class MoreWindowOut : public MoreWindow
{
public:
	MoreWindowOut(TestTableRowProperties* row, int startWidth, int startHeight) : MoreWindow(row, startWidth, startHeight) {};
protected:
	void initUiTableOut();

};

#define SIZE_MOREWINDOW_OUT_DIG		1200,300
class MoreWindowOutDig final : public MoreWindowOut
{
public:
	MoreWindowOutDig(TestTableRowProperties* row);
};

#define SIZE_MOREWINDOW_OUT_PWM		1200,300
class MoreWindowOutPwm final : public MoreWindowOut
{
public:
	MoreWindowOutPwm(TestTableRowProperties* row);
};

#define SIZE_MOREWINDOW_OUT_VNH		1500,300
class MoreWindowOutVnh final : public MoreWindowOut
{
public:
	MoreWindowOutVnh(TestTableRowProperties* row);
};



class MoreWindowIn : public MoreWindow
{
public:
	MoreWindowIn(TestTableRowProperties* row, int startWidth, int startHeight) : MoreWindow(row, startWidth, startHeight) {};
protected:
	void initUiTableIn(TypeMoreWindow typeWindow);

};

#define SIZE_MOREWINDOW_IN_DIG		1200,200
class MoreWindowInDig final : public MoreWindowIn
{
public:
	MoreWindowInDig(TestTableRowProperties* row);
};

#define SIZE_MOREWINDOW_IN_ANALOG		1200,250
class MoreWindowInAnalog final : public MoreWindowIn
{

public:
	MoreWindowInAnalog(TestTableRowProperties* row);


};
