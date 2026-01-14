#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#define MAX_TOKENS 4
#define ONLY_NAME 1
#define ONLY_DATE 2
#define DELIM "|"

uint8_t parse(char **tokens, char *input);

#endif
