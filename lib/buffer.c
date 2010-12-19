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

buffer buffer_new()
{
	return buffer_new_capacity(BUFFER_INIT_CAPACITY);
}

buffer buffer_new_capacity(size_t capacity)
{
	buffer abuffer = (buffer)object_new(sizeof(_buffer));  
	abuffer->data=(string)object_new(BUFFER_INIT_CAPACITY);
	abuffer->size=0;
	abuffer->capacity=capacity;
	return abuffer;
}

void buffer_doublesize(buffer abuffer)
{
	size_t newcapacity=abuffer->capacity*2;
	abuffer->data = object_resize((object)abuffer->data, newcapacity);
	abuffer->capacity=newcapacity;
}

void buffer_ensure_capacity(buffer abuffer, size_t capacity_required)
{
	while(capacity_required>abuffer->capacity) buffer_doublesize(abuffer);
}

void buffer_appendstring(buffer abuffer,string astring)
{
	size_t len=strlen(astring);
	buffer_ensure_capacity(abuffer,abuffer->size+len);
	strncpy(abuffer->data+abuffer->size,astring,len);
	abuffer->size+=len;
}

void buffer_appendchar(buffer abuffer,char c)
{
	buffer_ensure_capacity(abuffer,abuffer->size+sizeof(char));
	abuffer->data[abuffer->size]=c;
	abuffer->size++;
}

string buffer_tostring(buffer abuffer)
{
	string astring=string_new(abuffer->size);
	strncpy(astring, abuffer->data, abuffer->size);
	astring[abuffer->size]=0;
	return astring;
}

