#pragma once
#include "Definitions.h"
#include <string>
#include <vector>
#include "Signature.h"
struct Signatures_Signature
{
	enum TYPE
	{
		SEAL, SIGN
	};
	std::string id;
	TYPE type;
	ST_Loc *baseLoc = nullptr;
	//user-define
	bool notInXML = false;
};

struct Signatures
{
	std::string maxSignId;
	std::vector<std::pair<Signatures_Signature *, Signature *>> signatures;
};

