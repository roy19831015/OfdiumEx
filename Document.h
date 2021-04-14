#pragma once
#include "Definitions.h"
#include <vector>
#include "Page.h"
#include "Res.h"
#include "Signatures.h"
struct Print 
{
	bool printable;
	int copies = -1;
};

struct ValidPeriod
{
	time_t startDate;
	time_t endDate;
};

struct CT_Permission
{
	bool edit = true;
	bool annot = true;
	bool _export = true;
	bool signature = true;
	bool watermark = true;
	bool printScreen = true;
	Print *print = nullptr;
	ValidPeriod *validPeriod = nullptr;
};

struct CT_VPreferences
{
	enum PAGEMODE
	{
		NONE, FULLSCREEN, USEOUTLINES, USETHUMBS, USECUSTOMTAGS, USELAYERS, USEATTATCHS, USEBOOKMARKS
	};
	
	enum PAGELAYOUT
	{
		ONEPAGE, ONECOLUMN, TWOPAGEL, TWOCOLUMNL, TWOPAGER, TWOCOLUMNR
	};
	
	enum TABDISPLAY
	{
		DOCTITLE, FILENAME
	};
	
	enum ZOOMMODE
	{
		DEFAULT, FITHEIGHT, FITWIDTH, FITRECT
	};
	union ZoomData
	{
		ZOOMMODE zoomMode;
		double zoom;
	};
	PAGEMODE pageMode = NONE;
	PAGELAYOUT pageLayout = ONECOLUMN;
	TABDISPLAY tabDisplay = DOCTITLE;
	bool hideToolbar = false;
	bool hideMenubar = false;
	bool hideWindowUI = false;
	ZoomData zoomData;
};

struct CT_OutlineElem
{
	std::vector<CT_Action*> actions;
	std::vector<CT_OutlineElem *> outlineElems;
	std::string title;
	int count;
	bool expanded = true;
};

struct CT_Bookmark
{
	CT_Dest *dest = nullptr;
	std::string name;
};

struct TemplatePage;

struct CommonData
{
	ST_ID maxUnitID;
	CT_PageArea *pageArea = nullptr;
	std::vector<std::pair<ST_Loc*, Res *>> publicRes;
	std::vector<std::pair<ST_Loc*, Res *>> documentRes;
	std::vector<std::pair<TemplatePage *, Page *>> templatePages;
	ST_RefID defaultCS;
};

struct TemplatePage
{
	ST_ID id;
	std::string name;
	enum ZORDER
	{
		BACKGROUND, FOREGROUND
	};
	ZORDER zorder;
	ST_Loc *baseLoc = nullptr;
};
struct Document;
struct Document_Page
{
	int pageIndex = -1;
	Document *fromDoc = nullptr;
	ST_ID id;
	ST_Loc *baseLoc = nullptr;
	ST_Loc *loc = nullptr;
};

typedef std::vector<std::pair<Document_Page*, Page*>> Pages;
struct OFD;
struct Document
{
	int docIndex = -1;
	OFD *fromOfd = nullptr;
	CommonData *commonData = nullptr;
	Pages pages;
	std::vector<CT_OutlineElem *> outlines;
	CT_Permission *permissions = nullptr;
	CT_Action *actions = nullptr;
	CT_VPreferences *vPreferences = nullptr;
	std::vector<CT_Bookmark *> bookmarks;
	ST_Loc *annotations = nullptr;
	ST_Loc *customTags = nullptr;
	ST_Loc *attachments = nullptr;
	ST_Loc *extensions = nullptr;

	Signatures *signatures = nullptr;
};