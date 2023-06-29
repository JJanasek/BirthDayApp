#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define FILE_NAME "file.txt"
#define MAX_TOKENS 4
#define DELIM "|"

typedef struct person
{
    char *name;
    char *surname;

} person;

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

typedef struct heap
{
    unsigned int capacity;
    unsigned int last_idx;
    day *birthdays;

} heap;

bool destroy(heap *heap_p)
{
    for (size_t i = 0; i < heap_p->last_idx; i++) {
        day *current_day = &(heap_p->birthdays[i]);
        persons *current_persons = current_day->persons_arr;

        for (size_t j = 0; j <= current_persons->last_idx; j++) {
            person *current_person = &(current_persons->person_arr[j]);
            free(current_person->name);
            free(current_person->surname);
        }

        free(current_persons->person_arr);
        free(current_persons);
    }

    free(heap_p->birthdays);
    return true;
}

bool file_exists(const char *file)
{
    if (access(file, F_OK) != -1) {
        return true;
    }
    return false;
}

bool add_day(heap *heap_p, char **tokens)
{
    day new_day = { UINT8_MAX, UINT8_MAX, NULL };

    char *end;
    uint8_t day_i = strtol(tokens[0], &end, 10);
    if (end == tokens[0]) {
        perror("strtol");
        return false;
    }
    new_day.day = day_i;
    uint8_t month = strtol(tokens[1], &end, 10);
    if (end == tokens[1]) {
        perror("strtol");
        return false;
    }
    new_day.month = month;
    person new_person = { NULL, NULL };
    new_person.name = strdup(tokens[2]);
    if (new_person.name == NULL) {
        perror("strdup");
        return false;
    }
    new_person.surname = strdup(tokens[3]);
    if (new_person.surname == NULL) {
        perror("strdup");
        return false;
    }

    if (heap_p->last_idx == 0 || (heap_p->birthdays[heap_p->last_idx - 1].day != day_i || heap_p->birthdays[heap_p->last_idx - 1].month != month)) {
        persons new_persons = { 16, 0, NULL };
        new_persons.person_arr = (person *) malloc(new_persons.capacity * sizeof(person));
        if (new_persons.person_arr == NULL) {
            perror("alloc fail");
            return false;
        }
        new_persons.person_arr[0] = new_person;
        new_day.persons_arr = (persons *) malloc(sizeof(persons));
        if (new_day.persons_arr == NULL) {
            perror("alloc fail");
            return false;
        }
        new_day.persons_arr[0] = new_persons;
        heap_p->birthdays[heap_p->last_idx] = new_day;
    } else {
        if (heap_p->birthdays[heap_p->last_idx - 1].persons_arr->last_idx + 1 >= heap_p->birthdays[heap_p->last_idx - 1].persons_arr->capacity) {
            heap_p->birthdays[heap_p->last_idx - 1].persons_arr->capacity *= 2;
            person *new = NULL;
            new = (person *) realloc(heap_p->birthdays[heap_p->last_idx - 1].persons_arr->person_arr, heap_p->birthdays[heap_p->last_idx - 1].persons_arr->capacity * sizeof(person));
            if (new == NULL) {
                perror("realloc fail");
                return false;
            }
            heap_p->birthdays[heap_p->last_idx - 1].persons_arr->person_arr = new;
        }
        heap_p->last_idx--;
        heap_p->birthdays[heap_p->last_idx].persons_arr->last_idx++;
        uint8_t idx = heap_p->birthdays[heap_p->last_idx].persons_arr->last_idx;
        heap_p->birthdays[heap_p->last_idx].persons_arr->person_arr[idx] = new_person;
    }
    return true;
}

