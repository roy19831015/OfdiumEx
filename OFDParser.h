#pragma once
#include <string>
#include "OFD.h"
#include "pugixml.hpp"
using namespace pugi;

void OFD_free(OFD *ofd);
inline void DocBody_free(DocBody *docbody);
inline void Document_free(Document *doc);
inline void CT_DocInfo_free(CT_DocInfo *ct_docinfo);
inline void OFD_DataA_free(Res::TYPE type, OFD::DataA *dataA);
inline void CustomData_free(CustomData *customdata);
inline void OFD_DataA_free(Res::TYPE type, OFD::DataA *dataA);
inline void CT_ColorSpace_free(CT_ColorSpace *ct_colorspace);
inline void CT_DrawParam_free(CT_DrawParam *ct_drawparam);
inline void CT_Font_free(CT_Font *ct_font);
inline void CT_MultiMedia_free(CT_MultiMedia *ct_multimedia);
inline void CT_VectorG_free(CT_VectorG *ct_vectorg);
inline void CT_Color_free(CT_Color *ct_color);
inline void CT_Pattern_free(CT_Pattern * pattern);
inline void CT_AxialShd_free(CT_AxialShd * axialShd);
inline void CT_RadialShd_free(CT_RadialShd * radialShd);
inline void CT_GouraudShd_free(CT_GouraudShd * gouraudShd);
inline void CT_LaGouraudShd_free(CT_LaGouraudShd * laGouraudShd);
inline void CellContent_free(CellContent * cellContent);
inline void Segment_free(Segment * segment);
inline void CT_Point_free(CT_GouraudShd::Point *point);
inline void CT_Point_free(CT_LaGouraudShd::Point *point);
inline void CT_Action_free(CT_Action * actions);
inline void CT_Bookmark_free(CT_Bookmark * bm);
inline void CommonData_free(CommonData * commonData);
inline void CT_OutlineElem_free(CT_OutlineElem * ol);
inline void Document_Page_free(Document_Page * docpage);
inline void Page_free(Page * page);
inline void CT_Permission_free(CT_Permission * permissions);
inline void Signatures_free(Signatures * signatures);
inline void CT_VPreferences_free(CT_VPreferences * vPreferences);
inline void Goto_free(Goto * _goto);
inline void Uri_free(Uri * uri);
inline void GotoA_free(GotoA * gotoaA);
inline void Sound_free(Sound * sound);
inline void Movie_free(Movie * movie);
inline void Area_free(Area * area);
inline void Move_free(Move * move);
inline void Line_free(Line * line);
inline void QuadraticBezier_free(QuadraticBezier * quadraticBezier);
inline void CubicBezier_free(CubicBezier * cubicBezier);
inline void _Arc_free(_Arc * arc);
inline void CT_Dest_free(CT_Dest * dest);
inline void Res_free(Res * res);
inline void ColorSpaces_free(ColorSpaces * colorSpaces);
inline void DrawParam_free(DrawParams * drawParams);
inline void Fonts_free(Fonts * fonts);
inline void MultiMedias_free(MultiMedias * multiMedias);
inline void CompositeGraphicUnits_free(CompositeGraphicUnits * compositeGraphicUnits);
inline void CT_PageArea_free(CT_PageArea * area);
inline void Content_free(Content * content);
inline void CT_Layer_free(CT_Layer *layer);
inline void Print_free(Print * print);
inline void ValidPeriod_free(ValidPeriod * validPeriod);
inline void Signatures_Signature_free(Signatures_Signature * sign);
inline void Signature_free(Signature * sign);
inline void SignedInfo_free(SignedInfo * signedInfo);
inline void SignedValue_free(SignedValue * sv);
inline void CT_PageBlock_free(CT_PageBlock * ct_pageBlock);
inline void Provider_free(Provider * provider);
inline void References_free(References * references);
inline void Seal_free(Seal * seal);
inline void StampAnnot_free(StampAnnot * stampannot);
inline void ST_Box_free(ST_Box *box);
inline void TextObject_free(TextObject * textObject);
inline void PathObject_free(PathObject * pathObject);
inline void ImageObject_free(ImageObject * imageObject);
inline void CompositeObject_free(CompositeObject * compositeObject);
inline void PageBlock_free(PageBlock * pageBlock);
inline void CT_Composite_free(CT_Composite * ct_compositeObject);
inline void CT_Image_free(CT_Image * ct_imageObject);
inline void CT_Path_free(CT_Path * ct_path);
inline void CT_Text_free(CT_Text * ct_text);
inline void CT_GraphicUnit_free(CT_GraphicUnit * ct_graphicUnit);
inline void TextCode_And_CT_CGTransform_free(TextCode_And_CT_CGTransform * tac);
inline void CT_Clip_free(CT_Clip * clip);
inline void CT_Matrix_free(CT_Matrix * cTM);
inline void CT_CGTransform_free(CT_CGTransform * cg);
inline void TextCode_free(TextCode * textCode);
inline void AbbreviatedData_free(AbbreviatedData * abb);
inline void TemplatePage_free(TemplatePage * tplpage);

class OFDParser
{
public:
	OFDParser(std::string f) :
		ofd_dir(f)
	{
		document = new xml_document();
		std::string ofdfilepath = "ofd.xml";
		this->relativePath(this->ofd_dir, ofdfilepath);
		readOFD(ofdfilepath);
	}

	~OFDParser()
	{
		ofd_dir.clear();
		ofd_dir.shrink_to_fit();
		if (ofd)
		{
			OFD_free(ofd);
		}
		if (document)
		{
			delete document;
			document = nullptr;
		}
	}

	void relativePath(std::string dir, std::string &str);
	static bool OFDParser::isDir(const char *lpPath);
	OFD *getData();

	void openFile(std::string path);

	std::string ofd_dir = "";
protected:

private:
	OFD * ofd = nullptr;
	xml_document *document = nullptr;
	void readOFD(std::string path);
	Document *readDocument(std::string path);
	static void readOutlines(xml_node outlines_node, std::vector<CT_OutlineElem *>* outlines);
	static void readActions(xml_node actions_node, CT_OutlineElem * outlines);
	static void readAction(xml_node action_node, CT_Action *action);
	static void readPageArea(xml_node pageareas_node, CT_PageArea *data);
	static void readGraphicUnit(xml_node root_node, CT_GraphicUnit *data);
	void readFont(xml_node root_node, CT_Font *data, std::string path);
	void readResource(Res *res_data, std::string path);
	void readPage(Page *page_data, std::string path);
	void readTemplatePage(Page *page_data, std::string path);
	void readSignatures(Document * doc, std::string path);
	void readSignature(Signature * sig, std::string path);
	void readSignedValue(std::string path, unsigned char *data, size_t *len);
};


