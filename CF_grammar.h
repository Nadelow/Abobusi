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

    Grammar_rule(const Grammar_rule& obj)
    {
        rule_number = obj.rule_number;
        m_right_part = obj.m_right_part;
        m_non_terminal = obj.m_non_terminal;
    }

    Grammar_rule& operator=(const Grammar_rule& obj)
    {
        rule_number = obj.rule_number;
        m_non_terminal = obj.m_non_terminal;
        m_right_part = obj.m_right_part;
    }

    friend bool operator<(const Grammar_rule& obj1, const Grammar_rule& obj2)
    {
        auto it = obj1.m_right_part.begin();
        auto that = obj2.m_right_part.begin();
        while (it != obj1.m_right_part.end() && that != obj2.m_right_part.end() && *(it->get()) == *(that->get()))
        {
            ++it;
            ++that;
        }

        if (it == obj1.m_right_part.end() && that != obj2.m_right_part.end())
            return true;
        if (it != obj1.m_right_part.end() && that != obj2.m_right_part.end())
            return *(*it) < *(*that);

        return false;
    }

    friend bool operator==(const Grammar_rule& obj1, const Grammar_rule& obj2)
    {
        auto it = obj1.m_right_part.begin();
        auto that = obj2.m_right_part.begin();
        while (it != obj1.m_right_part.end() && that != obj2.m_right_part.end() && *(it->get()) == *(that->get()))
        {
            ++it;
            ++that;
        }

        if (it == obj1.m_right_part.end() && that == obj2.m_right_part.end())
            return true;
        else
            return false;
    }

};