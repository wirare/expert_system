/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grammar_analyzer.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wirare <wirare@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 00:52:49 by wirare            #+#    #+#             */
/*   Updated: 2025/05/26 04:32:49 by wirare           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "err_msg.hpp"
#include "parsing.hpp"
#include "expert_system.hpp"

#include <map>
#include <set>

void GrammarVerifyPar(Tvector &Tokens)
{
	int parCount = 0;
	int nb_line = 0;
	for (Token token: Tokens)
	{
		Token_type type = token.getType();
		if (type == TOKEN_NL)
		{
			if (parCount != 0)
				ParsingThrow(PAR_UNMATCH_CLOSE, nb_line);
			nb_line++;
			continue;
		}
		if (type & (TOKEN_THEN | TOKEN_IFF))
		{
			if (parCount != 0)
				ParsingThrow(PAR_COND_IN, nb_line);
			continue;
		}
		if (type == TOKEN_LP)
			parCount++;
		else if (type == TOKEN_RP)
		{
			parCount--;
			if (parCount < 0)
				ParsingThrow(PAR_UNMATCH_OPEN, nb_line);
		}
	}
}

void CheckConditionalToken(Tvector &Tokens)
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
				ParsingThrow(COND_MISSING, nb_line);
			found = false;
			isFactQuery = false;
			nb_line++;
			continue;
		}
		if (type & (TOKEN_THEN | TOKEN_IFF))
		{
			if (found)
				ParsingThrow(COND_TOO_MANY, nb_line);
			found = true;
			continue;
		}
	}
}

int CheckFact(Tvector Tokens)
{
	int factFound = -1;
	bool inFact = false;
	int nb_line = 0;
	std::set<char> symbols;
	for (Token token: Tokens)
	{
		Token_type type = token.getType();
		if (type == TOKEN_FACT)
		{
			if (factFound >= 0)
				ParsingThrow(FACT_TOO_MANY, nb_line);
			factFound = nb_line;
			inFact = true ;
			continue ;
		}
		if (type == TOKEN_NL)
		{
			inFact = false;
			nb_line++;
		}
		if (inFact && type == TOKEN_SYMBOL)
		{
			char val = token.getValue();
			if (symbols.find(val) != symbols.end())
				ParsingThrow(FACT_DUP_SYMB, nb_line);
			symbols.insert(token.getValue());
		}
		if (inFact && type != TOKEN_SYMBOL)
			ParsingThrow(FACT_NOT_SYMB, nb_line);
	}
	if (factFound == -1)
		ParsingThrow(FACT_MISSING, -1);
	return factFound;
}

int CheckQuery(Tvector Tokens)
{
	int queryFound = -1;
	bool inQuery = false;
	int nb_line = 0;
	std::set<char> symbols;
	for (Token token: Tokens)
	{
		Token_type type = token.getType();
		if (type == TOKEN_QUERY)
		{
			if (queryFound >= 0)
				ParsingThrow(QUERY_TOO_MANY, nb_line);
			queryFound = nb_line;
			inQuery = true ;
			continue ;
		}
		if (type == TOKEN_NL)
		{
			inQuery = false;
			nb_line++;
		}
		if (inQuery && type == TOKEN_SYMBOL)
		{
			char val = token.getValue();
			if (symbols.find(val) != symbols.end())
				ParsingThrow(QUERY_DUP_SYMB, nb_line);
			symbols.insert(token.getValue());
		}
		if (inQuery && type != TOKEN_SYMBOL)
			ParsingThrow(QUERY_NOT_SYMB, nb_line);
	}
	if (queryFound == -1)
		ParsingThrow(QUERY_MISSING, -1);
	return queryFound;
}

int GetGrammarRules(Token_type token)
{
	static const std::map<Token_type, int> grammarTable = {
		{TOKEN_START, TOKEN_SYMBOL | TOKEN_NOT | TOKEN_LP | TOKEN_QUERY | TOKEN_FACT},
		{TOKEN_SYMBOL, TOKEN_XOR | TOKEN_OR | TOKEN_AND | TOKEN_COND | TOKEN_NL | TOKEN_RP | TOKEN_SYMBOL},
		{TOKEN_NOT, TOKEN_SYMBOL | TOKEN_LP},
		{TOKEN_XOR, TOKEN_NOT | TOKEN_SYMBOL | TOKEN_LP},
		{TOKEN_OR, TOKEN_NOT | TOKEN_SYMBOL | TOKEN_LP},
		{TOKEN_AND, TOKEN_NOT | TOKEN_SYMBOL | TOKEN_LP},
		{TOKEN_QUERY, TOKEN_SYMBOL},
		{TOKEN_THEN, TOKEN_NOT | TOKEN_SYMBOL},
		{TOKEN_IFF, TOKEN_LP | TOKEN_SYMBOL | TOKEN_NOT},
		{TOKEN_FACT, TOKEN_SYMBOL | TOKEN_NL},
		{TOKEN_LP, TOKEN_NOT | TOKEN_SYMBOL},
		{TOKEN_RP, TOKEN_COND | TOKEN_OR | TOKEN_XOR | TOKEN_AND | TOKEN_NL},
		{TOKEN_NL, TOKEN_SYMBOL | TOKEN_LP | TOKEN_NOT | TOKEN_FACT | TOKEN_QUERY | TOKEN_END},
	};

	auto it = grammarTable.find(token);
	if (it != grammarTable.end())
		return it->second;
	return 0;
}

void CheckGrammar(Tvector &Tokens)
{
	int nb_line = 0;
	Token Current;
	Token Next;
	for (size_t i = 0; i < Tokens.size(); i++)
	{
		Current = Tokens[i];
		Next = Tokens[i+1];
		if (!(GetGrammarRules(Current.getType()) & Next.getType()))
		{
			std::ostringstream oss;
			oss << UNEXP_TKN << Next.getValue();
			ParsingThrow(oss.str(), nb_line);
		}
		if (Next.getType() == TOKEN_END)
			return ;
		if (Current.getType() == TOKEN_NL)
			nb_line++;
	}
}
