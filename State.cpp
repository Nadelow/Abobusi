#include "State.h"

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