#include "Components.h"
#include "MainWindow.h"
#include "TestWindow.h"
#include "WindowFrame.h"

Styles darkStyles;
Styles lightStyles;

void WindowFrame::initLightStyleSheets()
{
	lightStyles.headerDefaultStyle =
		R"(
		#header {
		    background-color: #A7A7A7;
		    border: 1px solid #A7A7A7;
		    border-top-left-radius: 5px;
		    border-top-right-radius: 5px;
		}
	)";

	lightStyles.headerCollapseStyle =
		R"(
		#header {
			background-color: #A7A7A7;
			border: 2px solid #A7A7A7;
			border-top-left-radius: 10px;
			border-top-right-radius: 10px;
			border-bottom-left-radius: 10px;
			border-bottom-right-radius: 10px;
		}
	)";

	lightStyles.headerMaximizeStyle =
		R"(
	    #header {
		    background-color: #A7A7A7;
		    border: 1px solid #A7A7A7;
		    border-radius: 0px;
		}
	)";

	lightStyles.bodyStyle =
		R"(
		#body{
			background-color: #F0F0F0;
			border-bottom: 2px #F0F0F0;
			border-bottom-left-radius: 10px;
			border-bottom-right-radius: 10px;
		}
	)";

	lightStyles.buttonStyle =
		R"(
			background-color: #B0B0B0;
			margin: 2px;
	)";

	lightStyles.titleStyle =
		R"(
			margin-left: 10px;

			color: black;
			font-family: Kartika;
			font-size: 14px;
			font-weight: 400;
	)";
}

void WindowFrame::initDarkStyleSheets()
{
	darkStyles.headerDefaultStyle =
		R"(
		#header {
		    background-color: #282828;
		    border: 1px solid #282828;
		    border-top-left-radius: 5px;
		    border-top-right-radius: 5px;
		}
	)";

	darkStyles.headerCollapseStyle =
		R"(
		#header {
			background-color: #282828;
			border: 2px solid #282828;
			border-top-left-radius: 10px;
			border-top-right-radius: 10px;
			border-bottom-left-radius: 10px;
			border-bottom-right-radius: 10px;
		}
	)";

	darkStyles.headerMaximizeStyle =
		R"(
	    #header {
		    background-color: #282828;
		    border: 1px solid #282828;
		    border-radius: 0px;
		}
	)";

	darkStyles.bodyStyle =
		R"(
		#body{
			background-color: #3A3A3A;
			border-bottom: 2px #3A3A3A;
			border-bottom-left-radius: 10px;
			border-bottom-right-radius: 10px;
		}
	)";

	darkStyles.buttonStyle =
		R"(
			background-color: #535353;
			margin: 2px;
	)";

	darkStyles.titleStyle =
		R"(
			margin-left: 10px;

			color: white;
			font-family: Kartika;
			font-size: 14px;
			font-weight: 400;
	)";
}

