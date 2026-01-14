#ifndef DAY_H
#define DAY_H

#include <stdint.h>
#include "person.h"

typedef struct persons
{
    unsigned int capacity;
    unsigned int last_idx;
    person *person_arr;

} persons;

typedef struct day
{
    uint8_t day;
    uint8_t month;
    persons *persons_arr;

} day;

#endif
