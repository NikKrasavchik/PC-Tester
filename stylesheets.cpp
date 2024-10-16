#include "stylesheets.h"
#include "mainwindow.h"

Styles darkStyles;
Styles lightStyles;
Styles lolol;

void MainWindow::initStyles()
{
	lightStyles.standartButton =
		R"(
		.QPushButton {
			border-radius: 25px;
			background-color: #D9D9D9;

			font-family: Kartika;
			font-size: 25px;
			font-weight: 700;
		}
		.QPushButton:hover {
			background-color: #B7B6B6;
		}
		.QPushButton:hover:pressed {
			background-color: #979797;
		}
	)";
	lightStyles.settingButton =
		R"(
		.QPushButton {
			border-radius: 25px;
			background-color: #D9D9D9;

			font-family: Kartika;
			font-size: 20px;
			font-weight: 600;
		}
		.QPushButton:hover {
			background-color: #B7B6B6;
		}
		.QPushButton:hover:pressed {
			background-color: #979797;
		}
	)";
	lightStyles.mainButton =
		R"(
		.QPushButton {
			border-radius: 20px;
			background-color: #999898;

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
	lightStyles.ThemaLangButton =
		R"(
		.QPushButton {
			border-radius: 10px;
			background-color: #DCDCDC;
			margin-top: 5px;
		}
		.QPushButton:hover {
			background-color: #B9B5B5;
		}
		.QPushButton:hover:pressed {
			background-color: #989898;
		}
	)";
	lightStyles.mainButtonNoActive =
		R"(
		.QPushButton {
			border-radius: 20px;
			background-color: #D9D9D9;

			font-family: Kartika;
			font-size: 20px;
			font-weight: 600;
			color: #7A7878;
		}
	)";
	lightStyles.alwaysActiveButton =
		R"(
		.QPushButton {
			border-radius: 25px;
			background-color: #979797;

			font-family: Kartika;
			font-size: 25px;
			font-weight: 700;
		}

	)";
	lightStyles.screenColor =
		R"(
	background-color: #FFFFFF;
	)";
	lightStyles.selectText =
		R"(
			line-height: 1px;
			font-family: Kartika;
			font-size: 11px;
			font-weight: 600;
	)";

	lolol.standartButton =
		R"(
		.QPushButton {
			margin-right: 40px;
			margin-left: 40px;
			height: 100 px;
			width:200px;

			border-radius: 20px;
			background-color: #D9D9D9;
		}
		.QPushButton:hover {
			background-color: #B7B6B6;
		}
		.QPushButton:hover:pressed {
			background-color: #979797;
		}
	)";

	// Dark
	darkStyles.standartButton =
		R"(
		.QPushButton {
			border-radius: 25px;
			background-color: #D9D9D9;

			font-family: Kartika;
			font-size: 25px;
			font-weight: 700;
		}
		.QPushButton:hover {
			background-color: #B7B6B6;
		}
		.QPushButton:hover:pressed {
			background-color: #979797;
		}
	)";
	darkStyles.settingButton =
		R"(
		.QPushButton {
			border-radius: 25px;
			background-color: #D9D9D9;

			font-family: Kartika;
			font-size: 20px;
			font-weight: 600;
		}
		.QPushButton:hover {
			background-color: #B7B6B6;
		}
		.QPushButton:hover:pressed {
			background-color: #979797;
		}
	)";
	darkStyles.mainButton =
		R"(
		.QPushButton {
			border-radius: 20px;
			background-color: #999898;

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
	darkStyles.ThemaLangButton =
		R"(
		.QPushButton {
			border-radius: 10px;
			background-color: #DCDCDC;
			margin-top: 5px;
		}
		.QPushButton:hover {
			background-color: #B9B5B5;
		}
		.QPushButton:hover:pressed {
			background-color: #989898;
		}
	)";
	darkStyles.mainButtonNoActive =
		R"(
		.QPushButton {
			border-radius: 20px;
			background-color: #D9D9D9;

			font-family: Kartika;
			font-size: 20px;
			font-weight: 600;
			color: #7A7878;
		}
	)";
	darkStyles.alwaysActiveButton =
		R"(
		.QPushButton {
			border-radius: 25px;
			background-color: #979797;

			font-family: Kartika;
			font-size: 25px;
			font-weight: 700;
		}

	)";
	darkStyles.screenColor =
		R"(
	background-color: #404040;
	)";
	darkStyles.selectText =
		R"(
			line-height: 1px;
			font-family: Kartika;
			font-size: 11px;
			font-weight: 600;
	)";
}