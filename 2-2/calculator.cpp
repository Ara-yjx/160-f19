
#include "calculator.hpp"
#include <string>
#include <cmath>
#include <iostream>
#include <vector>
#include <bits/stdc++.h> 

// Scanner implementation


Scanner::Scanner() : line(1), 
                     value(0),
                     pos(0) {
    
    for(int i = 0; i < 4096; i++)
        buff[i] = '\0';
    // std::cin.get(buff, 4096); // with \0
    
    int i = 0;
    while(!std::cin.eof() && i < 4096) {
        char got = std::cin.get();
        if(int(got) == -1) continue;
        buff[i] = got;
        i++;
        // std::cout<<int(got)<<'.';
    }
    // buff[i] = '\0';

    // DEBUG std::cout<<"buff: "<<buff<<std::endl;
}


char Scanner::next(int offset) {
    if(this->pos + offset >= 4096) return '\0';
    else return buff[this->pos + offset];
}


Token Scanner::nextToken() {
    char* next = this->buff + pos;

    // ignore whitespace
    while(next[0] == ' ' || next[0] == '\n') 
        next++;

    // num
    if(next[0] >= '0' && next[0] <= '9') {
        std::string num = "";
        for(int offset = 0; next[offset] >= '0' && next[offset] <= '9'; offset++) {
            num += next[offset];
        }
        
        // outofbound exception
        try {
            this->value = std::stoi(num);
        } catch(const std::exception& e) {
            // outOfBoundsError(line, 0);
            this->value = -1;
        }
        
        return T_NUMBER;
    }

    // mod
    if(next[0] == 'm') {
        if(next[1] != 'o') scanError(this->line, next[1]);
        if(next[2] != 'd') scanError(this->line, next[2]);
        return T_MODULO;
    }
    
    // others
    switch (next[0]) {
        case '+':
            return T_PLUS;
            break;
        case '-':
            return T_MINUS;
            break;
        case '*':
            return T_MULTIPLY;
            break;
        case '/':
            return T_DIVIDE;
            break;
        case ';':
            return T_SEMICOLON;
            break;
        case '(':
            return T_OPENPAREN;
            break;
        case ')':
            return T_CLOSEPAREN;
            break; 
        // case '\n':
        //     return T_NEWLN;
        //     break;
        case '\0':
            return T_EOF;
            break;
        default:
            // DEBUG std::cout<<"switch scan error "<<int(next())<<"@"<<pos<<std::endl;
            scanError(this->line, next[0]);
            return T_EOF; // just to stop warning
            break;
    }
}

void Scanner::eatToken(Token toConsume) {
    
    // ignore whitespace
    while(buff[pos] == ' ' || buff[pos] == '\n') {
        if(buff[pos] == '\n')
            line++;
        pos++;
    }

    Token nextToken = this->nextToken();
    // DEBUG 
    // std::cout<<"~"<<tokenToString(nextToken)
    // DEBUG     
    // <<(nextToken==T_NUMBER?std::to_string(this->value):"")<<std::endl;

    // //recursively eat newline
    // if(nextToken == T_NEWLN) { 
    //     this->pos++;
    //     this->line++;
    //     eatToken(toConsume);
    //     return;

    if(nextToken != toConsume) // check token match
        mismatchError(lineNumber(), toConsume, nextToken); 

    if(nextToken == T_MODULO) {
        this->pos += 3;

    } else if(nextToken == T_NUMBER) {
        while(buff[pos] >= '0' && buff[pos] <= '9')
            pos++;
            
    } else {
        this->pos ++;
    }
}
   
int Scanner::lineNumber() {
    return this->line;
}

int Scanner::getNumberValue() {
    return this->value;
}



