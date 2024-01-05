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
// path to resized image
#define BMP_IMG "./toansi.bmp"

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
    char *to_bmp = calloc(1024, 1);
    sprintf(to_bmp, "bash ./process_img %s %i", argv[1], RESIZE);
    // call command
    system(to_bmp);
    // free command
    free(to_bmp);

    // open generated bmp file
    FILE *fp = fopen(BMP_IMG, "r");

    if (fp == NULL)
    {
        printf(RED "Sorry, could not open " BMP_IMG " file\n" RESET);
        exit(2);
    }

    /**
     * at this point file exists. we can start parsing it
     */

    BITMAPFILEHEADER *bf = calloc(1, sizeof(BITMAPFILEHEADER));
    BITMAPINFOHEADER *bi = calloc(1, sizeof(BITMAPINFOHEADER));

    // read header and info header sections of bmp

    fread(bf, sizeof(BITMAPFILEHEADER), 1, fp);
    fread(bi, sizeof(BITMAPINFOHEADER), 1, fp);

    // signature of bmp file in file header
    char *sig = calloc(3, 1);
    sig[0] = bf->signature[0];
    sig[1] = bf->signature[1];

    if (bf->data_offset != 54 ||
        bi->compression != 0 || bi->bits_per_pixel != 24 ||
        (strcmp(sig, "BM") != 0))
    {
        printf(RED "Unsupported format\n" RESET);
        exit(3);
    }
    free(sig);

    int32_t h = bi->height;
    if (bi->height < 0)
    {
        h = -1 * bi->height;
    }

    int32_t w = bi->width;

    free(bf);
    free(bi);
}