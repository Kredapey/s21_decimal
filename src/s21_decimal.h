#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define START_LOW 0
#define START_MID 32
#define START_HIGHT 64
#define START_INFO 96

typedef struct s21_decimal {
  unsigned int bits[4];
} s21_decimal;

int getBit(s21_decimal d, int index);
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
void setBit(s21_decimal *d, int index, int bit);
void setSign(s21_decimal *d, int sign);
int getSign(s21_decimal d);
int getScale(s21_decimal d);
void setScale(s21_decimal *d, int scale);
void intToDecimal(s21_decimal *d, unsigned int num);
int decimalToInt(s21_decimal d, int *num);
int s21_sub(s21_decimal a, s21_decimal b, s21_decimal *res);
int s21_add(s21_decimal a, s21_decimal b, s21_decimal *res);
int s21_is_less(s21_decimal a, s21_decimal b);
int s21_is_less_or_equal(s21_decimal a, s21_decimal b);
int s21_is_greater(s21_decimal a, s21_decimal b);
int s21_is_greater_or_equal(s21_decimal a, s21_decimal b);
int s21_is_equal(s21_decimal a, s21_decimal b);
int s21_is_not_equal(s21_decimal a, s21_decimal b);
int workWithScale(s21_decimal *a, s21_decimal *b);
int s21_mul(s21_decimal a, s21_decimal b, s21_decimal *res);
int s21_div(s21_decimal a, s21_decimal b, s21_decimal *res);
void leftShift(s21_decimal *a);
int isZero(s21_decimal a);
void toZero(s21_decimal *a);
void deciCopy(s21_decimal sample, s21_decimal *dest);
void rightShift(s21_decimal *a);
int s21_from_decimal_to_float(s21_decimal src, float *dst);
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int workWithScaleForDiv(s21_decimal *a, s21_decimal *b);
int s21_sub_for_div(s21_decimal a, s21_decimal b, s21_decimal *res);
int s21_truncate(s21_decimal value, s21_decimal *result);
int from_decimal_to_double(s21_decimal src, long double *dest);
int s21_negate(s21_decimal value, s21_decimal *result);
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
void isGreaterLogic(int aSign, int *aMas, int *bMas, int *res, int match,
                    int scaleCounter, int *aFrac, int *bFrac);
void isLessLogic(int aSign, int *aMas, int *bMas, int *res, int match,
                 int scaleCounter, int *aFrac, int *bFrac);
void oneSignForSub(s21_decimal a, s21_decimal b, s21_decimal *decr,
                   s21_decimal *subt, s21_decimal *res, int aIsGreaterThenB,
                   int equal, int *resSign, int aSign, int resScale);
void nullSignForSub(s21_decimal a, s21_decimal b, s21_decimal *decr,
                    s21_decimal *subt, s21_decimal *res, int aIsGreaterThenB,
                    int equal, int *resSign, int aSign, int resScale);
void subtLogic(s21_decimal decr, s21_decimal subt, s21_decimal *res);
void workWithSign(int sign_1, int sign_2, s21_decimal *result,
                  s21_decimal *val_1, s21_decimal *val_2);
void divLogic(int *resScale, int scaleCount, s21_decimal *result,
              s21_decimal *val_1, s21_decimal *val_2, s21_decimal temp);
void isEqualLogic(s21_decimal tempA, s21_decimal tempB, int match, int *res);
void isNotEqualLogic(s21_decimal tempA, s21_decimal tempB, int match, int *res);

#endif  // S21_DECIMAL_H