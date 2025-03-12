#pragma once

#include "ui_ReportWindow.h"
#include <QDialog>
#include <QFormLayout>
#include <qtablewidget.h>
#include <qpushbutton.h>
#include <QHeaderView>
#include <qmessagebox.h>
#include <QString>
#include <QDateTime>
#include <QCheckBox>

#include "Components.h"
#include "TestWindow.h"
#include "Xlsx/xlsxdocument.h"

#define WINDOW_MIN_SIZE_WIDTH			1000
#define WINDOW_MIN_SIZE_HEIGHT			700
#define WINDOW_MIN_SIZE					WINDOW_MIN_SIZE_WIDTH,	WINDOW_MIN_SIZE_HEIGHT

#define LEFT_PADDING_MAIN_WIDGET		10
#define UP_PADDING_MAIN_WIDGET			10

#define MEASUREMENT_OFFSET_DOUBLE		2
#define MEASUREMENT_OFFSET_QUADRUPLE	4
#define MEASUREMENT_OFFSET_SEXTUPLE		6

#define MEASUREMENT_OFFSET_OUT			MEASUREMENT_OFFSET_SEXTUPLE
#define MEASUREMENT_OFFSET_IN			MEASUREMENT_OFFSET_DOUBLE
#define MEASUREMENT_OFFSET_IN_ANALOG	MEASUREMENT_OFFSET_QUADRUPLE

#define ROW_COUNT_BASE_TABLE			1
#define COLUMN_COUNT_BASE_TABLE			6
#define COLUMN_COUNT_MANUAL				7

#define SPAN_VERTICAL_DOUBLE			2, 1
#define SPAN_VERTICAL_TRIPPLE			3, 1
#define SPAN_VERTICAL_QUADRUPLE			4, 1

#define SPAN_HORIZONTAL_DOUBLE			1, 2
#define SPAN_HORIZONTAL_TRIPPLE			1, 3
#define SPAN_HORIZONTAL_QUADRUPLE		1, 4
#define SPAN_HORIZONTAL_SEXTUPLE		1, 6

#define SPAN_NONE						1, 1
#define SPAN_SQUAD_DOUBLE				2, 2

#define SPAN_TYPE_OUT					4, 5
#define SPAN_TYPE_IN					1, 5
#define SPAN_TYPE_IN_ANALOG				3, 5

#define SPAN_TYPE_COMMENT_OUT			4, 1
#define SPAN_TYPE_COMMENT_IN			1, 1
#define SPAN_TYPE_COMMENT_IN_ANALOG		3, 1

#define ROW_COUNT_SIGN_OUT				4
#define ROW_COUNT_SIGN_IN_ANALOG		3

#define IND_ROW_BASE_SIGN				0

#define IND_ROW_BASE_SIGN_CONNECTOR		IND_ROW_BASE_SIGN
#define IND_ROW_BASE_SIGN_PIN			IND_ROW_BASE_SIGN
#define IND_ROW_BASE_SIGN_DIRECTION		IND_ROW_BASE_SIGN
#define IND_ROW_BASE_SIGN_TYPE			IND_ROW_BASE_SIGN
#define IND_ROW_BASE_SIGN_NAME			IND_ROW_BASE_SIGN
#define IND_ROW_BASE_SIGN_EMPTY			IND_ROW_BASE_SIGN
#define IND_ROW_BASE_SIGN_COMMENT		IND_ROW_BASE_SIGN

#define IND_COLUMN_BASE_CONNECTOR		0
#define IND_COLUMN_BASE_PIN				1
#define IND_COLUMN_BASE_DIRECTION		2
#define IND_COLUMN_BASE_TYPE			3
#define IND_COLUMN_BASE_NAME			4

#define CELL_SIGN_BASE_CONNECTOR		IND_ROW_BASE_SIGN_CONNECTOR,	IND_COLUMN_BASE_CONNECTOR
#define CELL_SIGN_BASE_PIN				IND_ROW_BASE_SIGN_PIN,			IND_COLUMN_BASE_PIN
#define CELL_SIGN_BASE_DIRECTION		IND_ROW_BASE_SIGN_DIRECTION,	IND_COLUMN_BASE_DIRECTION
#define CELL_SIGN_BASE_TYPE				IND_ROW_BASE_SIGN_TYPE,			IND_COLUMN_BASE_TYPE
#define CELL_SIGN_BASE_NAME				IND_ROW_BASE_SIGN_NAME,			IND_COLUMN_BASE_NAME
#define CELL_SIGN_BASE_COMMENT			IND_ROW_BASE_SIGN_COMMENT,		IND_COLUMN_BASE_COMMENT

#define CELL_SIGN_CORRECT				IND_ROW_BASE_SIGN,				IND_COLUMN_BASE_COMMENT

#define IND_COLUMN_MANUAL_VALUE			5

// Save File
#define HEIGHT_HEADERFILE				7	
#define START_ROW_TABLE					9

class TestTableRowProperties;
class TestWindow;
class ReportWindow : public QDialog
{
	Q_OBJECT

public:
	ReportWindow(std::vector<TestTableRowProperties*> cableRows, TestBlockName testingBlock);
	ReportWindow(std::vector<TestTableRowProperties*> cableRows, std::vector<QCheckBox*> checkedState, TestBlockName testingBlock);
	~ReportWindow();

	void setTestingBlock(TestBlockName testingBlock) { this->testingBlock = testingBlock; }
	void setTestingType(WindowType testType) { this->testType = testType; }

private:
	QWidget* mainWidget;
	QWidget* footerWidget;
	QWidget* reportDataWidget;
	QTableWidget* tableWidget;
	QPushButton* saveButton;
	QVBoxLayout* mainVLayout;
	QHBoxLayout* reportDataHLayout;
	QHBoxLayout* footerHLayout;
	QSpacerItem* reportDataSpacer;
	QSpacerItem* mainMiddleSpacer;
	QSpacerItem* footerSpacer;
	QLineEdit* testerNameLineEdit;
	QLineEdit* serialNumberLineEdit;
	QLabel* testerNameLabel;
	QLabel* serialNumberLabel;

	TestBlockName testingBlock;
	QString serialNumberBlock;
	WindowType testType;

	std::vector<TestTableRowProperties*> cableRows;
	std::vector<std::vector<TestTableRowProperties*>> typedCableRows;
	std::vector<bool> checkedState;
	std::vector<std::vector<bool>> typedCheckedState;
	std::vector<QTextEdit*> commentsTextEdits;
	QString testerName;
	QString serialNumber;

	void initUi();
	void initUiTable();
	void initUiFooter();

	void generateTableBaseSign();
	void generateTableAuto();
	void generateTableManual();
	void generateTableSign(TypeCable type, int maxTypeOffset);
	void generateXlsx();
	void generateWarning(Warnings::ReportWindow warning);

	void fillTable(TypeCable type, std::vector<TestTableRowProperties*> cableRows);
	void fillTableOut(std::vector<TestTableRowProperties*> cableRows);
	void fillTableIn(std::vector<TestTableRowProperties*> cableRows);
	void fillTableInAnalog(std::vector<TestTableRowProperties*> cableRows);
	
	void resaveComments();

	void resetBaseLanguage();
	void resetTheme();
	QString getStrType(TypeCable type);

	void resizeEvent(QResizeEvent* event);

public slots:
	void on_saveButton_clicked();
};