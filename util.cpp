#include "stdafx.h"
#include "util.h"
#include <string>
#include <objbase.h>
#include <sstream>
#include <vector>
#include <comutil.h>
#include <algorithm>
#include <ShlObj.h>
#include <gdiplus.h>
#pragma comment(lib,"gdiplus.lib")
#include "Definitions.h"
#include "cairo.h"
#include <direct.h>
#include  <io.h>

int doubleCmp(double a, double b)
{
	const double EPS = 1e-6;  //一般这样子就够，但有时具体题目要考虑是否要更小的 
	if (fabs(a - b) < EPS)  //判断是否相等 
	{
		return 0;
	}
	else if (a > b + EPS)
	{
		return 1;
	}
	else{
		return -1;
	}
}



void CreateDir(const char *dir)
{
	int m = 0, n;
	std::string str1, str2;
	str1 = dir;
	str2 = str1.substr(0, 2);
	str1 = str1.substr(3, str1.size());
	while (m >= 0)
	{
		m = str1.find('\\');
		str2 += '\\' + str1.substr(0, m);
		n = _access(str2.c_str(), 0); //判断该目录是否存在
		if (n == -1)
		{
			_mkdir(str2.c_str());     //创建目录
		}
		str1 = str1.substr(m + 1, str1.size());
	}
}

std::string GenerateGUIDString()
{
	GUID guid;
	CoCreateGuid(&guid);
	char buf[64] = { 0 };
	sprintf_s(buf, sizeof(buf), "OFD-%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1],
		guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5],
		guid.Data4[6], guid.Data4[7]);
	return std::string(buf);
}

time_t string2time(std::string strTime)
{
	tm tm1;
	memset(&tm1, 0, sizeof(tm1));
	time_t time1;
	sscanf_s(strTime.c_str(), "%d-%d-%d %d:%d:%d",
		&(tm1.tm_year),
		&(tm1.tm_mon),
		&(tm1.tm_mday),
		&(tm1.tm_hour),
		&(tm1.tm_min),
		&(tm1.tm_sec));
	tm1.tm_year -= 1900;
	tm1.tm_mon -= 1;
	time1 = mktime(&tm1);
	return time1;
}

std::string time2string(time_t time,unsigned long usec)
{
	tm tm1;
	memset(&tm1, 0, sizeof(tm1));
	localtime_s(&tm1, &time);
	char tmp[MAX_PATH] = { 0 };
	strftime(tmp, MAX_PATH, "%Y%m%d%H%M%S", &tm1);
	std::string out(tmp);
	out += ".";
	char chtmp[4] = { 0 };
	ltoa(usec, chtmp, 10);
	out += chtmp;
	out += "Z";
	return out;
}

std::string time2generalizedstring(time_t time)
{
	tm tm1;
	memset(&tm1, 0, sizeof(tm1));
	localtime_s(&tm1, &time);
	char tmp[MAX_PATH] = { 0 };
	strftime(tmp, MAX_PATH, "%Y-%m-%d %H:%M:%S", &tm1);
	return std::string(tmp);
}

void split(const std::string& s, char delimiter, std::vector<std::string> &out)
{
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		out.push_back(token);
	}
}

cairo_status_t cairo_read_func_mine(void *closure, unsigned char *data, unsigned int length)
{
	auto* pPngData = (st_png_data*)closure;
	memcpy(data, pPngData->pdata + pPngData->length, length);
	pPngData->length += length;
	return CAIRO_STATUS_SUCCESS;
}

cairo_status_t cairo_write_func_mine(void *closure, const unsigned char *data, unsigned int length)
{
	auto* pPngData = (st_png_data*)closure;
	memcpy(pPngData->pdata + pPngData->length, data, length);
	pPngData->length += length;
	return CAIRO_STATUS_SUCCESS;
}

inline std::string convertPathToAbsolute(std::string relativePath, std::string baseDir)
{
	std::string str = relativePath;
	std::replace(str.begin(), str.end(), '/', '\\');
	if (PathIsRelative(str.c_str()))
	{
		char tmp[MAX_PATH] = { 0 };
		memcpy(tmp, baseDir.c_str(), baseDir.length());
		PathRemoveFileSpecA(tmp);
		PathAddBackslash(tmp);
		str = std::string(tmp) + str;
	}
	return str;
}

