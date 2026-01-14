#include <libnotify/notify.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "heap.h"
#include "storage.h"
#include "utils.h"

int main(int argc, char **args)
{
    if (argc > 3) {
        fprintf(stderr, "Too much arguments, try -h (help).");
        return EXIT_FAILURE;
    }

    if (argc == 2 && strcmp(args[1], "-h") == 0) {
        printf("-h for help\n-i 'DD|MM|NAME|SURNAME' for adding new person\n-d 'DD|MM|NAME|SURNAME' to delete birthday\n-l ['DATE OF BIRTH'] or ['NAME SURNAME'] to list birth day\n-s to print soonest birthday\n-f 'FILENAME' to import birthdays from text file\n--delete-all to remove all birthdays\n");
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

        time_t currentTime = time(NULL);
        struct tm *localTime = localtime(&currentTime);
        const int month_i = localTime->tm_mon + 1;
        const int day_i = localTime->tm_mday;
        decrease_key(heap_p, month_i, day_i);
    }

    if (argc == 3 && strcmp(args[1], "-f") == 0) {
        if (!import_text_file(args[2], heap_p)) {
            fprintf(stderr, "Failed to import file: %s\n", args[2]);
            return EXIT_FAILURE;
        }
        printf("Import successful.\n");
    }

    if (argc == 2 && strcmp(args[1], "--delete-all") == 0) {
        clear_heap(heap_p);
        printf("All birthdays deleted.\n");
    }

    if (argc == 3 && strcmp(args[1], "-d") == 0) {
        char *tokens[MAX_TOKENS];
        int day_idx = find(heap_p, args[2], tokens);
        if (day_idx == -1) {
            fprintf(stderr, "Birthday does not found\n");
            destroy(heap_p);
            return EXIT_FAILURE;
        }
        delete (heap_p, day_idx, tokens);
    }

    if ((argc == 3 || argc == 2) && strcmp(args[1], "-l") == 0) {
        if (argc == 2) {
            uint8_t month;
            uint8_t day;

            for (size_t i = 0; i < heap_p->last_idx; i++) {
                month = heap_p->birthdays[i].month;
                day = heap_p->birthdays[i].day;
                for (size_t j = 0; j <= heap_p->birthdays[i].persons_arr->last_idx; j++) {
                    printf("%u. %u. has Birthday -> %s %s", day, month, heap_p->birthdays[i].persons_arr->person_arr[j].name, heap_p->birthdays[i].persons_arr->person_arr[j].surname);
                }
            }

        } else {
            char *tokens[MAX_TOKENS];
            int day_idx = find(heap_p, args[2], tokens);
            if (day_idx == -1) {
                fprintf(stderr, "Birthday does not found\n");
                destroy(heap_p);
                return EXIT_FAILURE;
            }
            if (day_idx >= 0) {
                uint8_t month = heap_p->birthdays[day_idx].month;
                uint8_t day = heap_p->birthdays[day_idx].day;
                for (size_t j = 0; j <= heap_p->birthdays[day_idx].persons_arr->last_idx; j++) {
                    printf("%u. %u. has Birthday -> %s %s", day, month, heap_p->birthdays[day_idx].persons_arr->person_arr[j].name, heap_p->birthdays[day_idx].persons_arr->person_arr[j].surname);
                }
            }
        }
    }

    if (argc == 2 && strcmp(args[1], "-s") == 0) {
        time_t currentTime = time(NULL);
        struct tm *localTime = localtime(&currentTime);
        const int month = localTime->tm_mon + 1;
        const int day = localTime->tm_mday;
        for (int i = (heap_p->last_idx / 2) + 1; i >= 0; --i){
 		heapify(heap_p, i, day, month);
 	}

        uint8_t b_day = heap_p->birthdays[0].day;
        uint8_t b_month = heap_p->birthdays[0].month;

        if (b_day != day || b_month != month) {
            notify_uninit();
            if (!save_changes(heap_p)) {
                return EXIT_FAILURE;
            }
            destroy(heap_p);
            return EXIT_SUCCESS;
        }

        int cap = 30;
        char *message = malloc(cap * sizeof(char));
        if (message == NULL) {
            notify_uninit();
            destroy(heap_p);
            return EXIT_FAILURE;
        }

        strcpy(message, "Today -> ");
        for (uint8_t i = 0; i <= heap_p->birthdays[0].persons_arr->last_idx; i++) {
            size_t name_len = strlen(heap_p->birthdays[0].persons_arr->person_arr[i].name);
            size_t surname_len = strlen(heap_p->birthdays[0].persons_arr->person_arr[i].surname);

            char *temp = realloc(message, strlen(message) + name_len + surname_len + 2);
            if (temp == NULL) {
                fprintf(stderr, "Failed to allocate memory\n");
                free(message);
                return EXIT_FAILURE;
            }
            message = temp;

            strcat(message, heap_p->birthdays[0].persons_arr->person_arr[i].name);
            strcat(message, " ");
            strcat(message, heap_p->birthdays[0].persons_arr->person_arr[i].surname);
        }

        if (!notify_init("Notification Example")) {
            fprintf(stderr, "Failed to initialize libnotify\n");
            destroy(heap_p);
            return EXIT_FAILURE;
        }

        NotifyNotification *notification = notify_notification_new("BirthDay Reminder", message, NULL);
        notify_notification_show(notification, NULL);

        g_object_unref(G_OBJECT(notification));
        notify_uninit();
        free(message);
    }

    if (!save_changes(heap_p)) {
        return EXIT_FAILURE;
    }
    destroy(heap_p);

    return EXIT_SUCCESS;
}
