#include "utils/time.h"

#include <stdio.h>
#include <time.h>

void get_date_time_string(char *date_time) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    sprintf(date_time, "%02d%02d%02d_%02d%02d%02d", t->tm_year - 100, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
}
