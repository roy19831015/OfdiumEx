/**************************************************
* File name: base64.h
* Author: HAN Wei
* Author's blog: http://blog.csdn.net/henter/
* Date: Oct 31th, 2013
* Description: declare base64 encode and decode functions
			   When encoding is needed, EncodeBase64() function is invoked.
			   The user can decide the count of characters in a full line (the max
			   count is set to 76 in RFC 2045).
			   The user can decide to insert 0x0A (line feed) or 0x0D (carriage return)
			   0x0A into the end of each full line, and can also choose insert nothing.
			   When decoding is needed, DecodeBase64() function is invoked. 0x0A or
			   0x0D 0x0A in base64 codes will be ignored.
* Note: EncodeBase64() and DecodeBase64() are derived from EVP_EncodeBlock() and
		EVP_DecodeBlock() in encode.c of OpenSSL 1.0.1c.
  OpenSSL web site: http://www.openssl.org/
**************************************************/

#ifndef BASE64_ENCODE_AND_DECODE_H
#define BASE64_ENCODE_AND_DECODE_H

#ifdef  __cplusplus
extern "C" {
#endif

	/**************************************************
	*函数名称：CalcBase64EncodeBufLen
	*功能:  在使用Base64编码函数EncodeBase64()之前，该函数可以为函数调用者预先算出
			存放编码结果的缓冲区大小
	*参数:
	raw_data_len[in]    等待编码的原始数据长度，以字节为单位
	line_feed_flag[in]  为0表示编码时不插入换行符，
						为1表示编码时在换行处插入Unix或Linux下的换行0x0A
						为2表示编码时在换行处插入Windows下的换行0x0D,0x0A
	characters_count_per_base64_codes_line[in]  一行中能容纳的base64编码字符个数上限，
												line_feed_flag值为0时该参数被忽略，
												可设为任意值，因为此时base64编码不分行。
												line_feed_flag值为1或2时，该参数才有意义。
												根据RFC 2045的规定，该值最大可以设为76。
												要得到与OpenSSL中依次调用EVP_EncodeInit(),
												EVP_EncodeUpdate(),EVP_EncodeFinal()函数后
												生成的编码结果相同的编码时，将此值设为64，
												因为EVP_EncodeUpdate()中将一行能容纳的
												base64编码字符个数上限设为64
	*返回值: 所需存放base64编码结果的缓冲区大小，以字节为单位。当返回值为 -1 时表示出错
	**************************************************/
	unsigned int CalcBase64EncodeBufLen(unsigned int raw_data_len,
		int line_feed_flag,
		unsigned int characters_count_per_base64_codes_line);

	/**************************************************
	*函数名称：CalcBase64DecodeBufLen
	*功能:     在使用Base64解码函数DecodeBase64()之前，该函数可以为函数调用者预先算出
			   存放解码结果的缓冲区大小
	*参数:
	base64_codes[in]      指向待解码的base64字符串首地址的指针
	base64_codes_Len[in]  等待解码的base64字符串长度，以字节为单位。当base64字符串以'\0'结尾时，该输入参数表示的字符串长度不包含'\0'所占的1个字节，仅为base64字符的总数（如果其中有回车或换行，在统计base64字符总数时要把回车及换行也计入）
	*返回值: 存放base64解码结果的缓冲区大小，以字节为单位
	**************************************************/
	unsigned int CalcBase64DecodeBufLen(unsigned char* base64_codes,
		unsigned int base64_codes_Len);

	/**************************************************
	*函数名称：EncodeBase64
	*功能:  base64编码函数
	*参数:
	t[out]  指向输出缓冲区首地址的指针
	f[in]   指向输入缓冲区首地址的指针
	n[in]   待编码的字符串长度，如果该字符串以'\0'结尾，统计该长度时不计入'\0'
	line_feed_flag[in]  是否插入换行的标志，为0表示编码时不插入换行，
						为1表示在一行编码字符末尾插入Unix或Linux下的换行0x0A，
						为2表示在一行编码字符末尾插入Windows下的换行0x0D 0x0A
	characters_count_per_base64_codes_line[in]  一行中能容纳的base64编码字符个数上限，
												line_feed_flag值为0时该参数可设为任意值，
												因为此时base64编码不分行。
												line_feed_flag值为1或2时，该参数才有意义，
												根据RFC 2045的规定，该值最大可以设为76。
												要得到与OpenSSL中依次调用EVP_EncodeInit(),EVP_EncodeUpdate(),EVP_EncodeFinal()
	 生成的编码结果相同的编码时，应将此值设为64
	*返回值: base64编码结果字符串长度，以字节为单位。。当返回值为 -1 时表示出错。
			如果在编码过程中插入了换行，那么像回车0x0D, 换行0x0A这样的符号的个数也计入长度，
			该函数会在编码结果字符串末尾加上一个'\0'，但在计算返回值时，即统计编码字符串
			长度时不计入'\0'
	**************************************************/
	int EncodeBase64(unsigned char* t,
		unsigned char* f,
		int n,
		int line_feed_flag,
		unsigned int characters_count_per_base64_codes_line);

	/**************************************************
	*函数名称：DecodeBase64
	*功能:  base64解码函数
	*参数:
	t  指向输出缓冲区首地址的指针
	f  指向输入缓冲区首地址的指针
	n  待解码的base64字符串长度，统计该长度时如果字符串以'\0'结尾，不计入'\0'，但是
	   回车0x0D, 换行0x0A符号的个数也要计入长度，它们不会影响解码
	注意：
	  对于编码时在一行末尾插入的换行可能有两种：Windows下的换行符0x0D 0x0A，或Unix
	  （及Linux）下的换行符0x0A。这两种换行该函数都能正确解码，因为该函数会忽略这些符号
	*返回值: 存放base64解码结果字符串长度，即未编码的字符串长度。
			 如果编码时进行过填充，填充的字符个数不被计入，用户无需再手动去除填充字符个数
	**************************************************/
	int DecodeBase64(unsigned char* t, unsigned char* f, int n);

#ifdef  __cplusplus
}
#endif

#endif /* end of BASE64_ENCODE_AND_DECODE_H */
