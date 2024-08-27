// Copyright 2024 Dmitrii Khramtsov

/**
 * @file s21_grep.c
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

#include "s21_grep.h"

int main(int argc, char **argv) {
  detect_flag_grep stat = {0};
  if (!parsigs(argc, argv, &stat)) {
    read_file(argc, argv, &stat);
  }

  return 0;
}

int parsigs(int argc, char **argv, detect_flag_grep *stat) {
  int error = 0;
  stat->flag = 0;
  int pattern_count = 0;
  static struct option long_options[] = {{0, 0, 0, 0}};
  while ((stat->flag = getopt_long(argc, argv, "e:f:ivclnhso", long_options,
                                   NULL)) != -1) {
    if (stat->flag == 'e') {
      stat->e = 1;
      pattern_count++;

      // запись шаблонов в одну строку с добавлением оператора или = "|" между.
      if (pattern_count > 1 || stat->f) strcat(stat->pattern, "|");
      strcat(stat->pattern, optarg);
    }
    if (stat->flag == 'i') stat->i = 1;
    if (stat->flag == 'v') stat->v = 1;
    if (stat->flag == 'c') stat->c = 1;
    if (stat->flag == 'l') stat->l = 1;
    if (stat->flag == 'n') stat->n = 1;
    if (stat->flag == 'h') stat->h = 1;
    if (stat->flag == 's') stat->s = 1;
    if (stat->flag == 'f') {
      stat->f = 1;
      flag_f(stat, argv, optarg);
      pattern_count++;
    }
    if (stat->flag == 'o') stat->o = 1;
    if (stat->flag != 'e' && stat->flag != 'i' && stat->flag != 'v' &&
        stat->flag != 'c' && stat->flag != 'l' && stat->flag != 'n' &&
        stat->flag != 'h' && stat->flag != 's' && stat->flag != 'f' &&
        stat->flag != 'o') {
      stat->err_message_index = 1;
      err_message(argv, stat);
      error = 1;
    }
  }
  if (pattern_count == 0 && optind < argc) {
    // Если шаблон без флага -e, считаем следующий аргумент шаблоном.
    strcpy(stat->pattern, argv[optind]);
    optind++;  // увеличиваем индекс, так как шаблон уже обработан
  }
  stat->first_is_not_flag = optind;
  flag_collisions(stat);
  return error;
}

int read_file(int argc, char **argv, detect_flag_grep *stat) {
  int error = 0;
  stat->file_count = argc - stat->first_is_not_flag;
  FILE *file = NULL;
  for (; stat->first_is_not_flag < argc; stat->first_is_not_flag++) {
    file = fopen(argv[stat->first_is_not_flag], "r");
    if (file != NULL) {
      printer(file, stat, argv);
      fclose(file);
    } else {
      stat->err_message_index = 2;
      err_message(argv, stat);
      error = 1;
    }
  }
  return error;
}

void printer(FILE *file, detect_flag_grep *stat, char **argv) {
  regex_t regex;
  int mode_regcomp = REG_EXTENDED;
  stat->not_print = 0;
  stat->line_count = 0;
  stat->all_file_lines = 1;

  if (stat->i) {
    mode_regcomp = REG_ICASE | REG_EXTENDED;
  }

  stat->reg_result = regcomp(&regex, stat->pattern, mode_regcomp);
  if (!stat->reg_result) {
    while (fgets(stat->line, BUFFER_SIZE, file) != NULL) {
      stat->reg_result = regexec(&regex, stat->line, 0, NULL, 0);

      if (stat->v) flag_v(stat);
      if (!stat->h && !stat->c && !stat->l && !stat->reg_result)
        flag_h(stat, argv);
      if (stat->n && !stat->reg_result) flag_n(stat);
      if (stat->c && !stat->reg_result) {
        stat->line_count++;
        stat->not_print = 1;
      }
      if (stat->l && !stat->reg_result) {
        stat->not_print = 1;
      }
      if (stat->o && !stat->reg_result) flag_o(stat, &regex);
      if (!stat->not_print && !stat->reg_result) {
        printf("%s", stat->line);
        if (strlen(stat->line) && stat->line[strlen(stat->line) - 1] != '\n') {
          printf("\n");
        }
      }
      stat->all_file_lines++;
      stat->not_print = 0;
    }
    flag_c(stat, argv);
    flag_l(stat, argv);
    regfree(&regex);
  } else {
    stat->err_message_index = 3;
    err_message(argv, stat);
  }
}

void err_message(char **argv, detect_flag_grep *stat) {
  if (stat->err_message_index == 1 && !stat->s) {
    fprintf(stderr,
            "%s: illegal option -- %c usage: grep [-ivclnhso] [-e pattern] [-f "
            "file] [pattern] [file ...]",
            argv[0], stat->flag);
  }
  if (stat->err_message_index == 2 && !stat->s) {
    fprintf(stderr, "%s: %s: No such file or directory\n", argv[0],
            argv[stat->first_is_not_flag]);
  }
  if (stat->err_message_index == 3 && !stat->s) {
    fprintf(stderr, "Could not compile regex\n");
  }
}

void flag_collisions(detect_flag_grep *stat) {
  if (stat->c) {
    stat->n = 0;
    stat->e = 0;
    stat->o = 0;
  }
  if (stat->v) {
    stat->o = 0;
    stat->e = 0;
  }
  if (stat->l) {
    stat->n = 0;
    stat->h = 0;
    stat->e = 0;
    stat->o = 0;
  }
  if (stat->h) stat->e = 0;
  if (stat->o) stat->e = 0;
  //-lc, ищет первое совпадение в файле, затем прекращать поиск в этом файле
}

void flag_v(detect_flag_grep *stat) {
  if (stat->reg_result) {
    stat->reg_result = 0;
  } else
    stat->reg_result = 1;
}

void flag_h(detect_flag_grep *stat, char **argv) {
  if (stat->file_count > 1 && !stat->h) {
    printf("%s:", argv[stat->first_is_not_flag]);
  }
}

void flag_n(detect_flag_grep *stat) { printf("%d:", stat->all_file_lines); }

void flag_c(detect_flag_grep *stat, char **argv) {
  if (stat->c) {
    if (stat->file_count > 1 && !stat->h) {
      printf("%s:", argv[stat->first_is_not_flag]);
    }
    if (stat->line_count && stat->l && !stat->h) {
      stat->line_count = 1;
    }
    printf("%d\n", stat->line_count);
  }
}

void flag_l(detect_flag_grep *stat, char **argv) {
  if (stat->l) {
    printf("%s\n", argv[stat->first_is_not_flag]);
  }
}

void flag_o(detect_flag_grep *stat, const regex_t *regex) {
  regmatch_t match[1];
  char *o_line = stat->line;

  while (!regexec(regex, o_line, 1, match, 0)) {
    if (o_line[strlen(o_line) - 1] == '\n') {
      printf("%.*s\n", (int)(match[0].rm_eo - match[0].rm_so),
             o_line + match[0].rm_so);
    }
    o_line = o_line + (int)match[0].rm_eo + 1;
  }
  stat->not_print = 1;
}

int flag_f(detect_flag_grep *stat, char **argv, char *optarg) {
  int error = 0;
  char line[BUFFER_SIZE] = {0};
  FILE *pattern_file = NULL;
  pattern_file = fopen(optarg, "r");
  if (pattern_file != NULL) {
    while (fgets(line, BUFFER_SIZE, pattern_file) != NULL) {
      if (line[strlen(line) - 1] == '\n') line[strlen(line) - 1] = '\0';

      // запись шаблонов в одну строку с добавлением оператора или "|" между.
      if (strlen(stat->pattern) == 0) {
        strcat(stat->pattern, line);
      } else {
        strcat(stat->pattern, "|");
        strcat(stat->pattern, line);
      }
    }
    fclose(pattern_file);
  } else {
    stat->err_message_index = 2;
    err_message(argv, stat);
    error = 1;
  }
  return error;
}
