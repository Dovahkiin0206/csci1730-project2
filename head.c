#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define BUFFSIZE 4096

/**
 *@param fd the file descriptor to be read
 *@param numLines the number of lines to output
 *
 *This function prints a certain number of lines from a given file or stdin.
 */
int printLines(int fd, int numLines) {

    char buf[BUFFSIZE];
    int n;
    int newLineCount = 0; // ticker for line count

    if (numLines == 0) {

        return numLines;

    } // error checking

    while ((n = read(fd, buf, BUFFSIZE)) > 0) { //Reading file

        for (int i = 0; i < n; i++) { // for loop that writes character by character

            write(STDOUT_FILENO, &buf[i], sizeof(char)); // writing one char

            if (buf[i] == '\n') { // if the char is a newline

                ++newLineCount; // increment line count

            } // if

            if (newLineCount == numLines) { // if line count reached

                return newLineCount; //return

            } // if

        } //for

    } // while


    if (n == -1)
        perror("read"); // read error


    return EXIT_SUCCESS;

} //printLines

/**
 *@param fd the file descriptor
 *@param numBytes the number of bytes to print
 *
 *This prints a certain number of bytes given.
 */
int printBytes(int fd, int numBytes) {

    char buf[numBytes];
    int n;
    int byteCount = 0; // ticker for stdout

    if (fd == 0) { // standard input

        while ((n = read(fd, buf, BUFFSIZE)) > 0) { // loop to read each line

            for (int i = 0; i < n; i++) { // for loop writing charcater by character

                write(STDOUT_FILENO, &buf[i], 1); // writes one char
                byteCount++; //increments

                if (byteCount == numBytes) {

                    char format = '\n';
                    write(STDOUT_FILENO, &format, 1); // newline for formatting
                    return byteCount;

                } // if

            } // for

        } // while

    } else { // all files that arent stdout

        n = read(fd, buf, numBytes); // reading the file

        if (n == -1)
            perror("read");

        write(STDOUT_FILENO, buf, n);

    } // else

    return EXIT_SUCCESS;

} // printBytes

/**
 *
 *The main function.
 *
 */

int main(int argc, char* argv[]) {

    int fd; //file descriptor
    int opt; // used to determine the options
    int lineArg = 0, byteArg = 0; // storing whether or not the arguments were passed
    int lineNum, byteNum;

    while((opt = getopt(argc, argv, ":c:n:")) != -1) { //getOpt loop

        switch (opt) {

        case 'c':
            byteArg = 1;
            byteNum = atoi(optarg);
            break;
        case 'n':
            lineArg = 1;
            lineNum = atoi(optarg);
            break;
        case '?':
            perror("Invalid flag");
            exit(EXIT_FAILURE);

        } //switch

    } // while (getOpt)

    if (lineArg == 0 && byteArg == 0) { // if lineArg and byteArg are both 0 set defaults

        lineArg = 1;
        lineNum = 10;

    } // if

    if (lineArg == byteArg) { // if both flags are given

        perror("Only one flag (-c or -n) expected");
        exit(EXIT_FAILURE);

    } // if

    if (lineArg == 1) { // -n option

        if (argc - optind == 0) { // if no arguments (

            printf("\n-----> Standard Input <------\n");
            printLines(STDIN_FILENO, lineNum); // prints the linez

        } else { // not stdin

            for(; optind < argc; optind++) { // Leftover arguments (should be filenames)

                if (*argv[optind] == '-') { // if stdin

                    printf("\n-----> Standard Input <------\n");
                    printLines(STDIN_FILENO, lineNum);

                } else { // if filename

                    printf("\n-----> %s <------\n", argv[optind]);
                    fd = open(argv[optind], O_RDONLY);

                    if (fd == -1) {

                        perror("read");

                    } else {

                        printLines(fd, lineNum);
                        close(fd);

                    } // else

                } // else

            } // for


        } // if / else

    } else if (byteArg == 1) { //-c option

        if (argc - optind == 0) { //stdin

            printf("\n-----> Standard Input <------\n");
            printBytes(STDIN_FILENO, byteNum);


        } else { // if args given

            for(; optind < argc; optind++) {

                if (*argv[optind] == '-') { // if stdin

                    printf("\n-----> Standard Input <------\n");
                    printBytes(STDIN_FILENO, byteNum);

                } else {

                    printf("\n-----> %s <------\n", argv[optind]);
                    fd = open(argv[optind], O_RDONLY);

                    if (fd == -1) {

                        perror("read");

                    } else {

                        printBytes(fd, byteNum);
                        close(fd);

                    } // else

                } // else

            } // for


        } // if / else

    }
} // main
