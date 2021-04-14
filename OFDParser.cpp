#pragma once
#include "stdafx.h"
#include "OFDParser.h"
#include "util.h"
#include "Definitions.h"
#include "Res.h"
#include <math.h>
#include <algorithm>
#include <ShlObj.h>
#include <fstream>

inline void ST_Loc_free(ST_Loc *loc)
{
	if (!loc)
	{
		return;
	}
	loc->abs.clear();
	loc->abs.shrink_to_fit();
	loc->pln.clear();
	loc->pln.shrink_to_fit();
	delete loc;
	loc = nullptr;
}

inline void CustomData_free(CustomData *customdata)
{
	if (!customdata)
	{
		return;
	}
	customdata->data.clear();
	customdata->data.shrink_to_fit();
	customdata->name.clear();
	customdata->name.shrink_to_fit();
	delete customdata;
	customdata = nullptr;
}

inline void CT_DocInfo_free(CT_DocInfo *ct_docinfo)
{
	if (!ct_docinfo)
	{
		return;
	}
	ct_docinfo->abstract.clear();
	ct_docinfo->abstract.shrink_to_fit();
	ct_docinfo->author.clear();
	ct_docinfo->author.shrink_to_fit();
	ST_Loc_free(ct_docinfo->cover);
	ct_docinfo->creator.clear();
	ct_docinfo->creator.shrink_to_fit();
	ct_docinfo->creatorVersion.clear();
	ct_docinfo->creatorVersion.shrink_to_fit();
	for (auto &customdata : ct_docinfo->CustomDatas)
	{
		CustomData_free(customdata);
	}
	ct_docinfo->CustomDatas.clear();
	ct_docinfo->CustomDatas.shrink_to_fit();
	ct_docinfo->DocID.clear();
	ct_docinfo->DocID.shrink_to_fit();
	ct_docinfo->docUsage.clear();
	ct_docinfo->docUsage.shrink_to_fit();
	ct_docinfo->keywords.clear();
	ct_docinfo->keywords.shrink_to_fit();
	ct_docinfo->subject.clear();
	ct_docinfo->subject.shrink_to_fit();
	ct_docinfo->title.clear();
	ct_docinfo->title.shrink_to_fit();
	delete ct_docinfo;
	ct_docinfo = nullptr;
}

inline void Version_free(Version *version)
{
	if (!version)
	{
		return;
	}
	ST_Loc_free(version->baseLoc);
	delete version;
	version = nullptr;
}

inline void DocBody_free(DocBody *docbody)
{
	if (!docbody)
	{
		return;
	}
	CT_DocInfo_free(docbody->docInfo);
	ST_Loc_free(docbody->docRoot);
	ST_Loc_free(docbody->Signatures);
	for (auto &version : docbody->versions)
	{
		Version_free(version);
	}
	docbody->versions.clear();
	docbody->versions.shrink_to_fit();
	delete docbody;
	docbody = nullptr;
}

inline void Bookmark_free(Bookmark * bookmark)
{
	if (bookmark)
	{
		return;
	}
	bookmark->name.clear();
	bookmark->name.shrink_to_fit();
	delete bookmark;
	bookmark = nullptr;
}

inline void Dest_free(CT_Dest * dest)
{
	if (dest)
	{
		return;
	}
	delete dest;
	dest = nullptr;
}

inline void Goto_free(Goto * _goto)
{
	if (_goto)
	{
		return;
	}
	switch (_goto->type)
	{
	case Goto::TYPE::BOOKMARK:
		Bookmark_free(_goto->d.bookmark);
		break;
	case Goto::TYPE::DEST:
		Dest_free(_goto->d.dest);
		break;
	default:
		break;
	}
	delete _goto;
	_goto = nullptr;
}

inline void Uri_free(Uri * uri)
{
	if (uri)
	{
		return;
	}
	uri->base.clear();
	uri->base.shrink_to_fit();
	uri->target.clear();
	uri->target.shrink_to_fit();
	uri->uri.clear();
	uri->uri.shrink_to_fit();
	delete uri;
	uri = nullptr;
}

inline void GotoA_free(GotoA * gotoaA)
{
	if (!gotoaA)
	{
		return;
	}
	gotoaA->AttachID.clear();
	gotoaA->AttachID.shrink_to_fit();
	delete gotoaA;
	gotoaA = nullptr;
}

inline void Sound_free(Sound * sound)
{
	if (sound)
	{
		return;
	}
	delete sound;
	sound = nullptr;
}

inline void Movie_free(Movie * movie)
{
	if (movie)
	{
		return;
	}
	delete movie;
	movie = nullptr;
}

inline void Move_free(Move * move)
{
	if (!move)
	{
		return;
	}
	move->point1.clear();
	move->point1.shrink_to_fit();
	delete move;
	move = nullptr;
}

inline void Line_free(Line * line)
{
	if (!line)
	{
		return;
	}
	line->point1.clear();
	line->point1.shrink_to_fit();
	delete line;
	line = nullptr;
}

inline void QuadraticBezier_free(QuadraticBezier * quadraticBezier)
{
	if (!quadraticBezier)
	{
		return;
	}
	quadraticBezier->point1.clear();
	quadraticBezier->point1.shrink_to_fit();
	quadraticBezier->point2.clear();
	quadraticBezier->point2.shrink_to_fit();
	delete quadraticBezier;
	quadraticBezier = nullptr;
}

inline void CubicBezier_free(CubicBezier * cubicBezier)
{
	if (!cubicBezier)
	{
		return;
	}
	cubicBezier->point1.clear();
	cubicBezier->point1.shrink_to_fit();
	cubicBezier->point2.clear();
	cubicBezier->point2.shrink_to_fit();
	cubicBezier->point3.clear();
	cubicBezier->point3.shrink_to_fit();
	delete cubicBezier;
	cubicBezier = nullptr;
}

inline void _Arc_free(_Arc * arc)
{
	if (!arc)
	{
		return;
	}
	arc->ellipseSize.clear();
	arc->ellipseSize.shrink_to_fit();
	arc->endPoint.clear();
	arc->endPoint.shrink_to_fit();
	delete arc;
	arc = nullptr;
}

inline void Area_free(Area * area)
{
	if (!area)
	{
		return;
	}
	for (auto &typedata:area->d)
	{
		switch (typedata.first)
		{
		case Area::MOVE:
			Move_free(typedata.second->move);
			break;
		case Area::LINE:
			Line_free(typedata.second->line);
			break;
		case Area::QUADRATICBEZIER:
			QuadraticBezier_free(typedata.second->quadraticBezier);
			break;
		case Area::CUBICBEZIER:
			CubicBezier_free(typedata.second->cubicBezier);
			break;
		case Area::ARC:
			_Arc_free(typedata.second->arc);
			break;
		case Area::CLOSE:
			break;
		default:
			break;
		}
		if (typedata.second)
		{
			delete typedata.second;
			typedata.second = NULL;
		}
	}
	area->start.clear();
	area->start.shrink_to_fit();
	delete area;
	area = nullptr;
}

inline void CT_Action_free(CT_Action * actions)
{
	if (!actions)
	{
		return;
	}
	switch (actions->type)
	{
	case CT_Action::TYPE::GOTO:
		Goto_free(actions->d._goto);
		break;
	case CT_Action::TYPE::URI:
		Uri_free(actions->d.uri);
		break;
	case CT_Action::TYPE::GOTOA:
		GotoA_free(actions->d.gotoaA);
		break;
	case CT_Action::TYPE::SOUND:
		Sound_free(actions->d.sound);
		break;
	case CT_Action::TYPE::MOVIE:
		Movie_free(actions->d.movie);
		break;
	default:
		break;
	}
	for (auto &area : actions->region)
	{
		Area_free(area);
	}
	actions->region.clear();
	actions->region.shrink_to_fit();
	delete actions;
	actions = nullptr;
}

inline void CT_Dest_free(CT_Dest * dest)
{
	if (!dest)
	{
		return;
	}
	delete dest;
	dest = nullptr;
}

inline void CT_Bookmark_free(CT_Bookmark * bm)
{
	if (!bm)
	{
		return;
	}
	CT_Dest_free(bm->dest);
	bm->name.clear();
	bm->name.shrink_to_fit();
	delete bm;
	bm = nullptr;
}

inline void ColorSpaces_free(ColorSpaces * colorSpaces)
{
	if (!colorSpaces)
	{
		return;
	}
	for (auto &colorspace : *colorSpaces)
	{
		CT_ColorSpace_free(colorspace.second);
	}
	delete colorSpaces;
	colorSpaces = nullptr;
}

inline void DrawParam_free(DrawParams * drawParams)
{
	if (!drawParams)
	{
		return;
	}
	for (auto &drawparam : *drawParams)
	{
		CT_DrawParam_free(drawparam.second);
	}
	delete drawParams;
	drawParams = nullptr;
}

inline void Fonts_free(Fonts * fonts)
{
	if (!fonts)
	{
		return;
	}
	for (auto &font : *fonts)
	{
		CT_Font_free(font.second);
	}
	delete fonts;
	fonts = nullptr;
}

inline void MultiMedias_free(MultiMedias * multiMedias)
{
	if (!multiMedias)
	{
		return;
	}
	for (auto &multimedia : *multiMedias)
	{
		CT_MultiMedia_free(multimedia.second);
	}
	delete multiMedias;
	multiMedias = nullptr;
}

inline void CompositeGraphicUnits_free(CompositeGraphicUnits * compositeGraphicUnits)
{
	if (!compositeGraphicUnits)
	{
		return;
	}
	for (auto &compositegraphicunits : *compositeGraphicUnits)
	{
		CT_VectorG_free(compositegraphicunits.second);
	}
	delete compositeGraphicUnits;
	compositeGraphicUnits = nullptr;
}

inline void Res_free(Res * res)
{
	if (!res)
	{
		return;
	}
	ST_Loc_free(res->baseLoc);
	for (auto &typedata : res->resData)
	{
		switch (typedata.first)
		{
		case Res::COLORSPACES:
			ColorSpaces_free(typedata.second->colorSpaces);
			break;
		case Res::DRAWPARAMS:
			DrawParam_free(typedata.second->drawParams);
			break;
		case Res::FONTS:
			Fonts_free(typedata.second->fonts);
			break;
		case Res::MULTIMEDIAS:
			MultiMedias_free(typedata.second->multiMedias);
			break;
		case Res::COMPOSITEGRAPHICUNITS:
			CompositeGraphicUnits_free(typedata.second->compositeGraphicUnits);
			break;
		default:
			break;
		}
		if (typedata.second)
		{
			delete typedata.second;
			typedata.second = NULL;
		}
	}
	res->resData.clear();
	res->resData.shrink_to_fit();
	delete res;
	res = nullptr;
}

inline void TemplatePage_free(TemplatePage * tplpage)
{
	if (!tplpage)
	{
		return;
	}
	ST_Loc_free(tplpage->baseLoc);
	tplpage->name.clear();
	tplpage->name.shrink_to_fit();
	delete tplpage;
	tplpage = nullptr;
}

inline void CommonData_free(CommonData * commonData)
{
	if (!commonData)
	{
		return;
	}
	for (auto &locRes : commonData->documentRes)
	{
		ST_Loc_free(locRes.first);
		Res_free(locRes.second);
	}
	commonData->documentRes.clear();
	commonData->documentRes.shrink_to_fit();
	CT_PageArea_free(commonData->pageArea);
	for (auto &locRes : commonData->publicRes)
	{
		ST_Loc_free(locRes.first);
		Res_free(locRes.second);
	}
	for (auto &tplpage : commonData->templatePages)
	{
		TemplatePage_free(tplpage.first);
		Page_free(tplpage.second);
	}
	commonData->templatePages.clear();
	commonData->templatePages.shrink_to_fit();
	delete commonData;
	commonData = nullptr;
}

inline void CT_OutlineElem_free(CT_OutlineElem * ol)
{
	if (!ol)
	{
		return;
	}
	for (auto &action : ol->actions)
	{
		CT_Action_free(action);
	}
	ol->actions.clear();
	ol->actions.shrink_to_fit();
	for (auto &el : ol->outlineElems)
	{
		CT_OutlineElem_free(el);
	}
	ol->outlineElems.clear();
	ol->outlineElems.shrink_to_fit();
	ol->title.clear();
	ol->title.shrink_to_fit();
	delete ol;
	ol = nullptr;
}

inline void Document_Page_free(Document_Page * docpage)
{
	if (!docpage)
	{
		return;
	}
	ST_Loc_free(docpage->baseLoc);
	docpage->fromDoc = nullptr;
	ST_Loc_free(docpage->loc);
	delete docpage;
	docpage = nullptr;
}

inline void CT_PageArea_free(CT_PageArea * area)
{
	if (!area)
	{
		return;
	}
	ST_Box_free(area->applicationBox);
	ST_Box_free(area->bleedBox);
	ST_Box_free(area->contentBox);
	ST_Box_free(area->physicalBox);
	delete area;
	area = nullptr;
}

