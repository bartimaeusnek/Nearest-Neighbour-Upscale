#include "DirectoryHandler.h"

void free_file_list(FileNode *head) {
    FileNode *current = head;
    FileNode *next;

    while (current != NULL) {
        next = current->next;
        free(current->path);
        free(current);
        current = next;
    }
}

FileNode* add_file_to_list(FileNode *head, const char *path) {
    FileNode *new_node = (FileNode *)malloc(sizeof(FileNode));
    if (new_node == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    new_node->path = strdup(path);
    if (new_node->path == NULL) {
        perror("Failed to duplicate path");
        free(new_node);
        exit(EXIT_FAILURE);
    }
    new_node->next = head;
    return new_node;
}

FileNode* list_files_recursive(const char *base_path, FileNode *head, size_t *file_count) {
    struct dirent *dp;
    DIR *dir = opendir(base_path);

    if (dir == NULL) {
        perror("Failed to open directory");
        exit(EXIT_FAILURE);
    }

    char path[MAX_PATH_LENGTH];
    struct stat statbuf;

    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
            continue;

        snprintf(path, sizeof(path), "%s/%s", base_path, dp->d_name);

        if (stat(path, &statbuf) == -1) {
            perror("Failed to get file status");
            exit(EXIT_FAILURE);
        }

        if (S_ISDIR(statbuf.st_mode)) {
            head = list_files_recursive(path, head, file_count);
        } else if (S_ISREG(statbuf.st_mode)) {
            head = add_file_to_list(head, path);
            (*file_count)++;
        }
    }

    closedir(dir);
    return head;
}

FileNode* list_files(const char *base_path, size_t *file_count) {
    *file_count = 0;
    return list_files_recursive(base_path, NULL, file_count);
}

bool is_absolute_path(const char *path) {
    if (path == NULL) {
        return false;
    }

#ifdef _WIN32
    if ((isalpha(path[0]) && path[1] == ':') &&
        (path[2] == '\\' || path[2] == '/')) {
        return true;
    }
#else
    if (path[0] == '/') {
        return true;
    }
#endif

    return false;
}

char* make_absolute_path(const char *path) {
    if (path == NULL) {
        return NULL;
    }

#if defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__)
    char absolute_path[PATH_MAX];
    if (_fullpath(absolute_path, path, PATH_MAX) != NULL) {
        return strdup(absolute_path);
    }
#else
    char* realPathResult = realpath(path, NULL);
    if (realPathResult != NULL) {
        return realPathResult;
    }
#endif
    return NULL;
}

bool isDirectory(const char *path)
{
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return false;
    return S_ISDIR(statbuf.st_mode);
}