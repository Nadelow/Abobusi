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
Non_terminal::Non_terminal(const Symbol& S)
{
    m_id = S.m_id;
    m_name = S.m_name;
    m_atributes = S.m_atributes;
}
Non_terminal::Non_terminal(const Terminal& S)
{
    m_id = S.m_id;
    m_name = S.m_name;
    m_atributes = S.m_atributes;
}
Non_terminal::Non_terminal(const Non_terminal& S) : Symbol(S)
{
    m_id = S.m_id;
    m_name = S.m_name;
    m_atributes = S.m_atributes;
}

Non_terminal& Non_terminal::operator=(const Symbol S)
{
    m_id = S.m_id;
    m_name = S.m_name;
    m_atributes = S.m_atributes;
    return *this;
}
Non_terminal& Non_terminal::operator=(const Terminal S)
{
    m_id = S.m_id;
    m_name = S.m_name;
    m_atributes = S.m_atributes;
    return *this;
}
Non_terminal& Non_terminal::operator=(const Non_terminal S)
{
    m_id = S.m_id;
    m_name = S.m_name;
    m_atributes = S.m_atributes;
    return *this;
}

Terminal::Terminal(const Symbol& S)
{
   m_id = S.m_id;
   m_name = S.m_name;
   m_atributes = S.m_atributes;
}
Terminal::Terminal(const Terminal& S) : Symbol(S)
{
   m_id = S.m_id;
   m_name = S.m_name;
   m_atributes = S.m_atributes;
}
Terminal::Terminal(const Non_terminal& S)
{
   m_id = S.m_id;
   m_name = S.m_name;
   m_atributes = S.m_atributes;
}

Terminal& Terminal::operator=(const Symbol S)
{
   m_id = S.m_id;
   m_name = S.m_name;
   m_atributes = S.m_atributes;
   return *this;
}
Terminal& Terminal::operator=(const Terminal S)
{
   m_id = S.m_id;
   m_name = S.m_name;
   m_atributes = S.m_atributes;
   return *this;
}
Terminal& Terminal::operator=(const Non_terminal S)
{
   m_id = S.m_id;
   m_name = S.m_name;
   m_atributes = S.m_atributes;
   return *this;

}
