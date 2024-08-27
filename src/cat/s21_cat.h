// Copyright 2024 Dmitrii Khramtsov

/**
 * @file s21_cat.h
 *
 * @brief Implementation of the s21_cat utility for the SmartCalc v2.0 library.
 *
 * This file contains the implementation of the s21_cat utility, which is part of the
 * SmartCalc v2.0 library. The s21_cat utility is responsible for concatenating and
 * displaying the contents of files, with support for various flags to modify the
 * output format.
 *
 * @author Dmitrii Khramtsov (lonmouth@student.21-school.ru)
 *
 * @date 2024-06-03
 *
 * @copyright School-21 (c) 2023
 */

#ifndef S21_CAT_H
#define S21_CAT_H

#include <getopt.h>
#include <stdio.h>

typedef struct {
  unsigned int b;
  unsigned int e;
  unsigned int n;
  unsigned int s;
  unsigned int t;
  unsigned int v;
  unsigned int E;
  unsigned int T;

  //назначается в функции, для передачи необходимого вида ошибки.
  // err_message = 1 (ошибки в воде параметров в stdin, флаги паттерны..);
  // err_message = 2 (нет файла);
  unsigned int err_message_index;
  unsigned int line_count;
  unsigned int count;
  unsigned int not_print;

  char current;
  char past;
  int flag;
  int first_is_not_flag;
} detect_flag_cat;

void printer(FILE *file, detect_flag_cat *stat);

int parse_flag(int argc, char **argv, detect_flag_cat *stat);
int reader(int argc, char **argv, detect_flag_cat *stat);
void cat_err_message(char **argv, detect_flag_cat *stat);

void flag_s(detect_flag_cat *stat);
void flag_n(detect_flag_cat *stat);
void flag_b(detect_flag_cat *stat);
void flag_e(detect_flag_cat *stat);
void flag_t(detect_flag_cat *stat);
void flag_v(detect_flag_cat *stat);

#endif  // S21_CAT_H