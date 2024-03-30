#include "s21_decimal.h"

int isZero(s21_decimal a) {
  int res = 0;
  if (a.bits[0] == 0 && a.bits[1] == 0 && a.bits[2] == 0) {
    res = 1;
  }
  return res;
}

void toZero(s21_decimal *a) {
  for (int i = 0; i < 4; i++) {
    a->bits[i] = 0;
  }
}

void deciCopy(s21_decimal sample, s21_decimal *dest) {
  for (int i = 0; i < 128; i++) {
    setBit(dest, i, getBit(sample, i));
  }
}

int s21_mod(s21_decimal a, s21_decimal b, s21_decimal *result) {
  int resScale = 0;
  int sign = 0;
  int error = 0;
  if (isZero(b)) {
    error = 3;
  }
  if (error != 3) {
    int signA = getSign(a);
    int signB = getSign(b);
    if (signA == 1 || signB == 1) {
      sign = 1;
    }
    int scaleA = getScale(a);
    int scaleB = getScale(b);
    if (scaleA != scaleB) {
      resScale = scaleA > scaleB ? scaleA : scaleB;
    } else {
      resScale = scaleA;
    }
    setSign(&a, 0);
    setSign(&b, 0);
    s21_decimal temp = b;
    while (!(s21_is_greater_or_equal(b, a)) && !getBit(b, 95)) {
      leftShift(&b);
    }
    while (s21_is_greater_or_equal(b, temp) ||
           s21_is_greater_or_equal(a, temp)) {
      leftShift(result);
      if (s21_is_greater(b, a)) {
        setBit(result, 0, 0);
      } else {
        s21_sub_for_div(a, b, &a);
        setBit(result, 0, 1);
      }
      rightShift(&b);
    }
    *result = a;
    if (sign == 1) {
      setSign(result, 1);
    } else {
      setSign(result, 0);
    }
    setScale(result, resScale);
  }
  return error;
}

int s21_div(s21_decimal a, s21_decimal b, s21_decimal *result) {
  s21_decimal tempA = a, tempB = b;
  toZero(result);
  int signA = getSign(tempA), signB = getSign(tempB), scale_1 = getScale(tempA),
      scale_2 = getScale(tempB), resScale = 0, scaleCount = 0, counter = 0,
      error = 0;
  if (scale_1 == scale_2) {
    setScale(&tempA, 0);
    setScale(&tempB, 0);
  }
  if (isZero(b)) {
    error = 3;
  }
  if (error != 3) {
    workWithSign(signA, signB, result, &tempA, &tempB);
    if (s21_is_less(tempA, tempB)) {
      scaleCount = 1;
    }
    s21_decimal temp = tempB;
    if (scaleCount == 1) {
      while (s21_is_less(tempA, tempB) && resScale < 28) {
        leftShift(&tempA);
        if (counter != 0) resScale++;
        counter++;
      }
      if (s21_is_less(tempA, tempB) && resScale == 28) {
        error = 2;
      }
    }
    if (error != 2) {
      while (!(s21_is_greater_or_equal(tempB, tempA)) && !getBit(tempB, 95)) {
        leftShift(&tempB);
      }
      divLogic(&resScale, scaleCount, result, &tempA, &tempB, temp);
    }
  }
  if (scaleCount == 1 && error != 2) {
    setScale(result, resScale);
  }
  return error;
}

void workWithSign(int signA, int signB, s21_decimal *result, s21_decimal *tempA,
                  s21_decimal *tempB) {
  if (signA != signB) {
    setSign(result, 1);
  } else {
    setSign(result, 0);
  }
  setSign(tempA, 0);
  setSign(tempB, 0);
}

