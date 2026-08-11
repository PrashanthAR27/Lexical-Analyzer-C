/*
 * display.c
 * Handles ALL printing: the formatted token table, readable
 * token type names, line numbers, and end-of-program statistics.
 */

#include <stdio.h>
#include "display.h"

static int keywordCount;
static int identifierCount;
static int literalCount;
static int operatorCount;
static int commentCount;
static int invalidCount;

/*
 * Converts TokenType into display string.
 */
const char *getTokenTypeName(TokenType type)
{
    switch (type)
    {
        case TOKEN_KEYWORD:
            return "Keyword";

        case TOKEN_IDENTIFIER:
            return "Identifier";

        case TOKEN_LITERAL:
            return "Literal";

        case TOKEN_HEADER:
            return "Header File";

        case TOKEN_OPERATOR:
            return "Operator";

        case TOKEN_COMMENT:
            return "Comment";

        case TOKEN_INVALID:
            return "Invalid Token";

        case TOKEN_END_OF_FILE:
            return "EOF";

        default:
            return "Unknown";
    }
}

/*
 * Prints table heading.
 */
void displayTableHeader(void)
{
    printf("\n");
    printf("Parsing : Started\n\n");

    printf("%-10s %-15s : %s\n",
           "LINE",
           "TOKEN TYPE",
           "TOKEN");

    printf("--------------------------------------------------\n");
}

/*
 * Counts token statistics.
 */
void recordTokenStatistics(TokenType type)
{
    switch (type)
    {
        case TOKEN_KEYWORD:
            keywordCount++;
            break;

        case TOKEN_IDENTIFIER:
            identifierCount++;
            break;

        case TOKEN_LITERAL:
            literalCount++;
            break;

        case TOKEN_OPERATOR:
            operatorCount++;
            break;

        case TOKEN_COMMENT:
            commentCount++;
            break;

        case TOKEN_INVALID:
            invalidCount++;
            break;

        default:
            break;
    }
}

/*
 * Prints one token with its line number.
 */
void displayToken(Token token)
{
    if (token.type == TOKEN_END_OF_FILE)
    {
        return;
    }

    printf("%-10d %-15s : %s\n",
           token.lineNumber,
           getTokenTypeName(token.type),
           token.lexeme);

    recordTokenStatistics(token.type);
}

/*
 * Reset counters.
 */
void resetStatistics(void)
{
    keywordCount = 0;
    identifierCount = 0;
    literalCount = 0;
    operatorCount = 0;
    commentCount = 0;
    invalidCount = 0;
}

/*
 * Print summary.
 */
void displayStatistics(void)
{
    printf("\nParsing : Done\n\n");

    printf("------------- SUMMARY -------------\n");
    printf("Keywords       : %d\n", keywordCount);
    printf("Identifiers    : %d\n", identifierCount);
    printf("Literals       : %d\n", literalCount);
    printf("Operators      : %d\n", operatorCount);
    printf("Comments       : %d\n", commentCount);
    printf("Invalid Tokens : %d\n", invalidCount);
    printf("-----------------------------------\n");
}