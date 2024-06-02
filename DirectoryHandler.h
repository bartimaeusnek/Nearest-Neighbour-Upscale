#ifndef NEARESTNEIGHBOURUPSCALE_DIRECTORYHANDLER_H
#define NEARESTNEIGHBOURUPSCALE_DIRECTORYHANDLER_H

#include "MimallocOverrideCOnly.h"

#if defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__)
#include "unistd.h"
#include "dirent.h"
#else
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX_PATH_LENGTH 1024

typedef struct FileNode {
    char *path;
    struct FileNode *next;
} FileNode;

void free_file_list(FileNode *head);
FileNode* add_file_to_list(FileNode *head, const char *path);
FileNode* list_files_recursive(const char *base_path, FileNode *head, size_t *file_count);
FileNode* list_files(const char *base_path, size_t *file_count);
bool is_absolute_path(const char* path);
char* make_absolute_path(const char *path);
bool isDirectory(const char *path);
#endif //NEARESTNEIGHBOURUPSCALE_DIRECTORYHANDLER_H