cairo_surface_t *cairo_image_surface_create_from_jpeg(const char *filename, std::map<cairo_surface_t *, std::pair<Gdiplus::Bitmap *, Gdiplus::BitmapData *>> *mapImage, ULONG_PTR *pGdiplusToken)
{
	Gdiplus::GdiplusStartupInput m_gdiplusStartupInput;
	Gdiplus::GdiplusStartup(pGdiplusToken, &m_gdiplusStartupInput, NULL);

	/*  OnPaint显示 jpg图片  */
	auto* pImage = new Gdiplus::Bitmap(_com_util::ConvertStringToBSTR(filename));
	auto *bitmapData = new Gdiplus::BitmapData();
	auto * rect = new Gdiplus::Rect(0, 0, pImage->GetWidth(), pImage->GetHeight());
	pImage->LockBits(rect, Gdiplus::ImageLockMode::ImageLockModeRead, PixelFormat32bppARGB, bitmapData);
	delete rect;
	cairo_surface_t *surface = cairo_image_surface_create_for_data((BYTE *)bitmapData->Scan0, cairo_format_t::CAIRO_FORMAT_ARGB32, pImage->GetWidth(), pImage->GetHeight(), cairo_format_stride_for_width(cairo_format_t::CAIRO_FORMAT_ARGB32, pImage->GetWidth()));
	/* 使用完 GDI+函数后, 需要卸载 GDI+, 释放 GDI+使用的资源 */
	mapImage->insert(std::make_pair(surface, std::make_pair(pImage, bitmapData)));
	return surface;
}

void cairo_image_surface_delete_from_jpeg(cairo_surface_t *surface, std::map<cairo_surface_t *, std::pair<Gdiplus::Bitmap *, Gdiplus::BitmapData *>> *mapImage, ULONG_PTR *pGdiplusToken)
{
	delete mapImage->at(surface).first;
	delete mapImage->at(surface).second;
	mapImage->erase(surface);
	if (mapImage->size()==0)
	{
		Gdiplus::GdiplusShutdown(*pGdiplusToken);
		*pGdiplusToken = NULL;
	}
}

void cairo_image_surface_delete_from_jbig2(cairo_surface_t *surface, std::map<cairo_surface_t *, std::pair<Gdiplus::Bitmap *, Gdiplus::BitmapData *>> *mapImage)
{
	delete mapImage->at(surface).first;
	delete mapImage->at(surface).second;
	mapImage->erase(surface);
}

cairo_surface_t *cairo_image_surface_create_from_jbig2(const char *filename, int *width, int *height, unsigned char **ptrRememberToFree)
{
	FILE *f = nullptr, *f_page = nullptr;
	Jbig2Ctx *ctx;
	uint8_t buf[1];
	ctx = jbig2_ctx_new(nullptr, (Jbig2Options)0,
		nullptr,
		nullptr, nullptr);

	fopen_s(&f, filename, "rb");
	if (f == nullptr)
	{
		TRACE("error opening %s\n", filename);
		return nullptr;
	}

	for (;;)
	{
		int n_bytes = fread(buf, 1, sizeof(buf), f);
		if (n_bytes <= 0)
			break;
		if (jbig2_data_in(ctx, buf, n_bytes))
			break;
	}
	fclose(f);
	jbig2_complete_page(ctx);
	Jbig2Image *image;
	if ((image = jbig2_page_out(ctx)) != nullptr) {
		int newstride = (image->stride % 8 == 0 ? image->stride : (1 + (image->stride / 8)) * 8);
		int len = image->height*newstride;
		auto *newimage = (BYTE *)malloc(len);
		*ptrRememberToFree = newimage;
		memset(newimage, 0x00, len);
		for (int j = 0; j < image->height; j++)
		{
			byte *linedata = image->data + image->stride * j;
			memcpy(newimage + newstride*j, linedata, image->stride);
			for (int i = 0; i < newstride; i++)
			{
				ReverseBit(newimage + newstride*j+i);
			}
		}
		cairo_surface_t *ret = cairo_image_surface_create_for_data(newimage, cairo_format_t::CAIRO_FORMAT_A1, image->width, image->height, newstride);
		cairo_t *tret = cairo_create(ret);
		*width = image->width;
		*height = image->height;
		jbig2_release_page(ctx, image);
		jbig2_ctx_free(ctx);
		return ret;
	}
	jbig2_ctx_free(ctx);
	return nullptr;
}

