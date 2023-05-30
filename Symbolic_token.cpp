#include "Symbolic_token.h"

Symbolic_token::Symbolic_token(std::string name_, int value_, size_t id_)
{
    m_sym_name = name_; 
    m_sym_value = value_; 
    m_sym_id = id_;
}

Symbolic_token& Symbolic_token::operator=(const Symbolic_token& obj)
{
    m_sym_name = obj.m_sym_name;
    m_sym_id = obj.m_sym_id;
    m_sym_value = obj.m_sym_value;
    return *this;
}

int operator<(const Symbolic_token& obj1_, const Symbolic_token& obj2_)
{
    return obj1_.m_sym_id < obj2_.m_sym_id;
}

int operator==(const Symbolic_token& obj1_, const Symbolic_token& obj2_)
{
    return obj1_.m_sym_id == obj2_.m_sym_id;
}
