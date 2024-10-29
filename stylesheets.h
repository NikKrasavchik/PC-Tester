#pragma once

#include <QString>
#include <string>

struct Styles
{
	// Frame
	// header
	QString headerDefaultStyle;
	QString headerCollapseStyle;
	QString headerMaximizeStyle;
	QString bodyStyle;

	// button
	QString buttonStyle;

	// Mainwindow
	// Header
	//selectStand
	QString standButtons;
	QString alwaysActiveStandButton;
	QString roundSlider;
	QString bgSlider;

	//themeLanguage
	QString themeLangButton;

	// Main
	QString mainButton;
	QString mainButtonNoActive;
	QString mainSubstrateButtons;
	QString mainText;

	// Setting
	QString settingButton;
	QString settingComboBox;
	QString settingSelectText;
};

extern Styles darkStyles;
extern Styles lightStyles;
