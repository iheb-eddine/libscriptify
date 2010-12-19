/**
 *
 * libscriptify
 *
 * @author  Erik Poupaert <erik@sankuru.biz>
 * @author  Ibrahim Abd Elkader <freelancer.c@gmail.com>
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU LGPL as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * LGPL for more details at: http://www.gnu.org/licenses/lgpl.html
 *
 * @section DESCRIPTION
 *
 * A string is a series of UTF-8 characters followed by a '\0' byte.
 *
 */
#ifndef _STRING_UTF8_H
#define _STRING_UTF8_H

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
	extern "C" {
#endif

typedef char* string;

#define UTF8_FAIL -1;
#define CHAR_LF 0x0a;
#define CHAR_CR 0x0d;

/* new string*/
string string_new(size_t size);
string string_new_copy(string str);

/* utf8 functions*/
int string_utf8_getbytetype(char c);
size_t string_length_utf8(string str);
string string_substr_utf8(string str,size_t start,size_t size);
string string_tolower_utf8(string str);
string string_toupper_utf8(string str);
string string_charat_utf8(string str,size_t index);
bool string_valid_utf8(string str);

/* string functions*/
bool string_equal(string str1, string str2);
size_t string_length(string str);
string string_substr(string str,size_t start,size_t size);
string string_tolower(string str);
string string_toupper(string str);
string string_charat(string str, size_t index);
string string_trim(string str, bool left, bool right);
string string_format(string format, ...);

#ifdef __cplusplus
	}
#endif

#endif // _STRING_UTF8_H

