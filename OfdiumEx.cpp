// OfdiumEx.cpp : 定义 DLL 应用程序的导出函数。
//
#include "stdafx.h"
#include "OfdiumEx.h"
#include <stdio.h>
#include <fstream>
#include "OFDParser.h"
#include "OFD.h"
#include "ziptool.h"
#include "util.h"
#include "cairo.h"
#include "cairo-win32.h"
#include "cairo-ft.h"
#include <map>
#pragma comment(lib,"cairo.lib")


#include "freetype/freetype.h"
#pragma comment(lib,"freetype.lib")

#include <comutil.h>
#ifdef _DEBUG
#pragma comment(lib, "comsuppwd.lib")
#else
#pragma comment(lib, "comsuppw.lib")
#endif

#include <gdiplus.h>
#include "RefCounter.h"

#include "base64.h"
#include <direct.h>

#include <algorithm>

std::map<OFD *, OFDParser *> listOpenedOFD;
std::map<cairo_surface_t *, std::pair<Gdiplus::Bitmap *,Gdiplus::BitmapData *>> listUsedImage;
ULONG_PTR g_gdiplusToken;
static RefCounter refCounter;
int ii = 0;

//public static extern IntPtr FPDF_LoadCustomDocument([MarshalAs(UnmanagedType.LPStruct)]FPDF_FILEACCESS access, string password);
OFDIUMEX_API void *OFDIUM_LoadCustomDocument(const char *fileName, unsigned long openMode)
{
	//_CrtSetBreakAlloc(1399);
	char tmp[MAX_PATH] = { 0 };
	GetTempPath(MAX_PATH, tmp);
	PathAddBackslash(tmp);
	PathAppend(tmp, GenerateGUIDString().c_str());
	std::string dir = tmp;
	ZipTool::extractDir(fileName, dir.c_str());
	OFDParser *ofdpaser = new OFDParser(dir);
	OFD *ofd = ofdpaser->getData();
	int id = 0;
	if (ofdpaser && ofd)
	{
		listOpenedOFD.insert(std::make_pair(ofd, ofdpaser));
	}
	return ofd;
}

OFDIUMEX_API void *OFDIUM_LoadMemDocument(const unsigned char *data, unsigned long len, unsigned long openMode)
{
	char tmp[MAX_PATH] = { 0 };
	GetTempPath(MAX_PATH, tmp);
	PathAddBackslash(tmp);
	PathAppend(tmp, GenerateGUIDString().append(".ofd").c_str());
	std::string tmpofdfilename = tmp;

	FILE *fp = nullptr;
	fopen_s(&fp, tmpofdfilename.c_str(), "wb");
	fwrite(data, 1, len, fp);
	fflush(fp);
	fclose(fp);

	return OFDIUM_LoadCustomDocument(tmpofdfilename.c_str(), openMode);
}

//public static extern int FPDF_GetPageCount(IntPtr document);
OFDIUMEX_API int OFDIUM_GetPageCount(void *fileHandle)
{
	if (listOpenedOFD.find((OFD *)fileHandle) == listOpenedOFD.end())
	{
		return -1;
	}
	auto *ofd = (OFD *)fileHandle;

	if (ofd->documents.size() != 1)
	{
		return -1;
	}
	Document *doc = ofd->documents[0];
	return doc->pages.size();
}

//public static extern uint FPDF_GetDocPermissions(IntPtr document);
OFDIUMEX_API void *OFDIUM_GetDocPermissions(void *fileHandle)
{
	if (listOpenedOFD.find((OFD *)fileHandle) == listOpenedOFD.end())
	{
		return nullptr;
	}
	auto *ofd = (OFD *)fileHandle;
	
	if (ofd->documents.size() != 1)
	{
		return nullptr;
	}
	Document *doc = ofd->documents[0];
	return doc->permissions;
}

//public static extern IntPtr FPDFDOC_InitFormFillEnvironment(IntPtr document, FPDF_FORMFILLINFO formInfo);
OFDIUMEX_API void *OFDIUM_InitFormFillEnvironment(void *fileHandle, void *formInfo)
{
	return nullptr;
}

//public static extern void FPDF_SetFormFieldHighlightColor(IntPtr hHandle, int fieldType, uint color);
OFDIUMEX_API void OFDIUM_SetFormFieldHighlightColor(void *hHandle, int fieldType, unsigned int color)
{
}

//public static extern void FPDF_SetFormFieldHighlightAlpha(IntPtr hHandle, byte alpha);
OFDIUMEX_API void OFDIUM_SetFormFieldHighlightAlpha(void *hHandle, unsigned char alpha)
{
}

//public static extern void FORM_DoDocumentJSAction(IntPtr hHandle);
OFDIUMEX_API void OFDIUM_DoDocumentJSAction(void *hHandle)
{
}

//public static extern void FORM_DoDocumentOpenAction(IntPtr hHandle);
OFDIUMEX_API void OFDIUM_DoDocumentOpenAction(void *hHandle)
{
}

//public static extern void FPDFDOC_ExitFormFillEnvironment(IntPtr hHandle);
OFDIUMEX_API void OFDIUM_ExitFormFillEnvironment(void *hHandle)
{
}

//public static extern void FORM_DoDocumentAAction(IntPtr hHandle, FPDFDOC_AACTION aaType);
OFDIUMEX_API void OFDIUM_DoDocumentAAction(void *hHandle, int aaType)
{
}

//public static extern IntPtr FPDF_LoadPage(IntPtr document, int page_index);
OFDIUMEX_API void *OFDIUM_LoadPage(void *fileHandle, int page_index)
{
	if (listOpenedOFD.find((OFD *)fileHandle) == listOpenedOFD.end())
	{
		return nullptr;
	}
	auto *ofd = (OFD *)fileHandle;

	if (ofd->documents.size() != 1)
	{
		return nullptr;
	}
	Document *doc = ofd->documents[0];
	if (page_index >= (int)doc->pages.size())
	{
		return nullptr;
	}
	return doc->pages[page_index].second;
}

//public static extern IntPtr FPDFText_LoadPage(IntPtr page);
OFDIUMEX_API void *OFDIUM_Text_LoadPage(void *fileHandle, int page_index)
{
	if (listOpenedOFD.find((OFD *)fileHandle) == listOpenedOFD.end())
	{
		return nullptr;
	}
	auto *ofd = (OFD *)fileHandle;

	if (ofd->documents.size() != 1)
	{
		return nullptr;
	}
	Document *doc = ofd->documents[0];
	if (page_index >= (int)doc->pages.size())
	{
		return nullptr;
	}
	return doc->pages[page_index].first;
}

//public static extern void FORM_OnAfterLoadPage(IntPtr page, IntPtr _form);
OFDIUMEX_API void OFDIUM_OnAfterLoadPage(void *pageHandle, void *formHandle)
{
}

//public static extern void FORM_DoPageAAction(IntPtr page, IntPtr _form, FPDFPAGE_AACTION fPDFPAGE_AACTION);
OFDIUMEX_API void OFDIUM_DoPageAAction(void *pageHandle, void *formHandle, int fPDFPAGE_AACTION)
{
}

//public static extern double FPDF_GetPageWidth(IntPtr page);
OFDIUMEX_API double OFDIUM_GetPageWidth(void *pageHandle)
{
	if (pageHandle == nullptr)
	{
		return 0.0f;
	}
	auto *page = (Page *)pageHandle;
	double page_w = page->area == nullptr ? MM2PIXEL72P(page->fromDoc->commonData->pageArea->physicalBox->w) : MM2PIXEL72P(page->area->physicalBox->w);
	return page_w;
}

//public static extern double FPDF_GetPageHeight(IntPtr page);
OFDIUMEX_API double OFDIUM_GetPageHeight(void *pageHandle)
{
	if (pageHandle == nullptr)
	{
		return 0.0f;
	}
	auto *page = (Page *)pageHandle;
	double page_h = page->area == nullptr ? MM2PIXEL72P(page->fromDoc->commonData->pageArea->physicalBox->h) : MM2PIXEL72P(page->area->physicalBox->h);
	return page_h;
}

//public static extern void FORM_OnBeforeClosePage(IntPtr page, IntPtr _form);
OFDIUMEX_API void OFDIUM_OnBeforeClosePage(void *pageHandle, void *formHandle)
{
}

//public static extern void FPDFText_ClosePage(IntPtr text_page);
OFDIUMEX_API void OFDIUM_Text_ClosePage(void *textPageHandle)
{
}

//public static extern void FPDF_ClosePage(IntPtr page);
OFDIUMEX_API void OFDIUM_ClosePage(void *pageHandle)
{
}

//public static extern void FPDF_RenderPage(IntPtr dc, IntPtr page, int start_x, int start_y, int size_x, int size_y, int rotate, FPDF flags);
OFDIUMEX_API void OFDIUM_RenderPage(void *dc, void *pageHandle, int start_x, int start_y, int size_x, int size_y, int rotate, int flags)
{
	auto *page = (Page *)pageHandle;
	return OFDIUM_RenderDocumentPageToDC(page->fromDoc, page->pageIndex, size_x, size_y, start_x, start_y, rotate, dc);
}

//public static extern void FPDF_RenderPageBitmap(IntPtr bitmapHandle, IntPtr page, int start_x, int start_y, int size_x, int size_y, int rotate, FPDF flags);
OFDIUMEX_API void OFDIUM_RenderPageBitmap(void *bitmapHandle, void *pageHandle, int start_x, int start_y, int size_x, int size_y, int rotate, int flags)
{
	auto *page = (Page *)pageHandle;
	cairo_surface_t *targetsurface = (cairo_surface_t *)bitmapHandle;
	return OFDIUM_RenderDocumentPageToSurface(page->fromDoc->fromOfd, page->pageIndex, size_x, size_y, start_x, start_y, rotate, targetsurface);
}

//public static extern int FPDF_GetPageSizeByIndex(IntPtr document, int page_index, out double width, out double height);
OFDIUMEX_API int OFDIUM_GetPageSizeByIndex(void *fileHandle, int page_index, double *width, double *height)
{
	if (listOpenedOFD.find((OFD *)fileHandle) == listOpenedOFD.end())
	{
		return NULL;
	}
	auto *ofd = (OFD *)fileHandle;

	if (ofd->documents.size() != 1)
	{
		return NULL;
	}
	Document *doc = ofd->documents[0];
	if (page_index >= (int)doc->pages.size())
	{
		return NULL;
	}
	Page *page = doc->pages[0].second;
	*width = page->area == nullptr ? MM2PIXEL72P(page->fromDoc->commonData->pageArea->physicalBox->w) : MM2PIXEL72P(page->area->physicalBox->w);
	*height = page->area == nullptr ? MM2PIXEL72P(page->fromDoc->commonData->pageArea->physicalBox->h) : MM2PIXEL72P(page->area->physicalBox->h);
	return 0;
}

//public static extern IntPtr FPDFBitmap_CreateEx(int width, int height, int format, IntPtr first_scan, int stride);
OFDIUMEX_API void *OFDIUM_Bitmap_CreateEx(int width, int height, int format, unsigned char *first_scan, int stride)
{
	cairo_surface_t *surfaceTarget = cairo_image_surface_create_for_data(first_scan, (cairo_format_t)format, width, height, stride);
	return surfaceTarget;
}

//public static extern void FPDFBitmap_FillRect(IntPtr bitmapHandle, int left, int top, int width, int height, uint color);
OFDIUMEX_API void OFDIUM_Bitmap_FillRect(void *bitmapHandle, int left, int top, int width, int height, unsigned int color)
{
	cairo_surface_t *targetsurface = (cairo_surface_t *)bitmapHandle;
	cairo_t *cr = cairo_create(targetsurface);
	cairo_set_source_rgba(cr, GetRValue(color), GetGValue(color), GetBValue(color), 255);
	cairo_rectangle(cr, left, top, width, height);
	cairo_fill(cr);
	cairo_destroy(cr);
}

//public static extern IntPtr FPDFBitmap_Destroy(IntPtr bitmapHandle);
OFDIUMEX_API void *OFDIUM_Bitmap_Destroy(void *bitmapHandle)
{
	cairo_surface_t *targetsurface = (cairo_surface_t *)bitmapHandle;
	cairo_surface_destroy(targetsurface);
	return nullptr;
}

//public static extern IntPtr FPDFText_FindStart(IntPtr page, byte[] findWhat, FPDF_SEARCH_FLAGS flags, int start_index);
OFDIUMEX_API void *OFDIUM_Text_FindStart(void *page, const unsigned short*findWhat, int flags, int start_index)
{
	return nullptr;
}

//public static extern int FPDFText_GetSchResultIndex(IntPtr handle);
OFDIUMEX_API int OFDIUM_Text_GetSchResultIndex(void *handle)
{
	return -1;
}

//public static extern int FPDFText_GetSchCount(IntPtr handle);
OFDIUMEX_API int OFDIUM_Text_GetSchCount(void *handle)
{
	return -1;
}

//public static extern int FPDFText_GetText(IntPtr page, int start_index, int count, byte[] result);
OFDIUMEX_API int OFDIUM_Text_GetText(void *pageHandle, int start_index, int count, unsigned short* result)
{
	return -1;
}

//public static extern void FPDFText_GetCharBox(IntPtr page, int index, out double left, out double right, out double bottom, out double top);
OFDIUMEX_API int OFDIUM_Text_GetCharBox(void *pageHandle, int index, double *left, double *right, double *bottom, double *top)
{
	return 0;
}

//public static extern int FPDFText_CountChars(IntPtr page);
OFDIUMEX_API int OFDIUM_Text_CountChars(void *pageHandle)
{
	return 0;
}

//public static extern bool FPDFText_FindNext(IntPtr handle);
OFDIUMEX_API bool OFDIUM_Text_FindNext(void *pageHandle)
{
	return true;
}

//public static extern void FPDFText_FindClose(IntPtr handle);
OFDIUMEX_API void OFDIUM_Text_FindClose(void *pageHandle)
{
}

//public static extern bool FPDFLink_Enumerate(IntPtr page, ref int startPos, out IntPtr linkAnnot);
OFDIUMEX_API bool OFDIUM_Link_Enumerate(void *pageHandle, int *startPos, void **linkAnnot)
{
	return false;
}

//public static extern IntPtr FPDFLink_GetDest(IntPtr document, IntPtr link);
OFDIUMEX_API void *OFDIUM_Link_GetDest(void *fileHandle, void *link)
{
	return nullptr;
}

//public static extern uint FPDFDest_GetPageIndex(IntPtr document, IntPtr dest);
OFDIUMEX_API unsigned int OFDIUM_Dest_GetPageIndex(void *fileHandle, void *dest)
{
	return 0;
}

//public static extern bool FPDFLink_GetAnnotRect(IntPtr linkAnnot, FS_RECTF rect);
OFDIUMEX_API bool OFDIUM_Link_GetAnnotRect(void *linkAnnot, void *rect)
{
	return false;
}

//public static extern void FPDF_DeviceToPage(IntPtr page, int start_x, int start_y, int size_x, int size_y, int rotate, int device_x, int device_y, out double page_x, out double page_y);
OFDIUMEX_API void OFDIUM_DeviceToPage(void *pageHandle, int start_x, int start_y, int size_x, int size_y, int rotate, int device_x, int device_y, double *page_x, double *page_y)
{
}

//public static extern void FPDF_PageToDevice(IntPtr page, int start_x, int start_y, int size_x, int size_y, int rotate, double page_x, double page_y, out int device_x, out int device_y);
OFDIUMEX_API void OFDIUM_PageToDevice(void *pageHandle, int start_x, int start_y, int size_x, int size_y, int rotate, double page_x, double page_y, int *device_x, int *device_y)
{
}

//public static extern IntPtr FPDFLink_GetAction(IntPtr link);
OFDIUMEX_API void *OFDIUM_Link_GetAction(void *link)
{
	return nullptr;
}

//public static extern uint FPDFAction_GetURIPath(IntPtr document, IntPtr action, StringBuilder buffer, uint buflen);
OFDIUMEX_API unsigned long OFDIUM_Action_GetURIPath(void *fileHandle, void *action, void *buffer, unsigned long buflen)
{
	return 0;
}

//public static extern IntPtr FPDFBookmark_GetFirstChild(IntPtr document, IntPtr bookmark);
OFDIUMEX_API void *OFDIUM_Bookmark_GetFirstChild(void *fileHandle, void *bookmark)
{
	return nullptr;
}

//public static extern IntPtr FPDFBookmark_GetNextSibling(IntPtr document, IntPtr bookmark);
OFDIUMEX_API void *OFDIUM_Bookmark_GetNextSibling(void *fileHandle, void *bookmark)
{
	return nullptr;
}

//public static extern uint FPDFBookmark_GetTitle(IntPtr bookmark, byte[] buffer, uint buflen);
OFDIUMEX_API unsigned long OFDIUM_Bookmark_GetTitle(void *bookmark, void *buffer, unsigned long buflen)
{
	return NULL;
}

//public static extern IntPtr FPDFBookmark_GetAction(IntPtr bookmark);
OFDIUMEX_API void *OFDIUM_Bookmark_GetAction(void *bookmark)
{
	return nullptr;
}

