//
// Created by Lenovo on 2/27/2024.
//

#include "reader.h"
#include <stdio.h>
#include <stdlib.h>

char* readfile(const char* filepath) {
    FILE *file;
    char *text = NULL;
    long length;

    file = fopen(filepath, "r"); // open in read mode

    // check if file opened successfully
    if (file == NULL) {
        printf("Failed to open file.\n");
        return NULL; // function failed
    }

    // get file length
    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, 0, SEEK_SET);

    // allocate memory to store text
    text = (char*) malloc(length * sizeof(char));
    if (text == NULL) {
        printf("Memory allocation failed.\n");
        fclose(file);
        return NULL;
    }

    // read from file
    fread(text, sizeof(char), length, file);

    fclose(file); // close file

    return text;
}