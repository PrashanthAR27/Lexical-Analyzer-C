//The lexer is the heart of the project. It exposes a single
// function to the rest of the program: getNextToken().

#ifndef LEXER_H
#define LEXER_H

#include "token.h"

Token getNextToken(void);

#endif /* LEXER_H */
