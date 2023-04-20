bin/my_regexp_test : bin/my_regexp_test.o bin/my_regexp.o
	clang -Wall -o bin/my_regexp_test bin/my_regexp_test.o bin/my_regexp.o

bin/my_regexp_test.o : my_regexp_test.c my_regexp.h
	clang -Wall -c -o bin/my_regexp_test.o my_regexp_test.c

bin/my_regexp.o : my_regexp.c my_regexp.h
	clang -Wall -c -o bin/my_regexp.o my_regexp.c

bin/challenge15 : bin/challenge15.o bin/my_regexp.o
	clang -Wall -o bin/challenge15 bin/challenge15.o bin/my_regexp.o

bin/challenge15.o : challenge15.c my_regexp.h
	clang -Wall -c -o bin/challenge15.o challenge15.c

bin/exs42_scanf : exs42_scanf.c
	clang -Wall -o bin/exs42_scanf exs42_scanf.c

bin/fseekmax : fseekmax.c
	clang -Wall -o bin/fseekmax fseekmax.c
