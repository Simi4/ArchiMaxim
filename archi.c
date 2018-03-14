#include "archi.h"
#include "common.h"

static void write_to_bin(const char *rootpath, const char *path, int outbin_fd)
{
	const char *newpath = safe_clear_path(rootpath, path);
	size_t pathlen = strlen(newpath);
	safe_write(outbin_fd, &pathlen, sizeof(size_t));
	safe_write(outbin_fd, newpath, pathlen);
	int in_fd = safe_open(path, O_RDONLY);
	size_t datalen = safe_get_file_size(in_fd);
	safe_write(outbin_fd, &datalen, sizeof(size_t));
	char *buf = safe_malloc(datalen);
	safe_read(in_fd, buf, datalen);
	safe_close(in_fd);
	safe_write(outbin_fd, buf, datalen);
	safe_free(buf);
}

void pack_dir(const char *indir, const char *outbin)
{
	SAFE_START_STAT();

	int outbin_fd = safe_creat(outbin, S_IRWXU);
	size_t filecount = safe_find_files_in_dir(indir, indir, outbin_fd, &write_to_bin);
	safe_write(outbin_fd, &filecount, sizeof(size_t));
	safe_close(outbin_fd);

	SAFE_END_STAT();
}

void unpack_dir(const char *inbin, const char *outdir)
{
	SAFE_START_STAT();

	int inbin_fd = safe_open(inbin, O_RDONLY);
	safe_lseek(inbin_fd, -sizeof(size_t), SEEK_END);
	size_t filecount;
	safe_read(inbin_fd, &filecount, sizeof(size_t));
	safe_lseek(inbin_fd, 0, SEEK_SET);

	for (size_t i = 0; i<filecount; i++) {
		size_t pathlen;
		safe_read(inbin_fd, &pathlen, sizeof(size_t));
		char *path = safe_malloc(pathlen + 1);
		safe_read(inbin_fd, path, pathlen);
		size_t datalen;
		safe_read(inbin_fd, &datalen, sizeof(size_t));
		void *buf = safe_malloc(datalen);
		safe_read(inbin_fd, buf, datalen);
		char *new_path = safe_join_str(outdir, path);

		int outf_fd = safe_creat_from_filepath(new_path, S_IRWXU);
		safe_write(outf_fd, buf, datalen);
		safe_close(outf_fd);

		safe_free(new_path);
		safe_free(path);
		safe_free(buf);
	}

	safe_close(inbin_fd);

	SAFE_END_STAT();
}
