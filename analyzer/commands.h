// commands.h
// commands to use in the program

#include <stddef.h>

#define MAX_EXPLANATION_LINES 3

typedef struct {
    char *name;
    char *short_name;
    char *args_str;
    char *explanation[MAX_EXPLANATION_LINES];
    size_t explanation_lines;
    int (*handler)(int argc, char *argv[]);
} Command;

// all commands of the program
extern const Command commands[];
extern const size_t commands_count;

// check if the given command is represented by the given string
// params:
//      - command - potentailly represented command
//      - str - potentially representing string
// returns:
//      1 if command is represneted by string, 0 if not
extern int is_command(Command command, char *str);

// print information about help when program didn't recognize command
// params:
//      - prog_name - name of the program (argv[0])
extern void command_not_recognized(const char *prog_name);
