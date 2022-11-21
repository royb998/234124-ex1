/* ---------- Includes ---------- */

#include <assert.h>
#include <string.h>
#include <malloc.h>
#include "AsciiArtTool.h"

/* ---------- Macros ---------- */

#define ASSERT_ARG(ptr) do {            \
    if (NULL == (ptr))                  \
    {                                   \
        return RLE_LIST_NULL_ARGUMENT;  \
    }                                   \
} while (0)

#define ASSERT_RESULT(result) do {      \
    if (RLE_LIST_SUCCESS != (result))   \
    {                                   \
        return (result);                \
    }                                   \
} while (0)

/* ---------- Functions ---------- */

RLEList asciiArtRead(FILE* in_stream)
{
    RLEList list = RLEListCreate();
    char current = '\0';

    /* TODO: Free list and return NULL. */
    if (NULL == in_stream)
    {
        free(list);
        return NULL;
    }

    do
    {
        int result = fread(&current, sizeof(current), 1, in_stream);
        if (result < 1)
        {
            break;
        }

        RLEListAppend(list, current);
    } while (EOF != current);

    return list;
}

RLEListResult asciiArtPrint(RLEList list, FILE* out_stream)
{
    RLEListResult result = RLE_LIST_ERROR;
    char * exported = NULL;
    char current = '\0';
    int write_size = 0;

    ASSERT_ARG(list);
    ASSERT_ARG(out_stream);

    exported = RLEListExportToString(list, &result);
    ASSERT_RESULT(result);
    assert(exported);

    /* TODO: Improve loop. */
    for (int i = 0; i < RLEListSize(list); ++i)
    {
        current = RLEListGet(list, i, &result);
        ASSERT_RESULT(result);
        assert(current != 0);

        write_size = fwrite(&current, sizeof(current), 1, out_stream);
        if (1 > write_size)
        {
            return RLE_LIST_ERROR;
        }
    }

    return result;
}

RLEListResult asciiArtPrintEncoded(RLEList list, FILE* out_stream)
{
    RLEListResult result;
    char * buffer = NULL;
    size_t output_size = 0;
    int write_result = 0;

    ASSERT_ARG(list);
    ASSERT_ARG(out_stream);

    buffer = RLEListExportToString(list, &result);
    ASSERT_RESULT(result);
    assert(NULL != buffer);

    output_size = strlen(buffer);
    write_result = fwrite(buffer, sizeof(*buffer), output_size, out_stream);
    if (write_result < output_size)
    {
        return RLE_LIST_ERROR;
    }

    return RLE_LIST_SUCCESS;
}

