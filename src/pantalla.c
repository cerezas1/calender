#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Zeller's congruence adaptado, o mktime/localtime — con time.h es más simple
int first_weekday_of_month(int year, int month) {
    struct tm t = {0};
    t.tm_year = year - 1900;
    t.tm_mon = month - 1;
    t.tm_mday = 1;
    mktime(&t); // normaliza y llena tm_wday
    return t.tm_wday; // 0 = domingo
}

int days_in_month(int year, int month) {
    static const int dim[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0))
        return 29;
    return dim[month - 1];
}

int main(int argc, char *argv[]) {
    int year, month;
    // esperás argv[1]=year argv[2]=month, o usás la fecha actual si no hay args
    if (argc >= 3) {
        year = atoi(argv[1]);
        month = atoi(argv[2]);
    } else {
        time_t now = time(NULL);
        struct tm *tm_now = localtime(&now);
        year = tm_now->tm_year + 1900;
        month = tm_now->tm_mon + 1;
    }

    time_t now = time(NULL);
    struct tm *today = localtime(&now);
    int is_current_month = (today->tm_year + 1900 == year && today->tm_mon + 1 == month);

    int start_wd = first_weekday_of_month(year, month);
    int total_days = days_in_month(year, month);

    printf("(box :class \"calendar-grid\" :orientation \"v\"\n");
    // filas de días...
    int day = 1;
    for (int week = 0; day <= total_days; week++) {
        printf("  (box :class \"calendar-row\" :orientation \"h\"\n");
        for (int wd = 0; wd < 7; wd++) {
            if ((week == 0 && wd < start_wd) || day > total_days) {
                printf("    (label :class \"day-empty\" :text \"\")\n");
            } else {
                const char *cls = (is_current_month && day == today->tm_mday)
                                   ? "day today" : "day";
                printf("    (button :class \"%s\" :onclick \"echo %d\" :text \"%d\")\n",
                       cls, day, day);
                day++;
            }
        }
        printf("  )\n");
    }
    printf(")\n");
    return 0;
}
