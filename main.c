#include "commands.h"
#include "utils/fs.h"

int main(int argc, char *argv[]) {
    create_necessary_directories();

    // start first command if no arguments provided
    if (argc < 2)
        return commands[0].handler(argc, argv);

    for (size_t i = 0; i < commands_count; i++) {
        if (is_command(commands[i], argv[1]))
            return commands[i].handler(argc, argv);
    }

    command_not_recognized();
    return 1;
}
