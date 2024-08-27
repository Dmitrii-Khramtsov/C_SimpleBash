// Copyright 2024 Dmitrii Khramtsov

/**
 * @file s21_cat.c
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

#include "s21_cat.h"

int main(int argc, char **argv) {
  detect_flag_cat stat = {0};
  if (!parse_flag(argc, argv, &stat)) {
    reader(argc, argv, &stat);
  }
  return 0;
}

int parse_flag(int argc, char **argv, detect_flag_cat *stat) {
  int error = 0;
  stat->flag = 0;
  static struct option long_options[] = {{"--number-nonblank", 0, 0, 'b'},
                                         {"--number", 0, 0, 'n'},
                                         {"--squeeze-blank", 0, 0, 's'},
                                         {0, 0, 0, 0}};

  while ((stat->flag = getopt_long(argc, argv, "+benstvET", long_options, 0)) !=
         -1) {
    if (stat->flag == 'b') stat->b = 1;
    if (stat->flag == 'e') {
      stat->e = 1;
      stat->v = 1;
    }
    if (stat->flag == 'n') stat->n = 1;
    if (stat->flag == 's') stat->s = 1;
    if (stat->flag == 't') {
      stat->t = 1;
      stat->v = 1;
    }
    if (stat->flag == 'v') stat->v = 1;
    if (stat->flag == 'E') stat->e = 1;
    if (stat->flag == 'T') stat->t = 1;
    if (stat->flag != 'b' && stat->flag != 'e' && stat->flag != 'n' &&
        stat->flag != 's' && stat->flag != 't' && stat->flag != 'v' &&
        stat->flag != 'E' && stat->flag != 'T') {
      stat->err_message_index = 1;
      cat_err_message(argv, stat);
    }
  }

  if (stat->b) stat->n = 0;
  stat->first_is_not_flag = optind;

  return error;
}

int reader(int argc, char **argv, detect_flag_cat *stat) {
  int error = 0;
  FILE *file = NULL;
  for (; stat->first_is_not_flag < argc; stat->first_is_not_flag++) {
    file = fopen(argv[stat->first_is_not_flag], "r");
    if (file != NULL) {
      printer(file, stat);
      fclose(file);
    } else {
      stat->err_message_index = 2;
      cat_err_message(argv, stat);
    }
  }
  return error;
}

void printer(FILE *file, detect_flag_cat *stat) {
  stat->current = 0;
  stat->past = '\n';
  stat->line_count = 1;
  stat->count = 0;
  stat->not_print = 0;

  while ((stat->current = fgetc(file)) != EOF) {
    if (stat->s) flag_s(stat);
    if (stat->n) flag_n(stat);
    if (stat->b) flag_b(stat);
    if (stat->e) flag_e(stat);
    if (stat->t) flag_t(stat);
    if (stat->v) flag_v(stat);
    if (!stat->b && !stat->e && !stat->e && !stat->n && !stat->s && !stat->t &&
        !stat->v && !stat->E && !stat->T) {
      printf("%c", stat->current);
      stat->not_print = 1;
    }

    if (stat->current != '\n' && stat->past == '\n') {
      stat->count = 0;
    }
    if (!stat->not_print) {
      printf("%c", stat->current);
    }

    stat->past = stat->current;
    stat->not_print = 0;
  }
}

void cat_err_message(char **argv, detect_flag_cat *stat) {
  if (stat->err_message_index == 1) {
    fprintf(stderr, "%s: illegal option -- %c usage: cat [-benstuv] [file ...]",
            argv[0], stat->flag);
  }
  if (stat->err_message_index == 2) {
    fprintf(stderr, "%s: %s: No such file or directory\n", argv[0],
            argv[stat->first_is_not_flag]);
  }
}

void flag_s(detect_flag_cat *stat) {
  if (stat->past == '\n' && stat->current == '\n') {
    stat->count++;
    if (stat->count > 1) stat->not_print = 1;
  }
}

void flag_n(detect_flag_cat *stat) {
  if (stat->past == '\n' && !stat->not_print) {
    printf("%6d\t", stat->line_count);
    stat->line_count++;
  }
}

void flag_b(detect_flag_cat *stat) {
  if (stat->current != '\n' && stat->past == '\n') {
    printf("%6d\t", stat->line_count);
    stat->line_count++;
  }
}

void flag_e(detect_flag_cat *stat) {
  if (stat->current == '\n' && stat->count < 2) {
    printf("$");
  }
}

void flag_t(detect_flag_cat *stat) {
  if (stat->current == '\t') {
    printf("^I");
    stat->not_print = 1;
  }
}

void flag_v(detect_flag_cat *stat) {
  if (stat->current <= 31 && stat->current != '\n' && stat->current != '\t' &&
      stat->current != 127) {
    stat->current += 64;
    printf("^");
  } else if (stat->current == 127) {
    stat->current = 63;
    printf("^");
  }
}
