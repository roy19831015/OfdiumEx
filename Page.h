#pragma once
#include <vector>
#include "Definitions.h"
#include "cairo.h"

struct CT_Layer;

struct Content
{
	std::vector<std::pair<ST_ID, CT_Layer*>> layers;
};

struct Template
{
	ST_RefID templateID;
	enum ZORDER
	{
		BACKGROUND, FOREGROUND
	};
	ZORDER zOrder;
};
struct Document;
struct Page
{
	int pageIndex = -1;
	Document *fromDoc = nullptr;
	std::vector<Template*> templates;
	std::vector<ST_Loc*> pageRes;
	CT_PageArea *area = nullptr;
	Content *content = nullptr;
	std::vector<CT_Action*> actions;
};

struct CT_PageBlock;

struct CT_Layer
{
	enum TYPE
	{
		BACKGROUND, BODY, FOREGROUND, CUSTOM
	};
	TYPE type = BODY;
	CT_PageBlock *ct_pageBlock = nullptr;
	ST_RefID drawParam;
};

struct CT_Text;
struct TextObject;
struct PathObject;
struct ImageObject;
struct CompositeObject;
struct PageBlock;

struct CT_PageBlock
{
	enum TYPE
	{
		TEXTOBJECT, PATHOBJECT, IMAGEOBJECT, COMPOSITEOBJECT, PAGEBLOCK
	};
	union Data
	{
		TextObject *textObject = nullptr;
		PathObject *pathObject;
		ImageObject *imageObject;
		CompositeObject *compositeObject;
		PageBlock *pageBlock;
	};
	std::vector<std::pair<TYPE, Data*>> d;
};

struct CT_Path;

struct Page_Area
{
	enum TYPE
	{
		PATH, TEXT
	};
	TYPE type;
	union
	{
		CT_Path *ct_path = nullptr;
		CT_Text *ct_text;
	}d;
	ST_RefID drawParam;
	ST_Array cTM;
};

typedef std::vector<Area*> CT_Clip;

struct CT_GraphicUnit
{
	std::vector<CT_Action*> actions;
	std::vector<CT_Clip*> clips;
	ST_Box *boundary = nullptr;
	std::string name;
	bool visible = true;
	CT_Matrix *cTM = nullptr;
	ST_RefID drawParam;
	double lineWidth = 0.353;
	enum CAP
	{
		BUTT, ROUND, SQUARE
	};
	CAP cap = BUTT;
	enum JOIN
	{
		MITER, _ROUND, BEVEL
	};
	JOIN join = MITER;
	double miterLimit = 4.234;
	double DashOffset = 0.0;
	ST_Array dashPattern;
	int alpha = 255;
};

struct TextCode
{
	std::string str;
	double x;
	double y;
	ST_Array_Delta deltaX;
	ST_Array_Delta deltaY;
};

struct CT_CGTransform;

struct TextCode_And_CT_CGTransform
{
	std::vector<CT_CGTransform*> ct_cGTransforms;
	TextCode *textCode = nullptr;
};

struct CT_Color;

struct CT_Text
{
	CT_GraphicUnit *ct_graphicUnit = nullptr;
	CT_Color *strokeColor = nullptr;
	CT_Color *fillColor = nullptr;
	std::vector<TextCode_And_CT_CGTransform*> textcode_and_ct_cgtransforms;
	ST_RefID font;
	double size;
	bool stroke = false;
	bool fill = true;
	double hScale = 1.0;
	int readDirection = 0;
	int charDirection = 0;
	int weight = 400;
	bool italic = false;
};

struct CT_CGTransform
{
	ST_Array glyphs;
	int codePosition;
	int codeCount = 1;
	int glyphCount = 1;
};

struct Border
{
	CT_Color *BorderColor = nullptr;
	double lineWidth = 0.353;
	double horizonalCornerRadius = 0;
	double verticalCornerRadius = 0;
	double dashOffset = 0;
	ST_Array dashPattern;
};

