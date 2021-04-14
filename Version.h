#pragma once
#include "Definitions.h"
#include <vector>

using idFile = std::pair<std::string, ST_Loc*>;
using FileList = std::vector<idFile>;

struct DocVersion
{
	FileList fileList;
	ST_Loc *docRoot = nullptr;
	std::string id;
	std::string version;
	std::string name;
	time_t creationDate;
};
