#include <stdio.h>
#include <stdlib.h>
#include "DirectoryHandler.h"
#include "NearestNeighbourUpscaleDriver.h"

int main(int argc, char *argv[]) {
    if(argc != 3) {
        if (argc != 2) {
            fprintf(stderr, "No input filename, and/or no scale provided.\nUsage: ./NearestNeighbourUpscale <INPUT_IMAGE/DIRECTORY> <SCALE>\n");
            return EXIT_FAILURE;
        }
    }
    char *filename = argv[1];
    char* mallocedFilename = NULL;
    if (!is_absolute_path(filename)) {
        mallocedFilename = make_absolute_path(filename);
    }

    int_fast32_t scale = 2;

    if (argc == 3) {
        char *temp;
        size_t value = strtol(argv[2], (char **) &temp, 10);
        if (temp != argv[2] && *temp == '\0') {
            scale = (int_fast32_t) value;
            if (scale < 1) {
                fprintf(stderr,
                        "Invalid data in scale argument. Must be a non-zero integer.\nUsage: ./NearestNeighbourUpscale <INPUT_IMAGE/DIRECTORY> <SCALE>\n");
                return EXIT_FAILURE;
            }
        } else {
            fprintf(stderr,
                    "Invalid data in scale argument. Must be a non-zero integer.\nUsage: ./NearestNeighbourUpscale <INPUT_IMAGE/DIRECTORY> <SCALE>\n");
            return EXIT_FAILURE;
        }
    }
    if (isDirectory(mallocedFilename)) {
        size_t noFiles = 0;
        FileNode *files = list_files(mallocedFilename, &noFiles);

#pragma omp parallel for
        for(size_t fileIndex = 0; fileIndex < noFiles; ++fileIndex) {
            FileNode *current = files;
            for (size_t i = 0; i < fileIndex; ++i) {
                current = current->next;
            }
            UpscaleImage(current->path, scale);
        }

        free_file_list(files);
    }
    else
        UpscaleImage(mallocedFilename, scale);

    if (mallocedFilename != NULL)
        free(mallocedFilename);

    return EXIT_SUCCESS;
}