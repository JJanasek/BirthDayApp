#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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
    persons * persons_arr;

} day;


typedef struct heap
{
    unsigned int capacity;
    unsigned int last_idx;
    day *birthdays;

} heap;


int main(int argc, char **args)
{
    if (argc > 3)
    {
        fprintf(stderr, "Too much arguments, try -h (help).");
        return EXIT_FAILURE;
    }

    if (argc == 2 && strcmp(args[1], "-h") == 0){
        printf("-h for help\n-i 'DD-MM NAME SURNAME' for adding new person\n-d 'DD-MM NAME SURNAME' to delete birthday\n-l ['DATE OF BIRTH'] or ['NAME SURNAME'] to list birth day\n-s to print soonest birthday\n");
        return EXIT_SUCCESS;
    }

    return EXIT_SUCCESS;
}
