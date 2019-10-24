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

int signum(int x)
{
    return (x > 0) - (x < 0);
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

    my_str_resize(&string, length, 'a');
    const char* original_str = my_str_get_cstr(&string);
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
    ck_assert_int_eq(my_str_cmp_cstr(&string, my_str_get_cstr(&string2)), 0);

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

    suit = suite_create("String comparisons");

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



START_TEST(string_test_getc){
    my_str_from_cstr(&string, "abcde", 0);
    ck_assert_int_eq(my_str_getc(&string, 2), 'c');
}END_TEST

START_TEST(string_test_getc_empty){
    ck_assert_int_eq(my_str_getc(&string, 1), -1);

}END_TEST

START_TEST(string_test_getc_last){
    my_str_from_cstr(&string, "abcde", 0);
    ck_assert_int_eq(my_str_getc(&string, 5), '\0');
}END_TEST

START_TEST(string_test_getc_empty_last){
    ck_assert_int_eq(my_str_getc(&string, 0), '\0');

}END_TEST

START_TEST(string_test_getc_out_of_bounds){
    //my_str_resize(&string, 5, 'a');
    my_str_from_cstr(&string, "aaaaa ", 0);
    ck_assert_msg(my_str_getc(&string, 100)==-1, "String size is %zu", my_str_size(&string));
}
END_TEST


START_TEST(string_test_setc){
    my_str_from_cstr(&string, "abcde", 0);
    ck_assert_int_eq(my_str_putc(&string, 2, 'H'), 0);
    ck_assert_int_eq(my_str_getc(&string, 2), 'H');
}END_TEST

START_TEST(string_test_setc_empty){
    ck_assert_int_eq(my_str_putc(&string, 2, 'H'), -1);
}END_TEST

START_TEST(string_test_setc_last){
    my_str_from_cstr(&string, "aa", 0);
    ck_assert_int_eq(my_str_putc(&string, 2, 'H'), -1);
    ck_assert_int_eq(my_str_getc(&string, 2), '\0');
}END_TEST

START_TEST(string_test_setc_out_of_bounds){
    my_str_from_cstr(&string, "abcde", 0);
    ck_assert_int_eq(my_str_putc(&string, 10, 'H'), -1);
}
END_TEST

START_TEST(string_test_get_cstr){
    
    my_str_resize(&string, 5, 'a');
    ck_assert_str_eq(my_str_get_cstr(&string), "aaaaa");
}END_TEST

START_TEST(string_test_get_cstr_empty){
    
    ck_assert_str_eq(my_str_get_cstr(&string), "");
    
}END_TEST

START_TEST(string_test_get_cstr_with_empty_space){
    my_str_resize(&string, 10, 'a');
    my_str_resize(&string, 5, ' ');
    ck_assert_str_eq(my_str_get_cstr(&string), "aaaaa");
}END_TEST

Suite* string_getset_suite(void){
    Suite* suit;
    TCase* tc_getc;
    TCase* tc_setc;
    TCase* tc_get_cstr;
    suit = suite_create("Getters and setters tests");

    tc_getc = tcase_create("Get char tests");
    tcase_add_checked_fixture(tc_getc, setup, teardown);
    tcase_add_test(tc_getc, string_test_getc);
    tcase_add_test(tc_getc, string_test_getc_empty);
    tcase_add_test(tc_getc, string_test_getc_last);
    tcase_add_test(tc_getc, string_test_getc_empty_last);
    tcase_add_test(tc_getc, string_test_getc_out_of_bounds);

    tc_setc = tcase_create("Put char tests");
    tcase_add_checked_fixture(tc_setc, setup, teardown);
    tcase_add_test(tc_setc, string_test_setc);
    tcase_add_test(tc_setc, string_test_setc_empty);
    tcase_add_test(tc_setc, string_test_setc_last);
    tcase_add_test(tc_setc, string_test_setc_out_of_bounds);

    tc_get_cstr = tcase_create("Get cstring tests");
    tcase_add_checked_fixture(tc_get_cstr, setup, teardown);
    tcase_add_test(tc_get_cstr, string_test_setc);
    tcase_add_test(tc_get_cstr, string_test_setc_empty);
    tcase_add_test(tc_get_cstr, string_test_get_cstr_with_empty_space);

    
    suite_add_tcase(suit, tc_getc);
    suite_add_tcase(suit, tc_setc);
    suite_add_tcase(suit, tc_get_cstr);
    return suit;

}



START_TEST(string_test_create){
    ck_assert_int_eq(my_str_create(&string, 10), 0);
    ck_assert_int_eq(my_str_capacity(&string), 10);
    ck_assert_int_eq(my_str_size(&string), 0);
    my_str_free(&string);
}END_TEST

START_TEST(string_test_create_giant_buffer){
    ck_assert_int_eq(my_str_create(&string, (size_t)1844674407370955161), -2);// 2**64/10
}END_TEST


START_TEST(string_test_from_cstr){
    ck_assert_int_eq(my_str_from_cstr(&string, "abcd", 10), 0);
    ck_assert_str_eq(my_str_get_cstr(&string), "abcd" );
}END_TEST

START_TEST(string_test_from_cstr_empty){
    ck_assert_int_eq(my_str_from_cstr(&string, "ab", 10), 0);
    ck_assert_str_eq(my_str_get_cstr(&string), "ab" );
}END_TEST

START_TEST(string_test_from_cstr_buf_too_small){
    ck_assert_int_eq(my_str_from_cstr(&string, "abcdef", 3), -1);
}END_TEST

START_TEST(string_test_from_cstr_giant_buffer){
    ck_assert_int_eq(my_str_from_cstr(&string, "ab", (size_t)1844674407370955161), -2);// 2**64/10
}END_TEST

START_TEST(string_test_from_cstr_buf_zero){
    ck_assert_int_eq(my_str_from_cstr(&string, "ab", 0), 0);
    ck_assert_str_eq(my_str_get_cstr(&string), "ab" );
}END_TEST

Suite* string_constructors_suite(void){
    Suite* suit;
    TCase* tc_create;
    TCase* tc_from_cstr;
    suit = suite_create("Constructors tests");
    tc_create = tcase_create("Test create");
    tcase_add_test(tc_create, string_test_create);
    tcase_add_test(tc_create, string_test_create_giant_buffer);
    
    tc_from_cstr = tcase_create("Test create from cstring");
    tcase_add_checked_fixture(tc_from_cstr, setup, teardown);
    tcase_add_test(tc_from_cstr, string_test_from_cstr);
    tcase_add_test(tc_from_cstr, string_test_from_cstr_empty);
    tcase_add_test(tc_from_cstr, string_test_from_cstr_buf_too_small);
    tcase_add_test(tc_from_cstr, string_test_from_cstr_giant_buffer);
    tcase_add_test(tc_from_cstr, string_test_from_cstr_buf_zero);

    suite_add_tcase(suit, tc_create);
    suite_add_tcase(suit, tc_from_cstr);

    return suit;
}


START_TEST(string_test_pushback){
    my_str_resize(&string, 5, 'a');
    ck_assert_int_eq(my_str_pushback(&string, 'b'), 0);
    ck_assert_int_eq(my_str_getc(&string, 5), 'b');
    ck_assert_int_eq(my_str_size(&string), 6);
    
}END_TEST
START_TEST(string_test_pushback_empty){
    ck_assert_int_eq(my_str_pushback(&string, 'b'), 0);
    ck_assert_int_eq(my_str_getc(&string, 0), 'b');
    ck_assert_int_eq(my_str_size(&string), 1);
}END_TEST


START_TEST(string_test_popback)
{
    my_str_resize(&string, 5, 'a');
    ck_assert_int_eq(my_str_popback(&string), 'a');
    ck_assert_int_eq(my_str_size(&string), 4);
    
}END_TEST
START_TEST(string_test_popback_one_element)
{
    my_str_resize(&string, 1, 'a');
    ck_assert_int_eq(my_str_popback(&string), 'a');
    ck_assert_int_eq(my_str_size(&string), 0);
    
}END_TEST
START_TEST(string_test_popback_empty)
{
    
    ck_assert_int_eq(my_str_popback(&string), -2);
    ck_assert_int_eq(my_str_size(&string), 0);
    
}END_TEST


START_TEST(string_test_copy){
    my_str_from_cstr(&string, "abcde", 0);
    ck_assert_int_eq(my_str_copy(&string, &string2, 0), 0);
    ck_assert_str_eq(my_str_get_cstr(&string), my_str_get_cstr(&string2));
}END_TEST


START_TEST(string_test_copy_with_exect_size){
    ck_assert_int_eq(my_str_copy(&string, &string2, 1), 0);
    ck_assert_int_ne(my_str_capacity(&string), my_str_capacity(&string2));
}END_TEST


START_TEST(string_test_clear){
    my_str_resize(&string, 5, 'a');
    my_str_clear(&string);
    ck_assert_int_eq(my_str_size(&string), 0);
}END_TEST
START_TEST(string_test_clear_empty){
    my_str_clear(&string);
    ck_assert_int_eq(my_str_size(&string), 0);
}END_TEST

START_TEST(string_test_insert_c)
{
    my_str_resize(&string, 5, 'a');
    ck_assert_int_eq(my_str_insert_c(&string, 'b', 1), 0);
    ck_assert_str_eq(my_str_get_cstr(&string), "abaaaa");
}END_TEST

START_TEST(string_test_insert_c_out_of_bounds)
{
    my_str_resize(&string, 5, 'a');
    ck_assert_int_eq(my_str_insert_c(&string, 'b', 6), -1);
    ck_assert_str_eq(my_str_get_cstr(&string), "aaaaa");
}END_TEST

START_TEST(string_test_insert_c_last_index)
{
    my_str_resize(&string, 5, 'a');
    ck_assert_int_eq(my_str_insert_c(&string, 'b', 5), 0);
    ck_assert_str_eq(my_str_get_cstr(&string), "aaaaab");

}END_TEST

START_TEST(string_test_insert)
{
    my_str_resize(&string, 5, 'a');
    my_str_resize(&string2, 2, 'b');
    ck_assert_int_eq(my_str_insert(&string, &string2, 1), 0);
    ck_assert_str_eq(my_str_get_cstr(&string), "abbaaaa");
}END_TEST

START_TEST(string_test_insert_empty)
{
    my_str_resize(&string, 5, 'a');
    ck_assert_int_eq(my_str_insert(&string, &string2, 1), 0);
    ck_assert_str_eq(my_str_get_cstr(&string), "aaaaa");
}END_TEST

START_TEST(string_test_insert_cstr){
    my_str_resize(&string, 5, 'a');
    ck_assert_int_eq(my_str_insert_cstr(&string, "bb", 2), 0);
    ck_assert_str_eq(my_str_get_cstr(&string), "aabbaaa");
}END_TEST

START_TEST(string_test_insert_empty_cstr){
    my_str_resize(&string, 5, 'a');
    ck_assert_int_eq(my_str_insert_cstr(&string, "", 2), 0);
    ck_assert_str_eq(my_str_get_cstr(&string), "aaaaa");
}END_TEST
START_TEST(string_test_insert_cstr_empty){
    ck_assert_int_eq(my_str_insert_cstr(&string, "bb", 0), 0);
    ck_assert_str_eq(my_str_get_cstr(&string), "bb");
}END_TEST
START_TEST(string_test_insert_cstr_out_of_bounds){
    my_str_resize(&string, 5, 'a');
    ck_assert_int_eq(my_str_insert_cstr(&string, "bb", 10), -1);
    ck_assert_str_eq(my_str_get_cstr(&string), "aaaaa");
}END_TEST


START_TEST(string_test_append){
    my_str_resize(&string, 5, 'a');
    ck_assert_str_eq(my_str_get_cstr(&string), "aaaaa");
    my_str_resize(&string2, 4, 'b');
    ck_assert_str_eq(my_str_get_cstr(&string2), "bbbb");
    ck_assert_int_eq(my_str_append(&string, &string2), 0);
    ck_assert_str_eq(my_str_get_cstr(&string), "aaaaabbbb");
    ck_assert_str_eq(my_str_get_cstr(&string2), "bbbb");

}END_TEST
START_TEST(string_test_append_empty){
    my_str_resize(&string, 5, 'a');
    my_str_resize(&string2, 0, 'b');
    ck_assert_int_eq(my_str_append(&string, &string2), 0);
    ck_assert_str_eq(my_str_get_cstr(&string), "aaaaa");
}END_TEST
START_TEST(string_test_append_to_empty){
    my_str_resize(&string, 0, 'a');
    my_str_resize(&string2, 4, 'b');
    ck_assert_int_eq(my_str_append(&string, &string2), 0);
    ck_assert_str_eq(my_str_get_cstr(&string), "bbbb");
}END_TEST


START_TEST(string_test_append_cstr){
    my_str_resize(&string, 5, 'a');
    ck_assert_int_eq(my_str_append_cstr(&string, "bbb"), 0);
    ck_assert_str_eq(my_str_get_cstr(&string), "aaaaabbb");
}END_TEST
START_TEST(string_test_append_empty_cstr){
    my_str_resize(&string, 5, 'a');
    ck_assert_int_eq(my_str_append_cstr(&string, ""), 0);
    ck_assert_str_eq(my_str_get_cstr(&string), "aaaaa");
}END_TEST
START_TEST(string_test_append_cstr_to_empty){
    my_str_resize(&string, 0, 'a');
    ck_assert_int_eq(my_str_append_cstr(&string, "bbb"), 0);
    ck_assert_str_eq(my_str_get_cstr(&string), "bbb");
}END_TEST




START_TEST(string_test_substr){
    my_str_from_cstr(&string, "abcdefg", 10);
    my_str_resize(&string2, 4, 'b');
    ck_assert_int_eq(my_str_substr(&string, &string2, 2, 4), 0);
    ck_assert_str_eq(my_str_get_cstr(&string2), "cd");
}END_TEST

START_TEST(string_test_substr_from_empty){
    my_str_resize(&string2, 4, 'b');
    ck_assert_int_eq(my_str_substr(&string, &string2, 2, 4), -1);
    ck_assert_str_eq(my_str_get_cstr(&string2), "bbbb");

}END_TEST

START_TEST(string_test_substr_out_of_bounds){
    my_str_from_cstr(&string, "abcdefg", 10);
    my_str_resize(&string2, 4, 'b');
    ck_assert_int_eq(my_str_substr(&string, &string2, 9, 4), -1);
    ck_assert_str_eq(my_str_get_cstr(&string2), "bbbb");
}END_TEST

START_TEST(string_test_substr_end_out_of_bounds){
    my_str_from_cstr(&string, "abcdefg", 10);
    my_str_resize(&string2, 4, 'b');
    ck_assert_int_eq(my_str_substr(&string, &string2, 2, 10), 0);
    ck_assert_str_eq(my_str_get_cstr(&string2), "cdefg");

}END_TEST

START_TEST(string_test_substr_end_eq_beg){
    my_str_from_cstr(&string, "abcdefg", 10);
    my_str_resize(&string2, 4, 'b');
    ck_assert_int_eq(my_str_substr(&string, &string2, 4, 4), 0);
    ck_assert_str_eq(my_str_get_cstr(&string2), "");

}END_TEST

START_TEST(string_test_substr_end_less_beg){
    my_str_from_cstr(&string, "abcdefg", 10);
    my_str_resize(&string2, 4, 'b');
    ck_assert_int_eq(my_str_substr(&string, &string2, 4, 3), 0);
    ck_assert_str_eq(my_str_get_cstr(&string2), "");

}END_TEST



START_TEST(string_test_substr_cstr){
    char my_cstr[10];
    my_str_resize(&string, 7, 'a');
    ck_assert_int_eq(my_str_substr_cstr(&string, my_cstr, (size_t) 0, (size_t) 100), 0);
    ck_assert_str_eq(my_cstr, "aaaaaaa");
}END_TEST

START_TEST(string_test_substr_cstr2){
    char my_cstr[10];
    my_str_resize(&string, 3, 'a');
    my_str_resize(&string, 7, 'b');
    ck_assert_int_eq(my_str_substr_cstr(&string, my_cstr, (size_t) 1, (size_t) 5), 0);
    ck_assert_str_eq(my_cstr, "aabb");
}END_TEST

START_TEST(string_test_substr_cstr_from_empty){
    char my_cstr[1];
    ck_assert_int_eq(my_str_substr_cstr(&string, my_cstr, (size_t) 0, (size_t) 5), -1);
    
}END_TEST

START_TEST(string_test_substr_cstr_out_of_bounds){
    char my_cstr[1];
    my_str_resize(&string, 3, 'a');
    ck_assert_int_eq(my_str_substr_cstr(&string, my_cstr, (size_t) -2, (size_t) 5), -1);
}END_TEST

START_TEST(string_test_substr_cstr_out_of_bounds2){
    char my_cstr[1];
    my_str_resize(&string, 3, 'a');
    ck_assert_int_eq(my_str_substr_cstr(&string, my_cstr, (size_t) 5, (size_t) 1), -1);
}END_TEST

START_TEST(string_test_substr_cstr_out_of_bounds3){
    char my_cstr[1];
    my_str_resize(&string, 3, 'a');
    ck_assert_int_eq(my_str_substr_cstr(&string, my_cstr, (size_t) 100, (size_t) 200), -1);
}END_TEST


Suite* string_modification_suite(void)
{
    Suite* suit;
    TCase* tc_pushback;
    TCase* tc_popback;
    TCase* tc_copy;
    TCase* tc_clear;
    TCase* tc_insert_c;
    TCase* tc_insert;
    TCase* tc_insert_cstr;
    TCase* tc_append;
    TCase* tc_append_cstr;
    TCase* tc_substr;
    TCase* tc_substr_cstr;

    suit = suite_create("Modification suite");
       
    tc_pushback = tcase_create("Pushback test");
    tcase_add_checked_fixture(tc_pushback, setup, teardown);
    tcase_add_test(tc_pushback, string_test_pushback);
    tcase_add_test(tc_pushback, string_test_pushback_empty);


    tc_popback = tcase_create("Create test");
    tcase_add_checked_fixture(tc_popback, setup, teardown);
    tcase_add_test(tc_popback, string_test_popback);
    tcase_add_test(tc_popback, string_test_popback_one_element);
    tcase_add_test(tc_popback, string_test_popback_empty);

    tc_copy = tcase_create("Copy case");
    tcase_add_checked_fixture(tc_copy, setup2, teardown2);
    tcase_add_test(tc_copy, string_test_copy);
    tcase_add_test(tc_copy, string_test_copy_with_exect_size);

    tc_clear = tcase_create("String clear test");
    tcase_add_checked_fixture(tc_clear, setup, teardown);
    tcase_add_test(tc_clear, string_test_clear);
    tcase_add_test(tc_clear, string_test_clear_empty);

    tc_insert_c = tcase_create("String insert character test");
    tcase_add_checked_fixture(tc_insert_c, setup, teardown);
    tcase_add_test(tc_insert_c, string_test_insert_c);
    tcase_add_test(tc_insert_c, string_test_insert_c_out_of_bounds);
    tcase_add_test(tc_insert_c, string_test_insert_c_last_index);
    tc_insert_cstr = tcase_create("Insert cstring test");
    //FIXED free(): double free detected in tcache 2 FIXED
    tcase_add_checked_fixture(tc_insert_cstr, setup, teardown);
    tcase_add_test(tc_insert_cstr, string_test_insert_cstr);
    tcase_add_test(tc_insert_cstr, string_test_insert_empty_cstr);
    tcase_add_test(tc_insert_cstr, string_test_insert_cstr_empty);
    tcase_add_test(tc_insert_cstr, string_test_insert_cstr_out_of_bounds);

    tc_insert = tcase_create("Insert my_str_t test");
    tcase_add_checked_fixture(tc_insert, setup2, teardown2);
    tcase_add_test(tc_insert, string_test_insert);
    tcase_add_test(tc_insert, string_test_insert_empty);
    


    tc_append = tcase_create("String append test");
    tcase_add_checked_fixture(tc_append, setup2, teardown2);
    tcase_add_test(tc_append, string_test_append);
    tcase_add_test(tc_append, string_test_append_empty);
    tcase_add_test(tc_append, string_test_append_to_empty);

    tc_append_cstr = tcase_create("Cstring append test");
    tcase_add_checked_fixture(tc_append_cstr, setup2, teardown2);
    tcase_add_test(tc_append_cstr, string_test_append_cstr);
    tcase_add_test(tc_append_cstr, string_test_append_empty_cstr);
    tcase_add_test(tc_append_cstr, string_test_append_cstr_to_empty);

    tc_substr = tcase_create("Substring test");
    tcase_add_checked_fixture(tc_substr, setup2, teardown2);
    tcase_add_test(tc_substr, string_test_substr);
    tcase_add_test(tc_substr, string_test_substr_from_empty);
    tcase_add_test(tc_substr, string_test_substr_end_out_of_bounds);
    tcase_add_test(tc_substr, string_test_substr_out_of_bounds);
    tcase_add_test(tc_substr, string_test_substr_end_less_beg);
    tcase_add_test(tc_substr, string_test_substr_end_eq_beg);



    tc_substr_cstr = tcase_create("Substring c string test");
    tcase_add_checked_fixture(tc_substr_cstr, setup, teardown);
    tcase_add_test(tc_substr_cstr, string_test_substr_cstr);
    tcase_add_test(tc_substr_cstr, string_test_substr_cstr2);
    tcase_add_test(tc_substr_cstr, string_test_substr_cstr_from_empty);
    tcase_add_test(tc_substr_cstr, string_test_substr_cstr_out_of_bounds);
    tcase_add_test(tc_substr_cstr, string_test_substr_cstr_out_of_bounds2);
    tcase_add_test(tc_substr_cstr, string_test_substr_cstr_out_of_bounds3);
    
    
    
    suite_add_tcase(suit, tc_pushback);
    suite_add_tcase(suit, tc_popback);
    suite_add_tcase(suit, tc_copy);
    suite_add_tcase(suit, tc_clear);
    suite_add_tcase(suit, tc_insert_c);
    suite_add_tcase(suit, tc_insert_cstr);
    suite_add_tcase(suit, tc_insert);
    suite_add_tcase(suit, tc_append);
    suite_add_tcase(suit, tc_append_cstr);
    suite_add_tcase(suit, tc_substr);
    suite_add_tcase(suit, tc_substr_cstr);


    return suit;
}

int main(void)
{
    int failures = 0;
    SRunner *run;

    run = srunner_create(string_test_info_suite());
    srunner_add_suite(run, string_actions_suite());
    srunner_add_suite(run, string_cmps_suite());
    srunner_add_suite(run, string_getset_suite());
    srunner_add_suite(run, string_constructors_suite());
    srunner_add_suite(run, string_modification_suite());
    srunner_set_fork_status(run, CK_NOFORK);
    srunner_run_all(run, CK_NORMAL);
    failures = srunner_ntests_failed(run);
    srunner_free(run);
    return (failures == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}