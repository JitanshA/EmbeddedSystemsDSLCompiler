#include "lexer.h"
#include "token.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "errors.h"

Token **get_token_stream(char *input, ErrorList *error_list)
{
    const char delim_newline[] = "\n";
    const char delim_space[] = " ";
    int line_number = 1; // Tracks the current line number
    int token_count = 0; // Tracks the number of tokens generated
    int capacity = 128;  // Initial capacity for the token stream

    // Allocate space for the token stream
    Token **token_stream = calloc(capacity, sizeof(Token *));
    if (!token_stream)
    {
        add_new_error(error_list, line_number, 0, LEXER, "Failed to allocate memory for token stream");
        return NULL;
    }

    // Split the input into lines
    char *split_input_newline = strtok(input, delim_newline);
    while (split_input_newline)
    {
        int column_number = 1; // Tracks the column number for tokens in the current line

        // Split the current line into tokens by spaces
        char *split_input_space = strtok(split_input_newline, delim_space);
        while (split_input_space)
        {
            // Handle trailing special characters (e.g., `SET_PIN,`)
            while (split_input_space[0])
            {
                char *last_char = NULL;

                // **Check if the first character is a special character**
                if (split_input_space[0] == ',' || split_input_space[0] == ';' ||
                    split_input_space[0] == '(' || split_input_space[0] == ')' ||
                    split_input_space[0] == '{' || split_input_space[0] == '}')
                {
                    char special_char_str[2] = {split_input_space[0], '\0'};
                    TokenType token_type = get_token_type(special_char_str);

                    if (token_type != TOKEN_ERROR)
                    {
                        Token *new_token = create_new_token(token_type, special_char_str, line_number, column_number);
                        if (!new_token)
                        {
                            add_new_error(error_list, line_number, column_number, LEXER, "Failed to allocate memory for new token");
                            free(token_stream);
                            return NULL;
                        }

                        // Add the token to the stream
                        if (token_count >= capacity)
                        {
                            capacity *= 2;
                            token_stream = realloc(token_stream, capacity * sizeof(Token *));
                            if (!token_stream)
                            {
                                add_new_error(error_list, line_number, column_number, LEXER, "Failed to reallocate memory for token stream");
                                return NULL;
                            }
                        }

                        token_stream[token_count++] = new_token;
                        column_number++; // Special characters occupy one column

                        // Remove the first character from the current token
                        memmove(split_input_space, split_input_space + 1, strlen(split_input_space)); // Shift left
                        continue;                                                                     // Re-check the remaining string
                    }
                }

                // Check if the last character is a special character
                size_t len = strlen(split_input_space);
                if (len > 0 && (split_input_space[len - 1] == ',' || split_input_space[len - 1] == ';' ||
                                split_input_space[len - 1] == '(' || split_input_space[len - 1] == ')' ||
                                split_input_space[len - 1] == '{' || split_input_space[len - 1] == '}'))
                {
                    last_char = &split_input_space[len - 1];
                    split_input_space[len - 1] = '\0'; // Remove the special character from the current token
                }

                // Get the token type for the current part
                if (strlen(split_input_space) > 0)
                {
                    TokenType token_type = get_token_type(split_input_space);
                    if (token_type != TOKEN_ERROR)
                    {
                        // Create a new token
                        Token *new_token = create_new_token(token_type, split_input_space, line_number, column_number);
                        if (!new_token)
                        {
                            add_new_error(error_list, line_number, column_number, LEXER, "Failed to allocate memory for new token");
                            free(token_stream);
                            return NULL;
                        }

                        // Add the token to the stream
                        if (token_count >= capacity)
                        {
                            capacity *= 2;
                            token_stream = realloc(token_stream, capacity * sizeof(Token *));
                            if (!token_stream)
                            {
                                add_new_error(error_list, line_number, column_number, LEXER, "Failed to reallocate memory for token stream");
                                return NULL;
                            }
                        }
                        token_stream[token_count++] = new_token;
                        column_number += strlen(split_input_space) + 1; // Update column number
                    }
                    else
                    {
                        add_new_error(error_list, line_number, column_number, LEXER, "Invalid token found");
                    }
                }

                // Handle the special character as a separate token, if applicable
                if (last_char)
                {
                    char special_char_str[2] = {*last_char, '\0'};
                    TokenType token_type = get_token_type(special_char_str);
                    if (token_type != TOKEN_ERROR)
                    {
                        // Create a new token for the special character
                        Token *new_token = create_new_token(token_type, special_char_str, line_number, column_number);
                        if (!new_token)
                        {
                            add_new_error(error_list, line_number, column_number, LEXER, "Failed to allocate memory for new token");
                            free(token_stream);
                            return NULL;
                        }

                        // Add the token to the stream
                        if (token_count >= capacity)
                        {
                            capacity *= 2;
                            token_stream = realloc(token_stream, capacity * sizeof(Token *));
                            if (!token_stream)
                            {
                                add_new_error(error_list, line_number, column_number, LEXER, "Failed to reallocate memory for token stream");
                                return NULL;
                            }
                        }
                        token_stream[token_count++] = new_token;
                        column_number++; // Special characters occupy one column
                    }
                    else
                    {
                        add_new_error(error_list, line_number, column_number, LEXER, "Invalid token found");
                    }
                }

                break; // Process the next space-separated token
            }

            // Get the next token (space-separated)
            split_input_space = strtok(NULL, delim_space);
        }

        // Get the next line
        split_input_newline = strtok(NULL, delim_newline);
        line_number++;
    }

    // Add an EOF token to signify the end of the token stream
    Token *eof_token = create_new_token(TOKEN_EOF, "EOF", line_number, 0);
    if (!eof_token)
    {
        free(token_stream);
        return NULL;
    }
    token_stream[token_count++] = eof_token;

    return token_stream;
}

