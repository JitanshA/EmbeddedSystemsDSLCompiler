#include "lexer.h"
#include "token.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "errors.h"

static int is_valid_number_token(char *string)
{
    if (!string) {
        return 0;
    }

    size_t length = strlen(string);
    for (size_t i = 0; i < length; i++)
    {
        if (!isdigit(string[i]))
        {
            return 0;
        }
    }

    return 1;
}

static int is_valid_identifier_token(char *string)
{
    if (!string || !isalpha(*string)) {
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

static TokenType get_token_type(char *string)
{
    if (!string || strlen(string) == 0)
    {
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
    if (isdigit(string[0]))
    {
        if (is_valid_number_token(string))
        {
            return TOKEN_NUMBER;
        }
    }
    else
    {
        if (is_valid_identifier_token(string))
        {
            return TOKEN_IDENTIFIER;
        }
    }

    return TOKEN_ERROR;
}

static int is_special_character(char c)
{
    return (c == ',' || c == ';' || c == '(' ||
            c == ')' || c == '{' || c == '}' );
}

TokenStream *get_token_stream_from_input_file(char *input, ErrorList *error_list)
{
    if (!input || strspn(input, " \t\n") == strlen(input))
    {
        add_new_error(error_list, 0, 0, LEXER, "Input contains no valid tokens");
        return NULL;
    }

    TokenStream *token_stream = create_new_token_stream();
    if (!token_stream)
    {
        add_new_error(error_list, 0, 0, LEXER, "Failed to initialise token stream");
        return NULL;
    }

    int line_number = 1, column_number = 1;
    char *cursor = input;

    while (*cursor)
    {
        // Skip whitespace
        if (isspace(*cursor))
        {
            if (*cursor == '\n')
            {
                line_number++;
                column_number = 1;
            }
            else
            {
                column_number++;
            }
            cursor++;
            continue;
        }

        // Handle special characters
        if (is_special_character(*cursor))
        {
            char special_char_str[2] = {*cursor, '\0'};
            if (!add_new_token(token_stream, get_token_type(special_char_str), special_char_str, line_number, column_number))
            {
                add_new_error(error_list, line_number, column_number, LEXER, "Failed to create new token");
                free_token_stream(token_stream);
                return NULL;
            }
            column_number++;
            cursor++;
            continue;
        }

        // Handle keywords, identifiers, and numbers
        if (isalnum(*cursor) || *cursor == '_')
        {
            char token_buffer[256];
            int token_length = 0;

            while (*cursor && (isalnum(*cursor) || *cursor == '_'))
            {
                if (token_length < 255)
                {
                    token_buffer[token_length++] = *cursor;

                    // Check for undelimited token boundary
                    if (token_length > 1 && get_token_type(token_buffer) == TOKEN_ERROR)
                    {
                        // Undo the last character and process the valid token so far
                        token_buffer[--token_length] = '\0';
                        if (!add_new_token(token_stream, get_token_type(token_buffer), token_buffer, line_number, column_number - token_length))
                        {
                            add_new_error(error_list, line_number, column_number - token_length, LEXER, "Failed to create new token");
                            free_token_stream(token_stream);
                            return NULL;
                        }

                        // Log an error for the undelimited part
                        add_new_error(error_list, line_number, column_number, LEXER, "Undelimited token detected");
                        break;
                    }
                }
                else
                {
                    add_new_error(error_list, line_number, column_number, LEXER, "Token exceeds maximum length");
                    while (*cursor && (isalnum(*cursor) || *cursor == '_'))
                    {
                        cursor++;
                        column_number++;
                    }
                    token_length = 0;
                    break;
                }
                cursor++;
                column_number++;
            }

            // Process the valid token if no boundary error was detected
            if (token_length > 0)
            {
                token_buffer[token_length] = '\0';
                if (!add_new_token(token_stream, get_token_type(token_buffer), token_buffer, line_number, column_number - token_length))
                {
                    add_new_error(error_list, line_number, column_number - token_length, LEXER, "Failed to create new token");
                    free_token_stream(token_stream);
                    return NULL;
                }
            }
            continue;
        }

        // Handle unexpected characters
        add_new_error(error_list, line_number, column_number, LEXER, "Unexpected character encountered");
        cursor++;
        column_number++;
    }

    // Add an EOF token to signify the end of the token stream
    if (!add_new_token(token_stream, TOKEN_EOF, "EOF", line_number, column_number))
    {
        add_new_error(error_list, line_number, column_number, LEXER, "Failed to create EOF token");
        free_token_stream(token_stream);
        return NULL;
    }

    return token_stream;
}
