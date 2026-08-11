#include <stdio.h>

#include "lexer.h"
#include "scanner.h"
#include "display.h"
#include "bracket.h"

int main(int argc, char *argv[])
{
    FILE *sourceFile;
    Token currentToken;

    if (argc != 2)
    {
        printf("Usage: %s <source_file.c>\n", argv[0]);
        return 1;
    }

    sourceFile = fopen(argv[1], "r");

    if (sourceFile == NULL)
    {
        printf("Error: Could not open file '%s'\n", argv[1]);
        return 1;
    }

    initScanner(sourceFile);
    resetStatistics();
    initBracketStack();

    displayTableHeader();

    do
    {
        currentToken = getNextToken();

        displayToken(currentToken);

        if (currentToken.type != TOKEN_END_OF_FILE)
        {
            checkBracket(currentToken);
        }

    } while (currentToken.type != TOKEN_END_OF_FILE);

    /* Check whether any opening brackets were left */
    checkUnclosedBrackets();

    displayStatistics();

    fclose(sourceFile);

    return 0;
}