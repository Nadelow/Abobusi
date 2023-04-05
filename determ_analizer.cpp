#include "determ_analizer.h"

Symbolic_token& Symbolic_token::operator=(const Symbolic_token& obj)
{
    m_sym_name = obj.m_sym_name; 
    m_sym_id = obj.m_sym_id; 
    m_sym_value = obj.m_sym_value; 
    return *this;
}

State& State::operator=(const State& obj)
{
    m_name = obj.m_name; 
    m_id = obj.m_id; 

    return *this;
}

int operator<(const State& obj1, const State& obj2)
{
    return obj1.m_id < obj2.m_id;
}

int operator<(const Lexem& obj1, const Lexem& obj2)
{
    return obj1.m_lex_id < obj2.m_lex_id;
}

Lexem& Lexem::operator=(const Lexem& obj)
{
    m_lex_class = obj.m_lex_class; 
    m_lex_id = obj.m_lex_id; 

    return *this;
}