void divLogic(int *resScale, int scaleCount, s21_decimal *result,
              s21_decimal *tempA, s21_decimal *tempB, s21_decimal temp) {
  while (s21_is_greater_or_equal(*tempB, temp) ||
         s21_is_greater_or_equal(*tempA, temp)) {
    leftShift(result);
    if (s21_is_greater(*tempB, *tempA)) {
      setBit(result, 0, 0);
      if (scaleCount == 1) {
        *resScale += 1;
      }
    } else {
      s21_sub_for_div(*tempA, *tempB, tempA);
      setBit(result, 0, 1);
      if (scaleCount == 1) {
        *resScale += 1;
      }
    }
    rightShift(tempB);
  }
}

int s21_sub_for_div(s21_decimal a, s21_decimal b, s21_decimal *res) {
  s21_decimal decr = {{0, 0, 0, 0}}, subt = {{0, 0, 0, 0}};
  int finRes = 0, addCounter = 0;
  int aSign = getSign(a), bSign = getSign(b), resSign = 0, aScale = getScale(a),
      bScale = getScale(b), resScale = 0;
  int aIsGreaterThenB = s21_is_greater(a, b);
  int equal = s21_is_equal(a, b);
  if (aSign == 0 && bSign == 0) {
    if (aScale != bScale) {
      resScale = workWithScaleForDiv(&a, &b);
    } else {
      resScale = aScale;
    }
    setScale(res, resScale);
    nullSignForSub(a, b, &decr, &subt, res, aIsGreaterThenB, equal, &resSign,
                   aSign, resScale);
  }
  if (equal == 0 && addCounter == 0) {
    subtLogic(decr, subt, res);
  }
  setSign(res, resSign);
  return finRes;
}

void subtLogic(s21_decimal decr, s21_decimal subt, s21_decimal *res) {
  for (int i = 0; i < 96; i++) {
    if (getBit(decr, i) == 1 && getBit(subt, i) == 0) {
      setBit(res, i, 1);
    } else if (getBit(decr, i) == 1 && getBit(subt, i) == 1) {
      setBit(res, i, 0);
    } else if (getBit(decr, i) == 0 && getBit(subt, i) == 0) {
      setBit(res, i, 0);
    } else if (getBit(decr, i) == 0 && getBit(subt, i) == 1) {
      int e = 0;
      while (getBit(decr, i + e) != 1) {
        setBit(&decr, i + e, 1);
        e++;
      }
      setBit(&decr, i + e, 0);
      setBit(res, i, 1);
    }
  }
}

void nullSignForSub(s21_decimal a, s21_decimal b, s21_decimal *decr,
                    s21_decimal *subt, s21_decimal *res, int aIsGreaterThenB,
                    int equal, int *resSign, int aSign, int resScale) {
  if (aIsGreaterThenB == 1) {
    deciCopy(a, decr);
    deciCopy(b, subt);
    *resSign = 0;
  } else if (equal == 1) {
    *resSign = aSign;
    toZero(res);
    setScale(res, resScale);
  } else {
    deciCopy(b, decr);
    deciCopy(a, subt);
    *resSign = 1;
  }
}

void oneSignForSub(s21_decimal a, s21_decimal b, s21_decimal *decr,
                   s21_decimal *subt, s21_decimal *res, int aIsGreaterThenB,
                   int equal, int *resSign, int aSign, int resScale) {
  if (aIsGreaterThenB == 1) {
    deciCopy(b, decr);
    deciCopy(a, subt);
    *resSign = 0;
  } else if (equal == 1) {
    *resSign = aSign;
    toZero(res);
    setScale(res, resScale);
  } else {
    deciCopy(a, decr);
    deciCopy(b, subt);
    *resSign = 1;
  }
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  *dst = 0;
  double temp = (double)*dst;
  for (int i = 0; i < 96; i++) {
    temp += getBit(src, i) * pow(2, i);
  }
  temp = temp * pow(10, -getScale(src));
  if (getSign(src)) {
    temp *= -1;
  }
  *dst = (float)temp;
  return 0;
}

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int error = 0;
  toZero(dst);
  if (src < 0) {
    setSign(dst, 1);
    src *= -1;
  }
  if (src > INT32_MAX) {
    error = 1;
  } else {
    dst->bits[0] = src;
  }
  return error;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int error = 0;
  *dst = 0;
  int scale = getScale(src);
  if (src.bits[1] || src.bits[2]) {
    error = 1;
  } else {
    *dst = src.bits[0];
    if (scale > 0 && scale <= 28) {
      *dst /= pow(10, scale);
    }
  }
  if (getSign(src)) {
    *dst *= -1;
  }
  return error;
}

