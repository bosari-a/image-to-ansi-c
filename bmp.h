#include <stdint.h>

/**
 * Bitmap header structure
 * according to the BMP 3.x file specification
 */
typedef struct
{
    uint8_t signature[2];
    uint32_t file_size;
    uint32_t reserved;
    uint32_t data_offset;
} __attribute__((__packed__))
BITMAPFILEHEADER;

/**
 * Bitmap info header structure
 * according to the BMP 3.x file specification
 */
typedef struct
{
    uint32_t info_header_size;
    int32_t width;  // horizontal width in pixels
    int32_t height; // vertical height in pixels
    uint16_t planes;
    uint16_t bits_per_pixel;
    uint32_t compression;
    uint32_t image_size;     // (compressed)
    uint32_t x_pixels_per_m; // horizontal resolution pixels/meter
    uint32_t y_pixels_per_m; // vertital resolution pixels/meter
    uint32_t colors_used;
    uint32_t important_colors;

} __attribute__((__packed__))
BITMAPINFOHEADER;
/**
 * color tables present only if Info.BitsPerPixel
 * less than 8. here we only deal with 24 bit BMP files
 * so it is ignored
 */

/**
 * RGB values are stored backwards i.e. BGR.
 */
typedef struct
{
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} __attribute__((__packed__))
RGB;