//public static extern IntPtr FPDFBookmark_GetDest(IntPtr document, IntPtr bookmark);
OFDIUMEX_API void *OFDIUM_Bookmark_GetDest(void *fileHandle, void *bookmark)
{
	return nullptr;
}

//public static extern uint FPDFAction_GetType(IntPtr action);
OFDIUMEX_API unsigned long OFDIUM_Action_GetType(void *action)
{
	return NULL;
}

//public static extern uint FPDF_GetLastError();
OFDIUMEX_API unsigned long OFDIUM_GetLastError()
{
	return 0;
}

//public static extern uint FPDF_GetMetaText(IntPtr document, string tag, byte[] buffer, uint buflen);
OFDIUMEX_API unsigned long OFDIUM_GetMetaText(void *fileHandle, char *tag, void *buffer, unsigned long buflen)
{
	return NULL;
}

//public static extern void FPDF_FFLDraw(IntPtr form, IntPtr bitmap, IntPtr page, int start_x, int start_y, int size_x, int size_y, int rotate, FPDF flags);
OFDIUMEX_API void OFDIUM_FFLDraw(void *form, void *bitmap, void *page, int start_x, int start_y, int size_x, int size_y, int rotate, int flags)
{
}

//public static extern void FPDF_FFLDrawEx(IntPtr form, IntPtr bitmap, IntPtr page, int start_x, int start_y, int size_x, int size_y, int rotate, FPDF flags, string strJson);
OFDIUMEX_API void OFDIUM_FFLDrawEx(void *form, void *bitmap, void *page, int start_x, int start_y, int size_x, int size_y, int rotate, int flags, char *strJson)
{
}
//

OFDIUMEX_API int OFDIUM_GetDocumentPageWidth(void *fileHandle, int pageIndex)
{
	if (listOpenedOFD.find((OFD *)fileHandle) == listOpenedOFD.end())
	{
		return -1;
	}
	auto *ofd = (OFD *)fileHandle;

	if (ofd->documents.size() != 1)
	{
		return -1;
	}

	Document *doc = ofd->documents[0];

	if ((int)doc->pages.size() - 1 < pageIndex || doc->pages[pageIndex].second == nullptr)
	{
		return -1;
	}

	Page *page = doc->pages[pageIndex].second;

	int page_w = page->area == nullptr ? INTMM2PIXEL72P(doc->commonData->pageArea->physicalBox->w) : INTMM2PIXEL72P(page->area->physicalBox->w);
	return page_w;
}

OFDIUMEX_API int OFDIUM_GetDocumentPageHeight(void *fileHandle, int pageIndex)
{
	if (listOpenedOFD.find((OFD *)fileHandle) == listOpenedOFD.end())
	{
		return -1;
	}
	auto *ofd = (OFD *)fileHandle;

	if (ofd->documents.size() != 1)
	{
		return -1;
	}

	Document *doc = ofd->documents[0];

	if ((int)doc->pages.size() - 1 < pageIndex || doc->pages[pageIndex].second == nullptr)
	{
		return -1;
	}

	Page *page = doc->pages[pageIndex].second;

	int page_h = page->area == nullptr ? INTMM2PIXEL72P(doc->commonData->pageArea->physicalBox->h) : INTMM2PIXEL72P(page->area->physicalBox->h);
	return page_h;
}

void renderSealToCairoDevice(Document_Page *docpage, Signatures *signatures, cairo_t *cr)
{
	for (std::pair<Signatures_Signature *, Signature *> ss : signatures->signatures)
	{
		for (StampAnnot *sa : ss.second->signedInfo->stampAnnots)
		{
			if (docpage->id == sa->pageRef)
			{
				SignedValue *signedValue = ss.second->signedValue.second;
				V4_SES_Signature *v4_ses_signature = V4_SES_Signature_new();
				const unsigned char *tmp = signedValue->val;

				//std::ofstream outfile;
				//outfile.open("s.data");
				//outfile.write((const char *)signedValue->val, signedValue->len);
				//outfile.flush();
				//outfile.close();

				d2i_V4_SES_Signature(&v4_ses_signature, &tmp, signedValue->len);
				//print_errors();
				SES_Signature *v1_ses_signature = SES_Signature_new();
				if (v4_ses_signature==NULL)
				{
					d2i_SES_Signature(&v1_ses_signature, &tmp, signedValue->len);
					if (v1_ses_signature == NULL)
					{
						continue;
					}
				}
				unsigned char *imgdata = v4_ses_signature != NULL ? v4_ses_signature->toSign->eseal->esealInfo->picture->data->data : v1_ses_signature->toSign->eseal->esealInfo->picture->data->data;
				int imglen = v4_ses_signature != NULL ? v4_ses_signature->toSign->eseal->esealInfo->picture->data->length : v1_ses_signature->toSign->eseal->esealInfo->picture->data->length;
				st_png_data pngData = { (unsigned char*)imgdata, 0 };

				cairo_move_to(cr, MM2PIXEL(sa->boundary->x), MM2PIXEL(sa->boundary->y));
				std::string imgtype;
				imgtype.assign((char *)(v4_ses_signature != NULL ? v4_ses_signature->toSign->eseal->esealInfo->picture->type->data : v1_ses_signature->toSign->eseal->esealInfo->picture->type->data), v4_ses_signature != NULL ? v4_ses_signature->toSign->eseal->esealInfo->picture->type->length : v1_ses_signature->toSign->eseal->esealInfo->picture->type->length);
				if (_stricmp(imgtype.c_str(), "PNG") == 0)
				{
					//FILE *f = NULL;
					//fopen_s(&f, "save.png", "wb+");
					//fwrite(imgdata, 1, imglen, f);
					//fflush(f);
					//fclose(f);

					cairo_save(cr);
					cairo_surface_t *imagesurface = cairo_image_surface_create_from_png_stream(cairo_read_func_mine, &pngData);
					cairo_t *crimg = cairo_create(imagesurface);
					int image_w = cairo_image_surface_get_width(imagesurface);
					int image_h = cairo_image_surface_get_height(imagesurface);
					int boundary_w = INTMM2PIXEL(sa->boundary->w);
					int boundary_h = INTMM2PIXEL(sa->boundary->h);
					double factor_w = (double)boundary_w / (double)image_w;
					double factor_h = (double)boundary_h / (double)image_h;
					cairo_scale(cr, factor_w, factor_h);
					imagesurface = cairo_get_target(crimg);
					int image_w2 = cairo_image_surface_get_width(imagesurface);
					int image_h2 = cairo_image_surface_get_height(imagesurface);

					if (sa->clip)
					{
						double clip_x = MM2PIXEL(sa->boundary->x + sa->clip->x) / factor_w;
						double clip_y = MM2PIXEL(sa->boundary->y + sa->clip->y) / factor_h;
						double clip_w = MM2PIXEL(sa->clip->w) / factor_w;
						double clip_h = MM2PIXEL(sa->clip->h) / factor_h;
						cairo_rectangle(cr, clip_x, clip_y, clip_w, clip_h);
						cairo_clip(cr);
					}
					cairo_set_source_surface(cr, imagesurface, MM2PIXEL(sa->boundary->x) / factor_w, MM2PIXEL(sa->boundary->y) / factor_h);
					cairo_paint(cr);
					cairo_surface_destroy(imagesurface);
					cairo_restore(cr);
				}
				else if (_stricmp(imgtype.c_str(), "OFD") == 0)
				{
					cairo_save(cr);
					cairo_surface_t *imagesurface = (cairo_surface_t *)cairo_image_surface_create_from_ofd_stream(imgdata, imglen, INTMM2PIXEL(ASN1_INTEGER_get(v4_ses_signature->toSign->eseal->esealInfo->picture->width)), INTMM2PIXEL(ASN1_INTEGER_get(v4_ses_signature->toSign->eseal->esealInfo->picture->height)));
					if (imagesurface)
					{
						cairo_t *crimg = cairo_create(imagesurface);
						int image_w = cairo_image_surface_get_width(imagesurface);
						int image_h = cairo_image_surface_get_height(imagesurface);
						int boundary_w = INTMM2PIXEL(sa->boundary->w);
						int boundary_h = INTMM2PIXEL(sa->boundary->h);
						double factor_w = (double)boundary_w / (double)image_w;
						double factor_h = (double)boundary_h / (double)image_h;
						cairo_scale(cr, factor_w, factor_h);
						imagesurface = cairo_get_target(crimg);
						int image_w2 = cairo_image_surface_get_width(imagesurface);
						int image_h2 = cairo_image_surface_get_height(imagesurface);

						if (sa->clip)
						{
							double clip_x = MM2PIXEL(sa->boundary->x + sa->clip->x) / factor_w;
							double clip_y = MM2PIXEL(sa->boundary->y + sa->clip->y) / factor_h;
							double clip_w = MM2PIXEL(sa->clip->w) / factor_w;
							double clip_h = MM2PIXEL(sa->clip->h) / factor_h;
							cairo_rectangle(cr, clip_x, clip_y, clip_w, clip_h);
							cairo_clip(cr);
						}
						cairo_set_source_surface(cr, imagesurface, MM2PIXEL(sa->boundary->x) / factor_w, MM2PIXEL(sa->boundary->y) / factor_h);
						cairo_paint(cr);
						cairo_surface_destroy(imagesurface);
					}
					cairo_restore(cr);
				}
				SES_Signature_free(v1_ses_signature);
				V4_SES_Signature_free(v4_ses_signature);
			}
		}
	}
}

void renderDocumentTemplatePageToCairoDevice(OFD *ofd, Document* doc, cairo_t *cr)
{
	if (doc == NULL || doc->commonData == NULL || doc->commonData->templatePages.size() == 0 || ofd == NULL)
	{
		return;
	}
	for (size_t ti = 0; ti < doc->commonData->templatePages.size();ti++)
	{
		for (std::pair<ST_ID, CT_Layer*> layerpair : doc->commonData->templatePages[ti].second->content->layers)
		{
			for (std::pair<CT_PageBlock::TYPE, CT_PageBlock::Data *> pageblockobject : layerpair.second->ct_pageBlock->d)
			{
				if (pageblockobject.first == CT_PageBlock::TYPE::TEXTOBJECT)
				{
					cairo_save(cr);
					CT_Font *font = ofd->resAll->at(pageblockobject.second->textObject->ct_text->font).second->font;
					std::string family = font->familyName.empty() ? "SimSun" : font->familyName;
					cairo_surface_t *crtextsurface = cairo_image_surface_create(cairo_format_t::CAIRO_FORMAT_ARGB32, INTMM2PIXEL(pageblockobject.second->textObject->ct_text->ct_graphicUnit->boundary->w), INTMM2PIXEL(pageblockobject.second->textObject->ct_text->ct_graphicUnit->boundary->h));
					cairo_t *cttext = cairo_create(crtextsurface);

					cairo_font_options_t *option = cairo_font_options_create();
					cairo_font_options_set_antialias(option, cairo_antialias_t::CAIRO_ANTIALIAS_BEST);
					cairo_set_font_options(cttext, option);
					cairo_select_font_face(cttext,
						family.c_str(),
						font->italic ? CAIRO_FONT_SLANT_ITALIC : CAIRO_FONT_SLANT_NORMAL,
						font->bold || pageblockobject.second->textObject->ct_text->weight >= 700 ? CAIRO_FONT_WEIGHT_BOLD : CAIRO_FONT_WEIGHT_NORMAL);
					double ptSize = MM2PIXEL(pageblockobject.second->textObject->ct_text->size);
					cairo_set_font_size(cttext, ptSize);
					//TRACE("%d,%d,%d,%d\r\n", pageblockobject.second.textObject->ct_text->fillColor->valueR, pageblockobject.second.textObject->ct_text->fillColor->valueG, pageblockobject.second.textObject->ct_text->fillColor->valueB, pageblockobject.second.textObject->ct_text->fillColor->alpha);
					cairo_set_source_rgba(cttext, pageblockobject.second->textObject->ct_text->fillColor->valueR, pageblockobject.second->textObject->ct_text->fillColor->valueG, pageblockobject.second->textObject->ct_text->fillColor->valueB, pageblockobject.second->textObject->ct_text->fillColor->alpha);
					for (TextCode_And_CT_CGTransform* textcode_and_ct_cgtransform : pageblockobject.second->textObject->ct_text->textcode_and_ct_cgtransforms)
					{
						if (pageblockobject.second->textObject->ct_text->ct_graphicUnit->cTM != NULL)
						{
							cairo_matrix_t *ctm = new cairo_matrix_t();
							ct_matrix_to_cairo_matrix(pageblockobject.second->textObject->ct_text->ct_graphicUnit->cTM, ctm);
							cairo_set_matrix(cttext, ctm);
							delete ctm;
						}
						if (textcode_and_ct_cgtransform->textCode->deltaX.size() > 1 || textcode_and_ct_cgtransform->textCode->deltaY.size() > 1)
						{
							std::vector<std::string> vec;
							fnReadCharactersUTF8(textcode_and_ct_cgtransform->textCode->str.c_str(), vec);
							double x0 = MM2PIXEL(textcode_and_ct_cgtransform->textCode->x);
							double y0 = MM2PIXEL(textcode_and_ct_cgtransform->textCode->y);
							double toMoveX = x0;
							double toMoveY = y0;
							for (size_t i = 0; i < vec.size(); i++)
							{
								std::string strCh = vec[i];
								double deltaX = i < textcode_and_ct_cgtransform->textCode->deltaX.size() ? (MM2PIXEL(textcode_and_ct_cgtransform->textCode->deltaX[i])) : 0.0f;
								double deltaY = i < textcode_and_ct_cgtransform->textCode->deltaY.size() ? (MM2PIXEL(textcode_and_ct_cgtransform->textCode->deltaY[i])) : 0.0f;
								toMoveX += deltaX;
								toMoveY += deltaY;
								{
									cairo_move_to(
										cttext,
										toMoveX,
										toMoveY);
									cairo_show_text(cttext, strCh.c_str());
								}
							}
						}
						else
						{
							cairo_move_to(cttext, MM2PIXEL(textcode_and_ct_cgtransform->textCode->x), MM2PIXEL(textcode_and_ct_cgtransform->textCode->y));
							cairo_show_text(cttext, textcode_and_ct_cgtransform->textCode->str.c_str());
						}
					}

					//char tmp[MAX_PATH] = { 0 };
					//itoa(ii, tmp, 10);
					//std::string outpath = tmp;
					//outpath += ".png";
					//cairo_surface_write_to_png(crtextsurface, outpath.c_str());
					//ii++;

					cairo_set_source_surface(cr, crtextsurface, MM2PIXEL(pageblockobject.second->textObject->ct_text->ct_graphicUnit->boundary->x), MM2PIXEL(pageblockobject.second->textObject->ct_text->ct_graphicUnit->boundary->y));
					cairo_paint(cr);
					cairo_restore(cr);
				}
				else if (pageblockobject.first == CT_PageBlock::TYPE::IMAGEOBJECT)
				{
					cairo_move_to(cr, MM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->x), MM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->y));

					if (ofd->resAll->find(pageblockobject.second->imageObject->ct_imageObject->resourceID) != ofd->resAll->end())
					{
						CT_MultiMedia *multimedia = ofd->resAll->at(pageblockobject.second->imageObject->ct_imageObject->resourceID).second->multiMedia;
						if (multimedia->format.compare("PNG") == 0)
						{
							cairo_save(cr);
							cairo_surface_t *imagesurface = cairo_image_surface_create_from_png(multimedia->MediaFile->abs.c_str());
							cairo_t *crimg = cairo_create(imagesurface);
							int image_w = cairo_image_surface_get_width(imagesurface);
							int image_h = cairo_image_surface_get_height(imagesurface);
							int boundary_w = INTMM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->w);
							int boundary_h = INTMM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->h);
							double factor_w = (double)boundary_w / (double)image_w;
							double factor_h = (double)boundary_h / (double)image_h;
							cairo_scale(cr, factor_w, factor_h);
							imagesurface = cairo_get_target(crimg);
							int image_w2 = cairo_image_surface_get_width(imagesurface);
							int image_h2 = cairo_image_surface_get_height(imagesurface);
							cairo_set_source_surface(cr, imagesurface, MM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->x), MM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->y));
							cairo_paint(cr);
							cairo_surface_destroy(imagesurface);
							cairo_restore(cr);
						}
						else if (multimedia->format.compare("JB2") == 0)
						{
							cairo_save(cr);
							int image_w = 0;
							int image_h = 0;
							unsigned char*tmp = NULL;
							cairo_surface_t *imagesurface = cairo_image_surface_create_from_jbig2(multimedia->MediaFile->abs.c_str(), &image_w, &image_h, &tmp);
							cairo_t *crimg = cairo_create(imagesurface);
							//int image_w = cairo_image_surface_get_width(imagesurface);
							//int image_h = cairo_image_surface_get_height(imagesurface);
							int boundary_w = INTMM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->w);
							int boundary_h = INTMM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->h);
							double factor_w = (double)boundary_w / (double)image_w;
							double factor_h = (double)boundary_h / (double)image_h;
							cairo_scale(cr, factor_w, factor_h);
							imagesurface = cairo_get_target(crimg);
							int image_w2 = cairo_image_surface_get_width(imagesurface);
							int image_h2 = cairo_image_surface_get_height(imagesurface);
							double image_x = MM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->x) / factor_w;
							double image_y = MM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->y) / factor_h;

							//char tmp[MAX_PATH] = { 0 };
							//_itoa_s(ii, tmp, 10);
							//std::string outpath = tmp;
							//outpath += ".png";
							//cairo_surface_write_to_png(imagesurface, outpath.c_str());
							//ii++;

							cairo_set_source_surface(cr, imagesurface, image_x, image_y);
							cairo_paint(cr);
							cairo_surface_destroy(imagesurface);
							free(tmp);
							cairo_restore(cr);
						}
						else
						{
							cairo_save(cr);
							cairo_surface_t *imagesurface = cairo_image_surface_create_from_jpeg(multimedia->MediaFile->abs.c_str(), &listUsedImage, &g_gdiplusToken);
							cairo_t *crimg = cairo_create(imagesurface);
							int image_w = cairo_image_surface_get_width(imagesurface);
							int image_h = cairo_image_surface_get_height(imagesurface);
							int boundary_w = INTMM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->w);
							int boundary_h = INTMM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->h);
							double factor_w = (double)boundary_w / (double)image_w;
							double factor_h = (double)boundary_h / (double)image_h;
							cairo_scale(cr, factor_w, factor_h);
							imagesurface = cairo_get_target(crimg);
							int image_w2 = cairo_image_surface_get_width(imagesurface);
							int image_h2 = cairo_image_surface_get_height(imagesurface);
							double image_x = MM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->x) / factor_w;
							double image_y = MM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->y) / factor_h;

							cairo_set_source_surface(cr, imagesurface, image_x, image_y);
							cairo_paint(cr);
							cairo_image_surface_delete_from_jpeg(imagesurface, &listUsedImage, &g_gdiplusToken);
							cairo_surface_destroy(imagesurface);
							cairo_restore(cr);
						}
					}
				}
				else if (pageblockobject.first == CT_PageBlock::TYPE::PATHOBJECT)
				{
					cairo_move_to(cr, MM2PIXEL(pageblockobject.second->pathObject->ct_path->ct_graphicUnit->boundary->x), MM2PIXEL(pageblockobject.second->pathObject->ct_path->ct_graphicUnit->boundary->y));
					cairo_save(cr);
					cairo_surface_t *imagesurface = cairo_image_surface_create(cairo_format_t::CAIRO_FORMAT_ARGB32, INTMM2PIXEL(pageblockobject.second->pathObject->ct_path->ct_graphicUnit->boundary->w), INTMM2PIXEL(pageblockobject.second->pathObject->ct_path->ct_graphicUnit->boundary->h));
					cairo_t *crimg = cairo_create(imagesurface);
					double x0 = 0.0f;
					double y0 = 0.0f;
					for (AbbreviatedData * abb : pageblockobject.second->pathObject->ct_path->abbreviatedData)
					{
						if (abb->cMd.compare("S") == 0)
						{
							double x0 = abb->params[0];
							double y0 = abb->params[1];
						}
						else if (abb->cMd.compare("M") == 0)
						{
							double x = abb->params[0];
							double y = abb->params[1];
							cairo_move_to(crimg, MM2PIXEL(x), MM2PIXEL(y));
						}
						else if (abb->cMd.compare("L") == 0)
						{
							double x = abb->params[0];
							double y = abb->params[1];
							cairo_line_to(crimg, MM2PIXEL(x), MM2PIXEL(y));
						}
						else if (abb->cMd.compare("Q") == 0)
						{
							double x1 = abb->params[0];
							double y1 = abb->params[1];
							double x2 = abb->params[2];
							double y2 = abb->params[3];
							cairo_quadratic_to(crimg, MM2PIXEL(x1), MM2PIXEL(y1), MM2PIXEL(x2), MM2PIXEL(y2));
						}
						else if (abb->cMd.compare("B") == 0)
						{
							double x1 = abb->params[0];
							double y1 = abb->params[1];
							double x2 = abb->params[2];
							double y2 = abb->params[3];
							double x3 = abb->params[4];
							double y3 = abb->params[5];
							cairo_curve_to(crimg, MM2PIXEL(x1), MM2PIXEL(y1), MM2PIXEL(x2), MM2PIXEL(y2), MM2PIXEL(x3), MM2PIXEL(y3));
						}
						else if (abb->cMd.compare("A") == 0)
						{
							double rx = abb->params[0];
							double ry = abb->params[1];
							double angle1 = abb->params[2];
							double large = abb->params[3];
							double sweep = abb->params[4];
							double x = abb->params[5];
							double y = abb->params[6];
							//TODO 只会已知原点和长短轴及起始终止角度的椭圆圆弧
							double x2, y2;
							cairo_get_current_point(crimg, &x2, &y2);
							cairo_quadratic_to(crimg, x2, y2, MM2PIXEL(x), MM2PIXEL(y));
						}
						else if (abb->cMd.compare("C") == 0)
						{
							cairo_close_path(crimg);
						}
						else
						{
							continue;
						}
					}
					if (pageblockobject.second->pathObject->ct_path->stroke)
					{
						cairo_set_source_rgba(crimg, pageblockobject.second->pathObject->ct_path->strokeColor->valueR, pageblockobject.second->pathObject->ct_path->strokeColor->valueG, pageblockobject.second->pathObject->ct_path->strokeColor->valueB, pageblockobject.second->pathObject->ct_path->strokeColor->alpha);
						cairo_stroke(crimg);
					}
					if (pageblockobject.second->pathObject->ct_path->fill)
					{
						cairo_set_source_rgba(crimg, pageblockobject.second->pathObject->ct_path->fillColor->valueR, pageblockobject.second->pathObject->ct_path->fillColor->valueG, pageblockobject.second->pathObject->ct_path->fillColor->valueB, pageblockobject.second->pathObject->ct_path->fillColor->alpha);
						cairo_stroke(crimg);
					}

					cairo_set_source_surface(cr, imagesurface, MM2PIXEL(pageblockobject.second->pathObject->ct_path->ct_graphicUnit->boundary->x), MM2PIXEL(pageblockobject.second->pathObject->ct_path->ct_graphicUnit->boundary->y));
					cairo_paint(cr);
					cairo_surface_destroy(imagesurface);
					cairo_restore(cr);
				}
			}
		}
	}
	//char tmp[MAX_PATH] = { 0 };
	//itoa(ii, tmp, 10);
	//std::string outpath = tmp;
	//outpath += ".png";
	//cairo_surface_write_to_png(cairo_get_target(cr), outpath.c_str());
	//ii++;
}

