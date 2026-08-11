/*
 * keywords.c
 * Stores the C keyword table and checks whether a given word
 * is a keyword using simple strcmp() comparisons.
 */

#include <string.h>
#include "keywords.h"
static const char *keywordTable[] = {
    "auto", "break", "case", "char", "const", "continue", "default",
    "do", "double", "else", "enum", "extern", "float", "for", "goto",
    "if", "int", "long", "register", "return", "short", "signed",
    "sizeof", "static", "struct", "switch", "typedef", "union",
    "unsigned", "void", "volatile", "while"
};

#define KEYWORD_COUNT (int)(sizeof(keywordTable) / sizeof(keywordTable[0]))

/*
 * isKeyword
 * Purpose : Checks if the given word matches any entry in the
 *           keyword table.
 * Input   : Null terminated word read by the lexer.
 * Output  : 1 if keyword, 0 otherwise.
 */
int isKeyword(const char *word)
{
    int i;

    for (i = 0; i < KEYWORD_COUNT; i++) {
        if (strcmp(word, keywordTable[i]) == 0) {
            return 1;
        }
    }
    return 0;
}
