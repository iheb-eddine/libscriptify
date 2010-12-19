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
 * Unit test for the 'buffer' data type.
 *
 */
#include "test.h"
#include "buffer.h"
#include "string_utf8.h"

/*	----------------------
	TEST 1
	---------------------- 
*/

START_TEST (test_buffer_append)
{
    int i;
    buffer buf = buffer_new();

    int len = BUFFER_INIT_CAPACITY *2+ 5;
    string str = string_new(len);
    memset(str, 'c', len);
    strncpy(str, "Hello", 5);
    str[len] =0;

    buffer_appendstring(buf, "Hello");
    for (i=0; i<BUFFER_INIT_CAPACITY*2; i++) {
        buffer_appendchar(buf, 'c');
    }

    string bufstr = buffer_tostring(buf);
	fail_unless (strcmp(bufstr, str) == 0, "appending strings");
}
END_TEST

/*	----------------------
	REGISTER TESTS AND RUN
	---------------------- 
*/

TEST_HEADER
	tcase_add_test (tc, test_buffer_append);
TEST_FOOTER("BUFFER")

