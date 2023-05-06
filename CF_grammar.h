// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#pragma once
#include <string>
#include <set>
#include <map>
#include <fstream>
#include <iostream>
#include <utility>
#include <sstream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <memory>
#include "Symbol.h"

class Grammar_rule
{
public:
    std::shared_ptr<Symbol> m_non_terminal;                                        //Нетерминальный символ из левой части правила
    std::vector<std::shared_ptr<Symbol>> m_right_part;                             //Правая часть правила
    size_t rule_number;
public:

    Grammar_rule() { rule_number = -1; }

    ~Grammar_rule(){}

    Grammar_rule(Grammar_rule& obj)
    {
        rule_number = obj.rule_number;
        m_right_part = obj.m_right_part;
    }
};



class CF_grammar
{
protected:
    
    /*
    нужны мапы с терминалами и нетерминалами для заполнения итоговой таблицы анализатора
    */
    std::map<std::string, std::shared_ptr<Symbol>> m_terminals;
    std::map<std::string, std::shared_ptr<Symbol>> m_non_terminals;

    std::multimap<Symbol, Grammar_rule> m_rules;                      //Множество правил
public:
    CF_grammar(const std::string filename);    
protected:
    void fill_terminals();
};