OFDIUMEX_API void OFDIUM_GetDocumentPagePNG(void *fileHandle, int pageIndex, int width, int height, unsigned char *pngData, int *pngDataLen)
{
	if (pngData==nullptr)
	{
		*pngDataLen = width * height * 4;
		return;
	}
	if (listOpenedOFD.find((OFD *)fileHandle) == listOpenedOFD.end())
	{
		return;
	}
	auto *ofd = (OFD *)fileHandle;

	if (ofd->documents.size() != 1)
	{
		return;
	}

	Document *doc = ofd->documents[0];

	if ((int)doc->pages.size() - 1 < pageIndex || doc->pages[pageIndex].second == nullptr)
	{
		return;
	}

	Page *page = doc->pages[pageIndex].second;
	Document_Page *docpage = doc->pages[pageIndex].first;

	cairo_surface_t *surface, *surfaceTarget;
	cairo_t *cr, *crtarget;



	int page_w = page->area == nullptr ? INTMM2PIXEL(doc->commonData->pageArea->physicalBox->w) : INTMM2PIXEL(page->area->physicalBox->w);
	int page_h = page->area == nullptr ? INTMM2PIXEL(doc->commonData->pageArea->physicalBox->h) : INTMM2PIXEL(page->area->physicalBox->h);
	int page_x = page->area == nullptr ? INTMM2PIXEL(doc->commonData->pageArea->physicalBox->x) : INTMM2PIXEL(page->area->physicalBox->x);
	int page_y = page->area == nullptr ? INTMM2PIXEL(doc->commonData->pageArea->physicalBox->y) : INTMM2PIXEL(page->area->physicalBox->y);

	surfaceTarget = cairo_image_surface_create(cairo_format_t::CAIRO_FORMAT_ARGB32, width, height);
	crtarget = cairo_create(surfaceTarget);

	surface = cairo_image_surface_create(cairo_format_t::CAIRO_FORMAT_ARGB32, page_w, page_h);
	cr = cairo_create(surface);

	//cairo_set_source_rgba(cr, 255, 255, 255, 255);
	//cairo_rectangle(cr, page_x, page_y, page_w, page_h);
	//cairo_fill(cr);

	renderDocumentTemplatePageToCairoDevice(ofd, doc, cr);
	
	//char tmp[MAX_PATH] = { 0 };
	//itoa(ii, tmp, 10);
	//std::string outpath = tmp;
	//outpath += ".png";
	//cairo_surface_write_to_png(cairo_get_target(cr), outpath.c_str());
	//ii++;

	for (std::pair<ST_ID, CT_Layer*> layerpair : page->content->layers)
	{
		for (std::pair<CT_PageBlock::TYPE, CT_PageBlock::Data*> pageblockobject : layerpair.second->ct_pageBlock->d)
		{
			if (pageblockobject.first == CT_PageBlock::TYPE::TEXTOBJECT)
			{
				cairo_save(cr);
				CT_Font *font = ofd->resAll->at(pageblockobject.second->textObject->ct_text->font).second->font;
				std::string family = font->familyName.empty() ? (font->fontName.empty() ? "SimSun" : font->fontName) : font->familyName;
				cairo_surface_t *crtextsurface = cairo_image_surface_create(cairo_format_t::CAIRO_FORMAT_ARGB32, INTMM2PIXEL(pageblockobject.second->textObject->ct_text->ct_graphicUnit->boundary->w), INTMM2PIXEL(pageblockobject.second->textObject->ct_text->ct_graphicUnit->boundary->h));
				cairo_t *cttext = cairo_create(crtextsurface);

				cairo_font_options_t *option = cairo_font_options_create();
				cairo_font_options_set_antialias(option, cairo_antialias_t::CAIRO_ANTIALIAS_BEST);
				cairo_set_font_options(cttext, option);
				cairo_select_font_face(cttext,
					family.c_str(),
					font->italic ? CAIRO_FONT_SLANT_ITALIC : CAIRO_FONT_SLANT_NORMAL,
					font->bold || pageblockobject.second->textObject->ct_text->weight >= 700 ? CAIRO_FONT_WEIGHT_BOLD : CAIRO_FONT_WEIGHT_NORMAL);
				double ptSize = MM2PIXEL(pageblockobject.second->textObject->ct_text->size);
				cairo_set_font_size(cttext, ptSize);
				//TRACE("%d,%d,%d,%d\r\n", pageblockobject.second.textObject->ct_text->fillColor->valueR, pageblockobject.second.textObject->ct_text->fillColor->valueG, pageblockobject.second.textObject->ct_text->fillColor->valueB, pageblockobject.second.textObject->ct_text->fillColor->alpha);
				cairo_set_source_rgba(cttext, pageblockobject.second->textObject->ct_text->fillColor->valueR, pageblockobject.second->textObject->ct_text->fillColor->valueG, pageblockobject.second->textObject->ct_text->fillColor->valueB, pageblockobject.second->textObject->ct_text->fillColor->alpha);
				for (TextCode_And_CT_CGTransform* textcode_and_ct_cgtransform : pageblockobject.second->textObject->ct_text->textcode_and_ct_cgtransforms)
				{
					if (pageblockobject.second->textObject->ct_text->ct_graphicUnit->cTM != NULL)
					{
						cairo_matrix_t *ctm = new cairo_matrix_t();
						ct_matrix_to_cairo_matrix(pageblockobject.second->textObject->ct_text->ct_graphicUnit->cTM, ctm);
						cairo_set_matrix(cttext, ctm);
						delete ctm;
					}
					if (textcode_and_ct_cgtransform->textCode->deltaX.size() > 1 || textcode_and_ct_cgtransform->textCode->deltaY.size() > 1)
					{
						std::vector<std::string> vec;
						fnReadCharactersUTF8(textcode_and_ct_cgtransform->textCode->str.c_str(), vec);
						double x0 = MM2PIXEL(textcode_and_ct_cgtransform->textCode->x);
						double y0 = MM2PIXEL(textcode_and_ct_cgtransform->textCode->y);
						double toMoveX = x0;
						double toMoveY = y0;
						for (size_t i = 0; i < vec.size(); i++)
						{
							std::string strCh = vec[i];
							double deltaX = i < textcode_and_ct_cgtransform->textCode->deltaX.size() ? (MM2PIXEL(textcode_and_ct_cgtransform->textCode->deltaX[i])) : 0;
							double deltaY = i < textcode_and_ct_cgtransform->textCode->deltaY.size() ? (MM2PIXEL(textcode_and_ct_cgtransform->textCode->deltaY[i])) : 0;
							toMoveX += deltaX;
							toMoveY += deltaY;
							{
								cairo_move_to(
									cttext,
									toMoveX,
									toMoveY);
								cairo_show_text(cttext, strCh.c_str());
							}
						}
					}
					else
					{
						cairo_move_to(cttext, MM2PIXEL(textcode_and_ct_cgtransform->textCode->x), MM2PIXEL(textcode_and_ct_cgtransform->textCode->y));
						cairo_show_text(cttext, textcode_and_ct_cgtransform->textCode->str.c_str());
					}
				}

				//char tmp[MAX_PATH] = { 0 };
				//itoa(ii, tmp, 10);
				//std::string outpath = tmp;
				//outpath += ".png";
				//cairo_surface_write_to_png(cairo_get_target(cr), outpath.c_str());
				//ii++;

				cairo_set_source_surface(cr, crtextsurface, MM2PIXEL(pageblockobject.second->textObject->ct_text->ct_graphicUnit->boundary->x), MM2PIXEL(pageblockobject.second->textObject->ct_text->ct_graphicUnit->boundary->y));
				cairo_paint(cr);
				cairo_restore(cr);
			}
			else if (pageblockobject.first == CT_PageBlock::TYPE::IMAGEOBJECT)
			{
				cairo_move_to(cr, MM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->x), MM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->y));

				if (ofd->resAll->find(pageblockobject.second->imageObject->ct_imageObject->resourceID) != ofd->resAll->end())
				{
					CT_MultiMedia *multimedia = ofd->resAll->at(pageblockobject.second->imageObject->ct_imageObject->resourceID).second->multiMedia;
					if (multimedia->format.compare("PNG") == 0)
					{
						cairo_save(cr);
						cairo_surface_t *imagesurface = cairo_image_surface_create_from_png(multimedia->MediaFile->abs.c_str());
						cairo_t *crimg = cairo_create(imagesurface);
						if (pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->cTM != NULL)
						{
							cairo_matrix_t *ctm = new cairo_matrix_t();
							ct_matrix_to_cairo_matrix(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->cTM, ctm);
							cairo_set_matrix(crimg, ctm);
							delete ctm;
						}
						int image_w = cairo_image_surface_get_width(imagesurface);
						int image_h = cairo_image_surface_get_height(imagesurface);
						int boundary_w = INTMM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->w);
						int boundary_h = INTMM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->h);
						double factor_w = (double)boundary_w / (double)image_w;
						double factor_h = (double)boundary_h / (double)image_h;
						cairo_scale(cr, factor_w, factor_h);
						imagesurface = cairo_get_target(crimg);
						int image_w2 = cairo_image_surface_get_width(imagesurface);
						int image_h2 = cairo_image_surface_get_height(imagesurface);
						double image_x = MM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->x) / factor_w;
						double image_y = MM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->y) / factor_h;
						
						//char tmp[MAX_PATH] = { 0 };
						//itoa(ii, tmp, 10);
						//std::string outpath = tmp;
						//outpath += ".png";
						//cairo_surface_write_to_png(cairo_get_target(cr), outpath.c_str());
						//ii++;
						
						cairo_set_source_surface(cr, imagesurface, image_x, image_y);
						cairo_paint(cr);
						cairo_surface_destroy(imagesurface);
						cairo_restore(cr);
					}
					else if (multimedia->format.compare("JB2") == 0 || multimedia->format.compare("GBIG2") == 0)
					{
						cairo_save(cr);
						int image_w = 0;
						int image_h = 0;
						unsigned char*tmp = NULL;
						cairo_surface_t *imagesurface = cairo_image_surface_create_from_jbig2(multimedia->MediaFile->abs.c_str(), &image_w, &image_h,&tmp);
						cairo_t *crimg = cairo_create(imagesurface);
						if (pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->cTM != NULL)
						{
							cairo_matrix_t *ctm = new cairo_matrix_t();
							ct_matrix_to_cairo_matrix(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->cTM, ctm);
							cairo_set_matrix(crimg, ctm);
							delete ctm;
						}
						//int image_w = cairo_image_surface_get_width(imagesurface);
						//int image_h = cairo_image_surface_get_height(imagesurface);
						int boundary_w = INTMM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->w);
						int boundary_h = INTMM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->h);
						double factor_w = (double)boundary_w / (double)image_w;
						double factor_h = (double)boundary_h / (double)image_h;
						cairo_scale(cr, factor_w, factor_h);
						imagesurface = cairo_get_target(crimg);
						int image_w2 = cairo_image_surface_get_width(imagesurface);
						int image_h2 = cairo_image_surface_get_height(imagesurface);
						double image_x = MM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->x) / factor_w;
						double image_y = MM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->y) / factor_h;

						//char tmp[MAX_PATH] = { 0 };
						//itoa(ii, tmp, 10);
						//std::string outpath = tmp;
						//outpath += ".png";
						//cairo_surface_write_to_png(cairo_get_target(cr), outpath.c_str());
						//ii++;

						cairo_set_source_surface(cr, imagesurface, image_x, image_y);
						cairo_paint(cr);
						cairo_surface_destroy(imagesurface);
						free(tmp);
						cairo_restore(cr);
					}
					else
					{
						cairo_save(cr);
						cairo_surface_t *imagesurface = cairo_image_surface_create_from_jpeg(multimedia->MediaFile->abs.c_str(), &listUsedImage, &g_gdiplusToken);
						cairo_t *crimg = cairo_create(imagesurface);
						if (pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->cTM != NULL)
						{
							cairo_matrix_t *ctm = new cairo_matrix_t();
							ct_matrix_to_cairo_matrix(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->cTM, ctm);
							cairo_set_matrix(crimg, ctm);
							delete ctm;
						}
						int image_w = cairo_image_surface_get_width(imagesurface);
						int image_h = cairo_image_surface_get_height(imagesurface);
						int boundary_w = INTMM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->w);
						int boundary_h = INTMM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->h);
						double factor_w = (double)boundary_w / (double)image_w;
						double factor_h = (double)boundary_h / (double)image_h;
						cairo_scale(cr, factor_w, factor_h);
						imagesurface = cairo_get_target(crimg);
						int image_w2 = cairo_image_surface_get_width(imagesurface);
						int image_h2 = cairo_image_surface_get_height(imagesurface);
						double image_x = MM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->x) / factor_w;
						double image_y = MM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->y) / factor_h;

						//char tmp[MAX_PATH] = { 0 };
						//itoa(ii, tmp, 10);
						//std::string outpath = tmp;
						//outpath += ".png";
						//cairo_surface_write_to_png(cairo_get_target(cr), outpath.c_str());
						//ii++;

						cairo_set_source_surface(cr, imagesurface, image_x, image_y);
						cairo_paint(cr);
						cairo_image_surface_delete_from_jpeg(imagesurface, &listUsedImage, &g_gdiplusToken);
						cairo_surface_destroy(imagesurface);
						cairo_restore(cr);
					}
				}
			}
			else if (pageblockobject.first == CT_PageBlock::TYPE::PATHOBJECT)
			{
				cairo_move_to(cr, MM2PIXEL(pageblockobject.second->pathObject->ct_path->ct_graphicUnit->boundary->x), MM2PIXEL(pageblockobject.second->pathObject->ct_path->ct_graphicUnit->boundary->y));
				cairo_save(cr);
				cairo_surface_t *imagesurface = cairo_image_surface_create(cairo_format_t::CAIRO_FORMAT_ARGB32, INTMM2PIXEL(pageblockobject.second->pathObject->ct_path->ct_graphicUnit->boundary->w), INTMM2PIXEL(pageblockobject.second->pathObject->ct_path->ct_graphicUnit->boundary->h));
				cairo_t *crimg = cairo_create(imagesurface);
				if (pageblockobject.second->pathObject->ct_path->ct_graphicUnit->cTM != NULL)
				{
					cairo_matrix_t *ctm = new cairo_matrix_t();
					ct_matrix_to_cairo_matrix(pageblockobject.second->pathObject->ct_path->ct_graphicUnit->cTM, ctm);
					cairo_set_matrix(crimg, ctm);
					delete ctm;
				}
				double x0 = 0.0f;
				double y0 = 0.0f;
				for (AbbreviatedData * abb : pageblockobject.second->pathObject->ct_path->abbreviatedData)
				{
					if (abb->cMd.compare("S") == 0)
					{
						double x0 = abb->params[0];
						double y0 = abb->params[1];
					}
					else if (abb->cMd.compare("M") == 0)
					{
						double x = abb->params[0];
						double y = abb->params[1];
						cairo_move_to(crimg, MM2PIXEL(x), MM2PIXEL(y));
					}
					else if (abb->cMd.compare("L") == 0)
					{
						double x = abb->params[0];
						double y = abb->params[1];
						cairo_line_to(crimg, MM2PIXEL(x), MM2PIXEL(y));
					}
					else if (abb->cMd.compare("Q") == 0)
					{
						double x1 = abb->params[0];
						double y1 = abb->params[1];
						double x2 = abb->params[2];
						double y2 = abb->params[3];
						cairo_quadratic_to(crimg, MM2PIXEL(x1), MM2PIXEL(y1), MM2PIXEL(x2), MM2PIXEL(y2));
					}
					else if (abb->cMd.compare("B") == 0)
					{
						double x1 = abb->params[0];
						double y1 = abb->params[1];
						double x2 = abb->params[2];
						double y2 = abb->params[3];
						double x3 = abb->params[4];
						double y3 = abb->params[5];
						cairo_curve_to(crimg, MM2PIXEL(x1), MM2PIXEL(y1), MM2PIXEL(x2), MM2PIXEL(y2), MM2PIXEL(x3), MM2PIXEL(y3));
					}
					else if (abb->cMd.compare("A") == 0)
					{
						double rx = abb->params[0];
						double ry = abb->params[1];
						double angle1 = abb->params[2];
						double large = abb->params[3];
						double sweep = abb->params[4];
						double x = abb->params[5];
						double y = abb->params[6];
						//TODO 只会已知原点和长短轴及起始终止角度的椭圆圆弧
						double x2, y2;
						cairo_get_current_point(crimg, &x2, &y2);
						cairo_quadratic_to(crimg, x2, y2, MM2PIXEL(x), MM2PIXEL(y));
					}
					else if (abb->cMd.compare("C") == 0)
					{
						cairo_close_path(crimg);
					}
					else
					{
						continue;
					}
				}
				if (pageblockobject.second->pathObject->ct_path->stroke)
				{
					cairo_set_source_rgba(crimg, pageblockobject.second->pathObject->ct_path->strokeColor->valueR, pageblockobject.second->pathObject->ct_path->strokeColor->valueG, pageblockobject.second->pathObject->ct_path->strokeColor->valueB, pageblockobject.second->pathObject->ct_path->strokeColor->alpha);
					cairo_set_line_width(crimg, MM2PIXEL(pageblockobject.second->pathObject->ct_path->ct_graphicUnit->lineWidth));
					cairo_stroke(crimg);
				}
				if (pageblockobject.second->pathObject->ct_path->fill)
				{
					cairo_set_source_rgba(crimg, pageblockobject.second->pathObject->ct_path->fillColor->valueR, pageblockobject.second->pathObject->ct_path->fillColor->valueG, pageblockobject.second->pathObject->ct_path->fillColor->valueB, pageblockobject.second->pathObject->ct_path->fillColor->alpha);
					cairo_fill(crimg);
				}

				//char tmp[MAX_PATH] = { 0 };
				//itoa(ii, tmp, 10);
				//std::string outpath = tmp;
				//outpath += ".png";
				//cairo_surface_write_to_png(cairo_get_target(crimg), outpath.c_str());
				//ii++;

				cairo_set_source_surface(cr, imagesurface, MM2PIXEL(pageblockobject.second->pathObject->ct_path->ct_graphicUnit->boundary->x), MM2PIXEL(pageblockobject.second->pathObject->ct_path->ct_graphicUnit->boundary->y));
				cairo_paint(cr);
				cairo_surface_destroy(imagesurface);
				cairo_restore(cr);
			}
		}
	}

	if (doc->signatures)
	{
		renderSealToCairoDevice(docpage, doc->signatures, cr);
	}

	//char tmp[MAX_PATH] = { 0 };
	//itoa(ii, tmp, 10);
	//std::string outpath = tmp;
	//outpath += ".png";
	//cairo_surface_write_to_png(cairo_get_target(cr), outpath.c_str());
	//ii++;

	double factor_w = (double)width / (double)page_w;
	double factor_h = (double)height / (double)page_h;
	cairo_scale(crtarget, factor_w, factor_h);

	cairo_set_source_surface(crtarget, cairo_get_target(cr), 0, 0);
	cairo_paint(crtarget);



	st_png_data png_data = { nullptr };
	png_data.length = 0;
	png_data.pdata = pngData;
	cairo_surface_write_to_png_stream(surfaceTarget, cairo_write_func_mine, &png_data);
	*pngDataLen = png_data.length;


	cairo_destroy(cr);
	cairo_surface_destroy(surface);
	cairo_destroy(crtarget);
	cairo_surface_destroy(surfaceTarget);

}

