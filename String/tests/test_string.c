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
    size_t length = 1;
    int i;
    for (i = 0; i<6; i++)
    {
        length *= 10;
        ck_assert_int_eq(my_str_resize(&string, length, rem), 0);
        ck_assert(my_str_size(&string) == length);
    }
}
END_TEST

Suite* string_actions_suite(void)
{
    Suite* suit;
    TCase* tc_resize;

    suit = suite_create("Memory opertaions, copy, etc.");
    tc_resize = tcase_create("Resize/Reserve/Shrink");
    tcase_add_checked_fixture(tc_resize, setup, teardown);
    tcase_add_test(tc_resize, string_test_resize);

    suite_add_tcase(suit, tc_resize);
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