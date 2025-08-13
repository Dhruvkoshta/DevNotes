#ifndef SPLIT_STRING_H
#define SPLIT_STRING_H

#include <stdlib.h> // Required for size_t and NULL

/**
 * @brief Splits a string at the first occurrence of a delimiter character.
 *
 *
 * @param str The string to be split.
 * @param delimiter The character used as the delimiter.
 * @return A dynamically allocated array of two char* strings.
 * The first string is the part before the delimiter, and the second is the
 * part after. Returns NULL if the delimiter is not found or if memory
 * allocation fails.
 *
 * @note IMPORTANT MEMORY MANAGEMENT:
 * 1. free(result[0]);  // Free the first string.
 * 2. free(result[1]);  // Free the second string.
 * 3. free(result);     // Free the array of pointers itself.
 */
char **split_string(const char *str, char delimiter);

#endif // SPLIT_STRING_H
