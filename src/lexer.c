#include "lexer.h"
#include "token.h"
#include <string.h>
#include <stdlib.h>

Token** get_token_stream(char** input) {
    const char delim_newline[] = '\n';
    const char delim_space[] = ' ';
    int line_number = 1;

    // Split based on newline
    char *split_input_newline = strtok(input, delim_newline);
    while (split_input_newline) {
        char *split_input_space = strtok(input, delim_space);
        // Token start column number
        int column_number = 1;
        
        while (split_input_space != NULL)
        {
            TokenType token_type = get_token_type(split_input_space);
            if (token_type != TOKEN_ERROR) {
                Token* new_token = create_new_token(token_type, split_input_space, line_number, column_number);
                // Increment by length of token, not token - 1, to accomodate for ' '
                column_number += strlen(split_input_space);
            } else {
                // TODO: Handle error in file here
                
            }

            // Get the next element/token (separated by space)
            split_input_space = strtok(NULL, delim_space);
        }
        

        // Get the next line
        split_input_newline = strtok(NULL, delim_newline);
        line_number++;
    }
}

static TokenType get_token_type(char* string) {

}

static Token* create_new_token(TokenType token_type, char* string, int line, int column) {
    Token* new_token = calloc(1, sizeof(Token*));
    if (!new_token) {
        return NULL;
    }

    new_token->line = line;
    new_token->column = column;
    new_token->lexeme = strdup(string);
    new_token->type = token_type;

    return new_token;
}