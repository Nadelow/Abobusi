// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
//g++ -c Symbol.cpp CF_grammar.cpp Source.cpp -fsanitize=address -Wall -Wextra -std=c++20 -lm
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
#include <list>

class Symbol;
class Terminal;
class Non_terminal;

class Symbol
{
public:
	size_t m_id = 36;
	std::string m_name;
	std::list<long long int> m_atributes;
public:
	virtual ~Symbol() {}
	Symbol(std::string name_ = "", size_t id_ = 0, std::list<long long int> atributes = std::list<long long int>()) : m_id(id_), m_name(name_), m_atributes(atributes) {}
	
	Symbol(const Symbol& S);
	Symbol(const Terminal& S);
	Symbol(const Non_terminal& S);
	
	Symbol& operator=(const Symbol S) ;
	Symbol& operator=(const Terminal S);
	Symbol& operator=(const Non_terminal S);
	
	friend bool operator<(const Symbol& obj1, const Symbol& obj2)
	{
		return obj1.m_id < obj2.m_id;
	}
	friend bool operator==(const Symbol& obj1, const Symbol& obj2)
	{
		return obj1.m_id == obj2.m_id;
	}
};

class Non_terminal : public Symbol
{
public:
	virtual ~Non_terminal() {};
	Non_terminal(std::string name_ = "", size_t id_ = 0) : Symbol(name_, id_) {}
	
	Non_terminal(const Symbol& S);
	Non_terminal(const Terminal& S);
	Non_terminal(const Non_terminal& S);

	Non_terminal& operator=(const Symbol S);
	Non_terminal& operator=(const Terminal S);
	Non_terminal& operator=(const Non_terminal S);
	
	friend bool operator<(const Non_terminal& obj1, const Non_terminal& obj2)
	{
		return obj1.m_id < obj2.m_id;
	}
	friend bool operator==(const Non_terminal& obj1, const Non_terminal& obj2)
	{
		return obj1.m_id == obj2.m_id;
	}
};
