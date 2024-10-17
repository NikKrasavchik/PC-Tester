#pragma once

#include <vector>

#include "qstring.h"
#include "canlib.h"
#include "chai.h"

class Can
{
public:
	Can();

	void initCan(int frequency);

	std::vector<QString> getNameAdapters();

	canHandle hndTmp;
private:
};