void rightShift(s21_decimal *a) {
  int startMid = getBit(*a, START_MID);
  int startHigh = getBit(*a, START_HIGHT);
  for (int i = 0; i < 3; i++) {
    a->bits[i] >>= 1;
  }
  setBit(a, START_MID - 1, startMid);
  setBit(a, START_HIGHT - 1, startHigh);
}

void leftShift(s21_decimal *a) {
  int befMid = getBit(*a, START_MID - 1);
  int befHigh = getBit(*a, START_HIGHT - 1);
  for (int i = 0; i < 3; ++i) {
    a->bits[i] <<= 1;
  }
  setBit(a, START_MID, befMid);
  setBit(a, START_HIGHT, befHigh);
}

int s21_add(s21_decimal a, s21_decimal b, s21_decimal *res) {
  s21_decimal tempA = a, tempB = b;
  toZero(res);
  int finRes = 0;
  int aSign = getSign(tempA), bSign = getSign(tempB), aScale = getScale(tempA),
      bScale = getScale(tempB), resSign = 0, resScale = 0;
  int temp1 = 0, temp2 = 0, temp = 0, resTemp = 0;
  if ((aSign == 0 && bSign == 0) || (aSign == 1 && bSign == 1)) {
    if (aScale != bScale) {
      resScale = workWithScale(&tempA, &tempB);
    } else {
      resScale = aScale;
    }
    setScale(res, resScale);
    for (int i = 0; i < 96; i++) {
      temp1 = getBit(tempA, i);
      temp2 = getBit(tempB, i);
      resTemp = temp1 + temp2 + temp;
      if (resTemp == 0) {
        setBit(res, i, 0);
        temp = 0;
      } else if (resTemp == 1) {
        setBit(res, i, 1);
        temp = 0;
      } else if (resTemp > 1) {
        if (resTemp == 2) {
          setBit(res, i, 0);
        } else {
          setBit(res, i, 1);
        }
        temp = 1;
        if (i == 95) {
          finRes = 1;
          toZero(res);
        }
      }
    }
    if (finRes == 0) {
      resSign = aSign;
      setSign(res, resSign);
    }
  } else if (aSign == 1 && bSign == 0) {
    setSign(&tempA, 0);
    s21_sub(tempB, tempA, res);
  } else if (aSign == 0 && bSign == 1) {
    setSign(&tempB, 0);
    s21_sub(tempA, tempB, res);
  }
  return finRes;
}

