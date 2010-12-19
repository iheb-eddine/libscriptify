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
 * Unit test for the 'any' data type.
 *
 */
#include "test.h"
#include "any.h"
#include "string_utf8.h"

START_TEST (test_any_int)
{
	int i=5;
	any anyint=any_new_int(i);
	fail_unless (i==anyint->i, "allocating integer");
}
END_TEST

START_TEST (test_any_long)
{
	long lng=5;
	any anylong=any_new_long(lng);
	fail_unless (lng==anylong->lng, "allocating long");
}
END_TEST

START_TEST (test_any_string)
{
	string str="hello";
	any anystring=any_new_string(str);
	fail_unless (string_equal(str,anystring->str), "allocating string");
}
END_TEST

START_TEST (test_any_object)
{
	//find some better test ...
	string obj="hello";
	any anyobject=any_new_object(obj);
	fail_unless (string_equal(obj,anyobject->obj), "allocating object");
}
END_TEST

TEST_HEADER
	tcase_add_test (tc, test_any_int);
	tcase_add_test (tc, test_any_long);
	tcase_add_test (tc, test_any_string);
	tcase_add_test (tc, test_any_object);
TEST_FOOTER("ANY")

