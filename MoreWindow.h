#pragma once

#include <QDialog>
#include <QFormLayout>
#include <qtablewidget.h>
#include <qpushbutton.h>
#include <QHeaderView>
#include <QString>

#include "ui_MoreWindow.h"
#include "Components.h"
#include "TestWindow.h"

#define START_WINDOW_WIDTH				850
#define START_WINDOW_HEIGHT				250
#define START_MOREWINDOW_SIZE			START_WINDOW_WIDTH,START_WINDOW_HEIGHT

#define BUTTON_SIZE						100,40

#define PADDING_MAIN_WIDGET				10

#define CEll_PAD_TABLE					0,0
#define CELL_PIN_TABLE					0,1
#define CELL_ID_TABLE					0,2
#define CELL_TYPE_TABLE					0,3
#define CELL_NAME_TABLE					0,4
#define CELL_MEASURED_VALUE_TABLE		0,5
#define CELL_MEASURED_VALUE_U_TABLE		2,5
#define CELL_MEASURED_VALUE_I_TABLE		2,6
#define CELL_PROGS_TABLE				0,7
#define CELL_PROGS_U_TABLE				1,7
#define CELL_PROGS_I_TABLE				1,9
#define CELL_PROGS_U_MIN_TABLE			2,7
#define CELL_PROGS_U_MAX_TABLE			2,8
#define CELL_PROGS_I_MIN_TABLE			2,9
#define CELL_PROGS_I_MAX_TABLE			2,10
#define CEll_VALUE_PAD_TABLE					3,0
#define CELL_VALUE_PIN_TABLE					3,1
#define CELL_VALUE_ID_TABLE						3,2
#define CELL_VALUE_TYPE_TABLE					3,3
#define CELL_VALUE_NAME_TABLE					3,4
#define CELL_VALUE_MEASURED_VALUE_U_TABLE		3,5
#define CELL_VALUE_MEASURED_VALUE_I_TABLE		3,6
#define CELL_VALUE_PROGS_U_MIN_TABLE			3,7
#define CELL_VALUE_PROGS_U_MAX_TABLE			3,8
#define CELL_VALUE_PROGS_I_MIN_TABLE			3,9
#define CELL_VALUE_PROGS_I_MAX_TABLE			3,10

struct Measured
{
	double current;
	double voltage;
};

class MoreWindow : public QDialog
{
	Q_OBJECT

public:
	MoreWindow(Cable cable);
	~MoreWindow();

private:
	void initUi();
	void initUiGenerateTable();
	void initUiBottomLayout();

	void resetLanguage();

	void resizeEvent(QResizeEvent* event);

	QWidget* mainWidget;
	QVBoxLayout* mainVLayout;
	QTableWidget* mainTableWidget;
	QHBoxLayout* bottomHLayout;
	QSpacerItem* bottomSpacer;
	QPushButton* startTestButton;
	QPushButton* saveChangesButton;
	
	Cable cable;
	Measured measured;
};

