/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expert_system.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellanglo <ellanglo@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 22:35:15 by ellanglo          #+#    #+#             */
/*   Updated: 2025/05/26 04:16:50 by wirare           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#pragma once

#include "ast.hpp"
#include <parsing.hpp>
#include <macro.hpp>
#include <vector>
#include <map>

typedef std::vector<Token> Tvector;

struct Rule : Tvector
{
    ASTNode Ast;

    Rule() : Ast() {}
    inline Rule& operator=(const Tvector& copy) {
        Tvector::operator=(copy);
        Ast = ASTNode();
        return *this;
    }
};

class System
{
    SETGET(Tvector, _facts, Facts);
    SETGET(Tvector, _query, Query);

    public:
        System() : _err(0) {}
        ~System() {}

        inline void addRules(const Rule &rule) { _rules.push_back(rule); }

        inline Tvector& getRules(int index) { return _rules[index]; }

        inline std::vector<Rule>& getRules() {return _rules; }
        inline const std::vector<Rule>& getRules() const { return _rules; }

        inline unsigned int getNbRules() { return _rules.size(); }

        inline void setSymbolValue(const char &c, bool val) { _symbolMap[c] = val; }
        inline bool getSymbolValue(const char &c)
        { 
            auto it = _symbolMap.find(c);
            if (it == _symbolMap.end())
            {
                _symbolMap[c] = false;
                return false;
            }
            return it->second;
        }

        inline void setErr() { _err = 1; }
        inline bool getErr() { return _err; }

    private:
        std::vector<Rule> _rules;
        std::map<char, bool> _symbolMap;
        int _err;
};