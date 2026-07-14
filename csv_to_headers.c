#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#define EXT ".csv"
#define EXT_LEN strlen(".csv")
#define CSV_PATH "csv"
#define HEADER_PATH "analyzer/" CSV_PATH

#define BUFFER_SIZE 256
#define MAX_ARR_SIZE 4096

void create_header(char *file_name) {
    // open input file
    char input_path[BUFFER_SIZE];
    sprintf(input_path, "%s/%s", CSV_PATH, file_name);
    FILE *input_fp = fopen(input_path, "r");

    // read field names
    char from[BUFFER_SIZE], to[BUFFER_SIZE];
    fscanf(input_fp, "%[^,],%[^\n]", from, to);

    // read values from CSV
    char from_to_array[MAX_ARR_SIZE][BUFFER_SIZE] = {""};
    size_t max_index = 0;
    size_t val1;
    char val2[BUFFER_SIZE];
    while (fscanf(input_fp, "%zu,%[^\n]", &val1, val2) == 2) {
        strcpy(from_to_array[val1], val2);
        if (val1 > max_index)
            max_index = val1;
    }

    // close the input file
    fclose(input_fp);

    // create output file
    char output_path[BUFFER_SIZE];
    sprintf(output_path, "%s/%s_to_%s.h", HEADER_PATH, from, to);
    FILE *output_fp = fopen(output_path, "w");

    // write comments and beginning of output array
    fprintf(output_fp, "// %s\n", input_path);
    fprintf(output_fp, "// array translating %s to %s - generated from csv file\n\n", from, to);
    fprintf(output_fp, "char *%s_to_%s[] = {\n", from, to);

    // write values
    for (size_t i = 0; i <= max_index; i++) {
        if (strlen(from_to_array[i]) > 0)
            fprintf(output_fp, "    \"%s\",\n", from_to_array[i]);
        else
            fprintf(output_fp, "    \"%zu\",\n", i);
    }

    // write the end of array and len
    fprintf(output_fp, "};\n");
    fprintf(output_fp, "int %s_to_%s_len = %zu;", from, to, max_index + 1);

    // close the output file
    fclose(output_fp);
}

int main() {
    // make directory for csv header files
    mkdir(HEADER_PATH, 0755);

    DIR *csv_dir = opendir(CSV_PATH);
    struct dirent *entry;
    while ((entry = readdir(csv_dir))) {
        char *file_name = entry->d_name;
        size_t file_name_len = strlen(file_name);
        // create header for each *.csv file
        if (file_name_len > EXT_LEN && strcmp(file_name + file_name_len - EXT_LEN, EXT) == 0)
            create_header(entry->d_name);
    }
    closedir(csv_dir);
    return 0;
}
