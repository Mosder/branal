#include "utils/roman.h"

#include <stddef.h>
#include <string.h>

void to_roman(char *buffer, int number) {
    buffer[0] = '\0';
    int nums[ROMAN_CHAR_COUNT] = ROMAN_NUMS;
    char *chars[ROMAN_CHAR_COUNT] = ROMAN_CHARS;
    for (size_t i = 0; i < ROMAN_CHAR_COUNT; i++) {
        for (size_t j = 0; j < number / nums[i]; j++) {
            strcat(buffer, chars[i]);
        }
        number %= nums[i];
    }
}
