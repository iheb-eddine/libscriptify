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
 * Unit test for the 'string' data type.
 *
 */
#include "test.h"
#include "string_utf8.h"

START_TEST (test_string_new)
{
    string s = string_new_copy("Hello World");
	fail_unless (string_equal(s, "Hello World") == 1, "string_new failed");
}
END_TEST

START_TEST (test_string_length)
{
    string s = string_new_copy("Hello World");
	fail_unless (string_length(s) == strlen("Hello World"), "string_length failed");
}
END_TEST

START_TEST (test_string_substr)
{
    string s   = string_substr(string_new_copy("Hello World"), 6, 5);
	fail_unless (string_equal(s, "World"), "string_substr failed");
}
END_TEST

START_TEST (test_string_tolower)
{
    string s   = string_tolower(string_new_copy("Hello World"));
	fail_unless (string_equal(s, "hello world"), "string_tolower failed");
}
END_TEST

START_TEST (test_string_toupper)
{
    string s   = string_toupper(string_new_copy("Hello World"));
	fail_unless (string_equal(s, "HELLO WORLD"), "string_toupper failed");
}
END_TEST

START_TEST (test_string_charat)
{
	//XXX
}
END_TEST

START_TEST (test_string_trim)
{
    string s   = string_trim(string_new_copy("    Hello World     "), 1, 1);
	fail_unless (string_equal(s, "Hello World"), "string_trim failed");
}
END_TEST

START_TEST (test_string_format)
{
    string s = string_format("%d, %.2f, %s", 5, 3.14f, "Hello World");
	fail_unless (string_equal(s, "5, 3.14, Hello World"), "string_format failed");
}
END_TEST

START_TEST (test_string_length_utf8)
{
    string s = string_new_copy("Hello World");
	fail_unless (string_length_utf8(s) == strlen("Hello World"), "string_length_utf8 failed");
}
END_TEST

START_TEST (test_string_substr_utf8)
{
    string s   = string_substr_utf8(string_new_copy("Hello World"), 6, 5);
	fail_unless (string_equal(s, "World"), "string_substr_utf8 failed");
}
END_TEST

START_TEST (test_string_tolower_utf8)
{
    string s   = string_tolower_utf8(string_new_copy("Hello World"));
	fail_unless (string_equal(s, "hello world"), "string_tolower_utf8 failed");
}
END_TEST

START_TEST (test_string_toupper_utf8)
{
    string s   = string_toupper_utf8(string_new_copy("Hello World"));
	fail_unless (string_equal(s, "HELLO WORLD"), "string_toupper_utf8 failed");
}
END_TEST

START_TEST (test_string_charat_utf8)
{
    string s   = string_charat_utf8(string_new_copy("Hello World"), 4);
	fail_unless (string_equal(s, "o"), "string_charat_utf8 failed");
}
END_TEST

START_TEST (test_string_valid_utf8);
{
    bool valid  = string_valid_utf8(string_new_copy("Hello World"));
	fail_unless (valid, "string_toupper_utf8 failed");
}
END_TEST

TEST_HEADER
	tcase_add_test (tc, test_string_new);
	tcase_add_test (tc, test_string_length);
	tcase_add_test (tc, test_string_substr);
	tcase_add_test (tc, test_string_tolower);
	tcase_add_test (tc, test_string_toupper);
	tcase_add_test (tc, test_string_charat);
	tcase_add_test (tc, test_string_trim);
	tcase_add_test (tc, test_string_format);
	tcase_add_test (tc, test_string_length_utf8);
	tcase_add_test (tc, test_string_substr_utf8);
	tcase_add_test (tc, test_string_tolower_utf8);
	tcase_add_test (tc, test_string_toupper_utf8);
	tcase_add_test (tc, test_string_valid_utf8);
	tcase_add_test (tc, test_string_charat_utf8);
TEST_FOOTER("STRING_UTF8")

