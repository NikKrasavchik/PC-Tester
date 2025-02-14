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
	)";

	lightStyles.mainSubstrateButtons =
		R"(
		.QWidget {
			border-radius: 10px;
			background-color: #D3D3D3;
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

	lightStyles.settingButton =
		R"(
		.QPushButton {
			border-radius: 25px;
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
	)";

	lightStyles.settingComboBox =
		R"(
		background-color: #B0B0B0;
		border-radius: 10px;

		color: black;
		font-family: Kartika;
		font-size: 12px;
		font-weight: 600;
	)";

	lightStyles.settingSelectText =
		R"(
			color: black;
			line-height: 1px;
			font-family: Kartika;
			font-size: 12px;
			font-weight: 600;
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
	)";

	darkStyles.mainSubstrateButtons =
		R"(
		.QWidget {
			border-radius: 10px;
			background-color: #414141;
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

	darkStyles.settingButton =
		R"(
		.QPushButton {
			border-radius: 25px;
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
	)";

	darkStyles.settingComboBox =
		R"(
		background-color: #535353;
		border-radius: 10px;

		color: white;
		font-family: Kartika;
		font-size: 12px;
		font-weight: 600;
	)";

	darkStyles.settingSelectText =
		R"(
			color: white;
			line-height: 1px;
			font-family: Kartika;
			font-size: 12px;
			font-weight: 600;
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
	)";

	lightStyles.testwindowTestTimeComboBox =
		R"(
		background-color: #B0B0B0;
		border-radius: 10px;

		color: black;
		font-family: Kartika;
		font-size: 12px;
		font-weight: 600;
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

	)";

	lightStyles.testwindowNameLineEdit =
		R"(
			color: black;
			background-color: #D3D3D3;

			line-height: 1px;
			font-family: Kartika;
			font-size: 12px;
			font-weight: 600;

	)";
	lightStyles.testwindowLableBlock =
		R"(
			color: black;

			line-height: 1px;
			font-family: Kartika;
			font-size: 30px;
			font-weight: 800;

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
	)";

	darkStyles.testwindowTestTimeComboBox =
		R"(
		background-color: #535353;
		border-radius: 10px;

		color: white;
		font-family: Kartika;
		font-size: 12px;
		font-weight: 600;
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
		QPushButton {
			border-radius: 4px;
			background-color: #474747;

			color: white;
			font-family: Kartika;
			font-size: 13px;
			font-weight: 600;
		}
		QPushButton:hover {
			background-color: #6E6E6E;
		}
		QPushButton:hover:pressed {
			background-color: #858383;
		}

	)";

	darkStyles.testwindowNameLineEdit =
		R"(
			color: white;
			background-color: #414141;

			line-height: 1px;
			font-family: Kartika;
			font-size: 12px;
			font-weight: 600;

	)";
	darkStyles.testwindowLableBlock =
		R"(
			color: white;

			line-height: 1px;
			font-family: Kartika;
			font-size: 30px;
			font-weight: 800;

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
	)";
}
