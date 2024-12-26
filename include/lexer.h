#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include "errors.h"

extern TokenStream *get_token_stream_from_input_file(char *input, ErrorList *error_list);

#endif