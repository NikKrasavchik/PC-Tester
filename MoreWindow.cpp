#include "MoreWindow.h"

MoreWindow::MoreWindow(Cable cable)
{
	this->cable = cable;
	measured.current = -1;
	measured.voltage = -1;

	generateTable();
}
//
//MoreWindow::MoreWindow(Cable cable, Measured measured)
//{
//	this->cable = cable;
//	this->measured.current = measured.current;
//	this->measured.voltage -= measured.voltage;
//
//	generateTable();
//}
//
void MoreWindow::generateTable()
{

}