#include "test_s21_decimal.h"

int main(void) {
  srand(time(0));
  Suite *s;
  SRunner *sr;
  s = s21_math_operation();
  static int counter_testcase = 1;
  if (counter_testcase > 1) putchar('\n');
  printf("%s%d%s", "CURRENT TEST: ", counter_testcase, "\n");
  counter_testcase++;
  sr = srunner_create(s);
  srunner_run_all(sr, CK_VERBOSE);
  srunner_free(sr);
  return 0;
}

START_TEST(is_less) {
  // 1 case
  s21_decimal value_1 = {{0, 0, 0, 0}}, value_2 = {{0, 0, 0, 0}};
  int res = s21_is_less(value_1, value_2);
  ck_assert_int_eq(res, 0);

  // 2 case
  value_1.bits[0] = 1u << 31, value_2.bits[1] = 1;
  res = s21_is_less(value_1, value_2);
  ck_assert_int_eq(res, 1);

  toZero(&value_1);
  toZero(&value_2);

  // 3 case
  value_1.bits[2] = 1, value_1.bits[3] = 1u << 31;
  value_2.bits[0] = 1;
  res = s21_is_less(value_1, value_2);
  ck_assert_int_eq(res, 1);

  toZero(&value_1);
  toZero(&value_2);

  // 4 case
  value_1.bits[0] = 100, value_1.bits[3] = 3u << 16;
  value_2.bits[0] = 1, value_2.bits[3] = 1u << 16;
  res = s21_is_less(value_1, value_2);
  ck_assert_int_eq(res, 0);

  toZero(&value_1);
  toZero(&value_2);

  // 5 case
  value_1.bits[0] = UINT32_MAX, value_1.bits[1] = UINT32_MAX;
  value_1.bits[2] = UINT32_MAX, value_1.bits[3] = (1u << 31) + (28u << 16);
  value_2.bits[0] = UINT32_MAX, value_2.bits[1] = UINT32_MAX;
  value_2.bits[2] = UINT32_MAX, value_2.bits[3] = (1u << 31) + (28u << 16);
  res = s21_is_less(value_1, value_2);
  ck_assert_int_eq(res, 0);

  // 6 case
  value_2.bits[3] = (1u << 31) + (12u << 16);
  res = s21_is_less(value_1, value_2);
  // printf("VALUE 1 \n");
  // for (int i = 127; i >= 0; i--) {
  //   printf("%d", getBit(value_1, i));
  // }
  // printf("\n");
  // printf("VALUE 2\n");
  // for (int i = 127; i >= 0; i--) {
  //   printf("%d", getBit(value_2, i));
  // }
  // printf("\n");

  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(is_less_onemore) {
  int res;
  printf("START ONEMORE\n");
  s21_decimal value_11 = {{1, 2, 3, 655360}},
              value_21 = {{3, 2, 1, -2146828288}};
  res = s21_is_less(value_11, value_21);
  ck_assert_int_eq(res, 0);

  s21_decimal value_12 = {{1, 2, 3, -2146828288}},
              value_22 = {{3, 2, 1, -2146828288}};
  res = s21_is_less(value_12, value_22);
  ck_assert_int_eq(res, 1);

  s21_decimal value_13 = {{1, 2, 3, -2146566144}},
              value_23 = {{3, 2, 1, -2146959360}};
  res = s21_is_less(value_13, value_23);
  ck_assert_int_eq(res, 0);

  s21_decimal value_14 = {{10, 2, 3, -2146566144}},
              value_24 = {{3, 2, 1, 524288}};
  res = s21_is_less(value_14, value_24);
  ck_assert_int_eq(res, 1);

  s21_decimal value_15 = {{1, 2, 3, 917504}}, value_25 = {{1, 2, 3, 917504}};
  res = s21_is_less(value_15, value_25);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(is_less_or_equal_onemore) {
  int res;

  s21_decimal value_11 = {{1, 2, 3, 655360}},
              value_21 = {{3, 2, 1, -2146828288}};
  res = s21_is_less_or_equal(value_11, value_21);
  ck_assert_int_eq(res, 0);

  s21_decimal value_12 = {{1, 2, 3, -2146828288}},
              value_22 = {{3, 2, 1, -2146828288}};
  res = s21_is_less_or_equal(value_12, value_22);
  ck_assert_int_eq(res, 1);

  s21_decimal value_13 = {{1, 2, 3, -2146566144}},
              value_23 = {{3, 2, 1, -2146959360}};
  res = s21_is_less_or_equal(value_13, value_23);
  ck_assert_int_eq(res, 0);

  s21_decimal value_14 = {{10, 2, 3, -2146566144}},
              value_24 = {{3, 2, 1, 524288}};

  res = s21_is_less_or_equal(value_14, value_24);
  ck_assert_int_eq(res, 1);

  s21_decimal value_15 = {{1, 2, 3, 917504}}, value_25 = {{1, 2, 3, 917504}};
  res = s21_is_less_or_equal(value_15, value_25);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(is_greater_onemore) {
  int res;

  s21_decimal value_11 = {{1, 2, 3, 655360}},
              value_21 = {{3, 2, 1, -2146828288}};
  res = s21_is_greater(value_11, value_21);
  ck_assert_int_eq(res, 1);

  s21_decimal value_12 = {{1, 2, 3, -2146828288}},
              value_22 = {{3, 2, 1, -2146828288}};
  res = s21_is_greater(value_12, value_22);
  ck_assert_int_eq(res, 0);

  s21_decimal value_13 = {{1, 2, 3, -2146566144}},
              value_23 = {{3, 2, 1, -2146959360}};
  res = s21_is_greater(value_13, value_23);
  ck_assert_int_eq(res, 1);

  s21_decimal value_14 = {{10, 2, 3, -2146566144}},
              value_24 = {{3, 2, 1, 524288}};

  res = s21_is_greater(value_14, value_24);
  ck_assert_int_eq(res, 0);

  s21_decimal value_15 = {{1, 2, 3, 917504}}, value_25 = {{1, 2, 3, 917504}};
  res = s21_is_greater(value_15, value_25);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(is_greater_or_equal_onemore) {
  int res;

  s21_decimal value_11 = {{1, 2, 3, 655360}},
              value_21 = {{3, 2, 1, -2146828288}};
  res = s21_is_greater_or_equal(value_11, value_21);
  ck_assert_int_eq(res, 1);

  s21_decimal value_12 = {{1, 2, 3, -2146828288}},
              value_22 = {{3, 2, 1, -2146828288}};
  res = s21_is_greater_or_equal(value_12, value_22);
  ck_assert_int_eq(res, 0);

  s21_decimal value_13 = {{1, 2, 3, -2146566144}},
              value_23 = {{3, 2, 1, -2146959360}};
  res = s21_is_greater_or_equal(value_13, value_23);
  ck_assert_int_eq(res, 1);

  s21_decimal value_14 = {{10, 2, 3, -2146566144}},
              value_24 = {{3, 2, 1, 524288}};

  res = s21_is_greater_or_equal(value_14, value_24);
  ck_assert_int_eq(res, 0);

  s21_decimal value_15 = {{1, 2, 3, 917504}}, value_25 = {{1, 2, 3, 917504}};
  res = s21_is_greater_or_equal(value_15, value_25);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(is_equal_onemore) {
  int res;

  s21_decimal value_11 = {{1, 2, 3, 655360}},
              value_21 = {{3, 2, 1, -2146828288}};
  res = s21_is_equal(value_11, value_21);
  ck_assert_int_eq(res, 0);

  s21_decimal value_12 = {{1, 2, 3, -2146828288}},
              value_22 = {{3, 2, 1, -2146828288}};
  res = s21_is_equal(value_12, value_22);
  ck_assert_int_eq(res, 0);

  s21_decimal value_13 = {{1, 2, 3, -2146566144}},
              value_23 = {{3, 2, 1, -2146959360}};
  res = s21_is_equal(value_13, value_23);
  ck_assert_int_eq(res, 0);

  s21_decimal value_14 = {{10, 2, 3, -2146566144}},
              value_24 = {{3, 2, 1, 524288}};

  res = s21_is_equal(value_14, value_24);
  ck_assert_int_eq(res, 0);

  s21_decimal value_15 = {{1, 2, 3, 917504}}, value_25 = {{1, 2, 3, 917504}};
  res = s21_is_equal(value_15, value_25);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(is_not_equal_onemore) {
  int res;

  s21_decimal value_11 = {{1, 2, 3, 655360}},
              value_21 = {{3, 2, 1, -2146828288}};
  res = s21_is_not_equal(value_11, value_21);
  ck_assert_int_eq(res, 1);

  s21_decimal value_12 = {{1, 2, 3, -2146828288}},
              value_22 = {{3, 2, 1, -2146828288}};
  res = s21_is_not_equal(value_12, value_22);
  ck_assert_int_eq(res, 1);

  s21_decimal value_13 = {{1, 2, 3, -2146566144}},
              value_23 = {{3, 2, 1, -2146959360}};
  res = s21_is_not_equal(value_13, value_23);
  ck_assert_int_eq(res, 1);

  s21_decimal value_14 = {{10, 2, 3, -2146566144}},
              value_24 = {{3, 2, 1, 524288}};

  res = s21_is_not_equal(value_14, value_24);
  ck_assert_int_eq(res, 1);

  s21_decimal value_15 = {{1, 2, 3, 917504}}, value_25 = {{1, 2, 3, 917504}};
  res = s21_is_not_equal(value_15, value_25);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(is_less_or_equal) {
  // 1 case
  s21_decimal value_1 = {{0, 0, 0, 0}}, value_2 = {{0, 0, 0, 0}};
  int res = s21_is_less_or_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);

  // 2 case
  value_1.bits[1] = 1, value_2.bits[0] = 1u << 31;
  res = s21_is_less_or_equal(value_1, value_2);
  ck_assert_int_eq(res, 0);

  toZero(&value_1);
  toZero(&value_2);

  // 3 case
  value_1.bits[2] = 1, value_1.bits[3] = 1u << 31;
  value_2.bits[0] = 1;
  res = s21_is_less_or_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);

  toZero(&value_1);
  toZero(&value_2);

  // 4 case
  value_1.bits[0] = 1, value_1.bits[3] = 1u << 16;
  value_2.bits[0] = 100, value_2.bits[3] = 3u << 16;
  res = s21_is_less_or_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);

  toZero(&value_1);
  toZero(&value_2);

  // 5 case
  value_1.bits[0] = UINT32_MAX, value_1.bits[1] = UINT32_MAX;
  value_1.bits[2] = UINT32_MAX;
  value_1.bits[3] = (1u << 31) + (28u << 16);
  value_2.bits[0] = UINT32_MAX, value_2.bits[1] = UINT32_MAX;
  value_2.bits[2] = UINT32_MAX;
  value_2.bits[3] = (1u << 31) + (28u << 16);
  res = s21_is_less_or_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);

  // 6 case
  value_2.bits[3] = (1u << 31) + (12u << 16);
  res = s21_is_less_or_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(is_greater) {
  // 1 case
  s21_decimal value_1 = {{0, 0, 0, 0}}, value_2 = {{0, 0, 0, 0}};
  int res = s21_is_greater(value_1, value_2);
  ck_assert_int_eq(res, 0);

  // 2 case
  value_1.bits[1] = 1, value_2.bits[0] = 1u << 31;
  res = s21_is_greater(value_1, value_2);
  ck_assert_int_eq(res, 1);

  toZero(&value_1);
  toZero(&value_2);

  // 3 case
  value_1.bits[2] = 1, value_1.bits[3] = 1u << 31;
  value_2.bits[0] = 1;
  res = s21_is_greater(value_1, value_2);
  ck_assert_int_eq(res, 0);

  toZero(&value_1);
  toZero(&value_2);

  // 4 case
  value_1.bits[0] = 1, value_1.bits[3] = 1u << 16;
  value_2.bits[0] = 100, value_2.bits[3] = 3u << 16;
  res = s21_is_greater(value_1, value_2);
  ck_assert_int_eq(res, 0);

  toZero(&value_1);
  toZero(&value_2);

  // 5 case
  value_1.bits[0] = UINT32_MAX, value_1.bits[1] = UINT32_MAX;
  value_1.bits[2] = UINT32_MAX;
  value_1.bits[3] = (1u << 31) + (28u << 16);
  value_2.bits[0] = UINT32_MAX, value_2.bits[1] = UINT32_MAX;
  value_2.bits[2] = UINT32_MAX;
  value_2.bits[3] = (1u << 31) + (28u << 16);
  res = s21_is_greater(value_1, value_2);
  ck_assert_int_eq(res, 0);

  // 6 case
  value_2.bits[3] = (1u << 31) + (12u << 16);
  res = s21_is_greater(value_1, value_2);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(is_greater_or_equal) {
  // 1 case
  s21_decimal value_1 = {{0, 0, 0, 0}}, value_2 = {{0, 0, 0, 0}};
  int res = s21_is_greater_or_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);

  // 2 case
  value_1.bits[1] = 1, value_2.bits[0] = 1u << 31;
  res = s21_is_greater_or_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);

  toZero(&value_1);
  toZero(&value_2);

  // 3 case
  value_1.bits[2] = 1, value_1.bits[3] = 1u << 31;
  value_2.bits[0] = 1;
  res = s21_is_greater_or_equal(value_1, value_2);
  ck_assert_int_eq(res, 0);

  toZero(&value_1);
  toZero(&value_2);

  // 4 case
  value_1.bits[0] = 1, value_1.bits[3] = 1u << 16;
  value_2.bits[0] = 100, value_2.bits[3] = 3u << 16;
  res = s21_is_greater_or_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);
  toZero(&value_1);
  toZero(&value_2);

  // 5 case
  value_1.bits[0] = UINT32_MAX, value_1.bits[1] = UINT32_MAX;
  value_1.bits[2] = UINT32_MAX;
  value_1.bits[3] = (1u << 31) + (28u << 16);
  value_2.bits[0] = UINT32_MAX, value_2.bits[1] = UINT32_MAX;
  value_2.bits[2] = UINT32_MAX;
  value_2.bits[3] = (1u << 31) + (28u << 16);
  res = s21_is_greater_or_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);

  // 6 case
  value_2.bits[3] = (1u << 31) + (12u << 16);
  res = s21_is_greater_or_equal(value_1, value_2);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(is_equal) {
  // 1 case
  s21_decimal value_1 = {{0, 0, 0, 0}}, value_2 = {{0, 0, 0, 1u << 31}};
  int res = s21_is_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);

  // 2 case
  value_1.bits[1] = 1, value_2.bits[0] = 1u << 31;
  res = s21_is_equal(value_1, value_2);
  ck_assert_int_eq(res, 0);

  toZero(&value_1);
  toZero(&value_2);

  // 3 case
  value_1.bits[2] = 1, value_1.bits[3] = 1u << 31;
  value_2.bits[0] = 1;
  res = s21_is_equal(value_1, value_2);
  ck_assert_int_eq(res, 0);

  toZero(&value_1);
  toZero(&value_2);

  value_1.bits[0] = 1, value_1.bits[3] = 1u << 16;
  value_2.bits[0] = 100, value_2.bits[3] = 3u << 16;
  res = s21_is_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);

  toZero(&value_1);
  toZero(&value_2);

  // 4 case
  value_1.bits[0] = UINT32_MAX, value_1.bits[1] = UINT32_MAX;
  value_1.bits[2] = UINT32_MAX;
  value_1.bits[3] = (1u << 31) + (28u << 16);
  value_2.bits[0] = UINT32_MAX, value_2.bits[1] = UINT32_MAX;
  value_2.bits[2] = UINT32_MAX;
  value_2.bits[3] = (1u << 31) + (28u << 16);
  res = s21_is_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);

  // 5 case
  value_2.bits[3] = (1u << 31) + (12u << 16);
  res = s21_is_equal(value_1, value_2);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(is_not_equal) {
  // 1 case
  s21_decimal value_1 = {{0, 0, 0, 0}}, value_2 = {{0, 0, 0, 1u << 31}};
  int res = s21_is_not_equal(value_1, value_2);
  ck_assert_int_eq(res, 0);

  // 2 case
  value_1.bits[1] = 1, value_2.bits[0] = 1u << 31;
  res = s21_is_not_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);

  toZero(&value_1);
  toZero(&value_2);

  // 3 case
  value_1.bits[2] = 1, value_1.bits[3] = 1u << 31;
  value_2.bits[0] = 1;
  res = s21_is_not_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);

  toZero(&value_1);
  toZero(&value_2);

  value_1.bits[0] = 1, value_1.bits[3] = 1u << 16;
  value_2.bits[0] = 100, value_2.bits[3] = 3u << 16;
  res = s21_is_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);

  toZero(&value_1);
  toZero(&value_2);

  // 4 case
  value_1.bits[0] = UINT32_MAX, value_1.bits[1] = UINT32_MAX;
  value_1.bits[2] = UINT32_MAX;
  value_1.bits[3] = (1u << 31) + (28u << 16);
  value_2.bits[0] = UINT32_MAX, value_2.bits[1] = UINT32_MAX;
  value_2.bits[2] = UINT32_MAX;
  value_2.bits[3] = (1u << 31) + (28u << 16);
  res = s21_is_not_equal(value_1, value_2);
  ck_assert_int_eq(res, 0);

  // 5 case
  value_2.bits[3] = (1u << 31) + (12u << 16);
  res = s21_is_not_equal(value_1, value_2);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(add_0) {
  // 1 case 0 0
  s21_decimal value_1 = {{0, 0, 0, 0}}, value_2 = {{0, 0, 0, 0}},
              result = {{0, 0, 0, 0}};
  int res = s21_add(value_1, value_2, &value_1);
  ck_assert_int_eq(s21_is_equal(value_1, result), 1);
  ck_assert_int_eq(res, 0);

  // 2 case корректность сложения между bits
  value_1.bits[0] = UINT32_MAX;
  value_2.bits[0] = 1;
  result.bits[1] = 1;
  res = s21_add(value_1, value_2, &value_1);
  ck_assert_int_eq(s21_is_equal(value_1, result), 1);
  ck_assert_int_eq(res, 0);

  toZero(&value_1);
  toZero(&value_2);
  toZero(&result);

  // 3 case - +
  value_1.bits[0] = UINT32_MAX, value_1.bits[3] = 1u << 31;
  value_2.bits[1] = 1;
  result.bits[0] = 1;
  res = s21_add(value_1, value_2, &value_1);
  ck_assert_int_eq(s21_is_equal(value_1, result), 1);
  ck_assert_int_eq(res, 0);

  toZero(&value_1);
  toZero(&value_2);
  toZero(&result);

  // 4 case + -
  value_1.bits[1] = UINT32_MAX;
  value_2.bits[2] = 1, value_2.bits[3] = 1u << 31;
  result.bits[1] = 1, result.bits[3] = 1u << 31;
  res = s21_add(value_1, value_2, &value_1);
  ck_assert_int_eq(s21_is_equal(value_1, result), 1);
  ck_assert_int_eq(res, 0);

  toZero(&value_1);
  toZero(&value_2);
  toZero(&result);

  // 5 case - -
  value_1.bits[0] = UINT32_MAX, value_1.bits[3] = 1u << 31;
  value_2.bits[2] = 1, value_2.bits[3] = 1u << 31;
  result.bits[0] = UINT32_MAX, result.bits[2] = 1, result.bits[3] = 1u << 31;
  res = s21_add(value_1, value_2, &value_1);
  ck_assert_int_eq(s21_is_equal(value_1, result), 1);
  ck_assert_int_eq(res, 0);

  toZero(&value_1);
  toZero(&value_2);
  toZero(&result);

  // 6 со скейлом
  value_1.bits[0] = 1, value_1.bits[3] = 3u << 16;
  value_2.bits[0] = 1;
  result.bits[0] = 1001, result.bits[3] = 3u << 16;
  res = s21_add(value_1, value_2, &value_1);
  ck_assert_int_eq(s21_is_equal(value_1, result), 1);
  ck_assert_int_eq(res, 0);

  toZero(&value_1);
  toZero(&value_2);
  toZero(&result);

  // 7 case - + со скейлом
  value_1.bits[1] = 1, value_1.bits[3] = (1u << 31) + (1u << 16);
  value_2.bits[1] = 1, value_2.bits[3] = 5u << 16;
  result.bits[1] = 9999, result.bits[3] = (1u << 31) + (5u << 16);
  res = s21_add(value_1, value_2, &value_1);
  ck_assert_int_eq(s21_is_equal(value_1, result), 1);
  ck_assert_int_eq(res, 0);

  toZero(&value_1);
  toZero(&value_2);
  toZero(&result);

  // 8 case + -
  value_1.bits[1] = 1, value_1.bits[3] = 1u << 16;
  value_2.bits[1] = 1, value_2.bits[3] = (1u << 31) + (5u << 16);
  result.bits[1] = 9999, result.bits[3] = 5u << 16;
  res = s21_add(value_1, value_2, &value_1);
  ck_assert_int_eq(s21_is_equal(value_1, result), 1);
  ck_assert_int_eq(res, 0);

  toZero(&value_1);
  toZero(&value_2);
  toZero(&result);

  // 9 case - -
  value_1.bits[0] = UINT32_MAX, value_1.bits[1] = 1,
  value_1.bits[3] = (1u << 31) + (28u << 16);
  value_2.bits[1] = 1, value_2.bits[2] = 1,
  value_2.bits[3] = (1u << 31) + (25u << 16);
  result.bits[0] = UINT32_MAX, result.bits[1] = 1001, result.bits[2] = 1000,
  result.bits[3] = (1u << 31) + (28u << 16);
  res = s21_add(value_1, value_2, &value_1);
  ck_assert_int_eq(s21_is_equal(value_1, result), 1);
  ck_assert_int_eq(res, 0);

  toZero(&value_1);
  toZero(&value_2);
  toZero(&result);
}
END_TEST