inline void CT_Layer_free(CT_Layer *layer)
{
	if (!layer)
	{
		return;
	}
	CT_PageBlock_free(layer->ct_pageBlock);
	delete layer;
	layer = nullptr;
}

inline void Content_free(Content * content)
{
	if (!content)
	{
		return;
	}
	for (auto &layer : content->layers)
	{
		CT_Layer_free(layer.second);
	}
	content->layers.clear();
	content->layers.shrink_to_fit();
	delete content;
	content = nullptr;
}

inline void Page_free(Page * page)
{
	if (!page)
	{
		return;
	}
	for (auto &action : page->actions)
	{
		CT_Action_free(action);
	}
	page->actions.clear();
	page->actions.shrink_to_fit();
	CT_PageArea_free(page->area);
	Content_free(page->content);
	page->fromDoc = nullptr;
	for (auto &pageres : page->pageRes)
	{
		ST_Loc_free(pageres);
	}
	page->pageRes.clear();
	page->pageRes.shrink_to_fit();
	for (auto &tpl : page->templates)
	{
		if (!tpl)
		{
			continue;
		}
		delete tpl;
		tpl = NULL;
	}
	page->templates.clear();
	page->templates.shrink_to_fit();
	delete page;
	page = nullptr;
}

inline void Print_free(Print * print)
{
	if (!print)
	{
		return;
	}
	delete print;
	print = nullptr;
}

inline void ValidPeriod_free(ValidPeriod * validPeriod)
{
	if (!validPeriod)
	{
		return;
	}
	delete validPeriod;
	validPeriod = nullptr;
}

inline void CT_Permission_free(CT_Permission * permissions)
{
	if (!permissions)
	{
		return;
	}
	Print_free(permissions->print);
	ValidPeriod_free(permissions->validPeriod);
	delete permissions;
	permissions = nullptr;
}

inline void Signatures_Signature_free(Signatures_Signature * sign)
{
	if (!sign)
	{
		return;
	}
	ST_Loc_free(sign->baseLoc);
	sign->id.clear();
	sign->id.shrink_to_fit();
	delete sign;
	sign = nullptr;
}

inline void Provider_free(Provider * provider)
{
	if (!provider)
	{
		return;
	}
	provider->company.clear();
	provider->company.shrink_to_fit();
	provider->providerName.clear();
	provider->providerName.shrink_to_fit();
	provider->version.clear();
	provider->version.shrink_to_fit();
	delete provider;
	provider = nullptr;
}

inline void References_free(References * references)
{
	if (!references)
	{
		return;
	}
	for (auto &kv : *references->references)
	{
		ST_Loc_free(kv.first);
		kv.second.clear();
		kv.second.shrink_to_fit();
	}
	references->references->clear();
	delete references->references;
	if (references->references)
	{
		references->references = NULL;
	}
	delete references;
	references = nullptr;
}

inline void Seal_free(Seal * seal)
{
	if (!seal)
	{
		return;
	}
	ST_Loc_free(seal->baseLoc);
	delete seal;
	seal = nullptr;
}

inline void ST_Box_free(ST_Box *box)
{
	if (!box)
	{
		return;
	}
	delete box;
	box = nullptr;
}

inline void StampAnnot_free(StampAnnot * stampannot)
{
	if (!stampannot)
	{
		return;
	}
	ST_Box_free(stampannot->boundary);
	ST_Box_free(stampannot->clip);
	stampannot->id.clear();
	stampannot->id.shrink_to_fit();
	delete stampannot;
	stampannot = nullptr;
}

inline void SignedInfo_free(SignedInfo * signedInfo)
{
	if (!signedInfo)
	{
		return;
	}
	Provider_free(signedInfo->provider);
	References_free(signedInfo->references);
	Seal_free(signedInfo->seal);
	signedInfo->signatureDateTime.clear();
	signedInfo->signatureDateTime.shrink_to_fit();
	signedInfo->signatureMethod.clear();
	signedInfo->signatureMethod.shrink_to_fit();
	for (auto &stampannot : signedInfo->stampAnnots)
	{
		StampAnnot_free(stampannot);
	}
	signedInfo->stampAnnots.clear();
	signedInfo->stampAnnots.shrink_to_fit();
	delete signedInfo;
	signedInfo = nullptr;
}

inline void SignedValue_free(SignedValue * sv)
{
	if (!sv)
	{
		return;
	}
	if (sv->val)
	{
		delete sv->val;
		sv->val = nullptr;
	}
	delete sv;
	sv = nullptr;
}

inline void Signature_free(Signature * sign)
{
	if (!sign)
	{
		return;
	}
	SignedInfo_free(sign->signedInfo);
	ST_Loc_free(sign->signedValue.first);
	SignedValue_free(sign->signedValue.second);
	delete sign;
	sign = nullptr;
}

inline void Signatures_free(Signatures * signatures)
{
	if (!signatures)
	{
		return;
	}
	signatures->maxSignId.clear();
	signatures->maxSignId.shrink_to_fit();
	for (auto &signsign : signatures->signatures)
	{
		Signatures_Signature_free(signsign.first);
		Signature_free(signsign.second);
	}
	signatures->signatures.clear();
	signatures->signatures.shrink_to_fit();
	delete signatures;
	signatures = nullptr;
}

inline void CT_VPreferences_free(CT_VPreferences * vPreferences)
{
	if (!vPreferences)
	{
		return;
	}
	delete vPreferences;
	vPreferences = nullptr;
}

inline void Document_free(Document *doc)
{
	if (!doc)
	{
		return;
	}
	CT_Action_free(doc->actions);
	ST_Loc_free(doc->annotations);
	ST_Loc_free(doc->attachments);
	for (auto &bm : doc->bookmarks)
	{
		CT_Bookmark_free(bm);
	}
	doc->bookmarks.clear();
	doc->bookmarks.shrink_to_fit();
	CommonData_free(doc->commonData);
	ST_Loc_free(doc->customTags);
	ST_Loc_free(doc->extensions);
	doc->fromOfd = nullptr;
	for (auto &ol : doc->outlines)
	{
		CT_OutlineElem_free(ol);
	}
	doc->outlines.clear();
	doc->outlines.shrink_to_fit();
	for (auto &page : doc->pages)
	{
		Document_Page_free(page.first);
		Page_free(page.second);
	}
	doc->pages.clear();
	doc->pages.shrink_to_fit();
	CT_Permission_free(doc->permissions);
	Signatures_free(doc->signatures);
	CT_VPreferences_free(doc->vPreferences);
	delete doc;
	doc = nullptr;
}

inline void CT_Clip_free(CT_Clip * clip)
{
	if (!clip)
	{
		return;
	}
	for (auto &area : *clip)
	{
		Area_free(area);
	}
	clip->clear();
	clip->shrink_to_fit();
	delete clip;
	clip = nullptr;
}

inline void CT_Matrix_free(CT_Matrix * cTM)
{
	if (!cTM)
	{
		return;
	}
	delete cTM;
	cTM = nullptr;
}

inline void CT_GraphicUnit_free(CT_GraphicUnit * ct_graphicUnit)
{
	if (!ct_graphicUnit)
	{
		return;
	}
	for (auto &action : ct_graphicUnit->actions)
	{
		CT_Action_free(action);
	}
	ct_graphicUnit->actions.clear();
	ct_graphicUnit->actions.shrink_to_fit();
	ST_Box_free(ct_graphicUnit->boundary);
	for (auto &clip : ct_graphicUnit->clips)
	{
		CT_Clip_free(clip);
	}
	ct_graphicUnit->clips.clear();
	ct_graphicUnit->clips.shrink_to_fit();
	CT_Matrix_free(ct_graphicUnit->cTM);
	ct_graphicUnit->dashPattern.clear();
	ct_graphicUnit->dashPattern.shrink_to_fit();
	ct_graphicUnit->name.clear();
	ct_graphicUnit->name.shrink_to_fit();
	delete ct_graphicUnit;
	ct_graphicUnit = nullptr;
}

inline void CT_CGTransform_free(CT_CGTransform * cg)
{
	if (!cg)
	{
		return;
	}
	cg->glyphs.clear();
	cg->glyphs.shrink_to_fit();
	delete cg;
	cg = nullptr;
}

inline void TextCode_free(TextCode * textCode)
{
	if (!textCode)
	{
		return;
	}
	textCode->str.clear();
	textCode->str.shrink_to_fit();
	delete textCode;
	textCode = nullptr;
}

inline void TextCode_And_CT_CGTransform_free(TextCode_And_CT_CGTransform * tac)
{
	if (!tac)
	{
		return;
	}
	for (auto &cg : tac->ct_cGTransforms)
	{
		CT_CGTransform_free(cg);
	}
	tac->ct_cGTransforms.clear();
	tac->ct_cGTransforms.shrink_to_fit();
	TextCode_free(tac->textCode);
	delete tac;
	tac = nullptr;
}

inline void CT_Text_free(CT_Text * ct_text)
{
	if (!ct_text)
	{
		return;
	}
	CT_GraphicUnit_free(ct_text->ct_graphicUnit);
	CT_Color_free(ct_text->fillColor);
	CT_Color_free(ct_text->strokeColor);
	for (auto &tac : ct_text->textcode_and_ct_cgtransforms)
	{
		TextCode_And_CT_CGTransform_free(tac);
	}
	ct_text->textcode_and_ct_cgtransforms.clear();
	ct_text->textcode_and_ct_cgtransforms.shrink_to_fit();
	delete ct_text;
	ct_text = nullptr;
}

inline void TextObject_free(TextObject * textObject)
{
	if (!textObject)
	{
		return;
	}
	CT_Text_free(textObject->ct_text);
	delete textObject;
	textObject = nullptr;
}

inline void AbbreviatedData_free(AbbreviatedData * abb)
{
	if (!abb)
	{
		return;
	}
	abb->cMd.clear();
	abb->cMd.shrink_to_fit();
	abb->params.clear();
	abb->params.shrink_to_fit();
	delete abb;
	abb = nullptr;
}

inline void CT_Path_free(CT_Path * ct_path)
{
	if (!ct_path)
	{
		return;
	}
	for (auto &abb : ct_path->abbreviatedData)
	{
		AbbreviatedData_free(abb);
	}
	ct_path->abbreviatedData.clear();
	ct_path->abbreviatedData.shrink_to_fit();
	CT_GraphicUnit_free(ct_path->ct_graphicUnit);
	CT_Color_free(ct_path->fillColor);
	CT_Color_free(ct_path->strokeColor);
	delete ct_path;
	ct_path = nullptr;
}

inline void PathObject_free(PathObject * pathObject)
{
	if (!pathObject)
	{
		return;
	}
	CT_Path_free(pathObject->ct_path);
	delete pathObject;
	pathObject = nullptr;
}

inline void Border_free(Border * border)
{
	if (!border)
	{
		return;
	}
	CT_Color_free(border->BorderColor);
	border->dashPattern.clear();
	border->dashPattern.shrink_to_fit();
	delete border;
	border = nullptr;
}

inline void CT_Image_free(CT_Image * ct_imageObject)
{
	if (!ct_imageObject)
	{
		return;
	}
	Border_free(ct_imageObject->border);
	CT_GraphicUnit_free(ct_imageObject->ct_graphicUnit);
	delete ct_imageObject;
	ct_imageObject = nullptr;
}

inline void ImageObject_free(ImageObject * imageObject)
{
	if (!imageObject)
	{
		return;
	}
	CT_Image_free(imageObject->ct_imageObject);
	delete imageObject;
	imageObject = nullptr;
}

inline void CT_Composite_free(CT_Composite * ct_compositeObject)
{
	if (!ct_compositeObject)
	{
		return;
	}
	CT_GraphicUnit_free(ct_compositeObject->ct_graphicUnit);
	delete ct_compositeObject;
	ct_compositeObject = nullptr;
}

inline void CompositeObject_free(CompositeObject * compositeObject)
{
	if (!compositeObject)
	{
		return;
	}
	CT_Composite_free(compositeObject->ct_compositeObject);
	delete compositeObject;
	compositeObject = nullptr;
}

inline void PageBlock_free(PageBlock * pageBlock)
{
	if (!pageBlock)
	{
		return;
	}
	CT_PageBlock_free(pageBlock->ct_pageBlock);
	delete pageBlock;
	pageBlock = nullptr;
}

inline void CT_PageBlock_free(CT_PageBlock * ct_pageBlock)
{
	if (!ct_pageBlock)
	{
		return;
	}
	for (auto &typedata : ct_pageBlock->d)
	{
		switch (typedata.first)
		{
		case CT_PageBlock::TEXTOBJECT:
			TextObject_free(typedata.second->textObject);
			break;
		case CT_PageBlock::PATHOBJECT:
			PathObject_free(typedata.second->pathObject);
			break;
		case CT_PageBlock::IMAGEOBJECT:
			ImageObject_free(typedata.second->imageObject);
			break;
		case CT_PageBlock::COMPOSITEOBJECT:
			CompositeObject_free(typedata.second->compositeObject);
			break;
		case CT_PageBlock::PAGEBLOCK:
			PageBlock_free(typedata.second->pageBlock);
			break;
		default:
			break;
		}
		delete typedata.second;
	}
	ct_pageBlock->d.clear();
	ct_pageBlock->d.shrink_to_fit();
	delete ct_pageBlock;
	ct_pageBlock = nullptr;
}

