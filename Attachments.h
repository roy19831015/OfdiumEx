#pragma once
#include <string>
#include <time.h>
#include "Definitions.h"
struct CT_Attachment
{
	ST_Loc *fileLoc = nullptr;
	std::string iD;
	std::string name;
	std::string format;
	time_t creationDate;
	time_t modDate;
	double size;
	bool visible = true;
	std::string usage = "none";
};

using Attachments = std::vector<CT_Attachment *>;