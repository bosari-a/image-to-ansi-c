#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp.h"
// ansi escap codes for command-line messages
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define BLUE "\x1b[34m"
#define YELLOW "\x1b[33m"
#define RESET "\x1b[0m"
// resize image in argument
#define RESIZE 24

int main(int argc, char *argv[])
{
    /**
     * This section sets up the BMP 3.x format image
     * argument (can be any image, e.g.  png, jpeg, etc...)
     * is converted to BMP3 and resized to size RESIZE
     */

    // check correct usage
    if (argc != 3 || (strcmp(argv[2], "ansii") != 0 && strcmp(argv[2], "html") != 0))
    {
        printf(RED "Incorrect usage\n" RESET);
        printf(YELLOW "Usage: " RESET "toansi {path/to/image} option\n");
        printf(BLUE "\tExample: toansi" GREEN " ./img.png " YELLOW "ansii\n" RESET);
        printf("Available options: " YELLOW "[ansii] [html]\n" RESET);
        exit(1);
    }

    // create bmp conversion command
    char to_bmp[1024];
    sprintf(to_bmp, "bash ./process_img %s %i", argv[1], RESIZE);
    // call command
    system(to_bmp);

    // open generated bmp file
}