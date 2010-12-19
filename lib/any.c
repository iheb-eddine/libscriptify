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
 * The any data type represents any data type in scripting context: int, long, double, string, or object (void *).
 *
 */
#include "any.h"

any any_new()
{
	any anyone=(any)object_new(sizeof(_any));
	return anyone;
}

any any_new_int(int i)
{
	any anyone=any_new();
	anyone->type=TYPE_INT;
	anyone->i=i;
	return anyone;
}

any any_new_long(long lng)
{
	any anyone=any_new();
	anyone->type=TYPE_LONG;
	anyone->lng=lng;
	return anyone;
}

any any_new_double(double dbl)
{
	any anyone=any_new();
	anyone->type=TYPE_DOUBLE;
	anyone->dbl=dbl;
	return anyone;
}

any any_new_string(string str)
{
	any anyone=any_new();
	anyone->type=TYPE_STRING;
	anyone->str=str;
	return anyone;
}

any any_new_object(object obj)
{
	any anyone=any_new();
	anyone->type=TYPE_OBJECT;
	anyone->obj=obj;
	return anyone;
}

