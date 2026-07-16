#include "render.h"

#include <stdio.h>
#include <string.h>

// strlen counting utf-8 characters as one
size_t utf8_strlen(char *s) {
    size_t len = 0;
    while (*s) {
        // if the byte is not a continuation byte (10xxxxxx), it's a new character
        if ((*s & 0xC0) != 0x80)
            len++;
        s++;
    }
    return len;
}

void print_row_separator() {
    for (size_t i = 0; i < TABLE_LINE_WIDTH; i++)
        printf("%c", TABLE_ROW_SEPARATOR_CHAR);
    printf("\n");
}

// print string with given color
void print_color(char *str, int color) {
    printf("\033[%dm%s\033[0m", color, str);
}

void print_center(char *text, int color, size_t width, char padding_char) {
    size_t post_char_count = (width - strlen(text)) / 2;
    size_t pre_char_count = width - post_char_count - strlen(text);

    for (size_t i = 0; i < pre_char_count; i++)
        printf("%c", padding_char);

    print_color(text, color);

    for (size_t i = 0; i < post_char_count; i++)
        printf("%c", padding_char);
}

void print_enemy_list(EnemyResults *results, size_t n_enemies) {
    printf(TABLE_LINE_START);
    size_t line_length = TABLE_LINE_START_WIDTH;

    for (size_t i = 0; i < n_enemies; i++) {
        EnemyResults enemy = results[i];
        char enemy_buffer[TABLE_LINE_WIDTH + 1];
        sprintf(enemy_buffer, "%s (%d)%s", enemy.name, enemy.level, i < n_enemies - 1 ? ", " : "");

        // break line if length would be too big
        if (line_length + utf8_strlen(enemy_buffer) > TABLE_LINE_WIDTH - TABLE_LINE_END_WIDTH) {
            printf("%*s\n", (int)(TABLE_LINE_WIDTH - line_length), TABLE_LINE_END);
            printf(TABLE_LINE_START);
            line_length = TABLE_LINE_START_WIDTH;
        }

        printf("%s", enemy_buffer);
        line_length += utf8_strlen(enemy_buffer);
    }

    printf("%*s\n", (int)(TABLE_LINE_WIDTH - line_length), TABLE_LINE_END);
}

void print_column_names() {
    char *names[TABLE_COLUMN_COUNT] = TABLE_COLUMN_NAMES;
    int colors[TABLE_COLUMN_COUNT] = TABLE_COLUMN_COLORS;
    size_t widths[TABLE_COLUMN_COUNT] = TABLE_COLUMN_WIDTHS;

    printf(TABLE_LINE_START);
    for (size_t i = 0; i < TABLE_COLUMN_COUNT; i++) {
        print_center(names[i], colors[i], widths[i], ' ');
        printf(i < TABLE_COLUMN_COUNT - 1 ? TABLE_LINE_SEPARATOR : TABLE_LINE_END);
    }
    printf("\n");
}

void print_player_rewards(FriendlyResults results) {
    char buffers[TABLE_COLUMN_COUNT][TABLE_LINE_WIDTH];
    size_t widths[TABLE_COLUMN_COUNT] = TABLE_COLUMN_WIDTHS;
    int colors[TABLE_COLUMN_COUNT] = TABLE_COLUMN_COLORS;

    sprintf(buffers[0], "%s (%d)", results.name, results.level);
    sprintf(buffers[1], "%d", results.exp);
    sprintf(buffers[2], "%d", results.gold);

    if (results.psycho > 0)
        sprintf(buffers[3], "%d", results.psycho);
    else
        sprintf(buffers[3], "-");

    if (results.saturation > 0)
        sprintf(buffers[4], "%d (%s)", results.saturation, results.saturation_type);
    else
        sprintf(buffers[4], "-");

    if (results.splinters > 0)
        sprintf(buffers[5], "%d", results.splinters);
    else
        sprintf(buffers[5], "-");

    printf(TABLE_LINE_START);
    for (size_t i = 0; i < TABLE_COLUMN_COUNT; i++) {
        char to_color[widths[i] + 1];
        sprintf(to_color, "%*s", (int)widths[i], buffers[i]);
        print_color(to_color, colors[i]);
        printf(i < TABLE_COLUMN_COUNT - 1 ? TABLE_LINE_SEPARATOR : TABLE_LINE_END);
    }
    printf("\n");

    // TODO: change this when I parse the items etc. to a readable format
    // needs to utilize utf8_strlen, because right now utf8 characters mess the table up
    char big_buf[1024];
    sprintf(big_buf, "%s - %s - %s", results.items, results.gear, results.drifs);
    if (strlen(big_buf) > 6) {
        printf(TABLE_LINE_START);
        print_center("ITEMS", COLOR_WHITE, TABLE_ITEMS_WIDTH, ' ');
        printf(TABLE_LINE_END "\n");

        for (size_t i = 0; i < strlen(big_buf); i += TABLE_ITEMS_WIDTH) {
            printf(TABLE_LINE_START "%-*.*s" TABLE_LINE_END "\n", (int)TABLE_ITEMS_WIDTH, (int)TABLE_ITEMS_WIDTH, big_buf + i);
        }
    }
}

void render_fight_results(FightResults results) {
    print_center("ENEMIES", COLOR_RED, TABLE_LINE_WIDTH, TABLE_ROW_SEPARATOR_CHAR);
    printf("\n");

    print_enemy_list(results.enemy_results, results.num_enemy);

    print_center("PLAYER REWARDS", COLOR_GREEN, TABLE_LINE_WIDTH, TABLE_ROW_SEPARATOR_CHAR);
    printf("\n");

    print_column_names();
    print_row_separator();

    for (size_t i = 0; i < results.num_friendly; i++) {
        print_player_rewards(results.friendly_results[i]);
        print_row_separator();
    }
}