inline void CellContent_free(CellContent * cellContent)
{
	if (!cellContent)
	{
		return;
	}
	CT_PageBlock_free(cellContent->ct_pageBlock);
	delete cellContent;
	cellContent = nullptr;
}

inline void CT_Pattern_free(CT_Pattern * pattern)
{
	if (!pattern)
	{
		return;
	}
	pattern->cTM.clear();
	pattern->cTM.shrink_to_fit();
	CellContent_free(pattern->cellContent);
}

inline void Segment_free(Segment * segment)
{
	if (!segment)
	{
		return;
	}
	CT_Color_free(segment->color);
	delete segment;
	segment = nullptr;
}

inline void CT_AxialShd_free(CT_AxialShd * axialShd)
{
	if (!axialShd)
	{
		return;
	}
	axialShd->endPoint.clear();
	axialShd->endPoint.shrink_to_fit();
	for (auto &segment : axialShd->segments)
	{
		Segment_free(segment);
	}
	axialShd->startPoint.clear();
	axialShd->startPoint.shrink_to_fit();
	delete axialShd;
	axialShd = nullptr;
}

inline void CT_RadialShd_free(CT_RadialShd * radialShd)
{
	if (!radialShd)
	{
		return;
	}
	radialShd->endPoint.clear();
	radialShd->endPoint.shrink_to_fit();
	for (auto &segment : radialShd->segments)
	{
		Segment_free(segment);
	}
	radialShd->startPoint.clear();
	radialShd->startPoint.shrink_to_fit();
	delete radialShd;
	radialShd = nullptr;
}

inline void CT_Point_free(CT_GouraudShd::Point * point)
{
	if (!point)
	{
		return;
	}
	CT_Color_free(point->color);
	delete point;
	point = nullptr;
}

inline void CT_Point_free(CT_LaGouraudShd::Point * point)
{
	if (!point)
	{
		return;
	}
	CT_Color_free(point->color);
	delete point;
	point = nullptr;
}

inline void CT_GouraudShd_free(CT_GouraudShd * gouraudShd)
{
	if (!gouraudShd)
	{
		return;
	}
	CT_Color_free(gouraudShd->backColor);
	for (auto &point : gouraudShd->points)
	{
		CT_Point_free(point);
	}
	gouraudShd->points.clear();
	gouraudShd->points.shrink_to_fit();
	delete gouraudShd;
	gouraudShd = nullptr;
}

inline void CT_LaGouraudShd_free(CT_LaGouraudShd * laGouraudShd)
{
	if (!laGouraudShd)
	{
		return;
	}
	CT_Color_free(laGouraudShd->backColor);
	for (auto &point : laGouraudShd->points)
	{
		CT_Point_free(point);
	}
	laGouraudShd->points.clear();
	laGouraudShd->points.shrink_to_fit();
	delete laGouraudShd;
	laGouraudShd = nullptr;
}

inline void CT_Color_free(CT_Color *ct_color)
{
	if (!ct_color)
	{
		return;
	}
	switch (ct_color->type)
	{
	case CT_Color::TYPE::PATTERN:
		CT_Pattern_free(ct_color->d.pattern);
		break;
	case CT_Color::TYPE::AXIALSHD:
		CT_AxialShd_free(ct_color->d.axialShd);
		break; 
	case CT_Color::TYPE::RADIALSHD:
		CT_RadialShd_free(ct_color->d.radialShd);
		break; 
	case CT_Color::TYPE::GOURAUDSHD:
		CT_GouraudShd_free(ct_color->d.gouraudShd);
		break; 
	case CT_Color::TYPE::LAGOURANDSHD:
		CT_LaGouraudShd_free(ct_color->d.laGouraudShd);
		break;
	default:
		break;
	}
	delete ct_color;
	ct_color = nullptr;
}

inline void CT_ColorSpace_free(CT_ColorSpace *ct_colorspace)
{
	if (!ct_colorspace)
	{
		return;
	}
	for (auto &palette : ct_colorspace->Palette)
	{
		palette.clear();
		palette.shrink_to_fit();
	}
	ct_colorspace->Palette.clear();
	ct_colorspace->Palette.shrink_to_fit();
	ST_Loc_free(ct_colorspace->Profile);
	delete ct_colorspace;
	ct_colorspace = nullptr;
}

inline void CT_DrawParam_free(CT_DrawParam *ct_drawparam)
{
	if (!ct_drawparam)
	{
		return;
	}
	ct_drawparam->cap.clear();
	ct_drawparam->cap.shrink_to_fit();
	ct_drawparam->dashPattern.clear();
	ct_drawparam->dashPattern.shrink_to_fit();
	CT_Color_free(ct_drawparam->fillColor);
	ct_drawparam->join.clear();
	ct_drawparam->join.shrink_to_fit();
	CT_Color_free(ct_drawparam->strokeColor);
	delete ct_drawparam;
	ct_drawparam = nullptr;
}

inline void CT_Font_free(CT_Font *ct_font)
{
	if (!ct_font)
	{
		return;
	}
	ct_font->familyName.clear();
	ct_font->familyName.shrink_to_fit();
	ST_Loc_free(ct_font->fontFile);
	ct_font->fontName.clear();
	ct_font->fontName.shrink_to_fit();
	delete ct_font;
	ct_font = nullptr;
}

inline void CT_MultiMedia_free(CT_MultiMedia *ct_multimedia)
{
	if (!ct_multimedia)
	{
		return;
	}
	ct_multimedia->format.clear();
	ct_multimedia->format.shrink_to_fit();
	ST_Loc_free(ct_multimedia->MediaFile);
	delete ct_multimedia;
	ct_multimedia = nullptr;
}

inline void CT_VectorG_free(CT_VectorG *ct_vectorg)
{
	if (!ct_vectorg)
	{
		return;
	}
	CT_PageBlock_free(ct_vectorg->content);
	delete ct_vectorg;
	ct_vectorg = nullptr;
}

inline void OFD_DataA_free(Res::TYPE type, OFD::DataA *dataA)
{
	if (!dataA)
	{
		return;
	}
	delete dataA;
	dataA = nullptr;
}

void OFD_free(OFD *ofd)
{
	if (!ofd)
	{
		return;
	}
	for (DocBody *docbody : ofd->docBodies)
	{
		DocBody_free(docbody);
	}
	ofd->docBodies.clear();
	ofd->docBodies.shrink_to_fit();
	ofd->docType.clear();
	ofd->docType.shrink_to_fit();
	for (Document *doc : ofd->documents)
	{
		Document_free(doc);
	}
	ofd->documents.clear();
	ofd->documents.shrink_to_fit();
	for (auto &kv : *(ofd->resAll))
	{
		OFD_DataA_free(kv.second.first, kv.second.second);
	}
	ofd->resAll->clear();
	delete ofd->resAll;
	ofd->root_path.clear();
	ofd->root_path.shrink_to_fit();
	ofd->version.clear();
	ofd->version.shrink_to_fit();
	delete ofd;
	ofd = nullptr;
}

