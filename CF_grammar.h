#pragma once
#include <map>
#include <string>
#include <set>
#include"Non_terminal.h"
#include"BaseSymbolClass.h"
//#include"GrammarRuleClass.h"  //’з, как –емизов назвал файл заголовков класса Grammar_rule


class CF_grammar
{
protected:
	std::map<std::string, size_t> m_alphabet;
	std::map<size_t, BaseSymbol> m_reverse_alphabet;
	//std::multiset<Non_terminal, GrammarRule> m_rules;
};