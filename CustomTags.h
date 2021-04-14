#pragma once
#include <string>
#include <time.h>
#include "Definitions.h"
struct CustomTag
{
	ST_Loc *schemaLoc = nullptr;
	ST_Loc *fileLoc = nullptr;
	std::string nameSpace;
};

using CustomTags = std::vector<CustomTag *>;