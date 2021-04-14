// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件: 
#include <afxwin.h>

#include <WinCrypt.h>
#pragma comment(lib, "Crypt32.lib")
/*ssl*/
#include <openssl/asn1t.h>
#include <openssl/x509.h>
#include <openssl/pkcs7.h>
#include <openssl/err.h>
#include <openssl/rc4.h>
#include <openssl/sm2.h>
#pragma comment(lib, "libcrypto.lib")
/*ssl*/

// TODO:  在此处引用程序需要的其他头文件
