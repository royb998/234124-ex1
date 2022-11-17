///
/// Created by Roy on 2022-11-17
///

#ifndef HW1_ASCII_ART_TOOL_H
#define HW1_ASCII_ART_TOOL_H

/* ---------- Imports ---------- */

#include "RLEList.h"

/* ---------- Functions ---------- */

/**
 * Read the given input file `in_stream` and save it as am RLEList.
 *
 * @param in_stream: File handle for input to read data to compress.
 *
 * @return
 *  RLEList instance.
 * */
RLEList asciiArtRead(FILE * in_stream);

/**
 * Print out the full image represented in `list` into `out_stream`.
 *
 * @param list: Image to print, compressed with RLE.
 * @param out_stream: Stream handle to print into.
 *
 * @return
 *  TODO
 * */
RLEListResult asciiArtPrint(RLEList list, FILE *out_stream);

/**
 * Encode the image represented in `list` and print it into `out_stream`.
 *
 * @param list: Image to encode, compressed with RLE.
 * @param out_stream: Stream handle to print into.
 *
 * @return
 *  RLE_LIST_NULL_ARGUMENT - If any parameter is NULL.
 *  RLE_LIST_SUCCESS - In case of success.
 * */
RLEListResult asciiArtPrintEncoded(RLEList list, FILE *out_stream);

#endif