int s21_sub(s21_decimal a, s21_decimal b, s21_decimal *res) {
  s21_decimal tempA = a, tempB = b;
  toZero(res);
  s21_decimal decr = {{0, 0, 0, 0}}, subt = {{0, 0, 0, 0}};
  int finRes = 0, addCounter = 0;
  int aSign = getSign(tempA), bSign = getSign(tempB), resSign = 0,
      aScale = getScale(tempA), bScale = getScale(tempB), resScale = 0;
  int aIsGreaterThenB = s21_is_greater(a, b);
  int equal = s21_is_equal(tempA, tempB);
  if (aSign == 0 && bSign == 0) {
    if (aScale != bScale) {
      resScale = workWithScale(&tempA, &tempB);
    } else {
      resScale = aScale;
    }
    setScale(res, resScale);
    nullSignForSub(tempA, tempB, &decr, &subt, res, aIsGreaterThenB, equal,
                   &resSign, aSign, resScale);
  } else if (aSign == 1 && bSign == 1) {
    if (aScale != bScale) {
      resScale = workWithScale(&tempA, &tempB);
    } else {
      resScale = aScale;
    }
    setScale(res, resScale);
    oneSignForSub(tempA, tempB, &decr, &subt, res, aIsGreaterThenB, equal,
                  &resSign, aSign, resScale);
  } else if (aSign == 0 && bSign == 1) {
    setSign(&tempB, 0);
    finRes = s21_add(tempA, tempB, res);
    if (finRes == 1) {
      finRes = 2;
    }
    addCounter = 1;
    resSign = 0;
  } else if (aSign == 1 && bSign == 0) {
    setSign(&tempA, 0);
    finRes = s21_add(tempA, tempB, res);
    if (finRes == 1) {
      finRes = 2;
    }
    addCounter = 1;
    resSign = 1;
  }
  if (equal == 0 && addCounter == 0) {
    subtLogic(decr, subt, res);
  }
  setSign(res, resSign);
  return finRes;
}

int s21_mul(s21_decimal a, s21_decimal b, s21_decimal *res) {
  s21_decimal tempA = a, tempB = b;
  toZero(res);
  int aSign = getSign(tempA), bSign = getSign(tempB), aScale = getScale(tempA),
      bScale = getScale(tempB), bMas[96] = {0}, counter = 0, lenghtA = 0;
  s21_decimal tmp = {0}, zero = {0};
  for (int i = 0; i < 96; i++) {
    setBit(&zero, i, 0);
  }
  for (int i = 0; i < 96; i++) {
    setBit(&tmp, i, 0);
  }
  setScale(&tmp, aScale);
  for (int i = 0; i < 96; i++) {
    bMas[i] = getBit(tempB, i);
  }
  for (int i = 95; i >= 0; i--) {
    if (bMas[i] == 1 && counter == 0) {
      lenghtA = i;
      counter++;
    }
  }
  setSign(&tempA, 0);
  setSign(&tempB, 0);
  for (int i = 0; i <= lenghtA; i++) {
    if (bMas[i] == 1) {
      s21_add(tmp, tempA, res);
    } else {
      s21_add(tmp, zero, res);
    }
    leftShift(&tempA);
    // aMas[0] = 0;
    for (int i = 0; i < 96; i++) {
      setBit(&tmp, i, getBit(*res, i));
    }
  }
  int resScale = aScale + bScale;
  setScale(res, resScale);
  if ((aSign == 0 && bSign == 0) || (aSign == 1 && bSign == 1)) {
    setSign(res, 0);
  } else if ((aSign == 1 && bSign == 0) || (aSign == 0 && bSign == 1)) {
    setSign(res, 1);
  }
  return 0;
}

int s21_is_less(s21_decimal a, s21_decimal b) {
  int aSign = getSign(a), bSign = getSign(b), aScale = getScale(a),
      bScale = getScale(b), scaleCounter = 0, aMas[96] = {0}, bMas[96] = {0},
      aFrac[28] = {0}, bFrac[28] = {0}, res = 0, tempA = 0, tempB = 0;
  if (aSign == 0 && bSign == 1) {
    res = 0;
  } else if (aSign == 1 && bSign == 0) {
    res = 1;
  } else if ((aSign == 0 && bSign == 0) || (aSign == 1 && bSign == 1)) {
    int match = 0;
    for (int i = 0; i < 96; i++) {
      aMas[i] = getBit(a, i);
      bMas[i] = getBit(b, i);
    }
    for (int i = 0; i < 48; i++) {
      tempA = aMas[i];
      tempB = bMas[i];
      aMas[i] = aMas[95 - i];
      bMas[i] = bMas[95 - i];
      aMas[95 - i] = tempA;
      bMas[95 - i] = tempB;
    }
    if (aScale != 0) {
      int e = 0;
      for (int i = 95; i > 95 - aScale; i--) {
        aFrac[e] = aMas[i];
        e++;
      }
      for (int i = 95 - aScale; i >= 0; i--) {
        aMas[i + aScale] = aMas[i];
        aMas[i] = 0;
      }
      scaleCounter = 1;
    }
    if (bScale != 0) {
      int e = 0;
      for (int i = 95; i > 95 - bScale; i--) {
        bFrac[e] = bMas[i];
        e++;
      }
      for (int i = 95 - bScale; i >= 0; i--) {
        bMas[i + bScale] = bMas[i];
        bMas[i] = 0;
      }
      scaleCounter = 1;
    }
    isLessLogic(aSign, aMas, bMas, &res, match, scaleCounter, aFrac, bFrac);
  }
  return res;
}

