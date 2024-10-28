#include "stylesheets.h"
#include "MainWindow.h"

Styles darkStyles;
Styles lightStyles;

void WindowFrame::initLightStyleSheets()
{
	lightStyles.headerDefaultStyle =
		R"(
		#header {
		    background-color: #F0F0F0;
		    border: 1px solid #F0F0F0;
		    border-top-left-radius: 10px;
		    border-top-right-radius: 10px;
		}
	)";

	lightStyles.headerCollapseStyle =
		R"(
		#header {
			background-color: #F0F0F0;
			border: 2px solid #F0F0F0;
			border-top-left-radius: 10px;
			border-top-right-radius: 10px;
			border-bottom-left-radius: 10px;
			border-bottom-right-radius: 10px;
		}
	)";

	lightStyles.headerMaximizeStyle =
		R"(
	    #header {
		    background-color: #F0F0F0;
		    border: 1px solid #F0F0F0;
		    border-top-left-radius: 0px;
		    border-top-right-radius: 0px;
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
}

void WindowFrame::initDarkStyleSheets()
{
	darkStyles.headerDefaultStyle =
		R"(
		#header {
		    background-color: #3A3A3A;
		    border: 1px solid #3A3A3A;
		    border-top-left-radius: 10px;
		    border-top-right-radius: 10px;
		}
	)";

	darkStyles.headerCollapseStyle =
		R"(
		#header {
			background-color: #3A3A3A;
			border: 2px solid #3A3A3A;
			border-top-left-radius: 10px;
			border-top-right-radius: 10px;
			border-bottom-left-radius: 10px;
			border-bottom-right-radius: 10px;
		}
	)";

	darkStyles.headerMaximizeStyle =
		R"(
	    #header {
		    background-color: #3A3A3A;
		    border: 1px solid #3A3A3A;
		    border-top-left-radius: 0px;
		    border-top-right-radius: 0px;
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

	darkStyles.roundSlider = "#585858";
	darkStyles.bgSlider = "#777777";
}

