/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellanglo <ellanglo@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 22:35:16 by ellanglo          #+#    #+#             */
/*   Updated: 2025/05/21 22:58:19 by ellanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parsing.hpp"

#include <cctype>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <sstream>

void Tokenizer(std::ifstream& file, std::vector<Token>& Tokens)
{
	std::string line;
    MatchEntry matchTable[MatchFuncNb] = MatchFunc;
    bool commentFlag = false;
    int lineNumber = 0;
	while (std::getline(file, line))
	{
        lineNumber++;
		for ( std::string::iterator it=line.begin(); it!=line.end(); ++it)
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
            {
                std::ostringstream oss;
                oss << "Parsing error: unexpected character '" << *it
                    << "' at line " << lineNumber;
                throw std::runtime_error(oss.str());
            }
        }
        if (Tokens.size() && Tokens.back().getType() != TOKEN_NL)
            Tokens.push_back(Token(TOKEN_NL, '/'));
	}
}