void ct_matrix_to_cairo_matrix(CT_Matrix *ct_matrix, cairo_matrix_t *cairo_ctm)
{
	cairo_ctm->xx = ct_matrix->xx;
	cairo_ctm->xy = ct_matrix->xy;
	cairo_ctm->yx = ct_matrix->yx;
	cairo_ctm->yy = ct_matrix->yy;
	cairo_ctm->x0 = MM2PIXEL(ct_matrix->x0);
	cairo_ctm->y0 = MM2PIXEL(ct_matrix->y0);
}

void fnReadCharactersUTF8(const char* pszSentence, std::vector<std::string>& vec)
{
	std::string text = pszSentence;
	do
	{
		if (text.length() <= 0)
			break;
		std::string::const_iterator begin = text.begin();
		std::string::const_iterator end = text.end();
		while (begin != end)
		{
			unsigned char c = *begin;
			int n = 0;
			if ((c & 0x80) == 0)
				n = 1;
			else if ((c & 0xE0) == 0xC0)
				n = 2;
			else if ((c & 0xF0) == 0xE0)
				n = 3;
			else if ((c & 0xF8) == 0xF0)
				n = 4;
			else if ((c & 0xFC) == 0xF8)
				n = 5;
			else if ((c & 0xFE) == 0xFC)
				n = 6;
			else
				break;
			if (end - begin < n)
				break;
			std::string substring;
			substring += *begin;
			bool isError = false;
			for (int i = 1; i < n; ++i)
			{
				if ((begin[i] & 0xC0) != 0x80)
				{
					isError = true;
					break;
				}
				substring += begin[i];
			}
			if (isError)
				break;
			vec.push_back(substring);
			begin += n;
		}
	} while (false);
}

inline std::string UTF82GBK(const char *szUtf8)
{
	int n = MultiByteToWideChar(CP_UTF8, 0, szUtf8, -1, nullptr, 0);
	auto *wszGBK = new WCHAR[sizeof(WCHAR) * n];
	ZeroMemory(wszGBK, sizeof(WCHAR) * n);
	MultiByteToWideChar(CP_UTF8, 0, szUtf8, -1, wszGBK, n);
	char *ret = _com_util::ConvertBSTRToString(wszGBK);
	delete wszGBK;
	wszGBK = nullptr;
	return ret;
}

void cairo_quadratic_to(cairo_t *cr,double x1, double y1,double x2, double y2)
{
	double x0, y0;
	cairo_get_current_point(cr, &x0, &y0);
	cairo_curve_to(cr,
		2.0 / 3.0 * x1 + 1.0 / 3.0 * x0,
		2.0 / 3.0 * y1 + 1.0 / 3.0 * y0,
		2.0 / 3.0 * x1 + 1.0 / 3.0 * x2,
		2.0 / 3.0 * y1 + 1.0 / 3.0 * y2,
		y1, y2);
}

inline int jbig2_image_get_pixel(Jbig2Image *image, int x, int y)
{
	const int w = image->width;
	const int h = image->height;
	const int byte = (x >> 3) + y*image->stride;
	const int bit = 7 - (x & 7);

	if ((x < 0) || (x >= w)) return 0;
	if ((y < 0) || (y >= h)) return 0;

	return ((image->data[byte] >> bit) & 1);
}

inline void ReverseBit(unsigned char* pValue)
{
	unsigned char b = *pValue; // 要反转的字节
	b = (unsigned char)((b * 0x0202020202ULL & 0x010884422010ULL) % 1023);
	*pValue = b;
}

void print_errors()
{
	int    flags, line;
	char    *data, *file;
	unsigned long code;
	std::string outstr;

	code = ERR_get_error_line_data((const char **)&file, &line, (const char **)&data, &flags);
	while (code)
	{
		if (data && (flags & ERR_TXT_STRING))
		{
			outstr = "error data: ";
			outstr += data;
			outstr += "\n";
		}

		code = ERR_get_error_line_data((const char **)&file, &line, (const char **)&data, &flags);
	}
	MessageBox(nullptr, outstr.c_str(), "OPENSSL_ERROR", 0);
}

