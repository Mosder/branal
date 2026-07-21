// render.h
// functions for "rendering" data onto the terminal

#include "parser/fight_results/fight_results.h"

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

#define TABLE_COLUMN_COUNT 6

#define TABLE_NAME_LVL_WIDTH 17
#define TABLE_EXP_WIDTH 7
#define TABLE_GOLD_WIDTH 7
#define TABLE_PSYCHO_WIDTH 6
#define TABLE_SATURATION_WIDTH 13
#define TABLE_SPLINTERS_WIDTH 9

// clang-format off
#define TABLE_COLUMN_NAMES {"Nick (LVL)", "EXP", "Gold", "Psycho", "Saturation", "Splinters"}
#define TABLE_COLUMN_COLORS {COLOR_WHITE, COLOR_WHITE, COLOR_YELLOW, COLOR_AQUA, COLOR_GREEN, COLOR_BLUE}
#define TABLE_COLUMN_WIDTHS { \
    TABLE_NAME_LVL_WIDTH,   \
    TABLE_EXP_WIDTH,        \
    TABLE_GOLD_WIDTH,       \
    TABLE_PSYCHO_WIDTH,     \
    TABLE_SATURATION_WIDTH, \
    TABLE_SPLINTERS_WIDTH,  \
}

#define TABLE_LINE_WIDTH ( \
    TABLE_LINE_START_WIDTH + \
    TABLE_NAME_LVL_WIDTH +   \
    TABLE_EXP_WIDTH +        \
    TABLE_GOLD_WIDTH +       \
    TABLE_PSYCHO_WIDTH +     \
    TABLE_SATURATION_WIDTH + \
    TABLE_SPLINTERS_WIDTH +  \
    (TABLE_COLUMN_COUNT - 1) * TABLE_LINE_SEPARATOR_WIDTH + \
    TABLE_LINE_END_WIDTH     \
)
// clang-format on

#define TABLE_ITEMS_WIDTH (TABLE_LINE_WIDTH - TABLE_LINE_START_WIDTH - TABLE_LINE_END_WIDTH)

// render fight results onto the terminal
// params:
//      - results - fight results to render
extern void render_fight_results(FightResults results);
