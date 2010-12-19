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
#ifndef _BUFFER_H
#define _BUFFER_H

#include <stdlib.h>
#include "string_utf8.h"

#ifdef __cplusplus
	extern "C" {
#endif

#define BUFFER_INIT_CAPACITY 64

typedef struct
{
	string data;
	size_t size;
	size_t capacity;
} _buffer;

typedef _buffer* buffer;

//methods

buffer buffer_new();
buffer buffer_new_capacity(size_t capacity);
void buffer_doublesize(buffer abuffer);
void buffer_appendstring(buffer abuffer,string astring);
void buffer_appendchar(buffer abuffer,char c);
string buffer_tostring(buffer abuffer);

#ifdef __cplusplus
	}
#endif

#endif // _BUFFER_H

