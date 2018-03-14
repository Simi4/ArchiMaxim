#pragma once

#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h>



typedef void (*file_func_ptr)(const char *, const char *, int);

size_t safe_find_files_in_dir(const char *rootpath, const char *name, int outbin_fd, file_func_ptr func);

int safe_creat(const char *pathname, mode_t mode);

int safe_creat_from_filepath(const char *pathname, mode_t mode);

int safe_open(const char *pathname, int flags);

void safe_mkdirs(const char *dir, mode_t mode);

void safe_read(int fd, void *buf, size_t count);

void safe_lseek(int fd, off_t offset, int whence);

void* safe_malloc(size_t size);

void safe_free(void *buf);

void safe_write(int fd, const void *buf, size_t count);

void safe_close(int fd);

size_t safe_get_file_size(int fd);

char* safe_join_str(const char *s1, const char *s2);

const char* safe_clear_path(const char *rootpath, const char *fullpath);

void SAFE_START_STAT();

void SAFE_END_STAT();
