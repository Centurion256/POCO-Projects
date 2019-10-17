#include <stdlib.h>
#include <stdint.h>
#include <check.h>
#include <string.h>
#include "../src/string.h"


my_str_t string;
my_str_t string2;

void setup(void)
{
    my_str_create(&string, 0);

}

void teardown(void)
{
    my_str_free(&string);
}

void setup2(void){
    setup();
    my_str_create(&string2, 0);
}

void teardown2(void){
    teardown();
    my_str_free(&string2);
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

START_TEST(string_test_reserve_small_buffer)
{
    size_t length = (size_t)10;
    const char* original_str = my_str_get_cstr(&string);

    my_str_resize(&string, length, length);
    size_t initial_size = my_str_size(&string);
    size_t initial_capacity = my_str_capacity(&string);
    
    ck_assert_int_eq(my_str_reserve(&string, 5), 0);
    ck_assert_int_eq(my_str_size(&string), initial_size);
    ck_assert_int_eq(my_str_capacity(&string), initial_capacity);
    ck_assert_str_eq(my_str_get_cstr(&string), original_str);
}
END_TEST

START_TEST(string_test_reserve_normal_buffer)
{
    size_t length = (size_t)10;
    const char* original_str = my_str_get_cstr(&string);

    my_str_resize(&string, length, length);
    size_t initial_size = my_str_size(&string);
    
    ck_assert_int_eq(my_str_reserve(&string, (size_t)200), 0);
    ck_assert_int_eq(my_str_size(&string), initial_size);
    ck_assert_int_eq(my_str_capacity(&string), (size_t)200);
}
END_TEST

START_TEST(string_test_reserve_giant_buffer)
{
    size_t length = (size_t)10;
    const char* original_str = my_str_get_cstr(&string);

    my_str_resize(&string, length, length);

    ck_assert_int_eq(my_str_reserve(&string, (size_t)1844674407370955161), -2); //2^64/10
}
END_TEST

Suite* string_actions_suite(void)
{
    Suite* suit;
    TCase* tc_resize;
    TCase* tc_shrink;
    TCase* tc_reserve;

    suit = suite_create("Memory opertaions, copy, etc.");

    tc_resize = tcase_create("Resize tests");
    tcase_add_checked_fixture(tc_resize, setup, teardown);
    tcase_add_test(tc_resize, string_test_resize);
    tcase_add_test(tc_resize, string_test_resize_negative);

    tc_shrink = tcase_create("Shrink tests");
    tcase_add_checked_fixture(tc_shrink, setup, teardown);
    tcase_add_test(tc_shrink, string_test_shrink);

    tc_reserve = tcase_create("Reserve tests");
    tcase_add_checked_fixture(tc_reserve, setup, teardown);
    tcase_add_test(tc_reserve, string_test_reserve_small_buffer);
    tcase_add_test(tc_reserve, string_test_reserve_normal_buffer);
    tcase_add_test(tc_reserve, string_test_reserve_giant_buffer);

    suite_add_tcase(suit, tc_resize);
    suite_add_tcase(suit, tc_shrink);
    suite_add_tcase(suit, tc_reserve);
    return suit;
}
START_TEST(string_test_size)
{
    ck_assert_int_eq(my_str_size(&string), 0);
}
END_TEST

START_TEST(string_test_size_10)
{
    my_str_resize(&string, 10, 0);
    ck_assert_int_eq(my_str_size(&string), 10);

}
END_TEST

START_TEST(string_test_capacity)
{
    my_str_resize(&string, 10, 0);
    ck_assert_int_eq(my_str_capacity(&string), 16);
}
END_TEST

START_TEST(string_test_empty)
{
    my_str_resize(&string, 0, 0);
    ck_assert_int_eq(my_str_size(&string), 0);
}
END_TEST

Suite* string_test_info_suite(void)
{
    Suite* suit;
    TCase* tc_size;
    TCase* tc_capacity;
    TCase* tc_empty;

    suit = suite_create("String info");

    tc_size = tcase_create("Size test");
    tcase_add_checked_fixture(tc_size, setup, teardown);
    tcase_add_test(tc_size, string_test_size);
    tcase_add_test(tc_size, string_test_size_10);
    suite_add_tcase(suit, tc_size);

    tc_capacity = tcase_create("Capacity test");
    tcase_add_checked_fixture(tc_capacity, setup, teardown);
    tcase_add_test(tc_capacity, string_test_capacity);
    suite_add_tcase(suit, tc_capacity);

    tc_empty = tcase_create("Empty string test");
    tcase_add_checked_fixture(tc_empty, setup, teardown);
    tcase_add_test(tc_empty, string_test_empty);
    suite_add_tcase(suit, tc_empty);
    
    return suit;    
}

int signum(int x)
{
    return (x > 0) - (x < 0);
}

START_TEST(string_test_cmp_empty)
{
    ck_assert_int_eq(my_str_cmp( &string, &string2), 0);

}END_TEST

START_TEST(string_test_cmp_same)
{
    my_str_resize(&string, 10, 'a');
    my_str_resize(&string2, 10, 'a');
    ck_assert_int_eq(signum(my_str_cmp(&string, &string2)), 
                     signum(strcmp(my_str_get_cstr(&string), my_str_get_cstr(&string2)))
                    );
}END_TEST


START_TEST(string_test_cmp_diff_size_same_chars)
{   
    my_str_resize(&string, 10, 'a');
    my_str_resize(&string2, 12, 'a');

    ck_assert_int_eq(signum(my_str_cmp(&string, &string2)), 
                     signum(strcmp(my_str_get_cstr(&string), my_str_get_cstr(&string2)))
                    );
}END_TEST

START_TEST(string_test_cmp_diff_size_diff_chars)
{   
    my_str_resize(&string, 10, 'a');
    my_str_resize(&string2, 12, 'a');
    my_str_putc(&string, 2, 'b');

    ck_assert_int_eq(signum(my_str_cmp(&string, &string2)), 
                     signum(strcmp(my_str_get_cstr(&string), my_str_get_cstr(&string2)))
                    );
}END_TEST

START_TEST(string_test_cmp_same_size_diff_chars)
{
    my_str_resize(&string, 10, 'a');
    my_str_resize(&string2, 10, 'a');
    my_str_putc(&string2, 2, 'b');
    ck_assert_int_eq(signum(my_str_cmp(&string, &string2)), 
                     signum(strcmp(my_str_get_cstr(&string), my_str_get_cstr(&string2)))
                    );
}END_TEST

START_TEST(string_test_cmp_empty_cstr)
{
    ck_assert_int_eq(my_str_cmp_cstr( &string, my_str_get_cstr(&string2)), 0);

}END_TEST

START_TEST(string_test_cmp_same_cstr)
{
    my_str_resize(&string, 10, 'a');
    my_str_resize(&string2, 10, 'a');
    ck_assert_int_eq(signum(my_str_cmp_cstr(&string, my_str_get_cstr(&string2))), 
                     signum(strcmp(my_str_get_cstr(&string), my_str_get_cstr(&string2)))
                    );
}END_TEST


START_TEST(string_test_cmp_diff_size_same_chars_cstr)
{   
    my_str_resize(&string, 10, 'a');
    my_str_resize(&string2, 12, 'a');

    ck_assert_int_eq(signum(my_str_cmp_cstr(&string, my_str_get_cstr(&string2))), 
                     signum(strcmp(my_str_get_cstr(&string), my_str_get_cstr(&string2)))
                    );
}END_TEST

START_TEST(string_test_cmp_diff_size_diff_chars_cstr)
{   
    my_str_resize(&string, 10, 'a');
    my_str_resize(&string2, 12, 'a');
    my_str_putc(&string, 2, 'b');

    ck_assert_int_eq(signum(my_str_cmp_cstr(&string, my_str_get_cstr(&string2))), 
                     signum(strcmp(my_str_get_cstr(&string), my_str_get_cstr(&string2)))
                    );
}END_TEST

START_TEST(string_test_cmp_same_size_diff_chars_cstr)
{
    my_str_resize(&string, 10, 'a');
    my_str_resize(&string2, 10, 'a');
    my_str_putc(&string2, 2, 'b');
    ck_assert_int_eq(signum(my_str_cmp_cstr(&string, my_str_get_cstr(&string2))), 
                     signum(strcmp(my_str_get_cstr(&string), my_str_get_cstr(&string2)))
                    );
}END_TEST


Suite* string_cmps_suite(void){
    Suite* suit;
    TCase* tc_cmp;
    TCase* tc_cmp_cstr;

    suit = suite_create("Stirng comparisons");

    tc_cmp = tcase_create("Compare tests");
    tcase_add_checked_fixture(tc_cmp, setup2, teardown2);
    tcase_add_test(tc_cmp, string_test_cmp_empty);
    tcase_add_test(tc_cmp, string_test_cmp_same);
    tcase_add_test(tc_cmp, string_test_cmp_diff_size_same_chars);
    tcase_add_test(tc_cmp, string_test_cmp_diff_size_diff_chars);
    tcase_add_test(tc_cmp, string_test_cmp_same_size_diff_chars);

    tc_cmp_cstr = tcase_create("Compare with cstring tests ");
    tcase_add_checked_fixture(tc_cmp_cstr, setup2, teardown2);
    tcase_add_test(tc_cmp_cstr, string_test_cmp_empty_cstr);
    tcase_add_test(tc_cmp_cstr, string_test_cmp_same_cstr);
    tcase_add_test(tc_cmp_cstr, string_test_cmp_diff_size_same_chars_cstr);
    tcase_add_test(tc_cmp_cstr, string_test_cmp_diff_size_diff_chars_cstr);
    tcase_add_test(tc_cmp_cstr, string_test_cmp_same_size_diff_chars_cstr);


    suite_add_tcase(suit, tc_cmp);
    suite_add_tcase(suit, tc_cmp_cstr);
    return suit;
}

int main(void)
{
    int failures = 0;
    SRunner *run;

    run = srunner_create(string_actions_suite());
    srunner_add_suite(run, string_test_info_suite());
    srunner_add_suite(run, string_cmps_suite());
    srunner_set_fork_status(run, CK_NOFORK);
    srunner_run_all(run, CK_NORMAL);
    failures = srunner_ntests_failed(run);
    srunner_free(run);
    return (failures == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}