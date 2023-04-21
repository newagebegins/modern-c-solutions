CC = clang
CFLAGS = -Wall

circular-test : circular.o
my_regexp_test : my_regexp.o
my_regexp.o : my_regexp.h
challenge15 : my_regexp.o
