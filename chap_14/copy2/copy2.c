/**********************************************************************
 * Why does this dump core instead of printing an error message,
 * if it can't open an input file?
 **********************************************************************/

/**********************************************************************
 * Don't know, because it doesn't do that for me, or at least if
 * it dumps core it doesn't leave a core *file* behind. But it does
 * bitch about the arguments passed to fprintf on startup, so
 * it may be related to that?
 *
 * Oh duh. It's because stderr (or other filehandle) is not the
 * first arg to fprintf.
 **********************************************************************/

/**********************************************************************
 * copy -- copies one file to another.
 *
 * Usage
 *     Copy <from> <to>
 *
 * <from> -- The file to copy from.
 * <to>   -- The file to copy into.
 **********************************************************************/

#include <stdio.h>

#ifndef __MSDOS__           /* if we are not MS-DOS, then we */
#define __UNIX__            /* are UNIX. */
#endif /* __MSDOS__ */

#include <stdlib.h>

#ifdef __UNIX__
#include <sys/types.h>      /* file defines for UNIX systems */
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif /* __UNIX__ */

#ifdef __MSDOS__
#include <fcntl.h>          /* file defines for DOS filesystems */
#include <sys\stat.h>
#include <io.h>
#endif

#ifndef O_BINARY
#define O_BINARY 0          /* Define the flag if not defined yet */
#endif /* O_BINARY */

#define BUFFER_SIZE (16 * 1024)     /* Use 16K buffers */

int main(int argc, char *argv[]) {
	char buffer[BUFFER_SIZE];       /* buffer for data */
	int in_file;                    /* input file descriptor */
	int out_file;                   /* output file descriptor */
	int read_size;                  /* number of bytes on last read */

	if (argc != 3) {
		fprintf(stderr, "Error: Wrong number of arguments\n");
		fprintf(stderr, "Usage is: copy <from> <to>\n");
		exit(8);
	}

	in_file = open(argv[1], O_RDONLY|O_BINARY);
	if (in_file < 0) {
		/*fprintf("Error: Unable to open %s\n", argv[1]);*/
		/* need to add a filehandle here */
		fprintf(stderr, "Error: Unable to open %s\n", argv[1]);
		exit(8);
	}

	out_file = open(argv[2], O_WRONLY|O_TRUNC|O_CREAT|O_BINARY, 0666);
	if (out_file < 0) {
		/*fprintf("Error: Unable to open %s\n", argv[2]);*/
		/* need to add a filehandle here */
		fprintf(stderr, "Error: Unable to open %s\n", argv[2]);
		exit(8);
	}

	while (1) {
		read_size = read(in_file, buffer, sizeof(buffer));

		if (read_size == 0) {
			break;              /* end of file */
		}

		if (read_size < 0) {
			fprintf(stderr, "Error: read error\n");
			exit(8);
		}

		write(out_file, buffer, (unsigned int)read_size);
	}

	close(in_file);
	close(out_file);

	return(0);
}

