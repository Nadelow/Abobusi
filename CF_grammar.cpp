// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "CF_grammar.h"
#include <string>

/*
использовать shared ptr
*/

void CF_grammar::fill_terminals()
{
    m_terminals.emplace("line_num", std::make_shared<Terminal>("line_num", 0));
    m_terminals.emplace("operand", std::make_shared<Terminal>("operand", 1));
    m_terminals.emplace("aur_op", std::make_shared<Terminal>("aur_op", 2));
    m_terminals.emplace("relation", std::make_shared<Terminal>("relation", 3));
    m_terminals.emplace("NEXT", std::make_shared<Terminal>("NEXT", 4));
    m_terminals.emplace("LET", std::make_shared<Terminal>("LET", 5));
    m_terminals.emplace("FOR", std::make_shared<Terminal>("FOR", 6));
    m_terminals.emplace("GOTO", std::make_shared<Terminal>("GOTO", 7));
    m_terminals.emplace("GOSUB", std::make_shared<Terminal>("GOSUB", 8));
    m_terminals.emplace("LBRACE", std::make_shared<Terminal>("LBRACE", 9));
    m_terminals.emplace("RBRACE", std::make_shared<Terminal>("RBRACE", 10));
    m_terminals.emplace("IF", std::make_shared<Terminal>("IF", 11));
    m_terminals.emplace("RETURN", std::make_shared<Terminal>("RETURN", 12));
    m_terminals.emplace("END", std::make_shared<Terminal>("END", 13));
    m_terminals.emplace("TO", std::make_shared<Terminal>("TO", 14));
    m_terminals.emplace("STEP", std::make_shared<Terminal>("STEP", 15));
    m_terminals.emplace("REM", std::make_shared<Terminal>("REM", 16));
    m_terminals.emplace("ERROR", std::make_shared<Terminal>("ERROR", 17));
    m_terminals.emplace("EOF", std::make_shared<Terminal>("EOF", 18));
}

CF_grammar::CF_grammar(const std::string filename)
{
    //std::map<std::string, size_t> index_sym;
    Grammar_rule curr_rule;    //Текущее правило
    std::shared_ptr<Symbol> sym;          //Символы, из которых состоит правая часть правила
    std::string word;          //Текущее считанное слово

    std::map<int, int> a;
    

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







































