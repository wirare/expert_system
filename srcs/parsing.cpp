/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellanglo <ellanglo@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 22:35:16 by ellanglo          #+#    #+#             */
/*   Updated: 2025/05/24 04:58:54 by wirare           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parsing.hpp"

#include <fstream>
#include <iostream>
#include <vector>

std::ifstream openFile(char *filename)
{
	std::ifstream file(filename);
	if (!file.is_open())
	{
		std::cerr << "Error: Could not open file " << filename << std::endl;
		exit(EXIT_FAILURE);
	}
	return file;
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Usage: " << argv[0] << " <filename>" << std::endl;
		return 1;
	}

	std::ifstream file = openFile(argv[1]);

	std::vector<Token> Tokens;
	try { Tokenizer(file, Tokens); }
    catch (const std::runtime_error& e) {
        std::cerr << "Tokenizer error: " << e.what() << '\n';
        return 1;
    }
	try { 
		GrammarVerifyPar(Tokens);
		CheckConditionalToken(Tokens);
		CheckFactQueryToken(Tokens);
	}
	catch (const std::runtime_error& e) {
		std::cerr << "Grammar error: " << e.what() << '\n';
		return 1;
	}
	return 0;
}
