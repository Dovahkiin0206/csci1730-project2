#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define BUFFSIZE 4096

/**
 *This function returns a file descriptor to a text file containing stdin text.
 */
int tempfile() {

    int fd;
    char buf[BUFFSIZE];
    int n;
    fd = open("testfile.txt", O_CREAT | O_RDWR);

    while((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0) {

        if (write(fd, buf, n) != n) {

            perror("write (stdin)");

        } // if

    } // while

    lseek(fd, 0, SEEK_SET);

    return fd;

} // tempfile

/**
 *@param fd the file descriptor
 *
 *This function returns the number of bytes.
 */
int nBytes(int fd) {

    char buf[BUFFSIZE];
    int n;
    int numBytes = 0;

    while ((n = read(fd, buf, BUFFSIZE)) > 0) {

        for (int i = 0; i < n; i++) {

            numBytes++;

        } // for

    } // while

    lseek(fd, 0, SEEK_SET);
    return numBytes;

} // nBytes

/**
 *@param fd the file descriptor
 *
 *This function returns the number of lines.
 */
int nLines(int fd) {

    char buf[BUFFSIZE];
    int n;
    int numLines = 0;

    while ((n = read(fd, buf, BUFFSIZE)) > 0) {

        for (int i = 0; i < n; i++) {

            if (buf[i] == '\n') {
                numLines++;
            }

        } // for

    } // while

    lseek(fd, 0, SEEK_SET);
    return numLines;

} // nLines

/**
 *@param fd the file descriptor
 *
 *This function returns the number of words.
 */
int nWords(int fd) {

    char buf[BUFFSIZE];
    int n;
    int numWords = 0;

    while ((n = read(fd, buf, BUFFSIZE)) > 0) {

        for (int i = 0; i < n; i++) {

            if (buf[i] == ' ' || buf[i] == '\n') {

                numWords++;

            }

        } // for

    } // while

    lseek(fd, 0, SEEK_SET);
    return numWords;

} // nBytes

/**
 *The main function.
 */
int main(int argc, char* argv[]) {

    int opt, fd;
    int cTog = 0, lTog = 0, wTog = 0;

    while((opt = getopt(argc, argv, ":clw")) != -1) {

        switch (opt) {

        case 'c':
            cTog = 1;
            break;

        case 'w':
            wTog = 1;
            break;

        case 'l':
            lTog = 1;
            break;

        case '?':
            perror("Invalid Flag");
            exit(EXIT_FAILURE);


        } // switch


    } //while

    if (cTog == 0 && wTog == 0 && lTog == 0 ) {
        cTog = 1, wTog = 1, lTog = 1;
    } // if

    if (argc - optind == 0) { // if no arguments past the flag

        fd = tempfile();


        if (cTog == 1) {
            printf("Num bytes: %d\n", nBytes(fd));
        } if (wTog == 1) {
            printf("Num words: %d\n", nWords(fd));
        } if (lTog == 1) {
            printf("Num lines: %d\n", nLines(fd));
        } // if/else chain

        close(fd);
        remove("testfile.txt"); // removing temporary file

    } else {

        int std = 0;

        for(; optind < argc; optind++) { // Leftover arguments

            std = 0;

            if (*argv[optind] == '-') { // if stdin

                fd = tempfile();
                std = 1;

            } else { // if filename

                fd = open(argv[optind], O_RDONLY);

                if (fd == -1) {

                    perror("read");

                }

            }  // else

            if (cTog == 1) {
                printf("Num bytes: %d\n", nBytes(fd));
            } if (wTog == 1) {
                printf("Num words: %d\n", nWords(fd));
            } if (lTog == 1) {
                printf("Num lines: %d\n", nLines(fd));
            } // if/else chain

            close(fd);

            if (std == 1)
                remove("testfile.txt");

        } //for

    } // else

} // main
