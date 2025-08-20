
#ifndef STRING_SPLIT_H
#define STRING_SPLIT_H

#include <stdlib.h> // For size_t

/**
 * @brief Splits a string by a specified delimiter.
 *
 * This function takes an input string and splits it into an array of substrings
 * based on the provided delimiter. The function allocates memory for the array
 * of strings and for each individual string. It is the caller's responsibility
 * to free this memory using the free_split_string_array() function.
 *
 * @param str The null-terminated string to be split. Cannot be NULL.
 * @param delimiter The null-terminated string to use as a delimiter. Cannot be
 * NULL.
 * @param count A pointer to a size_t variable where the number of resulting
 * substrings (tokens) will be stored.
 *
 * @return A dynamically allocated array of dynamically allocated strings
 * (char**). The last element of the array is NULL. Returns NULL on failure
 * (e.g., memory allocation error, invalid input).
 */
char **string_split(const char *str, const char *delimiter, size_t *count);

/**
 * @brief Frees the memory allocated by string_split.
 *
 * Iterates through the array of strings, freeing each string, and then
 * frees the array itself.
 *
 * @param arr The array of strings to free.
 * @param count The number of elements in the array (as returned by
 * string_split).
 */
void free_split_string_array(char **arr, size_t count);

#endif // STRING_SPLIT_H