START_TEST(add_onemore) {
  s21_decimal value_11 = {{1, 1, 1, 65536}}, value_21 = {{1, 1, 1, 65536}};
  s21_decimal res_1 = {{2, 2, 2, 65536}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_add(value_11, value_21, &result);
  ck_assert_int_eq(s21_is_equal(res_1, result), 1);
  ck_assert_int_eq(res, 0);
  toZero(&result);

  s21_decimal value_12 = {{1, 1, 1, 65536}}, value_22 = {{1, 1, 1, 655360}};
  s21_decimal res_2 = {{1000000001, 1000000001, 1000000001, 655360}};
  res = s21_add(value_12, value_22, &result);
  ck_assert_int_eq(s21_is_equal(res_2, result), 1);
  ck_assert_int_eq(res, 0);
  toZero(&result);

  s21_decimal value_13 = {{24, 1, 1, -2147418112}},
              value_23 = {{1, 15, 1, 655360}};
  s21_decimal res_3 = {{-1769803777, 999999990, 999999999, -2146828288}};
  res = s21_add(value_13, value_23, &result);
  ck_assert_int_eq(s21_is_equal(res_3, result), 1);
  ck_assert_int_eq(res, 0);
  toZero(&result);

  s21_decimal value_14 = {{1, 1, 1, 655360}},
              value_24 = {{1, 1, 1, -2147418112}};
  s21_decimal res_4 = {{999999999, 999999999, 999999999, -2146828288}};
  res = s21_add(value_14, value_24, &result);
  ck_assert_int_eq(s21_is_equal(res_4, result), 1);
  ck_assert_int_eq(res, 0);
  toZero(&result);
}
END_TEST

START_TEST(add_1) {
  s21_decimal value_1 = {{UINT32_MAX, UINT32_MAX, UINT32_MAX, 0}},
              value_2 = {{1, 0, 0, 0}}, result = {{0, 0, 0, 0}};
  int res = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(sub_0) {
  // 1 case 0 0
  s21_decimal value_1 = {{0, 0, 0, 0}}, value_2 = {{0, 0, 0, 0}},
              result = {{0, 0, 0, 0}};
  int res = s21_sub(value_1, value_2, &value_1);
  ck_assert_int_eq(s21_is_equal(value_1, result), 1);
  ck_assert_int_eq(res, 0);

  // 2 case корректность вычитания между bits
  value_1.bits[2] = 1;
  value_2.bits[0] = 1;
  result.bits[0] = UINT32_MAX, result.bits[1] = UINT32_MAX;
  res = s21_sub(value_1, value_2, &value_1);
  ck_assert_int_eq(s21_is_equal(value_1, result), 1);
  ck_assert_int_eq(res, 0);

  toZero(&value_1);
  toZero(&value_2);
  toZero(&result);

  // 3 case - +
  value_1.bits[0] = UINT32_MAX, value_1.bits[1] = UINT32_MAX,
  value_1.bits[3] = 1u << 31;
  value_2.bits[0] = 1;
  result.bits[2] = 1, result.bits[3] = 1u << 31;
  res = s21_sub(value_1, value_2, &value_1);
  ck_assert_int_eq(s21_is_equal(value_1, result), 1);
  ck_assert_int_eq(res, 0);

  toZero(&value_1);
  toZero(&value_2);
  toZero(&result);

  // 4 case + -
  value_1.bits[1] = UINT32_MAX;
  value_2.bits[1] = 1, value_2.bits[3] = 1u << 31;
  result.bits[2] = 1;
  res = s21_sub(value_1, value_2, &value_1);
  ck_assert_int_eq(s21_is_equal(value_1, result), 1);
  ck_assert_int_eq(res, 0);

  toZero(&value_1);
  toZero(&value_2);
  toZero(&result);

  // 5 case - -
  value_1.bits[0] = UINT32_MAX, value_1.bits[3] = 1u << 31;
  value_2.bits[2] = 1, value_2.bits[3] = 1u << 31;
  result.bits[0] = 1, result.bits[1] = UINT32_MAX;
  res = s21_sub(value_1, value_2, &value_1);
  ck_assert_int_eq(s21_is_equal(value_1, result), 1);
  ck_assert_int_eq(res, 0);

  toZero(&value_1);
  toZero(&value_2);
  toZero(&result);

  // 6 со скейлом
  value_1.bits[0] = 1, value_1.bits[3] = 3u << 16;
  value_2.bits[0] = 1;
  result.bits[0] = 999, result.bits[3] = (1u << 31) + (3u << 16);
  res = s21_sub(value_1, value_2, &value_1);
  ck_assert_int_eq(s21_is_equal(value_1, result), 1);
  ck_assert_int_eq(res, 0);

  toZero(&value_1);
  toZero(&value_2);
  toZero(&result);

  // 7 case - + со скейлом
  value_1.bits[1] = 1, value_1.bits[3] = (1u << 31) + (1u << 16);
  value_2.bits[1] = 1, value_2.bits[3] = 5u << 16;
  result.bits[1] = 10001, result.bits[3] = (1u << 31) + (5u << 16);
  res = s21_sub(value_1, value_2, &value_1);
  ck_assert_int_eq(s21_is_equal(value_1, result), 1);
  ck_assert_int_eq(res, 0);

  toZero(&value_1);
  toZero(&value_2);
  toZero(&result);

  // 8 case + -
  value_1.bits[1] = 1, value_1.bits[3] = 1u << 16;
  value_2.bits[1] = 1, value_2.bits[3] = (1u << 31) + (5u << 16);
  result.bits[1] = 10001, result.bits[3] = 5u << 16;
  res = s21_sub(value_1, value_2, &value_1);
  ck_assert_int_eq(s21_is_equal(value_1, result), 1);
  ck_assert_int_eq(res, 0);

  toZero(&value_1);
  toZero(&value_2);
  toZero(&result);

  // 9 case - -
  value_1.bits[0] = UINT32_MAX, value_1.bits[1] = 1,
  value_1.bits[3] = (1u << 31) + (28u << 16);
  value_2.bits[1] = 1, value_2.bits[2] = 1,
  value_2.bits[3] = (1u << 31) + (25u << 16);
  result.bits[0] = 1, result.bits[1] = 998, result.bits[2] = 1000,
  result.bits[3] = 28u << 16;
  res = s21_sub(value_1, value_2, &value_1);
  ck_assert_int_eq(s21_is_equal(value_1, result), 1);
  ck_assert_int_eq(res, 0);

  toZero(&value_1);
  toZero(&value_2);
  toZero(&result);
}
END_TEST

START_TEST(negate_onemore) {
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal num_1 = {{32444, 100, 343253, -2145976320}};
  s21_decimal num_2 = {{1, 1, 1, 917504}};
  s21_decimal num_3 = {{0, 0, 0, 917504}};
  s21_decimal res_num_1 = {{32444, 100, 343253, 1507328}};
  s21_decimal res_num_2 = {{1, 1, 1, -2146566144}};
  s21_decimal res_num_3 = {{0, 0, 0, -2146566144}};
  int res = s21_negate(num_1, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(res_num_1, result), 1);

  toZero(&result);

  res = s21_negate(num_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(res_num_2, result), 1);

  toZero(&result);

  res = s21_negate(num_3, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(res_num_3, result), 1);
}
END_TEST

START_TEST(sub_onemore) {
  s21_decimal value_11 = {{1, 1, 1, 1u << 16}},
              value_21 = {{1, 1, 1, (1u << 31) + (1u << 16)}};
  s21_decimal res_1 = {{2, 2, 2, 1u << 16}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_sub(value_11, value_21, &result);
  ck_assert_int_eq(s21_is_equal(res_1, result), 1);
  ck_assert_int_eq(res, 0);
  toZero(&result);

  s21_decimal value_12 = {{1, 1, 1, (1u << 31) + (1u << 16)}},
              value_22 = {{1, 1, 1, (1u << 31) + (1u << 16)}};
  s21_decimal res_2 = {{0, 0, 0, (1u << 31) + (1u << 16)}};
  res = s21_sub(value_12, value_22, &result);
  ck_assert_int_eq(s21_is_equal(res_2, result), 1);
  ck_assert_int_eq(res, 0);
  toZero(&result);

  s21_decimal value_13 = {{1, 1, 1, (1u << 31) + (1u << 16)}},
              value_23 = {{1, 15, 1, 1u << 16}};
  s21_decimal res_3 = {{2, 16, 2, (1u << 31) + (1u << 16)}};
  res = s21_sub(value_13, value_23, &result);
  ck_assert_int_eq(s21_is_equal(res_3, result), 1);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(sub_1) {
  s21_decimal value_1 = {{UINT32_MAX, UINT32_MAX, UINT32_MAX, 1u << 31}},
              value_2 = {{1, 0, 0, 0}}, result = {{0, 0, 0, 0}};
  int res = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(res, 2);
}
END_TEST

START_TEST(mul_0) {
  // 1 case 0 0
  s21_decimal value_1 = {{0, 0, 0, 0}}, value_2 = {{0, 0, 0, 0}},
              result = {{0, 0, 0, 0}};
  int res = s21_mul(value_1, value_2, &value_1);
  ck_assert_int_eq(s21_is_equal(value_1, result), 1);
  ck_assert_int_eq(res, 0);

  // 2 case умножение на 1 + отрицательное число
  value_1.bits[0] = 1, value_1.bits[3] = 1u << 31;
  value_2.bits[0] = 1;
  result.bits[0] = 1, result.bits[3] = 1u << 31;
  res = s21_mul(value_1, value_2, &value_1);
  ck_assert_int_eq(s21_is_equal(value_1, result), 1);
  ck_assert_int_eq(res, 0);

  toZero(&value_1);
  toZero(&value_2);
  toZero(&result);

  // 3 case корректность умножения между bits
  value_1.bits[0] = 8, value_1.bits[3] = 1u << 31;
  value_2.bits[2] = 10, value_2.bits[3] = 1u << 31, value_2.bits[1] = 100;
  res = s21_mul(value_1, value_2, &value_1);
  for (int i = 0; i < 3; ++i) leftShift(&value_2);
  deciCopy(value_2, &result);
  setSign(&result, 0);
  ck_assert_int_eq(s21_is_equal(value_1, result), 1);
  ck_assert_int_eq(res, 0);

  toZero(&value_1);
  toZero(&value_2);
  toZero(&result);

  // 4 case скейл
  value_1.bits[0] = 258377, value_1.bits[3] = (1u << 31) + (3u << 16);
  value_2.bits[0] = 111567891, value_2.bits[3] = 6u << 16;
  result.bits[0] = 3051449451u, result.bits[1] = 6711,
  result.bits[3] = (1u << 31) + (9u << 16);
  res = s21_mul(value_1, value_2, &value_1);
  ck_assert_int_eq(s21_is_equal(value_1, result), 1);
  ck_assert_int_eq(res, 0);

  toZero(&value_1);
  toZero(&value_2);
  toZero(&result);
}
END_TEST

START_TEST(truncate_dec) {
  // 1 case
  s21_decimal value_1 = {{0, 0, 0, 0}}, result = {0}, check = {{2, 0, 0, 0}};
  int res = s21_truncate(value_1, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(value_1, result), 1);

  toZero(&value_1);
  toZero(&result);

  // 2 case. decimal = 2,1. res: 2,1 = 2.0
  value_1.bits[0] = 21, value_1.bits[3] = 1 << 16;
  res = s21_truncate(value_1, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(check, result), 1);

  toZero(&value_1);
  toZero(&result);

  // 3 case. decimal = 2,5. res: 2,5 = 2.0
  value_1.bits[0] = 25, value_1.bits[3] = 1 << 16;
  res = s21_truncate(value_1, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(check, result), 1);

  toZero(&value_1);
  toZero(&result);

  // 4 case. decimal = 2,9. res: 2,9 = 2.0
  value_1.bits[0] = 29, value_1.bits[3] = 1 << 16;
  res = s21_truncate(value_1, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(check, result), 1);

  toZero(&value_1);
  toZero(&result);

  setSign(&check, 1);

  // 5 case. decimal = -2,1. res: -2,1 = -2.0
  value_1.bits[0] = 21, value_1.bits[3] = (1u << 31) + (1 << 16);
  res = s21_truncate(value_1, &result);
  printf("\n");
  ck_assert_int_eq(s21_is_equal(check, result), 1);

  toZero(&value_1);
  toZero(&result);

  // 6 case. decimal = -2,5. res: -2,5 = -2.0
  value_1.bits[0] = 25, value_1.bits[3] = (1u << 31) + (1 << 16);
  res = s21_truncate(value_1, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(result, check), 1);

  toZero(&value_1);
  toZero(&result);

  // 7 case. decimal = -2,9. res: -2,9 = -2.0
  value_1.bits[0] = 29, value_1.bits[3] = (1u << 31) + (1 << 16);
  res = s21_truncate(value_1, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(check, result), 1);
}

END_TEST

START_TEST(truncate_onemore) {
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal num_1 = {{32444, 100, 343253, -2145976320}};
  s21_decimal num_2 = {{1, 1, 1, 917504}};
  s21_decimal num_3 = {{0, 0, 0, 851968}};
  s21_decimal res_num_1 = {{63, 0, 0, -2147483648}};
  s21_decimal res_num_2 = {{184467, 0, 0, 0}};
  s21_decimal res_num_3 = {{0, 0, 0, 0}};
  int res = s21_truncate(num_1, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(res_num_1, result), 1);

  toZero(&result);

  res = s21_truncate(num_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(res_num_2, result), 1);

  toZero(&result);

  res = s21_truncate(num_3, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(res_num_3, result), 1);
}
END_TEST

START_TEST(floor_dec) {
  //   1 case
  s21_decimal value_1 = {{0, 0, 0, 0}}, result = {0}, check = {{2, 0, 0, 0}};
  int res = s21_floor(value_1, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(value_1, result), 1);

  toZero(&value_1);
  toZero(&result);

  // 2 case. decimal = 2,1. res: 2,1 = 2.0
  value_1.bits[0] = 21, value_1.bits[3] = 1 << 16;
  res = s21_floor(value_1, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(check, result), 1);

  toZero(&value_1);
  toZero(&result);

  // 3 case. decimal = 2,5. res: 2,5 = 2.0
  value_1.bits[0] = 25, value_1.bits[3] = 1 << 16;
  res = s21_floor(value_1, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(check, result), 1);

  toZero(&value_1);
  toZero(&result);

  // 4 case. decimal = 2,9. res: 2,9 = 2.0
  value_1.bits[0] = 29, value_1.bits[3] = 1 << 16;
  res = s21_floor(value_1, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(check, result), 1);

  toZero(&value_1);
  toZero(&result);

  check.bits[0] = 3;
  s21_negate(check, &check);

  // 5 case. decimal = -2,1. res: -2,1 = -3.0
  value_1.bits[0] = 21, value_1.bits[3] = (1u << 31) + (1 << 16);
  res = s21_floor(value_1, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(check, result), 1);

  toZero(&value_1);
  toZero(&result);

  // 6 case. decimal = -2,5. res: -2,5 = -3.0
  value_1.bits[0] = 25, value_1.bits[3] = (1u << 31) + (1 << 16);
  res = s21_floor(value_1, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(check, result), 1);

  toZero(&value_1);
  toZero(&result);

  // 7 case. decimal = -2,9. res: -2,9 = -3.0
  value_1.bits[0] = 29, value_1.bits[3] = (1u << 31) + (1 << 16);
  res = s21_floor(value_1, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(check, result), 1);
}

END_TEST

START_TEST(floor_onemore) {
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal num_1 = {{1, 2, 3, 917504}};
  s21_decimal num_2 = {{1, 2, 3, -2146566144}};
  s21_decimal num_3 = {{33, 2, 3000, -2146107392}};
  s21_decimal res_num_1 = {{553402, 0, 0, 0}};
  s21_decimal res_num_2 = {{553403, 0, 0, -2147483648}};
  s21_decimal res_num_3 = {{56, 0, 0, -2147483648}};
  int res = s21_floor(num_1, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(res_num_1, result), 1);

  toZero(&result);

  res = s21_floor(num_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(res_num_2, result), 1);

  toZero(&result);

  res = s21_floor(num_3, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(res_num_3, result), 1);
}
END_TEST

START_TEST(negate_dec) {
  // 1 case
  s21_decimal value_1 = {{0, 0, 0, 0}}, value_2 = {{0, 0, 0, 1u << 31}},
              result = {0};
  int res = s21_negate(value_1, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(value_2, result), 1);

  // 2 case
  res = s21_negate(result, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(value_1, result), 1);

  toZero(&value_1);
  toZero(&value_2);

  // 3 case
  value_1.bits[0] = 1;
  value_2.bits[0] = 1, value_2.bits[3] = 1u << 31;
  res = s21_negate(value_1, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(value_2, result), 1);

  toZero(&value_1);
  toZero(&value_2);

  // 4 case
  value_1.bits[2] = 1, value_1.bits[3] = 5 << 16;
  value_2.bits[2] = 1, value_2.bits[3] = (1u << 31) + (5 << 16);
  res = s21_negate(value_1, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(value_2, result), 1);

  // 5 case
  res = s21_negate(value_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(value_1, result), 1);
}

END_TEST

START_TEST(mul_onemore) {
  s21_decimal value_11 = {{1, 1, 1, 10u << 16}},
              value_21 = {{1, 1, 1, 10u << 16}};
  s21_decimal res_1 = {{1, 2, 3, 20u << 16}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_mul(value_11, value_21, &result);
  ck_assert_int_eq(s21_is_equal(res_1, result), 1);
  ck_assert_int_eq(res, 0);
  toZero(&result);

  s21_decimal value_12 = {{1, 0, 0, 655360}}, value_22 = {{1, 0, 0, 655360}};
  s21_decimal res_2 = {{1, 0, 0, 1310720}};
  res = s21_mul(value_12, value_22, &result);
  ck_assert_int_eq(s21_is_equal(res_2, result), 1);
  ck_assert_int_eq(res, 0);
  toZero(&result);

  s21_decimal value_13 = {{1, 0, 0, 655360}},
              value_23 = {{1, 0, 0, -2146828288}};
  s21_decimal res_3 = {{1, 0, 0, -2146172928}};
  res = s21_mul(value_13, value_23, &result);
  ck_assert_int_eq(s21_is_equal(res_3, result), 1);
  ck_assert_int_eq(res, 0);

  s21_decimal value_14 = {{1, 0, 1, 917504}},
              value_24 = {{1, 0, 0, -2146828288}};
  s21_decimal res_4 = {{1, 0, 1, -2145910784}};
  res = s21_mul(value_14, value_24, &result);
  ck_assert_int_eq(s21_is_equal(res_4, result), 1);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(div_onemore) {
  s21_decimal value_01 = {{1, 0, 0, 0}},
              value_02 = {{UINT32_MAX, UINT32_MAX, UINT32_MAX, 0}};
  s21_decimal res_0 = {{0, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_div(value_01, value_02, &result);
  ck_assert_int_eq(s21_is_equal(res_0, result), 1);
  ck_assert_int_eq(res, 2);

  s21_decimal value_11 = {{1, 1, 1, 655360}}, value_21 = {{1, 1, 1, 655360}};
  s21_decimal res_1 = {{1, 0, 0, 0}};
  res = s21_div(value_11, value_21, &result);
  ck_assert_int_eq(s21_is_equal(res_1, result), 1);
  ck_assert_int_eq(res, 0);
  toZero(&result);

  s21_decimal value_12 = {{1, 1, 1, 10u << 16}},
              value_22 = {{1, 1, 1, 15u << 16}};
  s21_decimal res_2 = {{32, 0, 0, 0}};
  res = s21_div(value_12, value_22, &result);
  ck_assert_int_eq(s21_is_equal(res_2, result), 1);
  ck_assert_int_eq(res, 0);
  toZero(&result);

  s21_decimal value_13 = {{1, 1, 1, 0}}, value_23 = {{0, 0, 0, 0}};
  s21_decimal res_3 = {{0, 0, 0, 0}};
  res = s21_div(value_13, value_23, &res_3);
  ck_assert_int_eq(res, 3);

  s21_decimal value_14 = {{1, 1, 1, (15u << 16)}},
              value_24 = {{1, 1, 1, (1u << 31) + (15u << 16)}};
  s21_decimal res_4 = {{1, 0, 0, 1u << 31}};
  res = s21_div(value_14, value_24, &result);
  ck_assert_int_eq(s21_is_equal(res_4, result), 1);
  ck_assert_int_eq(res, 0);

  toZero(&result);

  s21_decimal value_15 = {{1, 0, 0, (15u << 16)}},
              value_25 = {{2, 0, 0, (1u << 31) + (15u << 16)}};
  s21_decimal res_5 = {{1, 0, 0, (1u << 31) + (1u << 16)}};
  s21_decimal res5;
  res = s21_div(value_15, value_25, &res5);
  ck_assert_int_eq(s21_is_equal(res_5, res5), 1);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(to_float_1) {
  float res;
  s21_decimal dec_num = {{123456, 0, 0, 1 << 16}};
  float float_num = 12345.6;
  int err = s21_from_decimal_to_float(dec_num, &res);
  ck_assert_float_eq(res, float_num);
  ck_assert_int_eq(err, 0);
}
END_TEST

START_TEST(to_float_2) {
  float res;
  s21_decimal dec_num = {{12345, 0, 0, (1 << 17) + (1 << 16)}};
  float float_num = 12.345;
  int err = s21_from_decimal_to_float(dec_num, &res);
  ck_assert_float_eq(res, float_num);
  ck_assert_int_eq(err, 0);
}
END_TEST

START_TEST(to_float_less_than0) {
  float res;
  s21_decimal dec_num = {{123456, 0, 0, (1u << 31) + (1 << 17)}};
  float float_num = -1234.56;
  int err = s21_from_decimal_to_float(dec_num, &res);
  ck_assert_float_eq(res, float_num);
  ck_assert_int_eq(err, 0);
}
END_TEST

START_TEST(to_float_less_than0_1) {
  float res;
  s21_decimal dec_num = {{56, 0, 0, (1u << 31) + (1 << 17)}};
  float float_num = -0.56;
  int err = s21_from_decimal_to_float(dec_num, &res);
  ck_assert_float_eq(res, float_num);
  ck_assert_int_eq(err, 0);
}
END_TEST

START_TEST(to_float_min) {
  float res;
  s21_decimal dec_num = {{1, 0, 0, (1u << 31) + 1835008}};
  float float_num = -1e-28;
  int err = s21_from_decimal_to_float(dec_num, &res);
  ck_assert_float_eq(res, float_num);
  ck_assert_int_eq(err, 0);
}
END_TEST

START_TEST(to_float_error) {
  float res;
  s21_decimal dec_num = {{4294967295, 4294967295, 4294967295, 1 << 16}};
  float float_num = 7922816251426433759354395033.5;
  int err = s21_from_decimal_to_float(dec_num, &res);
  ck_assert_float_eq(res, float_num);
  ck_assert_int_eq(err, 0);
}
END_TEST

START_TEST(to_float_degree_28) {
  float res;
  s21_decimal dec_num = {{858993460, 858993459, 3543348019, 28u << 16}};
  float float_num = 6.53632;
  int err = s21_from_decimal_to_float(dec_num, &res);
  ck_assert_float_eq_tol(res, float_num, 0.00001);
  ck_assert_int_eq(err, 0);
}
END_TEST

START_TEST(from_int) {
  s21_decimal res;
  s21_decimal dec_num = {{56, 0, 0, 0}};
  int int_num = 56;
  int err = s21_from_int_to_decimal(int_num, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(dec_num.bits[0], res.bits[0]);
  ck_assert_int_eq(dec_num.bits[1], res.bits[1]);
  ck_assert_int_eq(dec_num.bits[2], res.bits[2]);
  ck_assert_int_eq(dec_num.bits[3], res.bits[3]);
}
END_TEST

START_TEST(from_int_less_than_0) {
  s21_decimal res;
  s21_decimal dec_num = {{56, 0, 0, 1u << 31}};
  int int_num = -56;
  int err = s21_from_int_to_decimal(int_num, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(dec_num.bits[0], res.bits[0]);
  ck_assert_int_eq(dec_num.bits[1], res.bits[1]);
  ck_assert_int_eq(dec_num.bits[2], res.bits[2]);
  ck_assert_int_eq(dec_num.bits[3], res.bits[3]);
}
END_TEST

START_TEST(from_int_max) {
  s21_decimal res;
  s21_decimal dec_num = {{2147483647, 0, 0, 0}};
  int int_num = 2147483647;
  int err = s21_from_int_to_decimal(int_num, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(dec_num.bits[0], res.bits[0]);
  ck_assert_int_eq(dec_num.bits[1], res.bits[1]);
  ck_assert_int_eq(dec_num.bits[2], res.bits[2]);
  ck_assert_int_eq(dec_num.bits[3], res.bits[3]);
}
END_TEST

START_TEST(from_int_min) {
  s21_decimal res;
  s21_decimal dec_num = {{2147483648, 0, 0, 1u << 31}};
  int int_num = -2147483648;
  int err = s21_from_int_to_decimal(int_num, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(dec_num.bits[0], res.bits[0]);
  ck_assert_int_eq(dec_num.bits[1], res.bits[1]);
  ck_assert_int_eq(dec_num.bits[2], res.bits[2]);
  ck_assert_int_eq(dec_num.bits[3], res.bits[3]);
}
END_TEST

START_TEST(from_int_0) {
  s21_decimal res;
  s21_decimal dec_num = {{0, 0, 0, 0}};
  int int_num = 0;
  int err = s21_from_int_to_decimal(int_num, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(dec_num.bits[0], res.bits[0]);
  ck_assert_int_eq(dec_num.bits[1], res.bits[1]);
  ck_assert_int_eq(dec_num.bits[2], res.bits[2]);
  ck_assert_int_eq(dec_num.bits[3], res.bits[3]);
}
END_TEST

START_TEST(to_int_error) {
  int res;
  s21_decimal dec_num = {{0, 1, 0, 0}};
  int int_num = 0;
  int err = s21_from_decimal_to_int(dec_num, &res);
  ck_assert_int_eq(res, int_num);
  ck_assert_int_eq(err, 1);
}
END_TEST

START_TEST(to_int_0) {
  int res;
  s21_decimal dec_num = {{0, 0, 0, 0}};
  int int_num = 0;
  int err = s21_from_decimal_to_int(dec_num, &res);
  ck_assert_int_eq(res, int_num);
  ck_assert_int_eq(err, 0);
}
END_TEST

START_TEST(to_int_less_than0) {
  int res;
  s21_decimal dec_num = {{1, 0, 0, 1u << 31}};
  int int_num = -1;
  int err = s21_from_decimal_to_int(dec_num, &res);
  ck_assert_int_eq(res, int_num);
  ck_assert_int_eq(err, 0);
}
END_TEST

START_TEST(to_int_max) {
  int res;
  s21_decimal dec_num = {{2147483647, 0, 0, 0}};
  int int_num = 2147483647;
  int err = s21_from_decimal_to_int(dec_num, &res);
  ck_assert_int_eq(res, int_num);
  ck_assert_int_eq(err, 0);
}
END_TEST

START_TEST(to_int_min) {
  int res;
  s21_decimal dec_num = {{2147483648, 0, 0, 1u << 31}};
  int int_num = -2147483648;
  int err = s21_from_decimal_to_int(dec_num, &res);
  ck_assert_int_eq(res, int_num);
  ck_assert_int_eq(err, 0);
}
END_TEST

START_TEST(to_int_with_degree) {
  int res;
  s21_decimal dec_num = {{123, 0, 0, 1 << 16}};
  int int_num = 12;
  int err = s21_from_decimal_to_int(dec_num, &res);
  ck_assert_int_eq(res, int_num);
  ck_assert_int_eq(err, 0);
}
END_TEST

START_TEST(to_int_with_degree_1) {
  int res;
  s21_decimal dec_num = {{123499, 0, 0, 1 << 17}};
  int int_num = 1234;
  int err = s21_from_decimal_to_int(dec_num, &res);
  ck_assert_int_eq(res, int_num);
  ck_assert_int_eq(err, 0);
}
END_TEST

START_TEST(to_int_with_degree_2) {
  int res;
  s21_decimal dec_num = {{123456, 0, 0, 1 << 18}};
  int int_num = 12;
  int err = s21_from_decimal_to_int(dec_num, &res);
  ck_assert_int_eq(res, int_num);
  ck_assert_int_eq(err, 0);
}
END_TEST

START_TEST(to_int_with_degree_3) {
  int res;
  s21_decimal dec_num = {{123456, 0, 0, 1 << 20}};
  int int_num = 0;
  int err = s21_from_decimal_to_int(dec_num, &res);
  ck_assert_int_eq(res, int_num);
  ck_assert_int_eq(err, 0);
}
END_TEST

START_TEST(from_float_0) {
  s21_decimal res;
  s21_decimal dec_num = {{0, 0, 0, 0}};
  float float_num = -0;
  int err = s21_from_float_to_decimal(float_num, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(dec_num.bits[0], res.bits[0]);
  ck_assert_int_eq(dec_num.bits[1], res.bits[1]);
  ck_assert_int_eq(dec_num.bits[2], res.bits[2]);
  ck_assert_int_eq(dec_num.bits[3], res.bits[3]);
}
END_TEST

START_TEST(from_float_1) {
  s21_decimal res;
  s21_decimal dec_num = {{56, 0, 0, (1u << 31) + (1 << 17)}};
  float float_num = -0.56;
  int err = s21_from_float_to_decimal(float_num, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(dec_num.bits[0], res.bits[0]);
  ck_assert_int_eq(dec_num.bits[1], res.bits[1]);
  ck_assert_int_eq(dec_num.bits[2], res.bits[2]);
  ck_assert_int_eq(dec_num.bits[3], res.bits[3]);
}
END_TEST

START_TEST(from_float_2) {
  s21_decimal res;
  s21_decimal dec_num = {{123444, 0, 0, 1u << 16}};
  float float_num = 12344.4;
  int err = s21_from_float_to_decimal(float_num, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(dec_num.bits[0], res.bits[0]);
  ck_assert_int_eq(dec_num.bits[1], res.bits[1]);
  ck_assert_int_eq(dec_num.bits[2], res.bits[2]);
  ck_assert_int_eq(dec_num.bits[3], res.bits[3]);
}
END_TEST

START_TEST(from_float_when_no_f_part) {
  s21_decimal res;
  s21_decimal dec_num = {{56, 0, 0, 1u << 31}};
  float float_num = -56;
  int err = s21_from_float_to_decimal(float_num, &res);
  ck_assert_int_eq(err, 0);
  ck_assert_int_eq(dec_num.bits[0], res.bits[0]);
  ck_assert_int_eq(dec_num.bits[1], res.bits[1]);
  ck_assert_int_eq(dec_num.bits[2], res.bits[2]);
  ck_assert_int_eq(dec_num.bits[3], res.bits[3]);
}
END_TEST

START_TEST(from_float_less_than_min) {
  s21_decimal res;
  s21_decimal dec_num = {{0, 0, 0, 0}};
  float float_num = 1e-30;
  int err = s21_from_float_to_decimal(float_num, &res);
  ck_assert_int_eq(err, 1);
  ck_assert_int_eq(dec_num.bits[0], res.bits[0]);
  ck_assert_int_eq(dec_num.bits[1], res.bits[1]);
  ck_assert_int_eq(dec_num.bits[2], res.bits[2]);
  ck_assert_int_eq(dec_num.bits[3], res.bits[3]);
}
END_TEST

START_TEST(from_float_greater_than_max) {
  s21_decimal res;
  s21_decimal dec_num = {{0, 0, 0, 0}};
  float float_num = 1e-30;
  int err = s21_from_float_to_decimal(float_num, &res);
  ck_assert_int_eq(err, 1);
  ck_assert_int_eq(dec_num.bits[0], res.bits[0]);
  ck_assert_int_eq(dec_num.bits[1], res.bits[1]);
  ck_assert_int_eq(dec_num.bits[2], res.bits[2]);
  ck_assert_int_eq(dec_num.bits[3], res.bits[3]);
}
END_TEST

START_TEST(mod_onemore) {
  s21_decimal value_11 = {{1, 1, 1, 1u << 16}},
              value_21 = {{1, 1, 1, (1u << 31) + (1u << 16)}};
  s21_decimal res_1 = {{0, 0, 0, 1u << 31}};
  s21_decimal result = {{0, 0, 0, 0}};
  int res = s21_mod(value_11, value_21, &result);
  ck_assert_int_eq(s21_is_equal(res_1, result), 1);
  ck_assert_int_eq(res, 0);
  toZero(&result);

  s21_decimal value_12 = {{1, 1, 1, (1u << 31) + (1u << 16)}},
              value_22 = {{1, 1, 1, (1u << 31) + (10u << 16)}};
  s21_decimal res_2 = {{0, 0, 0, (1u << 31) + (10u << 16)}};
  res = s21_mod(value_12, value_22, &result);
  ck_assert_int_eq(s21_is_equal(res_2, result), 1);
  ck_assert_int_eq(res, 0);
  toZero(&result);

  s21_decimal value_13 = {{1, 1, 1, 0}}, value_23 = {{0, 0, 0, 0}};
  s21_decimal res_3 = {{0, 0, 0, 0}};
  res = s21_mod(value_13, value_23, &res_3);
  ck_assert_int_eq(res, 3);

  s21_decimal value_14 = {{1, 2, 3, (1u << 31) + (10u << 16)}},
              value_24 = {{3, 2, 1, (1u << 31) + (10u << 16)}};
  s21_decimal res_4 = {{-5, -3, 0, (1u << 31) + (10u << 16)}};
  res = s21_mod(value_14, value_24, &result);
  ck_assert_int_eq(s21_is_equal(res_4, result), 1);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(round_dec) {
  // 1 case
  s21_decimal value_1 = {{0, 0, 0, 0}}, result = {0}, check = {{2, 0, 0, 0}};
  int res = s21_round(value_1, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(value_1, result), 1);

  toZero(&value_1);
  toZero(&result);

  // 2 case. decimal = 2,1. res: 2,1 = 2.0
  value_1.bits[0] = 21, value_1.bits[3] = 1 << 16;
  res = s21_round(value_1, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(check, result), 1);

  toZero(&value_1);
  toZero(&result);

  s21_negate(check, &check);

  // 5 case. decimal = -2,1. res: -2,1 = -2.0
  value_1.bits[0] = 21, value_1.bits[3] = (1u << 31) + (1 << 16);
  res = s21_round(value_1, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(check, result), 1);

  toZero(&value_1);
  toZero(&result);

  check.bits[0] = 3;
}

END_TEST

START_TEST(round_dec_2) {
  s21_decimal value_1 = {{0, 0, 0, 0}}, result = {0},
              check = {{3, 0, 0, 1u << 31}};
  // 6 case. decimal = -2,5. res: -2,5 = -3.0
  value_1.bits[0] = 25, value_1.bits[3] = (1u << 31) + (1u << 16);
  int res = s21_round(value_1, &result);
  ck_assert_int_eq(res, 0);

  ck_assert_int_eq(s21_is_equal(check, result), 1);

  toZero(&value_1);
  toZero(&result);

  // 7 case. decimal = -2,9. res: -2,9 = -3.0
  value_1.bits[0] = 29, value_1.bits[3] = (1u << 31) + (1u << 16);
  res = s21_round(value_1, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(check, result), 1);

  toZero(&value_1);
  toZero(&result);

  s21_negate(check, &check);

  // 3 case. decimal = 2,5. res: 2,5 = 3.0
  value_1.bits[0] = 25, value_1.bits[3] = 1u << 16;
  res = s21_round(value_1, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(check, result), 1);

  toZero(&value_1);
  toZero(&result);

  // 4 case. decimal = 2,9. res: 2,9 = 3.0
  value_1.bits[0] = 29, value_1.bits[3] = 1u << 16;
  res = s21_round(value_1, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(check, result), 1);
}
END_TEST

START_TEST(round_onemore) {
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal num_1 = {{1, 2, 3, 917504}};
  s21_decimal num_2 = {{10, 100, 3, 1376256}};
  s21_decimal num_3 = {{16, 100, 3, 851968}};
  s21_decimal res_num_1 = {{553402, 0, 0, 0}};
  s21_decimal res_num_2 = {{0, 0, 0, 0}};
  s21_decimal res_num_3 = {{5534023, 0, 0, 0}};
  int res = s21_round(num_1, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(res_num_1, result), 1);

  toZero(&result);

  res = s21_round(num_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(res_num_2, result), 1);

  toZero(&result);

  res = s21_round(num_3, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(res_num_3, result), 1);
}
END_TEST

Suite *s21_math_operation(void) {
  Suite *s = suite_create("Siiiiiu");

  TCase *tc_is_less = tcase_create("is_less");
  suite_add_tcase(s, tc_is_less);
  tcase_add_test(tc_is_less, is_less);
  tcase_add_test(tc_is_less, is_less_onemore);

  TCase *tc_is_less_or_equal = tcase_create("is_less_or_equal");
  suite_add_tcase(s, tc_is_less_or_equal);
  tcase_add_test(tc_is_less_or_equal, is_less_or_equal);
  tcase_add_test(tc_is_less_or_equal, is_less_or_equal_onemore);

  TCase *tc_is_greater = tcase_create("is_greater");
  suite_add_tcase(s, tc_is_greater);
  tcase_add_test(tc_is_greater, is_greater);
  tcase_add_test(tc_is_greater, is_greater_onemore);

  TCase *tc_is_greater_or_equal = tcase_create("is_greater_or_equal");
  suite_add_tcase(s, tc_is_greater_or_equal);
  tcase_add_test(tc_is_greater_or_equal, is_greater_or_equal);
  tcase_add_test(tc_is_greater_or_equal, is_greater_or_equal_onemore);

  TCase *tc_is_equal = tcase_create("is_equal");
  suite_add_tcase(s, tc_is_equal);
  tcase_add_test(tc_is_equal, is_equal);
  tcase_add_test(tc_is_equal, is_equal_onemore);

  TCase *tc_is_not_equal = tcase_create("is_not_equal");
  suite_add_tcase(s, tc_is_not_equal);
  tcase_add_test(tc_is_not_equal, is_not_equal);
  tcase_add_test(tc_is_not_equal, is_not_equal_onemore);

  TCase *tc_round_dec = tcase_create("round_dec");
  suite_add_tcase(s, tc_round_dec);
  tcase_add_test(tc_round_dec, round_dec);
  tcase_add_test(tc_round_dec, round_dec_2);
  tcase_add_test(tc_round_dec, round_onemore);

  TCase *tc_floor_dec = tcase_create("floor_dec");
  suite_add_tcase(s, tc_floor_dec);
  tcase_add_test(tc_floor_dec, floor_dec);
  tcase_add_test(tc_floor_dec, floor_onemore);

  TCase *tc_negate_dec = tcase_create("negate_dec");
  suite_add_tcase(s, tc_negate_dec);
  tcase_add_test(tc_negate_dec, negate_dec);
  tcase_add_test(tc_negate_dec, negate_onemore);

  TCase *tc_truncate_dec = tcase_create("truncate_dec");
  suite_add_tcase(s, tc_truncate_dec);
  tcase_add_test(tc_truncate_dec, truncate_dec);
  tcase_add_test(tc_truncate_dec, truncate_onemore);

  TCase *tc_from_float_0 = tcase_create("from_float_0");
  suite_add_tcase(s, tc_from_float_0);
  tcase_add_test(tc_from_float_0, from_float_0);

  TCase *tc_from_float_1 = tcase_create("from_float_1");
  suite_add_tcase(s, tc_from_float_1);
  tcase_add_test(tc_from_float_1, from_float_1);

  TCase *tc_to_float_error = tcase_create("to_float_error");
  suite_add_tcase(s, tc_to_float_error);
  tcase_add_test(tc_to_float_error, to_float_error);

  TCase *tc_from_float_2 = tcase_create("from_float_2");
  suite_add_tcase(s, tc_from_float_2);
  tcase_add_test(tc_from_float_2, from_float_2);

  TCase *tc_from_float_when_no_f_part =
      tcase_create("from_float_when_no_f_part");
  suite_add_tcase(s, tc_from_float_when_no_f_part);
  tcase_add_test(tc_from_float_when_no_f_part, from_float_when_no_f_part);

  TCase *tc_from_float_less_than_min = tcase_create("from_float_less_than_min");
  suite_add_tcase(s, tc_from_float_less_than_min);
  tcase_add_test(tc_from_float_less_than_min, from_float_less_than_min);

  TCase *tc_from_float_greater_than_max =
      tcase_create("from_float_greater_than_max");
  suite_add_tcase(s, tc_from_float_greater_than_max);
  tcase_add_test(tc_from_float_greater_than_max, from_float_greater_than_max);

  TCase *tc_to_int_error = tcase_create("to_int_error");
  suite_add_tcase(s, tc_to_int_error);
  tcase_add_test(tc_to_int_error, to_int_error);

  TCase *tc_to_int_0 = tcase_create("to_int_0");
  suite_add_tcase(s, tc_to_int_0);
  tcase_add_test(tc_to_int_0, to_int_0);

  TCase *tc_to_int_less_than0 = tcase_create("to_int_less_than0");
  suite_add_tcase(s, tc_to_int_less_than0);
  tcase_add_test(tc_to_int_less_than0, to_int_less_than0);

  TCase *tc_to_int_max = tcase_create("to_int_max");
  suite_add_tcase(s, tc_to_int_max);
  tcase_add_test(tc_to_int_max, to_int_max);

  TCase *tc_to_int_min = tcase_create("to_int_min");
  suite_add_tcase(s, tc_to_int_min);
  tcase_add_test(tc_to_int_min, to_int_min);

  TCase *tc_to_int_with_degree = tcase_create("to_int_with_degree");
  suite_add_tcase(s, tc_to_int_with_degree);
  tcase_add_test(tc_to_int_with_degree, to_int_with_degree);

  TCase *tc_to_int_with_degree_1 = tcase_create("to_int_with_degree_1");
  suite_add_tcase(s, tc_to_int_with_degree_1);
  tcase_add_test(tc_to_int_with_degree_1, to_int_with_degree_1);

  TCase *tc_to_int_with_degree_2 = tcase_create("to_int_with_degree_2");
  suite_add_tcase(s, tc_to_int_with_degree_2);
  tcase_add_test(tc_to_int_with_degree_2, to_int_with_degree_2);

  TCase *tc_to_int_with_degree_3 = tcase_create("to_int_with_degree_3");
  suite_add_tcase(s, tc_to_int_with_degree_3);
  tcase_add_test(tc_to_int_with_degree_3, to_int_with_degree_3);

  TCase *tc_from_int = tcase_create("from_int");
  suite_add_tcase(s, tc_from_int);
  tcase_add_test(tc_from_int, from_int);

  TCase *tc_from_int_less_than_0 = tcase_create("from_int_less_than_0");
  suite_add_tcase(s, tc_from_int_less_than_0);
  tcase_add_test(tc_from_int_less_than_0, from_int_less_than_0);

  TCase *tc_from_int_max = tcase_create("from_int_max");
  suite_add_tcase(s, tc_from_int_max);
  tcase_add_test(tc_from_int_max, from_int_max);

  TCase *tc_from_int_min = tcase_create("from_int_min");
  suite_add_tcase(s, tc_from_int_min);
  tcase_add_test(tc_from_int_min, from_int_min);

  TCase *tc_from_int_0 = tcase_create("from_int_0");
  suite_add_tcase(s, tc_from_int_0);
  tcase_add_test(tc_from_int_0, from_int_0);

  TCase *tc_to_float_degree_28 = tcase_create("to_float_degree_28");
  suite_add_tcase(s, tc_to_float_degree_28);
  tcase_add_test(tc_to_float_degree_28, to_float_degree_28);

  TCase *tc_to_float_1 = tcase_create("to_float_1");
  suite_add_tcase(s, tc_to_float_1);
  tcase_add_test(tc_to_float_1, to_float_1);

  TCase *tc_to_float_2 = tcase_create("to_float_2");
  suite_add_tcase(s, tc_to_float_2);
  tcase_add_test(tc_to_float_2, to_float_2);

  TCase *tc_to_float_less_than0 = tcase_create("to_float_less_than0");
  suite_add_tcase(s, tc_to_float_less_than0);
  tcase_add_test(tc_to_float_less_than0, to_float_less_than0);

  TCase *tc_to_float_less_than0_1 = tcase_create("to_float_less_than0_1");
  suite_add_tcase(s, tc_to_float_less_than0_1);
  tcase_add_test(tc_to_float_less_than0_1, to_float_less_than0_1);

  TCase *tc_to_float_min = tcase_create("to_float_min");
  suite_add_tcase(s, tc_to_float_min);
  tcase_add_test(tc_to_float_min, to_float_min);

  TCase *tc_add_0 = tcase_create("add_0");
  suite_add_tcase(s, tc_add_0);
  tcase_add_test(tc_add_0, add_0);
  tcase_add_test(tc_add_0, add_onemore);

  TCase *tc_add_1 = tcase_create("add_1");
  suite_add_tcase(s, tc_add_1);
  tcase_add_test(tc_add_1, add_1);

  TCase *tc_sub_0 = tcase_create("sub_0");
  suite_add_tcase(s, tc_sub_0);
  tcase_add_test(tc_sub_0, sub_0);
  tcase_add_test(tc_sub_0, sub_onemore);

  TCase *tc_sub_1 = tcase_create("sub_1");
  suite_add_tcase(s, tc_sub_1);
  tcase_add_test(tc_sub_1, sub_1);

  TCase *tc_mul_0 = tcase_create("mul_0");
  suite_add_tcase(s, tc_mul_0);
  tcase_add_test(tc_mul_0, mul_0);
  tcase_add_test(tc_mul_0, mul_onemore);

  TCase *tc_div_onemore = tcase_create("div_onemore");
  suite_add_tcase(s, tc_div_onemore);
  tcase_add_test(tc_div_onemore, div_onemore);

  TCase *tc_mod_onemore = tcase_create("mod_onemore");
  suite_add_tcase(s, tc_mod_onemore);
  tcase_add_test(tc_mod_onemore, mod_onemore);

  return s;
}