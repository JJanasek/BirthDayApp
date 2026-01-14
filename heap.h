#ifndef HEAP_H
#define HEAP_H

#include <stdbool.h>
#include "day.h"

typedef struct heap
{
    unsigned int capacity;
    unsigned int last_idx;
    day *birthdays;

} heap;

bool destroy(heap *heap_p);
int add_day(heap *heap_p, char **tokens);
bool create_day(heap *heap_p, char *input);
void decrease_key(heap *heap_p, const int cur_month, const int cur_day);
void heapify(heap *heap_p, uint8_t idx, int curr_day, int cur_month);
void delete (heap *heap_p, uint8_t day_idx, char **tokens);
int find(heap *heap_p, char *input, char **tokens);
heap *heap_init(heap *heap_p);
void clear_heap(heap *heap_p);

#endif