// Start -> ExprList
// ExprList -> Expr ExprListRest              FIRST { ( num }       FOLLOW { $ }
// ExprListRest -> ; Epxr ExprListRest | e    FIRST { ; e }         FOLLOW { $ }
// Expr -> Term ExprRest                      FIRST { ( num }       FOLLOW { ; ) $ }
// ExprRest -> +- Term ExprRest | e           FIRST { + - e }       FOLLOW { ; ) $ }
// Term -> Factor TermRest                    FIRST { ( num }       FOLLOW { + - ; ) $ }
// TermRest -> */mod Factor | e               FIRST { * / mod e }   FOLLOW { + - ; ) $ }
// Factor -> ( Expr ) | num                   FIRST { ( num }       FOLLOW { + - * / mod ; ) $ }



// Parser implementation
// You may need to modify this constructor and make it do stuff, although it might not be neccessary.
Parser::Parser(bool eval) : evaluate(eval) {
    // WRITEME
    // scanner = Scanner();
}


void Parser::parse() {
    start();
}

void Parser::start() {
    // DEBUG std::cout<<"start"<<std::endl;
    
    std::vector<int> exprListValue = exprList();

    // should have eaten everything; otherwise throw error
    scanner.eatToken(T_EOF);

    // print
    for (std::vector<int>::iterator it=exprListValue.begin(); it<exprListValue.end(); it++)
        std::cout<<*it<<std::endl;
    
    // DEBUG std::cout<<"done~"<<std::endl;

    // I am a placeholder. Implement a recursive descent parser starting from me. Add other methods for different recursive steps.
    // Depending on how you designed your grammar, how many levels of operator precedence you need, etc., you might end up with more
    // or less steps in the process.
    
}

// list of expr values
std::vector<int> Parser::exprList() {
    // DEBUG 
    // std::cout<<"exprList"<<std::endl;

    // -> Expr ExprListRest
    if(scanner.nextToken() == T_OPENPAREN || scanner.nextToken() == T_NUMBER) {
        int exprValue = expr();
        std::vector<int> exprListRestValue = exprListRest();

        if(evaluate) {
            // insert this expr at beginning
            exprListRestValue.insert(exprListRestValue.begin(), exprValue);
            return exprListRestValue;
        } else {
            return std::vector<int>();
        }

    } else {
        parseError(scanner.lineNumber(), scanner.nextToken());
        return std::vector<int>();
    }
}

std::vector<int> Parser::exprListRest() {
    // DEBUG 
    // std::cout<<"exprListRest"<<std::endl;

    // -> ; Epxr ExprListRest
    if(scanner.nextToken() == T_SEMICOLON) {
        scanner.eatToken(T_SEMICOLON);
        int exprValue = expr();
        std::vector<int> exprListRestValue = exprListRest();
        
        if(evaluate) {
        // insert this expr at beginning
            exprListRestValue.insert(exprListRestValue.begin(), exprValue);
            return exprListRestValue;
        } else {
            return std::vector<int>();
        }
    
    // -> e
    } else if(scanner.nextToken() == T_EOF) {
        return std::vector<int>();
    
    } else {
        parseError(scanner.lineNumber(), scanner.nextToken());
        return std::vector<int>();
    }
}

int Parser::expr() {
    // DEBUG 
    // std::cout<<"expr"<<std::endl;
    // -> Term ExprRest
    if(scanner.nextToken() == T_OPENPAREN || scanner.nextToken() == T_NUMBER) {
        int termValue = term();
        int ret = exprRest(evaluate ? termValue : 0);
        return evaluate ? ret : 0;
    
    } else {
        parseError(scanner.lineNumber(), scanner.nextToken());
        return 0;
    }
}

int Parser::exprRest(int operand) {
    // left associative
    // DEBUG 
    // std::cout<<"exprRest"<<std::endl;
    // std::cout<<"--"<<tokenToString(scanner.nextToken())<<"--"<<std::endl;
    Token next = scanner.nextToken();

    // -> +- Term ExprRest
    if(next == T_PLUS || next == T_MINUS) {
        scanner.eatToken(next);
        int termValue = term();

        if(evaluate) {
            long selfValue;
            if(next == T_PLUS)
                selfValue = (long)operand + (long)termValue; // value on left
            else
                selfValue = (long)operand - (long)termValue; 

            // check out-of-bound
            if(selfValue > INT_MAX || selfValue < INT_MIN)
                outOfBoundsError(scanner.lineNumber(), selfValue);
            return exprRest((int)selfValue);

        } else {
            exprRest(0);
            return 0;
        }

    // -> e
    } else if(next == T_SEMICOLON || next == T_CLOSEPAREN || next == T_EOF) {
        return evaluate ? operand : 0;        
    
    } else {
        parseError(scanner.lineNumber(), scanner.nextToken());
        return 0;
    }
}