OFDIUMEX_API void OFDIUM_RenderDocumentPageToSurface(void *fileHandle, int pageIndex, int width, int height, int start_x, int start_y, int rotate, void *surfaceTarget)
{
	bool widthheightchange = (rotate % 2 == 1);

	if (widthheightchange)
	{
		std::swap(width, height);
	}
	
	if (listOpenedOFD.find((OFD *)fileHandle) == listOpenedOFD.end())
	{
		return;
	}
	auto *ofd = (OFD *)fileHandle;

	if (ofd->documents.size() != 1)
	{
		return;
	}

	Document *doc = ofd->documents[0];

	if ((int)doc->pages.size() - 1 < pageIndex || doc->pages[pageIndex].second == nullptr)
	{
		return;
	}

	Page *page = doc->pages[pageIndex].second;
	Document_Page *docpage = doc->pages[pageIndex].first;

	cairo_surface_t *surface;
	cairo_t *cr, *crtarget;



	int page_w = page->area == nullptr ? INTMM2PIXEL(doc->commonData->pageArea->physicalBox->w) : INTMM2PIXEL(page->area->physicalBox->w);
	int page_h = page->area == nullptr ? INTMM2PIXEL(doc->commonData->pageArea->physicalBox->h) : INTMM2PIXEL(page->area->physicalBox->h);
	int page_x = page->area == nullptr ? INTMM2PIXEL(doc->commonData->pageArea->physicalBox->x) : INTMM2PIXEL(page->area->physicalBox->x);
	int page_y = page->area == nullptr ? INTMM2PIXEL(doc->commonData->pageArea->physicalBox->y) : INTMM2PIXEL(page->area->physicalBox->y);

	crtarget = cairo_create((cairo_surface_t *)surfaceTarget);

	surface = cairo_image_surface_create(cairo_format_t::CAIRO_FORMAT_ARGB32, page_w, page_h);
	cr = cairo_create(surface);

	//cairo_set_source_rgba(cr, 255, 255, 255, 255);
	//cairo_rectangle(cr, page_x, page_y, page_w, page_h);
	//cairo_fill(cr);

	renderDocumentTemplatePageToCairoDevice(ofd, doc, cr);

	//char tmp[MAX_PATH] = { 0 };
	//itoa(ii, tmp, 10);
	//std::string outpath = tmp;
	//outpath += ".png";
	//cairo_surface_write_to_png(cairo_get_target(cr), outpath.c_str());
	//ii++;

	for (std::pair<ST_ID, CT_Layer*> layerpair : page->content->layers)
	{
		for (std::pair<CT_PageBlock::TYPE, CT_PageBlock::Data*> pageblockobject : layerpair.second->ct_pageBlock->d)
		{
			if (pageblockobject.first == CT_PageBlock::TYPE::TEXTOBJECT)
			{
				cairo_save(cr);
				CT_Font *font = ofd->resAll->at(pageblockobject.second->textObject->ct_text->font).second->font;
				std::string family = font->familyName.empty() ? (font->fontName.empty() ? "SimSun" : font->fontName) : font->familyName;
				cairo_surface_t *crtextsurface = cairo_image_surface_create(cairo_format_t::CAIRO_FORMAT_ARGB32, INTMM2PIXEL(pageblockobject.second->textObject->ct_text->ct_graphicUnit->boundary->w), INTMM2PIXEL(pageblockobject.second->textObject->ct_text->ct_graphicUnit->boundary->h));
				cairo_t *cttext = cairo_create(crtextsurface);

				cairo_font_options_t *option = cairo_font_options_create();
				cairo_font_options_set_antialias(option, cairo_antialias_t::CAIRO_ANTIALIAS_BEST);
				cairo_set_font_options(cttext, option);
				cairo_select_font_face(cttext,
					family.c_str(),
					font->italic ? CAIRO_FONT_SLANT_ITALIC : CAIRO_FONT_SLANT_NORMAL,
					font->bold || pageblockobject.second->textObject->ct_text->weight >= 700 ? CAIRO_FONT_WEIGHT_BOLD : CAIRO_FONT_WEIGHT_NORMAL);
				double ptSize = MM2PIXEL(pageblockobject.second->textObject->ct_text->size);
				cairo_set_font_size(cttext, ptSize);
				//TRACE("%d,%d,%d,%d\r\n", pageblockobject.second.textObject->ct_text->fillColor->valueR, pageblockobject.second.textObject->ct_text->fillColor->valueG, pageblockobject.second.textObject->ct_text->fillColor->valueB, pageblockobject.second.textObject->ct_text->fillColor->alpha);
				cairo_set_source_rgba(cttext, pageblockobject.second->textObject->ct_text->fillColor->valueR, pageblockobject.second->textObject->ct_text->fillColor->valueG, pageblockobject.second->textObject->ct_text->fillColor->valueB, pageblockobject.second->textObject->ct_text->fillColor->alpha);
				for (TextCode_And_CT_CGTransform* textcode_and_ct_cgtransform : pageblockobject.second->textObject->ct_text->textcode_and_ct_cgtransforms)
				{
					if (pageblockobject.second->textObject->ct_text->ct_graphicUnit->cTM != NULL)
					{
						cairo_matrix_t *ctm = new cairo_matrix_t();
						ct_matrix_to_cairo_matrix(pageblockobject.second->textObject->ct_text->ct_graphicUnit->cTM, ctm);
						cairo_set_matrix(cttext, ctm);
						delete ctm;
					}
					if (textcode_and_ct_cgtransform->textCode->deltaX.size() > 1 || textcode_and_ct_cgtransform->textCode->deltaY.size() > 1)
					{
						std::vector<std::string> vec;
						fnReadCharactersUTF8(textcode_and_ct_cgtransform->textCode->str.c_str(), vec);
						double x0 = MM2PIXEL(textcode_and_ct_cgtransform->textCode->x);
						double y0 = MM2PIXEL(textcode_and_ct_cgtransform->textCode->y);
						double toMoveX = x0;
						double toMoveY = y0;
						for (size_t i = 0; i < vec.size(); i++)
						{
							std::string strCh = vec[i];
							double deltaX = i < textcode_and_ct_cgtransform->textCode->deltaX.size() ? (MM2PIXEL(textcode_and_ct_cgtransform->textCode->deltaX[i])) : 0;
							double deltaY = i < textcode_and_ct_cgtransform->textCode->deltaY.size() ? (MM2PIXEL(textcode_and_ct_cgtransform->textCode->deltaY[i])) : 0;
							toMoveX += deltaX;
							toMoveY += deltaY;
							{
								cairo_move_to(
									cttext,
									toMoveX,
									toMoveY);
								cairo_show_text(cttext, strCh.c_str());
							}
						}
					}
					else
					{
						cairo_move_to(cttext, MM2PIXEL(textcode_and_ct_cgtransform->textCode->x), MM2PIXEL(textcode_and_ct_cgtransform->textCode->y));
						cairo_show_text(cttext, textcode_and_ct_cgtransform->textCode->str.c_str());
					}
				}

				//char tmp[MAX_PATH] = { 0 };
				//itoa(ii, tmp, 10);
				//std::string outpath = tmp;
				//outpath += ".png";
				//cairo_surface_write_to_png(cairo_get_target(cr), outpath.c_str());
				//ii++;

				cairo_set_source_surface(cr, crtextsurface, MM2PIXEL(pageblockobject.second->textObject->ct_text->ct_graphicUnit->boundary->x), MM2PIXEL(pageblockobject.second->textObject->ct_text->ct_graphicUnit->boundary->y));
				cairo_paint(cr);
				cairo_restore(cr);

				cairo_font_options_destroy(option);
				cairo_destroy(cttext);
				cairo_surface_destroy(crtextsurface);
			}
			else if (pageblockobject.first == CT_PageBlock::TYPE::IMAGEOBJECT)
			{
				cairo_move_to(cr, MM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->x), MM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->y));

				if (ofd->resAll->find(pageblockobject.second->imageObject->ct_imageObject->resourceID) != ofd->resAll->end())
				{
					CT_MultiMedia *multimedia = ofd->resAll->at(pageblockobject.second->imageObject->ct_imageObject->resourceID).second->multiMedia;
					if (multimedia->format.compare("PNG") == 0)
					{
						cairo_save(cr);
						cairo_surface_t *imagesurface = cairo_image_surface_create_from_png(multimedia->MediaFile->abs.c_str());
						cairo_t *crimg = cairo_create(imagesurface);
						if (pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->cTM != NULL)
						{
							cairo_matrix_t *ctm = new cairo_matrix_t();
							ct_matrix_to_cairo_matrix(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->cTM, ctm);
							cairo_set_matrix(crimg, ctm);
							delete ctm;
						}
						int image_w = cairo_image_surface_get_width(imagesurface);
						int image_h = cairo_image_surface_get_height(imagesurface);
						int boundary_w = INTMM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->w);
						int boundary_h = INTMM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->h);
						double factor_w = (double)boundary_w / (double)image_w;
						double factor_h = (double)boundary_h / (double)image_h;
						cairo_scale(cr, factor_w, factor_h);
						imagesurface = cairo_get_target(crimg);
						int image_w2 = cairo_image_surface_get_width(imagesurface);
						int image_h2 = cairo_image_surface_get_height(imagesurface);
						double image_x = MM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->x) / factor_w;
						double image_y = MM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->y) / factor_h;

						//char tmp[MAX_PATH] = { 0 };
						//itoa(ii, tmp, 10);
						//std::string outpath = tmp;
						//outpath += ".png";
						//cairo_surface_write_to_png(cairo_get_target(cr), outpath.c_str());
						//ii++;

						cairo_set_source_surface(cr, imagesurface, image_x, image_y);
						cairo_paint(cr);
						cairo_destroy(crimg);
						cairo_surface_destroy(imagesurface);
						cairo_restore(cr);
					}
					else if (multimedia->format.compare("JB2") == 0 || multimedia->format.compare("GBIG2") == 0)
					{
						cairo_save(cr);
						int image_w = 0;
						int image_h = 0;
						unsigned char*tmp = NULL;
						cairo_surface_t *imagesurface = cairo_image_surface_create_from_jbig2(multimedia->MediaFile->abs.c_str(), &image_w, &image_h,&tmp);
						cairo_t *crimg = cairo_create(imagesurface);
						if (pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->cTM != NULL)
						{
							cairo_matrix_t *ctm = new cairo_matrix_t();
							ct_matrix_to_cairo_matrix(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->cTM, ctm);
							cairo_set_matrix(crimg, ctm);
							delete ctm;
						}
						//int image_w = cairo_image_surface_get_width(imagesurface);
						//int image_h = cairo_image_surface_get_height(imagesurface);
						int boundary_w = INTMM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->w);
						int boundary_h = INTMM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->h);
						double factor_w = (double)boundary_w / (double)image_w;
						double factor_h = (double)boundary_h / (double)image_h;
						cairo_scale(cr, factor_w, factor_h);
						imagesurface = cairo_get_target(crimg);
						int image_w2 = cairo_image_surface_get_width(imagesurface);
						int image_h2 = cairo_image_surface_get_height(imagesurface);
						double image_x = MM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->x) / factor_w;
						double image_y = MM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->y) / factor_h;

						//char tmp[MAX_PATH] = { 0 };
						//itoa(ii, tmp, 10);
						//std::string outpath = tmp;
						//outpath += ".png";
						//cairo_surface_write_to_png(cairo_get_target(cr), outpath.c_str());
						//ii++;

						cairo_set_source_surface(cr, imagesurface, image_x, image_y);
						cairo_paint(cr);
						cairo_destroy(crimg);
						cairo_surface_destroy(imagesurface);
						free(tmp);
						cairo_restore(cr);
					}
					else
					{
						cairo_save(cr);
						cairo_surface_t *imagesurface = cairo_image_surface_create_from_jpeg(multimedia->MediaFile->abs.c_str(), &listUsedImage, &g_gdiplusToken);
						cairo_t *crimg = cairo_create(imagesurface);
						if (pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->cTM != NULL)
						{
							cairo_matrix_t *ctm = new cairo_matrix_t();
							ct_matrix_to_cairo_matrix(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->cTM, ctm);
							cairo_set_matrix(crimg, ctm);
							delete ctm;
						}
						int image_w = cairo_image_surface_get_width(imagesurface);
						int image_h = cairo_image_surface_get_height(imagesurface);
						int boundary_w = INTMM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->w);
						int boundary_h = INTMM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->h);
						double factor_w = (double)boundary_w / (double)image_w;
						double factor_h = (double)boundary_h / (double)image_h;
						cairo_scale(cr, factor_w, factor_h);
						imagesurface = cairo_get_target(crimg);
						int image_w2 = cairo_image_surface_get_width(imagesurface);
						int image_h2 = cairo_image_surface_get_height(imagesurface);
						double image_x = MM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->x) / factor_w;
						double image_y = MM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->y) / factor_h;

						//char tmp[MAX_PATH] = { 0 };
						//itoa(ii, tmp, 10);
						//std::string outpath = tmp;
						//outpath += ".png";
						//cairo_surface_write_to_png(cairo_get_target(cr), outpath.c_str());
						//ii++;

						cairo_set_source_surface(cr, imagesurface, image_x, image_y);
						cairo_paint(cr);
						cairo_image_surface_delete_from_jpeg(imagesurface, &listUsedImage, &g_gdiplusToken);
						cairo_destroy(crimg);
						cairo_surface_destroy(imagesurface);
						cairo_restore(cr);
					}
				}
			}
			else if (pageblockobject.first == CT_PageBlock::TYPE::PATHOBJECT)
			{
				cairo_move_to(cr, MM2PIXEL(pageblockobject.second->pathObject->ct_path->ct_graphicUnit->boundary->x), MM2PIXEL(pageblockobject.second->pathObject->ct_path->ct_graphicUnit->boundary->y));
				cairo_save(cr);
				cairo_surface_t *imagesurface = cairo_image_surface_create(cairo_format_t::CAIRO_FORMAT_ARGB32, INTMM2PIXEL(pageblockobject.second->pathObject->ct_path->ct_graphicUnit->boundary->w), INTMM2PIXEL(pageblockobject.second->pathObject->ct_path->ct_graphicUnit->boundary->h));
				cairo_t *crimg = cairo_create(imagesurface);
				if (pageblockobject.second->pathObject->ct_path->ct_graphicUnit->cTM != NULL)
				{
					cairo_matrix_t *ctm = new cairo_matrix_t();
					ct_matrix_to_cairo_matrix(pageblockobject.second->pathObject->ct_path->ct_graphicUnit->cTM, ctm);
					cairo_set_matrix(crimg, ctm);
					delete ctm;
				}
				double x0 = 0.0f;
				double y0 = 0.0f;
				for (AbbreviatedData * abb : pageblockobject.second->pathObject->ct_path->abbreviatedData)
				{
					if (abb->cMd.compare("S") == 0)
					{
						double x0 = abb->params[0];
						double y0 = abb->params[1];
					}
					else if (abb->cMd.compare("M") == 0)
					{
						double x = abb->params[0];
						double y = abb->params[1];
						cairo_move_to(crimg, MM2PIXEL(x), MM2PIXEL(y));
					}
					else if (abb->cMd.compare("L") == 0)
					{
						double x = abb->params[0];
						double y = abb->params[1];
						cairo_line_to(crimg, MM2PIXEL(x), MM2PIXEL(y));
					}
					else if (abb->cMd.compare("Q") == 0)
					{
						double x1 = abb->params[0];
						double y1 = abb->params[1];
						double x2 = abb->params[2];
						double y2 = abb->params[3];
						cairo_quadratic_to(crimg, MM2PIXEL(x1), MM2PIXEL(y1), MM2PIXEL(x2), MM2PIXEL(y2));
					}
					else if (abb->cMd.compare("B") == 0)
					{
						double x1 = abb->params[0];
						double y1 = abb->params[1];
						double x2 = abb->params[2];
						double y2 = abb->params[3];
						double x3 = abb->params[4];
						double y3 = abb->params[5];
						cairo_curve_to(crimg, MM2PIXEL(x1), MM2PIXEL(y1), MM2PIXEL(x2), MM2PIXEL(y2), MM2PIXEL(x3), MM2PIXEL(y3));
					}
					else if (abb->cMd.compare("A") == 0)
					{
						double rx = abb->params[0];
						double ry = abb->params[1];
						double angle1 = abb->params[2];
						double large = abb->params[3];
						double sweep = abb->params[4];
						double x = abb->params[5];
						double y = abb->params[6];
						//TODO 只会已知原点和长短轴及起始终止角度的椭圆圆弧
						double x2, y2;
						cairo_get_current_point(crimg, &x2, &y2);
						cairo_quadratic_to(crimg, x2, y2, MM2PIXEL(x), MM2PIXEL(y));
					}
					else if (abb->cMd.compare("C") == 0)
					{
						cairo_close_path(crimg);
					}
					else
					{
						continue;
					}
				}
				if (pageblockobject.second->pathObject->ct_path->stroke)
				{
					cairo_set_source_rgba(crimg, pageblockobject.second->pathObject->ct_path->strokeColor->valueR, pageblockobject.second->pathObject->ct_path->strokeColor->valueG, pageblockobject.second->pathObject->ct_path->strokeColor->valueB, pageblockobject.second->pathObject->ct_path->strokeColor->alpha);
					cairo_set_line_width(crimg, MM2PIXEL(pageblockobject.second->pathObject->ct_path->ct_graphicUnit->lineWidth));
					cairo_stroke(crimg);
				}
				if (pageblockobject.second->pathObject->ct_path->fill)
				{
					cairo_set_source_rgba(crimg, pageblockobject.second->pathObject->ct_path->fillColor->valueR, pageblockobject.second->pathObject->ct_path->fillColor->valueG, pageblockobject.second->pathObject->ct_path->fillColor->valueB, pageblockobject.second->pathObject->ct_path->fillColor->alpha);
					cairo_fill(crimg);
				}

				//char tmp[MAX_PATH] = { 0 };
				//itoa(ii, tmp, 10);
				//std::string outpath = tmp;
				//outpath += ".png";
				//cairo_surface_write_to_png(cairo_get_target(crimg), outpath.c_str());
				//ii++;

				cairo_set_source_surface(cr, imagesurface, MM2PIXEL(pageblockobject.second->pathObject->ct_path->ct_graphicUnit->boundary->x), MM2PIXEL(pageblockobject.second->pathObject->ct_path->ct_graphicUnit->boundary->y));
				cairo_paint(cr);
				cairo_destroy(crimg);
				cairo_surface_destroy(imagesurface);
				cairo_restore(cr);
			}
		}
	}

	if (doc->signatures)
	{
		renderSealToCairoDevice(docpage, doc->signatures, cr);
	}

	//char tmp[MAX_PATH] = { 0 };
	//itoa(ii, tmp, 10);
	//std::string outpath = tmp;
	//outpath += ".png";
	//cairo_surface_write_to_png(cairo_get_target(cr), outpath.c_str());
	//ii++;

	double factor_w = (double)width / (double)page_w;
	double factor_h = (double)height / (double)page_h;
	if (widthheightchange)
	{
		cairo_translate(crtarget, 0.5f*height, 0.5f*width);
		cairo_rotate(crtarget, (double)rotate*M_PI / 2.0f);
		cairo_translate(crtarget, -0.5f*width, -0.5f*height);
		cairo_scale(crtarget, factor_w, factor_h);
	}
	else
	{
		cairo_translate(crtarget, 0.5f*width, 0.5f*height);
		cairo_rotate(crtarget, (double)rotate*M_PI / 2.0f);
		cairo_translate(crtarget, -0.5f*width, -0.5f*height);
		cairo_scale(crtarget, factor_w, factor_h);
	}
	


	cairo_set_source_surface(crtarget, cairo_get_target(cr), start_x, start_y);
	cairo_paint(crtarget);

	cairo_destroy(cr);
	cairo_surface_destroy(surface);
	cairo_destroy(crtarget);

}