void MainWindow::initLightStyleSheets()
{
	lightStyles.standButtons =
		R"(
		.QPushButton {
			border-radius: 10px;
			background-color: #B0B0B0;

			color: black;
			font-family: Kartika;
			font-size: 25px;
			font-weight: 700;
		}

		.QPushButton:hover {
			background-color: #9D9D9D;
		}

		.QPushButton:hover:pressed {
			background-color: #767676;
		}
		QToolTip {
			color: #3f3f3f;
			background-color: #F0F0F0;
			border-radius: 5px;
			border: 1px solid #3f3f3f;
		}
	)";

	lightStyles.alwaysActiveStandButton =
		R"(
		.QPushButton {
			border-radius: 10px;
			background-color: #767676;

			color: black;
			font-family: Kartika;
			font-size: 25px;
			font-weight: 700;
		}
		QToolTip {
			color: #3f3f3f;
			background-color: #F0F0F0;
			border-radius: 5px;
			border: 1px solid #3f3f3f;
		}
	)";

	lightStyles.roundSlider = "#767676";
	lightStyles.bgSlider = "#D4D4D4";

	lightStyles.themeLangButton =
		R"(
		.QPushButton {
			border-radius: 10px;
			background-color: #B0B0B0;
			margin-top: 5px;
		}

		.QPushButton:hover {
			background-color: #9D9D9D;
		}

		.QPushButton:hover:pressed {
			background-color: #767676;
		}
		QToolTip {
			color: #3f3f3f;
			background-color: #F0F0F0;
			border-radius: 5px;
			border: 1px solid #3f3f3f;
		}
	)";

	lightStyles.mainButton =
		R"(
		.QPushButton {
			border-radius: 20px;
			background-color: #999898;

			color: black;
			font-family: Kartika;
			font-size: 20px;
			font-weight: 600;
		}

		.QPushButton:hover {
			background-color: #4E4C4C;
		}

		.QPushButton:hover:pressed {
			background-color: #373737;
		}
		QToolTip {
			color: #3f3f3f;
			background-color: #F0F0F0;
			border-radius: 5px;
			border: 1px solid #3f3f3f;
		}
	)";

	lightStyles.mainButtonNoActive =
		R"(
		.QPushButton {
			border-radius: 20px;
			background-color: #CACACA;

			color: #5B5B5B;
			font-family: Kartika;
			font-size: 20px;
			font-weight: 600;
		}
		QToolTip {
			color: #3f3f3f;
			background-color: #F0F0F0;
			border-radius: 5px;
			border: 1px solid #3f3f3f;
		}
	)";

	lightStyles.mainSubstrateButtons =
		R"(
		.QWidget {
			border-radius: 10px;
			background-color: #D3D3D3;
		}
		QToolTip {
			color: #3f3f3f;
			background-color: #F0F0F0;
			border-radius: 5px;
			border: 1px solid #3f3f3f;
		}
	)";

	lightStyles.mainText =
		R"(
			color: black;
			line-height: 1px;
			font-family: Kartika;
			font-size: 20px;
			font-weight: 800;
	)";

	lightStyles.settingComboBox =
		R"(
		QWidget {
			background-color: #B0B0B0;
			border-radius: 10px;

			color: black;
			font-family: Kartika;
			font-size: 12px;
			font-weight: 600;
		}
		QToolTip {
			color: #3f3f3f;
			background-color: #F0F0F0;
			border-radius: 5px;
			border: 1px solid #3f3f3f;
		}
	)";

	lightStyles.settingSelectText =
		R"(
		QWidget {
			color: black;
			background-color: #F0F0F0;
			line-height: 1px;
			font-family: Kartika;
			font-size: 12px;
			font-weight: 600;
		}
		QToolTip {
			color: #3f3f3f;
			background-color: #F0F0F0;
			border-radius: 5px;
			border: 1px solid #3f3f3f;
		}
	)";
}

