#include "parser/structs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/memory.h"

void fill_struct(void *obj, const StructField *fields, size_t n_fields, const char *data, size_t data_len, const char *sep) {
    for (size_t i = 0; i < n_fields; i++) {
        char *next_sep = memmem(data, data_len, sep, strlen(sep));
        size_t len = next_sep ? next_sep - data : data_len;

        char *str = safe_malloc(len + 1);
        memcpy(str, data, len);
        str[len] = '\0';

        // parse depending on variable type
        void *p_field = (char *)obj + fields[i].offset;
        switch (fields[i].type) {
            case INT:
                sscanf(str, "%d", (int *)p_field);
                free(str);
                break;
            case FLOAT:
                sscanf(str, "%f", (float *)p_field);
                free(str);
                break;
            case DOUBLE:
                sscanf(str, "%lf", (double *)p_field);
                free(str);
                break;
            case STRING:
                *(char **)p_field = str;
                break;
        }

        // update data pointer and length
        if (next_sep) {
            data_len -= next_sep - data + strlen(sep);
            data = next_sep + strlen(sep);
        }
    }
}

void cleanup_struct(void *obj, const StructField *fields, size_t n_fields) {
    for (size_t i = 0; i < n_fields; i++) {
        if (fields[i].type == STRING)
            free(*(char **)((char *)obj + fields[i].offset));
    }
}
