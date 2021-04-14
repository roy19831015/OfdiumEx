#include <string>
#include <vector>
#include <map>
#include <gdiplus.h>
#include "cairo.h"
#include "Definitions.h"

#include "jbig2dec/include/jbig2.h"
#include "jbig2dec/include/jbig2_image.h"

#pragma comment(lib,"jbig2dec.lib")

#define ALLOCATE(v,l,t) v = (t *)malloc((size_t)(sizeof(t)*(l)))
int doubleCmp(double a, double b);
void CreateDir(const char *dir);
std::string GenerateGUIDString();
time_t string2time(std::string strTime);
std::string time2string(time_t time, unsigned long usec);
std::string time2generalizedstring(time_t time);
void split(const std::string& s, char delimiter, std::vector<std::string> &out);
inline std::string UTF82GBK(const char *szUtf8);
#define MM2PIXEL72P(m) (double)(2.8346456692913387*(double)(m))
#define MM2PIXEL(m) (double)(11.811023622047246*(double)(m))

#define INTMM2PIXEL72P(m) (int)(2.8346456692913387*(double)(m))
#define INTMM2PIXEL(m) (int)(11.811023622047246*(double)(m))
struct st_png_data
{
	unsigned char* pdata;
	unsigned int length;
};

cairo_status_t cairo_read_func_mine(void *closure, unsigned char *data, unsigned int length);

cairo_status_t cairo_write_func_mine(void *closure, const unsigned char *data, unsigned int length);

inline std::string convertPathToAbsolute(std::string relativePath, std::string baseDir);

cairo_surface_t *cairo_image_surface_create_from_jpeg(const char *filename, std::map<cairo_surface_t *, std::pair<Gdiplus::Bitmap *, Gdiplus::BitmapData *>> *mapImage, ULONG_PTR *pGdiplusToken);

void cairo_image_surface_delete_from_jpeg(cairo_surface_t *surface, std::map<cairo_surface_t *, std::pair<Gdiplus::Bitmap *, Gdiplus::BitmapData *>> *mapImage, ULONG_PTR *pGdiplusToken);

cairo_surface_t *cairo_image_surface_create_from_jbig2(const char *filename, int *width, int *height, unsigned char **ptrRememberToFree);

void ct_matrix_to_cairo_matrix(CT_Matrix *ct_matrix, cairo_matrix_t *cairo_ctm);

void fnReadCharactersUTF8(const char* pszSentence, std::vector<std::string>& vec);

void cairo_quadratic_to(cairo_t *cr, double x1, double y1, double x2, double y2);

inline int jbig2_image_get_pixel(Jbig2Image *image, int x, int y);

inline void ReverseBit(unsigned char* pValue);

void print_errors();

#define M_PI       3.14159265358979323846

