#include "lexer.h"
#include "token.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "errors.h"

// Check if the token is a valid number
static int is_valid_number_token(char *string)
{
    if (!string)
    {
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

// Check if the token is a valid identifier
static int is_valid_identifier_token(char *string)
{
    if (!string || !isalpha(*string))
    {
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

// Check if a string is a keyword
static TokenType get_keyword_type(char *string)
{
    if (strcmp(string, "int") == 0)
        return TOKEN_INT;
    if (strcmp(string, "bool") == 0)
        return TOKEN_BOOL;
    if (strcmp(string, "true") == 0)
        return TOKEN_TRUE;
    if (strcmp(string, "false") == 0)
        return TOKEN_FALSE;
    if (strcmp(string, "if") == 0)
        return TOKEN_IF;
    if (strcmp(string, "else") == 0)
        return TOKEN_ELSE;
    if (strcmp(string, "while") == 0)
        return TOKEN_WHILE;
    if (strcmp(string, "SET_PIN") == 0)
        return TOKEN_SET_PIN;
    if (strcmp(string, "READ_PIN") == 0)
        return TOKEN_READ_PIN;
    if (strcmp(string, "HIGH") == 0)
        return TOKEN_HIGH;
    if (strcmp(string, "LOW") == 0)
        return TOKEN_LOW;
    return TOKEN_ERROR;
}

// Check if a string is an operator
static TokenType get_operator_type(char *string)
{
    if (strcmp(string, "+") == 0)
        return TOKEN_PLUS;
    if (strcmp(string, "-") == 0)
        return TOKEN_MINUS;
    if (strcmp(string, "*") == 0)
        return TOKEN_STAR;
    if (strcmp(string, "/") == 0)
        return TOKEN_SLASH;
    if (strcmp(string, "=") == 0)
        return TOKEN_ASSIGN;
    if (strcmp(string, "==") == 0)
        return TOKEN_EQ;
    if (strcmp(string, "!=") == 0)
        return TOKEN_NEQ;
    if (strcmp(string, "<") == 0)
        return TOKEN_LT;
    if (strcmp(string, ">") == 0)
        return TOKEN_GT;
    if (strcmp(string, "<=") == 0)
        return TOKEN_LTE;
    if (strcmp(string, ">=") == 0)
        return TOKEN_GTE;
    if (strcmp(string, "&&") == 0)
        return TOKEN_AND;
    if (strcmp(string, "||") == 0)
        return TOKEN_OR;
    if (strcmp(string, "!") == 0)
        return TOKEN_NOT;
    return TOKEN_ERROR;
}

// Check if a string is punctuation
static TokenType get_punctuation_type(char *string)
{
    if (strcmp(string, ";") == 0)
        return TOKEN_SEMICOLON;
    if (strcmp(string, ",") == 0)
        return TOKEN_COMMA;
    if (strcmp(string, "(") == 0)
        return TOKEN_LPAREN;
    if (strcmp(string, ")") == 0)
        return TOKEN_RPAREN;
    if (strcmp(string, "{") == 0)
        return TOKEN_LBRACE;
    if (strcmp(string, "}") == 0)
        return TOKEN_RBRACE;
    return TOKEN_ERROR;
}

// Check if a string is a number or identifier
static TokenType get_identifier_or_number_type(char *string)
{
    if (isdigit(string[0]) && is_valid_number_token(string))
        return TOKEN_NUMBER;
    if (is_valid_identifier_token(string))
        return TOKEN_IDENTIFIER;
    return TOKEN_ERROR;
}

static TokenType get_token_type(char *string)
{
    if (!string || strlen(string) == 0)
    {
        return TOKEN_ERROR;
    }

    // Check for specific token categories
    TokenType token_type;

    // Check for keywords
    token_type = get_keyword_type(string);
    if (token_type != TOKEN_ERROR)
        return token_type;

    // Check for operators
    token_type = get_operator_type(string);
    if (token_type != TOKEN_ERROR)
        return token_type;

    // Check for punctuation
    token_type = get_punctuation_type(string);
    if (token_type != TOKEN_ERROR)
        return token_type;

    // Check for identifiers and numbers
    token_type = get_identifier_or_number_type(string);
    if (token_type != TOKEN_ERROR)
        return token_type;

    // Default case: token not recognized
    return TOKEN_ERROR;
}

// Check if a character is a special character i.e. punctuation
static int is_special_character(char c)
{
    return (c == ',' || c == ';' || c == '(' ||
            c == ')' || c == '{' || c == '}' );
}

// Check if a character is a single-character operator
static int is_single_character_operator(char c)
{
    return (c == '+' || c == '-' || c == '*' ||
            c == '/' || c == '=' || c == '<' ||
            c == '>' || c == '!');
}

// Handle whitespace
static void handle_whitespace(char **cursor, int *line_number, int *column_number)
{
    if (**cursor == '\n')
    {
        (*line_number)++;
        *column_number = 1;
    }
    else
    {
        (*column_number)++;
    }
    (*cursor)++;
}

// Handle special characters
static int handle_special_character(TokenStream *token_stream, char **cursor, int line_number, int column_number, ErrorList *error_list)
{
    char special_char_str[2] = {**cursor, '\0'};
    TokenType token_type = get_token_type(special_char_str);
    if (token_type != TOKEN_ERROR)
    {
        if (!add_new_token(token_stream, token_type, special_char_str, line_number, column_number))
        {
            add_new_error(error_list, line_number, column_number, LEXER, "Failed to create special character token");
            return 0;
        }
    }
    else
    {
        add_new_error(error_list, line_number, column_number, LEXER, "Invalid special character");
    }
    (*cursor)++;
    return 1;
}

// Handle operators
static int handle_operator(TokenStream *token_stream, char **cursor, int line_number, int column_number, ErrorList *error_list)
{
    char operator_str[2] = {**cursor, '\0'};
    TokenType token_type = get_token_type(operator_str);
    if (token_type != TOKEN_ERROR)
    {
        if (!add_new_token(token_stream, token_type, operator_str, line_number, column_number))
        {
            add_new_error(error_list, line_number, column_number, LEXER, "Failed to create operator token");
            return 0;
        }
    }
    else
    {
        add_new_error(error_list, line_number, column_number, LEXER, "Invalid operator detected");
    }
    (*cursor)++;
    return 1;
}

// Handle keywords, identifiers, and numbers
static int handle_identifier_or_number(TokenStream *token_stream, char **cursor, int *line_number, int *column_number, ErrorList *error_list)
{
    char token_buffer[256];
    int token_length = 0;

    while (**cursor && (isalnum(**cursor) || **cursor == '_'))
    {
        if (token_length < 255)
        {
            token_buffer[token_length++] = **cursor;
        }
        else
        {
            add_new_error(error_list, *line_number, *column_number, LEXER, "Token exceeds maximum length");
            token_length = 0;
            break;
        }
        (*cursor)++;
        (*column_number)++;
    }

    token_buffer[token_length] = '\0';

    TokenType token_type = get_token_type(token_buffer);
    if (token_type != TOKEN_ERROR)
    {
        if (!add_new_token(token_stream, token_type, token_buffer, *line_number, *column_number - token_length))
        {
            add_new_error(error_list, *line_number, *column_number - token_length, LEXER, "Failed to create identifier/number token");
            return 0;
        }
    }
    else
    {
        add_new_error(error_list, *line_number, *column_number - token_length, LEXER, "Invalid token detected");
    }
    return 1;
}

// Get the token stram from the input file
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
        if (isspace(*cursor))
        {
            handle_whitespace(&cursor, &line_number, &column_number);
            continue;
        }

        if (is_special_character(*cursor))
        {
            if (!handle_special_character(token_stream, &cursor, line_number, column_number, error_list))
            {
                free_token_stream(token_stream);
                return NULL;
            }
            column_number++;
            continue;
        }

        if (is_single_character_operator(*cursor))
        {
            char operator_str[2] = {*cursor, '\0'};
            TokenType token_type = get_operator_type(operator_str);
            if (token_type != TOKEN_ERROR)
            {
                if (!add_new_token(token_stream, token_type, operator_str, line_number, column_number))
                {
                    add_new_error(error_list, line_number, column_number, LEXER, "Failed to create operator token");
                    free_token_stream(token_stream);
                    return NULL;
                }
            }
            else
            {
                add_new_error(error_list, line_number, column_number, LEXER, "Invalid operator detected");
            }
            cursor++;
            column_number++;
            continue;
        }

        if (isalnum(*cursor) || *cursor == '_')
        {
            if (!handle_identifier_or_number(token_stream, &cursor, &line_number, &column_number, error_list))
            {
                free_token_stream(token_stream);
                return NULL;
            }
            continue;
        }

        add_new_error(error_list, line_number, column_number, LEXER, "Unrecognized or invalid token");
        cursor++;
        column_number++;
    }

    if (!add_new_token(token_stream, TOKEN_EOF, "EOF", line_number, column_number))
    {
        add_new_error(error_list, line_number, column_number, LEXER, "Failed to create EOF token");
        free_token_stream(token_stream);
        return NULL;
    }

    return token_stream;
}
