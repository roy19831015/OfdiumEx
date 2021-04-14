// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 OFDIUMEX_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// OFDIUMEX_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef OFDIUMEX_EXPORTS
#define OFDIUMEX_API __declspec(dllexport)
#else
#define OFDIUMEX_API __declspec(dllimport)
#endif
OFDIUMEX_API void OFDIUM_AddRef();
OFDIUMEX_API void OFDIUM_Release();
OFDIUMEX_API void *OFDIUM_LoadCustomDocument(const char *fileName, unsigned long openMode);

OFDIUMEX_API int OFDIUM_GetPageCount(void *fileHandle);
OFDIUMEX_API void *OFDIUM_GetDocPermissions(void *fileHandle);
OFDIUMEX_API void *OFDIUM_InitFormFillEnvironment(void *fileHandle, void *formInfo);
OFDIUMEX_API void OFDIUM_SetFormFieldHighlightColor(void *fileHandle, int fieldType, unsigned int color);
OFDIUMEX_API void OFDIUM_SetFormFieldHighlightAlpha(void *fileHandle, unsigned char alpha);
OFDIUMEX_API void OFDIUM_DoDocumentJSAction(void *hHandle);
OFDIUMEX_API void OFDIUM_DoDocumentOpenAction(void *hHandle);
OFDIUMEX_API void OFDIUM_ExitFormFillEnvironment(void *hHandle);
OFDIUMEX_API void OFDIUM_DoDocumentAAction(void *hHandle, int aaType);
OFDIUMEX_API void *OFDIUM_LoadPage(void *fileHandle, int page_index);
OFDIUMEX_API void *OFDIUM_Text_LoadPage(void *fileHandle, int page_index);
OFDIUMEX_API void OFDIUM_OnAfterLoadPage(void *pageHandle, void *formHandle);
OFDIUMEX_API void OFDIUM_DoPageAAction(void *pageHandle, void *formHandle, int fPDFPAGE_AACTION);
OFDIUMEX_API double OFDIUM_GetPageWidth(void *pageHandle);
OFDIUMEX_API double OFDIUM_GetPageHeight(void *pageHandle);
OFDIUMEX_API void OFDIUM_OnBeforeClosePage(void *pageHandle, void *formHandle);
OFDIUMEX_API void OFDIUM_Text_ClosePage(void *textPageHandle);
OFDIUMEX_API void OFDIUM_ClosePage(void *pageHandle);
OFDIUMEX_API void OFDIUM_RenderPage(void *dc, void *pageHandle, int start_x, int start_y, int size_x, int size_y, int rotate, int flags);
OFDIUMEX_API void OFDIUM_RenderPageBitmap(void *bitmapHandle, void *pageHandle, int start_x, int start_y, int size_x, int size_y, int rotate, int flags);
OFDIUMEX_API int OFDIUM_GetPageSizeByIndex(void *fileHandle, int page_index, double *width, double *height);
OFDIUMEX_API void *OFDIUM_Bitmap_CreateEx(int width, int height, int format, unsigned char *first_scan, int stride);
OFDIUMEX_API void OFDIUM_Bitmap_FillRect(void *bitmapHandle, int left, int top, int width, int height, unsigned int color);
OFDIUMEX_API void *OFDIUM_Bitmap_Destroy(void *bitmapHandle);
OFDIUMEX_API void *OFDIUM_Text_FindStart(void *page, const unsigned short*findWhat, int flags, int start_index);
OFDIUMEX_API int OFDIUM_Text_GetSchResultIndex(void *handle);
OFDIUMEX_API int OFDIUM_Text_GetSchCount(void *handle);
OFDIUMEX_API int OFDIUM_Text_GetText(void *pageHandle, int start_index, int count, unsigned short* result);
OFDIUMEX_API int OFDIUM_Text_GetCharBox(void *pageHandle, int index, double *left, double *right, double *bottom, double *top);
OFDIUMEX_API int OFDIUM_Text_CountChars(void *pageHandle);
OFDIUMEX_API bool OFDIUM_Text_FindNext(void *pageHandle);
OFDIUMEX_API void OFDIUM_Text_FindClose(void *pageHandle);
OFDIUMEX_API bool OFDIUM_Link_Enumerate(void *pageHandle, int *startPos, void **linkAnnot);
OFDIUMEX_API void *OFDIUM_Link_GetDest(void *fileHandle, void *link);
OFDIUMEX_API unsigned int OFDIUM_Dest_GetPageIndex(void *fileHandle, void *dest);
OFDIUMEX_API bool OFDIUM_Link_GetAnnotRect(void *linkAnnot, void *rect);
OFDIUMEX_API void OFDIUM_DeviceToPage(void *pageHandle, int start_x, int start_y, int size_x, int size_y, int rotate, int device_x, int device_y, double *page_x, double *page_y);
OFDIUMEX_API void OFDIUM_PageToDevice(void *pageHandle, int start_x, int start_y, int size_x, int size_y, int rotate, double page_x, double page_y, int *device_x, int *device_y);
OFDIUMEX_API void *OFDIUM_Link_GetAction(void *link);
OFDIUMEX_API unsigned long OFDIUM_Action_GetURIPath(void *fileHandle, void *action, void *buffer, unsigned long buflen);
OFDIUMEX_API void *OFDIUM_Bookmark_GetFirstChild(void *fileHandle, void *bookmark);
OFDIUMEX_API void *OFDIUM_Bookmark_GetNextSibling(void *fileHandle, void *bookmark);
OFDIUMEX_API unsigned long OFDIUM_Bookmark_GetTitle(void *bookmark, void *buffer, unsigned long buflen);
OFDIUMEX_API void *OFDIUM_Bookmark_GetAction(void *bookmark);
OFDIUMEX_API void *OFDIUM_Bookmark_GetDest(void *fileHandle, void *bookmark);
OFDIUMEX_API unsigned long OFDIUM_Action_GetType(void *action);
OFDIUMEX_API unsigned long OFDIUM_GetLastError();
OFDIUMEX_API unsigned long OFDIUM_GetMetaText(void *fileHandle, char *tag, void *buffer, unsigned long buflen);
OFDIUMEX_API void OFDIUM_FFLDraw(void *form, void *bitmap, void *page, int start_x, int start_y, int size_x, int size_y, int rotate, int flags);
OFDIUMEX_API void OFDIUM_FFLDrawEx(void *form, void *bitmap, void *page, int start_x, int start_y, int size_x, int size_y, int rotate, int flags, char *strJson);
OFDIUMEX_API void OFDIUM_CloseDocument(void *fileHandle);
//TESTFUNC
OFDIUMEX_API void OFDIUM_GetDocumentPagePNG(void *fileHandle, int pageIndex, int width, int height, unsigned char *pngData, int *pngDataLen);
OFDIUMEX_API void OFDIUM_RenderDocumentPageToDC(void *fileHandle, int pageIndex, int width, int height, int start_x, int start_y, int rotate, void *hdc);
OFDIUMEX_API void OFDIUM_RenderDocumentPageToSurface(void *fileHandle, int pageIndex, int width, int height, int start_x, int start_y, int rotate, void *surfaceTarget);
OFDIUMEX_API int OFDIUM_GetDocumentPageWidth(void *fileHandle, int pageIndex);
OFDIUMEX_API int OFDIUM_GetDocumentPageHeight(void *fileHandle, int pageIndex);
OFDIUMEX_API void OFDIUM_Test();

