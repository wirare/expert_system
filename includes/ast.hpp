#pragma once

#include "parsing.hpp"

enum Assoc {LEFT, RIGHT};

struct OperatorInfo {
    int precedence;
    Assoc associativity;
};

Tvector rpn(const Tvector &Tokens);

struct ASTNode {
    Token token;
    ASTNode* left = nullptr;
    ASTNode* right = nullptr;

    ASTNode() : token() {}
    ASTNode(Token t) : token(t) {}
    ASTNode(Token t, ASTNode* child) : token(t), left(child) {}
    ASTNode(Token t, ASTNode* l, ASTNode* r) : token(t), left(l), right(r) {}
};