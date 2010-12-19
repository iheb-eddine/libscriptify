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
 * In scripting context, an object represents any pointer type; as opposed to a primitive
 * value such as int, long, double. We handle strings separately, even though strings are also
 * pointers.
 *
 */
#include "object.h"
#include <gc/gc.h>

object object_new(size_t size)
{
	return GC_malloc(size);
}

object object_resize(object obj, size_t size)
{
	return GC_realloc(obj,size);
}

