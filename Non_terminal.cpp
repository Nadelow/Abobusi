#include"Non_terminal.h"


bool Non_terminal::operator== (const Non_terminal& other)
{
	return(m_id == other.m_id);
}

bool Non_terminal::operator< (const Non_terminal& other)
{
	return(m_id < other.m_id);
}