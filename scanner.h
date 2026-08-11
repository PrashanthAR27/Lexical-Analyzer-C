

//Character-level input functions used by the lexer.


#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>

void initScanner(FILE *file);

int getNextChar(void);
int peekChar(void);

void ungetChar(int ch);
void skipWhitespace(void);

int getCurrentLine(void);

#endif 