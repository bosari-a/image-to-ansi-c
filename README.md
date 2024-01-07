# Images to ANSI (and HTML)

## Description

This project is a command-line tool that allows you to create ANSI art from images.

## Features

- Simple command with two arguments.
- Outputs both ANSI and HTML art.

## Installation

- Step 1 - Download or copypaste the `process_img` bash script file.
- ⭕Important⭕ Run `$ sudo mv process_img /usr/local/bin/` in order to make the command available globally to your shell.
- Step 2 - Compile the C code in `toansi.c` or download the executable binary.
- (Optional) you can also `$ sudo mv toansi /usr/local/bin/`
if you want to use the command globally.

## Usage

The command is quite simple:

```bash
$ toansi {path/to/image} option
$ toansi ./img.png ansi # example
$ Available options: [ansi] [html]
```

You may also find it useful to redirect the HTML output into an html file. For example: `$ toansi some_img.jpeg html > index.html`

## Example results

<div style="display:flex;gap:10px;padding:5px">
<img style="width:250px" src="https://github.com/bosari-a/image-to-ansi-c/assets/kirby_ansi.png">
<img style="width:250px" src="https://github.com/bosari-a/image-to-ansi-c/assets/kirby.png">
</div>


<div style="display:flex;gap:10px;padding:5px">
<img style="width:250px" src="https://github.com/bosari-a/image-to-ansi-c/assets/eg_bocchi_ansi.png">
<img style="width:250px" src="https://github.com/bosari-a/image-to-ansi-c/assets/bocchi.png">
</div>


<div style="display:flex;gap:10px;padding:5px">
<img style="width:250px" src="https://github.com/bosari-a/image-to-ansi-c/assets/flag_ansi.png">
<img style="width:250px" src="https://github.com/bosari-a/image-to-ansi-c/assets/FLAG_B24.BMP">
</div>

## Process explanation

The C executable uses the `system` function from `stdlib.h` to call the `process_img` bash script. The `process_img` script looks at the image path argument and converts the image to a 24 bit RGB truecolor BMP Windows 3.x 35 pixel wide bitmap file.

The executable then continues if the temporary BMP file is successfully created. It parses the BMP file and creates the ANSI or HTML art from it.

The temporary BMP file is removed after execution.

No libraries were used to parse the BMP file. All the parsing code and code in `bmp.h` are written by me with the help of researching the BMP 3.x spec and analyzing the CS50x filter-more problem set `bmp.h` file.

## Challenges

- When using `fread` at some point I encountered strange "memory bugs". I discovered later with `printf(sizeof(SOME_STRUCT))` that C structs are padded in memory. So I had to use `__attribute__((__packed__))` in order to disable the padding behaviour.

- According to the BMP spec, BMP files may have padding in their rows:
    > Each scan line is zero padded to the nearest 4-byte boundary. If the image has a width that is not divisible by four, say, 21 bytes, there would be 3 bytes of padding at the end of every scan line.

    The challenge for me was to `fseek` through the padding since I am using `fread` one `ROW` at a time. This was made a bit more difficult due to the fact that the padding was variable.

    I tried writting various padding values by hand and I got something like this:

    | width (pixels) | padding (bytes) |
    | -------------- | --------------- |
    | 2              | 2               |
    | 3              | 3               |
    | 4              | 0               |
    | 5              | 1               |
    | 6              | 2               |
    | 7              | 3               |
    | 8              | 0               |
    | 9              | 1               |
    | .              | .               |
    | .              | .               |
    | .              | .               |
    | `w`            |`(4 - ((wp * 3) % 4)) % 4`|


    I came to the conclusion that the padding in the rows of a 24 bit truecolor BMP file according to the spec was given by:
    ```
    (4 - ((wp * 3) % 4)) % 4
    ```

    Here is an explanation. According to the BMP specification, a row would be padded if the width (in bytes) was not divisible by 4.
    I.e.: "If the image has a width that is not divisible by four, say, 21 bytes, there would be 3 bytes of padding at the end of every scan line." - [Reference.](https://www.ece.ualberta.ca/~elliott/ee552/studentAppNotes/2003_w/misc/bmp_file_format/bmp_file_format.htm)
    
    Consider:
    ```
    let wp = width in pixels
    let wb = width in bytes
    let pb = padding in bytes

    Consider any positive integer n. Then the following statements are true:
    n % 4 = r (some remainder).
    (n - r) % 4 = 0
    In spoken terms: any number minus its remainder that you get from dividing the number by 4 would result in something divisible by 4. E.g: 
    15 % 4 = 3 (same as 15 - 3 * 4 = 3)
    15 - 3 = 12 which is divisible by 4.

    Now padding (in bytes) is *added* to a row in the BMP image. So we need to flip around: instead of subtracting to get something divisible by 4 as we did above we need to add something to n to get something divisible by 4. And that is really simple. It's just 4 minus the remainder. E.g (continuing above 15 % 4):
    4 - 3 = 1 (i.e. 4 - remainder of 15/4 is 1)
    1 + 15 = 16 which is divisible by  4 and would be appropriate padding for a BMP with 15 bytes width.

    So:
    (4 - ((wp * 3) % 4)) is the padding (where wp * 3 is the width in bytes: wb = wp * 3). 
    
    But, there is a small problem. The values from the above formula are always between 0 and 4 (inclusive). For the case when the remainder (wb % 4) is 0 you will find out that the result is 4. This is nonesensical as you don't need padding when the width (in bytes) is divisible by 4 according to the BMP specification. So the formula becomes:

    pb = (4 - ((wp * 3) % 4)) % 4

    This extra % 4 eliminates the case when pb = 4 since 4 % 4 is 0.
    ```