OFDIUMEX_API void OFDIUM_RenderDocumentPageToDC(void *fileHandle, int pageIndex, int width, int height, int start_x, int start_y, int rotate, void *hdc)
{
	bool widthheightchange = (rotate % 2 == 1);

	if (widthheightchange)
	{
		std::swap(width, height);
	}

	if (listOpenedOFD.find((OFD *)fileHandle) == listOpenedOFD.end())
	{
		return;
	}
	auto *ofd = (OFD *)fileHandle;

	if (ofd->documents.size() != 1)
	{
		return;
	}

	Document *doc = ofd->documents[0];

	if ((int)doc->pages.size() - 1 < pageIndex || doc->pages[pageIndex].second == nullptr)
	{
		return;
	}

	Page *page = doc->pages[pageIndex].second;
	Document_Page *docpage = doc->pages[pageIndex].first;

	cairo_surface_t *surface, *surfaceTarget;
	cairo_t *cr, *crtarget;



	int page_w = page->area == nullptr ? INTMM2PIXEL(doc->commonData->pageArea->physicalBox->w) : INTMM2PIXEL(page->area->physicalBox->w);
	int page_h = page->area == nullptr ? INTMM2PIXEL(doc->commonData->pageArea->physicalBox->h) : INTMM2PIXEL(page->area->physicalBox->h);
	int page_x = page->area == nullptr ? INTMM2PIXEL(doc->commonData->pageArea->physicalBox->x) : INTMM2PIXEL(page->area->physicalBox->x);
	int page_y = page->area == nullptr ? INTMM2PIXEL(doc->commonData->pageArea->physicalBox->y) : INTMM2PIXEL(page->area->physicalBox->y);

	surfaceTarget = cairo_win32_surface_create((HDC)hdc);
	crtarget = cairo_create(surfaceTarget);

	surface = cairo_image_surface_create(cairo_format_t::CAIRO_FORMAT_ARGB32, page_w, page_h);
	cr = cairo_create(surface);

	//cairo_set_source_rgba(cr, 255, 255, 255, 255);
	//cairo_rectangle(cr, page_x, page_y, page_w, page_h);
	//cairo_fill(cr);

	renderDocumentTemplatePageToCairoDevice(ofd, doc, cr);

	//char tmp[MAX_PATH] = { 0 };
	//itoa(ii, tmp, 10);
	//std::string outpath = tmp;
	//outpath += ".png";
	//cairo_surface_write_to_png(cairo_get_target(cr), outpath.c_str());
	//ii++;

	for (std::pair<ST_ID, CT_Layer*> layerpair : page->content->layers)
	{
		for (std::pair<CT_PageBlock::TYPE, CT_PageBlock::Data*> pageblockobject : layerpair.second->ct_pageBlock->d)
		{
			if (pageblockobject.first == CT_PageBlock::TYPE::TEXTOBJECT)
			{
				cairo_save(cr);
				CT_Font *font = ofd->resAll->at(pageblockobject.second->textObject->ct_text->font).second->font;
				std::string family = font->familyName.empty() ? (font->fontName.empty() ? "SimSun" : font->fontName) : font->familyName;
				cairo_surface_t *crtextsurface = cairo_image_surface_create(cairo_format_t::CAIRO_FORMAT_ARGB32, INTMM2PIXEL(pageblockobject.second->textObject->ct_text->ct_graphicUnit->boundary->w), INTMM2PIXEL(pageblockobject.second->textObject->ct_text->ct_graphicUnit->boundary->h));
				cairo_t *cttext = cairo_create(crtextsurface);

				cairo_font_options_t *option = cairo_font_options_create();
				cairo_font_options_set_antialias(option, cairo_antialias_t::CAIRO_ANTIALIAS_BEST);
				cairo_set_font_options(cttext, option);
				cairo_select_font_face(cttext,
					family.c_str(),
					font->italic ? CAIRO_FONT_SLANT_ITALIC : CAIRO_FONT_SLANT_NORMAL,
					font->bold || pageblockobject.second->textObject->ct_text->weight >= 700 ? CAIRO_FONT_WEIGHT_BOLD : CAIRO_FONT_WEIGHT_NORMAL);
				double ptSize = MM2PIXEL(pageblockobject.second->textObject->ct_text->size);
				cairo_set_font_size(cttext, ptSize);
				//TRACE("%d,%d,%d,%d\r\n", pageblockobject.second.textObject->ct_text->fillColor->valueR, pageblockobject.second.textObject->ct_text->fillColor->valueG, pageblockobject.second.textObject->ct_text->fillColor->valueB, pageblockobject.second.textObject->ct_text->fillColor->alpha);
				cairo_set_source_rgba(cttext, pageblockobject.second->textObject->ct_text->fillColor->valueR, pageblockobject.second->textObject->ct_text->fillColor->valueG, pageblockobject.second->textObject->ct_text->fillColor->valueB, pageblockobject.second->textObject->ct_text->fillColor->alpha);
				for (TextCode_And_CT_CGTransform* textcode_and_ct_cgtransform : pageblockobject.second->textObject->ct_text->textcode_and_ct_cgtransforms)
				{
					if (pageblockobject.second->textObject->ct_text->ct_graphicUnit->cTM != NULL)
					{
						cairo_matrix_t *ctm = new cairo_matrix_t();
						ct_matrix_to_cairo_matrix(pageblockobject.second->textObject->ct_text->ct_graphicUnit->cTM, ctm);
						cairo_set_matrix(cttext, ctm);
						delete ctm;
					}
					if (textcode_and_ct_cgtransform->textCode->deltaX.size() > 1 || textcode_and_ct_cgtransform->textCode->deltaY.size() > 1)
					{
						std::vector<std::string> vec;
						fnReadCharactersUTF8(textcode_and_ct_cgtransform->textCode->str.c_str(), vec);
						double x0 = MM2PIXEL(textcode_and_ct_cgtransform->textCode->x);
						double y0 = MM2PIXEL(textcode_and_ct_cgtransform->textCode->y);
						double toMoveX = x0;
						double toMoveY = y0;
						for (size_t i = 0; i < vec.size(); i++)
						{
							std::string strCh = vec[i];
							double deltaX = i < textcode_and_ct_cgtransform->textCode->deltaX.size() ? (MM2PIXEL(textcode_and_ct_cgtransform->textCode->deltaX[i])) : 0;
							double deltaY = i < textcode_and_ct_cgtransform->textCode->deltaY.size() ? (MM2PIXEL(textcode_and_ct_cgtransform->textCode->deltaY[i])) : 0;
							toMoveX += deltaX;
							toMoveY += deltaY;
							{
								cairo_move_to(
									cttext,
									toMoveX,
									toMoveY);
								cairo_show_text(cttext, strCh.c_str());
							}
						}
					}
					else
					{
						cairo_move_to(cttext, MM2PIXEL(textcode_and_ct_cgtransform->textCode->x), MM2PIXEL(textcode_and_ct_cgtransform->textCode->y));
						cairo_show_text(cttext, textcode_and_ct_cgtransform->textCode->str.c_str());
					}
				}

				//char tmp[MAX_PATH] = { 0 };
				//itoa(ii, tmp, 10);
				//std::string outpath = tmp;
				//outpath += ".png";
				//cairo_surface_write_to_png(cairo_get_target(cr), outpath.c_str());
				//ii++;

				cairo_set_source_surface(cr, crtextsurface, MM2PIXEL(pageblockobject.second->textObject->ct_text->ct_graphicUnit->boundary->x), MM2PIXEL(pageblockobject.second->textObject->ct_text->ct_graphicUnit->boundary->y));
				cairo_paint(cr);
				cairo_restore(cr);
			}
			else if (pageblockobject.first == CT_PageBlock::TYPE::IMAGEOBJECT)
			{
				cairo_move_to(cr, MM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->x), MM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->y));

				if (ofd->resAll->find(pageblockobject.second->imageObject->ct_imageObject->resourceID) != ofd->resAll->end())
				{
					CT_MultiMedia *multimedia = ofd->resAll->at(pageblockobject.second->imageObject->ct_imageObject->resourceID).second->multiMedia;
					if (multimedia->format.compare("PNG") == 0)
					{
						cairo_save(cr);
						cairo_surface_t *imagesurface = cairo_image_surface_create_from_png(multimedia->MediaFile->abs.c_str());
						cairo_t *crimg = cairo_create(imagesurface);
						if (pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->cTM != NULL)
						{
							cairo_matrix_t *ctm = new cairo_matrix_t();
							ct_matrix_to_cairo_matrix(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->cTM, ctm);
							cairo_set_matrix(crimg, ctm);
							delete ctm;
						}
						int image_w = cairo_image_surface_get_width(imagesurface);
						int image_h = cairo_image_surface_get_height(imagesurface);
						int boundary_w = INTMM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->w);
						int boundary_h = INTMM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->h);
						double factor_w = (double)boundary_w / (double)image_w;
						double factor_h = (double)boundary_h / (double)image_h;
						cairo_scale(cr, factor_w, factor_h);
						imagesurface = cairo_get_target(crimg);
						int image_w2 = cairo_image_surface_get_width(imagesurface);
						int image_h2 = cairo_image_surface_get_height(imagesurface);
						double image_x = MM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->x) / factor_w;
						double image_y = MM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->y) / factor_h;

						//char tmp[MAX_PATH] = { 0 };
						//itoa(ii, tmp, 10);
						//std::string outpath = tmp;
						//outpath += ".png";
						//cairo_surface_write_to_png(cairo_get_target(cr), outpath.c_str());
						//ii++;

						cairo_set_source_surface(cr, imagesurface, image_x, image_y);
						cairo_paint(cr);
						cairo_surface_destroy(imagesurface);
						cairo_restore(cr);
					}
					else if (multimedia->format.compare("JB2") == 0 || multimedia->format.compare("GBIG2") == 0)
					{
						cairo_save(cr);
						int image_w = 0;
						int image_h = 0;
						unsigned char*tmp = NULL;
						cairo_surface_t *imagesurface = cairo_image_surface_create_from_jbig2(multimedia->MediaFile->abs.c_str(), &image_w, &image_h,&tmp);
						cairo_t *crimg = cairo_create(imagesurface);
						if (pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->cTM != NULL)
						{
							cairo_matrix_t *ctm = new cairo_matrix_t();
							ct_matrix_to_cairo_matrix(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->cTM, ctm);
							cairo_set_matrix(crimg, ctm);
							delete ctm;
						}
						//int image_w = cairo_image_surface_get_width(imagesurface);
						//int image_h = cairo_image_surface_get_height(imagesurface);
						int boundary_w = INTMM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->w);
						int boundary_h = INTMM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->h);
						double factor_w = (double)boundary_w / (double)image_w;
						double factor_h = (double)boundary_h / (double)image_h;
						cairo_scale(cr, factor_w, factor_h);
						imagesurface = cairo_get_target(crimg);
						int image_w2 = cairo_image_surface_get_width(imagesurface);
						int image_h2 = cairo_image_surface_get_height(imagesurface);
						double image_x = MM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->x) / factor_w;
						double image_y = MM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->y) / factor_h;

						//char tmp[MAX_PATH] = { 0 };
						//itoa(ii, tmp, 10);
						//std::string outpath = tmp;
						//outpath += ".png";
						//cairo_surface_write_to_png(cairo_get_target(cr), outpath.c_str());
						//ii++;

						cairo_set_source_surface(cr, imagesurface, image_x, image_y);
						cairo_paint(cr);
						cairo_surface_destroy(imagesurface);
						free(tmp);
						cairo_restore(cr);
					}
					else
					{
						cairo_save(cr);
						cairo_surface_t *imagesurface = cairo_image_surface_create_from_jpeg(multimedia->MediaFile->abs.c_str(), &listUsedImage, &g_gdiplusToken);
						cairo_t *crimg = cairo_create(imagesurface);
						if (pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->cTM != NULL)
						{
							cairo_matrix_t *ctm = new cairo_matrix_t();
							ct_matrix_to_cairo_matrix(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->cTM, ctm);
							cairo_set_matrix(crimg, ctm);
							delete ctm;
						}
						int image_w = cairo_image_surface_get_width(imagesurface);
						int image_h = cairo_image_surface_get_height(imagesurface);
						int boundary_w = INTMM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->w);
						int boundary_h = INTMM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->h);
						double factor_w = (double)boundary_w / (double)image_w;
						double factor_h = (double)boundary_h / (double)image_h;
						cairo_scale(cr, factor_w, factor_h);
						imagesurface = cairo_get_target(crimg);
						int image_w2 = cairo_image_surface_get_width(imagesurface);
						int image_h2 = cairo_image_surface_get_height(imagesurface);
						double image_x = MM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->x) / factor_w;
						double image_y = MM2PIXEL(pageblockobject.second->imageObject->ct_imageObject->ct_graphicUnit->boundary->y) / factor_h;

						//char tmp[MAX_PATH] = { 0 };
						//itoa(ii, tmp, 10);
						//std::string outpath = tmp;
						//outpath += ".png";
						//cairo_surface_write_to_png(cairo_get_target(cr), outpath.c_str());
						//ii++;

						cairo_set_source_surface(cr, imagesurface, image_x, image_y);
						cairo_paint(cr);
						cairo_image_surface_delete_from_jpeg(imagesurface, &listUsedImage, &g_gdiplusToken);
						cairo_surface_destroy(imagesurface);
						cairo_restore(cr);
					}
				}
			}
			else if (pageblockobject.first == CT_PageBlock::TYPE::PATHOBJECT)
			{
				cairo_move_to(cr, MM2PIXEL(pageblockobject.second->pathObject->ct_path->ct_graphicUnit->boundary->x), MM2PIXEL(pageblockobject.second->pathObject->ct_path->ct_graphicUnit->boundary->y));
				cairo_save(cr);
				cairo_surface_t *imagesurface = cairo_image_surface_create(cairo_format_t::CAIRO_FORMAT_ARGB32, INTMM2PIXEL(pageblockobject.second->pathObject->ct_path->ct_graphicUnit->boundary->w), INTMM2PIXEL(pageblockobject.second->pathObject->ct_path->ct_graphicUnit->boundary->h));
				cairo_t *crimg = cairo_create(imagesurface);
				if (pageblockobject.second->pathObject->ct_path->ct_graphicUnit->cTM != NULL)
				{
					cairo_matrix_t *ctm = new cairo_matrix_t();
					ct_matrix_to_cairo_matrix(pageblockobject.second->pathObject->ct_path->ct_graphicUnit->cTM, ctm);
					cairo_set_matrix(crimg, ctm);
					delete ctm;
				}
				double x0 = 0.0f;
				double y0 = 0.0f;
				for (AbbreviatedData * abb : pageblockobject.second->pathObject->ct_path->abbreviatedData)
				{
					if (abb->cMd.compare("S") == 0)
					{
						double x0 = abb->params[0];
						double y0 = abb->params[1];
					}
					else if (abb->cMd.compare("M") == 0)
					{
						double x = abb->params[0];
						double y = abb->params[1];
						cairo_move_to(crimg, MM2PIXEL(x), MM2PIXEL(y));
					}
					else if (abb->cMd.compare("L") == 0)
					{
						double x = abb->params[0];
						double y = abb->params[1];
						cairo_line_to(crimg, MM2PIXEL(x), MM2PIXEL(y));
					}
					else if (abb->cMd.compare("Q") == 0)
					{
						double x1 = abb->params[0];
						double y1 = abb->params[1];
						double x2 = abb->params[2];
						double y2 = abb->params[3];
						cairo_quadratic_to(crimg, MM2PIXEL(x1), MM2PIXEL(y1), MM2PIXEL(x2), MM2PIXEL(y2));
					}
					else if (abb->cMd.compare("B") == 0)
					{
						double x1 = abb->params[0];
						double y1 = abb->params[1];
						double x2 = abb->params[2];
						double y2 = abb->params[3];
						double x3 = abb->params[4];
						double y3 = abb->params[5];
						cairo_curve_to(crimg, MM2PIXEL(x1), MM2PIXEL(y1), MM2PIXEL(x2), MM2PIXEL(y2), MM2PIXEL(x3), MM2PIXEL(y3));
					}
					else if (abb->cMd.compare("A") == 0)
					{
						double rx = abb->params[0];
						double ry = abb->params[1];
						double angle1 = abb->params[2];
						double large = abb->params[3];
						double sweep = abb->params[4];
						double x = abb->params[5];
						double y = abb->params[6];
						//TODO 只会已知原点和长短轴及起始终止角度的椭圆圆弧
						double x2, y2;
						cairo_get_current_point(crimg, &x2, &y2);
						cairo_quadratic_to(crimg, x2, y2, MM2PIXEL(x), MM2PIXEL(y));
					}
					else if (abb->cMd.compare("C") == 0)
					{
						cairo_close_path(crimg);
					}
					else
					{
						continue;
					}
				}
				if (pageblockobject.second->pathObject->ct_path->stroke)
				{
					cairo_set_source_rgba(crimg, pageblockobject.second->pathObject->ct_path->strokeColor->valueR, pageblockobject.second->pathObject->ct_path->strokeColor->valueG, pageblockobject.second->pathObject->ct_path->strokeColor->valueB, pageblockobject.second->pathObject->ct_path->strokeColor->alpha);
					cairo_set_line_width(crimg, MM2PIXEL(pageblockobject.second->pathObject->ct_path->ct_graphicUnit->lineWidth));
					cairo_stroke(crimg);
				}
				if (pageblockobject.second->pathObject->ct_path->fill)
				{
					cairo_set_source_rgba(crimg, pageblockobject.second->pathObject->ct_path->fillColor->valueR, pageblockobject.second->pathObject->ct_path->fillColor->valueG, pageblockobject.second->pathObject->ct_path->fillColor->valueB, pageblockobject.second->pathObject->ct_path->fillColor->alpha);
					cairo_fill(crimg);
				}

				//char tmp[MAX_PATH] = { 0 };
				//itoa(ii, tmp, 10);
				//std::string outpath = tmp;
				//outpath += ".png";
				//cairo_surface_write_to_png(cairo_get_target(crimg), outpath.c_str());
				//ii++;

				cairo_set_source_surface(cr, imagesurface, MM2PIXEL(pageblockobject.second->pathObject->ct_path->ct_graphicUnit->boundary->x), MM2PIXEL(pageblockobject.second->pathObject->ct_path->ct_graphicUnit->boundary->y));
				cairo_paint(cr);
				cairo_surface_destroy(imagesurface);
				cairo_restore(cr);
			}
		}
	}

	if (doc->signatures)
	{
		renderSealToCairoDevice(docpage, doc->signatures, cr);
	}

	//char tmp[MAX_PATH] = { 0 };
	//itoa(ii, tmp, 10);
	//std::string outpath = tmp;
	//outpath += ".png";
	//cairo_surface_write_to_png(cairo_get_target(cr), outpath.c_str());
	//ii++;

	double factor_w = (double)width / (double)page_w;
	double factor_h = (double)height / (double)page_h;
	if (widthheightchange)
	{
		cairo_translate(crtarget, 0.5f*height, 0.5f*width);
		cairo_rotate(crtarget, (double)rotate*M_PI / 2.0f);
		cairo_translate(crtarget, -0.5f*width, -0.5f*height);
		cairo_scale(crtarget, factor_w, factor_h);
	}
	else
	{
		cairo_translate(crtarget, 0.5f*width, 0.5f*height);
		cairo_rotate(crtarget, (double)rotate*M_PI / 2.0f);
		cairo_translate(crtarget, -0.5f*width, -0.5f*height);
		cairo_scale(crtarget, factor_w, factor_h);
	}

	cairo_set_source_surface(crtarget, cairo_get_target(cr), start_x, start_y);
	cairo_paint(crtarget);

	cairo_destroy(cr);
	cairo_surface_destroy(surface);
	cairo_destroy(crtarget);
	cairo_surface_destroy(surfaceTarget);

}

