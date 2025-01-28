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
	this->thresholds.push_back(Thresholds());
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
	this->thresholds.push_back(Thresholds());
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
	for (int i = 0; i < cable.thresholds.size(); i++)
		this->thresholds.push_back(Thresholds(cable.thresholds[i].minCurrent, cable.thresholds[i].maxCurrent, cable.thresholds[i].minVoltage, cable.thresholds[i].maxVoltage));
	this->name = cable.name;
	this->component = cable.component;
}

void Cable::setThresholds(std::vector<Thresholds> thresholds)
{
	this->thresholds.clear();
	for (int i = 0; i < thresholds.size(); i++)
		this->thresholds.push_back(Thresholds(thresholds[i].minCurrent, thresholds[i].maxCurrent, thresholds[i].minVoltage, thresholds[i].maxVoltage));
}