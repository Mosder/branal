#include <stdio.h>
#include <string.h>

#include "analyzer/capture.h"
#include "analyzer/utils/fs.h"

// prints help
void print_help() {
    printf("A bare-bones analyzer for the game Broken Ranks\n");
    printf("\nUsage:\n");
    printf("  branal                      - start live analysis\n");
    printf("  branal (l | live)           - start live analysis (same as above)\n");
    printf("  branal (c | capture) [file] - start capturing packets to [file].\n");
    printf("                                If [file] not given - captures to $HOME/.local/share/branal/captures\n");
    printf("  branal (f | file) <file>    - analyze the specified <file>\n");
    printf("  branal (h | help)           - display this help\n");
}

// starts live analyzer mode
void start_live_analyzer() {
    // TODO: implement
    printf("Not implemented\n");
}

// analyzes the specified file
// params:
// 	- file_path - path to the .pcap file to be analyzed
void analyze_file(char *file_path) {
    // TODO: implement
    printf("Not implemented\n");
}

int main(int argc, char *argv[]) {
    create_necessary_directories();

    // start live analysis if no arguments provided
    if (argc < 2) {
        start_live_analyzer();
        return 0;
    }

    if (strcmp("l", argv[1]) == 0 || strcmp("live", argv[1]) == 0) {
        start_live_analyzer();
    }
    else if (strcmp("c", argv[1]) == 0 || strcmp("capture", argv[1]) == 0) {
        capturing_mode(argc >= 3 ? argv[2] : NULL);
    }
    else if (strcmp("h", argv[1]) == 0 || strcmp("help", argv[1]) == 0) {
        print_help();
    }
    else if (argc >= 3 && (strcmp("f", argv[1]) == 0 || strcmp("file", argv[1]) == 0)) {
        analyze_file(argv[2]);
    }
    else {
        printf("Command not recognized\n");
        printf("Use \"branal (h | help)\" for help\n");
        return 1;
    }

    return 0;
}
