// utils/roman.h
// functions for roman numbers

#define ROMAN_CHAR_COUNT 13
#define ROMAN_NUMS {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1}
#define ROMAN_CHARS {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"}

// convert a arabic number to roman number
// params:
//      buffer - buffer to save the roman number to
//      number - arabic number to convert
extern void to_roman(char *buffer, int number);
