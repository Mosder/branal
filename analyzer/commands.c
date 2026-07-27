#include "commands.h"

#include <stdio.h>
#include <string.h>

#include "analyzer.h"
#include "capture.h"
#include "utils/memory.h"

#define INDENT "  "
#define MAX_COMMAND_LENGTH 100

// calculate the longest command length to align explenations
int longest_command_length(const char *prog_name) {
    int longest_len = 0;
    for (size_t i = 0; i < commands_count; i++) {
        Command cmd = commands[i];
        int len = strlen(prog_name);
        len += strlen(" ( | ) ");
        len += strlen(cmd.short_name) + strlen(cmd.name);
        len += strlen(cmd.args_str) == 0 ? 0 : strlen(cmd.args_str) + 1;
        if (len > longest_len)
            longest_len = len;
    }
    return longest_len;
}

// help command handler
int print_help(int argc, char *argv[]) {
    (void)argc;

    int len = longest_command_length(argv[0]);

    printf("A bare-bones analyzer for the game Broken Ranks\n");
    printf("\nUsage:\n");
    printf(INDENT "%-*s- same as below\n", len, argv[0]);

    for (size_t i = 0; i < commands_count; i++) {
        Command cmd = commands[i];

        char buffer[MAX_COMMAND_LENGTH];
        sprintf(buffer, "%s (%s | %s) ", argv[0], cmd.short_name, cmd.name);
        if (strlen(cmd.args_str))
            strcat(buffer, cmd.args_str);

        printf(INDENT "%-*s- %s\n", len, buffer, cmd.explanation[0]);
        for (size_t j = 1; j < cmd.explanation_lines; j++)
            printf(INDENT "%*s%s\n", len + 2, "", cmd.explanation[j]);
    }

    return 0;
}

// live analyzer command handler
int live_anal(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    // TODO: implement
    printf("Not implemented\n");
    return 1;
}

// capturing mode command handler
int capture(int argc, char *argv[]) {
    capturing_mode(argc >= 3 ? argv[2] : NULL);
    return 0;
}

// file analysis command handler
int file_anal(int argc, char *argv[]) {
    if (argc < 3) {
        printf("You need to provide a file (or files) to analyze");
        return 1;
    }

    for (size_t i = 2; i < (size_t)argc; i++) {
        printf("FILE: %s\n", argv[i]);
        analyze_file(argv[i]);
        printf("\n");
    }

    return 0;
}

const Command help_command = {"help", "h", "", {"display this help"}, 1, print_help};
const Command commands[] = {
    {"live", "l", "", {"start live analysis"}, 1, live_anal},
    {"capture", "c", "[file]", {"start capturing packers to file", "If file isn't provided - captures to: $HOME/.local/share/branal/captures"}, 2, capture},
    {"file", "f", "<file...>", {"analyze the specified file(s)"}, 1, file_anal},
    help_command
};
const size_t commands_count = ARR_LEN(commands);

int is_command(Command command, char *str) {
    return !strcmp(command.short_name, str) || !strcmp(command.name, str);
}

void command_not_recognized(const char *prog_name) {
    printf("Command not recognized\n");
    printf("Use \"%s (%s | %s)\" for help\n", prog_name, help_command.short_name, help_command.name);
}
