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
#ifndef _ANY_H
#define _ANY_H

#include "object.h"
#include "string_utf8.h"

#ifdef __cplusplus
	extern "C" {
#endif

enum _vartype
{
	   TYPE_INT
	,  TYPE_LONG
	, TYPE_DOUBLE
	, TYPE_STRING
	, TYPE_OBJECT
};

typedef enum _vartype vartype;

typedef struct
{
	vartype type;
	union
	{
		int i;
		long lng;
		double dbl;
		string str;
		object obj;
	};
} _any;

typedef _any* any;

typedef any* manyany;


any any_new_int(int i);
any any_new_long(long lng);
any any_new_double(double dbl);
any any_new_string(string str);
any any_new_object(object obj);

#ifdef __cplusplus
	}
#endif

#endif // _ANY_H

