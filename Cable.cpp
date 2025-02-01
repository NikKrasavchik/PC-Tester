#include "Cable.h"

Cable::Cable()
{
	this->id = NOT_SET;
	this->connector = ConnectorId::EMPTY;
	this->pin = NOT_SET;
	this->direction = NOT_SET;
	this->type = NOT_SET;
	this->canId = NOT_SET;
	this->bit = NOT_SET;
	this->thresholds.push_back(Thresholds());
	this->name = "";
	this->component = "";
}

Cable::Cable(ConnectorId connector, int pin)
{
	this->id = NOT_SET;
	this->connector = connector;
	this->pin = pin;
	this->direction = NOT_SET;
	this->type = NOT_SET;
	this->canId = NOT_SET;
	this->bit = NOT_SET;
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

void Cable::setMeasureds(std::vector<Measureds> measureds)
{
	this->measureds.clear();
	for (int i = 0; i < measureds.size(); i++)
		this->measureds.push_back(Measureds(measureds[i].voltage, measureds[i].current));
}