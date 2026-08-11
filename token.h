//Defines the TokenType categories and the Token structure

#ifndef TOKEN_H
#define TOKEN_H

#define MAX_LEXEME_LEN 100

typedef enum
{
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_LITERAL,
    TOKEN_HEADER,
    TOKEN_OPERATOR,
    TOKEN_COMMENT,
    TOKEN_INVALID,
    TOKEN_END_OF_FILE

} TokenType;

typedef struct
{
    TokenType type;
    char lexeme[MAX_LEXEME_LEN];
    int lineNumber;
} Token;

#endif /* TOKEN_H */