OFDIUMEX_API void OFDIUM_Test()
{
	cairo_surface_t *crtextsurface = cairo_image_surface_create(cairo_format_t::CAIRO_FORMAT_ARGB32, 128, 128);
	cairo_t *cttext = cairo_create(crtextsurface);

	FT_Library ft_library;
	FT_Init_FreeType(&ft_library);

	FT_Face ft_face;
	FT_New_Face(ft_library, "C:\\Users\\Roy\\AppData\\Local\\Temp\\ofd-test\\Doc_0\\Res\\cc282774b5eb4b86957ed2e42c3b6ffa.ttf", 0, &ft_face);
	wchar_t wch = L'A';
	auto char_index = FT_Get_Char_Index(ft_face, (FT_Long)wch);

	cairo_font_face_t *cairo_ft_face;
	cairo_ft_face = cairo_ft_font_face_create_for_ft_face(ft_face, 0);

	unsigned int glyph_count = 1;
	cairo_glyph_t *cairo_glyphs = cairo_glyph_allocate(glyph_count);
	cairo_glyphs[0].index = char_index;
	cairo_glyphs[0].x = 0;
	cairo_glyphs[0].y = 128;
	cairo_font_options_t *option = cairo_font_options_create();
	cairo_font_options_set_antialias(option, cairo_antialias_t::CAIRO_ANTIALIAS_BEST);
	cairo_set_font_options(cttext, option);
	

	cairo_set_source_rgba(cttext, 0, 0, 0, 255);

	cairo_set_font_face(cttext, cairo_ft_face);
	//cairo_set_font_size(cttext, 292);
	{
		cairo_matrix_t matrix;
		cairo_matrix_init_scale(&matrix, 128, 128);
		cairo_set_font_matrix(cttext, &matrix);
	}

	cairo_move_to(
		cttext,
		0,
		0);
	
	cairo_show_glyphs(cttext, cairo_glyphs, glyph_count);
	//cairo_glyph_path(cttext, cairo_glyphs, glyph_count);
	FT_Done_FreeType(ft_library);

	char tmp[MAX_PATH] = { 0 };
	_itoa_s(123, tmp, 10);
	std::string dest = "image";
	dest += tmp;
	dest += ".png";
	cairo_surface_write_to_png(crtextsurface, dest.c_str());

}

OFDIUMEX_API void OFDIUM_CloseDocument(void *fileHandle)
{
	if (listOpenedOFD.find((OFD *)fileHandle) == listOpenedOFD.end())
	{
		return;
	}
	OFDParser *ofdparser = listOpenedOFD[(OFD *)fileHandle];
	ZipTool::deleteFolder(ofdparser->ofd_dir);
	delete ofdparser;
	listOpenedOFD.erase((OFD *)fileHandle);
}

OFDIUMEX_API void OFDIUM_AddRef()
{
	refCounter.AddRef();
}

