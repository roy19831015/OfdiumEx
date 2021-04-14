#pragma once
#include <vector>
#include "Definitions.h"

struct Page
{
	ST_Loc *fileLoc = nullptr;
	ST_RefID pageID;
};

using Annotations = std::vector<Page *>;