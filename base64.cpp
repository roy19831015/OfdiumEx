#include "stdafx.h"
/**************************************************
* File name: base64.c
* Author: HAN Wei
* Author's blog: http://blog.csdn.net/henter/
* Date: Oct 31th, 2013
* Description: implement base64 encode and decode functions
* Note: EncodeBase64() and DecodeBase64() are derived from EVP_EncodeBlock() and
		EVP_DecodeBlock() in encode.c of OpenSSL 1.0.1c.
  OpenSSL web site: http://www.openssl.org/
**************************************************/

#include "base64.h"
#include <stdio.h>

#define conv_bin2ascii(a)	(data_bin2ascii[(a)&0x3f])
#define conv_ascii2bin(a)	(data_ascii2bin[(a)&0x7f])
#define B64_EOLN		    0xF0
#define B64_CR			    0xF1
#define B64_EOF			    0xF2
#define B64_WS			    0xE0
#define B64_ERROR           0xFF
#define B64_NOT_BASE64(a)	(((a)|0x13) == 0xF3)

static const unsigned char data_bin2ascii[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const unsigned char data_ascii2bin[128] = {
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xE0,0xF0,0xFF,0xFF,0xF1,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xE0,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0x3E,0xFF,0xF2,0xFF,0x3F,
	0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,
	0x3C,0x3D,0xFF,0xFF,0xFF,0x00,0xFF,0xFF,
	0xFF,0x00,0x01,0x02,0x03,0x04,0x05,0x06,
	0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,
	0x0F,0x10,0x11,0x12,0x13,0x14,0x15,0x16,
	0x17,0x18,0x19,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,
	0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,0x30,
	0x31,0x32,0x33,0xFF,0xFF,0xFF,0xFF,0xFF,
};

unsigned int CalcBase64EncodeBufLen(unsigned int raw_data_len,
	int line_feed_flag,
	unsigned int characters_count_per_base64_codes_line)
{
	unsigned int pure_base64_encode_len, pem_len = 0, total = 0;

	pure_base64_encode_len = (raw_data_len + 2) / 3 * 4;
	if (line_feed_flag != 0)
	{
		if (pure_base64_encode_len % characters_count_per_base64_codes_line == 0)
			pem_len = pure_base64_encode_len / characters_count_per_base64_codes_line;
		else
			pem_len = pure_base64_encode_len / characters_count_per_base64_codes_line + 1;
	}
	
	switch (line_feed_flag)
	{
	case 0:
		total = pure_base64_encode_len + 1;
		break;
	case 1:
		total = pure_base64_encode_len + pem_len + 1;
		break;
	case 2:
		total = pure_base64_encode_len + pem_len * 2 + 1;
		break;
	default:
		printf("invalid input parameter line_feed_flag=%d at %s, line %d!\n", line_feed_flag, __FILE__, __LINE__);
		return (0xFFFFFFFF);
	}
	return total;
}

unsigned int CalcBase64DecodeBufLen(unsigned char* base64_codes,
	unsigned int base64_codes_Len)

{
	unsigned int i, line_feed_count = 0, carriage_return_count = 0, buffer_len = 0;
	unsigned char* p;

	p = base64_codes;
	for (i = 0; i < base64_codes_Len; i++)
	{
		if (p[i] == 0x0D)
			carriage_return_count++;
		else
			if (p[i] == 0x0A)
				line_feed_count++;
	}
	buffer_len = (base64_codes_Len - carriage_return_count - line_feed_count) / 4 * 3;
	return buffer_len;
}

int EncodeBase64(unsigned char* t,
	unsigned char* f,
	int n,
	int line_feed_flag,
	unsigned int characters_count_per_base64_codes_line)
{
	int i, ret = 0, counter = 0;
	unsigned long l;
	int characters_count_per_raw_data_line, pure_base64_encode_len;

	characters_count_per_raw_data_line = characters_count_per_base64_codes_line / 4 * 3;
	pure_base64_encode_len = (n + 2) / 3 * 4;

	for (i = n; i > 0; i -= 3)
	{
		if (i >= 3)
		{
			l = (((unsigned long)f[0]) << 16L) | (((unsigned long)f[1]) << 8L) | f[2];
			*(t++) = conv_bin2ascii(l >> 18L);
			*(t++) = conv_bin2ascii(l >> 12L);
			*(t++) = conv_bin2ascii(l >> 6L);
			*(t++) = conv_bin2ascii(l);
		}
		else
		{
			l = ((unsigned long)f[0]) << 16L;
			if (i == 2)
				l |= ((unsigned long)f[1] << 8L);

			*(t++) = conv_bin2ascii(l >> 18L);
			*(t++) = conv_bin2ascii(l >> 12L);
			*(t++) = (i == 1) ? '=' : conv_bin2ascii(l >> 6L);
			*(t++) = '=';
		}
		f += 3;
		ret += 4;
		counter += 3;
		if (line_feed_flag)
		{
			if (counter % characters_count_per_raw_data_line == 0)
			{
				switch (line_feed_flag)
				{
				case 1:
					*t = 0x0a;
					t++;
					ret++;
					break;
				case 2:
					*t = 0x0D;
					t++;
					*t = 0x0a;
					t++;
					ret += 2;
					break;
				default:
					printf("invalid input parameter line_feed_flag=%d at %s, line %d!\n", line_feed_flag, __FILE__, __LINE__);
					return (-1);
				}
			}
		}
	}
	if (line_feed_flag)
	{
		if (pure_base64_encode_len % characters_count_per_base64_codes_line != 0)
		{
			switch (line_feed_flag)
			{
			case 1:
				*t = 0x0a;
				t++;
				ret++;
				break;
			case 2:
				*t = 0x0D;
				t++;
				*t = 0x0a;
				t++;
				ret += 2;
				break;
			default:
				printf("invalid input parameter line_feed_flag=%d at %s, line %d!\n", line_feed_flag, __FILE__, __LINE__);
				return (-1);
			}
		}
	}
	*t = '\0';
	return(ret);
}

int DecodeBase64(unsigned char* t, unsigned char* f, int n)
{
	int i, ret = 0, a, b, c, d;
	unsigned long l;
	unsigned char* p, * encode_buf = f;
	int pad_count = 0;

	/* trim white space from the start of the input buffer. */
	while ((conv_ascii2bin(*f) == B64_WS) && (n > 0))
	{
		f++;
		n--;
	}
	/* trim B64_WS, B64_EOLN, B64_CR, B64_EOF at the end of the input buffer */
	while ((n > 3) && (B64_NOT_BASE64(conv_ascii2bin(f[n - 1]))))
		n--;
	if (n < 4) return (-1);

	/* count the padding */
	p = &encode_buf[n - 1];
	for (i = 0; i < 2; i++)
	{
		if (*p == '=')
		{
			pad_count++;
			p--;
		}
	}
#ifdef _DEBUG
	printf("pad_count=%d\n", pad_count);
#endif

	/* base64 decoding */
	for (i = 0; i < n; i += 4)
	{
		a = conv_ascii2bin(*(f++));
		b = conv_ascii2bin(*(f++));
		c = conv_ascii2bin(*(f++));
		d = conv_ascii2bin(*(f++));
		if ((a & 0x80) || (b & 0x80) || (c & 0x80) || (d & 0x80))
			return(-1);
		l = ((((unsigned long)a) << 18L) | (((unsigned long)b) << 12L) | (((unsigned long)c) << 6L) | (((unsigned long)d)));
		*(t++) = (unsigned char)(l >> 16L) & 0xff;
		*(t++) = (unsigned char)(l >> 8L) & 0xff;
		*(t++) = (unsigned char)(l) & 0xff;
		ret += 3;
		/* ignore CR (0x0D) and LF (0x0A) */
		if (*f == 0x0D)
		{
			f++;
			n--;
		}
		if (*f == 0x0A)
		{
			f++;
			n--;
		}
	}
	ret -= pad_count;
	return(ret);
}