void isLessLogic(int aSign, int *aMas, int *bMas, int *res, int match,
                 int scaleCounter, int *aFrac, int *bFrac) {
  if (aSign == 0) {
    for (int i = 0; i < 96; i++) {
      if (aMas[i] != bMas[i]) {
        if (aMas[i] > bMas[i]) {
          *res = 0;
        } else {
          *res = 1;
        }
        match = 1;
        break;
      }
    }
    if (match == 0 && scaleCounter == 1) {
      for (int i = 0; i < 28; i++) {
        if (aFrac[i] != bFrac[i]) {
          if (aFrac[i] > bFrac[i]) {
            *res = 1;
          } else {
            *res = 0;
          }
          break;
        }
      }
    }
  }
  if (aSign == 1) {
    for (int i = 0; i < 96; i++) {
      if (aMas[i] != bMas[i]) {
        if (aMas[i] > bMas[i]) {
          *res = 1;
        } else {
          *res = 0;
        }
        match = 1;
        break;
      }
    }
    if (match == 0 && scaleCounter == 1) {
      for (int i = 0; i < 28; i++) {
        if (aFrac[i] != bFrac[i]) {
          if (aFrac[i] > bFrac[i]) {
            *res = 0;
          } else {
            *res = 1;
          }
          break;
        }
      }
    }
  }
}

int s21_is_less_or_equal(s21_decimal a, s21_decimal b) {
  s21_decimal tempA = a, tempB = b;
  int res = 0, match = 0, scaleA = getScale(a), scaleB = getScale(b),
      signA = getSign(a), signB = getSign(b);
  if (scaleA != scaleB) {
    workWithScale(&tempA, &tempB);
  }
  if (signA == 0 && signB == 1) {
    res = 0;
  } else if (signA == 1 && signB == 0) {
    res = 1;
  } else if (signA == 1 && signB == 1) {
    setSign(&tempA, 0);
    setSign(&tempB, 0);
    res = s21_is_greater_or_equal(tempA, tempB);
  } else {
    for (int i = 95; i >= 0; i--) {
      if (getBit(tempA, i) < getBit(tempB, i)) {
        res = 1;
        match = 1;
        break;
      } else if (getBit(tempA, i) > getBit(tempB, i)) {
        res = 0;
        match = 1;
        break;
      }
    }
    if (match == 0) {
      res = 1;
    }
  }
  if (isZero(a) && isZero(b)) {
    res = 1;
  }
  return res;
}