static TokenType get_token_type(char* string) {
    if (!string || strlen(string) == 0) {
        return TOKEN_ERROR;
    }

    // Check for keywords and punctuation
    if (strcmp(string, "int") == 0) 
    {
        return TOKEN_INT;
    } 
    else if (strcmp(string, "bool") == 0) 
    {
        return TOKEN_BOOL;
    }
    else if (strcmp(string, "true") == 0) 
    {
        return TOKEN_TRUE;
    }
    else if (strcmp(string, "false") == 0)
    {
        return TOKEN_FALSE;
    }
    else if (strcmp(string, "if") == 0)
    {
        return TOKEN_IF;
    }
    else if (strcmp(string, "else") == 0)
    {
        return TOKEN_ELSE;
    }
    else if (strcmp(string, "while") == 0)
    {
        return TOKEN_WHILE;
    }
    else if (strcmp(string, "SET_PIN") == 0)
    {
        return TOKEN_SET_PIN;
    }
    else if (strcmp(string, "READ_PIN") == 0)
    {
        return TOKEN_READ_PIN;
    }
    else if (strcmp(string, "HIGH") == 0)
    {
        return TOKEN_HIGH;
    }
    else if (strcmp(string, "LOW") == 0)
    {
        return TOKEN_LOW;
    }
    else if (strcmp(string, ";") == 0)
    {
        return TOKEN_SEMICOLON;
    }
    else if (strcmp(string, ",") == 0)
    {
        return TOKEN_COMMA;
    }
    else if (strcmp(string, "(") == 0)
    {
        return TOKEN_LPAREN;
    }
    else if (strcmp(string, ")") == 0)
    {
        return TOKEN_RPAREN;
    }
    else if (strcmp(string, "{") == 0)
    {
        return TOKEN_LBRACE;
    }
    else if (strcmp(string, "}") == 0)
    {
        return TOKEN_RBRACE;
    }

    // Check for identifiers and numbers
    if (isdigit(string[0])) {
        if (is_valid_number_token(string)) 
        {
            return TOKEN_NUMBER;
        }
    } else {
        if (is_valid_identifier_token(string))
        {
            return TOKEN_IDENTIFIER;
        }
    }

    return TOKEN_ERROR;
}

static int is_valid_number_token(char* string) {
    size_t length = strlen(string);
    for (size_t i = 0; i < length; i++) {
        if (!isdigit(string[i])) {
            return 0;
        }
    }

    return 1;
}

static int is_valid_identifier_token(char *string)
{
    if (!isalpha(string[0])) {
        return 0;
    }

    size_t length = strlen(string);
    for (size_t i = 1; i < length; i++)
    {
        if (!(isalnum(string[i]) || string[i] == '_'))
        {
            return 0;
        }
    }

    return 1;
}