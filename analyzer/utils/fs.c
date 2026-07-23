#include "utils/fs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void get_home_dir(char *home_dir) {
    strcpy(home_dir, getenv("HOME"));
}

void get_main_dir(char *main_dir) {
    char home_dir[256];
    get_home_dir(home_dir);
    sprintf(main_dir, "%s/.local/share/branal", home_dir);
}

void get_capture_dir(char *capture_dir) {
    char main_dir[256];
    get_main_dir(main_dir);
    sprintf(capture_dir, "%s/captures", main_dir);
}

void get_saves_dir(char *saves_dir) {
    char main_dir[256];
    get_main_dir(main_dir);
    sprintf(saves_dir, "%s/saves", main_dir);
}

void create_necessary_directories() {
    char main_dir[256], capture_dir[256], saves_dir[256];
    get_main_dir(main_dir);
    get_capture_dir(capture_dir);
    get_saves_dir(saves_dir);
    mkdir(main_dir, 0755);
    mkdir(capture_dir, 0755);
    mkdir(saves_dir, 0755);
}
