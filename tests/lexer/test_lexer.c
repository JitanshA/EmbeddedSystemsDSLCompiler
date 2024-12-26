#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "token.h"
#include "errors.h"

// Function to print a token stream in the specified format
static void print_token_stream(TokenStream *token_stream)
{
    for (int i = 0; i < token_stream->size; i++)
    {
        Token *token = token_stream->tokens[i];
        printf("%s \"%s\" [line: %d, column: %d]\n",
                token_type_to_string(token->type), token->lexeme, token->line, token->column);
    }
}

int main()
{
    // Buffer to read the input
    char input[1024];

    // Create error list
    ErrorList *error_list = create_new_error_list();

    // Read the input from stdin
    size_t input_size = fread(input, 1, sizeof(input) - 1, stdin);
    input[input_size] = '\0'; // Null-terminate the input string

    // Get the token stream
    TokenStream *token_stream = get_token_stream_from_input_file(input, error_list);

    // If the token stream exists, print it
    if (token_stream)
    {
        print_token_stream(token_stream);
        free_token_stream(token_stream);
    }

    // Print any errors encountered
    report_errors(error_list);

    // Free the error list
    free_error_list(error_list);

    return 0;
}
