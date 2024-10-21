#include "stylesheets.h"
#include "mainwindow.h"

Styles darkStyles;
Styles lightStyles;

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
	lightStyles.comboBox =
		R"(
			background-color: #D9D9D9;
			border-radius: 10px;

			font-family: Kartika;
			font-size: 11px;
			font-weight: 600;
	)";
	lightStyles.roundSlider = "#939393";
	lightStyles.bgSlider = "#E8E8E8";

	// Dark
	darkStyles.standartButton =
		R"(
		.QPushButton {
			border-radius: 25px;
			background-color: #807F7F;

			color: #D8D8D8;
			font-family: Kartika;
			font-size: 25px;
			font-weight: 700;
		}
		.QPushButton:hover {
			background-color: #707070;
		}
		.QPushButton:hover:pressed {
			background-color: #585858;
		}
	)";
	darkStyles.settingButton =
		R"(
		.QPushButton {
			border-radius: 25px;
			background-color: #807F7F;

			color: #D8D8D8;
			font-family: Kartika;
			font-size: 20px;
			font-weight: 600;
		}
		.QPushButton:hover {
			background-color: #707070;
		}
		.QPushButton:hover:pressed {
			background-color: #585858;
		}
	)";
	darkStyles.mainButton =
		R"(
		.QPushButton {
			border-radius: 20px;
			background-color: #807F7F;

			color: #D8D8D8;
			font-family: Kartika;
			font-size: 20px;
			font-weight: 600;
		}
		.QPushButton:hover {
			background-color: #707070;
		}
		.QPushButton:hover:pressed {
			background-color: #585858;
		}
	)";
	darkStyles.ThemaLangButton =
		R"(
		.QPushButton {
			border-radius: 10px;
			background-color: #797777;
			margin-top: 5px;
		}
		.QPushButton:hover {
			background-color: #6F6E6E;
		}
		.QPushButton:hover:pressed {
			background-color: #939393;
		}
	)";
	darkStyles.mainButtonNoActive =
		R"(
		.QPushButton {
			border-radius: 20px;
			background-color: #929292;

			color: #A7A7A7;
			font-family: Kartika;
			font-size: 20px;
			font-weight: 600;
		}
	)";
	darkStyles.alwaysActiveButton =
		R"(
		.QPushButton {
			border-radius: 25px;
			background-color: #585858;

			color: #D8D8D8;
			font-family: Kartika;
			font-size: 25px;
			font-weight: 700;
		}

	)";
	darkStyles.screenColor =
		R"(
	background-color: #282828;
	)";
	darkStyles.selectText =
		R"(
			color: #D8D8D8;
			line-height: 1px;
			font-family: Kartika;
			font-size: 11px;
			font-weight: 600;
	)";
	darkStyles.comboBox =
		R"(
			background-color: #807F7F;
			border-radius: 10px;

			color: #D8D8D8;
			font-family: Kartika;
			font-size: 11px;
			font-weight: 600;
	)";
	darkStyles.roundSlider = "#585858";
	darkStyles.bgSlider = "#777777";
}