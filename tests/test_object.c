#include "test.h"
#include "object.h"
#include "string_utf8.h"

/*	----------------------
	TEST 1
	---------------------- 
*/

START_TEST (test_object_new)
{
	//find some better test ...
	string s="hello";
	object obj=object_new(sizeof(string));
	obj=(object)s;
	fail_unless (s==obj, "new object");
}
END_TEST

START_TEST (test_object_resize)
{
	//find some better test ...
	string s="hello";
	object obj=object_new(sizeof(string));
	obj=object_resize(obj,2*sizeof(string));
	obj=(object)s;
	fail_unless (s==obj, "resize object");
}
END_TEST

/*	----------------------
	REGISTER TESTS AND RUN
	---------------------- 
*/

TEST_HEADER
	tcase_add_test (tc, test_object_new);
	tcase_add_test (tc, test_object_resize);
TEST_FOOTER("OBJECT")

