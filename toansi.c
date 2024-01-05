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
#define RESIZE 35
// path to resized image
#define BMP_IMG "./tmp.bmp"

int main(int argc, char *argv[])
{
    /**
     * This section sets up the BMP 3.x format image
     * argument (can be any image, e.g.  png, jpeg, etc...)
     * is converted to BMP3 and resized to size RESIZE
     */

    // check correct usage
    if (argc != 3 || (strcmp(argv[2], "ansi") != 0 && strcmp(argv[2], "html") != 0))
    {
        printf(RED "Incorrect usage\n" RESET);
        printf(YELLOW "Usage: " RESET "toansi {path/to/image} option\n");
        printf(BLUE "\tExample: toansi" GREEN " ./img.png " YELLOW "ansi\n" RESET);
        printf("Available options: " YELLOW "[ansi] [html]\n" RESET);
        exit(1);
    }

    // create bmp conversion command
    char *to_bmp = calloc(1024, 1);
    sprintf(to_bmp, "process_img %s %i %s", argv[1], RESIZE, BMP_IMG);
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

    typedef struct
    {
        RGB rgb[w];
    } __attribute__((__packed__))
    ROW;

    ROW img[h];
    for (int32_t i = 0; i < h; i++)
    {
        ROW row;
        fread(&row, sizeof(RGB), w, fp);
        img[i] = row;

        /**
         * Each scan line is zero padded to the nearest 4-byte
         * boundary. If the image has a width that is not divisible
         * by four, say, 21 bytes, there would be 3 bytes of padding
         * at the end of every scan line.
         * (From BMP spec)
         */
        fseek(fp, w % 4, SEEK_CUR);
    }
    // Scan lines are stored bottom to top instead of top to bottom (BMP spec).
    for (int32_t i = h - 1; i > 0; i--)
    {
        if (strcmp(argv[2], "html") == 0)
        {
            printf("<div class=\"char-container\">");
        }
        else
        {
            printf("\t");
        }
        for (int32_t j = 0; j < w; j++)
        {
            if (strcmp(argv[2], "ansi") == 0)
            {
                // ansi code spec: \x1b[38;2;r;g;bm - where r -> red, g -> green, b -> blue
                printf("\x1b[38;2;%d;%d;%dm%%" RESET, img[i].rgb[j].red, img[i].rgb[j].green, img[i].rgb[j].blue);
            }
            else
            {
                printf("<span style=\"color:rgba(%d,%d,%d)\">\u25fc</span>", img[i].rgb[j].red, img[i].rgb[j].green, img[i].rgb[j].blue);
            }
        }
        if (strcmp(argv[2], "ansi") == 0)
        {
            printf("\n");
        }
        else
        {
            printf("</div>");
        }
    }

    free(bf);
    free(bi);
    fclose(fp);

    int stat = remove(BMP_IMG);
    if (stat != 0)
    {
        printf("failed to remove " BMP_IMG " file created during process.\n");
        exit(4);
    }
}