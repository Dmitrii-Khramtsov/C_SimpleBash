# Implementation of `cat` and `grep` Utilities

## Project Description

This project is an implementation of two popular Unix utilities: `cat` and `grep`.

### `cat`

The `cat` utility is used to concatenate and display the contents of files. It supports various flags for formatting output, such as line numbering, displaying non-printable characters, and squeezing blank lines.

### `grep`

The `grep` utility is used to search for lines matching a given pattern in one or more files. It supports a multitude of options, including case-insensitive search, inverted search, displaying line numbers, and showing only the matching parts of lines.

## Functionality

### `cat`

- **Line numbering**: `-n`
- **Number non-empty lines**: `-b`
- **Display non-printable characters**: `-e`, `-E`, `-t`, `-T`
- **Squeeze blank lines**: `-s`

### `grep`

- **Case-insensitive search**: `-i`
- **Inverted search**: `-v`
- **Display line numbers**: `-n`
- **Search pattern from file**: `-f`
- **Show only matching parts of lines**: `-o`
- **Count matches**: `-c`
- **Display only filenames with matches**: `-l`
- **Suppress filename display**: `-h`
- **Silent mode**: `-s`
- **Implementation of all `grep` flags**: Support for all flags, including `-h`, `-s`, `-f`, `-o`.
- **Combination of `grep` flags**: Support for paired flag combinations (e.g., `-iv`, `-in`).

## Technologies

- **Programming Language**: C
- **Libraries**: Standard libraries for file handling and regular expressions
- **Standard**: C11
- **Compiler**: gcc
- **Coding Style**: Google Style
- **Testing**: Integration tests based on comparison with the behavior of real Bash utilities.

## Project Structure
- **Source Files**: Located in the `src/cat/` and `src/grep/` directories.
- **Executables**: `s21_cat` and `s21_grep` are located in the respective directories.
- **Makefile**: Configured to build the project with targets `s21_cat` and `s21_grep`.

## Usage Examples

### `cat`

```sh
$ ./s21_cat -n file.txt
```

### `grep`

```sh
$ ./s21_grep -i "pattern" file.txt
```