/* SESeal */
ASN1_SEQUENCE(SESeal) = {
	ASN1_SIMPLE(SESeal, esealInfo, SES_SealInfo),
	ASN1_SIMPLE(SESeal, signInfo, SES_SignInfo)
} ASN1_SEQUENCE_END(SESeal)
IMPLEMENT_ASN1_FUNCTIONS(SESeal)

/* V4SESeal */
ASN1_SEQUENCE(V4SESeal) = {
	ASN1_SIMPLE(V4SESeal, esealInfo, V4_SES_SealInfo),
	ASN1_SIMPLE(V4SESeal, cert, ASN1_OCTET_STRING),
	ASN1_SIMPLE(V4SESeal, signAlgID, ASN1_OBJECT),
	ASN1_SIMPLE(V4SESeal, signedValue, ASN1_BIT_STRING)
} ASN1_SEQUENCE_END(V4SESeal)
IMPLEMENT_ASN1_FUNCTIONS(V4SESeal)

/* SES_SealInfo */
ASN1_SEQUENCE(SES_SealInfo) = {
	ASN1_SIMPLE(SES_SealInfo, header, SES_Header),
	ASN1_SIMPLE(SES_SealInfo, esID, ASN1_IA5STRING),
	ASN1_SIMPLE(SES_SealInfo, property, SES_ESPropertyInfo),
	ASN1_SIMPLE(SES_SealInfo, picture, SES_ESPictrueInfo),
	ASN1_EXP_SEQUENCE_OF_OPT(SES_SealInfo, extDatas, ExtData, 0)
} ASN1_SEQUENCE_END(SES_SealInfo)
IMPLEMENT_ASN1_FUNCTIONS(SES_SealInfo)

/*V4_SES_SealInfo */
ASN1_SEQUENCE(V4_SES_SealInfo) = {
	ASN1_SIMPLE(V4_SES_SealInfo, header, SES_Header),
	ASN1_SIMPLE(V4_SES_SealInfo, esID, ASN1_IA5STRING),
	ASN1_SIMPLE(V4_SES_SealInfo, property, V4_SES_ESPropertyInfo),
	ASN1_SIMPLE(V4_SES_SealInfo, picture, V4_SES_ESPictrueInfo),
	ASN1_SEQUENCE_OF_OPT(V4_SES_SealInfo, extDatas, V4ExtData)
} ASN1_SEQUENCE_END(V4_SES_SealInfo)
IMPLEMENT_ASN1_FUNCTIONS(V4_SES_SealInfo)

/* ExtData */
ASN1_SEQUENCE(ExtData) = {
	ASN1_SIMPLE(ExtData, extnID, ASN1_OBJECT),
	ASN1_SIMPLE(ExtData, critical, ASN1_BOOLEAN),
	ASN1_SIMPLE(ExtData, extnValue, ASN1_OCTET_STRING)
} ASN1_SEQUENCE_END(ExtData)
IMPLEMENT_ASN1_FUNCTIONS(ExtData)

/* V4ExtData */
ASN1_SEQUENCE(V4ExtData) = {
	ASN1_SIMPLE(V4ExtData, extnID, ASN1_OBJECT),
	ASN1_SIMPLE(V4ExtData, critical, ASN1_BOOLEAN),
	ASN1_SIMPLE(V4ExtData, extnValue, ASN1_OCTET_STRING)
} ASN1_SEQUENCE_END(V4ExtData)
IMPLEMENT_ASN1_FUNCTIONS(V4ExtData)

/* CertDigestObj */
ASN1_SEQUENCE(CertDigestObj) = {
	ASN1_SIMPLE(CertDigestObj, type, ASN1_PRINTABLESTRING),
	ASN1_SIMPLE(CertDigestObj, value, ASN1_OCTET_STRING),
} ASN1_SEQUENCE_END(CertDigestObj)
IMPLEMENT_ASN1_FUNCTIONS(CertDigestObj)

