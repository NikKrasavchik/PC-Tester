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
	lightStyles.activeButton =
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