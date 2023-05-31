#include "Symbol.h"

Symbol::Symbol(const Symbol& S)
{
    m_id = S.m_id;
    m_name = S.m_name;
    m_atributes = S.m_atributes;
}
Symbol::Symbol(const Terminal& S)
{
    m_id = S.m_id;
    m_name = S.m_name;
    m_atributes = S.m_atributes;
}
Symbol::Symbol(const Non_terminal& S)
{
    m_id = S.m_id;
    m_name = S.m_name;
    m_atributes = S.m_atributes;
}

Symbol& Symbol::operator=(const Symbol S)
{
    m_id = S.m_id;
    m_name = S.m_name;
    m_atributes = S.m_atributes;
    return *this;
}
Symbol& Symbol::operator=(const Terminal S)
{
    m_id = S.m_id;
    m_name = S.m_name;
    m_atributes = S.m_atributes;
    return *this;
}
Symbol& Symbol::operator=(const Non_terminal S)
{
    m_id = S.m_id;
    m_name = S.m_name;
    m_atributes = S.m_atributes;
    return *this;
}
