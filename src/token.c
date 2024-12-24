#include "token.h"
#include <stdlib.h>
#include <string.h>

static int resize_token_stream(TokenStream *token_stream)
{
    Token **temp_token_stream = realloc(token_stream->tokens, token_stream->capacity * 2 * sizeof(Token *));
    if (!temp_token_stream)
    {
        return 0;
    }

    token_stream->tokens = temp_token_stream;
    token_stream->capacity *= 2;

    return 1;
}

static void free_token(Token *token)
{
    if (!token)
    {
        return;
    }

    free(token->lexeme);
    free(token);
}

const char *token_type_to_string(TokenType type)
{
    switch (type)
    {
    // Keywords
    case TOKEN_INT:
        return "TOKEN_INT";
    case TOKEN_BOOL:
        return "TOKEN_BOOL";
    case TOKEN_TRUE:
        return "TOKEN_TRUE";
    case TOKEN_FALSE:
        return "TOKEN_FALSE";
    case TOKEN_IF:
        return "TOKEN_IF";
    case TOKEN_ELSE:
        return "TOKEN_ELSE";
    case TOKEN_WHILE:
        return "TOKEN_WHILE";
    case TOKEN_SET_PIN:
        return "TOKEN_SET_PIN";
    case TOKEN_READ_PIN:
        return "TOKEN_READ_PIN";
    case TOKEN_HIGH:
        return "TOKEN_HIGH";
    case TOKEN_LOW:
        return "TOKEN_LOW";

    // Identifiers and literals
    case TOKEN_IDENTIFIER:
        return "TOKEN_IDENTIFIER";
    case TOKEN_NUMBER:
        return "TOKEN_NUMBER";

    // Operators
    case TOKEN_PLUS:
        return "TOKEN_PLUS";
    case TOKEN_MINUS:
        return "TOKEN_MINUS";
    case TOKEN_STAR:
        return "TOKEN_STAR";
    case TOKEN_SLASH:
        return "TOKEN_SLASH";
    case TOKEN_ASSIGN:
        return "TOKEN_ASSIGN";
    case TOKEN_EQ:
        return "TOKEN_EQ";
    case TOKEN_NEQ:
        return "TOKEN_NEQ";
    case TOKEN_LT:
        return "TOKEN_LT";
    case TOKEN_GT:
        return "TOKEN_GT";
    case TOKEN_LTE:
        return "TOKEN_LTE";
    case TOKEN_GTE:
        return "TOKEN_GTE";
    case TOKEN_AND:
        return "TOKEN_AND";
    case TOKEN_OR:
        return "TOKEN_OR";
    case TOKEN_NOT:
        return "TOKEN_NOT";

    // Punctuation
    case TOKEN_SEMICOLON:
        return "TOKEN_SEMICOLON";
    case TOKEN_COMMA:
        return "TOKEN_COMMA";
    case TOKEN_LPAREN:
        return "TOKEN_LPAREN";
    case TOKEN_RPAREN:
        return "TOKEN_RPAREN";
    case TOKEN_LBRACE:
        return "TOKEN_LBRACE";
    case TOKEN_RBRACE:
        return "TOKEN_RBRACE";

    // Special
    case TOKEN_EOF:
        return "TOKEN_EOF";
    case TOKEN_ERROR:
        return "TOKEN_ERROR";

    // Default case
    default:
        return "UNKNOWN_TOKEN";
    }
}

TokenStream *create_new_token_stream()
{
    TokenStream *token_stream = calloc(1, sizeof(TokenStream));
    if (!token_stream)
    {
        return NULL;
    }

    token_stream->capacity = DEFAULT_TOKEN_STREAM_CAPACITY;
    token_stream->size = 0;

    token_stream->tokens = calloc(token_stream->capacity, sizeof(Token *));
    if (!token_stream->tokens)
    {
        free(token_stream);
        return NULL;
    }

    return token_stream;
}

int add_new_token(TokenStream *token_stream, TokenType token_type, char *string, int line, int column)
{
    if (!token_stream || line < 0 || column < 0 || !string)
    {
        return 0;
    }

    // Check if the token provided is a valid token
    if (token_type < TOKEN_INT || token_type > TOKEN_ERROR)
    {
        return 0;
    }

    Token *new_token = calloc(1, sizeof(Token));
    if (!new_token)
    {
        return 0;
    }

    new_token->line = line;
    new_token->column = column;

    new_token->lexeme = strdup(string);
    if (!new_token->lexeme)
    {
        free(new_token);
        return 0;
    }

    new_token->type = token_type;

    if (token_stream->size == token_stream->capacity) {
        if (!resize_token_stream(token_stream)) {
            free_token(new_token);
            return 0;
        }
    }

    token_stream->tokens[token_stream->size] = new_token;
    token_stream->size++;

    return 1;
}

void free_token_stream(TokenStream *token_stream) {
    if (!token_stream) {
        return;
    }

    if (token_stream->tokens) {
        for (int i = 0; i < token_stream->size; i++) {
            free(token_stream->tokens[i]);
        }

        free(token_stream->tokens);
    }

    free(token_stream);
}