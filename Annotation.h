#pragma once
#include <vector>
#include <string>
#include <map>
#include "Definitions.h"
#include "Page.h"

typedef std::map<std::string, std::string> Parameters;
struct Appearance
{
	ST_Box *boundary = nullptr;
	CT_PageBlock *ct_pageblock = nullptr;
};

struct Annot
{
	enum TYPE
	{
		LINK, PATH, HIGHLIGHT, STAMP, WATERMARK
	};
	std::string remark;
	Parameters parameters;
	Appearance *appearance = nullptr;
	ST_ID id;
	TYPE type;
	std::string creator;
	time_t lastModDate;
	bool visible = true;
	std::string subtype;
	bool print = true;
	bool noZoom = false;
	bool noRotate = false;
	bool readOnly = true;
};

typedef std::vector<Annot> PageAnnot;
