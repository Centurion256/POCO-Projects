#include <stdlib.h>
#include <stdint.h>
#include <check.h>
#include "../src/string.h"


my_str_t string;

void setup(void)
{
    my_str_create(&string, 0);
}

void teardown(void)
{
    my_str_free(&string);
}

START_TEST(string_test_resize)
{
    char rem = 'R';
    size_t length = (size_t)1;
    int i;
    for (i = 0; i<6; i++)
    {
        length *= (size_t)10;
        ck_assert_int_eq(my_str_resize(&string, length, rem), 0);
        ck_assert_msg(my_str_size(&string) == length, "my_str_size returns %zu, while length is %zu", my_str_size(&string), length);
    }
}
END_TEST

START_TEST(string_test_resize_negative)
{
    char rem = 'R';
    size_t length = (size_t)0;
    ck_assert_int_eq(my_str_resize(&string, length, rem), 0);
    ck_assert_int_eq(my_str_size(&string), 0);
}
END_TEST

START_TEST(string_test_shrink)
{
    size_t length = (size_t)10;
    const char* original_str = my_str_get_cstr(&string);
    my_str_resize(&string, length, 0);
    size_t actual_size = my_str_size(&string);
    ck_assert_int_eq(my_str_shrink_to_fit(&string), 0);
    ck_assert_int_eq(my_str_size(&string), actual_size);
    ck_assert_int_eq(my_str_capacity(&string), actual_size);
    ck_assert_str_eq(my_str_get_cstr(&string), original_str);
}
END_TEST

Suite* string_actions_suite(void)
{
    Suite* suit;
    TCase* tc_resize;
    TCase* tc_shrink;

    suit = suite_create("Memory opertaions, copy, etc.");

    tc_resize = tcase_create("Resize tests");
    tcase_add_checked_fixture(tc_resize, setup, teardown);
    tcase_add_test(tc_resize, string_test_resize);
    tcase_add_test(tc_resize, string_test_resize_negative);

    tc_shrink = tcase_create("Shrink tests");
    tcase_add_checked_fixture(tc_shrink, setup, teardown);
    tcase_add_test(tc_shrink, string_test_shrink);

    suite_add_tcase(suit, tc_resize);
    suite_add_tcase(suit, tc_shrink);
    return suit;
}

int main(void)
{
    int failures = 0;
    SRunner *run;

    run = srunner_create(string_actions_suite());
    srunner_set_fork_status(run, CK_NOFORK);
    srunner_run_all(run, CK_VERBOSE);
    failures = srunner_ntests_failed(run);
    srunner_free(run);
    return (failures == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}