int s21_is_greater(s21_decimal a, s21_decimal b) {
  int aSign = getSign(a), bSign = getSign(b), aScale = getScale(a),
      bScale = getScale(b), scaleCounter = 0, aMas[96] = {0}, bMas[96] = {0},
      aFrac[28] = {0}, bFrac[28] = {0}, res = 0, tempA = 0, tempB = 0;
  if (aSign == 0 && bSign == 1) {
    res = 1;
  } else if (aSign == 1 && bSign == 0) {
    res = 0;
  } else if ((aSign == 0 && bSign == 0) || (aSign == 1 && bSign == 1)) {
    int match = 0;
    for (int i = 0; i < 96; i++) {
      aMas[i] = getBit(a, i);
      bMas[i] = getBit(b, i);
    }
    for (int i = 0; i < 48; i++) {
      tempA = aMas[i];
      tempB = bMas[i];
      aMas[i] = aMas[95 - i];
      bMas[i] = bMas[95 - i];
      aMas[95 - i] = tempA;
      bMas[95 - i] = tempB;
    }
    if (aScale != 0) {
      int e = 0;
      for (int i = 95; i > 95 - aScale; i--) {
        aFrac[e] = aMas[i];
        e++;
      }
      for (int i = 95 - aScale; i >= 0; i--) {
        aMas[i + aScale] = aMas[i];
        aMas[i] = 0;
      }
      scaleCounter = 1;
    }
    if (bScale != 0) {
      int e = 0;
      for (int i = 95; i > 95 - bScale; i--) {
        bFrac[e] = bMas[i];
        e++;
      }
      for (int i = 95 - bScale; i >= 0; i--) {
        bMas[i + bScale] = bMas[i];
        bMas[i] = 0;
      }
      scaleCounter = 1;
    }
    isGreaterLogic(aSign, aMas, bMas, &res, match, scaleCounter, aFrac, bFrac);
  }
  return res;
}

void isGreaterLogic(int aSign, int *aMas, int *bMas, int *res, int match,
                    int scaleCounter, int *aFrac, int *bFrac) {
  if (aSign == 0) {
    for (int i = 0; i < 96; i++) {
      if (aMas[i] != bMas[i]) {
        if (aMas[i] > bMas[i]) {
          *res = 1;
        } else {
          *res = 0;
        }
        match = 1;
        break;
      }
    }
    if (match == 0 && scaleCounter == 1) {
      for (int i = 0; i < 28; i++) {
        if (aFrac[i] != bFrac[i]) {
          if (aFrac[i] > bFrac[i]) {
            *res = 1;
          } else {
            *res = 0;
          }
          break;
        }
      }
    }
  }
  if (aSign == 1) {
    for (int i = 0; i < 96; i++) {
      if (aMas[i] != bMas[i]) {
        if (aMas[i] > bMas[i]) {
          *res = 0;
        } else {
          *res = 1;
        }
        match = 1;
        break;
      }
    }
    if (match == 0 && scaleCounter == 1) {
      for (int i = 0; i < 28; i++) {
        if (aFrac[i] != bFrac[i]) {
          if (aFrac[i] > bFrac[i]) {
            *res = 0;
          } else {
            *res = 1;
          }
          break;
        }
      }
    }
  }
}

int s21_is_greater_or_equal(s21_decimal a, s21_decimal b) {
  s21_decimal tempA = a, tempB = b;
  int res = 0, match = 0, scaleA = getScale(a), scaleB = getScale(b),
      signA = getSign(a), signB = getSign(b);
  if (scaleA != scaleB) {
    workWithScale(&tempA, &tempB);
  }
  if (signA == 0 && signB == 1) {
    res = 1;
  } else if (signA == 1 && signB == 0) {
    res = 0;
  } else if (signA == 1 && signB == 1) {
    setSign(&tempA, 0);
    setSign(&tempB, 0);
    res = s21_is_less_or_equal(tempA, tempB);
  } else {
    for (int i = 95; i >= 0; i--) {
      if (getBit(tempA, i) > getBit(tempB, i)) {
        res = 1;
        match = 1;
        break;
      } else if (getBit(tempA, i) < getBit(tempB, i)) {
        res = 0;
        match = 1;
        break;
      }
    }
    if (match == 0) {
      res = 1;
    }
  }
  if (isZero(a) && isZero(b)) {
    res = 1;
  }
  return res;
}