int Parser::term() {
    // DEBUG 
    // std::cout<<"term"<<std::endl;
    // scanner.eatToken(T_NUMBER);
    // -> Factor TermRest
    if(scanner.nextToken() == T_OPENPAREN || scanner.nextToken() == T_NUMBER) {
        int factorValue = factor();
        int termRestValue = termRest(evaluate ? factorValue : 0);
        return evaluate ? termRestValue : 0;

    } else {
        parseError(scanner.lineNumber(), scanner.nextToken());
        return 0;
    }
}

int Parser::termRest(int operand) {
    // left associative
    // DEBUG 
    // std::cout<<"termRest"<<std::endl;
    Token next = scanner.nextToken();

    // -> * Factor TermRest
    if(next == T_MULTIPLY) {
        scanner.eatToken(T_MULTIPLY);
        int factorValue = factor();
        if(evaluate) {
            long selfValue = (long)operand * (long)factorValue; // value so far
            // check out-of-bound by mod: 
            if(selfValue > INT_MAX || selfValue < INT_MIN)
                outOfBoundsError(scanner.lineNumber(), selfValue);
            return termRest((int)selfValue);
        } else {
            termRest(0);
            return 0;
        }

    // -> / Factor TermRest
    } else if(next == T_DIVIDE) {
        scanner.eatToken(T_DIVIDE);    
        int factorValue = factor();
        if(evaluate) {
            // check divider zero
            if(factorValue == 0) 
                divideByZeroError(scanner.lineNumber(), operand);
            int selfValue = operand / factorValue;
            return termRest(selfValue);
        } else {
            termRest(0);
            return 0;
        }
        

    // -> mod Factor TermRest
    } else if(next == T_MODULO) {
        scanner.eatToken(T_MODULO);    
        int factorValue = factor();
        if(evaluate) {
            // check divider zero
            if(factorValue == 0) 
                divideByZeroError(scanner.lineNumber(), operand);
            int selfValue = operand % factorValue;
            if(selfValue < 0) 
                selfValue += factorValue;
            return termRest(selfValue);
        } else {
            termRest(0);
            return 0;
        }

    // -> epsilon    
    } else if (next == T_PLUS || next == T_MINUS || next == T_SEMICOLON || next == T_CLOSEPAREN || next == T_EOF) { 
        return evaluate ? operand : 0;
    
    } else {
        parseError(scanner.lineNumber(), scanner.nextToken());
        return 0;
    }
}

int Parser::factor() {
    // DEBUG 
    // std::cout<<"factor"<<std::endl;
    
    Token next = scanner.nextToken();
    
    // -> ( Expr )
    if(next == T_OPENPAREN) {
        scanner.eatToken(T_OPENPAREN);
            
        // // patching
        // if(scanner.nextToken() != T_NUMBER && scanner.nextToken() != T_OPENPAREN)
        //     parseError(scanner.lineNumber(), scanner.nextToken());

        int exprValue = expr();
        scanner.eatToken(T_CLOSEPAREN);
        return evaluate ? exprValue : 0;
    
    // -> num
    } else if (next == T_NUMBER) {
        scanner.eatToken(T_NUMBER);    
        return evaluate ? scanner.getNumberValue() : 0;
    
    } else {
        parseError(scanner.lineNumber(), scanner.nextToken());
        return 0;
    }

}


// You will need to add more methods for this to work. Don't forget to also define them in calculator.hpp!
// WRITEME

// todo: safe add instead of normal add