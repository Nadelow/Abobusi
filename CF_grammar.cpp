// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "CF_grammar.h"
#include <string>

/*
использовать shared ptr
*/

void CF_grammar::fill_terminals()
{
    m_terminals.emplace("LINE_NUM",    std::make_shared<Terminal>("LINE_NUM" ,0)); //line num
    m_terminals.emplace("OPERAND",std::make_shared<Terminal>("OPERAND" ,1)); //variable/constant
    m_terminals.emplace("^", std::make_shared<Terminal>("^" ,2));
    m_terminals.emplace("REL",  std::make_shared<Terminal>("REL" ,3));
    m_terminals.emplace("NEXT",   std::make_shared<Terminal>("NEXT" ,4));
    m_terminals.emplace("LET",    std::make_shared<Terminal>("LET" ,5));
    m_terminals.emplace("FOR",    std::make_shared<Terminal>("FOR" ,6));
    m_terminals.emplace("GOTO",   std::make_shared<Terminal>("GOTO" ,7));
    m_terminals.emplace("GOSUB",  std::make_shared<Terminal>("GOSUB" ,8));
    m_terminals.emplace("(",      std::make_shared<Terminal>("(" ,9));
    m_terminals.emplace(")",      std::make_shared<Terminal>(")" ,10));
    m_terminals.emplace("IF",     std::make_shared<Terminal>("IF" ,11));
    m_terminals.emplace("RETURN", std::make_shared<Terminal>("RETURN" ,12));
    m_terminals.emplace("END",    std::make_shared<Terminal>("END" ,13));
    m_terminals.emplace("TO",     std::make_shared<Terminal>("TO" ,14));
    m_terminals.emplace("STEP",   std::make_shared<Terminal>("STEP" ,15));
    m_terminals.emplace("REM",    std::make_shared<Terminal>("REM" ,16));
    m_terminals.emplace("!error!",std::make_shared<Terminal>("!error!" ,17));
    m_terminals.emplace("EOF",    std::make_shared<Terminal>("EOF" ,18));
    m_terminals.emplace("+", std::make_shared<Terminal>("+", 19));
    m_terminals.emplace("-", std::make_shared<Terminal>("-", 20));
    m_terminals.emplace("*", std::make_shared<Terminal>("*", 21));
    m_terminals.emplace("/", std::make_shared<Terminal>("/", 22));
}

CF_grammar::CF_grammar(const std::string filename)
{
    //std::map<std::string, size_t> index_sym;
    Grammar_rule curr_rule;    //Текущее правило
    std::shared_ptr<Symbol> sym;          //Символы, из которых состоит правая часть правила
    std::string word;          //Текущее считанное слово

    fill_terminals();          //Заполняем коллекцию терминальных символов

    size_t curr_non_term_index = m_terminals.size() + 1;  //Начинаем нумерацию нетерминалов со следующего номера, после немерации терминалов
    size_t curr_rule_num = 1;

    std::ifstream input_file(filename);
    if(!input_file)
    {
        std::cout << "In CF_grammar::CF_grammar error openning " << filename << std::endl;
        return;
    }
    
    //Цикл идёт по строкам входного файла
    while(input_file.peek() != EOF)
    {
        curr_rule.rule_number = curr_rule_num++;

        input_file >> word;
        
        if (word[0] != '[')                            //В начале строки не нетерминал
        {
            std::cout << "Wrong input format";
            input_file.close();
            exit(1);
        }

        if (m_non_terminals.contains(word))
            curr_rule.m_non_terminal = m_non_terminals[word];
        else
        {
            curr_rule.m_non_terminal = std::make_shared<Non_terminal>();
            curr_rule.m_non_terminal->m_id = curr_non_term_index;
            curr_non_term_index++;
            curr_rule.m_non_terminal->m_name = word;
            m_non_terminals.emplace(word, curr_rule.m_non_terminal);
        }
        
        input_file >> word;

        while(input_file.peek()!='\n' && input_file.peek() != EOF)          //Цикл идёт до конца строки, считывая слова и определяя, являются ли они термилалами, или нет
        {
            
            input_file >> word;

            if (word[0] != '[') {
                if (!m_terminals.contains(word))
                {
                    std::cout << "unexpected terminal";
                    input_file.close();
                    exit(1);
                }
                
                sym = m_terminals[word];
            }
            else
            {
                if (m_non_terminals.contains(word))
                    sym = m_non_terminals[word];
                else
                {
                    sym = std::make_shared<Non_terminal>();
                    sym->m_id = curr_non_term_index;
                    sym->m_name = word;
                    m_non_terminals.emplace(word, sym);
                    curr_non_term_index++;
                }
            }
            curr_rule.m_right_part.push_back(sym);

            while (input_file.peek() == ' ')
                input_file.ignore();
        }

        m_rules.emplace(*curr_rule.m_non_terminal, curr_rule);
        curr_rule.m_right_part.clear();
        
    }

    input_file.close();
}







std::set<std::shared_ptr<Symbol>> CF_grammar::FIRST1(const std::shared_ptr<Symbol>& S)
{
    std::set<Symbol> seen;

    if (dynamic_cast<Terminal*>(S.get()))
    {
        std::set<std::shared_ptr<Symbol>> res;
        res.insert(S);
        return res;
    }


    return FIRST1_REC(S, seen);
}
std::set<std::shared_ptr<Symbol>> CF_grammar::FIRST1_REC(const std::shared_ptr<Symbol>& S, std::set<Symbol>& seen)
{
    std::set<std::shared_ptr<Symbol>> first_syms;

    size_t rules_amount = m_rules.count(*S);
    auto it = m_rules.find(*S);

    for (size_t i = 0; it != m_rules.end() && i < rules_amount; ++i, ++it)
    {
        if (dynamic_cast<Terminal*>((*it).second.m_right_part[0].get()))        
        else if (seen.find(*((*it).second.m_right_part[0])) == seen.end())      
        {
            seen.insert(*((*it).second.m_right_part[0]));
            first_syms.merge(FIRST1_REC((*it).second.m_right_part[0], seen));
        }
    }

    return first_syms;
}


std::set<std::shared_ptr<Symbol>> CF_grammar::END1(const std::shared_ptr<Symbol>& S)
{
    if (dynamic_cast<Terminal*>(S.get()))
    {
        return std::set<std::shared_ptr<Symbol>>();
    }

    std::set<Symbol> seen;

    return END1_REC(S, seen);
}

std::set<std::shared_ptr<Symbol>> CF_grammar::END1_REC(const std::shared_ptr<Symbol>& S, std::set<Symbol>& seen)
{
    std::set<std::shared_ptr<Symbol>> last_syms;

    size_t rules_amount = m_rules.count(*S);
    auto it = m_rules.find(*S);

    for (size_t i = 0; it != m_rules.end() && i < rules_amount; ++i, ++it)
    {
        last_syms.insert((*it).second.m_right_part[(*it).second.m_right_part.size() - 1]);

        if (seen.find(*it->second.m_right_part[(*it).second.m_right_part.size() - 1]) == seen.end())
        {
            seen.insert(*it->second.m_right_part[(*it).second.m_right_part.size() - 1]);
            last_syms.merge(END1_REC((*it).second.m_right_part[(*it).second.m_right_part.size() - 1], seen));
        }
    }

    return last_syms;
}

