bool OFDParser::isDir(const char *lpPath)
{
	return (GetFileAttributesA(lpPath)&FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY;
}

void OFDParser::relativePath(std::string dir, std::string &str)
{
	std::replace(dir.begin(), dir.end(), '/', '\\');
	std::replace(str.begin(), str.end(), '/', '\\');
	std::string out;
	if (PathIsRelative(str.c_str()))
	{
		char tmp[MAX_PATH] = { 0 };
		memcpy(tmp, dir.c_str(), dir.length());
		if (!isDir(tmp))
		{
			PathRemoveFileSpecA(tmp);
		}
		out = tmp;
		out += '\\';
		out += str;
	}
	else if (str[0] == '\\')
	{
		char tmp[MAX_PATH] = { 0 };
		memcpy(tmp, this->ofd_dir.c_str(), this->ofd_dir.length());
		if (!isDir(tmp))
		{
			PathRemoveFileSpecA(tmp);
		}
		out = tmp;
		out += str;
	} 
	else
	{
		out.assign(str);
	}
	str.assign(out);
}

OFD *OFDParser::getData()
{
	return this->ofd;
}

void OFDParser::openFile(std::string path) {
	if (document)
	{
		delete document;
	}
	document = new xml_document();
	document->load_file(path.c_str(), pugi::parse_default, pugi::encoding_utf8);
}

void OFDParser::readOFD(std::string path) {
	openFile(path);
	xml_node new_ofd = document->child("ofd:OFD");
	//OFD *ofd_data = NULL;
	if (!new_ofd.empty()) {
		if (this->ofd)
		{
			delete this->ofd;
		}
		this->ofd = new OFD();
		this->ofd->ofdRoot.abs = path;
		this->ofd->ofdRoot.pln = "OFD.xml";
		this->ofd->resAll = new std::map<ST_ID, std::pair<Res::TYPE, OFD::DataA *>>();
		if (!new_ofd.attribute("DocType").empty())
			this->ofd->docType = new_ofd.attribute("DocType").as_string();
		if (new_ofd.hasAttribute("Version"))
			this->ofd->version = new_ofd.attribute("Version").as_string();
		xml_node new_docbody = new_ofd.child("ofd:DocBody");

		while (!new_docbody.isNull()) {
			auto *docbody_data = new DocBody();
			this->ofd->docBodies.push_back(docbody_data);

			xml_node new_docinfo = new_docbody.child("ofd:DocInfo");
			if (!new_docinfo.isNull()) {
				auto *docinfo_data = new CT_DocInfo();
				docbody_data->docInfo = docinfo_data;

				xml_node t;
				if (!(t = new_docinfo.child("ofd:DocID")).isNull())
					docinfo_data->DocID = t.text().as_string();
				if (!(t = new_docinfo.child("ofd:CreationDate")).isNull())
					docinfo_data->creationDate = string2time(t.text().as_string());
				if (!(t = new_docinfo.child("ofd:ModDate")).isNull())
					docinfo_data->modDate = string2time(t.text().as_string());
				if (!(t = new_docinfo.child("ofd:Title")).isNull())
					docinfo_data->title = t.text().as_string();
				if (!(t = new_docinfo.child("ofd:Author")).isNull())
					docinfo_data->author = t.text().as_string();
				if (!(t = new_docinfo.child("ofd:Subject")).isNull())
					docinfo_data->subject = t.text().as_string();
				if (!(t = new_docinfo.child("ofd:Creator")).isNull())
					docinfo_data->creator = t.text().as_string();
				if (!(t = new_docinfo.child("ofd:CreatorVersion")).isNull())
					docinfo_data->creatorVersion = t.text().as_string();
				if (!(t = new_docinfo.child("ofd:DocUsage")).isNull())
					docinfo_data->docUsage = t.text().as_string();
				//读取CustomsDatas
				if (!(t = new_docinfo.child("ofd:CustomDatas")).isNull()) {
					xml_node t2 = t.child("ofd:CustomData");
					while (!t2.isNull()) {
						auto * customData = new CustomData();
						customData->name = t2.attribute("Name").as_string();
						customData->data = t2.text().as_string();
						docinfo_data->CustomDatas.push_back(customData);
						t2 = t2.next_sibling("ofd:CustomData");
					}
				}
			}
			else {
				return;
			}
			xml_node new_docroot = new_docbody.child("ofd:DocRoot");
			if (!new_docroot.isNull() && strlen(new_docroot.text().as_string()) != 0) {
				docbody_data->docRoot = new ST_Loc();
				docbody_data->docRoot->pln = new_docroot.text().as_string();
				docbody_data->docRoot->abs = new_docroot.text().as_string();
				this->relativePath(ofd_dir, docbody_data->docRoot->abs);
			}

			xml_node new_signatures = new_docbody.child("ofd:Signatures");
			if (!new_signatures.isNull() && strlen(new_signatures.text().as_string()) != 0) {
				docbody_data->Signatures = new ST_Loc();
				docbody_data->Signatures->pln = new_signatures.text().as_string();
				docbody_data->Signatures->abs = new_signatures.text().as_string();
				this->relativePath(ofd_dir, docbody_data->Signatures->abs);
			}

			new_docbody = new_ofd.next_sibling("ofd:DocBody");
		}

	}
	if (this->ofd) {
		for (DocBody *docbody : ofd->docBodies)
		{
			std::string docfilepath = docbody->docRoot->abs;
			Document *crdoc = readDocument(docfilepath);
			if (docbody->Signatures)
			{
				readSignatures(crdoc, docbody->Signatures->abs);
			}
			this->ofd->documents.push_back(crdoc);
		}
	}
}

Document * OFDParser::readDocument(std::string path) {
	openFile(path);
	xml_node new_document = document->child("ofd:Document");
	Document *document_data;
	if (!new_document.isNull()) {
		document_data = new Document();
		document_data->fromOfd = this->ofd;
		document_data->docIndex = 0;
		// commonData
		xml_node new_commondata = new_document.child("ofd:CommonData");
		if (!new_commondata.isNull()) {
			auto *commondata_data = new CommonData();
			document_data->commonData = commondata_data;
			xml_node t;

			// MaxUnitID
			if (!(t = new_commondata.child("ofd:MaxUnitID")).isNull()) {
				commondata_data->maxUnitID = t.text().as_uint();
			}
			else {
				return nullptr;
				//Error
			}

			// PageArea
			if (!(t = new_commondata.child("ofd:PageArea")).isNull()) {
				commondata_data->pageArea = new CT_PageArea();
				readPageArea(t, commondata_data->pageArea);
			}
			else {
				return nullptr;
			}

			// publicRes
			t = new_commondata.child("ofd:PublicRes");
			while (!t.isNull() && strlen(t.text().as_string("", "GBK")) != 0) {
				auto *p = new ST_Loc();
				p->pln = t.text().as_string("", "GBK");
				p->abs = t.text().as_string("", "GBK");
				this->relativePath(path, p->abs);
				commondata_data->publicRes.push_back(std::make_pair(p, nullptr));
				t = t.next_sibling("ofd:PublicRes");
			}

			// documentRes
			t = new_commondata.child("ofd:DocumentRes");
			while (!t.isNull() && strlen(t.text().as_string("", "GBK")) != 0) {
				auto *p = new ST_Loc();
				p->pln = t.text().as_string("", "GBK");
				p->abs = t.text().as_string("", "GBK");
				this->relativePath(path, p->abs);
				commondata_data->documentRes.push_back(std::make_pair(p, nullptr));
				t = t.next_sibling("ofd:DocumentRes");
			}

			// TemplatePage
			t = new_commondata.child("ofd:TemplatePage");
			while (!t.isNull()) {
				auto *templatePage = new TemplatePage();

				templatePage->id = t.attribute("ID").as_uint();
				if (t.hasAttribute("BaseLoc"))
				{
					templatePage->baseLoc = new ST_Loc();
					templatePage->baseLoc->pln = t.attribute("BaseLoc").as_string();
					templatePage->baseLoc->abs = t.attribute("BaseLoc").as_string();
					this->relativePath(path, templatePage->baseLoc->abs);
				}

				if (t.hasAttribute("ZOrder"))
				{
					std::string zorder = t.attribute("ZOrder").as_string();
					if (zorder.compare("Background")==0)
					{
						templatePage->zorder = TemplatePage::ZORDER::BACKGROUND;
					}
					else if (zorder.compare("Foreground") == 0)
					{
						templatePage->zorder = TemplatePage::ZORDER::FOREGROUND;
					}
				}
				templatePage->name = t.attribute("Name").as_string();
				commondata_data->templatePages.push_back(std::make_pair(templatePage, nullptr));
				t = t.next_sibling("ofd:TemplatePage");
			}
			std::sort(commondata_data->templatePages.begin(), commondata_data->templatePages.end(), [](const std::pair<TemplatePage*, Page*> &x, const std::pair<TemplatePage*, Page*> &y){return x.first->zorder < y.first->zorder; });
			if (!(t = new_commondata.child("ofd:DefaultCS")).isNull()) {
				//TODO
				commondata_data->defaultCS = t.text().as_uint();
			}

		}
		else {
			return nullptr;
		}

		// Pages
		int pageIndex = 0;
		xml_node new_pages = new_document.child("ofd:Pages");
		if (!new_pages.isNull()) {
			xml_node new_page = new_pages.child("ofd:Page");
			while (!new_page.isNull()) {
				auto *page = new Document_Page();
				page->id = new_page.attribute("ID").as_uint();
				page->fromDoc = document_data;
				page->pageIndex = pageIndex;

				if (new_page.hasAttribute("BaseLoc"))
				{
					page->baseLoc = new ST_Loc();
					page->baseLoc->pln = new_page.attribute("BaseLoc").as_string();
					page->baseLoc->abs = new_page.attribute("BaseLoc").as_string();
					this->relativePath(path, page->baseLoc->abs);
				}

				if (strlen(new_page.text().as_string()) != 0)
				{
					page->loc = new ST_Loc();
					page->loc->pln = new_page.text().as_string();
					page->loc->abs = new_page.text().as_string();
					this->relativePath(path, page->loc->abs);
				}
				document_data->pages.push_back(std::make_pair(page, nullptr));
				new_page = new_page.next_sibling("ofd:Page");
				pageIndex++;
			}
		}
		else {
			return nullptr;
		}
		
		xml_node new_outlines = new_document.child("ofd:Outlines");
		readOutlines(new_outlines, &document_data->outlines);
		

		xml_node new_customtags = new_document.child("ofd:CustomTags");
		if (!new_customtags.isNull() && strlen(new_customtags.text().as_string()) != 0) {
			document_data->customTags = new ST_Loc();
			document_data->customTags->pln = new_customtags.text().as_string("", "GBK");
			document_data->customTags->abs = new_customtags.text().as_string("", "GBK");
			this->relativePath(path, document_data->customTags->abs);
		}

		xml_node new_extensions = new_document.child("ofd:Extensions");
		if (!new_extensions.isNull() && strlen(new_extensions.text().as_string()) != 0) {
			document_data->extensions = new ST_Loc();
			document_data->extensions->pln = new_extensions.text().as_string("", "GBK");
			document_data->extensions->abs = new_extensions.text().as_string("", "GBK");
			this->relativePath(path, document_data->customTags->abs);
		}

		xml_node new_annotations = new_document.child("ofd:Annotations");
		if (!new_annotations.isNull() && strlen(new_annotations.text().as_string()) != 0) {
			document_data->annotations = new ST_Loc();
			document_data->annotations->pln = new_annotations.text().as_string("", "GBK");
			document_data->annotations->abs = new_annotations.text().as_string("", "GBK");
			this->relativePath(path, document_data->annotations->abs);
		}

	}
	else {
		return nullptr;
	}

	// publicRes
	for (auto &st_loc_res : document_data->commonData->publicRes) {
		st_loc_res.second = new Res();
		readResource(st_loc_res.second, st_loc_res.first->abs);
	}

	// documentRes
	for (auto &st_loc_res : document_data->commonData->documentRes) {
		st_loc_res.second = new Res();
		readResource(st_loc_res.second, st_loc_res.first->abs);
	}

	//访问页面
	for (auto &st_loc_page : document_data->pages) {
		st_loc_page.second = new Page();
		st_loc_page.second->fromDoc = document_data;
		st_loc_page.second->pageIndex = st_loc_page.first->pageIndex;
		readPage(st_loc_page.second, st_loc_page.first->loc ? st_loc_page.first->loc->abs : st_loc_page.first->baseLoc->abs);
	}

	//访问模板
	for (std::pair<TemplatePage*, Page*> &st_loc_page : document_data->commonData->templatePages) {
		st_loc_page.second = new Page();
		readTemplatePage(st_loc_page.second, st_loc_page.first->baseLoc->abs);
	}

	//访问自定义标签
	if (document_data->customTags) {
		//TODO
	}
	return document_data;
}

void OFDParser::readOutlines(xml_node new_outlines, std::vector<CT_OutlineElem *>* outlines)
{
	if (!new_outlines.isNull()) {
		xml_node new_ctoutlineelem = new_outlines.child("ofd:OutlineElem");

		auto *outline_elem = new CT_OutlineElem();
		outline_elem->title = new_ctoutlineelem.attribute("Title").as_string();
		outline_elem->count = new_ctoutlineelem.attribute("Count").as_int();
		outline_elem->expanded = new_ctoutlineelem.attribute("Expanded").as_bool();
		if (!outline_elem->expanded)
		{
			xml_node childoutlineelem = new_ctoutlineelem.child("OutlineElem");
			readOutlines(childoutlineelem, &outline_elem->outlineElems);
		}
		xml_node actions_node = new_ctoutlineelem.child("Actions");
		readActions(actions_node, outline_elem);
		outlines->push_back(outline_elem);
	}
}

void OFDParser::readActions(xml_node actions_node, CT_OutlineElem *outlines)
{
	if (!actions_node.isNull()) {
		xml_node new_action = actions_node.child("ofd:Action");
		while (!new_action.isNull())
		{
			auto *ct_action = new CT_Action();
			readAction(new_action, ct_action);
			outlines->actions.push_back(ct_action);
		}
	}
}

void OFDParser::readAction(xml_node action_node, CT_Action *action)
{
	if (!action_node.child("Goto").isNull())
	{
		xml_node goto_node = action_node.child("Goto");
		action->type = CT_Action::GOTO;
		auto *_goto = new Goto();
		if (!goto_node.child("Dest").isNull())
		{
			xml_node dest = goto_node.child("Dest");
			_goto->type = Goto::TYPE::DEST;
			_goto->d.dest = new CT_Dest();
			std::string strDestType = dest.attribute("Type").as_string();
			if (strDestType.compare("XYZ")==0)
			{
				_goto->d.dest->type = CT_Dest::TYPE::XYZ;
			}
			else if (strDestType.compare("Fit") == 0)
			{
				_goto->d.dest->type = CT_Dest::TYPE::FIT;
			}
			else if (strDestType.compare("FitH") == 0)
			{
				_goto->d.dest->type = CT_Dest::TYPE::FITH;
			}
			else if (strDestType.compare("FitV") == 0)
			{
				_goto->d.dest->type = CT_Dest::TYPE::FITV;
			}
			else if (strDestType.compare("FitR") == 0)
			{
				_goto->d.dest->type = CT_Dest::TYPE::FITR;
			}
			else
			{
				return;
			}
			_goto->d.dest->pageID = dest.attribute("ST_RefID").as_uint();
			_goto->d.dest->left = dest.attribute("Left").as_double();
			_goto->d.dest->top = dest.attribute("Top").as_double();
			_goto->d.dest->right = dest.attribute("Right").as_double();
			_goto->d.dest->bottom = dest.attribute("Bottom").as_double();
			_goto->d.dest->zoom = dest.attribute("Zoom").as_double();
		} 
		else if (!goto_node.child("Bookmark").isNull())
		{
			_goto->type = Goto::TYPE::BOOKMARK;
		}
		action->d._goto = _goto;
	}
	else if (!action_node.child("URI").isNull())
	{
		//TODO
	}
	else if (!action_node.child("GotoA").isNull())
	{
		//TODO
	}
	else if (!action_node.child("Sound").isNull())
	{
		//TODO
	}
	else if (!action_node.child("Movie").isNull())
	{
		//TODO
	}
	else if (!action_node.child("Goto").isNull())
	{
		//TODO
	}
}

void OFDParser::readPageArea(xml_node pageareas_node, CT_PageArea *data) {
	xml_node t;
	if (!(t = pageareas_node.child("ofd:PhysicalBox")).isNull())
	{
		std::string values = t.text().as_string();
		std::vector<std::string> strBox;
		split(values, ' ', strBox);
		if (strBox.size() == 4)
		{
			data->physicalBox = new ST_Box();
			data->physicalBox->x = atof(strBox[0].c_str());
			data->physicalBox->y = atof(strBox[1].c_str());
			data->physicalBox->w = atof(strBox[2].c_str());
			data->physicalBox->h = atof(strBox[3].c_str());
		}
		else
		{
			return;
		}
	}
	else
	{
		return;
	}
	if (!(t = pageareas_node.child("ofd:ApplicationBox")).isNull())
	{
		std::string values = t.text().as_string();
		std::vector<std::string> strBox;
		split(values, ' ', strBox);
		if (strBox.size() == 4)
		{
			data->applicationBox = new ST_Box();
			data->applicationBox->x = atof(strBox[0].c_str());
			data->applicationBox->y = atof(strBox[1].c_str());
			data->applicationBox->w = atof(strBox[2].c_str());
			data->applicationBox->h = atof(strBox[3].c_str());
		}
		else
		{
			return;
		}
	}

	if (!(t = pageareas_node.child("ofd:ContentBox")).isNull())
	{
		std::string values = t.text().as_string();
		std::vector<std::string> strBox;
		split(values, ' ', strBox);
		if (strBox.size() == 4)
		{
			data->contentBox = new ST_Box();
			data->contentBox->x = atof(strBox[0].c_str());
			data->contentBox->y = atof(strBox[1].c_str());
			data->contentBox->w = atof(strBox[2].c_str());
			data->contentBox->h = atof(strBox[3].c_str());
		}
		else
		{
			return;
		}
	}


	if (!(t = pageareas_node.child("ofd:BleedBox")).isNull())
	{
		std::string values = t.text().as_string();
		std::vector<std::string> strBox;
		split(values, ' ', strBox);
		if (strBox.size() == 4)
		{
			data->bleedBox = new ST_Box();
			data->bleedBox->x = atof(strBox[0].c_str());
			data->bleedBox->y = atof(strBox[1].c_str());
			data->bleedBox->w = atof(strBox[2].c_str());
			data->bleedBox->h = atof(strBox[3].c_str());
		}
		else
		{
			return;
		}
	}

}

void OFDParser::readResource(Res * res_data, std::string path) {
	openFile(path);
	xml_node new_res = document->child("ofd:Res");
	if (!new_res.isNull())
	{
		//读取属性
		if (new_res.hasAttribute("BaseLoc"))
		{
			res_data->baseLoc = new ST_Loc();
			res_data->baseLoc->pln = new_res.attribute("BaseLoc").as_string();
			res_data->baseLoc->abs = new_res.attribute("BaseLoc").as_string();
			this->relativePath(path, res_data->baseLoc->abs);
		}
		/*else
		{
			return;
		}*/

		//读取成员--font
		xml_node t;
		if (!(t = new_res.child("ofd:Fonts")).isNull())
		{
			auto *fonts = new Res::Data();
			fonts->fonts = new Fonts();
			xml_node t2 = t.child("ofd:Font");
			while (!t2.isNull())
			{
				ST_ID i(t2.attribute("ID").as_uint());
				auto *ct_font = new CT_Font();
				readFont(t2, ct_font, res_data->baseLoc ? res_data->baseLoc->abs : path);
				fonts->fonts->push_back(std::make_pair(i, ct_font));
				auto *dataA = new OFD::DataA();
				dataA->font = ct_font;
				this->ofd->resAll->insert(std::make_pair(i, std::make_pair(Res::TYPE::FONTS, dataA)));
				t2 = t2.next_sibling("ofd:Font");
			}

			Res::TYPE type_data = Res::FONTS;
			res_data->resData.push_back(std::make_pair(type_data, fonts));
		}

		// colorSpaces
		if (!(t = new_res.child("ofd:ColorSpaces")).isNull())
		{
			xml_node t2 = t.child("ofd:ColorSpace");
			while (!t2.isNull())
			{
				auto *colorspaces = new Res::Data();
				colorspaces->colorSpaces = new ColorSpaces();
				Res::TYPE type_data = Res::COLORSPACES;
				res_data->resData.push_back(std::make_pair(type_data, colorspaces));
				t2 = t2.next_sibling("ofd:ColorSpace");
			}
		}

		// DrawParams
		if (!(t = new_res.child("ofd:DrawParams")).isNull())
		{
			xml_node t2 = t.child("ofd:DrawParam");
			while (!t2.isNull())
			{
				auto *drawparams = new Res::Data();
				drawparams->drawParams = new DrawParams();
				Res::TYPE type_data = Res::DRAWPARAMS;
				res_data->resData.push_back(std::make_pair(type_data, drawparams));
				t2 = t2.next_sibling("ofd:DrawParam");
			}
		}

		// MultMedia
		if (!(t = new_res.child("ofd:MultiMedias")).isNull())
		{
			auto *multimedias = new Res::Data();
			multimedias->multiMedias = new MultiMedias();
			xml_node t2 = t.child("ofd:MultiMedia");
			while (!t2.isNull())
			{
				ST_ID i(t2.attribute("ID").as_uint());
				auto *ct_multimedia = new CT_MultiMedia();

				if (strlen(t2.child("ofd:MediaFile").text().as_string("", "GBK")) != 0)
				{
					ct_multimedia->MediaFile = new ST_Loc();
					ct_multimedia->MediaFile->pln = t2.child("ofd:MediaFile").text().as_string("", "GBK");
					ct_multimedia->MediaFile->abs = t2.child("ofd:MediaFile").text().as_string("", "GBK");
					this->relativePath(res_data->baseLoc ? res_data->baseLoc->abs : path, ct_multimedia->MediaFile->abs);
				}
				ct_multimedia->type = CT_MultiMedia::TYPE::IMAGE;
				ct_multimedia->format = t2.attribute("Format").as_string();
				multimedias->multiMedias->push_back(std::make_pair(i, ct_multimedia));
				auto *dataA = new OFD::DataA();
				dataA->multiMedia = ct_multimedia;
				this->ofd->resAll->insert(std::make_pair(i, std::make_pair(Res::TYPE::MULTIMEDIAS, dataA)));
				t2 = t2.next_sibling("ofd:MultiMedia");
			}
			
			Res::TYPE type_data = Res::MULTIMEDIAS;
			res_data->resData.push_back(std::make_pair(type_data, multimedias));
		}

	}
	else
	{
		return;
	}
}

void OFDParser::readTemplatePage(Page * page_data, std::string path) {
	openFile(path);
	xml_node new_page = document->child("ofd:Page");
	if (!new_page.isNull())
	{
		xml_node t;

		// pageArea
		if (!(t = new_page.child("ofd:Area")).isNull())
		{
			page_data->area = new CT_PageArea();
			readPageArea(t, page_data->area);
		}

		// pageRes
		t = new_page.child("ofd:PageRes");
		while (!t.isNull() && strlen(t.text().as_string("", "GBK")) != 0)
		{
			auto *p = new ST_Loc();
			p->pln = t.text().as_string("", "GBK");
			p->abs = t.text().as_string("", "GBK");
			this->relativePath(path, p->abs);
			//TODO
			auto * new_res = new Res();
			readResource(new_res, p->abs);
			page_data->pageRes.push_back(p);
			t = t.next_sibling("ofd:PageRes");
		}

		// Content
		if (!(t = new_page.child("ofd:Content")).isNull())
		{
			page_data->content = new Content();
			// layer
			xml_node new_layer = t.child("ofd:Layer");
			while (!new_layer.isNull())
			{
				auto * layer_data = new CT_Layer();
				// ID
				ST_ID i(new_layer.attribute("ID").as_uint());

				// Type
				if (new_layer.hasAttribute("Type"))
				{
					std::string strType = new_layer.attribute("Type").as_string();
					if (strType.compare("Body") == 0)
					{
						layer_data->type = CT_Layer::TYPE::BODY;
					}
					else if (strType.compare("Background") == 0)
					{
						layer_data->type = CT_Layer::TYPE::BACKGROUND;
					}
					else if (strType.compare("Foreground") == 0)
					{
						layer_data->type = CT_Layer::TYPE::FOREGROUND;
					}
					else if (strType.compare("Custom") == 0)
					{
						layer_data->type = CT_Layer::TYPE::CUSTOM;
					}
					else
					{
						layer_data->type = CT_Layer::TYPE::BODY;
					}
				}
				// DrawParam
				if (new_layer.hasAttribute("DrawParam"))
				{
					layer_data->drawParam = new_layer.attribute("DrawParam").as_uint();
				}

				layer_data->ct_pageBlock = new CT_PageBlock();
				xml_node t = new_layer.first_child();
				while (!t.isNull())
				{

					//解析文字对象
					bool is_first_textcode_x = true, is_first_textcode_y = true;
					double last_x = 0.0, last_y = 0.0;

					// TextObject
					if (std::string(t.name()).compare("ofd:TextObject") == 0)
					{
						auto *data = new CT_PageBlock::Data();
						data->textObject = new TextObject();
						data->textObject->ct_text = new CT_Text();
						data->textObject->id = t.attribute("ID").as_uint();
						data->textObject->ct_text->ct_graphicUnit = new CT_GraphicUnit();
						readGraphicUnit(t, data->textObject->ct_text->ct_graphicUnit);
						if (t.hasAttribute("Font"))
						{
							data->textObject->ct_text->font = t.attribute("Font").as_uint();
						}
						else
						{
							t = t.next_sibling("ofd:TextObject");
							continue;
						}

						// size
						if (t.hasAttribute("Size"))
						{
							data->textObject->ct_text->size = t.attribute("Size").as_double();
						}
						else
						{
							t = t.next_sibling("ofd:TextObject");
							continue;
						}

						// Italic
						if (t.hasAttribute("Italic"))
						{
							data->textObject->ct_text->italic = t.attribute("Italic").as_bool();
						}

						// Weight
						if (t.hasAttribute("Weight"))
						{
							data->textObject->ct_text->weight = t.attribute("Weight").as_int();
						}

						// Fill
						if (t.hasAttribute("Fill"))
						{
							data->textObject->ct_text->fill = t.attribute("Fill").as_bool();
						}

						// Stroke
						if (t.hasAttribute("Stroke"))
						{
							data->textObject->ct_text->stroke = t.attribute("Stroke").as_bool();
						}

						// HScale
						if (t.hasAttribute("HScale"))
						{
							data->textObject->ct_text->hScale = t.attribute("HScale").as_double();
						}

						// ReadDirection
						if (t.hasAttribute("ReadDirection"))
						{
							data->textObject->ct_text->readDirection = t.attribute("ReadDirection").as_int();
						}

						// CharDirection
						if (t.hasAttribute("CharDirection"))
						{
							data->textObject->ct_text->charDirection = t.attribute("CharDirection").as_int();
						}

						data->textObject->ct_text->fillColor = new CT_Color();
						// FillColor
						if (!t.child("ofd:FillColor").isNull())
						{
							ST_Array fillColor = t.child("ofd:FillColor").attribute("Value").as_string();
							std::vector<std::string> strBox;
							split(fillColor, ' ', strBox);
							data->textObject->ct_text->fillColor->valueR = atof(strBox[0].c_str()) / 255.0f;
							data->textObject->ct_text->fillColor->valueG = atof(strBox[1].c_str()) / 255.0f;
							data->textObject->ct_text->fillColor->valueB = atof(strBox[2].c_str()) / 255.0f;
						}

						data->textObject->ct_text->strokeColor = new CT_Color();
						// StrokeColor
						if (!t.child("ofd:StrokeColor").isNull())
						{
							ST_Array strokeColor = t.child("ofd:StrokeColor").attribute("Value").as_string();
							std::vector<std::string> strBox;
							split(strokeColor, ' ', strBox);
							data->textObject->ct_text->strokeColor->valueR = atof(strBox[0].c_str()) / 255.0f;
							data->textObject->ct_text->strokeColor->valueG = atof(strBox[1].c_str()) / 255.0f;
							data->textObject->ct_text->strokeColor->valueB = atof(strBox[2].c_str()) / 255.0f;
						}

						// textCode
						xml_node t2 = t.child("ofd:TextCode");
						if (t2.isNull())
						{
							t = t.next_sibling("ofd:TextObject");
							continue;
						}
						//处理CT_CGTransform
						auto *textcode_and_ct_cgtransform = new TextCode_And_CT_CGTransform();
						xml_node tcgt = t.child("ofd:CGTransform");
						while (!tcgt.isNull())
						{
							auto * cgt = new CT_CGTransform();
							if (tcgt.hasAttribute("CodePosition"))
							{
								cgt->codePosition = tcgt.attribute("CodePosition").as_int();
							}
							else
							{
								tcgt = tcgt.next_sibling("ofd:CGTransform");
								continue;
							}
							if (tcgt.hasAttribute("CodeCount"))
							{
								cgt->codeCount = tcgt.attribute("CodeCount").as_int();
							}
							if (tcgt.hasAttribute("GlyphCount"))
							{
								cgt->glyphCount = tcgt.attribute("GlyphCount").as_int();
							}
							xml_node tglyphs = tcgt.child("ofd:Glyphs");
							if (tglyphs.isNull())
							{
								tcgt = tcgt.next_sibling("ofd:CGTransform");
								continue;
							}
							cgt->glyphs = tglyphs.text().as_string();
							textcode_and_ct_cgtransform->ct_cGTransforms.push_back(cgt);
							tcgt = tcgt.next_sibling("ofd:CGTransform");
						}

						while (!t2.isNull())
						{
							textcode_and_ct_cgtransform->textCode = new TextCode();
							if (t2.hasAttribute("X"))
							{
								last_x = textcode_and_ct_cgtransform->textCode->x = t2.attribute("X").as_double();
								if (is_first_textcode_x)
									is_first_textcode_x = false;
							}
							else
							{
								if (!is_first_textcode_x && !is_first_textcode_y)
									textcode_and_ct_cgtransform->textCode->x = last_x;
								else
								{
									t2 = t2.next_sibling("ofd:TextCode");
									continue;
								}
							}
							if (t2.hasAttribute("Y"))
							{
								last_y = textcode_and_ct_cgtransform->textCode->y = t2.attribute("Y").as_double();
								if (is_first_textcode_y)
									is_first_textcode_y = false;
							}
							else
							{
								if (!is_first_textcode_x && !is_first_textcode_y)
									textcode_and_ct_cgtransform->textCode->y = last_y;
								else
								{
									t2 = t2.next_sibling("ofd:TextCode");
									continue;
								}
							}
							if (t2.hasAttribute("DeltaX"))
							{
								ST_Array delta_x_data(t2.attribute("DeltaX").as_string());
								int a = delta_x_data.length();
								ST_Array_Delta delta_x;
								std::vector<std::string> strBox;
								split(delta_x_data, ' ', strBox);
								delta_x.push_back(0.0);
								for (size_t i = 0; i < strBox.size(); i++)
								{
									if (strBox[i].compare("g") == 0)
									{
										if (i + 2 < strBox.size())
										{
											for (int j = 0; j < atoi(strBox[i + 1].c_str()); j++)
											{
												delta_x.push_back(atof(strBox[i + 2].c_str()));
											}
											i += 2;
										}
										else
										{
											delta_x.push_back(0.0);
										}
									}
									else
									{
										delta_x.push_back(atof(strBox[i].c_str()));
									}
								}
								textcode_and_ct_cgtransform->textCode->deltaX = delta_x;
							}
							if (t2.hasAttribute("DeltaY"))
							{
								ST_Array delta_y_data(t2.attribute("DeltaY").as_string());
								ST_Array_Delta delta_y;
								std::vector<std::string> strBox;
								split(delta_y_data, ' ', strBox);
								delta_y.push_back(0.0);
								for (size_t i = 0; i < strBox.size(); i++)
								{
									if (strBox[i].compare("g") == 0)
									{
										if (i + 2 < strBox.size())
										{
											for (int j = 0; j < atoi(strBox[i + 1].c_str()); j++)
											{
												delta_y.push_back(atoi(strBox[i + 2].c_str()));
											}
											i += 2;
										}
										else
										{
											delta_y.push_back(0.0);
										}
									}
									else
									{
										delta_y.push_back(atof(strBox[i].c_str()));
									}
								}
								textcode_and_ct_cgtransform->textCode->deltaY = delta_y;
							}
							textcode_and_ct_cgtransform->textCode->str = t2.text().as_string();
							data->textObject->ct_text->textcode_and_ct_cgtransforms.push_back(textcode_and_ct_cgtransform);
							t2 = t2.next_sibling("ofd:TextCode");
						}
						layer_data->ct_pageBlock->d.push_back(std::make_pair(CT_PageBlock::TYPE::TEXTOBJECT, data));
					}

					//解析矢量图对象
					//t = new_layer.child("ofd:PathObject");
					if (std::string(t.name()).compare("ofd:PathObject") == 0)
					{
						auto *data = new CT_PageBlock::Data();
						data->pathObject = new PathObject();
						data->pathObject->ct_path = new CT_Path();
						data->pathObject->id = t.attribute("ID").as_uint();
						data->pathObject->ct_path->ct_graphicUnit = new CT_GraphicUnit();
						readGraphicUnit(t, data->pathObject->ct_path->ct_graphicUnit);
						if (t.hasAttribute("Stroke"))
						{
							data->pathObject->ct_path->stroke = t.attribute("Stroke").as_bool();
						}
						if (t.hasAttribute("Fill"))
						{
							data->pathObject->ct_path->fill = t.attribute("Fill").as_bool();
						}
						if (t.hasAttribute("Rule"))
						{
							if (strcmp(t.attribute("Rule").as_string(), "NonZero") == 0)
								data->pathObject->ct_path->rule = CT_Path::RULE::NONZERO;
							else if (strcmp(t.attribute("Rule").as_string(), "Even-Odd") == 0)
								data->pathObject->ct_path->rule = CT_Path::RULE::EVEN_ODD;
							else
							{
								return;
							}
						}

						data->pathObject->ct_path->fillColor = new CT_Color();
						// FillColor
						if (!t.child("ofd:FillColor").isNull())
						{
							ST_Array fillColor = t.child("ofd:FillColor").attribute("Value").as_string();
							std::vector<std::string> strBox;
							split(fillColor, ' ', strBox);
							data->pathObject->ct_path->fillColor->valueR = atof(strBox[0].c_str()) / 255.0f;
							data->pathObject->ct_path->fillColor->valueG = atof(strBox[1].c_str()) / 255.0f;
							data->pathObject->ct_path->fillColor->valueB = atof(strBox[2].c_str()) / 255.0f;
						}

						data->pathObject->ct_path->strokeColor = new CT_Color();
						// StrokeColor
						if (!t.child("ofd:StrokeColor").isNull())
						{
							ST_Array strokeColor = t.child("ofd:StrokeColor").attribute("Value").as_string();
							std::vector<std::string> strBox;
							split(strokeColor, ' ', strBox);
							data->pathObject->ct_path->strokeColor->valueR = atof(strBox[0].c_str()) / 255.0f;
							data->pathObject->ct_path->strokeColor->valueG = atof(strBox[1].c_str()) / 255.0f;
							data->pathObject->ct_path->strokeColor->valueB = atof(strBox[2].c_str()) / 255.0f;
						}

						// 图形变换的描述
						if (!t.child("ofd:AbbreviatedData").isNull())
						{
							ST_Array strAbbreviatedData = t.child("ofd:AbbreviatedData").text().as_string();
							std::vector<std::string> strBox;
							split(strAbbreviatedData, ' ', strBox);
							for (size_t i = 0; i < strBox.size(); i++)
							{
								if (strBox[i].compare("S") == 0)
								{
									AbbreviatedData *abb = new AbbreviatedData();
									abb->cMd = "S";
									abb->params.push_back(atof(strBox[i + 1].c_str()));
									abb->params.push_back(atof(strBox[i + 2].c_str()));
									i += 2;
									data->pathObject->ct_path->abbreviatedData.push_back(abb);
								}
								else if (strBox[i].compare("M") == 0)
								{
									AbbreviatedData *abb = new AbbreviatedData();
									abb->cMd = "M";
									abb->params.push_back(atof(strBox[i + 1].c_str()));
									abb->params.push_back(atof(strBox[i + 2].c_str()));
									i += 2;
									data->pathObject->ct_path->abbreviatedData.push_back(abb);
								}
								else if (strBox[i].compare("L") == 0)
								{
									AbbreviatedData *abb = new AbbreviatedData();
									abb->cMd = "L";
									abb->params.push_back(atof(strBox[i + 1].c_str()));
									abb->params.push_back(atof(strBox[i + 2].c_str()));
									i += 2;
									data->pathObject->ct_path->abbreviatedData.push_back(abb);
								}
								else if (strBox[i].compare("Q") == 0)
								{
									AbbreviatedData *abb = new AbbreviatedData();
									abb->cMd = "Q";
									abb->params.push_back(atof(strBox[i + 1].c_str()));
									abb->params.push_back(atof(strBox[i + 2].c_str()));
									abb->params.push_back(atof(strBox[i + 3].c_str()));
									abb->params.push_back(atof(strBox[i + 4].c_str()));
									i += 4;
									data->pathObject->ct_path->abbreviatedData.push_back(abb);
								}
								else if (strBox[i].compare("B") == 0)
								{
									AbbreviatedData *abb = new AbbreviatedData();
									abb->cMd = "B";
									abb->params.push_back(atof(strBox[i + 1].c_str()));
									abb->params.push_back(atof(strBox[i + 2].c_str()));
									abb->params.push_back(atof(strBox[i + 3].c_str()));
									abb->params.push_back(atof(strBox[i + 4].c_str()));
									abb->params.push_back(atof(strBox[i + 5].c_str()));
									abb->params.push_back(atof(strBox[i + 6].c_str()));
									i += 6;
									data->pathObject->ct_path->abbreviatedData.push_back(abb);
								}
								else if (strBox[i].compare("A") == 0)
								{
									AbbreviatedData *abb = new AbbreviatedData();
									abb->cMd = "A";
									abb->params.push_back(atof(strBox[i + 1].c_str()));
									abb->params.push_back(atof(strBox[i + 2].c_str()));
									abb->params.push_back(atof(strBox[i + 3].c_str()));
									abb->params.push_back(atof(strBox[i + 4].c_str()));
									abb->params.push_back(atof(strBox[i + 5].c_str()));
									abb->params.push_back(atof(strBox[i + 6].c_str()));
									abb->params.push_back(atof(strBox[i + 7].c_str()));
									i += 7;
									data->pathObject->ct_path->abbreviatedData.push_back(abb);
								}
								else if (strBox[i].compare("C") == 0)
								{
									AbbreviatedData *abb = new AbbreviatedData();
									abb->cMd = "C";
									data->pathObject->ct_path->abbreviatedData.push_back(abb);
								}
								else
								{
									continue;
								}
							}
						}
						else
						{
							return;
						}
						layer_data->ct_pageBlock->d.push_back(std::make_pair(CT_PageBlock::TYPE::PATHOBJECT, data));
					}

					//解析位图对象
					// ImageObject

					//t = new_layer.child("ofd:ImageObject");
					if (std::string(t.name()).compare("ofd:ImageObject") == 0)
					{
						auto *data = new CT_PageBlock::Data();
						data->imageObject = new ImageObject();
						data->imageObject->id = t.attribute("ID").as_uint();
						data->imageObject->ct_imageObject = new CT_Image();
						data->imageObject->ct_imageObject->ct_graphicUnit = new CT_GraphicUnit();
						readGraphicUnit(t, data->imageObject->ct_imageObject->ct_graphicUnit);             // 读取图元里的共用的数据
						if (t.hasAttribute("ResourceID"))
						{
							data->imageObject->ct_imageObject->resourceID = t.attribute("ResourceID").as_int();
						}
						else
						{
							return;
						}

						if (t.hasAttribute("Substitution"))
						{
							data->imageObject->ct_imageObject->substitution = t.attribute("Substitution").as_int();
						}
						layer_data->ct_pageBlock->d.push_back(std::make_pair(CT_PageBlock::TYPE::IMAGEOBJECT, data));
					}
					t = t.next_sibling();
				}
				page_data->content->layers.push_back(std::make_pair(i, layer_data));
				new_layer = new_layer.next_sibling("ofd:Layer");
			}
			std::sort(page_data->content->layers.begin(), page_data->content->layers.end(), [](const std::pair<ST_ID, CT_Layer*> &x, const std::pair<ST_ID, CT_Layer*> &y){return x.second->type < y.second->type; });
		}
	}
	else
	{
		return;
	}
}

void OFDParser::readSignatures(Document * doc, std::string path)
{
	openFile(path);
	xml_node new_signs = document->child("ofd:Signatures");
	if (!new_signs.isNull())
	{
		doc->signatures = new Signatures();
		xml_node t;
		if (!(t = new_signs.child("ofd:MaxSignId")).isNull())
		{
			doc->signatures->maxSignId = t.text().as_string();
		}
		t = new_signs.child("ofd:Signature");
		while (!t.isNull())
		{
			auto *ssig = new Signatures_Signature();
			if (t.hasAttribute("ID"))
			{
				ssig->id = t.attribute("ID").as_uint();
			}
			if (t.hasAttribute("BaseLoc"))
			{
				ssig->baseLoc = new ST_Loc();
				ssig->baseLoc->pln = t.attribute("BaseLoc").as_string();
				ssig->baseLoc->abs = t.attribute("BaseLoc").as_string();
				this->relativePath(path, ssig->baseLoc->abs);
			}
			if (t.hasAttribute("Type"))
			{
				std::string sigtype = t.attribute("Type").as_string();
				if (sigtype.compare("Sign") == 0)
				{
					ssig->type = Signatures_Signature::TYPE::SIGN;
				}
				else
				{
					ssig->type = Signatures_Signature::TYPE::SEAL;
				}
			}
			doc->signatures->signatures.push_back(std::make_pair(ssig, nullptr));
			t = t.next_sibling("ofd:Signature");
		}
	}

	for (std::pair<Signatures_Signature *, Signature *> &st_loc_sig : doc->signatures->signatures) {
		st_loc_sig.second = new Signature();
		st_loc_sig.second->fromDoc = doc;
		readSignature(st_loc_sig.second, st_loc_sig.first->baseLoc->abs);
	}
}

void OFDParser::readSignature(Signature * sig, std::string path)
{
	openFile(path);
	xml_node new_sig = document->child("ofd:Signature");
	if (!new_sig.isNull())
	{
		xml_node t = new_sig.child("ofd:SignedInfo");
		if (!t.isNull())
		{
			sig->signedInfo = new SignedInfo();
			xml_node c = t.first_child();
			while (!c.isNull())
			{

				if (strcmp(c.name(), "ofd:Provider") == 0)
				{
					sig->signedInfo->provider = new Provider();
					if (c.hasAttribute("ProviderName"))
					{
						sig->signedInfo->provider->providerName = c.attribute("ProviderName").as_string();
					}
					if (c.hasAttribute("Version"))
					{
						sig->signedInfo->provider->version = c.attribute("Version").as_string();
					}
					if (c.hasAttribute("Company"))
					{
						sig->signedInfo->provider->company = c.attribute("Company").as_string();
					}
				}
				if (strcmp(c.name(), "ofd:SignatureMethod") == 0)
				{
					sig->signedInfo->signatureMethod = c.text().as_string();
				}
				if (strcmp(c.name(), "ofd:SignatureDateTime") == 0)
				{
					sig->signedInfo->signatureDateTime = c.text().as_string();
				}
				if (strcmp(c.name(), "ofd:References") == 0)
				{
					sig->signedInfo->references = new References();
					if (c.hasAttribute("CheckMethod"))
					{
						std::string checkMethod = c.attribute("CheckMethod").as_string();
						if (checkMethod.compare("1.2.156.10197.1.401") == 0 || checkMethod.compare("SM3") == 0)
						{
							sig->signedInfo->references->checkMethod = References::CHECKMETHOD::SM3;
						}
						else if (checkMethod.compare("SHA1") == 0)
						{
							sig->signedInfo->references->checkMethod = References::CHECKMETHOD::SHA1;
						}
						else
						{
							sig->signedInfo->references->checkMethod = References::CHECKMETHOD::MD5;
						}
					}
					xml_node r = c.child("ofd:Reference");
					sig->signedInfo->references->references = new std::map<ST_Loc*, std::string>();
					while (!r.isNull())
					{
						ST_Loc *fRef = nullptr;
						if (r.hasAttribute("FileRef"))
						{
							fRef = new ST_Loc();
							fRef->pln = r.attribute("FileRef").as_string("", "GBK");
							fRef->abs = r.attribute("FileRef").as_string("", "GBK");
							this->relativePath(path, fRef->abs);
						}
						std::string val;
						if (!r.child("ofd:CheckValue").isNull())
						{
							val = r.child("ofd:CheckValue").text().as_string();
						}
						sig->signedInfo->references->references->insert(std::make_pair(fRef, val));
						r = c.next_sibling("ofd:Reference");
					}
				}
				if (strcmp(c.name(), "ofd:StampAnnot") == 0)
				{
					auto *sa = new StampAnnot();
					sa->fromSignature = sig;
					if (c.hasAttribute("ID"))
					{
						sa->id = c.attribute("ID").as_string();
					}
					if (c.hasAttribute("PageRef"))
					{
						sa->pageRef = c.attribute("PageRef").as_uint();
					}
					if (c.hasAttribute("Boundary"))
					{
						ST_Array bo = c.attribute("Boundary").as_string();
						std::vector<std::string> strBox;
						split(bo, ' ', strBox);
						sa->boundary = new ST_Box();
						//TRACE("%f", atof(strBox[0].c_str()));
						sa->boundary->x = atof(strBox[0].c_str());
						sa->boundary->y = atof(strBox[1].c_str());
						sa->boundary->w = atof(strBox[2].c_str());
						sa->boundary->h = atof(strBox[3].c_str());
					}
					if (c.hasAttribute("Clip"))
					{
						ST_Array cl = c.attribute("Clip").as_string();
						std::vector<std::string> strBox;
						split(cl, ' ', strBox);
						sa->clip = new ST_Box();
						sa->clip->x = atof(strBox[0].c_str());
						sa->clip->y = atof(strBox[1].c_str());
						sa->clip->w = atof(strBox[2].c_str());
						sa->clip->h = atof(strBox[3].c_str());
					}
					sig->signedInfo->stampAnnots.push_back(sa);
				}
				c = c.next_sibling();
			}
		}
		
		t = new_sig.child("ofd:SignedValue");
		if (!t.isNull() && strlen(t.text().as_string("", "GBK")) != 0)
		{
			auto *sl = new ST_Loc();
			sl->pln = t.text().as_string("", "GBK");
			sl->abs = t.text().as_string("", "GBK");
			this->relativePath(path, sl->abs);
			auto *signedValue = new SignedValue();
			readSignedValue(sl->abs, signedValue->val, &signedValue->len);
			signedValue->val = new unsigned char[signedValue->len]{0};
			readSignedValue(sl->abs, signedValue->val, &signedValue->len);
			sig->signedValue = std::make_pair(sl, signedValue);
		}
	}
}

void OFDParser::readSignedValue(std::string path, unsigned char *data, size_t *len)
{
	clock_t start = clock();
	std::ifstream fin(path.c_str(), std::ios::binary);
	fin.seekg(0, std::ios::beg);
	fin.seekg(0, std::ios::end);
	*len = (size_t)fin.tellg();
	if (data == nullptr)
	{
		fin.close();
		return;
	}
	fin.seekg(0, std::ios::beg);
	fin.read((char *)data, static_cast<std::streamsize>(*len));
	fin.close();
}

void OFDParser::readPage(Page * page_data, std::string path) {
	openFile(path);
	xml_node new_page = document->child("ofd:Page");
	if (!new_page.isNull())
	{
		xml_node t;

		// pageArea
		if (!(t = new_page.child("ofd:Area")).isNull())
		{
			page_data->area = new CT_PageArea();
			readPageArea(t, page_data->area);
		}

		// pageRes
		t = new_page.child("ofd:PageRes");
		while (!t.isNull() && strlen(t.text().as_string("", "GBK")) != 0)
		{
			auto *p = new ST_Loc();
			p->pln = t.text().as_string("", "GBK");
			p->abs = t.text().as_string("", "GBK");
			this->relativePath(path, p->abs);
			auto * new_res = new Res();
			readResource(new_res, p->abs);
			page_data->pageRes.push_back(p);
			t = t.next_sibling("ofd:PageRes");
		}

		// Content
		if (!(t = new_page.child("ofd:Content")).isNull())
		{
			page_data->content = new Content();
			// layer
			xml_node new_layer = t.child("ofd:Layer");
			while (!new_layer.isNull())
			{
				auto * layer_data = new CT_Layer();
				// ID
				ST_ID i(new_layer.attribute("ID").as_uint());
				
				// Type
				if (new_layer.hasAttribute("Type"))
				{
					std::string strType = new_layer.attribute("Type").as_string();
					if (strType.compare("Body") == 0)
					{
						layer_data->type = CT_Layer::TYPE::BODY;
					} 
					else if (strType.compare("Background") == 0)
					{
						layer_data->type = CT_Layer::TYPE::BACKGROUND;
					}
					else if (strType.compare("Foreground") == 0)
					{
						layer_data->type = CT_Layer::TYPE::FOREGROUND;
					}
					else if (strType.compare("Custom") == 0)
					{
						layer_data->type = CT_Layer::TYPE::CUSTOM;
					}
					else
					{
						layer_data->type = CT_Layer::TYPE::BODY;
					}
				}
				// DrawParam
				if (new_layer.hasAttribute("DrawParam"))
				{
					layer_data->drawParam = new_layer.attribute("DrawParam").as_uint();
				}

				layer_data->ct_pageBlock = new CT_PageBlock();
				xml_node t = new_layer.first_child();
				while (!t.isNull())
				{
					
					//解析文字对象
					bool is_first_textcode_x = true, is_first_textcode_y = true;
					double last_x = 0.0, last_y = 0.0;

					// TextObject
					if (std::string(t.name()).compare("ofd:TextObject") == 0)
					{
						auto *data = new CT_PageBlock::Data();
						data->textObject = new TextObject();
						data->textObject->ct_text = new CT_Text();
						data->textObject->id = t.attribute("ID").as_uint();
						data->textObject->ct_text->ct_graphicUnit = new CT_GraphicUnit();
						readGraphicUnit(t, data->textObject->ct_text->ct_graphicUnit);
						//Read TextObject attributes
						if (t.hasAttribute("Font"))
						{
							data->textObject->ct_text->font = t.attribute("Font").as_uint();
						}
						else
						{
							t = t.next_sibling("ofd:TextObject");
							continue;
						}

						// size
						if (t.hasAttribute("Size"))
						{
							data->textObject->ct_text->size = t.attribute("Size").as_double();
						}
						else
						{
							t = t.next_sibling("ofd:TextObject");
							continue;
						}

						// Italic
						if (t.hasAttribute("Italic"))
						{
							data->textObject->ct_text->italic = t.attribute("Italic").as_bool();
						}

						// Weight
						if (t.hasAttribute("Weight"))
						{
							data->textObject->ct_text->weight = t.attribute("Weight").as_int();
						}

						// Fill
						if (t.hasAttribute("Fill"))
						{
							data->textObject->ct_text->fill = t.attribute("Fill").as_bool();
						}

						// Stroke
						if (t.hasAttribute("Stroke"))
						{
							data->textObject->ct_text->stroke = t.attribute("Stroke").as_bool();
						}

						// HScale
						if (t.hasAttribute("HScale"))
						{
							data->textObject->ct_text->hScale = t.attribute("HScale").as_double();
						}

						// ReadDirection
						if (t.hasAttribute("ReadDirection"))
						{
							data->textObject->ct_text->readDirection = t.attribute("ReadDirection").as_int();
						}

						// CharDirection
						if (t.hasAttribute("CharDirection"))
						{
							data->textObject->ct_text->charDirection = t.attribute("CharDirection").as_int();
						}

						data->textObject->ct_text->fillColor = new CT_Color();
						// FillColor
						if (!t.child("ofd:FillColor").isNull())
						{
							ST_Array fillColor = t.child("ofd:FillColor").attribute("Value").as_string();
							std::vector<std::string> strBox;
							split(fillColor, ' ', strBox);
							data->textObject->ct_text->fillColor->valueR = atof(strBox[0].c_str()) / 255.0f;
							data->textObject->ct_text->fillColor->valueG = atof(strBox[1].c_str()) / 255.0f;
							data->textObject->ct_text->fillColor->valueB = atof(strBox[2].c_str()) / 255.0f;
						}

						data->textObject->ct_text->strokeColor = new CT_Color();
						// StrokeColor
						if (!t.child("ofd:StrokeColor").isNull())
						{
							ST_Array strokeColor = t.child("ofd:StrokeColor").attribute("Value").as_string();
							std::vector<std::string> strBox;
							split(strokeColor, ' ', strBox);
							data->textObject->ct_text->strokeColor->valueR = atof(strBox[0].c_str()) / 255.0f;
							data->textObject->ct_text->strokeColor->valueG = atof(strBox[1].c_str()) / 255.0f;
							data->textObject->ct_text->strokeColor->valueB = atof(strBox[2].c_str()) / 255.0f;
						}

						// textCode
						xml_node t2 = t.child("ofd:TextCode");
						if (t2.isNull())
						{
							t = t.next_sibling("ofd:TextObject");
							continue;
						}
						//处理CT_CGTransform
						auto *textcode_and_ct_cgtransform = new TextCode_And_CT_CGTransform();
						xml_node tcgt = t.child("ofd:CGTransform");
						while (!tcgt.isNull())
						{
							auto * cgt = new CT_CGTransform();
							if (tcgt.hasAttribute("CodePosition"))
							{
								cgt->codePosition = tcgt.attribute("CodePosition").as_int();
							}
							else
							{
								tcgt = tcgt.next_sibling("ofd:CGTransform");
								continue;
							}
							if (tcgt.hasAttribute("CodeCount"))
							{
								cgt->codeCount = tcgt.attribute("CodeCount").as_int();
							}
							if (tcgt.hasAttribute("GlyphCount"))
							{
								cgt->glyphCount = tcgt.attribute("GlyphCount").as_int();
							}
							xml_node tglyphs = tcgt.child("ofd:Glyphs");
							if (tglyphs.isNull())
							{
								tcgt = tcgt.next_sibling("ofd:CGTransform");
								continue;
							}
							cgt->glyphs = tglyphs.text().as_string();
							textcode_and_ct_cgtransform->ct_cGTransforms.push_back(cgt);
							tcgt = tcgt.next_sibling("ofd:CGTransform");
						}

						while (!t2.isNull())
						{
							textcode_and_ct_cgtransform->textCode = new TextCode();
							if (t2.hasAttribute("X"))
							{
								last_x = textcode_and_ct_cgtransform->textCode->x = t2.attribute("X").as_double();
								if (is_first_textcode_x)
									is_first_textcode_x = false;
							}
							else
							{
								if (!is_first_textcode_x && !is_first_textcode_y)
									textcode_and_ct_cgtransform->textCode->x = last_x;
								else
								{
									t2 = t2.next_sibling("ofd:TextCode");
									continue;
								}
							}
							if (t2.hasAttribute("Y"))
							{
								last_y = textcode_and_ct_cgtransform->textCode->y = t2.attribute("Y").as_double();
								if (is_first_textcode_y)
									is_first_textcode_y = false;
							}
							else
							{
								if (!is_first_textcode_x && !is_first_textcode_y)
									textcode_and_ct_cgtransform->textCode->y = last_y;
								else
								{
									t2 = t2.next_sibling("ofd:TextCode");
									continue;
								}
							}
							if (t2.hasAttribute("DeltaX"))
							{
								ST_Array delta_x_data(t2.attribute("DeltaX").as_string());
								int a = delta_x_data.length();
								ST_Array_Delta delta_x;
								std::vector<std::string> strBox;
								split(delta_x_data, ' ', strBox);
								delta_x.push_back(0.0);
								for (size_t i = 0; i < strBox.size(); i++)
								{
									if (strBox[i].compare("g") == 0)
									{
										if (i + 2 < strBox.size())
										{
											for (int j = 0; j < atoi(strBox[i + 1].c_str()); j++)
											{
												delta_x.push_back(atof(strBox[i + 2].c_str()));
											}
											i += 2;
										}
										else
										{
											delta_x.push_back(0.0);
										}
									}
									else
									{
										delta_x.push_back(atof(strBox[i].c_str()));
									}
								}
								textcode_and_ct_cgtransform->textCode->deltaX = delta_x;
							}
							if (t2.hasAttribute("DeltaY"))
							{
								ST_Array delta_y_data(t2.attribute("DeltaY").as_string());
								ST_Array_Delta delta_y;
								std::vector<std::string> strBox;
								split(delta_y_data, ' ', strBox);
								delta_y.push_back(0.0);
								for (size_t i = 0; i < strBox.size(); i++)
								{
									if (strBox[i].compare("g") == 0)
									{
										if (i + 2 < strBox.size())
										{
											for (int j = 0; j < atoi(strBox[i + 1].c_str()); j++)
											{
												delta_y.push_back(atoi(strBox[i + 2].c_str()));
											}
											i += 2;
										}
										else
										{
											delta_y.push_back(0.0);
										}
									}
									else
									{
										delta_y.push_back(atof(strBox[i].c_str()));
									}
								}
								textcode_and_ct_cgtransform->textCode->deltaY = delta_y;
							}
							textcode_and_ct_cgtransform->textCode->str = t2.text().as_string();
							data->textObject->ct_text->textcode_and_ct_cgtransforms.push_back(textcode_and_ct_cgtransform);
							t2 = t2.next_sibling("ofd:TextCode");
						}
						layer_data->ct_pageBlock->d.push_back(std::make_pair(CT_PageBlock::TYPE::TEXTOBJECT, data));
					}

					//解析矢量图对象
					//t = new_layer.child("ofd:PathObject");
					if (std::string(t.name()).compare("ofd:PathObject") == 0)
					{
						auto *data = new CT_PageBlock::Data();
						data->pathObject = new PathObject();
						data->pathObject->ct_path = new CT_Path();
						data->pathObject->id = t.attribute("ID").as_uint();
						data->pathObject->ct_path->ct_graphicUnit = new CT_GraphicUnit();
						readGraphicUnit(t, data->pathObject->ct_path->ct_graphicUnit);
						if (t.hasAttribute("Stroke"))
						{
							data->pathObject->ct_path->stroke = t.attribute("Stroke").as_bool();
						}
						if (t.hasAttribute("Fill"))
						{
							data->pathObject->ct_path->fill = t.attribute("Fill").as_bool();
						}
						if (t.hasAttribute("LineWidth"))
						{
							data->pathObject->ct_path->ct_graphicUnit->lineWidth = t.attribute("LineWidth").as_double();
						}
						if (t.hasAttribute("Rule"))
						{
							if (strcmp(t.attribute("Rule").as_string(), "NonZero") == 0)
								data->pathObject->ct_path->rule = CT_Path::RULE::NONZERO;
							else if (strcmp(t.attribute("Rule").as_string(), "Even-Odd") == 0)
								data->pathObject->ct_path->rule = CT_Path::RULE::EVEN_ODD;
							else
							{
								return;
							}
						}

						data->pathObject->ct_path->fillColor = new CT_Color();
						// FillColor
						if (!t.child("ofd:FillColor").isNull())
						{
							ST_Array fillColor = t.child("ofd:FillColor").attribute("Value").as_string();
							std::vector<std::string> strBox;
							split(fillColor, ' ', strBox);
							data->pathObject->ct_path->fillColor->valueR = atof(strBox[0].c_str()) / 255.0f;
							data->pathObject->ct_path->fillColor->valueG = atof(strBox[1].c_str()) / 255.0f;
							data->pathObject->ct_path->fillColor->valueB = atof(strBox[2].c_str()) / 255.0f;
						}

						data->pathObject->ct_path->strokeColor = new CT_Color();
						// StrokeColor
						if (!t.child("ofd:StrokeColor").isNull())
						{
							ST_Array strokeColor = t.child("ofd:StrokeColor").attribute("Value").as_string();
							std::vector<std::string> strBox;
							split(strokeColor, ' ', strBox);
							data->pathObject->ct_path->strokeColor->valueR = atof(strBox[0].c_str()) / 255.0f;
							data->pathObject->ct_path->strokeColor->valueG = atof(strBox[1].c_str()) / 255.0f;
							data->pathObject->ct_path->strokeColor->valueB = atof(strBox[2].c_str()) / 255.0f;
						}

						// 图形变换的描述
						if (!t.child("ofd:AbbreviatedData").isNull())
						{
							ST_Array strAbbreviatedData = t.child("ofd:AbbreviatedData").text().as_string();
							std::vector<std::string> strBox;
							split(strAbbreviatedData, ' ', strBox);
							for (size_t i = 0; i < strBox.size(); i++)
							{
								if (strBox[i].compare("S") == 0)
								{
									AbbreviatedData *abb = new AbbreviatedData();
									abb->cMd = "S";
									abb->params.push_back(atof(strBox[i + 1].c_str()));
									abb->params.push_back(atof(strBox[i + 2].c_str()));
									i += 2;
									data->pathObject->ct_path->abbreviatedData.push_back(abb);
								}
								else if (strBox[i].compare("M") == 0)
								{
									AbbreviatedData *abb = new AbbreviatedData();
									abb->cMd = "M";
									abb->params.push_back(atof(strBox[i + 1].c_str()));
									abb->params.push_back(atof(strBox[i + 2].c_str()));
									i += 2;
									data->pathObject->ct_path->abbreviatedData.push_back(abb);
								}
								else if (strBox[i].compare("L") == 0)
								{
									AbbreviatedData *abb = new AbbreviatedData();
									abb->cMd = "L";
									abb->params.push_back(atof(strBox[i + 1].c_str()));
									abb->params.push_back(atof(strBox[i + 2].c_str()));
									i += 2;
									data->pathObject->ct_path->abbreviatedData.push_back(abb);
								}
								else if (strBox[i].compare("Q") == 0)
								{
									AbbreviatedData *abb = new AbbreviatedData();
									abb->cMd = "Q";
									abb->params.push_back(atof(strBox[i + 1].c_str()));
									abb->params.push_back(atof(strBox[i + 2].c_str()));
									abb->params.push_back(atof(strBox[i + 3].c_str()));
									abb->params.push_back(atof(strBox[i + 4].c_str()));
									i += 4;
									data->pathObject->ct_path->abbreviatedData.push_back(abb);
								}
								else if (strBox[i].compare("B") == 0)
								{
									AbbreviatedData *abb = new AbbreviatedData();
									abb->cMd = "B";
									abb->params.push_back(atof(strBox[i + 1].c_str()));
									abb->params.push_back(atof(strBox[i + 2].c_str()));
									abb->params.push_back(atof(strBox[i + 3].c_str()));
									abb->params.push_back(atof(strBox[i + 4].c_str()));
									abb->params.push_back(atof(strBox[i + 5].c_str()));
									abb->params.push_back(atof(strBox[i + 6].c_str()));
									i += 6;
									data->pathObject->ct_path->abbreviatedData.push_back(abb);
								}
								else if (strBox[i].compare("A") == 0)
								{
									AbbreviatedData *abb = new AbbreviatedData();
									abb->cMd = "A";
									abb->params.push_back(atof(strBox[i + 1].c_str()));
									abb->params.push_back(atof(strBox[i + 2].c_str()));
									abb->params.push_back(atof(strBox[i + 3].c_str()));
									abb->params.push_back(atof(strBox[i + 4].c_str()));
									abb->params.push_back(atof(strBox[i + 5].c_str()));
									abb->params.push_back(atof(strBox[i + 6].c_str()));
									abb->params.push_back(atof(strBox[i + 7].c_str()));
									i += 7;
									data->pathObject->ct_path->abbreviatedData.push_back(abb);
								}
								else if (strBox[i].compare("C") == 0)
								{
									AbbreviatedData *abb = new AbbreviatedData();
									abb->cMd = "C";
									data->pathObject->ct_path->abbreviatedData.push_back(abb);
								}
								else
								{
									continue;
								}
							}
						}
						else
						{
							return;
						}
						layer_data->ct_pageBlock->d.push_back(std::make_pair(CT_PageBlock::TYPE::PATHOBJECT, data));
					}

					//解析位图对象
					// ImageObject

					//t = new_layer.child("ofd:ImageObject");
					if (std::string(t.name()).compare("ofd:ImageObject") == 0)
					{
						auto *data = new CT_PageBlock::Data();
						data->imageObject = new ImageObject();
						data->imageObject->id = t.attribute("ID").as_uint();
						data->imageObject->ct_imageObject = new CT_Image();
						data->imageObject->ct_imageObject->ct_graphicUnit = new CT_GraphicUnit();
						readGraphicUnit(t, data->imageObject->ct_imageObject->ct_graphicUnit);             // 读取图元里的共用的数据
						if (t.hasAttribute("ResourceID"))
						{
							data->imageObject->ct_imageObject->resourceID = t.attribute("ResourceID").as_int();
						}
						else
						{
							return;
						}

						if (t.hasAttribute("Substitution"))
						{
							data->imageObject->ct_imageObject->substitution = t.attribute("Substitution").as_int();
						}
						layer_data->ct_pageBlock->d.push_back(std::make_pair(CT_PageBlock::TYPE::IMAGEOBJECT, data));
					}
					t = t.next_sibling();
				}
				page_data->content->layers.push_back(std::make_pair(i, layer_data));
				new_layer = new_layer.next_sibling("ofd:Layer");
			}
			std::sort(page_data->content->layers.begin(), page_data->content->layers.end(), [](const std::pair<ST_ID, CT_Layer*> &x, const std::pair<ST_ID, CT_Layer*> &y){return x.second->type < x.second->type; });
		}
	}
	else
	{
		return;
	}
}

void OFDParser::readGraphicUnit(xml_node root_node, CT_GraphicUnit *data)
{
	// Boundary
	if (root_node.hasAttribute("Boundary"))
	{
		std::string values = root_node.attribute("Boundary").as_string();
		std::vector<std::string> strBox;
		split(values, ' ', strBox);
		if (strBox.size() == 4)
		{
			data->boundary = new ST_Box();
			data->boundary->x = atof(strBox[0].c_str());
			data->boundary->y = atof(strBox[1].c_str());
			data->boundary->w = atof(strBox[2].c_str());
			data->boundary->h = atof(strBox[3].c_str());
		}
		else
		{
			return;
		}
	}
	
	// CTM
	if (root_node.hasAttribute("CTM"))
	{
		std::string values = root_node.attribute("CTM").as_string();
		std::vector<std::string> strCTM;
		split(values, ' ', strCTM);
		if (strCTM.size() == 6)
		{
			data->cTM = new CT_Matrix();
			data->cTM->xx = atof(strCTM[0].c_str());
			data->cTM->yx = atof(strCTM[1].c_str());
			data->cTM->xy = atof(strCTM[2].c_str());
			data->cTM->yy = atof(strCTM[3].c_str());
			data->cTM->x0 = atof(strCTM[4].c_str());
			data->cTM->y0 = atof(strCTM[5].c_str());
		}
		else
		{
			return;
		}
	}
}

void OFDParser::readFont(xml_node root_node, CT_Font *data, std::string path)
{
	if (root_node.hasAttribute("ID"))
	{
		data->id =root_node.attribute("ID").as_uint();
	}
	else
	{
		return;
	}

	if (root_node.hasAttribute("FontName"))
	{
		data->fontName = root_node.attribute("FontName").as_string("", "UTF8");
	}
	else
	{
		return;
	}

	if (root_node.hasAttribute("FamilyName"))
	{
		data->familyName = root_node.attribute("FamilyName").as_string("", "UTF8");
	}
	if (strlen(root_node.child("ofd:FontFile").text().as_string()) != 0)
	{
		data->fontFile = new ST_Loc();
		data->fontFile->pln = root_node.child("ofd:FontFile").text().as_string();
		data->fontFile->abs = root_node.child("ofd:FontFile").text().as_string();
		this->relativePath(path, data->fontFile->abs);
	}
	if (root_node.hasAttribute("Charset"))
	{
		std::string charset = root_node.attribute("Charset").as_string("", "GBK");
		if (charset.compare("symbol") == 0)
		{
			data->charset = CT_Font::CHARSET::SYMBOL;
		} 
		else if (charset.compare("prc") == 0)
		{
			data->charset = CT_Font::CHARSET::PRC;
		}
		else if (charset.compare("big5") == 0)
		{
			data->charset = CT_Font::CHARSET::BIG5;
		}
		else if (charset.compare("shift-jis") == 0)
		{
			data->charset = CT_Font::CHARSET::SHIFT_JIS;
		}
		else if (charset.compare("wansung") == 0)
		{
			data->charset = CT_Font::CHARSET::WANSUNG;
		}
		else if (charset.compare("johab") == 0)
		{
			data->charset = CT_Font::CHARSET::JOHAB;
		}
		else if (charset.compare("unicode") == 0)
		{
			data->charset = CT_Font::CHARSET::__UNICODE;
		}
		else
		{
			data->charset = CT_Font::CHARSET::__UNICODE;
		}
	}

	if (root_node.hasAttribute("Italic"))
	{
		data->italic = root_node.attribute("Italic").as_bool();
	}

	if (root_node.hasAttribute("Bold"))
	{
		data->bold = root_node.attribute("Bold").as_bool();
	}

	if (root_node.hasAttribute("Serif"))
	{
		data->serif = root_node.attribute("Serif").as_bool();
	}

	if (root_node.hasAttribute("FixedWidth"))
	{
		data->fixedWidth = root_node.attribute("FixedWidth").as_bool();
	}
}