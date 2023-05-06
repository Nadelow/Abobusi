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
#include "Grammar_rule.h"




class CF_grammar
{
protected:
    
    std::map<std::string, std::shared_ptr<Symbol>> m_terminals;
    std::map<std::string, std::shared_ptr<Symbol>> m_non_terminals;

    std::multimap<Symbol, Grammar_rule> m_rules;                      //Множество правил
public:
    CF_grammar(const std::string filename);    
protected:
    void fill_terminals();
};