//SESeal:: = SEQUENCE{
//	esealInfo SES_SealInfo, --印章信息
//	signInfo SES_SignInfo -- 制章人对印签名的信息
//}
//SES_SealInfo:: = SEQUENCE{
//	header SES_Header, --头信息
//	esID IA5String, --电子印章标识，数据的唯一编码
//	property SES_ESPropertyInfo, --印章属性信息
//	picture SES_ESPictrueInfo, --电子印章图片数据
//	extDatas EXPLICIT ExtensionDatas OPTIONAL -- 自定义数据
//	国家公共资源交易服务平台一期工程电子印章互认系统软件开发与集成项目电子印章厂商对接指南
//	30
//}
//其中：
//esID ：区分电子印章数据的唯一标识编码，用于查找和索引其他信息。
//extDatas ：用于厂商使自定义数据。
//ExtensionDatas ：： = SEQUENCE SIZE （0..MAX ）OF ExtData
//ExtData ：： = SEQUENCE{
//		extnID OBJECT IDENTIFIER, --自定义扩展字段标识
//		critical BOOLEAN DEFAULT FALSE, --自定义扩展字段是否关键
//		extnValue OCTET STRING -- 自定义扩展字段数据值
//	}
//	（3） 印章头信息
//	头信息的 ASN.1 定义为：
//	SES_Header ：： = SEQUENCE{
//			ID IA5String, --电子印章数据标识
//			version INTEGER, --电子印章数据版本号标识
//			Vid IA5String -- 电子印章厂商 ID }
//SES_ESPropertyInfo ：： = SEQUENCE{
//	type INTEGER, --印章类型
//	name UTF8String ， -- 印章名称
//	certList SEQUENCE OF cert ， -- 签章人证书列表
//	createDate UTCTIME, --印章制作日期
//	validStart UTCTIME, --印章有效起始日期
//	validEnd UTCTIME -- 印章有效终止日期
//}
//SES_ESPictrueInfo ：： = SEOUENCE{
//	type IA5String ， -- 图片类型
//	国家公共资源交易服务平台一期工程电子印章互认系统软件开发与集成项目电子印章厂商对接指南
//	32
//	data OCTET STRING, --图片数据
//	width INTEGER, --图片显示宽度
//	height INTEGER -- 图片显示高度
//}
//SES_SignInfo ：： = SEQUENCE{
//	cert OCTET STRING, --制章人签名证书
//	signatureAlorithm OBJECT IDENTIFIER, --签名算法标识
//	signData BIT STRING -- 制章人的签名值
//}

typedef struct cert_digest_obj_st
{
	ASN1_PRINTABLESTRING *type;
	ASN1_OCTET_STRING *value;
}CertDigestObj;
DECLARE_ASN1_FUNCTIONS(CertDigestObj)

typedef struct ses_certlist_st {
	//# define GEN_CERTINFOLIST			0
	//# define GEN_CERTDIGESTLIST			1
	//int type;
	//	union {
	//char *ptr;
	STACK_OF(ASN1_OCTET_STRING) *certs;
	//		STACK_OF(CertDigestObj) *certDigestList;
	//	}d;
} SES_CertList;
DEFINE_STACK_OF(ASN1_OCTET_STRING)
DECLARE_ASN1_FUNCTIONS(SES_CertList)

typedef struct ses_espropertyinfo_st {
	ASN1_INTEGER *type;
	ASN1_UTF8STRING *name;
	STACK_OF(ASN1_OCTET_STRING) *certList;
	ASN1_UTCTIME *createDate;
	ASN1_UTCTIME *validStart;
	ASN1_UTCTIME *validEnd;
} SES_ESPropertyInfo;
DECLARE_ASN1_FUNCTIONS(SES_ESPropertyInfo)

typedef struct v4_ses_espropertyinfo_st {
	ASN1_INTEGER *type;
	ASN1_UTF8STRING *name;
	ASN1_INTEGER *certListType;
	SES_CertList *certList;
	ASN1_GENERALIZEDTIME *createDate;
	ASN1_GENERALIZEDTIME *validStart;
	ASN1_GENERALIZEDTIME *validEnd;
} V4_SES_ESPropertyInfo;
DECLARE_ASN1_FUNCTIONS(V4_SES_ESPropertyInfo)

typedef struct ses_espictrueinfo_st {
	ASN1_IA5STRING *type;
	ASN1_OCTET_STRING *data;
	ASN1_INTEGER *width;
	ASN1_INTEGER *height;
} SES_ESPictrueInfo;
DECLARE_ASN1_FUNCTIONS(SES_ESPictrueInfo)

typedef struct v4_ses_espictrueinfo_st {
	ASN1_IA5STRING *type;
	ASN1_OCTET_STRING *data;
	ASN1_INTEGER *width;
	ASN1_INTEGER *height;
} V4_SES_ESPictrueInfo;
DECLARE_ASN1_FUNCTIONS(V4_SES_ESPictrueInfo)

typedef struct ses_header_st {
	ASN1_IA5STRING *ID;
	ASN1_INTEGER *version;
	ASN1_IA5STRING *Vid;
} SES_Header;
DECLARE_ASN1_FUNCTIONS(SES_Header)

