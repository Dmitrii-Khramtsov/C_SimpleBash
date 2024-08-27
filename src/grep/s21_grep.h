// Copyright 2024 Dmitrii Khramtsov

/**
 * @file s21_grep.h
 *
 * @brief Implementation of the grep command, which searches for patterns in files.
 *
 * This file contains the implementation of the grep command, including parsing of command-line flags,
 * reading files, and searching for patterns with various options for displaying the results.
 *
 * @author Dmitrii Khramtsov (lonmouth@student.21-school.ru)
 *
 * @date 2024-07-10
 *
 * @copyright School-21 (c) 2023
 */

#ifndef S21_GREP_H
#define S21_GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>

#define _GNU_SOURCE
#define BUFFER_SIZE 4096
#define BUFFER_PATTERN_SIZE 1024

typedef struct {
  //Флаг -e: Флаг -e позволяет указать шаблон, который следует использовать для
  //поиска. Это особенно полезно, если вы хотите указать несколько шаблонов для
  //поиска. Например: grep -e 'pattern1' -e 'pattern2' file.txt.
  unsigned int e;

  //Флаг -i: Флаг -i делает поиск нечувствительным к регистру. Так, grep -i
  //'pattern' file.txt найдет 'pattern', 'Pattern', 'PATTERN' и т.д.
  unsigned int i;

  //Флаг -v: Флаг -v инвертирует поиск, выводя строки, которые не соответствуют
  //шаблону. Например, grep -v 'pattern' file.txt выведет все строки, которые не
  //содержат 'pattern'.
  unsigned int v;

  //Флаг -c: Флаг -c выводит количество строк, которые соответствуют шаблону.
  //Например: grep -c 'pattern' file.txt.
  unsigned int c;

  //Флаг -l: Флаг -l выводит имена файлов, в которых найдено совпадение, а не
  //сами строки. Это полезно, если вы ищете в нескольких файлах: grep -l
  //'pattern' *.txt.
  unsigned int l;

  //Флаг -n: Флаг -n выводит номера строк, в которых найдено совпадение.
  //Например: grep -n 'pattern' file.txt.
  unsigned int n;

  //Флаг -h: Флаг -h подавляет вывод имен файлов в результатах поиска. Это
  //полезно при поиске в нескольких файлах, если вам не нужно знать, из какого
  //файла каждая строка.
  unsigned int h;

  //Флаг -s: Флаг -s подавляет сообщения об ошибках, связанных с недоступными
  //файлами.
  unsigned int s;

  //Флаг -f: Флаг -f позволяет указать файл, содержащий один или несколько
  //шаблонов, по которым нужно выполнить поиск. Например: grep -f patterns.txt
  // file.txt, где patterns.txt содержит шаблоны поиска.
  unsigned int f;

  //Флаг -o: Флаг -o выводит только ту часть строки, которая соответствует
  //шаблону. Например, grep -o 'pattern' file.txt выведет только 'pattern', если
  //оно присутствует в файле.
  unsigned int o;

  char pattern[BUFFER_PATTERN_SIZE];
  char line[BUFFER_SIZE];

  //назначается в функции, для передачи необходимого вида ошибки. err_message =
  // 1 (ошибки в воде параметров в stdin, флаги паттерны..); err_message = 2
  // (нет файла); err_message = 3 (если не скомпилировался regcomp);
  unsigned int err_message_index;

  unsigned int file_count;
  unsigned int line_count;
  unsigned int all_file_lines;
  unsigned int not_print;
  int first_is_not_flag;
  int flag;
  int reg_result;
} detect_flag_grep;

int read_file(int argc, char **argv, detect_flag_grep *stat);
int parsigs(int argc, char **argv, detect_flag_grep *stat);
void printer(FILE *file, detect_flag_grep *stat, char **argv);
void err_message(char **argv, detect_flag_grep *stat);
void flag_collisions(detect_flag_grep *stat);

void flag_v(detect_flag_grep *stat);
void flag_n(detect_flag_grep *stat);
void flag_h(detect_flag_grep *stat, char **argv);
void flag_c(detect_flag_grep *stat, char **argv);
void flag_l(detect_flag_grep *stat, char **argv);
void flag_o(detect_flag_grep *stat, const regex_t *regex);
int flag_f(detect_flag_grep *stat, char **argv, char *optarg);

#endif  // S21_GREP_H