int s21_is_equal(s21_decimal a, s21_decimal b) {
  s21_decimal tempA = a, tempB = b;
  int res = 0, match = 0;
  if (getSign(a) == getSign(b)) {
    if (tempA.bits[3] == tempB.bits[3]) {
      isEqualLogic(tempA, tempB, match, &res);
    } else {
      workWithScale(&tempA, &tempB);
      isEqualLogic(tempA, tempB, match, &res);
    }
  }
  if (isZero(a) && isZero(b)) {
    res = 1;
  }
  return res;
}

void isEqualLogic(s21_decimal tempA, s21_decimal tempB, int match, int *res) {
  for (int i = 95; i >= 0; i--) {
    if (getBit(tempA, i) != getBit(tempB, i)) {
      match = 1;
      break;
    }
  }
  if (match == 0) {
    *res = 1;
  }
}

int s21_is_not_equal(s21_decimal a, s21_decimal b) {
  s21_decimal tempA = a, tempB = b;
  int res = 0, match = 0;
  if (getSign(a) == getSign(b)) {
    if (tempA.bits[3] == tempB.bits[3]) {
      isNotEqualLogic(tempA, tempB, match, &res);
    } else {
      workWithScale(&tempA, &tempB);
      isNotEqualLogic(tempA, tempB, match, &res);
    }
  } else {
    res = 1;
  }
  if (isZero(a) && isZero(b)) {
    res = 0;
  }
  return res;
}

void isNotEqualLogic(s21_decimal tempA, s21_decimal tempB, int match,
                     int *res) {
  for (int i = 95; i >= 0; i--) {
    if (getBit(tempA, i) != getBit(tempB, i)) {
      match = 1;
      break;
    }
  }
  if (match == 1) {
    *res = 1;
  }
}

int getBit(s21_decimal d, int index) {
  int bitNum = index % 32;
  int elemNum = index / 32;
  int mask = 1u << bitNum;
  return (d.bits[elemNum] & mask) >> bitNum;
}

void setBit(s21_decimal *d, int index, int bit) {
  int bitNum = index % 32;
  int elemNum = index / 32;
  if (bit == 1) {
    d->bits[elemNum] |= (1u << bitNum);
  } else {
    d->bits[elemNum] &= (~(1u << bitNum));
  }
}

void setScale(s21_decimal *d, int scale) {
  for (int i = 112; i < 119; i++) {
    setBit(d, i, scale & 1);
    scale >>= 1;
  }
}

int getScale(s21_decimal d) {
  int scale = 0;
  for (int i = 119; i >= 112; i--) {
    scale <<= 1;
    scale |= getBit(d, i);
  }
  return scale;
}

int getSign(s21_decimal d) { return (getBit(d, 127)); }

void setSign(s21_decimal *d, int sign) { setBit(d, 127, sign); }

int workWithScale(s21_decimal *a, s21_decimal *b) {
  int resScale = 0;
  s21_decimal ten = {{10, 0, 0, 0}};
  int scaleA = getScale(*a);
  int scaleB = getScale(*b);
  int res = scaleA > scaleB ? 0 : 1;
  int temp = 0;
  if (res == 0) {
    temp = scaleA - scaleB;
  } else {
    if (scaleA != scaleB) {
      temp = scaleB - scaleA;
    }
  }
  if (res == 0) {
    resScale = scaleA;
  } else {
    resScale = scaleB;
  }
  for (int i = 0; i < temp; i++) {
    if (res == 0) {
      s21_mul(*b, ten, b);
    } else {
      s21_mul(*a, ten, a);
    }
  }
  if (res == 0) {
    setScale(b, resScale);
  } else {
    setScale(a, resScale);
  }
  return resScale;
}

