#pragma once
#include <string>
#include <time.h>
#include <comutil.h>
#include "Definitions.h"
#include <vector>

struct Property
{
	std::string name;
	std::string type;
	std::string value;
};

struct CT_Extension
{
	enum TYPE
	{
		PROPERTY, DATA, EXTENDDATA
	};
	union Data
	{
		Property *property;
		VARIANT *data;
		ST_Loc *extendData = nullptr;
	};
	std::vector<std::pair<TYPE, Data>> d;
	std::string appName;
	std::string company;
	std::string appVersion;
	std::string date;
	ST_RefID RefId;
};

using Extensions = std::vector<CT_Extension>;