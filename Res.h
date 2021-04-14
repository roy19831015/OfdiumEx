#pragma once
#include "Definitions.h"
#include "Page.h"
#include <vector>

struct CT_ColorSpace
{
	enum TYPE
	{
		GRAY, RGB, CMYK
	};
	TYPE type;
	int bitsPerComponent = 8;
	ST_Loc *Profile = nullptr;
	std::vector<ST_Array> Palette;
};

struct CT_DrawParam
{
	CT_Color *fillColor = nullptr;
	CT_Color *strokeColor = nullptr;
	ST_RefID relative;
	double lineWidth = 0.353;
	std::string	join = "Miter";
	std::string	cap = "Butt";
	double dashOffset = 0.0;
	ST_Array dashPattern;
	double miterLimit = 4.234;
};

struct CT_Font
{
	ST_ID id;
	enum CHARSET
	{
		SYMBOL, PRC, BIG5, SHIFT_JIS, WANSUNG, JOHAB, __UNICODE
	};
	ST_Loc *fontFile = nullptr;
	std::string fontName;
	std::string familyName;
	CHARSET charset = __UNICODE;
	bool italic = false;
	bool bold = false;
	bool serif = false;
	bool fixedWidth = false;
};

struct CT_MultiMedia
{
	enum TYPE
	{
		IMAGE, AUDIO, VIDEO
	};
	TYPE type;
	ST_Loc *MediaFile = nullptr;
	std::string format;
};

struct CT_VectorG
{
	ST_RefID thumbnail;
	ST_RefID substitution;
	CT_PageBlock *content = nullptr;
	double Width;
	double Height;
};

using ColorSpace = std::pair<ST_ID, CT_ColorSpace*>;
typedef std::vector<ColorSpace> ColorSpaces;
using DrawParam = std::pair<ST_ID, CT_DrawParam*>;
typedef std::vector<DrawParam> DrawParams;
using Font = std::pair<ST_ID, CT_Font*>;
typedef std::vector<Font> Fonts;
using MultiMedia = std::pair<ST_ID, CT_MultiMedia*>;
typedef std::vector<MultiMedia> MultiMedias;
using CompositeGraphicUnit = std::pair<ST_ID, CT_VectorG*>;
typedef std::vector<CompositeGraphicUnit> CompositeGraphicUnits;

struct Res
{
	enum TYPE
	{
		COLORSPACES, DRAWPARAMS, FONTS, MULTIMEDIAS, COMPOSITEGRAPHICUNITS,
	};
	union Data
	{
		ColorSpaces *colorSpaces = nullptr;
		DrawParams *drawParams;
		Fonts *fonts;
		MultiMedias *multiMedias;
		CompositeGraphicUnits *compositeGraphicUnits;
	};
	std::vector<std::pair<TYPE, Data*>> resData;
	ST_Loc *baseLoc = nullptr;
};