void MainWindow::initDarkStyleSheets()
{
	darkStyles.standButtons =
		R"(
		.QPushButton {
			border-radius: 10px;
			background-color: #535353;

			color: white;
			font-family: Kartika;
			font-size: 25px;
			font-weight: 700;
		}

		.QPushButton:hover {
			background-color: #6E6E6E;
		}

		.QPushButton:hover:pressed {
			background-color: #858383;
		}
		QToolTip {
        color: #bebebe;
        background-color: #3A3A3A;
		border-radius: 5px;
        border: 1px solid #bebebe;
		}
	)";

	darkStyles.alwaysActiveStandButton =
		R"(
		.QPushButton {
			border-radius: 10px;
			background-color: #858383;

			color: white;
			font-family: Kartika;
			font-size: 25px;
			font-weight: 700;
		}
		QToolTip {
        color: #bebebe;
        background-color: #3A3A3A;
		border-radius: 5px;
        border: 1px solid #bebebe;
		}
	)";

	darkStyles.roundSlider = "#858383";
	darkStyles.bgSlider = "#2B2A2A";

	darkStyles.themeLangButton =
		R"(
		.QPushButton {
			border-radius: 10px;
			background-color: #535353;
			margin-top: 5px;
		}

		.QPushButton:hover {
			background-color: #6E6E6E;
		}

		.QPushButton:hover:pressed {
			background-color: #858383;
		}
		QToolTip {
        color: #bebebe;
        background-color: #3A3A3A;
		border-radius: 5px;
        border: 1px solid #bebebe;
		}
	)";

	darkStyles.mainButton =
		R"(
		.QPushButton {
			border-radius: 20px;
			background-color: #535353;

			color: white;
			font-family: Kartika;
			font-size: 20px;
			font-weight: 600;
		}

		.QPushButton:hover {
			background-color: #6E6E6E;
		}

		.QPushButton:hover:pressed {
			background-color: #858383;
		}
		QToolTip {
        color: #bebebe;
        background-color: #3A3A3A;
		border-radius: 5px;
        border: 1px solid #bebebe;
		}
	)";

	darkStyles.mainButtonNoActive =
		R"(
		.QPushButton {
			border-radius: 20px;
			background-color: #2B2A2A;

			color: #7A7878;
			font-family: Kartika;
			font-size: 20px;
			font-weight: 600;
		}
		QToolTip {
        color: #bebebe;
        background-color: #3A3A3A;
		border-radius: 5px;
        border: 1px solid #bebebe;
		}
	)";

	darkStyles.mainSubstrateButtons =
		R"(
		.QWidget {
			border-radius: 10px;
			background-color: #414141;
		}
		QToolTip {
			color: #bebebe;
			background-color: #3A3A3A;
			border-radius: 5px;
			border: 1px solid #bebebe;
		}

	)";

	darkStyles.mainText =
		R"(
			color: white;
			line-height: 1px;
			font-family: Kartika;
			font-size: 20px;
			font-weight: 800;
	)";

	darkStyles.settingComboBox =
		R"(
		QWidget {
			background-color: #535353;
			border-radius: 10px;

			color: white;
			font-family: Kartika;
			font-size: 12px;
			font-weight: 600;
		}
		QToolTip {
			color: #bebebe;
			background-color: #3A3A3A;
			border-radius: 5px;
			border: 1px solid #bebebe;
		}

	)";

	darkStyles.settingSelectText =
		R"(
		QWidget {
			color: white;
			background-color: #3A3A3A;
			line-height: 1px;
			font-family: Kartika;
			font-size: 12px;
			font-weight: 600;
			}
		QToolTip {
			color: #bebebe;
			background-color: #3A3A3A;
			border-radius: 5px;
			border: 1px solid #bebebe;
			}
	)";
}

