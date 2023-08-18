CC = clang
CFLAGS = -Wall -O2

circular-test : circular.o
my_regexp_test : my_regexp.o
my_regexp.o : my_regexp.h
challenge15 : my_regexp.o
