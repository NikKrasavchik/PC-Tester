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

#define ROW_PAD_TABLE					0
#define COLUMN_PAD_TABLE				0
#define CELL_PAD_TABLE							ROW_PAD_TABLE,COLUMN_PAD_TABLE
#define ROW_PIN_TABLE					0
#define COLUMN_PIN_TABLE				1
#define CELL_PIN_TABLE							ROW_TYPE_TABLE,COLUMN_PIN_TABLE
#define ROW_TYPE_TABLE					0
#define COLUMN_TYPE_TABLE				2
#define CELL_TYPE_TABLE							ROW_TYPE_TABLE,COLUMN_TYPE_TABLE
#define ROW_NAME_TABLE					0
#define COLUMN_NAME_TABLE				3
#define CELL_NAME_TABLE							ROW_NAME_TABLE,COLUMN_NAME_TABLE
#define ROW_SPACE_TABLE					0
#define COLUMN_SPACE_TABLE				4
#define CELL_SPACE_TABLE						ROW_SPACE_TABLE,COLUMN_SPACE_TABLE

#define ROW_MEASURED_VALUE_TABLE				0
#define COLUMN_MEASURED_VALUE_TABLE				4
#define ROW_MEASURED_VALUE_U_TABLE				2
#define COLUMN_MEASURED_VALUE_U_TABLE			4
#define ROW_MEASURED_VALUE_I_TABLE				2
#define COLUMN_MEASURED_VALUE_I_TABLE			5
#define ROW_TRESHHOLDERS_TABLE					0
#define COLUMN_TRESHHOLDERS_TABLE				6
#define ROW_TRESHHOLDERS_U_TABLE				1
#define COLUMN_TRESHHOLDERS_U_TABLE				6
#define ROW_TRESHHOLDERS_I_TABLE				1
#define COLUMN_TRESHHOLDERS_I_TABLE				8
#define ROW_TRESHHOLDERS_U_MIN_TABLE			2
#define COLUMN_TRESHHOLDERS_U_MIN_TABLE			6
#define ROW_TRESHHOLDERS_U_MAX_TABLE			2
#define COLUMN_TRESHHOLDERS_U_MAX_TABLE			7
#define ROW_TRESHHOLDERS_I_MIN_TABLE			2
#define COLUMN_TRESHHOLDERS_I_MIN_TABLE			8
#define ROW_TRESHHOLDERS_I_MAX_TABLE			2
#define COLUMN_TRESHHOLDERS_I_MAX_TABLE			9

#define CELL_COMMENT_TABLE						0,10


#define CEll_VALUE_PAD_TABLE					3,0
#define CELL_VALUE_PIN_TABLE					3,1
#define CELL_VALUE_TYPE_TABLE					3,2
#define CELL_VALUE_NAME_TABLE					3,3
#define CELL_VALUE_MEASURED_VALUE_U_TABLE		3,4
#define CELL_VALUE_MEASURED_VALUE_I_TABLE		3,5
#define CELL_VALUE_PROGS_U_MIN_TABLE			3,6
#define CELL_VALUE_PROGS_U_MAX_TABLE			3,7
#define CELL_VALUE_PROGS_I_MIN_TABLE			3,8
#define CELL_VALUE_PROGS_I_MAX_TABLE			3,9
#define CELL_VALUE_COMMENT_TABLE				3,10

class TestTableRowProperties;
class MoreWindow : public QDialog
{
	Q_OBJECT

public:
	MoreWindow(TestTableRowProperties* row, int startWidth, int startHeight);
	~MoreWindow();

protected:
	QTableWidget* mainTableWidget;
	QFont* font;

	int coutTrehholders;

	void initUiGeneratePartTableInOut();

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
	QTextEdit* commentTextEdit;
	
	TestTableRowProperties* row;
	float changedThresholds[4];
public slots:
	void on_mainTableWidget_cellChanged(int row, int column);
	void on_commentTextEdit_textChanged();
	void on_saveChangesButton_clicked();
	void on_startTestButton_clicked();
};

class MoreWindowOutDig final : public MoreWindow
{
public:
	MoreWindowOutDig(TestTableRowProperties* row) : MoreWindow(row, 1200, 250) { initUiGenerateTable(); };

private:
	void initUiGenerateTable();


};

class MoreWindowOutPwm final : public MoreWindow
{

public:
	MoreWindowOutPwm(TestTableRowProperties* row) : MoreWindow(row, 1200, 250) { initUiGenerateTable(); };

private:
	void initUiGenerateTable();


};

class MoreWindowOutVnh final : public MoreWindow
{
public:
	MoreWindowOutVnh(TestTableRowProperties* row) : MoreWindow(row, 1400, 250) { initUiGenerateTable(); };

private:
	void initUiGenerateTable();


};
