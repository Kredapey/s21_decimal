CC = gcc
CC_FLAGS = -Wall -Wextra -Werror -std=c11
NAME = s21_decimal.a check
OS=$(shell uname -s)
ifeq ($(OS), Linux)
	TEST_LIBS = -lcheck -lm -lpthread -lrt -lsubunit
else
	TEST_LIBS = -lcheck -lm -lpthread
endif
OBJECTS = s21_decimal.o
SOURSES = s21_decimal.c

.PHONY: all clean test s21_decimal.a gcov_report

all: $(NAME)

test: test.o s21_decimal.a $(OBJECTS)
	$(CC) $(OBJECTS) test.o s21_decimal.a $(TEST_LIBS) -o test
	./test

main: s21_decimal.o
	$(CC) s21_decimal.o -o main
	./main

s21_decimal.o: s21_decimal.c
	$(CC) $(CC_FLAGS) -c s21_decimal.c -g

test.o: test.c	
	$(CC) $(CC_FLAGS) -c test.c

gcov_report: s21_decimal.a 
	$(CC) $(CC_FLAGS) --coverage test.c s21_decimal.c s21_decimal.a $(TEST_LIBS) -o gcov_test
	chmod +x *
	./gcov_test
	lcov -t "gcov_test" -o gcov_test.info --no-external -c -d .
	genhtml -o report/ gcov_test.info
	open ./report/index.html

check: s21_decimal.a
	cppcheck *.h *.c
	clang-format -n *.c *.h
	$(CC) test.c s21_decimal.a -lcheck
	$(CC) $(CC_FLAGS) -c test.c -o test.o
	$(CC) test.o s21_decimal.a $(TEST_LIBS) -o test
	CK_FORK=no leaks --atExit -- ./test

s21_decimal.a: s21_decimal.o
	$(CC) -c $(SOURSES)
	ar rcs s21_decimal.a $(OBJECTS)

clean:
	rm -rf *.a
	rm -rf *.o
	rm -rf *.gcno
	rm -rf *.gcda
	rm -rf gcov_test
	rm -rf test
	rm -rf *.txt
	rm -rf *.info
	rm -rf *.out
	rm -rf *.dSYM
	rm -R report

vg: s21_decimal.a
	CK_FORK=no valgrind --track-origins=yes --leak-check=full --leak-resolution=med --show-leak-kinds=all -s ./test
	 
lik: s21_decimal.a
	CK_FORK=no leaks -atExit -- ./test