#pragma once
#include <string>
#include <vector>
#include "Definitions.h"
#include "Document.h"
#include <map>
#include "Signatures.h"
struct CustomData
{
	std::string name;
	std::string data;
};

struct CT_DocInfo
{
	std::string DocID;
	std::string title;
	std::string author;
	std::string subject;
	std::string abstract;
	time_t creationDate;
	time_t modDate;
	std::string docUsage;
	ST_Loc *cover = nullptr;
	std::vector<std::string> keywords;
	std::string creator;
	std::string creatorVersion;
	std::vector<CustomData *> CustomDatas;
};

struct Version
{
	std::string id;
	int index;
	bool current = false;
	ST_Loc *baseLoc = nullptr;
};

struct DocBody
{
	CT_DocInfo *docInfo = nullptr;
	ST_Loc *docRoot = nullptr;
	std::vector<Version *> versions;
	ST_Loc *Signatures = nullptr;
};

struct OFD
{
	std::vector<DocBody *> docBodies;
	std::string version = "1.0";
	std::string docType = "OFD";
	ST_Loc ofdRoot;
	//userDef
	std::string root_path = "";
	std::vector<Document *> documents;
	union DataA
	{
		CT_ColorSpace *colorSpace = nullptr;
		CT_DrawParam *drawParam;
		CT_Font *font;
		CT_MultiMedia *multiMedia;
		CT_VectorG *vectorG;
	};
	std::map<ST_ID, std::pair<Res::TYPE, OFD::DataA*>> *resAll;
};

