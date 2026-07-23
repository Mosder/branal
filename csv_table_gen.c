#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#define EXT ".csv"
#define EXT_LEN strlen(".csv")
#define CSV_PATH "csv"
#define OUT_PATH "analyzer/" CSV_PATH

#define BUFFER_SIZE 256
#define MAX_ARR_SIZE 4096

// read values from input csv and save them to buffers
// returns max_index of the array
int read_csv(char *path, char *from, char *to, char from_to_array[][BUFFER_SIZE]) {
    FILE *fp = fopen(path, "r");

    // read field names
    fscanf(fp, "%[^,],%[^\n]", from, to);

    // read values
    size_t max_index = 0;
    size_t val1;
    char val2[BUFFER_SIZE];
    while (fscanf(fp, "%zu,%[^\n]", &val1, val2) == 2) {
        strcpy(from_to_array[val1], val2);
        if (val1 > max_index)
            max_index = val1;
    }

    fclose(fp);

    return max_index;
}

// create output .h file
void create_h(char *path, char *from, char *to) {
    FILE *fp = fopen(path, "w");

    // write comments and function prototype
    fprintf(fp, "// %s\n", strstr(path, CSV_PATH));
    fprintf(fp, "// translate %s to %s - generated from csv\n\n", from, to);
    fprintf(fp, "// get %s from %s\n", to, from);
    fprintf(fp, "// params:\n");
    fprintf(fp, "//      - %s - %s to translate from\n", from, from);
    fprintf(fp, "// returns:\n");
    fprintf(fp, "//      %s corresponding to %s\n", to, from);
    fprintf(fp, "extern const char *get_%s_from_%s(int %s);", to, from, from);

    fclose(fp);
}

// create output .c file
void create_c(char *path, char *h_path, char *from, char *to, char from_to_array[][BUFFER_SIZE], size_t max_index) {
    FILE *fp = fopen(path, "w");

    char arr_name[BUFFER_SIZE], arr_len_name[BUFFER_SIZE];
    sprintf(arr_name, "%s_to_%s", from, to);
    sprintf(arr_len_name, "%s_to_%s_len", from, to);

    // write includes and the beginning of array
    fprintf(fp, "#include \"%s\"\n\n", strstr(h_path, CSV_PATH));
    fprintf(fp, "#include <stdio.h>\n\n");
    fprintf(fp, "static const char *%s[] = {\n", arr_name);

    // write values
    for (size_t i = 0; i <= max_index; i++) {
        if (strlen(from_to_array[i]) > 0)
            fprintf(fp, "    \"%s\",\n", from_to_array[i]);
        else
            fprintf(fp, "    \"%zu\",\n", i);
    }

    // write the end of array and len
    fprintf(fp, "};\n");
    fprintf(fp, "int %s = %zu;\n\n", arr_len_name, max_index + 1);

    // write the getter
    fprintf(fp, "const char *get_%s_from_%s(int %s) {\n", to, from, from);
    fprintf(fp, "    if (%s >= 0 && %s < %s)\n", from, from, arr_len_name);
    fprintf(fp, "        return %s[%s];\n", arr_name, from);
    fprintf(fp, "    fprintf(stderr, \"%s = %%d is outside of table range [0; %%d]\\n\", %s, %s - 1);\n", from, from, arr_len_name);
    fprintf(fp, "    return \"null\";\n");
    fprintf(fp, "}");

    fclose(fp);
}

void create_table(char *file_name) {
    // read from input file
    char input_path[BUFFER_SIZE];
    sprintf(input_path, "%s/%s", CSV_PATH, file_name);
    char from[BUFFER_SIZE], to[BUFFER_SIZE];
    char from_to_array[MAX_ARR_SIZE][BUFFER_SIZE] = {0};
    size_t max_index = read_csv(input_path, from, to, from_to_array);

    // create output .h file
    char h_path[BUFFER_SIZE];
    sprintf(h_path, "%s/%s_to_%s.h", OUT_PATH, from, to);
    create_h(h_path, from, to);

    // create output .c file
    char c_path[BUFFER_SIZE];
    sprintf(c_path, "%s/%s_to_%s.c", OUT_PATH, from, to);
    create_c(c_path, h_path, from, to, from_to_array, max_index);
}

int main() {
    // make directory for output files
    mkdir(OUT_PATH, 0755);

    DIR *csv_dir = opendir(CSV_PATH);
    struct dirent *entry;
    while ((entry = readdir(csv_dir))) {
        char *file_name = entry->d_name;
        size_t file_name_len = strlen(file_name);
        // create lookup tables for each *.csv file
        if (file_name_len > EXT_LEN && !strcmp(file_name + file_name_len - EXT_LEN, EXT))
            create_table(entry->d_name);
    }
    closedir(csv_dir);

    return 0;
}
