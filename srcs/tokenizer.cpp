/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellanglo <ellanglo@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 22:35:16 by ellanglo          #+#    #+#             */
/*   Updated: 2025/05/26 04:24:37 by wirare           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "expert_system.hpp"
#include "parsing.hpp"

#include <cctype>
#include <fstream>

void Tokenizer(std::ifstream& file, Tvector& Tokens)
{
	std::string line;
    MatchEntry matchTable[MatchFuncNb] = MatchFunc;
    bool commentFlag = false;
    int lineNumber = 0;
	Tokens.push_back(Token(TOKEN_START, 'S'));
	while (std::getline(file, line))
	{
		for ( auto it=line.begin(); it!=line.end(); ++it)
        {
            if (commentFlag && *it == '$')
                commentFlag = false;
            if (!commentFlag && *it == '#')
                commentFlag = true;
            if (*it == ' ' || *it == '$' || commentFlag)
                continue;
            int res = 0;
            for (int i = 0; i < MatchFuncNb; i++)
            {
                res = matchTable[i].func(&*it);
                if (res)
                {
                    Tokens.push_back(Token(matchTable[i].type, *it));
                    it += res - 1;
                    break;
                }
            }
            if (!res && !commentFlag)
				ParsingThrow("Unexpected character", lineNumber);
        }
        if (Tokens.size() - 1 && Tokens.back().getType() != TOKEN_NL)
            Tokens.push_back(Token(TOKEN_NL, '/'));
        lineNumber++;
	}
	Tokens.push_back(Token(TOKEN_END, 'E'));
}
