bin/my_regexp_test : bin/my_regexp_test.o bin/my_regexp.o
	clang -Wall -o bin/my_regexp_test bin/my_regexp_test.o bin/my_regexp.o

bin/my_regexp_test.o : my_regexp_test.c my_regexp.h
	clang -Wall -c -o bin/my_regexp_test.o my_regexp_test.c

bin/my_regexp.o : my_regexp.c my_regexp.h
	clang -Wall -c -o bin/my_regexp.o my_regexp.c
