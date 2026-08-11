// The heart of the project. Uses scanner.c to obtain characters
//  and classifies them into tokens. Does NOT print anything and
//  does NOT open files -- those are other modules' jobs.

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"
#include "scanner.h"
#include "keywords.h"
#include "token.h"

/* Local helper functions -- not visible outside this file */
static Token makeToken(TokenType type, const char *lexeme, int lineNumber);

static Token recognizeIdentifierOrKeyword(int firstChar);

static Token recognizeNumber(int firstChar);

static Token recognizeString(void);

static Token recognizeCharacter(void);

static Token recognizeHeader(void);

static Token recognizeComment(void);

static Token recognizeOperator(int firstChar);

// Returns the next classified token from the source file. This is the ONLY function lexer.c exposes.
Token getNextToken(void)
{
    int ch;
    int line;

    // Skip spaces, tabs and newlines
    skipWhitespace();

    ch = getNextChar();
    line = getCurrentLine();

    /* End of file */
    if (ch == EOF)
    {
        return makeToken(TOKEN_END_OF_FILE, "EOF", line);
    }

    /* Identifier or Keyword */
    if (isalpha(ch) || ch == '_')
    {
        return recognizeIdentifierOrKeyword(ch);
    }

    /* Numeric Literal */
    if (isdigit(ch))
    {
        return recognizeNumber(ch);
    }

    /* String Literal */
    if (ch == '"')
    {
        return recognizeString();
    }

    /* Character Literal */
    if (ch == '\'')
    {
        return recognizeCharacter();
    }

    /* Header File */
    if (ch == '#')
    {
        return recognizeHeader();
    }

    /* Comment or '/' operator */
    if (ch == '/')
    {
        int next = peekChar();

        if (next == '/' || next == '*')
        {
            return recognizeComment();
        }

        return recognizeOperator(ch);
    }

    /* All remaining operators and symbols */
    if (strchr("+-*%<>=!&|^~(){}[];,.:?", ch) != NULL)
    {
        return recognizeOperator(ch);
    }

    /* Invalid Token */
    {
        char lexeme[2];

        lexeme[0] = (char)ch;
        lexeme[1] = '\0';

        return makeToken(TOKEN_INVALID, lexeme, line);
    }
}

// Small helper that fills a Token structure.
// Keeps every recognizer function short and readable.

static Token makeToken(TokenType type, const char *lexeme, int lineNumber)
{
    Token token;

    token.type = type;
    strncpy(token.lexeme, lexeme, MAX_LEXEME_LEN - 1);
    token.lexeme[MAX_LEXEME_LEN - 1] = '\0';
    token.lineNumber = lineNumber;

    return token;
}

/*

 * Read the complete word first, Then check the
 * keyword table. Every keyword is first read as
 * if it  an identifier -- that is why the
 * keyword check happens only after the full word
 * has been built.
 */
static Token recognizeIdentifierOrKeyword(int firstChar)
{
    char buffer[MAX_LEXEME_LEN];
    int i = 0;
    int line = getCurrentLine();
    int ch;

    buffer[i++] = (char)firstChar;

    ch = getNextChar();
    while ((isalnum(ch) || ch == '_') && i < MAX_LEXEME_LEN - 1)
    {
        buffer[i++] = (char)ch;
        ch = getNextChar();
    }
    buffer[i] = '\0';

    // put back the character that ended the loop
    ungetChar(ch);

    if (isKeyword(buffer))
    {
        return makeToken(TOKEN_KEYWORD, buffer, line);
    }
    return makeToken(TOKEN_IDENTIFIER, buffer, line);
}

// Handles integer and floating-point literals.
//  Invalid numbers such as 123abc or 12.3.4 are returned as TOKEN_INVALID.
static Token recognizeNumber(int firstChar)
{
    char buffer[MAX_LEXEME_LEN];
    int i = 0;
    int line = getCurrentLine();
    int dotCount = 0;
    int hasLetters = 0;
    int ch;

    buffer[i++] = (char)firstChar;

    ch = getNextChar();

    while ((isdigit(ch) || ch == '.') && i < MAX_LEXEME_LEN - 1)
    {
        if (ch == '.')
        {
            dotCount++;
        }

        buffer[i++] = (char)ch;
        ch = getNextChar();
    }

    /* Invalid number like 123abc */
    while (isalpha(ch) && i < MAX_LEXEME_LEN - 1)
    {
        hasLetters = 1;
        buffer[i++] = (char)ch;
        ch = getNextChar();
    }

    buffer[i] = '\0';

    ungetChar(ch);

    if (hasLetters || dotCount > 1)
    {
        return makeToken(TOKEN_INVALID, buffer, line);
    }

    return makeToken(TOKEN_LITERAL, buffer, line);
}

