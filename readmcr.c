#include "usim.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>

#include "ucode.h"
#include "misc.h"

int showmcr;
int debug;
int needswap;

ucw_t ucode[16 * 1024];

int
dump_i_mem(int fd, int start, int size)
{
	int i;
	int loc;

	loc = start;
	for (i = 0; i < size; i++) {
		unsigned int w1;
		unsigned int w2;
		unsigned int w3;
		unsigned int w4;
		unsigned long long ll;

		w1 = read16(fd);
		w2 = read16(fd);
		w3 = read16(fd);
		w4 = read16(fd);
		ll =
			((unsigned long long) w1 << 48) |
			((unsigned long long) w2 << 32) |
			((unsigned long long) w3 << 16) |
			((unsigned long long) w4 << 0);

		if (showmcr)
			printf("%03o %016Lo\n", loc, ll);

		ucode[loc] = ll;
		loc++;
	}
	return 0;
}

int
dump_d_mem(int fd, int start, int size)
{
	int i;

	for (i = 0; i < size; i++) {
		read16(fd);
		read16(fd);
	}

	return 0;
}

int
dump_a_mem(int fd, int start, int size)
{
	int i;
	unsigned int v;

	printf("a-memory; start %o, size %o\n", start, size);
	for (i = 0; i < size; i++) {
		v = read32(fd);
		if ((i >= 0347 && i <= 0400) |
		    (i >= 0600 && i <= 0610) |
		    (i < 010)) {
			printf("%o <- %o\n", i, v);
		}
	}

	return 0;
}

int
dump_main_mem(int fd, int start, int size)
{
	read32(fd);
	printf("start %d, size %d\n", start, size);
	lseek(fd, 0, SEEK_CUR);
	return 0;
}

void
usage(void)
{
	fprintf(stderr, "usage: readmcr FILE>\n");
	fprintf(stderr, "dump a microcode file\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "  -b             swap bytes\n");
	fprintf(stderr, "  -d             extra debug info\n");
	fprintf(stderr, "  -s N           skip N * 32-bit values\n");
	exit(1);
}

extern char *optarg;
extern int optind;

int
main(int argc, char *argv[])
{
	int c;
	int fd;
	int done;
	int skip;

	showmcr = 0;
	needswap = 1;
	skip = 0;

	while ((c = getopt(argc, argv, "mbds:")) != -1) {
		switch (c) {
		case 'b':
			needswap = 0;
			break;
		case 'd':
			debug++;
			break;
		case 's':
			skip = atoi(optarg);
			break;
		case 'm':
			showmcr = 1;
			break;
		}
	}

	if (optind >= argc)
		usage();

	fd = open(argv[optind], O_RDONLY);
	if (fd) {
		int code;
		int start;
		int size;

		if (skip) {
			while (skip--)
				read32(fd);
		}

		done = 0;
		while (!done) {
			code = read32(fd);
			start = read32(fd);
			size = read32(fd);

			printf("code %d, start %o, size %o\n", code, start, size);

			switch (code) {
			case 1:
				printf("i-memory\n");
				dump_i_mem(fd, start, size);
				break;
			case 2:
				printf("d-memory\n");
				dump_d_mem(fd, start, size);
				break;
			case 3:
				printf("main-memory\n");
				dump_main_mem(fd, start, size);
				break;
			case 4:
				printf("a-memory\n");
				dump_a_mem(fd, start, size);
				done = 1;
				break;
			}
		}
	}

	exit(0);
}
