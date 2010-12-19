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
 * A buffer is a dynamically growing memory area, that grows by adding strings or individual characters (bytes).
 *
 */
#include <string.h>
#include <stdlib.h>
#include "object.h"
#include "buffer.h"
#include "string_utf8.h"

/**
* Creates a new buffer with default capacity (BUFFER_INIT_CAPACITY).
* The buffer grows automatically when adding new strings or characters.
*
* @return A pointer to the new buffer.
*/
buffer buffer_new()
{
	return buffer_new_capacity(BUFFER_INIT_CAPACITY);
}

/**
* Creates a new buffer with given initial capacity.
* The buffer grows automatically when adding new strings or characters.
*
* @param capacity Initial buffer capacity.
* @return A pointer to the new buffer.
*/
buffer buffer_new_capacity(size_t capacity)
{
	buffer abuffer = (buffer)object_new(sizeof(_buffer));  
	abuffer->data=(string)object_new(BUFFER_INIT_CAPACITY);
	abuffer->size=0;
	abuffer->capacity=capacity;
	return abuffer;
}

/**
* Doubles the size of the buffer.
*
* @param abuffer the buffer to double the size for.
*/
void buffer_doublesize(buffer abuffer)
{
	size_t newcapacity=abuffer->capacity*2;
	abuffer->data = object_resize((object)abuffer->data, newcapacity);
	abuffer->capacity=newcapacity;
}

/**
* Ensures that the buffer has the capacity required.
* Keeps doubling the buffer capacity until it is equal or larger than the requirement.
*
* @param abuffer the buffer to ensure the capacity for.
* @param capacity_required the capacity requirement.
*/
void buffer_ensure_capacity(buffer abuffer, size_t capacity_required)
{
	while(capacity_required>abuffer->capacity) buffer_doublesize(abuffer);
}

/**
* Appends a string to a buffer.
* The buffer grows automatically, if needed, for adding the string supplied.
*
* @param abuffer the buffer to append the string to.
* @param astring the string to append.
*/
void buffer_appendstring(buffer abuffer,string astring)
{
	size_t len=strlen(astring);
	buffer_ensure_capacity(abuffer,abuffer->size+len);
	strncpy(abuffer->data+abuffer->size,astring,len);
	abuffer->size+=len;
}

/**
* Appends a character (byte) to a buffer.
* The buffer grows automatically, if needed, for adding the character (byte) supplied.
*
* @param abuffer the buffer to append the character (byte) to.
* @param c the character (byte) to append.
*/
void buffer_appendchar(buffer abuffer,char c)
{
	buffer_ensure_capacity(abuffer,abuffer->size+sizeof(char));
	abuffer->data[abuffer->size]=c;
	abuffer->size++;
}

/**
* Returns a null-terminated string containing the content of the buffer.
* Warning: make sure never to have added null characters in the middle of the buffer,
* or the resulting string will be truncated at the first such null character.
*
* @param abuffer the buffer to return the string from.
* @return A pointer to new string containing the content of the buffer.
*/
string buffer_tostring(buffer abuffer)
{
	string astring=string_new(abuffer->size);
	strncpy(astring, abuffer->data, abuffer->size);
	astring[abuffer->size]=0;
	return astring;
}

