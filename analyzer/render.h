// render.h
// functions for "rendering" data onto the terminal

#include "parser/fight_results/fight_results.h"
#include "utils/memory.h"

// ANSI escape sequence colors
#define COLOR_GOLD 33
#define COLOR_GRAY 90
#define COLOR_RED 91
#define COLOR_GREEN 92
#define COLOR_YELLOW 93
#define COLOR_BLUE 94
#define COLOR_PURPLE 95
#define COLOR_AQUA 96
#define COLOR_WHITE 97

#define TABLE_ROW_SEPARATOR_CHAR '-'

#define TABLE_LINE_START "| "
#define TABLE_LINE_END " |"
#define TABLE_LINE_SEPARATOR " | "
#define TABLE_LINE_START_WIDTH (sizeof(TABLE_LINE_START) - 1)
#define TABLE_LINE_END_WIDTH (sizeof(TABLE_LINE_END) - 1)
#define TABLE_LINE_SEPARATOR_WIDTH (sizeof(TABLE_LINE_SEPARATOR) - 1)

// column info
typedef struct {
    char *name;
    size_t width;
    int color;
} ColumnInfo;
static const ColumnInfo columns[] = {
    {"Nick (LVL)", 17, COLOR_WHITE},
    {"EXP", 7, COLOR_WHITE},
    {"Gold", 7, COLOR_YELLOW},
    {"Psycho", 6, COLOR_AQUA},
    {"Saturation", 13, COLOR_GREEN},
    {"Splinters", 9, COLOR_BLUE},
};
static const inline size_t sum_col_widths() {
    size_t sum = 0;
    for (size_t i = 0; i < ARR_LEN(columns); i++)
        sum += columns[i].width;
    return sum;
}

#define TABLE_LINE_WIDTH (TABLE_LINE_START_WIDTH + sum_col_widths() + (ARR_LEN(columns) - 1) * TABLE_LINE_SEPARATOR_WIDTH + TABLE_LINE_END_WIDTH)

#define TABLE_ITEMS_WIDTH (TABLE_LINE_WIDTH - TABLE_LINE_START_WIDTH - TABLE_LINE_END_WIDTH)

// render fight results onto the terminal
// params:
//      - results - fight results to render
extern void render_fight_results(FightResults results);
