/*
 * display.h
 * All printing responsibilities live here -- the token table,
 * the token type names, and the end-of-program statistics.
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include "token.h"

void displayTableHeader(void);
void displayToken(Token token);
void recordTokenStatistics(TokenType type);
void resetStatistics(void);
void displayStatistics(void);
const char *getTokenTypeName(TokenType type);

#endif /* DISPLAY_H */
