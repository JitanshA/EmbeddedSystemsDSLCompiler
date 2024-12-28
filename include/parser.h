#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "errors.h"

typedef enum
{
    FUNCTION,
    FUNCTION_LIST,
    FUNCTION_PARAMETERS,
    STATEMENT,
    STATEMENT_LIST,
    TYPE,
    IDENTIFIER,
    IDENTIFIER_DECLARATION,
    IDENTIFIER_DEFINITION,
    BOOL_VALUE,
    CONDITIONAL,
    OPTIONAL_ELSE,
    EXPRESSION,
    LOGICAL_EXPRESSION,
    RELATIONAL_EXPRESSION,
    ADDITIVE_EXPRESSION,
    MULTIPLICATIVE_EXPRESSION,
    TERM,
    GPIO_OPERATION,
    GPIO_VALUES,
    GPIO_PIN,
    WHILE_LOOP,
    RETURN_STATEMENT
} ASTNodeType;

// Structure for AST nodes
typedef struct ASTNode
{
    ASTNodeType type;          // Node type (e.g., "expression", "statement")
    struct ASTNode **children; // Array of child nodes
    int num_children;          // Number of children
} ASTNode;

// Parser function
extern ASTNode *parse_token_stream(TokenStream *token_stream, ErrorList *error_list);

#endif