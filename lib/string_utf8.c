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
	, { UTF8_B4, 0xf0, 0xf4 }
	, { UTF8_ERROR, 0, 0 }

};

//PRIVATE

/**
* Creates a new string with size 'size'.
* The function will increase the size by one to add space for the terminating zero character.
* 
* @param size Size of the string, exclusive of terminating zero character.
* @return A pointer to the new string.
*/
string string_new(size_t size)
{
	return (string)object_new((size+1)*sizeof(char));
}

/**
* Creates a new string copied from an existing one.
* 
* @param str the original string to copy.
* @return A pointer to the new string.
*/
string string_new_copy(string str)    
{	
	//string_new already allocates space for the terminating \0 char.
	string s = string_new(strlen(str));
	//strcpy copies including the terminating \0 char.
	strcpy(s, str);         
	return s;
}

/**
* Computes the byte type for byte.
*	  UTF8_B1  ==> start of 1-char sequence
*	  UTF8_B2  ==> start of 2-char sequence
*	  UTF8_B3  ==> start of 3-char sequence
*	  UTF8_B4  ==> start of 4-char sequence
*
* UTFB_B2, UTFB_B3, UTFB_B4 are followed by UTF8_TAIL bytes  
*	  UTF8_TAIL ==> follow-on char in multichar sequence
*
*         Not all byte values are allowed in UTF-8
	  UTF8_ERROR ==> character illegal in utf8
* 
* 
* @param c the byte for which we want to know the byte type.
* @return the byte type.
*/

UTF8_CHARTYPE string_utf8_getbytetype(char c)
{
	int i=0;
	struct _utf8_range range=utf8_ranges[i];
	//loop through the allowable ranges of bytes in the UTF-8 sequence
	while (range.code != UTF8_ERROR) {
		if (c>=range.start && c<=range.end) 
		{
		   //the byte falls in a valid range: stop searching
 	           break;
 	       	}
		range=utf8_ranges[++i];
	}
	//the last range is UTF8_ERROR. If the while loop never stopped searching, it is UTF8_EROR
	//that will be returned.
	return range.code;
}

/**
* Check if an UTF8_B2, UTF8_B3, and UTF8_B4 byte is followed by the correct number of 
* UTF8_TAIL bytes.
* 
* @param str the string in which to check.
* @param index the position from which to check.
* @param numtailbytes the number of tail bytes that must be found.
* @return true, if the required number of tail bytes have been found; false, otherwise.
*/
bool utf8_ensure_correct_tail(string str,size_t index,int numtailbytes)
{
	int i;
	for(i=0; i<numtailbytes; i++)
	{
		if(string_utf8_getbytetype(str[index+i])!=UTF8_TAIL) 
			return false;
	}
	return true;
}

/**
* Move to the next utf-8 character
* which is 1,2,3, or 4 bytes further in the string
*
* @param str the string in which to move further.
* @param index the position from to move further.
* @return the new position in the string in which the next utf-8 character starts.
*/
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

/**
* Returns the number of utf-8 characters in a string. 
* That number of utf-8 characters is always equal or smaller than the number
* of bytes in the string.
*
* @param str the string in which to move further.
* @return the number of utf-8 characters found.
*/
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

/**
* Returns a new string starting in character position 'start', 
* and with a number of utf-8 character 'size'. 
* The number of bytes is always equal or larger than the number of utf-8 characters.
*
*
*
* @param str the string in which to move further.
* @param start the utf-8 character position from which to start.
* @param size the number of utf-8 characters to return.
* @return A new string with the utf-8 characters found.
*/
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

/**
* Converts a utf-8 string entirely to lowercase.
*
* @param str the original utf-8 string.
* @return A new utf-8 string with all utf-8 characters in lowercase.
*/
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

/**
* Converts a utf-8 string entirely to uppercase.
*
* @param str the original utf-8 string.
* @return A new utf-8 string with all utf-8 characters in uppercase.
*/
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

/**
* Retrieves the utf-8 character at position 'index' in an utf-8 string.
*
* @param str the utf-8 string to search the character in.
* @return the utf-8 character found; or NULL if it wasn't found (out of bound).
*/
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

/**
* Checks if a utf-8 string contains invalid character sequences.
*
* @param str the utf-8 string to search invalid character sequences in.
* @return true if the utf-8 string is valid; false if not.
*/
bool string_valid_utf8(string str)
{
	//XXX Wrong. Not enough as a check.
	//It has to use movenext, to check if every utf-8 start byte
	//is followed by the correct number of tail bytes
	while (string_utf8_getbytetype(*str++) != UTF8_ERROR && *str);
	return (*str == 0);
}