struct CT_Image
{
	CT_GraphicUnit *ct_graphicUnit = nullptr;
	Border *border = nullptr;
	ST_RefID resourceID;
	ST_RefID substitution;
	ST_RefID imageMask;
};

struct CT_Composite
{
	CT_GraphicUnit *ct_graphicUnit = nullptr;
	ST_RefID resourceID;
};

struct AbbreviatedData
{
	std::string cMd;
	std::vector<double> params;
};

struct CT_Path
{
	CT_GraphicUnit *ct_graphicUnit = nullptr;
	CT_Color *strokeColor = nullptr;
	CT_Color *fillColor = nullptr;
	std::vector<AbbreviatedData *> abbreviatedData;
	bool stroke = true;
	bool fill = true;
	enum RULE
	{
		NONZERO, EVEN_ODD
	};
	RULE rule = NONZERO;
};

struct TextObject
{
	CT_Text *ct_text = nullptr;
	ST_ID id;
};

struct PathObject
{
	CT_Path *ct_path = nullptr;
	ST_ID id;
};

struct ImageObject
{
	CT_Image *ct_imageObject = nullptr;
	ST_ID id;
};

struct CompositeObject
{
	CT_Composite *ct_compositeObject = nullptr;
	ST_ID id;
};

struct PageBlock
{
	CT_PageBlock *ct_pageBlock = nullptr;
	ST_ID id;
};

struct Segment
{
	CT_Color *color = nullptr;
	double position;
};

struct CellContent
{
	CT_PageBlock *ct_pageBlock = nullptr;
	ST_RefID Thumbnail;
};

struct CT_Pattern
{
	CellContent *cellContent = nullptr;
	double width;
	double height;
	double xStep;
	double yStep;
	enum REFLECTMETHOD
	{
		NORMAL, ROW, COLUMN, ROWANDCOLUMN
	};
	REFLECTMETHOD reflectMethod = NORMAL;
	enum RELATIVETO
	{
		PAGE, OBJECT
	};
	RELATIVETO relativeTo = OBJECT;
	ST_Array cTM;
};

struct CT_AxialShd
{
	enum MAPTYPE
	{
		DIRECT, REPEAT, REFLECT
	};
	std::vector<Segment*> segments;
	MAPTYPE mapType = DIRECT;
	double MapUnit;
	int extend = 0;
	ST_Pos startPoint;
	ST_Pos endPoint;
	
};

struct CT_RadialShd
{
	enum MAPTYPE
	{
		DIRECT, REPEAT, REFLECT
	};
	std::vector<Segment*> segments;
	MAPTYPE mapType = DIRECT;
	double MapUnit;
	double eccentricity = 0.0;
	double angle = 0.0;
	ST_Pos startPoint;
	double startRadius = 0.0;
	ST_Pos endPoint;
	double endRadius;
	int extend = 0;
};


struct CT_LaGouraudShd
{
	struct Point
	{
		CT_Color *color = nullptr;
		double x;
		double y;
	};
	std::vector<Point*> points;
	CT_Color *backColor = nullptr;
	int verticesPerRow;
	int extend;
};

struct CT_GouraudShd
{
	struct Point
	{
		CT_Color *color = nullptr;
		double x;
		double y;
		int edgeFlag;
	};
	std::vector<Point*> points;
	CT_Color *backColor = nullptr;
	int extend;
};

struct CT_Color
{
	enum TYPE
	{
		PATTERN, AXIALSHD, RADIALSHD, GOURAUDSHD, LAGOURANDSHD
	};
	TYPE type;
	union
	{
		CT_Pattern *pattern = nullptr;
		CT_AxialShd *axialShd;
		CT_RadialShd *radialShd;
		CT_GouraudShd *gouraudShd;
		CT_LaGouraudShd *laGouraudShd;
	}d;
	double valueR = 0.0f;
	double valueG = 0.0f;
	double valueB = 0.0f;
	int index;
	ST_RefID colorSpace;
	double alpha = 1.0f;
};