typedef struct v4_ses_header_st {
	ASN1_IA5STRING *ID;
	ASN1_INTEGER *version;
	ASN1_IA5STRING *Vid;
} V4_SES_Header;
DECLARE_ASN1_FUNCTIONS(V4_SES_Header)

typedef struct extdata_st {
	ASN1_OBJECT *extnID;
	ASN1_BOOLEAN *critical;
	ASN1_OCTET_STRING *extnValue;
} ExtData;
DECLARE_ASN1_FUNCTIONS(ExtData)

typedef struct v4_extdata_st {
	ASN1_OBJECT *extnID;
	ASN1_BOOLEAN *critical;
	ASN1_OCTET_STRING *extnValue;
} V4ExtData;
DECLARE_ASN1_FUNCTIONS(V4ExtData)

typedef struct ses_sealinfo_st {
	SES_Header *header;
	ASN1_IA5STRING *esID;
	SES_ESPropertyInfo *property;
	SES_ESPictrueInfo *picture;
	STACK_OF(ExtData) *extDatas;
} SES_SealInfo;
DECLARE_ASN1_FUNCTIONS(SES_SealInfo)

typedef struct v4_ses_sealinfo_st {
	V4_SES_Header *header;
	ASN1_IA5STRING *esID;
	V4_SES_ESPropertyInfo *property;
	V4_SES_ESPictrueInfo *picture;
	STACK_OF(V4ExtData) *extDatas;
} V4_SES_SealInfo;
DECLARE_ASN1_FUNCTIONS(V4_SES_SealInfo)

typedef struct ses_signinfo_st
{
	ASN1_OCTET_STRING *cert;
	ASN1_OBJECT *signatureAlorithm;
	ASN1_BIT_STRING *signData;
} SES_SignInfo;
DECLARE_ASN1_FUNCTIONS(SES_SignInfo)

typedef struct seseal_st
{
	SES_SealInfo *esealInfo;
	SES_SignInfo *signInfo;
} SESeal;
DECLARE_ASN1_FUNCTIONS(SESeal)

typedef struct v4_seseal_st
{
	V4_SES_SealInfo *esealInfo;
	ASN1_OCTET_STRING *cert;
	ASN1_OBJECT *signAlgID;
	ASN1_BIT_STRING *signedValue;
} V4SESeal;
DECLARE_ASN1_FUNCTIONS(V4SESeal)

typedef struct tbssign_st
{
	ASN1_INTEGER *version;
	SESeal *eseal;
	ASN1_BIT_STRING *timeinfo;
	ASN1_BIT_STRING *dataHash;
	ASN1_IA5STRING *propertyInfo;
	ASN1_OCTET_STRING *cert;
	ASN1_OBJECT *signatureAlgorithm;
}TBS_Sign;
DECLARE_ASN1_FUNCTIONS(TBS_Sign)

typedef struct v4_tbssign_st
{
	ASN1_INTEGER *version;
	V4SESeal *eseal;
	ASN1_GENERALIZEDTIME *timeinfo;
	ASN1_BIT_STRING *dataHash;
	ASN1_IA5STRING *propertyInfo;
	STACK_OF(V4ExtData) *extDatas;
} V4_TBS_Sign;
DECLARE_ASN1_FUNCTIONS(V4_TBS_Sign)

typedef struct sessignature_st
{
	TBS_Sign *toSign;
	ASN1_BIT_STRING *signature;
} SES_Signature;
DECLARE_ASN1_FUNCTIONS(SES_Signature)

typedef struct v4_sessignature_st
{
	V4_TBS_Sign *toSign;
	ASN1_OCTET_STRING *cert;
	ASN1_OBJECT *signatureAlgID;
	ASN1_BIT_STRING *signature;
	ASN1_BIT_STRING *timeStamp;
} V4_SES_Signature;
DECLARE_ASN1_FUNCTIONS(V4_SES_Signature)