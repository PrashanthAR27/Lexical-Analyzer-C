

#include "scanner.h"

static FILE *sourceFile;
static int currentLine;
static int hasPushback;
static int pushbackChar;

//initScanner Purpose is to Initializes the scanner to read from the given file.
void initScanner(FILE *file)
{
    sourceFile = file;
    currentLine = 1;
    hasPushback = 0;
    pushbackChar = 0;
}

//Returns the next character from the source file.Updates the line number whenever a newline is read.
int getNextChar(void)
{
    int ch;

    if (hasPushback)
    {
        hasPushback = 0;
        ch = pushbackChar;
    }
    else
    {
        ch = fgetc(sourceFile);
    }

    if (ch == '\n')
    {
        currentLine++;
    }

    return ch;
}

// Returns the next character without removing it from the input stream.
int peekChar(void)
{
    int ch;

    ch = getNextChar();
    ungetChar(ch);

    return ch;
}

// Pushes one character back into the scanner.Only one level of pushback is supported.
void ungetChar(int ch)
{
    if (ch == '\n')
    {
        currentLine--;
    }

    pushbackChar = ch;
    hasPushback = 1;
}

//Skips spaces, tabs, carriage returns and newlines until a meaningful character is found.
void skipWhitespace(void)
{
    int ch;

    while ((ch = getNextChar()) != EOF)
    {
        if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r')
        {
            continue;
        }

        ungetChar(ch);
        break;
    }
}

//Returns the current line number.
int getCurrentLine(void)
{
    return currentLine;
}