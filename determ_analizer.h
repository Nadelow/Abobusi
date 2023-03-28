#pragma once
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <tuple>

class State {
public:
	std::string m_name;
	size_t m_id;
	State(const std::string& name_ = "", size_t id_ = 0) : m_name(name_),  m_id(id_){}
	int friend operator<(const State& obj1, const State& obj2) { return obj1.m_id < obj2.m_id; }
	State& operator=(const State& obj) { m_name = obj.m_name; m_id = obj.m_id; return *this; }
};

class Symbolic_token {
public:
	std::string m_s_lex_name;          //имя лексемы
	int m_s_lex_value;                 //значение символьной лексемы
	size_t m_s_lex_id;                 //id лексемы
	Symbolic_token(std::string name_ = "", int value_ = -1, size_t id_ = 0) : m_s_lex_name(name_), m_s_lex_value(value_), m_s_lex_id(id_){};
	Symbolic_token& operator=(const Symbolic_token& obj) { m_s_lex_name = obj.m_s_lex_name; m_s_lex_id = obj.m_s_lex_id; m_s_lex_value = obj.m_s_lex_value; return *this; };
	int friend operator<(const Symbolic_token& obj1_, const Symbolic_token& obj2_) { return obj1_.m_s_lex_id < obj2_.m_s_lex_id; }
};

class Lexem {
public:
	size_t m_lex_id;                  //id лексемы
	std::string m_lex_class;          //класс лексемы
	Lexem(std::string lex_class = "", size_t id = 0) : m_lex_class(lex_class), m_lex_id(id) {}
	Lexem& operator=(const Lexem& obj) { m_lex_class = obj.m_lex_class; m_lex_id = obj.m_lex_id; return *this; }
	int friend operator<(const Lexem& obj1, const Lexem& obj2) { return obj1.m_lex_id < obj2.m_lex_id; }
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
	//determ_analizer(std::string filename); //Конструктор конечного анализатора
	virtual void parse() = 0;  //Функция парсинга
};

