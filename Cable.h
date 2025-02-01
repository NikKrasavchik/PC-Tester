#pragma once

#include <vector>

#include "Components.h"

struct Thresholds
{
	double minCurrent;
	double maxCurrent;
	double minVoltage;
	double maxVoltage;

	double minValue;
	double maxValue;

	Thresholds()
	{
		minCurrent = -1;
		maxCurrent = -1;
		minVoltage = -1;
		maxVoltage = -1;
		minValue = -1;
		maxValue = -1;
	}

	Thresholds(double minValue, double maxValue)
	{
		this->minValue = minValue;
		this->maxValue = maxValue;
		this->minCurrent = -1;
		this->maxCurrent = -1;
		this->minVoltage = -1;
		this->maxVoltage = -1;
	}

	Thresholds(double minCurrent, double maxCurrent, double minVoltage, double maxVoltage)
	{
		this->minCurrent = minCurrent;
		this->maxCurrent = maxCurrent;
		this->minVoltage = minVoltage;
		this->maxVoltage = maxVoltage;
		this->minValue = -1;
		this->maxValue = -1;
	}

	Thresholds(const Thresholds& thresholds)
	{
		this->minCurrent = thresholds.minCurrent;
		this->maxCurrent = thresholds.maxCurrent;
		this->minVoltage = thresholds.minVoltage;
		this->maxVoltage = thresholds.maxVoltage;
		this->minValue = thresholds.minValue;
		this->maxValue = thresholds.maxValue;
	}
};

struct Measureds
{
	double current;
	double voltage;

	Measureds()
	{
		current = NOT_SET;
		voltage = NOT_SET;
	}

	Measureds(double voltage, double current)
	{
		this->voltage = voltage;
		this->current = current;
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
	void setMeasureds(std::vector<Measureds> measureds);
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
	std::vector<Measureds> getMeasureds()		{ return this->measureds; }
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
	std::vector<Measureds> measureds;
	QString name;
	QString component;
};
