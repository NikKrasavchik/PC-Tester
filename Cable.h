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
		minVoltage = -1;
		maxVoltage = -1;
		minCurrent = -1;
		maxCurrent = -1;
		minValue = -1;
		maxValue = -1;
	}

	Thresholds(double minValue, double maxValue)
	{
		this->minValue = minValue;
		this->maxValue = maxValue;
		this->minVoltage = -1;
		this->maxVoltage = -1;
		this->minCurrent = -1;
		this->maxCurrent = -1;
	}

	Thresholds( double minVoltage, double maxVoltage, double minCurrent, double maxCurrent)
	{
		this->minVoltage = minVoltage;
		this->maxVoltage = maxVoltage;
		this->minCurrent = minCurrent;
		this->maxCurrent = maxCurrent;
		this->minValue = -1;
		this->maxValue = -1;
	}

	Thresholds(double minVoltage, double maxVoltage, double minCurrent, double maxCurrent, int minValue, int maxValue)
	{
		this->minVoltage = minVoltage;
		this->maxVoltage = maxVoltage;
		this->minCurrent = minCurrent;
		this->maxCurrent = maxCurrent;
		this->minValue = minValue;
		this->maxValue = maxValue;
	}

	Thresholds(const Thresholds& thresholds)
	{
		this->minVoltage = thresholds.minVoltage;
		this->maxVoltage = thresholds.maxVoltage;
		this->minCurrent = thresholds.minCurrent;
		this->maxCurrent = thresholds.maxCurrent;
		this->minValue = thresholds.minValue;
		this->maxValue = thresholds.maxValue;
	}
};

struct Measureds
{
	double voltage;
	double current;
	int digValue;

	Measureds()
	{
		voltage = NOT_SET;
		current = NOT_SET;
		digValue = NOT_SET;
	}

	Measureds(double voltage, double current)
	{
		this->voltage = voltage;
		this->current = current;
		this->digValue = -1;
	}

	Measureds(double voltage, double current, int digValue)
	{
		this->voltage = voltage;
		this->current = current;
		this->digValue = digValue;
	}

	Measureds(int digValue)
	{
		this->voltage = -1;
		this->current = -1;
		this->digValue = digValue;
	}

	Measureds(const Measureds& measureds)
	{
		this->voltage = measureds.voltage;
		this->current = measureds.current;
		this->digValue = measureds.digValue;
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
	void setThresholdsManual(std::vector<Thresholds> thresholdsManual);
	void setThresholdsAuto(std::vector<Thresholds> thresholdsAuto);
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
	std::vector<Thresholds> getThresholdsManual()		{ return this->thresholdsManual; }
	std::vector<Thresholds> getThresholdsAuto()		{ return this->thresholdsAuto; }
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
	std::vector<Thresholds> thresholdsManual;
	std::vector<Thresholds> thresholdsAuto;
	std::vector<Measureds> measureds;
	QString name;
	QString component;
};
