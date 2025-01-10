#pragma once

#include <QDialog>

#include "ui_MoreWindow.h"
#include "Components.h"

struct Measured
{
	double current;
	double voltage;
};

class MoreWindow : public QDialog
{
	Q_OBJECT

public:
	MoreWindow(Cable cable);
//	MoreWindow(Cable cable, Measured measured);
	~MoreWindow();
//
//private:
	void generateTable();
//
	Cable cable;
	Measured measured;
};

