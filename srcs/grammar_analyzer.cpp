/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grammar_analyzer.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wirare <wirare@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 00:52:49 by wirare            #+#    #+#             */
/*   Updated: 2025/05/24 05:01:01 by wirare           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parsing.hpp"

#include <vector>

void GrammarVerifyPar(std::vector<Token> &Tokens)
{
	int parCount = 0;
	int nb_line = 0;
	for (Token token: Tokens)
	{
		Token_type type = token.getType();
		if (type == TOKEN_NL)
		{
			if (parCount != 0)
				ParsingThrow("Broken parenthesis", nb_line);
			nb_line++;
			continue;
		}
		if (type & (TOKEN_THEN | TOKEN_IFF))
		{
			if (parCount != 0)
				ParsingThrow("Broken parenthesis", nb_line);
			continue;
		}
		if (type == TOKEN_LP)
			parCount++;
		else if (type == TOKEN_RP)
		{
			parCount--;
			if (parCount < 0)
				ParsingThrow("Broken parenthesis", nb_line);
		}
	}
}

void CheckConditionalToken(std::vector<Token> &Tokens)
{
	bool found = false;
	int nb_line = 0;
	bool isFactQuery = false;
	for (Token token: Tokens)
	{
		Token_type type = token.getType();
		if (type & (TOKEN_FACT | TOKEN_QUERY))
		{
			isFactQuery = true;
			continue;
		}
		if (type == TOKEN_NL)
		{
			if (!found && !isFactQuery)
				ParsingThrow("Missing conditional token", nb_line);
			found = false;
			isFactQuery = false;
			nb_line++;
			continue;
		}
		if (type & (TOKEN_THEN | TOKEN_IFF))
		{
			if (found)
				ParsingThrow("Too many conditional tokens", nb_line);
			found = true;
			continue;
		}
	}
}

void CheckFactQueryToken(std::vector<Token> Tokens)
{
	int factFound = -1;
	int queryFound = -1;
	int nb_line = 0;
	for (Token token: Tokens)
	{
		Token_type type = token.getType();
		if (type == TOKEN_FACT)
		{
			if (factFound >= 0)
				ParsingThrow("Two or more Fact founds", nb_line);
			factFound = nb_line;
			if (queryFound == factFound)
				ParsingThrow("Fact and Query can't be on the same line", nb_line);
			continue ;
		}
		if (type == TOKEN_QUERY)
		{
			if (queryFound >= 0)
				ParsingThrow("Two or more Query founds", nb_line);
			queryFound = nb_line;
			if (queryFound == factFound)
				ParsingThrow("Fact and Query can't be on the same line", nb_line);
			continue ;
		}
		if (type == TOKEN_NL)
			nb_line++;
	}
}
/*
static int GetGrammarRules(Token_type token)
{
	static const int grammarTable[] = {
		[TOKEN_START] = TOKEN_SYMBOL | TOKEN_NOT | TOKEN_LP | TOKEN_QUERY | TOKEN_FACT,
		[TOKEN_SYMBOL] = TOKEN_XOR | TOKEN_OR | TOKEN_AND | TOKEN_IFF | TOKEN_THEN | TOKEN_NL | TOKEN_RP,
		[TOKEN_NOT] = TOKEN_SYMBOL | TOKEN_LP,
		[TOKEN_XOR] = TOKEN_NOT | TOKEN_SYMBOL | TOKEN_LP,
		[TOKEN_OR] = TOKEN_NOT | TOKEN_SYMBOL | TOKEN_LP,
		[TOKEN_AND] = TOKEN_NOT | TOKEN_SYMBOL | TOKEN_LP,
		[TOKEN_QUERY] = TOKEN_SYMBOL,
		[TOKEN_THEN] = TOKEN_NOT | TOKEN_SYMBOL,
		[TOKEN_IFF] = TOKEN_LP | TOKEN_SYMBOL | TOKEN_NOT,
		[TOKEN_FACT] = TOKEN_SYMBOL,
		[TOKEN_LP] = TOKEN_NOT | TOKEN_SYMBOL,
		[TOKEN_RP] = TOKEN_THEN | TOKEN_IFF | TOKEN_OR | TOKEN_XOR | TOKEN_AND | TOKEN_NL,
		[TOKEN_NL] = TOKEN_SYMBOL | TOKEN_LP | TOKEN_NOT | TOKEN_FACT | TOKEN_QUERY,
	};

	return grammarTable[token];
}
*/