void TestWindow::initLightStyleSheets()
{
	lightStyles.testwindowMoveButtonStyle =
		R"(
		.QPushButton {
			margin: 3px;

			border-radius: 10px;
			background-color: #B0B0B0;

			color: black;
			font-family: Kartika;
			font-size: 20px;
			font-weight: 600;
		}

		.QPushButton:hover {
			background-color: #9D9D9D;
		}

		.QPushButton:hover:pressed {
			background-color: #767676;
		}
		QToolTip {
			color: #3f3f3f;
			background-color: #F0F0F0;
			border-radius: 5px;
			border: 1px solid #3f3f3f;
		}
	)";

	lightStyles.testwindowConnectButtonStyleConnect =
		R"(
		.QPushButton {
			margin: 3px;

			border-radius: 10px;
			background-color: #7CC770  ;

			color: black;
			font-family: Kartika;
			font-size: 13px;
			font-weight: 600;
		}
		QToolTip {
			color: #3f3f3f;
			background-color: #F0F0F0;
			border-radius: 5px;
			border: 1px solid #3f3f3f;
		}
	)";

	lightStyles.testwindowConnectButtonStyleDisconnected =
		R"(
		.QPushButton {
			margin: 3px;

			border-radius: 10px;
			background-color: #FF8686;

			color: black;
			font-family: Kartika;
			font-size: 13px;
			font-weight: 600;
		}
		QToolTip {
			color: #3f3f3f;
			background-color: #F0F0F0;
			border-radius: 5px;
			border: 1px solid #3f3f3f;
		}
	)";

	lightStyles.testwindowButtonStyle =
		R"(
		.QPushButton {
			margin: 3px;

			border-radius: 10px;
			background-color: #B0B0B0;

			color: black;
			font-family: Kartika;
			font-size: 13px;
			font-weight: 600;
		}

		.QPushButton:hover {
			background-color: #9D9D9D;
		}

		.QPushButton:hover:pressed {
			background-color: #767676;
		}
		QToolTip {
			color: #3f3f3f;
			background-color: #F0F0F0;
			border-radius: 5px;
			border: 1px solid #3f3f3f;
		}
	)";

	lightStyles.testwindowTestTimeComboBox =
		R"(
		QWidget {
			background-color: #B0B0B0;
			border-radius: 10px;

			color: black;
			font-family: Kartika;
			font-size: 12px;
			font-weight: 600;
		}
		QToolTip {
			color: #3f3f3f;
			background-color: #F0F0F0;
			border-radius: 5px;
			border: 1px solid #3f3f3f;
		}
	)";

	lightStyles.testwindowTableWidget =
		R"(
		QScrollBar:horizontal {
    		background-color: #6E6E6E;
		}

		QScrollBar:vertical {
    		background-color: #6E6E6E;
		}

		QHeaderView {
			background-color: #D3D3D3;
		}

		QHeaderView::section {
			background-color: #9D9D9D;

			color: black;
			font-family: Kartika;
			font-size: 12px;
			font-weight: 600;
		}

		QTableView {
			background-color: #B0B0B0;

			color: black;
			font-family: Kartika;
			font-size: 12px;
			font-weight: 600;
		}

		QTableView::item:selected {
			background-color: #6E6E6E;
		}

		QComboBox {
			background-color: #B0B0B0;
			border-radius: 10px;

			color: white;
			font-family: Kartika;
			font-size: 12px;
			font-weight: 600;
		}
		
		QLineEdit {
			color: black;
			background-color: #D3D3D3;

			line-height: 1px;
			font-family: Kartika;
			font-size: 12px;
			font-weight: 600;
		}

		QPushButton {
			border-radius: 4px;
			background-color: #949494;

			color: black;
			font-family: Kartika;
			font-size: 12px;
			font-weight: 400;
		}

		QPushButton:hover {
			background-color: #9D9D9D;
		}

		QPushButton:hover:pressed {
			background-color: #767676;
		}
		QToolTip {
			color: #3f3f3f;
			background-color: #F0F0F0;
			border-radius: 5px;
			border: 1px solid #3f3f3f;
		}
	)";

	lightStyles.reportwindowNameLineEdit =
		R"(
		QWidget {
			color: black;
			background-color: #D3D3D3;

			line-height: 1px;
			font-family: Kartika;
			font-size: 12px;
			font-weight: 600;
		}
		QToolTip {
			color: #3f3f3f;
			background-color: #F0F0F0;
			border-radius: 5px;
			border: 1px solid #3f3f3f;
		}
	)";

	lightStyles.reportwindowSerialLineEdit =
		R"(
		QWidget {
			color: black;
			background-color: #D3D3D3;

			line-height: 1px;
			font-family: Kartika;
			font-size: 12px;
			font-weight: 600;
		}
		QToolTip {
			font-size: 15px;
			font-weight: 600;
			color: #3f3f3f;
			background-color: #F0F0F0;
			border-radius: 5px;
			border: 1px solid #3f3f3f;
		}
	)";

	lightStyles.testwindowLableBlock =
		R"(
		QWidget {
			color: black;

			line-height: 1px;
			font-family: Kartika;
			font-size: 30px;
			font-weight: 800;
		}
		QToolTip {
			color: #3f3f3f;
			background-color: #F0F0F0;
			border-radius: 5px;
			border: 1px solid #3f3f3f;
		}
	)";

	lightStyles.activeTableButton =
		R"(
		.QPushButton {
			margin: 3px;

			border-radius: 10px;
			background-color: #767676;

			color: black;
			font-family: Kartika;
			font-size: 12px;
			font-weight: 400;
		}
		QToolTip {
			color: #3f3f3f;
			background-color: #F0F0F0;
			border-radius: 5px;
			border: 1px solid #3f3f3f;
		}
	)";

	lightStyles.inactiveTableButton =
		R"(
		.QPushButton {
			margin: 3px;

			border-radius: 10px;
			background-color: #949494;

			color: black;
			font-family: Kartika;
			font-size: 12px;
			font-weight: 400;
		}

		.QPushButton:hover {
			background-color: #9D9D9D;
		}

		.QPushButton:hover:pressed {
			background-color: #767676;
		}
		QToolTip {
			color: #3f3f3f;
			background-color: #F0F0F0;
			border-radius: 5px;
			border: 1px solid #3f3f3f;
		}
	)";

	lightStyles.testwindowManualCheckBox =
		R"(
		.QCheckBox{ 
			font:18px;
		}
		.QCheckBox::indicator {
			border: 3px solid rgb(252, 85, 85);
			width: 20px;
			height: 20px;
			border-radius: 12px;
			background-color: rgb(135, 135, 135);
		}
		.QCheckBox::indicator:checked {
			border: 3px solid rgb(135, 135, 135);
			background-color: rgb(124, 199, 112);
		}
		QToolTip {
			color: #3f3f3f;
			background-color: #F0F0F0;
			border-radius: 5px;
			border: 1px solid #3f3f3f;
		}
	)";
}

