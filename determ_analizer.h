#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <list>
#include <string>
#include <tuple>

class State {
public:
	std::string m_name;
	State(const std::string& name_ = "") : m_name(name_) {}
	int friend operator<(const State& obj1, const State& obj2) { return obj1.m_name < obj2.m_name; }
	State& operator=(const std::string& str) { m_name = str; return *this; }

};

class Symbolic_token {
public:
	std::string m_s_lex_name;          //имя лексемы
	int m_s_lex_value;                 //значение символьной лексемы
	size_t m_s_lex_id;                 //id лексемы
	Symbolic_token& operator=(const std::string& str) { m_s_lex_name = str; return *this; };
};

class Lexem {
public:
	size_t m_lex_id;                  //id лексемы
	std::string m_lex_class;          //класс лексемы
	Lexem& operator=(const std::string str) { m_lex_class = str; return *this; }
};

//объявление указателя на функции. Возвращаемое значение State - состояние, в которое автомат переходит после выполнения процедуры
typedef State(*funct_ptr)();


class determ_analizer
{
protected:
	std::map<State, std::map<Symbolic_token, funct_ptr>> m_transition_table; //Таблица переходов анализатора
	State m_cur_state; //Текущее состояние
	Symbolic_token m_cur_sym; //Текущий символ
	std::fstream m_input_file; //Файл
	std::map<std::string, Symbolic_token> m_sym_lexems; //Коллекция символьных лексем
	std::map<std::string, State> m_states;

	virtual Symbolic_token transliterator(int sym) = 0; //Виртуальная функция транслитерации. По входному символу определяет класс лексемы и присваивает ей значение.
public:
	virtual void fill_sym_lexems() = 0;
	virtual void fill_states() = 0;
	virtual void fill_transition_table() = 0;
	determ_analizer(std::string filename); //Конструктор конечного анализатора
	virtual void parse() = 0;  //Функция парсинга
};

