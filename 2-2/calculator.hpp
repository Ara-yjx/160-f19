#pragma once

#include "helpers.hpp"

#include <climits>
#include <vector>

// SCANNER DEFINITION

// You can freely add member fields and functions to this class.
class Scanner {
    int line;
    int value;
    char buff[4096];
    int pos;

public:
    char next(int offset = 0);
    void ignoreWhitespace();

    // You really need to implement these four methods for the scanner to work.
    Token nextToken();
    void eatToken(Token);
    int lineNumber();
    int getNumberValue();
    
    Scanner();
};

// PARSER DEFINITION

// You can freely add member fields and functions to this class.
class Parser {
    Scanner scanner;
    
    bool evaluate;

    void start();
    int eatWithNewline(Token toConsume);
    std::vector<int> exprList();
    std::vector<int> exprListRest();
    int expr();
    int exprRest(int operand);
    int term();
    int termRest(int operand);
    int factor();

    // You will need to define the recursive descent functions you're going to use here.
    // WRITEME
public:
    void parse();
    
    Parser(bool);
};



// eatWithNewline
//   eatToken
//     ignoreWhitespace