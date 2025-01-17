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

#define PADDING_MAIN_WIDGET				10

#define CELL_PAD_TABLE							0,0
#define CELL_PIN_TABLE							0,1
#define CELL_TYPE_TABLE							0,2
#define CELL_NAME_TABLE							0,3
#define CELL_MEASURED_VALUE_TABLE				0,4
#define CELL_MEASURED_VALUE_U_TABLE				2,4
#define CELL_MEASURED_VALUE_I_TABLE				2,5
#define CELL_TRESHHOLDERS_TABLE					0,6
#define CELL_TRESHHOLDERS_U_TABLE				1,6
#define CELL_TRESHHOLDERS_I_TABLE				1,8
#define CELL_TRESHHOLDERS_U_MIN_TABLE			2,6
#define CELL_TRESHHOLDERS_U_MAX_TABLE			2,7
#define CELL_TRESHHOLDERS_I_MIN_TABLE			2,8
#define CELL_TRESHHOLDERS_I_MAX_TABLE			2,9
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
	MoreWindow(TestTableRowProperties* row);
	~MoreWindow();

private:
	void initUi();
	void initUiGenerateTable();
	void initUiSetValueTable();
	void initUiBottomLayout();

	void resizeEvent(QResizeEvent* event);

	void resaveFile(QString fileName, Cable newCable);

	QWidget* mainWidget;
	QVBoxLayout* mainVLayout;
	QTableWidget* mainTableWidget;
	QHBoxLayout* bottomHLayout;
	QSpacerItem* bottomSpacer;
	QPushButton* startTestButton;
	QPushButton* saveChangesButton;
	QTextEdit* commentTextEdit;
	
	TestTableRowProperties* row;
	TestWindow* testwindow;
	Measured measured;
	float changedThresholds[4];
public slots:
	void on_mainTableWidget_cellChanged(int row, int column);
	void on_commentTextEdit_textChanged();
	void on_saveChangesButton_clicked();
	void on_startTestButton_clicked();
};

