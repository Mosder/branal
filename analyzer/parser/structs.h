// parser/structs.h
// generic struct functionality for parsing

#include <stddef.h>

#ifndef PARSER_STRUCTS_H
#define PARSER_STRUCTS_H

typedef enum { INT, FLOAT, DOUBLE, STRING } TypeEnum;

typedef struct {
    TypeEnum type;
    size_t offset;
} StructField;

// fill generic struct with given data
// params:
//      - obj - pointer to the struct to fill
//      - fields - field types and offsets of the struct
//      - n_fields - count of fields in the struct
//      - data - data to fill the struct with
//      - data_len - length of the data
//      - sep - separator seperating the fields in data
extern void fill_struct(void *obj, const StructField *fields, size_t n_fields, const char *data, size_t data_len, const char *sep);

// cleanup allocations inside generic struct
// params:
//      - obj - pointer to the struct to cleanup
//      - fields - field types and offsets of the struct
//      - n_fields - count of fields in the struct
extern void cleanup_struct(void *obj, const StructField *fields, size_t n_fields);

#endif
