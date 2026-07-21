#include "utils/roman.h"

#include <string.h>

#include "utils/memory.h"

void to_roman(char *buffer, int number) {
    buffer[0] = '\0';
    for (size_t i = 0; i < ARR_LEN(roman_symbols); i++) {
        for (size_t j = 0; j < number / roman_symbols[i].num; j++) {
            strcat(buffer, roman_symbols[i].symbol);
        }
        number %= roman_symbols[i].num;
    }
}
