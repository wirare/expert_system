#include <stack>
#include "ast.hpp"
#include "expert_system.hpp"
#include "parsing.hpp"

static const std::map<Token_type, OperatorInfo> &getOpInfo()
{
    static const std::map<Token_type, OperatorInfo> opInfo = {
        {TOKEN_NOT, {5, RIGHT}},
        {TOKEN_AND, {4, LEFT}},
        {TOKEN_XOR, {3, LEFT}},
        {TOKEN_OR, {2, LEFT}},
        {TOKEN_THEN, {1, RIGHT}},
    };

    return opInfo;
}

Tvector rpn(const Tvector &Tokens)
{
    std::map<Token_type, OperatorInfo> opInfo = getOpInfo();
    Tvector output;
    std::stack<Token> op;

    for (const Token &token : Tokens)
    {
        if (token.isSymbol())
            output.push_back(token);
        else if (token.isOperator())
        {
            while (!op.empty())
            {
                Token top = op.top();
                if (top.isOperator())
                {
                    OperatorInfo topInfo = opInfo[top.getType()];
                    OperatorInfo tkInfo = opInfo[token.getType()];
                    bool higherPrecedence =
                        (tkInfo.associativity == LEFT && tkInfo.precedence <= topInfo.precedence) ||
                        (tkInfo.associativity == RIGHT && tkInfo.precedence < topInfo.precedence);
                    
                    if (higherPrecedence) {
                        output.push_back(top);
                        op.pop();
                        continue;
                    }
                }
                break ;
            }
            op.push(token);
        }
        else if (token.getType() & TOKEN_LP)
            op.push(token);
        else if (token.getType() & TOKEN_RP)
        {
            while (op.top().getType() != TOKEN_LP)
            {
                output.push_back(op.top());
                op.pop();
            }
            op.pop();
        }
    }
    while (!op.empty())
    {
        output.push_back(op.top());
        op.pop();
    }
    return output;
}