bool load_file(FILE *file, heap *heap_p)
{
    char *line = NULL;
    size_t bufsize = 0;
    ssize_t characters_read;
    char *token;

    while ((characters_read = getline(&line, &bufsize, file)) != -1) {
        char *tokens[MAX_TOKENS];
        uint8_t num_of_tokens = 0;
        token = strtok(line, DELIM);
        while (token != NULL && num_of_tokens < MAX_TOKENS) {
            tokens[num_of_tokens] = token;
            token = strtok(NULL, DELIM);
            num_of_tokens++;
        }

        if (heap_p->last_idx + 1 >= heap_p->capacity) {
            heap_p->capacity *= 2;
            day *new = NULL;
            new = (day *) realloc(heap_p->birthdays, heap_p->capacity * sizeof(day));
            if (new == NULL) {
                free(line);
                return false;
            }
            heap_p->birthdays = new;
        }

        if (!add_day(heap_p, tokens)) {
            free(line);
            return false;
        }

        heap_p->last_idx += 1;
    }
    free(line);

    if (characters_read == -1 && feof(file) == 0) {
        perror("getline");
        return false;
    }
    return true;
}

heap *heap_init(heap *heap_p)
{
    heap_p->birthdays = malloc(heap_p->capacity * sizeof(day));
    if (heap_p->birthdays == NULL) {
        perror("Allocation fail");
        return NULL;
    }

    if (!file_exists(FILE_NAME)) {
        FILE *file = fopen(FILE_NAME, "w");
        if (file == NULL) {
            perror("creating file");
            destroy(heap_p);
            return NULL;
        }
        if (fclose(file) == -1) {
            perror("creating file");
        }
    }

    FILE *file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        perror("data file");
        destroy(heap_p);
        return NULL;
    }

    if (!load_file(file, heap_p)) {
        destroy(heap_p);
        if (fclose(file) == -1) {
            perror("data file");
        }

        return NULL;
    }

    if (fclose(file) == -1) {
        perror("data file");
        return NULL;
    }

    return heap_p;
}

bool save_changes(heap *heap_p)
{
    FILE *file = fopen(FILE_NAME, "w");
    if (file == NULL) {
        perror("data file");
        destroy(heap_p);
        return false;
    }
    uint8_t month;
    uint8_t day;

    for (size_t i = 0; i < heap_p->last_idx; i++) {
        month = heap_p->birthdays[i].month;
        day = heap_p->birthdays[i].day;
        for (size_t j = 0; j <= heap_p->birthdays[i].persons_arr->last_idx; j++) {
            fprintf(file, "%u|%u|%s|%s", day, month, heap_p->birthdays[i].persons_arr->person_arr[j].name, heap_p->birthdays[i].persons_arr->person_arr[j].surname);
        }
    }

    if (fclose(file) == -1) {
        perror("data file");
        return false;
    }
    return true;
}

bool create_day(heap *heap_p, char *input)
{
    char *tokens[MAX_TOKENS];
    uint8_t num_of_tokens = 0;
    char *token;
    token = strtok(input, DELIM);
    while (token != NULL && num_of_tokens < MAX_TOKENS) {
        tokens[num_of_tokens] = token;
        token = strtok(NULL, DELIM);
        num_of_tokens++;
    }
    if (num_of_tokens != MAX_TOKENS) {
        destroy(heap_p);
        fprintf(stderr, "Invalid day\n");
        return false;
    }

    char new_surname[strlen(tokens[3]) + 2];
    strcpy(new_surname, tokens[3]);
    size_t length = strlen(new_surname);
    new_surname[length] = '\n';
    new_surname[length + 1] = '\0';
    tokens[3] = new_surname;
    char *end;
    uint8_t day_i = strtol(tokens[0], &end, 10);
    if (end == tokens[0]) {
        destroy(heap_p);
        perror("strtol");
        return false;
    }
    if (day_i < 1 || day_i > 31) {
        destroy(heap_p);
        fprintf(stderr, "Invalid day\n");
        return false;
    }
    uint8_t month = strtol(tokens[1], &end, 10);
    if (end == tokens[1]) {
        destroy(heap_p);
        perror("strtol");
        return false;
    }
    if (month < 1 || month > 12) {
        destroy(heap_p);
        fprintf(stderr, "Invalid day\n");
        return false;
    }
    if (!add_day(heap_p, tokens)) {
        destroy(heap_p);
        return false;
    }
    return true;
}