OFDIUMEX_API void OFDIUM_Release()
{
	refCounter.Release();
}

void *cairo_image_surface_create_from_ofd_stream(const unsigned char *data, const unsigned int len, int width, int height)
{
	auto *ofd = (OFD *)OFDIUM_LoadMemDocument(data, len, 0);
	if (ofd == nullptr)
	{
		return nullptr;
	}
	unsigned char *ucPNG = nullptr;
	int ucPNGlen = 0;
	OFDIUM_GetDocumentPagePNG(ofd, 0, width, height, ucPNG, &ucPNGlen);
	ucPNG = new unsigned char[ucPNGlen];
	OFDIUM_GetDocumentPagePNG(ofd, 0, width, height, ucPNG, &ucPNGlen);
	OFDIUM_CloseDocument(ofd);
	st_png_data pngData = { (unsigned char*)ucPNG, 0 };
	cairo_surface_t *retSurface = cairo_image_surface_create_from_png_stream(cairo_read_func_mine, &pngData);
	delete[] ucPNG;
	return retSurface;
}

void makeSignatures(void * fileHandle, void *parserHandle, int docIndex, bool isFinalSign)
{
	auto *ofd = (OFD *)fileHandle;
	auto *ofdparser = (OFDParser *)parserHandle;
	auto * xmldoc = new xml_document();
	if (!ofd->docBodies[docIndex]->Signatures)
	{
		pugi::xml_node pre = xmldoc->prepend_child(pugi::node_declaration);
		pre.append_attribute("version") = "1.0";
		pre.append_attribute("encoding") = "UTF-8";

		xml_node t = xmldoc->append_child("ofd:Signatures");
		t.append_attribute("xmlns:ofd").set_value("http://www.ofdspec.org/2016");
		xml_node c = t.append_child("ofd:MaxSignId");
		c.append_child(node_pcdata).set_value(ofd->documents[docIndex]->signatures->maxSignId.c_str());
		for (auto sss : ofd->documents[docIndex]->signatures->signatures)
		{
			xml_node c = t.append_child("ofd:Signature");
			c.append_attribute("ID").set_value(sss.first->id.c_str());
			c.append_attribute("BaseLoc").set_value(sss.first->baseLoc->pln.c_str());
		}
		ofd->docBodies[docIndex]->Signatures = new ST_Loc();
		ofd->docBodies[docIndex]->Signatures->pln = "/Doc_0/Signs/Signatures.xml";
		ofd->docBodies[docIndex]->Signatures->abs = "/Doc_0/Signs/Signatures.xml";
		ofdparser->relativePath(ofdparser->ofd_dir, ofd->docBodies[docIndex]->Signatures->abs);
		char str[MAX_PATH] = { 0 };
		memcpy_s(str, ofd->docBodies[docIndex]->Signatures->abs.length(), ofd->docBodies[docIndex]->Signatures->abs.c_str(), ofd->docBodies[docIndex]->Signatures->abs.length());
		if (!PathIsDirectory(str))
		{
			PathRemoveFileSpecA(str);
		}
		CreateDir(str);
		auto rootdoc = new xml_document();
		rootdoc->load_file(ofd->ofdRoot.abs.c_str(), pugi::parse_default, pugi::encoding_utf8);
		auto b = rootdoc->child("ofd:OFD").child("ofd:DocBody");
		while (!b.isNull())
		{
			if (strcmp(b.child("ofd:DocInfo").child("ofd:DocID").text().as_string(), ofd->docBodies[docIndex]->docInfo->DocID.c_str()) == 0)
			{
				b.append_child("ofd:Signatures").append_child(node_pcdata).set_value(ofd->docBodies[docIndex]->Signatures->pln.c_str());
				rootdoc->save_file(ofd->ofdRoot.abs.c_str());
				break;
			}
			b = b.next_sibling();
		}
		delete rootdoc;
	}
	else
	{
		xmldoc->load_file(ofd->docBodies[docIndex]->Signatures->abs.c_str(), pugi::parse_default, pugi::encoding_utf8);
		xml_node t = xmldoc->child("ofd:Signatures");
		xml_node c = t.child("ofd:MaxSignId");
		if (c.isNull())
		{
			c = t.append_child("ofd:MaxSignId");
			c.append_child(node_pcdata).set_value(ofd->documents[docIndex]->signatures->maxSignId.c_str());
		}
		else
		{
			c.text().set(ofd->documents[docIndex]->signatures->maxSignId.c_str());
		}
		for (auto &sss : ofd->documents[docIndex]->signatures->signatures)
		{
			if (!sss.first->notInXML)
			{
				continue;
			}
			xml_node c = t.append_child("ofd:Signature");
			c.append_attribute("ID").set_value(sss.first->id.c_str());
			c.append_attribute("BaseLoc").set_value(sss.first->baseLoc->pln.c_str());
		}
	}
	xmldoc->save_file(ofd->docBodies[docIndex]->Signatures->abs.c_str());
	delete xmldoc;
}

inline std::string getCheckMethodFromType(int type)
{
	switch (type)
	{
	case 0:
		return "MD5";
	case 1:
		return "SHA1";
	case 2:
		return "1.2.156.10197.1.401";
	default:
		return "";
	}
}

void makeSignature(void * fileHandle, void * parserHandle, void * sigHandle, void *ssHandle)
{
	auto *ofd = (OFD *)fileHandle;
	auto *ofdparser = (OFDParser *)parserHandle;
	auto *sig = (Signature *)sigHandle;
	auto *ss = (Signatures_Signature *)ssHandle;
	auto * xmldoc = new xml_document();
	pugi::xml_node pre = xmldoc->prepend_child(pugi::node_declaration);
	pre.append_attribute("version") = "1.0";
	pre.append_attribute("encoding") = "UTF-8";

	xml_node t = xmldoc->append_child("ofd:Signature");
	t.append_attribute("xmlns:ofd").set_value("http://www.ofdspec.org/2016");
	xml_node si = t.append_child("ofd:SignedInfo");
	xml_node prov = si.append_child("ofd:Provider");
	prov.append_attribute("ProviderName").set_value(sig->signedInfo->provider->providerName.c_str());
	prov.append_attribute("Version").set_value(sig->signedInfo->provider->version.c_str());
	prov.append_attribute("Company").set_value(sig->signedInfo->provider->company.c_str());
	si.append_child("ofd:SignatureMethod").append_child(node_pcdata).set_value(sig->signedInfo->signatureMethod.c_str());
	si.append_child("ofd:SignatureDateTime").append_child(node_pcdata).set_value(sig->signedInfo->signatureDateTime.c_str());
	xml_node refs = si.append_child("ofd:References");
	refs.append_attribute("CheckMethod").set_value(getCheckMethodFromType(sig->signedInfo->references->checkMethod).c_str());
	for (auto refe : *(sig->signedInfo->references->references))
	{
		xml_node refnode = refs.append_child("ofd:Reference");
		refnode.append_attribute("FileRef").set_value(refe.first->pln.c_str());
		refnode.append_child("ofd:CheckValue").append_child(node_pcdata).set_value(refe.second.c_str());
	}
	for (auto sa : sig->signedInfo->stampAnnots)
	{
		xml_node sanode = si.append_child("ofd:StampAnnot");
		sanode.append_attribute("ID").set_value(sa->id.c_str());
		sanode.append_attribute("PageRef").set_value(sa->pageRef);
		char tmp[MAX_PATH] = { 0 };
		std::string strBoundary = itoa(sa->boundary->x, tmp, 10);
		strBoundary += " ";
		strBoundary += itoa(sa->boundary->y, tmp, 10);
		strBoundary += " ";
		strBoundary += itoa(sa->boundary->w, tmp, 10);
		strBoundary += " ";
		strBoundary += itoa(sa->boundary->h, tmp, 10);
		sanode.append_attribute("Boundary").set_value(strBoundary.c_str());
		if (sa->clip)
		{
			ZeroMemory(tmp, MAX_PATH);
			std::string strClip = itoa(sa->clip->x, tmp, 10);
			strClip += " ";
			strClip += itoa(sa->clip->y, tmp, 10);
			strClip += " ";
			strClip += itoa(sa->clip->w, tmp, 10);
			strClip += " ";
			strClip += itoa(sa->clip->h, tmp, 10);
			sanode.append_attribute("Clip").set_value(strClip.c_str());
		}
	}
	t.append_child("ofd:SignedValue").append_child(node_pcdata).set_value(sig->signedValue.first->pln.c_str());
	char str[MAX_PATH] = { 0 };
	memcpy_s(str, ss->baseLoc->abs.length(), ss->baseLoc->abs.c_str(), ss->baseLoc->abs.length());
	if (!PathIsDirectory(str))
	{
		PathRemoveFileSpecA(str);
	}
	CreateDir(str);
	xmldoc->save_file(ss->baseLoc->abs.c_str());
	delete xmldoc;
}

void FileTimeToTime_t(FILETIME  ft, time_t  *t)
{
	LONGLONG  ll;

	ULARGE_INTEGER            ui;
	ui.LowPart = ft.dwLowDateTime;
	ui.HighPart = ft.dwHighDateTime;

	ll = ft.dwHighDateTime << 32 + ft.dwLowDateTime;  //这一步是不是多余的

	*t = ((LONGLONG)(ui.QuadPart - 116444736000000000) / 10000000);
	//将ui.QuadPart的数据类型ULONGLONG强制转换为time_t的LONGLONG数据类型
}

void signAndSaveSignedValue(unsigned char * puchDocProperty, int iDocPropertyLen, time_t t, unsigned char * digestData, int digestDataLen, const unsigned char * eslData, const unsigned int eslLen, void *sig0, OFDIUM_P1Sign_Func signFunc, OFDIUM_GetSignCert_Fun certFunc)
{
	//Cert
	int cbCert = 0;
	int certAlgOidLen = 0;
	certFunc(nullptr, &certAlgOidLen, nullptr, &cbCert);
	auto pbCert = new unsigned char[cbCert];
	auto certAlgOid = new char[certAlgOidLen];
	memset(pbCert, 0, cbCert);
	memset(certAlgOid, 0, certAlgOidLen);
	certFunc(certAlgOid, &certAlgOidLen, pbCert, &cbCert);

	V4_SES_Signature *sessignature = V4_SES_Signature_new();
	V4_TBS_Sign_free(sessignature->toSign);
	sessignature->toSign = V4_TBS_Sign_new();
	ASN1_INTEGER_set(sessignature->toSign->version, 4);

	const auto tmp = (const unsigned char **)&eslData;
	V4SESeal_free(sessignature->toSign->eseal);
	sessignature->toSign->eseal = d2i_V4SESeal(NULL, tmp, eslLen);

	

	ASN1_GENERALIZEDTIME_set(sessignature->toSign->timeinfo, t);

	ASN1_BIT_STRING_set(sessignature->toSign->dataHash, digestData, digestDataLen);
	ASN1_STRING_set(sessignature->toSign->propertyInfo, puchDocProperty, iDocPropertyLen);

	BYTE *tbsdata = nullptr;
	int tbslen = i2d_V4_TBS_Sign(sessignature->toSign, &tbsdata);

	int iDestLen = CalcBase64EncodeBufLen(tbslen, 0, 0);
	auto* chDest = new char[iDestLen];
	ZeroMemory(chDest, iDestLen);
	iDestLen = EncodeBase64((unsigned char *)chDest, tbsdata, tbslen, 0, 0);

	//Sign
	auto sig = (Signature *)sig0;
	int len = 0;
	signFunc((unsigned char *)chDest, strlen(chDest), nullptr, &len);
	auto sigdata = new unsigned char[len];
	memset(sigdata, 0, len);
	signFunc((unsigned char *)chDest, strlen(chDest), sigdata, &len);

	int iDestLen2 = CalcBase64DecodeBufLen(sigdata, len);
	auto* chDest2 = new unsigned char[iDestLen2];
	ZeroMemory(chDest2, iDestLen2);
	iDestLen2 = DecodeBase64(chDest2, sigdata, len);

	sessignature->signature->flags = 8;
	ASN1_BIT_STRING_set(sessignature->signature, chDest2, iDestLen2);
	sessignature->signature->flags = 8;
	ASN1_OCTET_STRING_set(sessignature->cert, pbCert, cbCert);

	sessignature->signatureAlgID = OBJ_txt2obj(certAlgOid, 1);

	BYTE *out = nullptr;
	int outlen = i2d_V4_SES_Signature(sessignature, &out);

	sig->signedValue.second->len = outlen;
	sig->signedValue.second->val = new unsigned char[sig->signedValue.second->len]{0};
	memcpy_s(sig->signedValue.second->val, sig->signedValue.second->len, out, sig->signedValue.second->len);
	//Save
	char str[MAX_PATH] = { 0 };
	memcpy_s(str, sig->signedValue.first->abs.length(), sig->signedValue.first->abs.c_str(), sig->signedValue.first->abs.length());
	if (!PathIsDirectory(str))
	{
		PathRemoveFileSpecA(str);
	}
	CreateDir(str);
	FILE *f = nullptr;
	fopen_s(&f, sig->signedValue.first->abs.c_str(), "wb+");
	fwrite(sig->signedValue.second->val, 1, sig->signedValue.second->len, f);
	fflush(f);
	fclose(f);
	free(chDest);
	free(chDest2);
	OPENSSL_free(out);
	OPENSSL_free(tbsdata);
	V4_SES_Signature_free(sessignature);
	sessignature = nullptr;
	free(pbCert);
	free(certAlgOid);
	free(sigdata);
	return;
}

//public static Hashtable sign(String src, String dest, byte[] fileByte,String reason, String location, int pageNum, String tsaDataBase64, String timestampUrl, String userName, String pwd,float x, float y, float width, float height, UCAPICOMLib.CertificateXClass cert_obj, long stPosition = -1)
OFDIUMEX_API int OFDIUM_Sign(char * outputSignedOFDFilePath, int digestMethod, void *fileHandle, const unsigned char *esldata, const unsigned int esllen, int docIndex, void *pstStampProp, int stStampPropCount, OFDIUM_P1Sign_Func signFunc, OFDIUM_GetSignCert_Fun certFunc, int isFinalSign)
{
	digestMethod = 2;
	if (listOpenedOFD.find((OFD *)fileHandle) == listOpenedOFD.end())
	{
		return 1;
	}
	char tmp[MAX_PATH] = { 0 };
	auto *ofd = (OFD *)fileHandle;
	OFDParser *ofdparser = listOpenedOFD[ofd];
	std::vector<ST_Loc*> tbsList;
	int sigIndex = 0;
	if (!ofd->documents[docIndex]->signatures)
	{
		ofd->documents[docIndex]->signatures = new Signatures();
		ofd->documents[docIndex]->signatures->maxSignId = "1";
		sigIndex = 0;
	}
	else
	{
		int i = atoi(ofd->documents[docIndex]->signatures->maxSignId.c_str());
		if (i!=0)
		{
			i++;
			ofd->documents[docIndex]->signatures->maxSignId = itoa(i, tmp, 10);
			sigIndex = i;
		}
		else if (ofd->documents[docIndex]->signatures->maxSignId[0]=='s')
		{
			i = atoi(ofd->documents[docIndex]->signatures->maxSignId.c_str() + 1);
			if (i != 0)
			{
				i++;
				ofd->documents[docIndex]->signatures->maxSignId = "s";
				sprintf_s(tmp, "s%03d", i);
				ofd->documents[docIndex]->signatures->maxSignId = tmp;
				sigIndex = i;
			}
			else
			{
				ofd->documents[docIndex]->signatures->maxSignId = "s001";
				sigIndex = 0;
			}
		}
		else
		{
			ofd->documents[docIndex]->signatures->maxSignId = "s001";
			sigIndex = 0;
		}
	}
	//S_S
	auto *ss = new Signatures_Signature();
	ss->notInXML = true;
	ss->id = ofd->documents[docIndex]->signatures->maxSignId;
	ss->type = Signatures_Signature::TYPE::SEAL;
	ss->baseLoc = new ST_Loc();
	ss->baseLoc->pln = "/Doc_" + std::string(_itoa(docIndex, tmp, 10)) + "/Signs/Sign_" + std::string(_itoa(sigIndex, tmp, 10)) + "/Signature.xml";
	ss->baseLoc->abs = "/Doc_" + std::string(_itoa(docIndex, tmp, 10)) + "/Signs/Sign_" + std::string(_itoa(sigIndex, tmp, 10)) + "/Signature.xml";
	ofdparser->relativePath(ofdparser->ofd_dir, ss->baseLoc->abs);
	//S
	auto *sig = new Signature();
	sig->signedInfo = new SignedInfo();
	sig->signedInfo->provider = new Provider();
	elsGetProvider(esldata, esllen, sig->signedInfo->provider);
	sig->signedInfo->signatureMethod = "1.2.156.10197.1.501";//TODO
	ZeroMemory(tmp, MAX_PATH);
	time_t timenow;
	time(&timenow);
	sig->signedInfo->signatureDateTime = time2string(timenow, GetTickCount() % 1000);
	

	for (int i = 0; i < stStampPropCount;i++)
	{
		auto *sa = new StampAnnot();
		ofd->documents[docIndex]->commonData->maxUnitID++;
		ZeroMemory(tmp, MAX_PATH);

		int pageIndex = ((st_StampPropForSign *)pstStampProp)[i].pageIndex;
		double x = ((st_StampPropForSign *)pstStampProp)[i].x;
		double y = ((st_StampPropForSign *)pstStampProp)[i].y;
		double width = ((st_StampPropForSign *)pstStampProp)[i].width;
		double height = ((st_StampPropForSign *)pstStampProp)[i].height;
		double clipx = ((st_StampPropForSign *)pstStampProp)[i].clipx;
		double clipy = ((st_StampPropForSign *)pstStampProp)[i].clipy;
		double clipwidth = ((st_StampPropForSign *)pstStampProp)[i].clipwidth;
		double clipheight = ((st_StampPropForSign *)pstStampProp)[i].clipheight;

		sa->id = _itoa(ofd->documents[docIndex]->commonData->maxUnitID, tmp, 10);
		sa->boundary = new ST_Box();
		sa->boundary->x = x;
		sa->boundary->y = y;
		sa->boundary->w = width;
		sa->boundary->h = height;
		if (doubleCmp(clipwidth, -1.0f) != 0)
		{
			sa->clip = new ST_Box();
			sa->clip->x = clipx;
			sa->clip->y = clipy;
			sa->clip->w = clipwidth;
			sa->clip->h = clipheight;
		}

		sa->pageRef = ofd->documents[docIndex]->pages[pageIndex].first->id;
		sig->signedInfo->stampAnnots.push_back(sa);
	}
	
	sig->signedInfo->references = new References();
	sig->signedInfo->references->checkMethod = (References::CHECKMETHOD)digestMethod;
	auto *svloc = new ST_Loc();
	svloc->pln = "/Doc_" + std::string(_itoa(docIndex, tmp, 10)) + "/Signs/Sign_" + std::string(_itoa(sigIndex, tmp, 10)) + "/SignedValue.dat";
	svloc->abs = "/Doc_" + std::string(_itoa(docIndex, tmp, 10)) + "/Signs/Sign_" + std::string(_itoa(sigIndex, tmp, 10)) + "/SignedValue.dat";
	ofdparser->relativePath(ofdparser->ofd_dir, svloc->abs);
	auto *sv = new SignedValue();
	sig->signedValue = std::make_pair(svloc, sv);
	ofd->documents[docIndex]->signatures->signatures.push_back(std::make_pair(ss, sig));
	//make
	makeReferences(ofd->docBodies[docIndex], ofd->documents[docIndex], ofdparser, sig->signedInfo->references, isFinalSign != 0);
	makeSignature(ofd, ofdparser, sig, ss);
	makeSignatures(ofd, ofdparser, docIndex, isFinalSign != 0);

	int digestlen = 0;
	unsigned char *digest = nullptr;
	digestFile(References::CHECKMETHOD::SM3, ss->baseLoc->abs.c_str(), digest, &digestlen);
	digest = new unsigned char[digestlen];
	digestFile(References::CHECKMETHOD::SM3, ss->baseLoc->abs.c_str(), digest, &digestlen);

	signAndSaveSignedValue((unsigned char *)ss->baseLoc->pln.c_str(), ss->baseLoc->pln.length(), timenow, digest, digestlen, esldata, esllen, sig, signFunc, certFunc);
	free(digest);
	ZipTool::compressDir(std::string(outputSignedOFDFilePath), ofdparser->ofd_dir+"\\*");
	return 0;
}