void TestWindow::initDarkStyleSheets()
{
	darkStyles.testwindowMoveButtonStyle =
		R"(
		.QPushButton {
			margin: 3px;

			border-radius: 10px;
			background-color: #535353;

			color: white;
			font-family: Kartika;
			font-size: 20px;
			font-weight: 600;
		}

		.QPushButton:hover {
			background-color: #6E6E6E;
		}

		.QPushButton:hover:pressed {
			background-color: #858383;
		}
		QToolTip {
			color: #bebebe;
			background-color: #3A3A3A;
			border-radius: 5px;
			border: 1px solid #bebebe;
		}
	)";

	darkStyles.testwindowConnectButtonStyleConnect =
		R"(
		.QPushButton {
			margin: 3px;

			border-radius: 10px;
			background-color: #7CC770;

			color: white;
			font-family: Kartika;
			font-size: 13px;
			font-weight: 600;
		}
		QToolTip {
			color: #bebebe;
			background-color: #3A3A3A;
			border-radius: 5px;
			border: 1px solid #bebebe;
		}
	)";

	darkStyles.testwindowConnectButtonStyleDisconnected =
		R"(
		.QPushButton {
			margin: 3px;

			border-radius: 10px;
			background-color: #FF8686;

			color: white;
			font-family: Kartika;
			font-size: 13px;
			font-weight: 600;
		}
		QToolTip {
			color: #bebebe;
			background-color: #3A3A3A;
			border-radius: 5px;
			border: 1px solid #bebebe;
		}
	)";

	darkStyles.testwindowButtonStyle =
		R"(
		.QPushButton {
			margin: 3px;

			border-radius: 10px;
			background-color: #535353;

			color: white;
			font-family: Kartika;
			font-size: 13px;
			font-weight: 600;
		}

		.QPushButton:hover {
			background-color: #6E6E6E;
		}

		.QPushButton:hover:pressed {
			background-color: #858383;
		}
		QToolTip {
			color: #bebebe;
			background-color: #3A3A3A;
			border-radius: 5px;
			border: 1px solid #bebebe;
		}
	)";

	darkStyles.testwindowTestTimeComboBox =
		R"(
		QWidget {
			background-color: #535353;
			border-radius: 10px;

			color: white;
			font-family: Kartika;
			font-size: 12px;
			font-weight: 600;
		}
		QToolTip {
			color: #bebebe;
			background-color: #3A3A3A;
			border-radius: 5px;
			border: 1px solid #bebebe;
		}
	)";

	darkStyles.testwindowTableWidget =
		R"(
		QScrollBar:horizontal {
    		background-color: #6E6E6E;
		}

		QScrollBar:vertical {
    		background-color: #6E6E6E;
		}

		QHeaderView::section {
			background-color: #414141;

			color: white;
			font-family: Kartika;
			font-size: 12px;
			font-weight: 600;
		}

		QTableView {
			background-color: #535353;

			color: white;
			font-family: Kartika;
			font-size: 12px;
			font-weight: 600;
		}

		QTableView::item:selected {
			background-color: #9D9D9D;
		}

		QComboBox {
			background-color: #535353;
			border-radius: 10px;

			color: white;
			font-family: Kartika;
			font-size: 12px;
			font-weight: 600;
		}

		QLineEdit {
			color: white;
			background-color: #414141;

			line-height: 1px;
			font-family: Kartika;
			font-size: 12px;
			font-weight: 600;
		}
		
		QPushButton {
			border-radius: 4px;
			background-color: #949494;

			color: black;
			font-family: Kartika;
			font-size: 12px;
			font-weight: 400;
		}

		QPushButton:hover {
			background-color: #9D9D9D;
		}
		QPushButton:hover:pressed {
			background-color: #858383;
		}
		QToolTip {
			color: #bebebe;
			background-color: #3A3A3A;
			border-radius: 5px;
			border: 1px solid #bebebe;
		}
			
	)";

	darkStyles.reportwindowNameLineEdit =
		R"(
		QWidget {
			color: white;
			background-color: #414141;

			line-height: 1px;
			font-family: Kartika;
			font-size: 12px;
			font-weight: 600;
		}
		QToolTip {
			color: #bebebe;
			background-color: #3A3A3A;
			border-radius: 5px;
			border: 1px solid #bebebe;
		}
	)";

	darkStyles.reportwindowSerialLineEdit =
		R"(
		QWidget {
			color: white;
			background-color: #414141;

			line-height: 1px;
			font-family: Kartika;
			font-size: 12px;
			font-weight: 600;
		}
		QToolTip {
			font-size: 15px;
			font-weight: 600;
			color: #bebebe;
			background-color: #3A3A3A;
			border-radius: 5px;
			border: 1px solid #bebebe;
		}
	)";

	darkStyles.testwindowLableBlock =
		R"(
		QWidget {
			color: white;

			line-height: 1px;
			font-family: Kartika;
			font-size: 30px;
			font-weight: 800;
		}
		QToolTip {
			color: #bebebe;
			background-color: #3A3A3A;
			border-radius: 5px;
			border: 1px solid #bebebe;
		}
	)";

	darkStyles.activeTableButton =
		R"(
		.QPushButton {
			margin: 3px;

			border-radius: 10px;
			background-color: #858383;

			color: white;
			font-family: Kartika;
			font-size: 13px;
			font-weight: 600;
		}
		QToolTip {
			color: #bebebe;
			background-color: #3A3A3A;
			border-radius: 5px;
			border: 1px solid #bebebe;
		}
	)";

	darkStyles.inactiveTableButton =
		R"(
		.QPushButton {
			margin: 3px;

			border-radius: 10px;
			background-color: #535353;

			color: white;
			font-family: Kartika;
			font-size: 13px;
			font-weight: 600;
		}

		.QPushButton:hover {
			background-color: #6E6E6E;
		}

		.QPushButton:hover:pressed {
			background-color: #858383;
		}
		QToolTip {
			color: #bebebe;
			background-color: #3A3A3A;
			border-radius: 5px;
			border: 1px solid #bebebe;
		}
	)";
}