int cmp(day *last, day *parent, const int cur_month, const int cur_day)
{
    if (last->day == parent->day && last->month == parent->month) {
        return 0;
    }

    int diff_l = abs(cur_month - last->month);
    int diff_p = abs(cur_month - parent->month);
    if (diff_l < diff_p) {
        return -1;
    }
    if (diff_p == diff_l) {
        diff_l = abs(cur_day - last->day);
        diff_p = abs(cur_day - parent->day);
        if (diff_l < diff_p) {
            return -1;
        }
    }
    return 1;
}

uint8_t parent_idx(uint8_t curr)
{
    if (curr == 0) {
        return UINT8_MAX;
    }
    return ((curr - 1) / 2);
}

void swap(heap *heap_p, uint8_t last, uint8_t parent)
{
    day temp = heap_p->birthdays[last];
    heap_p->birthdays[last] = heap_p->birthdays[parent];
    heap_p->birthdays[parent] = temp;
}

void decrease_key(heap *heap_p, const int cur_month, const int cur_day)
{
    uint8_t idx = heap_p->last_idx - 1;
    day *last = &heap_p->birthdays[idx];

    if (parent_idx(idx) == UINT8_MAX) {
        return;
    }

    day *parent = &heap_p->birthdays[parent_idx(idx)];

    while (parent_idx(idx) != UINT8_MAX && cmp(last, parent, cur_month, cur_day) == -1) {
        swap(heap_p, idx, parent_idx(idx));
        idx = parent_idx(idx);
        last = &heap_p->birthdays[idx];
        if (parent_idx(idx) == UINT8_MAX) {
            break;
        }
        parent = &heap_p->birthdays[parent_idx(idx)];
    }
}

int main(int argc, char **args)
{
    if (argc > 3) {
        fprintf(stderr, "Too much arguments, try -h (help).");
        return EXIT_FAILURE;
    }

    if (argc == 2 && strcmp(args[1], "-h") == 0) {
        printf("-h for help\n-i 'DD|MM|NAME|SURNAME' for adding new person\n-d 'DD|MM|NAME|SURNAME' to delete birthday\n-l ['DATE OF BIRTH'] or ['NAME SURNAME'] to list birth day\n-s to print soonest birthday\n");
        return EXIT_SUCCESS;
    }

    heap new_heap = { 16, 0, NULL };
    heap *heap_p = &new_heap;
    heap_p = heap_init(heap_p);

    if (heap_p == NULL) {
        return EXIT_FAILURE;
    }

    if (argc == 3 && strcmp(args[1], "-i") == 0) {
        if (!create_day(heap_p, args[2])) {
            return EXIT_FAILURE;
        }
        heap_p->last_idx++;

        time_t currentTime = time(NULL);
        struct tm *localTime = localtime(&currentTime);
        const int month = localTime->tm_mon + 1;
        const int day = localTime->tm_mday;
        decrease_key(heap_p, month, day);
    }

    uint8_t month;
    uint8_t day;

    for (size_t i = 0; i < heap_p->last_idx; i++) {
        month = heap_p->birthdays[i].month;
        day = heap_p->birthdays[i].day;
        for (size_t j = 0; j <= heap_p->birthdays[i].persons_arr->last_idx; j++) {
            printf("%u|%u|%s|%s", day, month, heap_p->birthdays[i].persons_arr->person_arr[j].name, heap_p->birthdays[i].persons_arr->person_arr[j].surname);
        }
    }

    if (!save_changes(heap_p)) {
        return EXIT_FAILURE;
    }
    destroy(heap_p);

    return EXIT_SUCCESS;
}
