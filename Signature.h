#pragma once
#include "Definitions.h"
#include <string>
#include <map>

struct Seal
{
	ST_Loc *baseLoc = nullptr;
};

struct Signature;
struct StampAnnot
{
	Signature *fromSignature;
	std::string id;
	ST_RefID pageRef;
	//int pageIndex;
	ST_Box *boundary = nullptr;
	ST_Box *clip = nullptr;
};

struct References
{
	enum CHECKMETHOD
	{
		MD5, SHA1, SM3
	};
	CHECKMETHOD checkMethod = MD5;
	std::map<ST_Loc*, std::string> *references = nullptr;
};

struct Provider
{
	std::string providerName;
	std::string version;
	std::string company;
};

struct SignedInfo
{
	Provider *provider = nullptr;
	std::string signatureMethod;
	std::string signatureDateTime;
	References *references = nullptr;
	std::vector<StampAnnot *> stampAnnots;
	Seal *seal = nullptr;
};

struct SignedValue
{
	size_t len;
	unsigned char *val = nullptr;
};

struct Signature
{
	Document *fromDoc = nullptr;
	SignedInfo *signedInfo = nullptr;
	std::pair<ST_Loc*, SignedValue*> signedValue;
};