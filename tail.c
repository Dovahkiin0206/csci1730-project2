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

    int std = 0;

    if (fd == 0) { //STDIN

        std = 1;
        fd = open("tempfile.txt", O_CREAT | O_RDWR); // Making a temp file to read STDIN

        if (fd == -1)
            perror("open");

        int tempN;
        char tempBuf[BUFFSIZE];

        while((tempN = read(0, tempBuf, BUFFSIZE)) > 0) {

            if (write(fd, tempBuf, tempN) != tempN) // Writing to temp file
                perror("write (temp)");

        } // while

    } // if

    lseek(fd, 0, SEEK_SET);


    char buf[BUFFSIZE];
    int n;
    int totLineCount = 0; // ticker for total line count

    if (numLines == 0) {

        return numLines;

    } // error checking

    while ((n = read(fd, buf, BUFFSIZE)) > 0) { //Reading file

        for (int i = 0; i < n; i++) { // for loop that writes character by character

            if (buf[i] == '\n') { // if the char is a newline

                ++totLineCount; // increment line count

            } // if

        } //for

    } // while

    lseek(fd, 0, SEEK_SET);

    if (n == -1)
        perror("read"); // read error


    int skipLines = totLineCount - numLines; // Number of lines to skip
    int tempLines = 0;


    while ((n = read(fd, buf, BUFFSIZE)) > 0) {

        for (int i = 0; i < n; i++) {

            if (buf[i] == '\n') // If a newline is found,
                tempLines++; // increment temp lines

            if(tempLines >= skipLines) { // once we have skipped....

                if((write(STDOUT_FILENO, &buf[i], 1)) != 1) // print
                    perror("write");

            } // if

        } // for

    } // while

    if (std == 1) {
        close(fd);
        remove("tempfile.txt"); //close and remove in case file was opened for stdin
    } // if

    return EXIT_SUCCESS;


} // printLines


/**
 *@param fd the file descriptor
 *@param numBytes the number of bytes to print
 *
 *This prints a certain number of bytes given.
 */
int printBytes(int fd, int numBytes) {

    int std = 0;

    if (fd == 0) { //STDIN

        std = 1;
        fd = open("tempfile.txt", O_CREAT | O_RDWR); // Making a temp file to read STDIN

        int tempN;
        char tempBuf[BUFFSIZE];

        while((tempN = read(0, tempBuf, BUFFSIZE)) > 0) {

            if (write(fd, tempBuf, tempN) != tempN) // Writing to temp file
                perror("write (temp)");

        } // while

    } // if

    lseek(fd, (numBytes * -1), SEEK_END); // finding the end minus however many bytes wanted

    int n;
    char buf[BUFFSIZE];


    while ((n = read(fd, buf, BUFFSIZE)) > 0) {

        if(write(STDOUT_FILENO, buf, n) != n)
            perror("write (main) ");

    } // while

    if (std == 1) {
        close(fd);
        remove("tempfile.txt");
    }

    return EXIT_FAILURE;

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

                    } // else (error checking

                } // else (std checking)

            } // for

        } // if / else (no files
    } // if else (byte arg vs line arg
    return 0;

} // main
