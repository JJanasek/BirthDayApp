#include "storage.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

char *get_data_file_path()
{
    char *data_home = getenv("XDG_DATA_HOME");
    char *path = NULL;
    if (data_home) {
        size_t len = strlen(data_home) + strlen("/BirthDayApp/birthdays.dat") + 1;
        path = malloc(len);
        if (path) sprintf(path, "%s/BirthDayApp/birthdays.dat", data_home);
    } else {
        char *home = getenv("HOME");
        if (home) {
            size_t len = strlen(home) + strlen("/.local/share/BirthDayApp/birthdays.dat") + 1;
            path = malloc(len);
            if (path) sprintf(path, "%s/.local/share/BirthDayApp/birthdays.dat", home);
        }
    }
    return path;
}

void ensure_directory_exists(const char *path)
{
    char *p = strdup(path);
    char *slash = strrchr(p, '/');
    if (slash) {
        *slash = '\0';
        char cmd[512];
        snprintf(cmd, sizeof(cmd), "mkdir -p \"%s\"", p);
        int ret = system(cmd);
        (void)ret; // Suppress unused warning
    }
    free(p);
}

bool load_binary(FILE *file, heap *heap_p)
{
    char magic[4];
    if (fread(magic, 1, 4, file) != 4) return false;
    if (memcmp(magic, "BDAY", 4) != 0) return false;

    uint8_t version;
    if (fread(&version, 1, 1, file) != 1) return false;
    if (version != 1) return false;

    if (fread(&heap_p->capacity, sizeof(unsigned int), 1, file) != 1) return false;
    if (fread(&heap_p->last_idx, sizeof(unsigned int), 1, file) != 1) return false;

    heap_p->birthdays = malloc(heap_p->capacity * sizeof(day));
    if (heap_p->birthdays == NULL) return false;

    for (size_t i = 0; i < heap_p->last_idx; i++) {
        day *d = &heap_p->birthdays[i];
        if (fread(&d->day, sizeof(uint8_t), 1, file) != 1) return false;
        if (fread(&d->month, sizeof(uint8_t), 1, file) != 1) return false;

        d->persons_arr = malloc(sizeof(persons));
        if (d->persons_arr == NULL) return false;

        if (fread(&d->persons_arr->capacity, sizeof(unsigned int), 1, file) != 1) return false;
        if (fread(&d->persons_arr->last_idx, sizeof(unsigned int), 1, file) != 1) return false;

        d->persons_arr->person_arr = malloc(d->persons_arr->capacity * sizeof(person));
        if (d->persons_arr->person_arr == NULL) return false;

        for (size_t j = 0; j <= d->persons_arr->last_idx; j++) {
            person *p = &d->persons_arr->person_arr[j];
            uint8_t name_len;
            if (fread(&name_len, sizeof(uint8_t), 1, file) != 1) return false;
            p->name = malloc(name_len + 1);
            if (p->name == NULL) return false;
            if (fread(p->name, 1, name_len, file) != name_len) return false;
            p->name[name_len] = '\0';

            uint8_t surname_len;
            if (fread(&surname_len, sizeof(uint8_t), 1, file) != 1) return false;
            p->surname = malloc(surname_len + 1);
            if (p->surname == NULL) return false;
            if (fread(p->surname, 1, surname_len, file) != surname_len) return false;
            p->surname[surname_len] = '\0';
        }
    }
    return true;
}

bool save_changes(heap *heap_p)
{
    char *path = get_data_file_path();
    if (path == NULL) return false;
    ensure_directory_exists(path);

    FILE *file = fopen(path, "wb");
    if (file == NULL) {
        perror("data file");
        free(path);
        destroy(heap_p);
        return false;
    }

    // Header
    fwrite("BDAY", 1, 4, file);
    uint8_t version = 1;
    fwrite(&version, 1, 1, file);
    fwrite(&heap_p->capacity, sizeof(unsigned int), 1, file);
    fwrite(&heap_p->last_idx, sizeof(unsigned int), 1, file);

    for (size_t i = 0; i < heap_p->last_idx; i++) {
        day *d = &heap_p->birthdays[i];
        fwrite(&d->day, sizeof(uint8_t), 1, file);
        fwrite(&d->month, sizeof(uint8_t), 1, file);
        fwrite(&d->persons_arr->capacity, sizeof(unsigned int), 1, file);
        fwrite(&d->persons_arr->last_idx, sizeof(unsigned int), 1, file);

        for (size_t j = 0; j <= d->persons_arr->last_idx; j++) {
            person *p = &d->persons_arr->person_arr[j];
            uint8_t name_len = strlen(p->name);
            fwrite(&name_len, sizeof(uint8_t), 1, file);
            fwrite(p->name, 1, name_len, file);

            uint8_t surname_len = strlen(p->surname);
            fwrite(&surname_len, sizeof(uint8_t), 1, file);
            fwrite(p->surname, 1, surname_len, file);
        }
    }

    if (fclose(file) == -1) {
        perror("data file");
        free(path);
        return false;
    }
    free(path);
    return true;
}