OFDIUMEX_API int OFDIUM_GetSealCount(void *fileHandle)
{
	if (listOpenedOFD.find((OFD *)fileHandle) == listOpenedOFD.end())
	{
		return -1;
	}
	auto *ofd = (OFD *)fileHandle;

	if (ofd->documents.size() != 1)
	{
		return -1;
	}

	Document *doc = ofd->documents[0];

	return doc->signatures?doc->signatures->signatures.size():0;
}

OFDIUMEX_API void * OFDIUM_GetSealByIndex(void *fileHandle, int index)
{
	if (listOpenedOFD.find((OFD *)fileHandle) == listOpenedOFD.end())
	{
		return nullptr;
	}
	auto *ofd = (OFD *)fileHandle;

	if (ofd->documents.size() != 1)
	{
		return nullptr;
	}

	Document *doc = ofd->documents[0];

	if (index < 0 || index > doc->signatures->signatures.size() - 1)
	{
		return nullptr;
	}
	return doc->signatures->signatures[index].second;
}

OFDIUMEX_API int OFDIUM_GetSealProp(void *sealHandle, int sealPropType, void **propdata, unsigned int *propdatalen)
{
	auto *signature = (Signature *)sealHandle;
	switch (sealPropType)
	{
	case SEALPROPTYPE::PROVIDERNAME:
		*propdatalen = signature->signedInfo->provider->providerName.length() + 1;
		if (propdata)
		{
			memcpy(propdata, signature->signedInfo->provider->providerName.c_str(), signature->signedInfo->provider->providerName.length());
			((char *)propdata)[signature->signedInfo->provider->providerName.length()] = '\0';
		}
		break;
	case SEALPROPTYPE::_VERSION:
		*propdatalen = signature->signedInfo->provider->version.length() + 1;
		if (propdata)
		{
			memcpy(propdata, signature->signedInfo->provider->version.c_str(), signature->signedInfo->provider->version.length());
			((char *)propdata)[signature->signedInfo->provider->version.length()] = '\0';
		}
		break;
	case SEALPROPTYPE::COMPANY:
		*propdatalen = signature->signedInfo->provider->company.length() + 1;
		if (propdata)
		{
			memcpy(propdata, signature->signedInfo->provider->company.c_str(), signature->signedInfo->provider->company.length());
			((char *)propdata)[signature->signedInfo->provider->company.length()] = '\0';
		}
		break;
	case SEALPROPTYPE::SIGNATUREMETHOD:
		*propdatalen = signature->signedInfo->signatureMethod.length() + 1;
		if (propdata)
		{
			memcpy(propdata, signature->signedInfo->signatureMethod.c_str(), signature->signedInfo->signatureMethod.length());
			((char *)propdata)[signature->signedInfo->signatureMethod.length()] = '\0';
		}
		break;
	case SEALPROPTYPE::SIGNATUREDATETIME:
		*propdatalen = signature->signedInfo->signatureDateTime.length() + 1;
		if (propdata)
		{
			memcpy(propdata, signature->signedInfo->signatureDateTime.c_str(), signature->signedInfo->signatureDateTime.length());
			((char *)propdata)[signature->signedInfo->signatureDateTime.length()] = '\0';
		}
		break;
	case SEALPROPTYPE::STAMPANNOTCOUNT:
		*propdatalen = sizeof(int);
		if (propdata)
		{
			auto dest = (int *)propdata;
			*dest = signature->signedInfo->stampAnnots.size();
		}
		break;
	case SEALPROPTYPE::SIGNEDVALUEDATABASE64:
		*propdatalen = CalcBase64EncodeBufLen(signature->signedValue.second->len, 0, 0);
		if (propdata)
		{
			*propdatalen = EncodeBase64((unsigned char *)propdata, signature->signedValue.second->val, signature->signedValue.second->len, 0, 0);
		}
		break;
	default:
		return -1;
		break;
	}

	return 0;
}

OFDIUMEX_API void * OFDIUM_GetStampAnnotByIndex(void *sealHandle, int index)
{
	auto *signature = (Signature *)sealHandle;
	if (index < 0 || index > signature->signedInfo->stampAnnots.size() - 1)
	{
		return nullptr;
	}
	return signature->signedInfo->stampAnnots[index];
}

OFDIUMEX_API int OFDIUM_GetStampAnnotProp(void *stampAnnotHandle, int stampAnnotPropType, void **propdata, unsigned int *propdatalen)
{
	auto *stampAnnot = (StampAnnot *)stampAnnotHandle;
	switch (stampAnnotPropType)
	{
	case PAGEINDEX:
		*propdatalen = sizeof(int);
		if (propdata)
		{
			auto dest = (int *)propdata;
			*dest = -1;
			for (auto page : stampAnnot->fromSignature->fromDoc->pages)
			{
				if (stampAnnot->pageRef == page.first->id)
				{
					*dest = page.second->pageIndex;
				}
			}
		}
		break;
	case X:
		*propdatalen = sizeof(double);
		if (propdata)
		{
			auto dest = (double *)propdata;
			*dest = stampAnnot->boundary->x;
		}
		break;
	case Y:
		*propdatalen = sizeof(double);
		if (propdata)
		{
			auto dest = (double *)propdata;
			*dest = stampAnnot->boundary->y;
		}
		break;
	case W:
		*propdatalen = sizeof(double);
		if (propdata)
		{
			auto dest = (double *)propdata;
			*dest = stampAnnot->boundary->w;
		}
		break;
	case H:
		*propdatalen = sizeof(double);
		if (propdata)
		{
			auto dest = (double *)propdata;
			*dest = stampAnnot->boundary->h;
		}
		break;
	case CLIPX:
		*propdatalen = sizeof(double);
		if (propdata)
		{
			auto dest = (double *)propdata;
			*dest = stampAnnot->clip ? stampAnnot->clip->x : -1.0f;
		}
		break;
	case CLIPY:
		*propdatalen = sizeof(double);
		if (propdata)
		{
			auto dest = (double *)propdata;
			*dest = stampAnnot->clip ? stampAnnot->clip->y : -1.0f;
		}
		break;
	case CLIPW:
		*propdatalen = sizeof(double);
		if (propdata)
		{
			auto dest = (double *)propdata;
			*dest = stampAnnot->clip ? stampAnnot->clip->w : -1.0f;
		}
		break;
	case CLIPH:
		*propdatalen = sizeof(double);
		if (propdata)
		{
			auto dest = (double *)propdata;
			*dest = stampAnnot->clip ? stampAnnot->clip->h : -1.0f;
		}
		break;
	case SIGNFIELDNAME:
		*propdatalen = stampAnnot->id.length() + 1;
		if (propdata)
		{
			memcpy(propdata, stampAnnot->id.c_str(), stampAnnot->id.length());
			((char *)propdata)[stampAnnot->id.length()] = '\0';
		}
	default:
		return -1;
		break;
	}
	return 0;
}

inline void elsGetProvider(const unsigned char *esldata, const unsigned int esllen, void *provider)
{
	auto *prov = (Provider *)provider;
	prov->company = "XXCA Co.,Ltd.";
	prov->providerName = "XXCA";
	prov->version = "V4";
}

inline void digestFile(int checkmethod, const char *filename, unsigned char *digest, int *digestlen)
{
	if (!digest)
	{
		*digestlen = EVP_MAX_MD_SIZE;
		return;
	}
	EVP_MD_CTX *ctx = EVP_MD_CTX_new();
	const EVP_MD *md = nullptr;
	if (checkmethod == References::CHECKMETHOD::MD5)
	{
		md = EVP_md5();
	} 
	else if (checkmethod == References::CHECKMETHOD::SHA1)
	{
		md = EVP_sha1();
	}
	else if (checkmethod == References::CHECKMETHOD::SM3)
	{
		md = EVP_sm3();
	}
	else{
		md = EVP_md5();
	}
	int ret = EVP_DigestInit(ctx, md);
	FILE *f = nullptr;
	fopen_s(&f, filename, "rb");
	if (f == nullptr)
	{
		TRACE("error opening %s\n", filename);
		return;
	}
	unsigned char buf[16];
	for (;;)
	{
		int n_bytes = fread(buf, 1, 16, f);
		if (n_bytes <= 0)
			break;
		if (!EVP_DigestUpdate(ctx, buf, n_bytes))
			break;
	}
	fclose(f);
	
	ret = EVP_DigestFinal(ctx, digest, (unsigned int *)digestlen);
	EVP_MD_CTX_free(ctx);
}

inline std::string digestFileBase64(int checkMethod, const char *filename)
{
	int digestlen = 0;
	unsigned char *digest = nullptr;
	digestFile(checkMethod, filename, digest, &digestlen);
	digest = new unsigned char[digestlen];
	digestFile(checkMethod, filename, digest, &digestlen);
	int iDestLen = CalcBase64EncodeBufLen(digestlen, 0, 0);
	auto* chDest = new char[iDestLen];
	ZeroMemory(chDest, iDestLen);
	iDestLen = EncodeBase64((unsigned char *)chDest, digest, digestlen, 0, 0);
	delete digest;
	digest = nullptr;
	std::string strMd = chDest;
	delete chDest;
	chDest = nullptr;
	return strMd;
}

ST_Loc *convertStlocWithRoot(ST_Loc *path, std::string root_path)
{
	auto ret = new ST_Loc();
	ret->abs = path->abs;
	ret->pln = path->abs.c_str() + root_path.length();
	std::replace(ret->pln.begin(), ret->pln.end(), '\\', '/');
	return ret;
}

void makeReferences(void *docbody, void *doc, void *ofdparser, void *references, bool isFinalSign)
{
	auto *indocbody = (DocBody *)docbody;
	auto *indoc = (Document *)doc;
	auto *inparser = (OFDParser *)ofdparser;
	auto *inlistTbs = (References *)references;
	inlistTbs->references = new std::map<ST_Loc *, std::string>();
	inlistTbs->references->insert(std::make_pair(convertStlocWithRoot(indocbody->docRoot, inparser->ofd_dir), digestFileBase64(inlistTbs->checkMethod, indocbody->docRoot->abs.c_str())));
	if (isFinalSign)
	{
		inlistTbs->references->insert(std::make_pair(convertStlocWithRoot(indocbody->Signatures, inparser->ofd_dir), digestFileBase64(inlistTbs->checkMethod, indocbody->Signatures->abs.c_str())));
	}
	if (indoc->attachments)
	{
		inlistTbs->references->insert(std::make_pair(convertStlocWithRoot(indoc->attachments, inparser->ofd_dir), digestFileBase64(inlistTbs->checkMethod, indoc->attachments->abs.c_str())));
	}
	if (indoc->annotations)
	{
		inlistTbs->references->insert(std::make_pair(convertStlocWithRoot(indoc->annotations, inparser->ofd_dir), digestFileBase64(inlistTbs->checkMethod, indoc->annotations->abs.c_str())));
	}
	if (indoc->customTags)
	{
		inlistTbs->references->insert(std::make_pair(convertStlocWithRoot(indoc->customTags, inparser->ofd_dir), digestFileBase64(inlistTbs->checkMethod, indoc->customTags->abs.c_str())));
	}
	if (indoc->extensions)
	{
		inlistTbs->references->insert(std::make_pair(convertStlocWithRoot(indoc->extensions, inparser->ofd_dir), digestFileBase64(inlistTbs->checkMethod, indoc->extensions->abs.c_str())));
	}
	for (auto resf : indoc->commonData->documentRes)
	{
		inlistTbs->references->insert(std::make_pair(convertStlocWithRoot(resf.first, inparser->ofd_dir), digestFileBase64(inlistTbs->checkMethod, resf.first->abs.c_str())));
	}
	for (auto resf : indoc->commonData->publicRes)
	{
		inlistTbs->references->insert(std::make_pair(convertStlocWithRoot(resf.first, inparser->ofd_dir), digestFileBase64(inlistTbs->checkMethod, resf.first->abs.c_str())));
	}
	for (auto tpl : indoc->commonData->templatePages)
	{
		inlistTbs->references->insert(std::make_pair(convertStlocWithRoot(tpl.first->baseLoc, inparser->ofd_dir), digestFileBase64(inlistTbs->checkMethod, tpl.first->baseLoc->abs.c_str())));
	}
	for (auto &kv : *(indoc->fromOfd->resAll))
	{
		switch (kv.second.first)
		{
		case Res::TYPE::FONTS:
			if (kv.second.second->font->fontFile)
			{
				inlistTbs->references->insert(std::make_pair(convertStlocWithRoot(kv.second.second->font->fontFile, inparser->ofd_dir), digestFileBase64(inlistTbs->checkMethod, kv.second.second->font->fontFile->abs.c_str())));
			}
			break;
		case Res::TYPE::MULTIMEDIAS:
			if (kv.second.second->multiMedia->MediaFile)
			{
				inlistTbs->references->insert(std::make_pair(convertStlocWithRoot(kv.second.second->multiMedia->MediaFile, inparser->ofd_dir), digestFileBase64(inlistTbs->checkMethod, kv.second.second->multiMedia->MediaFile->abs.c_str())));
			}
			break;
		default:
			break;
		}
	}
	for (auto page : indoc->pages)
	{
		inlistTbs->references->insert(std::make_pair(page.first->baseLoc ? convertStlocWithRoot(page.first->baseLoc, inparser->ofd_dir) : convertStlocWithRoot(page.first->loc, inparser->ofd_dir), digestFileBase64(inlistTbs->checkMethod, page.first->baseLoc ? page.first->baseLoc->abs.c_str() : page.first->loc->abs.c_str())));
		for (auto pageRes : page.second->pageRes)
		{
			inlistTbs->references->insert(std::make_pair(convertStlocWithRoot(pageRes, inparser->ofd_dir), digestFileBase64(inlistTbs->checkMethod, pageRes->abs.c_str())));
		}
	}
}