void MainWindow::initLightStyleSheets()
{
	lightStyles.standartButton =
		R"(
		.QPushButton {
			border-radius: 25px;
			background-color: #B0B0B0;

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

	lightStyles.settingButton =
		R"(
		.QPushButton {
			border-radius: 25px;
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

	lightStyles.roundSlider = "#939393";
	lightStyles.bgSlider = "#E8E8E8";
}

void MainWindow::initDarkStyleSheets()
{
	darkStyles.standartButton =
		R"(
		.QPushButton {
			border-radius: 25px;
			background-color: #535353;

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

	darkStyles.settingButton =
		R"(
		.QPushButton {
			border-radius: 25px;
			background-color: #535353;

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

	darkStyles.mainButton =
		R"(
		.QPushButton {
			border-radius: 20px;
			background-color: #535353;

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

	darkStyles.mainButtonNoActive =
		R"(
		.QPushButton {
			border-radius: 20px;
			background-color: #2B2A2A;

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
			background-color: #2B2A2A;

			font-family: Kartika;
			font-size: 25px;
			font-weight: 700;
		}
	)";
}

//void MainWindow::initStyleSheets()
//{
	//initLightStyles();
	//initDarkStyles();

	//lightStyles.standartButton =
	//	R"(
	//	.QPushButton {
	//		border-radius: 25px;
	//		background-color: #D9D9D9;

	//		font-family: Kartika;
	//		font-size: 25px;
	//		font-weight: 700;
	//	}
	//	.QPushButton:hover {
	//		background-color: #B7B6B6;
	//	}
	//	.QPushButton:hover:pressed {
	//		background-color: #979797;
	//	}
	//)";
	//lightStyles.settingButton =
	//	R"(
	//	.QPushButton {
	//		border-radius: 25px;
	//		background-color: #D9D9D9;

	//		font-family: Kartika;
	//		font-size: 20px;
	//		font-weight: 600;
	//	}
	//	.QPushButton:hover {
	//		background-color: #B7B6B6;
	//	}
	//	.QPushButton:hover:pressed {
	//		background-color: #979797;
	//	}
	//)";
	//lightStyles.mainButton =
	//	R"(
	//	.QPushButton {
	//		border-radius: 20px;
	//		background-color: #999898;

	//		font-family: Kartika;
	//		font-size: 20px;
	//		font-weight: 600;
	//	}
	//	.QPushButton:hover {
	//		background-color: #4E4C4C;
	//	}
	//	.QPushButton:hover:pressed {
	//		background-color: #373737;
	//	}
	//)";
	//lightStyles.themeLangButton =
	//	R"(
	//	.QPushButton {
	//		border-radius: 10px;
	//		background-color: #DCDCDC;
	//		margin-top: 5px;
	//	}
	//	.QPushButton:hover {
	//		background-color: #B9B5B5;
	//	}
	//	.QPushButton:hover:pressed {
	//		background-color: #989898;
	//	}
	//)";
	//lightStyles.mainButtonNoActive =
	//	R"(
	//	.QPushButton {
	//		border-radius: 20px;
	//		background-color: #D9D9D9;

	//		font-family: Kartika;
	//		font-size: 20px;
	//		font-weight: 600;
	//		color: #7A7878;
	//	}
	//)";
	//lightStyles.alwaysActiveButton =
	//	R"(
	//	.QPushButton {
	//		border-radius: 25px;
	//		background-color: #979797;

	//		font-family: Kartika;
	//		font-size: 25px;
	//		font-weight: 700;
	//	}

	//)";
	//lightStyles.screenColor =
	//	R"(
	//background-color: #FFFFFF;
	//)";
	//lightStyles.selectText =
	//	R"(
	//		line-height: 1px;
	//		font-family: Kartika;
	//		font-size: 11px;
	//		font-weight: 600;
	//)";
	//lightStyles.comboBox =
	//	R"(
	//		background-color: #D9D9D9;
	//		border-radius: 10px;

	//		font-family: Kartika;
	//		font-size: 11px;
	//		font-weight: 600;
	//)";
	//lightStyles.roundSlider = "#939393";
	//lightStyles.bgSlider = "#E8E8E8";

	//lightStyles.testButtonBackground =
	//	R"(
	//	background-color: #EEEEEE;
	//	border-radius: 20px;
	//)";

	//// Dark
	//QFile styleFile(lightStylePath);
	//styleFile.open(QFile::ReadOnly);
	//lightStyles.mainStyle = styleFile.readAll();

	//darkStyles.standartButton =
	//	R"(
	//	.QPushButton {
	//		border-radius: 25px;
	//		background-color: #807F7F;

	//		color: #D8D8D8;
	//		font-family: Kartika;
	//		font-size: 25px;
	//		font-weight: 700;
	//	}
	//	.QPushButton:hover {
	//		background-color: #707070;
	//	}
	//	.QPushButton:hover:pressed {
	//		background-color: #585858;
	//	}
	//)";
	//darkStyles.settingButton =
	//	R"(
	//	.QPushButton {
	//		border-radius: 25px;
	//		background-color: #807F7F;

	//		color: #D8D8D8;
	//		font-family: Kartika;
	//		font-size: 20px;
	//		font-weight: 600;
	//	}
	//	.QPushButton:hover {
	//		background-color: #707070;
	//	}
	//	.QPushButton:hover:pressed {
	//		background-color: #585858;
	//	}
	//)";
	//darkStyles.mainButton =
	//	R"(
	//	.QPushButton {
	//		border-radius: 20px;
	//		background-color: #807F7F;

	//		color: #D8D8D8;
	//		font-family: Kartika;
	//		font-size: 20px;
	//		font-weight: 600;
	//	}
	//	.QPushButton:hover {
	//		background-color: #707070;
	//	}
	//	.QPushButton:hover:pressed {
	//		background-color: #585858;
	//	}
	//)";
	//darkStyles.themeLangButton =
	//	R"(
	//	.QPushButton {
	//		border-radius: 10px;
	//		background-color: #797777;
	//		margin-top: 5px;
	//	}
	//	.QPushButton:hover {
	//		background-color: #6F6E6E;
	//	}
	//	.QPushButton:hover:pressed {
	//		background-color: #939393;
	//	}
	//)";
	//darkStyles.mainButtonNoActive =
	//	R"(
	//	.QPushButton {
	//		border-radius: 20px;
	//		background-color: #929292;

	//		color: #A7A7A7;
	//		font-family: Kartika;
	//		font-size: 20px;
	//		font-weight: 600;
	//	}
	//)";
	//darkStyles.alwaysActiveButton =
	//	R"(
	//	.QPushButton {
	//		border-radius: 25px;
	//		background-color: #585858;

	//		color: #D8D8D8;
	//		font-family: Kartika;
	//		font-size: 25px;
	//		font-weight: 700;
	//	}

	//)";
	//darkStyles.screenColor =
	//	R"(
	//background-color: #282828;
	//)";
	//darkStyles.selectText =
	//	R"(
	//		color: #D8D8D8;
	//		line-height: 1px;
	//		font-family: Kartika;
	//		font-size: 11px;
	//		font-weight: 600;
	//)";
	//darkStyles.comboBox =
	//	R"(
	//		background-color: #807F7F;
	//		border-radius: 10px;

	//		color: #D8D8D8;
	//		font-family: Kartika;
	//		font-size: 11px;
	//		font-weight: 600;
	//)";
	//darkStyles.roundSlider = "#585858";
	//darkStyles.bgSlider = "#777777";
//}