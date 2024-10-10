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
	lightStyles.selectFileText =
		R"(
			font-family: Kartika;
			font-size: 15px;
			font-weight: 400;
			margin-top: 15px;
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
}