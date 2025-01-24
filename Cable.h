#pragma once

#include "Components.h"

class Cable
{
public:
	Cable();
	Cable(ConnectorId connector, int pin);
	Cable(const Cable& cable);

	void setId(int id) { this->id = id; }
	void setConnector(ConnectorId connector) { this->connector = connector; }
	void setPin(int pin) { this->pin = pin; }
	void setDirection(int direction) { this->direction = direction; }
	void setType(int type) { this->type = type; }
	void setCanId(int canId) { this->canId = canId; }
	void setBit(int bit) { this->bit = bit; }
	void setMinCurrent(double minCurrent) { this->minCurrent = minCurrent; }
	void setMaxCurrent(double maxCurrent) { this->maxCurrent = maxCurrent; }
	void setMinVoltage(double minVoltage) { this->minVoltage = minVoltage; }
	void setMaxVoltage(double maxVoltage) { this->maxVoltage = maxVoltage; }
	void setName(QString name) { this->name = name; }
	void setComponent(QString component) { this->component = component; }

	int	getId() { return this->id; }
	ConnectorId getConnector() { return this->connector; }
	int	getPin() { return this->pin; }
	int	getDirection() { return this->direction; }
	int getType() { return this->type; }
	int getCanId() { return this->canId; }
	int getBit() { return this->bit; }
	double getMinCurrent() { return this->minCurrent; }
	double getMaxCurrent() { return this->maxCurrent; }
	double getMinVoltage() { return this->minVoltage; }
	double getMaxVoltage() { return this->maxVoltage; }
	QString getName() { return this->name; }
	QString getComponent() { return this->component; }

private:
	int id;
	ConnectorId connector;
	int pin;
	int direction;
	int type;
	int canId;
	int bit;
	double minCurrent;
	double maxCurrent;
	double minVoltage;
	double maxVoltage;
	QString name;
	QString component;
};

