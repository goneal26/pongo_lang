//
// Created by Tre on 2/27/2024.
//

#include "reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* read_file(const char* filepath);
// just for file reading, keeping it in the scope of this file only

char** split_text(const char *text, int *num_strings);
// split text into array of substrings split on delimiter ;
// may need to update for goto labels which end in : ^^^

char** remove_comments(char **strings, int numStrings, int *newNumStrings);
// remove comments denoted by "rem" from the array of substrings
// (needs review)

char* read_file(const char* filepath) {
    FILE *file;
    char *text = NULL;
    long length;

    file = fopen(filepath, "r"); // open in read mode

    // check if file opened successfully
    if (file == NULL) {
        printf("Failed to open file at location %s\n", filepath);
        return NULL; // function failed
    }

    // get file length
    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, 0, SEEK_SET);

    // allocate memory to store text
    text = (char*) malloc(length * sizeof(char));
    if (text == NULL) {
        printf("Memory allocation failed for file %s\n", filepath);
        fclose(file);
        return NULL;
    }

    // read from file
    fread(text, sizeof(char), length, file);

    fclose(file); // close file

    return text;
}

char** split_text(const char *text, int *num_strings) {
    int count = 1; // count number of statements using delimiter ;
    const char *ptr = text;
    while (*ptr != '\0') {
        if (*ptr == ';') {
            count++;
        }
        ptr++;
    }

    // memory for the array of strings
    char **strings = (char**)malloc(count * sizeof(char*));
    if (strings == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    // vars for splitting
    const char *start = text;
    int i = 0;

    // split into substrings
    while (*text != '\0') {
        if (*text == ';') {
            // memory for string
            strings[i] = (char*) malloc((text - start + 1) * sizeof(char));
            if (strings[i] == NULL) {
                printf("Memory allocation failed for statement %d\n", i);
                // freeup memory
                for (int j = 0; j < i; j++) {
                    free(strings[j]);
                }
                free(strings);
                return NULL;
            }

            // copy substring into array
            strncpy(strings[i], start, text - start); // !!! might be unsafe
            strings[i][text - start] = '\0'; // null-terminate

            // to next string
            i++;
            start = text + 1; // move past delimiter
        }
        text++;
    }

    // memory for the last string
    strings[i] = (char*) malloc((text - start + 1) * sizeof(char));
    if (strings[i] == NULL) {
        printf("Memory allocation failed for last string/statement.\n");
        // freeup
        for (int j = 0; j < i; j++) {
            free(strings[j]);
        }
        free(strings);
        return NULL;
    }

    // copy last substring into array
    strncpy(strings[i], start, text - start);
    strings[i][text - start] = '\0'; // null-terminate

    *num_strings = count; // set number of strings

    return strings;
}

// left off here, need to go through this code VVV
char** remove_comments(char **strings, int numStrings, int *newNumStrings) {
    // Allocate memory for the new array of strings
    char **newStrings = (char**)malloc(numStrings * sizeof(char*));
    if (newStrings == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    int count = 0; // Counter for the new number of strings

    // Iterate through the original array of strings
    for (int i = 0; i < numStrings; i++) {
        // Check if the substring contains the keyword "rem"
        char *keyword = strstr(strings[i], "rem");
        if (keyword != NULL) {
            // Check if the substring starts with the keyword "rem"
            if (keyword == strings[i]) {
                // Skip this string by not copying it to the new array
                continue;
            } else {
                // Copy the substring up to the keyword "rem" to the new array
                newStrings[count] = (char*)malloc((keyword - strings[i] + 1) * sizeof(char));
                if (newStrings[count] == NULL) {
                    printf("Memory allocation failed.\n");
                    // Free allocated memory
                    for (int j = 0; j < count; j++) {
                        free(newStrings[j]);
                    }
                    free(newStrings);
                    return NULL;
                }
                strncpy(newStrings[count], strings[i], keyword - strings[i]);
                newStrings[count][keyword - strings[i]] = '\0'; // Null-terminate the string
                count++;
            }
        } else {
            // Copy the entire substring to the new array
            newStrings[count] = (char*)malloc((strlen(strings[i]) + 1) * sizeof(char));
            if (newStrings[count] == NULL) {
                printf("Memory allocation failed.\n");
                // Free allocated memory
                for (int j = 0; j < count; j++) {
                    free(newStrings[j]);
                }
                free(newStrings);
                return NULL;
            }
            strcpy(newStrings[count], strings[i]);
            count++;
        }
    }

    *newNumStrings = count; // Set the new number of strings

    return newStrings;
}
