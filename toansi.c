#include <stdio.h>
#include <string.h>
#include "bmp.h"

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define BLUE "\x1b[34m"
#define YELLOW "\x1b[33m"
#define RESET "\x1b[0m"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf(RED "\tIncorrect usage\n" RESET);
        printf(YELLOW "\tUsage: " RESET);
    }
}