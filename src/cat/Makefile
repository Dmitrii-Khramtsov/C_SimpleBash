CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c11
CLANGC=clang-format -i *.c
CLANGH=clang-format -i *.h

all: s21_cat

rebuild: clean all

s21_cat: s21_cat.c
	$(CC) $(CFLAGS) s21_cat.c -o s21_cat

clang:
	$(CLANGC)
	$(CLANGH) 

clean:
	rm -f s21_cat