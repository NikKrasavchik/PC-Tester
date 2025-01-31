#pragma once

#include <vector>

#include "Components.h"

struct Thresholds
{
	double minCurrent;
	double maxCurrent;
	double minVoltage;
	double maxVoltage;

	Thresholds()
	{
		minCurrent = -1;
		maxCurrent = -1;
		minVoltage = -1;
		maxVoltage = -1;
	}

	Thresholds(double minCurrent, double maxCurrent, double minVoltage, double maxVoltage)
	{
		this->minCurrent = minCurrent;
		this->maxCurrent = maxCurrent;
		this->minVoltage = minVoltage;
		this->maxVoltage = maxVoltage;
	}

	Thresholds(const Thresholds& thresholds)
	{
		this->minCurrent = thresholds.minCurrent;
		this->maxCurrent = thresholds.maxCurrent;
		this->minVoltage = thresholds.minVoltage;
		this->maxVoltage = thresholds.maxVoltage;
	}
};

class Cable
{
public:
	Cable();
	Cable(ConnectorId connector, int pin);
	Cable(const Cable& cable);

	void setId(int id)							{ this->id = id; }
	void setConnector(ConnectorId connector)	{ this->connector = connector; }
	void setPin(int pin)						{ this->pin = pin; }
	void setDirection(int direction)			{ this->direction = direction; }
	void setType(int type)						{ this->type = type; }
	void setCanId(int canId)					{ this->canId = canId; }
	void setBit(int bit)						{ this->bit = bit; }
	void setThresholds(std::vector<Thresholds> thresholds);
	void setName(QString name)					{ this->name = name; }
	void setComponent(QString component)		{ this->component = component; }

	int	getId()									{ return this->id; }
	ConnectorId getConnector()					{ return this->connector; }
	int	getPin()								{ return this->pin; }
	int	getDirection()							{ return this->direction; }
	int getType()								{ return this->type; }
	int getCanId()								{ return this->canId; }
	int getBit()								{ return this->bit; }
	std::vector<Thresholds> getThresholds()		{ return this->thresholds; }
	QString getName()							{ return this->name; }
	QString getComponent()						{ return this->component; }

private:
	int id;
	ConnectorId connector;
	int pin;
	int direction;
	int type;
	int canId;
	int bit;
	std::vector<Thresholds> thresholds;
	QString name;
	QString component;
};
