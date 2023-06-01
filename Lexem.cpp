#include "Lexem.h"

int operator<(const Lexem& obj1, const Lexem& obj2)
{
    return obj1.m_lex_id < obj2.m_lex_id;
}

bool operator==(const Lexem& obj1, const Lexem& obj2)
{
    return obj1.m_lex_id == obj2.m_lex_id;
}

Lexem::Lexem(const Lexem& obj)
{
    m_lex_class = obj.m_lex_class;
    m_lex_id = obj.m_lex_id;
}

Lexem& Lexem::operator=(const Lexem& obj)
{
    m_lex_class = obj.m_lex_class;
    m_lex_id = obj.m_lex_id;

    return *this;
}