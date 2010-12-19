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
#include "string_utf8.h"
#include "object.h"
#include "buffer.h"
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <wctype.h>

//PRIVATE

enum _UTF8_CHARTYPE
{
	  UTF8_B1 //start of 1-char sequence
	, UTF8_TAIL  //follow-on char in multichar sequence
	, UTF8_B2 //start of 2-char sequence
	, UTF8_B3 //start of 3-char sequence
	, UTF8_B4 //start of 4-char sequence
	, UTF8_ERROR //character illegal in utf8
};

typedef enum _UTF8_CHARTYPE UTF8_CHARTYPE;

struct _utf8_range
{
	int code;
	int start;
	int end;
};

typedef struct _utf8_range* utf8_range;

struct _utf8_range utf8_ranges[6]=
{ 
	  { UTF8_B1, 0x00, 0x7f }
	, { UTF8_TAIL, 0x80, 0xbf }
	, { UTF8_B2, 0xc2, 0xdf }
	, { UTF8_B3, 0xe0, 0xef }
	, { UTF8_B3, 0xf0, 0xf4 }
	, { UTF8_ERROR, 0, 0 }

};

//PRIVATE

string string_new(size_t size)
{
	return (string)object_new((size+1)*sizeof(char));
}

string string_new_copy(string str)    
{	
	//string_new already allocates space for the terminating \0 char.
	string s = string_new(strlen(str));
	//strcpy copies including the terminating \0 char.
	strcpy(s, str);         
	return s;
}

int string_utf8_getbytetype(char c)
{
	int i=0;
	struct _utf8_range range=utf8_ranges[i];
	while (range.code != UTF8_ERROR) {
		if (c>=range.start && c<=range.end) {
            break;
        }
		range=utf8_ranges[++i];
	}
	return range.code;
}

bool utf8_ensure_correct_tail(string str,size_t index,int numbytes)
{
	int i;
	for(i=0; i<numbytes; i++)
	{
		if(string_utf8_getbytetype(str[index+i])!=UTF8_TAIL) 
			return false;
	}
	return true;
}

size_t utf8_movenext(string str, size_t index)
{
	char byte=str[index];
	int bytetype=string_utf8_getbytetype(byte);
	switch(bytetype)
	{
		case UTF8_B1: return index+1;
		case UTF8_B2: if(utf8_ensure_correct_tail(str,index+1,1)) 
					return index+2; 
				else return UTF8_FAIL;
		case UTF8_B3: if(utf8_ensure_correct_tail(str,index+1,2)) 
					return index+3; 
				else return UTF8_FAIL;
		case UTF8_B4: if(utf8_ensure_correct_tail(str,index+1,3)) 
					return index+4; 
				else return UTF8_FAIL;
		default: return UTF8_FAIL;
	}
}

size_t string_length_utf8(string str)
{
	size_t byte_length=(size_t)strlen(str);
	size_t utf8_length=0;
	size_t index=0;
	while(index<byte_length)
	{
		index=utf8_movenext(str, index);
		utf8_length++;
	}
	return utf8_length;
}

string string_substr_utf8(string str, size_t start, size_t size)
{
    string s = NULL;
	size_t utf8_index=0;
	size_t start_index=0;
	size_t end_index=0;
	size_t end = start + size;
	size_t byte_length = strlen(str);
    
	while(start < byte_length && utf8_index < start)
	{
		start_index=utf8_movenext(str, start_index);
		utf8_index++;
	}

    end_index = start_index;

    while(end <= byte_length && utf8_index <= end)
	{
		end_index = utf8_movenext(str, end_index);
		utf8_index++;
	}

    byte_length = end_index - start_index;
    s = string_new(byte_length);
    strncpy(s, str+start_index, byte_length);
    s[byte_length] = 0;
    return s;
}

string string_tolower_utf8(string str)    
{
    string s;
    size_t byte_length;
    size_t char_length;
    wchar_t *wstr, *wptr;
 
    byte_length = strlen(str);
    char_length = string_length_utf8(str) + 1;
    s = string_new(byte_length);
    wstr = wptr = malloc(sizeof(wchar_t) * char_length);
    
    utf8_to_wchar(str, byte_length, wstr, char_length, 0);
    wstr[char_length]=0;

    while ((*wptr++ = towlower(*wptr))); 

    wchar_to_utf8(wstr, char_length,  s, byte_length, 0);
    free(wstr);

    s[byte_length] = 0;
    return s;
}

string string_toupper_utf8(string str)
{
    string s;
    size_t byte_length;
    size_t char_length;
    wchar_t *wstr, *wptr;
 
    byte_length = strlen(str);
    char_length = string_length_utf8(str) + 1;
    s = string_new(byte_length);
    wstr = wptr = malloc(sizeof(wchar_t) * char_length);
    
    utf8_to_wchar(str, byte_length, wstr, char_length, 0);
    wstr[char_length]=0;

    while ((*wptr++ = towupper(*wptr))); 

    wchar_to_utf8(wstr, char_length,  s, byte_length, 0);
    free(wstr);

    s[byte_length] = 0;
    return s;
}

string string_charat_utf8(string str, size_t index)
{
    string s = NULL;
	size_t utf8_index=0;
	size_t curr_index=0;
	size_t byte_length = strlen(str);
    
	while(index<byte_length && utf8_index < index)
	{
		curr_index=utf8_movenext(str, curr_index);
		utf8_index++;
	}

	if(utf8_index == index)
	{
		int curr_index_start=curr_index;
		int curr_index_end=utf8_movenext(str, curr_index_start);
        byte_length = curr_index_end - curr_index_start;
        s = string_new(byte_length);
        memcpy(s, str +curr_index_start, byte_length);
        s[byte_length] = 0;
	}
	return s;
}

bool string_valid_utf8(string str)
{
    while (string_utf8_getbytetype(*str++) != UTF8_ERROR && *str);

    return (*str == 0);
}

bool string_equal(string str1, string str2)
{
	return (strcmp(str1,str2)==0);
}

size_t string_length(string str)
{
    return strlen(str);
}

string string_substr(string str, size_t start, size_t size)
{
    string s = string_new(size);
    strncpy(s, str+start, size);
    s[size] = 0; //string_new adds 1 byte
    return s;
}

string string_tolower(string str)
{
    string s, p;
    s = p = string_new(string_length(str));
    while ((*p++ = tolower(*str++)));
    return s;
}

string string_toupper(string str)
{
    string s, p;
    s = p = string_new(string_length(str));
    while ((*p++ = toupper(*str++)));
    return s;
}

string string_charat(string str, size_t index) 
{
    string s = string_new(sizeof(char));
    *s     = str[index];
    *(s+1) = 0;
    return s;
}


string string_trim(string str, bool left, bool right)
{
    size_t len;
    string s;
    string start = str;
    string end   = str + strlen(str)-1;
    
    if (left) {
       while ((*start == ' ') && (start++ != end));
    }

    if (right) {
       while ((*end == ' ') && (start != end--));
    }
    len = (end - start)+1;
    s = string_new(len);
    strncpy(s, start, len);
    s[len] = 0;
    return s;
}

string string_format(string format, ...)
{
	va_list args;
    int allocated;
	buffer abuffer;
   
    abuffer = buffer_new();	
	while(1)
	{
		va_start(args, format);
		allocated = vsnprintf(abuffer->data, abuffer->capacity, format, args);
		va_end(args);
		if (allocated > -1 && allocated < abuffer->capacity) {
            break;
        }
		buffer_doublesize(abuffer);
	}

    abuffer->size = allocated;
    return buffer_tostring(abuffer);
}
