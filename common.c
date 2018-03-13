#include "common.h"



int count_allocations;
int count_file_descriptors;



int safe_creat(const char *pathname, mode_t mode)
{  
	int fd = creat(pathname, mode);
	assert( fd != -1 );
	count_file_descriptors++;
	return fd;
}

int safe_open(const char *pathname, int flags)
{  
	int fd = open(pathname, flags);
	assert( fd != -1 );
	count_file_descriptors++;
	return fd;
}

void safe_mkdirs(const char *dir, mode_t mode)
{
    size_t len = strlen(dir);
    char *tmp = safe_malloc(len + 1);
	strcpy(tmp, dir);
	assert( tmp[len] == '\0' );
    for (size_t i = 1; i <= len; i++) {
   		if (tmp[i] == '/') {
        	tmp[i] = 0;
            mkdir(tmp, mode) == 0;
            tmp[i] = '/';
        }
	}
    mkdir(tmp, mode);
	safe_free(tmp);
}

int safe_creat_from_filepath(const char *pathname, mode_t mode)
{
	char *dir = safe_malloc(strlen(pathname));
	strcpy(dir, pathname);
	dirname(dir);
	safe_mkdirs(dir, mode);
	safe_free(dir);
	return safe_creat(pathname, mode);
}

size_t safe_find_files_in_dir(const char *name, int outbin_fd, file_func_ptr func)
{
    DIR *dir;
    struct dirent *entry;
	size_t filecount = 0;

    if (!(dir = opendir(name)))
        return filecount;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            char *path = safe_join_str(name, entry->d_name);
            filecount += safe_find_files_in_dir(path, outbin_fd, func);
			safe_free(path);
        } else {
            char *path = safe_join_str(name, entry->d_name);
            func(path, outbin_fd);
			safe_free(path);
			filecount++;
        }
    }
    closedir(dir);
	return filecount;
}

void safe_read(int fd, void *buf, size_t count)
{  
	assert( read(fd, buf, count) == count );
}

void safe_lseek(int fd, off_t offset, int whence)
{
	assert( lseek(fd, offset, whence) != (off_t)-1 );
}

void* safe_malloc(size_t size)
{
	void *buf = malloc(size);
	assert( buf != NULL );
	count_allocations++;
	return buf;
}

void safe_free(void *buf)
{
	assert( buf != NULL );
	free(buf);
	count_allocations--;
}

void safe_write(int fd, const void *buf, size_t count)
{  
	assert( write(fd, buf, count) == count );
}

void safe_close(int fd)
{  
	assert( close(fd) == 0 );
	count_file_descriptors--;
}

size_t safe_get_file_size(int fd)
{  
	struct stat st;
	assert( fstat(fd, &st) == 0 );
	return st.st_size;
}

char* safe_join_str(const char *s1, const char *s2)
{
	size_t len = strlen(s1) + strlen(s2) + 1;
	char *path = safe_malloc(len + 1);
	assert( sprintf(path, "%s/%s", s1, s2) == len );
	assert( path[len] == '\0' );
	return path;
}

void SAFE_START_STAT()
{
	count_allocations = 0;
	count_file_descriptors = 0;
}

void SAFE_END_STAT()
{
	assert( count_allocations == 0 );
	assert( count_file_descriptors == 0 );
}
