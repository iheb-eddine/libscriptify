/**
 *
 * libscriptify
 *
 * @author  Erik Poupaert <erik@sankuru.biz>
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
 * In scripting context, an object represents any dynamically allocated memory area; as opposed to 
 * primitive values such as int, long, double, that are allocated on the stack. 
 * We handle strings separately, even though strings are also dynamically allocated pointers.
 *
 */
#ifndef _OBJECT_H
#define _OBJECT_H

#include <stddef.h>

#ifdef __cplusplus
	extern "C" {
#endif

typedef void* object;

object object_new(size_t size);
object object_resize(object obj, size_t size);

#ifdef __cplusplus
	}
#endif

#endif // _OBJECT_H

