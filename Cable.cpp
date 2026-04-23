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
	this->thresholdsManual.push_back(Thresholds());
	this->thresholdsAuto.push_back(Thresholds());
	this->measureds.push_back(Measureds());
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
	this->thresholdsManual.push_back(Thresholds());
	this->thresholdsAuto.push_back(Thresholds());
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
	for (int i = 0; i < cable.thresholdsManual.size(); i++)
		this->thresholdsManual.push_back(Thresholds(cable.thresholdsManual[i]));
	for (int i = 0; i < cable.thresholdsAuto.size(); i++)
		this->thresholdsAuto.push_back(Thresholds(cable.thresholdsAuto[i]));
	for (int i = 0; i < cable.measureds.size(); i++)
		this->measureds.push_back(Measureds(cable.measureds[i]));
	this->name = cable.name;
	this->component = cable.component;
}

void Cable::setThresholdsManual(std::vector<Thresholds> thresholdsManual)
{
	this->thresholdsManual.clear();
	for (int i = 0; i < thresholdsManual.size(); i++)
		this->thresholdsManual.push_back(Thresholds(thresholdsManual[i]));
}

void Cable::setThresholdsAuto(std::vector<Thresholds> thresholdsAuto)
{
	this->thresholdsAuto.clear();
	for (int i = 0; i < thresholdsAuto.size(); i++)
		this->thresholdsAuto.push_back(Thresholds(thresholdsAuto[i]));
}

void Cable::setMeasureds(std::vector<Measureds> measureds)
{
	this->measureds.clear();
	for (int i = 0; i < measureds.size(); i++)
		this->measureds.push_back(Measureds(measureds[i].voltage, measureds[i].current));
}