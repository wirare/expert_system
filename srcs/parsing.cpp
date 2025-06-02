/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellanglo <ellanglo@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 22:35:16 by ellanglo          #+#    #+#             */
/*   Updated: 2025/05/26 04:32:58 by wirare           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parsing.hpp"
#include "err_msg.hpp"
#include "expert_system.hpp"
#include "ast.hpp"

#include <fstream>
#include <iterator>

std::ifstream openFile(char *filename)
{
	std::ifstream file(filename);
	if (!file.is_open())
	{
		std::cerr << FILE_N_OPEN << filename << std::endl;
		exit(EXIT_FAILURE);
	}
	return file;
}

void separateIff(System &System)
{
	for (Tvector &rule: System.getRules())
	{
		Tvector lhs, rhs;
		Rule tmp;
		bool after_cond = false;
		bool cond_is_iff = false;
		for (Token &token: rule)
		{
			if (token & TOKEN_COND)
			{
				after_cond = true;
				if (token & TOKEN_IFF)
				{
					cond_is_iff = true;
					token.setType(TOKEN_THEN);
					token.setValue('=');
				}
			}
			else if (!after_cond)
				lhs.push_back(token);
			else
			 	rhs.push_back(token);
		}
		if (cond_is_iff)
		{
			tmp = rhs;
			tmp.push_back(Token(TOKEN_THEN, '='));
			tmp.insert(tmp.end(), std::make_move_iterator(lhs.begin()), std::make_move_iterator(lhs.end()) );
			System.addRules(tmp);
		}
	}
}

void Parser(System &System, std::ifstream &file)
{
	Tvector Tokens;

	try { Tokenizer(file, Tokens); }
    catch (const std::runtime_error& e) {
        std::cerr << TOKENIZER_ERR << e.what() << '\n';
		System.setErr();
    }
	try { 
		GrammarVerifyPar(Tokens);
		CheckConditionalToken(Tokens);
		if (CheckFact(Tokens) == CheckQuery(Tokens))
			ParsingThrow(FACT_QUERY_LINE, -1);
		CheckGrammar(Tokens);
	}
	catch (const std::runtime_error& e) {
		std::cerr << GRAMMAR_ERR << e.what() << '\n';
        System.setErr();
	}
	Rule tmp;
	int mode = 0;
	for (Token token: Tokens)
	{
		if (token.getType() & (TOKEN_END | TOKEN_START))
			continue;
		if (token.getType() & TOKEN_NL)
		{
			switch (mode) 
			{
				case 1:
					System.setFacts(tmp);
					break;
				case 2:
					System.setQuery(tmp);
					break;
				default:
					System.addRules(tmp);
			}
			mode = 0;
			tmp.clear();
		}
		else if (token.getType() & TOKEN_FACT)
			mode = 1;
		else if (token.getType() & TOKEN_QUERY)
			mode = 2;
		else 
			tmp.push_back(token);
	}
	separateIff(System);
	for (const Tvector &rule: System.getRules())
		rpn(rule);
}