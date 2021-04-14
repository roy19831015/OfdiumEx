#pragma once
#include <string>
#include <vector>

using ST_ID = unsigned int;
using ST_RefID = unsigned int;
struct ST_Loc
{
	std::string pln;
	std::string abs;
};
using ST_Array = std::string;
using ST_Array_Delta = std::vector<double>;
using ST_Pos = std::string;

struct ST_Box
{
	double x;
	double y;
	double w;
	double h;
};

struct CT_Matrix {
	double xx;
	double yx;
	double xy;
	double yy;
	double x0;
	double y0;
};

struct CT_Dest
{
	enum TYPE
	{
		XYZ, FIT, FITH, FITV, FITR
	};
	TYPE type;
	ST_RefID pageID;
	double left;
	double top;
	double right;
	double bottom;
	double zoom;
};

struct CT_PageArea
{
	ST_Box *physicalBox = nullptr;
	ST_Box *applicationBox = nullptr;
	ST_Box *contentBox = nullptr;
	ST_Box *bleedBox = nullptr;
};

struct Bookmark
{
	std::string name;
};

struct Goto
{
	enum TYPE
	{
		DEST, BOOKMARK
	};
	TYPE type;
	union
	{
		CT_Dest *dest = nullptr;
		Bookmark *bookmark;
	}d;
};

struct Uri
{
	std::string uri;
	std::string base;
	std::string target;
};

struct GotoA
{
	std::string AttachID;
	bool NewWindow = true;
};

struct Sound 
{
	ST_RefID resourceID;
	int volume;
	bool repeat;
	bool synchronous;
};

struct Movie
{
	enum TYPE
	{
		PLAY, STOP, PAUSE, RESUME
	};
	ST_RefID resourceID;
	TYPE oper;
};

struct Move
{
	ST_Pos point1;
};

struct Line
{
	ST_Pos point1;
};

struct QuadraticBezier
{
	ST_Pos point1;
	ST_Pos point2;
};

struct CubicBezier
{
	ST_Pos point1;
	ST_Pos point2;
	ST_Pos point3;
};

struct _Arc
{
	bool sweepDirection;
	bool largeArc;
	double rotationAngle;
	ST_Array ellipseSize;
	ST_Pos endPoint;
};

struct Area
{
	enum TYPE
	{
		MOVE, LINE, QUADRATICBEZIER, CUBICBEZIER, ARC, CLOSE
	}; 
	union Data
	{
		Move *move = nullptr;
		Line *line;
		QuadraticBezier *quadraticBezier;
		CubicBezier *cubicBezier;
		_Arc *arc;
	};
	std::vector<std::pair<TYPE, Data*>> d;
	ST_Pos start;
};

using CT_Region = std::vector<Area*>;

struct CT_Action
{
	enum TYPE
	{
		GOTO, URI, GOTOA, SOUND, MOVIE
	};
	enum EVENT
	{
		DO, PO, CLICK

	};

	CT_Region region;
	TYPE type;
	union
	{
		Goto *_goto = nullptr;
		Uri *uri;
		GotoA *gotoaA;
		Sound *sound;
		Movie *movie;
		EVENT event;
	}d;
};