/**
* Checks if two strings are equal.
* if you want to check if both strings are equal, ignoring case, 
* convert both strings to uppercase (or lowercase) before checking.
*
* @param str1 the first string.
* @param str2 the second string.
* @return true if str1 is equal to str2; false, if not.
*/
bool string_equal(string str1, string str2)
{
	return (strcmp(str1,str2)==0);
}

/**
* Computes the length of a string in bytes.
*
* @param str the string to calculate the length in bytes.
* @return the length in bytes.
*/
size_t string_length(string str)
{
	return strlen(str);
}

/**
* Computes a substring starting at byte position 'start' and 
* with maximum number of bytes 'size'.
*
* @param str the string to calculate the substring from.
* @param start the byte at which to start copying.
* @param the maximum number of bytes to copy.
* @return a new string containing the substring.
*/
string string_substr(string str, size_t start, size_t size)
{
	string s = string_new(size);
	strncpy(s, str+start, size);
	s[size] = 0; //string_new adds 1 byte
	return s;
}

/**
* Converts a string with one-byte characters entirely to lowercase.
*
* Warning: do not use this function for utf-8 strings, 
* because the characters in utf-8 are up to 4 bytes.
*
* @param str the original string containing one-byte characters.
* @return A new string with all one-byte characters in lowercase.
*/
string string_tolower(string str)
{
	string s, p;
	s = p = string_new(string_length(str));
	while ((*p++ = tolower(*str++)));
	return s;
}

/**
* Converts a string with one-byte characters entirely to uppercase.
*
* Warning: do not use this function for utf-8 strings, 
* because the characters in utf-8 are up to 4 bytes.
*
* @param str the original string containing one-byte characters.
* @return A new string with all one-byte characters in uppercase.
*/
string string_toupper(string str)
{
	string s, p;
	s = p = string_new(string_length(str));
	while ((*p++ = toupper(*str++)));
	return s;
}

/**
* Returns the character in byte position 'index' in the string.
*
* Warning: do not use this function for utf-8 strings, 
* because it could be returning a byte in the middle of a multi-byte character.
*
* @param str the original string containing one-byte characters.
* @param index the byte position in which to return the one-byte character.
* @return The byte in position 'index'.
*/
char string_charat(string str, size_t index) 
{
	return str[index];
}

/**
* Removes leading (left) and/or trailing whitespace from a string with single-byte characters.
*
* @param str the original string to remove the leading/trailing whitespace from.
* @param left if true, remove leading whitespace.
* @param right if true, remove trailing whitespace.
* @return A new string with one-byte characters and whitespace removed.
*/
string string_trim(string str, bool left, bool right)
{
    size_t len;
    string s;
    string start = str;
    string end   = str + strlen(str)-1;

	//XXX Wrong: Whitespace is not just the space character
	//http://www.delorie.com/djgpp/doc/libc/libc_514.html
	//use the isspace() function 

/*

 	
#include <ctype.h>
int isspace(int c);

*/

    
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

/**
* Checks if an utf-8 character is whitespace.
*
* @param utf8character the character to check.
* @return true, if the character is whitespace; false, if not.
*/
bool utf8_isspace(string utf8character)
{
	/* XXX be careful for the definition of whitespace
	http://www.cs.tut.fi/~jkorpela/chars/spaces.html
	http://en.wikipedia.org/wiki/Whitespace_character
	*/
}

/**
* Removes leading (left) and/or trailing whitespace from a utf-8 string.
*
* @param str the original string to remove the leading/trailing whitespace from.
* @param left if true, remove leading whitespace.
* @param right if true, remove trailing whitespace.
* @return A new utf-8 string with whitespace removed.
*/
string string_trim_utf8(string str, bool left, bool right)
{


/*
	run through the beginning of the string and remove as long as string_isspace_utf8() returns true
	run from the end of the string and remove as long as string_isspace_utf8() returns true

*/

}

/**
* Formats a string according the 'format'.
*
* @param format the string format.
* @param ... the variables in the format.
* @return A new string formatted according to 'format'.
*/
string string_format(string format, ...)
{
	va_list args;
	int allocated;
	buffer abuffer;

	/*
		//XXX
		create a buffer with double the capacity of the format string
	*/   
	abuffer = buffer_new();	
	while(1)
	{
		va_start(args, format);
		allocated = vsnprintf(abuffer->data, abuffer->capacity, format, args);
		va_end(args);
		if (allocated > -1 && allocated < abuffer->capacity) 
		{
	            break;
	        }
		buffer_doublesize(abuffer);
	}

    abuffer->size = allocated;
    return buffer_tostring(abuffer);
}

