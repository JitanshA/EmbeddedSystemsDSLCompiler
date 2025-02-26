#ifndef TOKEN_H
#define TOKEN_H

#define DEFAULT_TOKEN_STREAM_CAPACITY 128

// Enum for token types
typedef enum
{
    // Keywords
    TOKEN_INT,
    TOKEN_BOOL,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_WHILE,
    TOKEN_SET_PIN,
    TOKEN_READ_PIN,
    TOKEN_HIGH,
    TOKEN_LOW,

    // Identifiers and literals
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,

    // Operators
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_ASSIGN,
    TOKEN_EQ,
    TOKEN_NEQ,
    TOKEN_LT,
    TOKEN_GT,
    TOKEN_LTE,
    TOKEN_GTE,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_NOT,

    // Punctuation
    TOKEN_SEMICOLON,
    TOKEN_COMMA,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,

    // Special
    TOKEN_EOF,
    TOKEN_ERROR
} TokenType;

// Structure for a token
typedef struct
{
    TokenType type; // Type of the token
    char *lexeme;   // The actual string value of the token
    int line;       // Line number where the token appears
    int column;     // Column number where the token starts
} Token;

typedef struct {
    Token ** tokens;
    int capacity;
    int size;
} TokenStream;

extern int add_new_token(TokenStream *token_stream, TokenType token_type, char *string, int line, int column);
extern TokenStream *create_new_token_stream();
extern void free_token_stream(TokenStream *token_stream);
extern const char *token_type_to_string(TokenType type);

#endif // TOKEN_H
