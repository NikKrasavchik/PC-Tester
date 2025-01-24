#include "Cable.h"

Cable::Cable()
{
	this->id = -1;
	this->connector = ConnectorId::EMPTY;
	this->pin = -1;
	this->direction = -1;
	this->type = -1;
	this->canId = -1;
	this->bit = -1;
	this->minCurrent = -1;
	this->maxCurrent = -1;
	this->minVoltage = -1;
	this->maxVoltage = -1;
	this->name = "";
	this->component = "";
}

Cable::Cable(ConnectorId connector, int pin)
{
	this->id = -1;
	this->connector = connector;
	this->pin = pin;
	this->direction = -1;
	this->type = -1;
	this->canId = -1;
	this->bit = -1;
	this->minCurrent = -1;
	this->maxCurrent = -1;
	this->minVoltage = -1;
	this->maxVoltage = -1;
	this->name = "";
	this->component = "";
}

Cable::Cable(const Cable& cable)
{
	this->id = cable.id;
	this->connector = cable.connector;
	this->pin = cable.pin;
	this->direction = cable.direction;
	this->type = cable.type;
	this->canId = cable.canId;
	this->bit = cable.bit;
	this->minCurrent = cable.minCurrent;
	this->maxCurrent = cable.maxCurrent;
	this->minVoltage = cable.maxVoltage;
	this->name = cable.name;
	this->component = cable.component;
}