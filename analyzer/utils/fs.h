// utils/fs.h
// functions for operating on the file system, i.e. creating directories, writing files, etc.

#include <sys/types.h>

// get main directory used by the program
// params:
//      - main_dir - buffer to save the directory to
extern void get_main_dir(char *main_dir);

// get directory to save the .pcap files to
// params:
//      - capture_dir - buffer to save the directory to
extern void get_capture_dir(char *capture_dir);

// get directory of save data of the program
// params:
//      - saves_dir - buffer to save the directory to
extern void get_saves_dir(char *saves_dir);

// create directories that will be used by the program
extern void create_necessary_directories();

// writes file to the specified path
// params:
//      - path - path to write the file to
//      - contents - contents of the file
//      - contents_len - amount of bytes of contents to write
//      - flags - flags to open the file with
//      - permissions - permissions of the written file
extern void write_file(char *path, void *contents, int contents_len, int flags, mode_t permissions);
