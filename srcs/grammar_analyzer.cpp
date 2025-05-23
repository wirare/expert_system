/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grammar_analyzer.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wirare <wirare@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 00:52:49 by wirare            #+#    #+#             */
/*   Updated: 2025/05/24 01:21:47 by wirare           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parsing.hpp"

#include <vector>
#include <stdexcept>
#include <sstream>

void GrammarVerifyPar(std::vector<Token> &Tokens)
{
	int parCount = 0;
	int nb_line = 0;
	for (Token token: Tokens)
	{
		if (token.getType() == TOKEN_NL)
		{
			if (parCount != 0)
				goto THROW;
			nb_line++;
			continue;
		}
		if (token.getType() & (TOKEN_THEN | TOKEN_IFF))
		{
			if (parCount != 0)
				goto THROW;
			continue;
		}
		if (token.getType() == TOKEN_LP)
		{
			parCount++;
		}
		else if (token.getType() == TOKEN_RP)
		{
			parCount--;
			if (parCount < 0)
				goto THROW;
		}
	}
	return ;
THROW:
	std::ostringstream oss;
	oss << "Error: Broken parenthesis at line " << nb_line;
	throw std::runtime_error(oss.str());
}
