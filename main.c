#include "AsciiArtTool.h"

/* ---------- Macros ---------- */

#define CLOSE_FILE(ptr) do {    \
    if (NULL != (ptr))          \
    {                           \
        fclose(ptr);            \
        ptr = NULL;             \
    }                           \
} while (0)

/* ---------- Functions ---------- */


/**
 * Save the image in path `source` into a file in path `target` as an encoded
 * ascii art.
 *
 * @param source: File handle to source ascii image.
 * @param target: File handle to write encoded image into.
 * */
void encode_image(FILE * source, FILE * target)
{
    RLEList list = NULL;
    RLEListResult result;

    list = asciiArtRead(source);
    if (NULL == list)
    {
        return;
    }

    result = asciiArtPrintEncoded(list, target);
    if (RLE_LIST_SUCCESS != result)
    {
        /* TODO: Error output? */
        RLEListDestroy(list);
        return;
    }

    RLEListDestroy(list);
}


/**
 * Invert the given character, such that every space (' ') is replaced with an
 * "at" sign ('@') and vice-versa. Any other character is mapped to itself.
 *
 * @param c: Character to map.
 *
 * @return: Inverted character.
 * */
char invert_ascii(char c)
{
    switch (c)
    {
        case ' ':
            return '@';
        case '@':
            return ' ';
        default:
            return c;
    }
}


/**
 * Save the image in path `source` into a file in path `target` as an inverted
 * ascii art.
 *
 * @param source: File handle to source ascii image.
 * @param target: File handle to write inverted image into.
 * */
void invert_image(FILE * source, FILE * target)
{
    RLEList list = NULL;
    RLEListResult result;

    list = asciiArtRead(source);
    if (NULL == list)
    {
        return;
    }

    result = RLEListMap(list, &invert_ascii);
    if (RLE_LIST_SUCCESS != result)
    {
        RLEListDestroy(list);
        return;
    }

    /* TODO: Check result? */
    asciiArtPrint(list, target);

    RLEListDestroy(list);
}


/**
 * Open the source and target files for the tool.
 *
 * @param source_path: Path to source file to open.
 * @param target_path: Path to target file to open.
 * @param OUT source_file: File handler for the source file.
 * @param OUT target_file: File handler for the target file.
 * */
void open_files(
        char * source_path,
        char * target_path,
        FILE ** source_file,
        FILE ** target_file)
{
    FILE * source_out = NULL;
    FILE * target_out = NULL;

    source_out = fopen(source_path, "rt");
    if (NULL == source_out)
    {
        *source_file = NULL;
        *target_file = NULL;
        return;
    }

    target_out = fopen(target_path, "wt");
    if (NULL == target_out)
    {
        CLOSE_FILE(source_out);
        *target_file = NULL;
        return;
    }

    *source_file = source_out;
    *target_file = target_out;
}


int main(int argc, char * argv[])
{
    char * mode = NULL;
    FILE * source_file = NULL;
    FILE * target_file = NULL;

    if (4 != argc)
    {
        /* TODO: Remove usage/find required print. */
        printf("USAGE: asciiArtTool FLAG SOURCE TARGET\n");
        return 0;
    }
    mode = argv[1];

    open_files(argv[2], argv[3], &source_file, &target_file);
    if (NULL == source_file || NULL == target_file)
    {
        CLOSE_FILE(source_file);
        CLOSE_FILE(target_file);
    }

    if (mode[0] != '-')
    {
        /* TODO: Error message? Non-zero output? */
        return 0;
    }


    switch(mode[1])
    {
        case 'e':
            encode_image(source_file, target_file);
            break;
        case 'i':
            invert_image(source_file, target_file);
            break;
    }

    CLOSE_FILE(source_file);
    CLOSE_FILE(target_file);
    return 0;
}
