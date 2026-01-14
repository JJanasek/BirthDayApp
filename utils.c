#include "utils.h"
#include <string.h>
#include <ctype.h>
#include <stddef.h>

uint8_t parse(char **tokens, char *input)
{
    uint8_t num_of_tokens = 0;
    char *token = strtok(input, DELIM);
    while (token != NULL && num_of_tokens < MAX_TOKENS) {
        tokens[num_of_tokens] = token;
        token = strtok(NULL, DELIM);
        num_of_tokens++;
    }

    if (num_of_tokens == MAX_TOKENS) {
        return MAX_TOKENS;
    } else {
        if (isdigit(tokens[0][0])) {
            return ONLY_DATE;
        }
        return ONLY_NAME;
    }
}
