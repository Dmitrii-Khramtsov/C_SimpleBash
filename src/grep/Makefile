CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c11
CLANGC=clang-format -i *.c
CLANGH=clang-format -i *.h

all: s21_grep

rebuild: clean all

s21_grep: s21_grep.c
	$(CC) $(CFLAGS) s21_grep.c -o s21_grep

clang:
	$(CLANGC)
	$(CLANGH)

clean:
	rm -f s21_grep