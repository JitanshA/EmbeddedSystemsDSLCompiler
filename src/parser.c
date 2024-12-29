#include "parser.h"
#include "errors.h"
#include <stdlib.h>
#include <string.h>

// Used for moving the look_ahead token to the next token in token stream
static Token *get_next_token(TokenStream *token_stream, int *current_index) 
{
    if (*current_index >= token_stream->size)
    {
        return NULL; // End of token stream
    }
    return &token_stream->tokens[(*current_index)++];
}

// Matches the current token with the expected type
static void match(TokenStream *token_stream, TokenType expected_type, ErrorList *error_list, int *current_index)
{
    Token *current_token = getNextToken(token_stream, current_index);
    if (!current_token || current_token->type != expected_type)
    {
        char message[256];
        snprintf(message, sizeof(message),
                    "Unexpected token '%s' of type '%s'.",
                    current_token ? current_token->lexeme : "NULL",
                    current_token ? token_type_to_string(current_token->type) : "UNKNOWN");

        add_new_error(error_list, current_token ? current_token->line : -1,
                        current_token ? current_token->column : -1,
                        PARSER, message);
    }
}

ASTNode *parse_token_stream(TokenStream *token_stream, ErrorList *error_list) {
    if (!token_stream || token_stream->size == 0) {
        add_new_error(error_list, 0, 0, PARSER, "Invalid token stream passed");
        return NULL;
    }

    // TODO: Implement the rest of the function
}