#pragma pack (1)
struct st_StampPropForSign
{
	int pageIndex;
	double x;
	double y;
	double width;
	double height;
	double clipx;
	double clipy;
	double clipwidth;
	double clipheight;
};
#pragma pack ()

OFDIUMEX_API void *OFDIUM_LoadMemDocument(const unsigned char *data, unsigned long len, unsigned long openMode);
using OFDIUM_P1Sign_Func = int(*)(unsigned char *data, int datalen, unsigned char *out, int *outlen);
using OFDIUM_GetSignCert_Fun = int(*)(char *certAlgOid, int *certAlgOidLen, unsigned char *pbCert, int *pcbCert);
OFDIUMEX_API int OFDIUM_Sign(char * outputSignedOFDFilePath, int digestMethod, void *fileHandle, const unsigned char *esldata, const unsigned int esllen, int docIndex, void *pstStampProp, int stStampPropCount, OFDIUM_P1Sign_Func signFunc, OFDIUM_GetSignCert_Fun certFunc, int isFinalSign = 0);
OFDIUMEX_API int OFDIUM_GetSealCount(void *fileHandle);

enum SEALPROPTYPE
{
	PROVIDERNAME, _VERSION, COMPANY, SIGNATUREMETHOD, SIGNATUREDATETIME, STAMPANNOTCOUNT, SIGNEDVALUEDATABASE64
};
enum STAMPANNOTPROPTYPE
{
	PAGEINDEX, X, Y, W, H, CLIPX, CLIPY, CLIPW, CLIPH, SIGNFIELDNAME
};

OFDIUMEX_API void *OFDIUM_GetSealByIndex(void *fileHandle, int index);
OFDIUMEX_API int OFDIUM_GetSealProp(void *sealHandle, int sealPropType, void **propdata, unsigned int *propdatalen);
OFDIUMEX_API void *OFDIUM_GetStampAnnotByIndex(void *sealHandle, int index);
OFDIUMEX_API int OFDIUM_GetStampAnnotProp(void *stampAnnotHandle, int stampAnnotPropType, void **propdata, unsigned int *propdatalen);


void *cairo_image_surface_create_from_ofd_stream(const unsigned char *data, const unsigned int len, int width, int height);
void makeReferences(void *docbody, void *doc, void *ofdparser, void *references, bool isFinalSign = false);
inline void digestFile(int checkmethod, const char *filename, unsigned char *digest, int *digestlen);
inline void elsGetProvider(const unsigned char *esldata, const unsigned int esllen, void *provider);
void makeSignatures(void * fileHandle, void *parserHandle, int docIndex, bool isFinalSign);
void makeSignature(void * fileHandle, void * parserHandle, void * sigHandle, void *ssHandle);
void signAndSaveSignedValue(unsigned char * puchDocProperty, int iDocPropertyLen, time_t t, unsigned char * digestData, int digestDataLen, const unsigned char * eslData, const unsigned int eslLen, void *sig0, OFDIUM_P1Sign_Func signFunc, OFDIUM_GetSignCert_Fun certFunc);
//TESTFUNC
