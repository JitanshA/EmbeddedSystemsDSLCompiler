#ifndef PARSER_H
#define PARSER_H

#include "token.h"

// Structure for AST nodes
typedef struct ASTNode
{
    char *type;                // Node type (e.g., "expression", "statement")
    struct ASTNode **children; // Array of child nodes
    int num_children;          // Number of children
} ASTNode;

// Parser function
ASTNode *parse_tokenstream(TokenStream *token_stream);

#endif