/* SES_CertList */
ASN1_SEQUENCE(SES_CertList) = {
	ASN1_SIMPLE(SES_CertList, certs, ASN1_OCTET_STRING),
	//ASN1_IMP_SEQUENCE_OF(SES_CertList, d.certDigestList, CertDigestObj, GEN_CERTDIGESTLIST),
} ASN1_SEQUENCE_END(SES_CertList)
IMPLEMENT_ASN1_FUNCTIONS(SES_CertList)

/* SES_Header */
ASN1_SEQUENCE(SES_Header) = {
	ASN1_SIMPLE(SES_Header, ID, ASN1_IA5STRING),
	ASN1_SIMPLE(SES_Header, version, ASN1_INTEGER),
	ASN1_SIMPLE(SES_Header, Vid, ASN1_IA5STRING)
} ASN1_SEQUENCE_END(SES_Header)
IMPLEMENT_ASN1_FUNCTIONS(SES_Header)

/* V4_SES_Header */
ASN1_SEQUENCE(V4_SES_Header) = {
	ASN1_SIMPLE(V4_SES_Header, ID, ASN1_IA5STRING),
	ASN1_SIMPLE(V4_SES_Header, version, ASN1_INTEGER),
	ASN1_SIMPLE(V4_SES_Header, Vid, ASN1_IA5STRING)
} ASN1_SEQUENCE_END(V4_SES_Header)
IMPLEMENT_ASN1_FUNCTIONS(V4_SES_Header)

/* SES_ESPropertyInfo */
ASN1_SEQUENCE(SES_ESPropertyInfo) = {
	ASN1_SIMPLE(SES_ESPropertyInfo, type, ASN1_INTEGER),
	ASN1_SIMPLE(SES_ESPropertyInfo, name, ASN1_UTF8STRING),
	ASN1_SEQUENCE_OF(SES_ESPropertyInfo, certList, ASN1_OCTET_STRING),
	ASN1_SIMPLE(SES_ESPropertyInfo, createDate, ASN1_UTCTIME),
	ASN1_SIMPLE(SES_ESPropertyInfo, validStart, ASN1_UTCTIME),
	ASN1_SIMPLE(SES_ESPropertyInfo, validEnd, ASN1_UTCTIME)
} ASN1_SEQUENCE_END(SES_ESPropertyInfo)
IMPLEMENT_ASN1_FUNCTIONS(SES_ESPropertyInfo)

/* V4_SES_ESPropertyInfo */
ASN1_SEQUENCE(V4_SES_ESPropertyInfo) = {
	ASN1_SIMPLE(V4_SES_ESPropertyInfo, type, ASN1_INTEGER),
	ASN1_SIMPLE(V4_SES_ESPropertyInfo, name, ASN1_UTF8STRING),
	ASN1_SIMPLE(V4_SES_ESPropertyInfo, certListType, ASN1_INTEGER),
	ASN1_SEQUENCE_OF(V4_SES_ESPropertyInfo, certList, ASN1_OCTET_STRING),
	ASN1_SIMPLE(V4_SES_ESPropertyInfo, createDate, ASN1_GENERALIZEDTIME),
	ASN1_SIMPLE(V4_SES_ESPropertyInfo, validStart, ASN1_GENERALIZEDTIME),
	ASN1_SIMPLE(V4_SES_ESPropertyInfo, validEnd, ASN1_GENERALIZEDTIME)
} ASN1_SEQUENCE_END(V4_SES_ESPropertyInfo)
IMPLEMENT_ASN1_FUNCTIONS(V4_SES_ESPropertyInfo)

/* SES_ESPictrueInfo */
ASN1_SEQUENCE(SES_ESPictrueInfo) = {
	ASN1_SIMPLE(SES_ESPictrueInfo, type, ASN1_IA5STRING),
	ASN1_SIMPLE(SES_ESPictrueInfo, data, ASN1_OCTET_STRING),
	ASN1_SIMPLE(SES_ESPictrueInfo, width, ASN1_INTEGER),
	ASN1_SIMPLE(SES_ESPictrueInfo, height, ASN1_INTEGER)
} ASN1_SEQUENCE_END(SES_ESPictrueInfo)
IMPLEMENT_ASN1_FUNCTIONS(SES_ESPictrueInfo)

