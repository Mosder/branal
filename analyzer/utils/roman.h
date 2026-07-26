// utils/roman.h
// functions for roman numbers

typedef struct {
    unsigned int num;
    char *symbol;
} RomanSymbol;

static const RomanSymbol roman_symbols[] = {
    {1000, "M"},
    {900, "CM"},
    {500, "D"},
    {400, "CD"},
    {100, "C"},
    {90, "XC"},
    {50, "L"},
    {40, "XL"},
    {10, "X"},
    {9, "IX"},
    {5, "V"},
    {4, "IV"},
    {1, "I"},
};

// convert a arabic number to roman number
// params:
//      buffer - buffer to save the roman number to
//      number - arabic number to convert
extern void to_roman(char *buffer, unsigned int number);
