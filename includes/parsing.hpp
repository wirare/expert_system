/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellanglo <ellanglo@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 22:35:15 by ellanglo          #+#    #+#             */
/*   Updated: 2025/05/24 04:58:38 by wirare           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <stdexcept>

#define SETGET(type, _name, Name) 											\
	public: 																\
		inline void set##Name(const type &value) { this->_name = value; }	\
		inline type get##Name(void) const { return this->_name; }			\
	private:																\
		type _name;															\

typedef enum
{
	TOKEN_DFLT = 0,
	TOKEN_FACT = 1 << 0,
	TOKEN_SYMBOL = 1 << 1,
	TOKEN_THEN = 1 << 2,
	TOKEN_IFF = 1 << 3,
	TOKEN_XOR = 1 << 4,
	TOKEN_AND = 1 << 5,
	TOKEN_OR = 1 << 6,
	TOKEN_NOT = 1 << 7,
	TOKEN_QUERY = 1 << 8,
	TOKEN_NL = 1 << 9,
	TOKEN_LP = 1 << 10,
	TOKEN_RP = 1 << 11,
	TOKEN_START = 1 << 12,
	TOKEN_END = 1 << 13,
}	Token_type;

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

class Token
{
	SETGET(Token_type, _type, Type);
	SETGET(char, _value, Value);

	public:
		Token() {
			_type = TOKEN_DFLT;
			_value = '*';
		};
		Token(const Token_type type, const char &c)	{
			_type = type;
			_value = c;
		};
		~Token() {};
};
inline std::string TokenTypeToStr(const Token &token);
inline std::ostream &operator<<(std::ostream &os, const Token &token) {
	os << "Token type : " << TokenTypeToStr(token) << std::endl;
	os << "Token value : " << token.getValue() << std::endl;
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

#define TYPE_TO_STR(Type) case TOKEN_##Type : return "TOKEN_"#Type;

inline std::string TokenTypeToStr(const Token &token)
{
	std::string str;
	switch (token.getType())
	{
		TYPE_TO_STR(DFLT)
		case TOKEN_FACT:
			str = "TOKEN_FACT";
			break;
		TYPE_TO_STR(SYMBOL)
		case TOKEN_THEN:
			str = "TOKEN_THEN";
			break;
		case TOKEN_IFF:
			str = "TOKEN_IFF";
			break;
		case TOKEN_XOR:
			str = "TOKEN_XOR";
			break;
		case TOKEN_AND:
			str = "TOKEN_AND";
			break;
		case TOKEN_OR:
			str = "TOKEN_OR";
			break;
		case TOKEN_NOT:
			str = "TOKEN_NOT";
			break;
		case TOKEN_QUERY:
			str = "TOKEN_QUERY";
			break;
		case TOKEN_NL:
			str = "TOKEN_NL";
			break;
		TYPE_TO_STR(END)
		default:
			str = "UNKNOWN_TOKEN";
			break;
	}
	return str;
}

inline void ParsingThrow(const std::string& msg, int line)
{
	std::ostringstream oss;
	oss << msg << " at line " << line;
	throw std::runtime_error(oss.str());
}

void CheckConditionalToken(std::vector<Token> &Tokens);
void Tokenizer(std::ifstream& file, std::vector<Token>& Tokens);
void GrammarVerifyPar(std::vector<Token> &Tokens);
void CheckFactQueryToken(std::vector<Token> Tokens);
