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
			border-radius :20px;
			background-color: #D9D9D9;
			border: 5px solid darkblue;
		}
		.QPushButton:hover {
			background-color: #B7B6B6;
		}
	)";
	lightStyles.screenColor =
		R"(
	background-color: #FFFFFF;
	)";
}