/* V4_SES_ESPictrueInfo */
ASN1_SEQUENCE(V4_SES_ESPictrueInfo) = {
	ASN1_SIMPLE(V4_SES_ESPictrueInfo, type, ASN1_IA5STRING),
	ASN1_SIMPLE(V4_SES_ESPictrueInfo, data, ASN1_OCTET_STRING),
	ASN1_SIMPLE(V4_SES_ESPictrueInfo, width, ASN1_INTEGER),
	ASN1_SIMPLE(V4_SES_ESPictrueInfo, height, ASN1_INTEGER),
} ASN1_SEQUENCE_END(V4_SES_ESPictrueInfo)
IMPLEMENT_ASN1_FUNCTIONS(V4_SES_ESPictrueInfo)

/* SES_SignInfo */
ASN1_SEQUENCE(SES_SignInfo) = {
	ASN1_SIMPLE(SES_SignInfo, cert, ASN1_OCTET_STRING),
	ASN1_SIMPLE(SES_SignInfo, signatureAlorithm, ASN1_OBJECT),
	ASN1_SIMPLE(SES_SignInfo, signData, ASN1_BIT_STRING)
} ASN1_SEQUENCE_END(SES_SignInfo)
IMPLEMENT_ASN1_FUNCTIONS(SES_SignInfo)

/* TBS_Sign */
ASN1_SEQUENCE(TBS_Sign) = {
	ASN1_SIMPLE(TBS_Sign, version, ASN1_INTEGER),
	ASN1_SIMPLE(TBS_Sign, eseal, SESeal),
	ASN1_SIMPLE(TBS_Sign, timeinfo, ASN1_BIT_STRING),
	ASN1_SIMPLE(TBS_Sign, dataHash, ASN1_BIT_STRING),
	ASN1_SIMPLE(TBS_Sign, propertyInfo, ASN1_IA5STRING),
	ASN1_SIMPLE(TBS_Sign, cert, ASN1_OCTET_STRING),
	ASN1_SIMPLE(TBS_Sign, signatureAlgorithm, ASN1_OBJECT)
} ASN1_SEQUENCE_END(TBS_Sign)
IMPLEMENT_ASN1_FUNCTIONS(TBS_Sign)

/* V4_TBS_Sign */
ASN1_SEQUENCE(V4_TBS_Sign) = {
	ASN1_SIMPLE(V4_TBS_Sign, version, ASN1_INTEGER),
	ASN1_SIMPLE(V4_TBS_Sign, eseal, V4SESeal),
	ASN1_SIMPLE(V4_TBS_Sign, timeinfo, ASN1_GENERALIZEDTIME),
	ASN1_SIMPLE(V4_TBS_Sign, dataHash, ASN1_BIT_STRING),
	ASN1_SIMPLE(V4_TBS_Sign, propertyInfo, ASN1_IA5STRING),
	ASN1_SEQUENCE_OF_OPT(V4_TBS_Sign, extDatas, V4ExtData)
} ASN1_SEQUENCE_END(V4_TBS_Sign)
IMPLEMENT_ASN1_FUNCTIONS(V4_TBS_Sign)

/* SES_Signature */
ASN1_SEQUENCE(SES_Signature) = {
	ASN1_SIMPLE(SES_Signature, toSign, TBS_Sign),
	ASN1_SIMPLE(SES_Signature, signature, ASN1_BIT_STRING)
} ASN1_SEQUENCE_END(SES_Signature)
IMPLEMENT_ASN1_FUNCTIONS(SES_Signature)

/* V4_SES_Signature */
ASN1_SEQUENCE(V4_SES_Signature) = {
	ASN1_SIMPLE(V4_SES_Signature, toSign, V4_TBS_Sign),
	ASN1_SIMPLE(V4_SES_Signature, cert, ASN1_OCTET_STRING),
	ASN1_SIMPLE(V4_SES_Signature, signatureAlgID, ASN1_OBJECT),
	ASN1_SIMPLE(V4_SES_Signature, signature, ASN1_BIT_STRING),
	ASN1_EXP_OPT(V4_SES_Signature, timeStamp, ASN1_BIT_STRING, 0)
} ASN1_SEQUENCE_END(V4_SES_Signature)
IMPLEMENT_ASN1_FUNCTIONS(V4_SES_Signature)