#include "token.h"
#include <stdlib.h>
#include <string.h>

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

static int resize_token_stream(TokenStream *token_stream)
{
    Token **temp_token_stream = realloc(token_stream->tokens, token_stream->capacity * 2 * sizeof(Token*));
    if (!temp_token_stream)
    {
        return 0;
    }

    token_stream->tokens = temp_token_stream;
    token_stream->capacity *= 2;

    return 1;
}

static void free_token(Token *token) {
    if (!token) {
        return;
    }

    free(token->lexeme);
    free(token);
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