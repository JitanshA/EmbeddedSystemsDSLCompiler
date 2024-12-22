#include "token.h"
#include <stdlib.h>

Token *create_new_token(TokenType token_type, char *string, int line, int column)
{
    Token *new_token = calloc(1, sizeof(Token));
    if (!new_token)
    {
        return NULL;
    }

    new_token->line = line;
    new_token->column = column;
    new_token->lexeme = strdup(string);
    new_token->type = token_type;

    return new_token;
}