// Reads characters between double quotes.
// If EOF or newline is reached before the closing quote, the string is invalid.
static Token recognizeString(void)
{
    char buffer[MAX_LEXEME_LEN];
    int i = 0;
    int line = getCurrentLine();
    int ch;

    buffer[i++] = '"';

    ch = getNextChar();

    while (ch != '"' &&
           ch != EOF &&
           ch != '\n' &&
           i < MAX_LEXEME_LEN - 1)
    {
        buffer[i++] = (char)ch;
        ch = getNextChar();
    }

    if (ch == '"')
    {
        if (i < MAX_LEXEME_LEN - 1)
        {
            buffer[i++] = '"';
        }

        buffer[i] = '\0';

        return makeToken(TOKEN_LITERAL, buffer, line);
    }

    /* Unterminated string */
    buffer[i] = '\0';

    return makeToken(TOKEN_INVALID, buffer, line);
}

// Handles character literals like 'A' and escape sequences like '\n'.
// Anything that does not close properly with a single character (e.g. 'AB') is reported as invalid.
static Token recognizeCharacter(void)
{
    char buffer[MAX_LEXEME_LEN];
    int i = 0;
    int line = getCurrentLine();
    int ch;

    buffer[i++] = '\'';

    ch = getNextChar();

    if (ch == '\\')
    {
        /* Escape sequence */
        buffer[i++] = (char)ch;

        ch = getNextChar();
        buffer[i++] = (char)ch;

        ch = getNextChar();
    }
    else
    {
        buffer[i++] = (char)ch;
        ch = getNextChar();
    }

    if (ch == '\'')
    {
        buffer[i++] = '\'';
        buffer[i] = '\0';

        return makeToken(TOKEN_LITERAL, buffer, line);
    }

    /* Invalid character literal */
    while (ch != '\'' && ch != EOF && ch != '\n' && i < MAX_LEXEME_LEN - 1)
    {
        buffer[i++] = (char)ch;
        ch = getNextChar();
    }

    if (ch == '\'' &&
        i < MAX_LEXEME_LEN - 1)
    {
        buffer[i++] = '\'';
    }

    buffer[i] = '\0';

    return makeToken(TOKEN_INVALID, buffer, line);
}

// Handles #include<stdio.h> and #include"myheader.h".
// This is a simplified preprocessor reader used only for recognizing header files.
static Token recognizeHeader(void)
{
    char buffer[MAX_LEXEME_LEN];
    char directive[MAX_LEXEME_LEN];
    int i = 0;
    int d = 0;
    int line = getCurrentLine();
    int ch;

    ch = getNextChar();

    while (ch == ' ' || ch == '\t')
    {
        ch = getNextChar();
    }

    while (isalpha(ch) &&
           d < MAX_LEXEME_LEN - 1)
    {
        directive[d++] = (char)ch;
        ch = getNextChar();
    }

    directive[d] = '\0';

    while (ch == ' ' || ch == '\t')
    {
        ch = getNextChar();
    }

    if (strcmp(directive, "include") == 0 &&
        (ch == '<' || ch == '"'))
    {
        char closing = (ch == '<') ? '>' : '"';

        ch = getNextChar();

        while (ch != closing &&
               ch != EOF &&
               ch != '\n' &&
               i < MAX_LEXEME_LEN - 1)
        {
            buffer[i++] = (char)ch;
            ch = getNextChar();
        }

        buffer[i] = '\0';

        if (ch == closing)
        {
            return makeToken(TOKEN_HEADER, buffer, line);
        }

        return makeToken(TOKEN_INVALID, buffer, line);
    }

    /* Unsupported preprocessor directive */
    ungetChar(ch);

    buffer[0] = '#';

    strncpy(buffer + 1,
            directive,
            MAX_LEXEME_LEN - 2);

    buffer[MAX_LEXEME_LEN - 1] = '\0';

    return makeToken(TOKEN_INVALID, buffer, line);
}

