#ifndef STORAGE_H
#define STORAGE_H

#include <stdbool.h>
#include <stdio.h>
#include "heap.h"

char *get_data_file_path();
void ensure_directory_exists(const char *path);
bool load_binary(FILE *file, heap *heap_p);
bool save_changes(heap *heap_p);

#endif
