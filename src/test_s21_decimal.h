#ifndef TEST
#define TEST

#include <check.h>
#include <stdlib.h>
#include <time.h>

#include "s21_decimal.h"

Suite *s21_math_operation(void);

void run_tests(void);
void run_testcase(Suite *testcase);
#endif  //  TEST