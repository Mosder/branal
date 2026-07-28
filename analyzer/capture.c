#include "capture.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "headers/interface.h"
#include "headers/server.h"
#include "utils/fs.h"
#include "utils/time.h"

void capture_to_file(char *path) {
    char command[128];
    sprintf(command, "tshark -i %s -f \"host %s\" -w %s", INTERFACE, SERVER_IP, path);
    system(command);
}

void capturing_mode(char *path) {
    if (path != NULL) {
        capture_to_file(path);
        return;
    }

    char capture_dir[PATH_MAX], new_path[PATH_MAX], date_time[128];
    get_capture_dir(capture_dir);
    get_date_time_string(date_time);
    sprintf(new_path, "%s/capture-%s.pcap", capture_dir, date_time);
    capture_to_file(new_path);
}
