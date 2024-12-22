#ifndef ERRORS_H
#define ERRORS_H

#define DEFAULT_ERROR_LIST_CAPACITY 20

// Enum for the stage where error occurs
typedef enum {
    LEXER,
    PARSER,
    CODEGEN
} ErrorStage;

typedef struct {
    int line;          // Line where error occured
    int column;        // Column where error occured
    ErrorStage stage;  // Stage of compiler where the error occured
    char message[256]; // Error message
} Error;

typedef struct {
    Error **errors;    // Array of error pointers
    int capacity;      // Maximum size of the list
    int size;          // Current size of the list
} ErrorList;

extern ErrorList *create_new_error_list();
extern void add_new_error(ErrorList *error_list, int line, int column, ErrorStage stage, char* message);
extern void report_errors(ErrorList *error_list);
extern void free_error_list(ErrorList *error_list);

#endif