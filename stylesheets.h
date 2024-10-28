#pragma once

#include <QString>
#include <string>

struct Styles
{
	QString mainStyle;
	QString headerDefaultStyle;
	QString headerCollapseStyle;
	QString headerMaximizeStyle;
	QString bodyStyle;

	QString standartButton;
	QString settingButton;
	QString mainButton;
	QString mainButtonNoActive;
	QString alwaysActiveButton;
	QString screenColor;
	QString selectText;
	QString roundSlider;
	QString bgSlider;
	QString comboBox;
	QString themeLangButton;
	QString testButtonBackground;
};

extern Styles darkStyles;
extern Styles lightStyles;
