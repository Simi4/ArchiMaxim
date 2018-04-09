#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "archi.h"

int main(int argc, char *argv[])
{
	if (argc == 4) {

		if (strcmp(argv[1], "--pack") == 0) {
			pack_dir(argv[2], argv[3]);
			puts("Success packing!");
			exit(EXIT_SUCCESS);
		}

		if (strcmp(argv[1], "--unpack") == 0) {
			unpack_dir(argv[2], argv[3]);
			puts("Success unpacking!");
			exit(EXIT_SUCCESS);
		}

	}

	puts("");
	puts("Example command for packing:");
	printf("%s --pack test_dir test_archive.bin\n", argv[0]);
	puts("");
	puts("Example command for unpacking:");
	printf("%s --unpack test_archive.bin new_test_dir\n", argv[0]);
	puts("");

	exit(EXIT_SUCCESS);
}
