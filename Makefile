all:
	gcc main.c archi.c common.c -o bin

san:
	gcc -fsanitize=address main.c archi.c common.c -o bin

checkstyle:
	perl /home/max/DEV/linux-4.14.22/scripts/checkpatch.pl -f *.h