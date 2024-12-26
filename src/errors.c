#include "errors.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

const char *ErrorStageNames[] = {"LEXER", "PARSER", "CODEGEN"};

// Helper method to double the error list capacity
static int resize_error_list(ErrorList *error_list)
{
    Error **temp_error_list = realloc(error_list->errors, error_list->capacity * 2 * sizeof(Error *));
    if (!temp_error_list)
    {
        return 0;
    }

    error_list->errors = temp_error_list;
    error_list->capacity *= 2;

    return 1;
}

// Method to create a new error list with default capacity
ErrorList *create_new_error_list() {
    ErrorList *error_list = calloc(1, sizeof(ErrorList));
    if (!error_list) {
        return NULL;
    }

    error_list->capacity = DEFAULT_ERROR_LIST_CAPACITY;
    error_list->size = 0;

    error_list->errors = calloc(error_list->capacity, sizeof(Error*));
    if (!error_list->errors) {
        free(error_list);
        return NULL;
    }

    return error_list;
}

// Method to create and add a new error to the error_list
void add_new_error(ErrorList *error_list, int line, int column, ErrorStage stage, char* message) {
    if (!error_list || line < 0 || column < 0 || !message) {
        return;
    }

    // Check if the stage provided is a valid stage
    if (stage < LEXER || stage > CODEGEN) {
        return;
    }

    Error *error = calloc(1, sizeof(Error));
    if (!error) {
        return;
    }

    // Copy only first 256 characters, add null at the end if the original message exceeds 256 characters
    strncpy(error->message, message, 256);
    error->message[255] = '\0';
    error->line = line;
    error->column = column;
    error->stage = stage;

    // Resize list if needed
    if (error_list->size == error_list->capacity) {
        if (!resize_error_list(error_list)) {
            return;
        }
    }

    error_list->errors[error_list->size] = error;
    error_list->size++;
}

// Method to print all the errors in the list
void report_errors(ErrorList *error_list) {
    for (int i = 0; i < error_list->size; i++) {
        printf("Error at line %d column %d during stage %s\nError message: %s\n\n",
                error_list->errors[i]->line, error_list->errors[i]->column,
                ErrorStageNames[error_list->errors[i]->stage], error_list->errors[i]->message);
    }
}

// Method to free the error list memory
void free_error_list(ErrorList *error_list) {
    for (int i = 0; i < error_list->size; i++) {
        free(error_list->errors[i]);
    }

    free(error_list->errors);
    free(error_list);
}