int workWithScaleForDiv(s21_decimal *a, s21_decimal *b) {
  int resScale = 0;
  int scaleA = getScale(*a);
  int scaleB = getScale(*b);
  int res = scaleA > scaleB ? 0 : 1;
  int temp = 0;
  if (res == 0) {
    temp = scaleA - scaleB;
  } else {
    if (scaleA != scaleB) {
      temp = scaleB - scaleA;
    }
  }
  if (res == 0) {
    resScale = scaleA;
  } else {
    resScale = scaleB;
  }
  for (int i = 0; i < temp; i++) {
    if (res == 0) {
      leftShift(b);
    } else {
      leftShift(a);
    }
  }
  return resScale;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  toZero(result);
  s21_decimal one = {{1, 0, 0, 0}};
  s21_decimal temp = {{0, 0, 0, 0}};
  int sign = getSign(value);
  if (sign) {
    s21_truncate(value, &temp);
    s21_sub(temp, one, &temp);
  } else {
    s21_truncate(value, &temp);
  }
  deciCopy(temp, result);
  return 0;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  double dif = 0;
  int valScale = getScale(value);
  if (valScale) {
    long double tempDouble = 0;
    from_decimal_to_double(value, &tempDouble);
    tempDouble = trunc(tempDouble);
    if (tempDouble < 0.0) {
      setSign(result, 1);
      tempDouble *= -1;
    }
    for (int i = 0; tempDouble >= 1 && i < 96; i++) {
      tempDouble = floor(tempDouble) / 2;
      dif = tempDouble - floor(tempDouble);
      if (dif > 0.0) {
        setBit(result, i, 1);
      } else {
        setBit(result, i, 0);
      }
      setScale(result, 0);
    }
  } else {
    deciCopy(value, result);
  }
  return 0;
}

int from_decimal_to_double(s21_decimal src, long double *dest) {
  long double temp = *dest;
  for (int i = 0; i < 96; i++) {
    temp += getBit(src, i) * pow(2, i);
  }
  temp = temp * pow(10, -getScale(src));
  if (getSign(src)) {
    temp *= -1;
  }
  *dest = temp;
  return 0;
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  int valSign = getSign(value);
  int resSign = 0;
  if (valSign == 0) {
    resSign = 1;
  } else {
    resSign = 0;
  }
  deciCopy(value, result);
  setSign(result, resSign);
  return 0;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  double dif = 0;
  int valScale = getScale(value);
  if (valScale) {
    long double tempDouble = 0;
    from_decimal_to_double(value, &tempDouble);
    tempDouble = round(tempDouble);
    if (tempDouble < 0.0) {
      setSign(result, 1);
      tempDouble *= -1;
    }
    for (int i = 0; tempDouble >= 1 && i < 96; i++) {
      tempDouble = floor(tempDouble) / 2;
      dif = tempDouble - floor(tempDouble);
      if (dif > 0.0) {
        setBit(result, i, 1);
      } else {
        setBit(result, i, 0);
      }
      setScale(result, 0);
    }
  } else {
    deciCopy(value, result);
  }
  return 0;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int error = 0;
  long double dif = 0.0;
  toZero(dst);
  if (isnan(src) || isinf(src)) {
    error = 1;
  } else if ((fabsl(src) < 1e-28 || fabsl(src) < 0) && src != -0) {
    error = 1;
  } else {
    if (dst) {
      error = 0;
      int sign = 0;
      if (src < 0) {
        sign = 1;
        src *= -1;
      }
      long int new = (long int)src;
      long int exp = 0;
      while (src - ((float)new / (long int)(pow(10, exp))) != 0) {
        exp++;
        new = src *(long int)(pow(10, exp));
      }
      long double tempNew = new;
      for (int i = 0; tempNew >= 1 && i < 96; i++) {
        tempNew = floor(tempNew) / 2;
        dif = tempNew - floor(tempNew);
        if (dif > 0.0) {
          setBit(dst, i, 1);
        } else {
          setBit(dst, i, 0);
        }
      }
      if (sign) {
        setSign(dst, 1);
      }
      dst->bits[3] += exp << 16;
    } else {
      error = 1;
    }
  }
  return error;
}