// Called after a '/' has already been seen and the next character is known to be '/' or '*'.
// Handles both single line and multi line comments, including unterminated
//  multi-line comments (EOF reached before closing).
static Token recognizeComment(void)
{
    char buffer[MAX_LEXEME_LEN];
    int i = 0;
    int line = getCurrentLine();
    int second = getNextChar(); /* consume the second '/' or '*' */
    int ch;

    if (second == '/')
    {
        buffer[i++] = '/';
        buffer[i++] = '/';
        ch = getNextChar();
        while (ch != '\n' && ch != EOF && i < MAX_LEXEME_LEN - 1)
        {
            buffer[i++] = (char)ch;
            ch = getNextChar();
        }
        buffer[i] = '\0';
        return makeToken(TOKEN_COMMENT, buffer, line);
    }

    /* Multi line comment */
    buffer[i++] = '/';
    buffer[i++] = '*';
    {
        int prev = 0;
        int closed = 0;

        while ((ch = getNextChar()) != EOF)
        {
            if (i < MAX_LEXEME_LEN - 1)
            {
                buffer[i++] = (char)ch;
            }
            if (prev == '*' && ch == '/')
            {
                closed = 1;
                break;
            }
            prev = ch;
        }
        buffer[i] = '\0';

        if (closed)
        {
            return makeToken(TOKEN_COMMENT, buffer, line);
        }
        return makeToken(TOKEN_INVALID, buffer, line);
    }
}

// Recognizes all operators and special symbols.
// Multi-character operators are handled using one-character lookahead with peekChar().
static Token recognizeOperator(int firstChar)
{
    int line = getCurrentLine();
    int next = peekChar();

    switch (firstChar)
    {
    case '+':
        if (next == '+' || next == '=')
        {
            char op[3];
            op[0] = '+';
            op[1] = (char)getNextChar();
            op[2] = '\0';
            return makeToken(TOKEN_OPERATOR, op, line);
        }
        return makeToken(TOKEN_OPERATOR, "+", line);

    case '-':
        if (next == '-' || next == '=')
        {
            char op[3];
            op[0] = '-';
            op[1] = (char)getNextChar();
            op[2] = '\0';
            return makeToken(TOKEN_OPERATOR, op, line);
        }
        return makeToken(TOKEN_OPERATOR, "-", line);

    case '*':
    case '/':
    case '%':
        if (next == '=')
        {
            char op[3];
            op[0] = (char)firstChar;
            op[1] = (char)getNextChar();
            op[2] = '\0';
            return makeToken(TOKEN_OPERATOR, op, line);
        }
        {
            char op[2];
            op[0] = (char)firstChar;
            op[1] = '\0';
            return makeToken(TOKEN_OPERATOR, op, line);
        }

    case '<':
    case '>':
        if (next == '=' || next == firstChar)
        {
            char op[3];
            op[0] = (char)firstChar;
            op[1] = (char)getNextChar();
            op[2] = '\0';
            return makeToken(TOKEN_OPERATOR, op, line);
        }
        {
            char op[2];
            op[0] = (char)firstChar;
            op[1] = '\0';
            return makeToken(TOKEN_OPERATOR, op, line);
        }

    case '=':
    case '!':
        if (next == '=')
        {
            char op[3];
            op[0] = (char)firstChar;
            op[1] = (char)getNextChar();
            op[2] = '\0';
            return makeToken(TOKEN_OPERATOR, op, line);
        }
        {
            char op[2];
            op[0] = (char)firstChar;
            op[1] = '\0';
            return makeToken(TOKEN_OPERATOR, op, line);
        }

    case '&':
    case '|':
        if (next == firstChar)
        {
            char op[3];
            op[0] = (char)firstChar;
            op[1] = (char)getNextChar();
            op[2] = '\0';
            return makeToken(TOKEN_OPERATOR, op, line);
        }
        {
            char op[2];
            op[0] = (char)firstChar;
            op[1] = '\0';
            return makeToken(TOKEN_OPERATOR, op, line);
        }

    case '^':
    case '~':
    case '(':
    case ')':
    case '{':
    case '}':
    case '[':
    case ']':
    case ';':
    case ',':
    case '.':
    case ':':
    case '?':
    {
        char op[2];
        op[0] = (char)firstChar;
        op[1] = '\0';
        return makeToken(TOKEN_OPERATOR, op, line);
    }

    default:
    {
        char op[2];
        op[0] = (char)firstChar;
        op[1] = '\0';
        return makeToken(TOKEN_INVALID, op, line);
    }
    }
}