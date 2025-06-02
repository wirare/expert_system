/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellanglo <ellanglo@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 22:35:15 by ellanglo          #+#    #+#             */
/*   Updated: 2025/05/26 04:16:50 by wirare           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#pragma once

#include <ostream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <macro.hpp>
#include <fstream>


typedef enum
{
	TOKEN_DFLT = 0,
	TOKEN_FACT = 1 << 0,
	TOKEN_SYMBOL = 1 << 1,
	TOKEN_THEN = 1 << 2,
	TOKEN_IFF = 1 << 3,
	TOKEN_COND = TOKEN_THEN | TOKEN_IFF,
	TOKEN_XOR = 1 << 4,
	TOKEN_AND = 1 << 5,
	TOKEN_OR = 1 << 6,
	TOKEN_NOT = 1 << 7,
	TOKEN_QUERY = 1 << 8,
	TOKEN_NL = 1 << 9,
	TOKEN_LP = 1 << 10,
	TOKEN_RP = 1 << 11,
	TOKEN_PAR = TOKEN_LP | TOKEN_RP,
	TOKEN_START = 1 << 12,
	TOKEN_END = 1 << 13,
}	Token_type;

#define NB_TOKEN = 15;

#define MatchFunc { 			\
	{TOKEN_SYMBOL, isSymbol},	\
	{TOKEN_XOR, isXor},			\
	{TOKEN_AND, isAnd},			\
	{TOKEN_OR, isOr},			\
	{TOKEN_NOT, isNot},			\
	{TOKEN_QUERY, isQuery},		\
	{TOKEN_THEN, isThen},		\
	{TOKEN_IFF, isIff},			\
	{TOKEN_FACT, isFact},		\
	{TOKEN_LP, isLP},			\
	{TOKEN_RP, isRP},			\
}

#define MatchFuncNb 11

typedef int (*MatchFuncPtr)(std::string);
typedef struct {
	Token_type type;
	MatchFuncPtr func;
} MatchEntry;

#define TYPE_TO_STR(Type) case TOKEN_##Type : return "TOKEN_"#Type;

class Token
{
	SETGET(Token_type, _type, Type);
	SETGET(char, _value, Value);

	public:
		std::string toString(void) const 
		{
			switch (_type) 
			{
				TYPE_TO_STR(DFLT)
				TYPE_TO_STR(FACT)
				TYPE_TO_STR(SYMBOL)
				TYPE_TO_STR(THEN)
				TYPE_TO_STR(LP)
				TYPE_TO_STR(RP)
				TYPE_TO_STR(IFF)
				TYPE_TO_STR(XOR)
				TYPE_TO_STR(AND)
				TYPE_TO_STR(OR)
				TYPE_TO_STR(NOT)
				TYPE_TO_STR(QUERY)
				TYPE_TO_STR(NL)
				TYPE_TO_STR(START)
				TYPE_TO_STR(END)
				default:
					return "UNKNOWN";
			}
		}

		Token() {
			_type = TOKEN_DFLT;
			_value = '*';
		};
		Token(const Token_type type, const char &c)	{
			_type = type;
			_value = c;
		};
		~Token() {};

		inline bool isOperator() const { return _type & (TOKEN_XOR | TOKEN_AND | TOKEN_OR | TOKEN_NOT | TOKEN_THEN | TOKEN_IFF); }
		inline bool isSymbol() const { return _type & TOKEN_SYMBOL; }
		inline bool operator&(const int mask) const { return _type & mask; }
};
typedef std::vector<Token> Tvector;
inline std::ostream &operator<<(std::ostream &os, const Token &token) {
	os << "Token type : " << token.toString() << std::endl;
	os << "Token value : " << token.getValue() << std::endl;
	return os;
}

inline std::ostream &operator<<(std::ostream &os, const Tvector &Tokens) {
	for (Token token : Tokens)
		os << token.getValue();
	os << std::endl;
	return os;
}

inline int isFact(std::string str)
{
    return (str[0] == '=');
}

inline int isQuery(std::string str)
{
    return (str[0] == '?');
}

inline int isXor(std::string str)
{
    return (str[0] == '^');
}

inline int isAnd(std::string str)
{
    return (str[0] == '+');
}

inline int isOr(std::string str)
{
    return (str[0] == '|');
}

inline int isNot(std::string str)
{
    return (str[0] == '!');
}

inline int isSymbol(std::string str)
{
    return (str[0] >= 'A' && str[0] <= 'Z');
}

inline int isThen(std::string str)
{
	if (str.length() >= 2)
    	return (2*(str[0] == '=' && str[1] == '>'));
	return (0);
}

inline int isIff(std::string str)
{
	if (str.length() >= 3)
    	return (3*(str[0] == '<' && str[1] == '=' && str[2] == '>'));
	return (0);
}

inline int isLP(std::string str)
{
	return (str[0] == '(');
}

inline int isRP(std::string str)
{
	return (str[0] == ')');
}

inline void ParsingThrow(const std::string& msg, int line)
{
	std::ostringstream oss;
	if (line != -1)
		oss << msg << " at line " << line;
	else
		oss << msg;
	throw std::runtime_error(oss.str());
}

class System;

void CheckConditionalToken(std::vector<Token> &Tokens);
void Tokenizer(std::ifstream& file, std::vector<Token> &Tokens);
void GrammarVerifyPar(std::vector<Token> &Tokens);
int CheckFact(std::vector<Token> Tokens);
int CheckQuery(std::vector<Token> Tokens);
void CheckGrammar(std::vector<Token> &Tokens);
std::ifstream openFile(char *filename);
void Parser(System &System, std::ifstream &file);