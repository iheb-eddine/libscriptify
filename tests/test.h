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
 * This header allows of administrative simplification and paperwork reduction in the test suites.
 *
 */
#ifndef _TEST_H
#define _TEST_H

#ifdef __cplusplus
	extern "C" {
#endif

#include <check.h>
#include <stdlib.h>
#include "string_utf8.h"

typedef TCase * test_case;
typedef Suite * suite;
typedef SRunner * suite_runner;

test_case test_case_setup(string name);

#define TEST_HEADER \
	suite suite_setup(string name) \
	{ \
	suite s=suite_create(name); \
	suite_add_tcase (s, test_case_setup(name)); \
	return s; \
	} \
 \
	test_case test_case_setup(string name) \
	{ \
		test_case tc=tcase_create(name);


#define TEST_FOOTER(x) \
		return tc; \
	 } \
 \
int suite_run() \
{ \
	suite_runner sr = srunner_create(suite_setup(x));  \
	srunner_run_all (sr, CK_NORMAL);  \
	int number_failed = srunner_ntests_failed (sr);  \
	srunner_free (sr);  \
	return number_failed; \
} \
 \
int main ()  \
{  \
	int number_failed=suite_run();  \
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;  \
}

#ifdef __cplusplus
	}
#endif

#endif // _TEST_H

