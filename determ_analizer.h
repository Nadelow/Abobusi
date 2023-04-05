#pragma once
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <tuple>

class State {
public:
	std::string m_name;                //Имя состояния 
	size_t m_id;                       //Идентификатор состояния(для каждого имени уникальный)

	//Конструктор с параметрами по умолчанию. name_ - имя состояния, id_ - идентификатор
	State(const std::string& name_ = "", size_t id_ = 0) : m_name(name_),  m_id(id_){}

	//Оператор <. Сравниваются только id
	int friend operator<(const State& obj1, const State& obj2);

	//Оператор присваивания
	State& operator=(const State& obj);
};

class Symbolic_token {
public:
	std::string m_sym_name;          //имя лексемы
	int m_sym_value;                 //значение символьной лексемы
	size_t m_sym_id;                 //id лексемы

	//Конструктор с параметрами по умолчанию. name_ - имя, value_ - значение, id_ - идентификатор
	Symbolic_token(std::string name_ = "", int value_ = -1, size_t id_ = 0) : m_sym_name(name_), m_sym_value(value_), m_sym_id(id_){};

	//Оператор присваивания
	Symbolic_token& operator=(const Symbolic_token& obj);

	//Оператор <. Сравниваются только id
	int friend operator<(const Symbolic_token& obj1_, const Symbolic_token& obj2_) { return obj1_.m_sym_id < obj2_.m_sym_id; }
};

class Lexem {
public:
	size_t m_lex_id;                  //id лексемы
	std::string m_lex_class;          //класс лексемы

	//Конструктор с параметрами по умолчанию. lex_class - класс лексемы(std::string), id - идентификатор(уникальный для каждого класса)
	Lexem(std::string lex_class = "", size_t id = 0) : m_lex_class(lex_class), m_lex_id(id) {}

	//Оператор присваивания
	Lexem& operator=(const Lexem& obj);
	
	//Оператор <. Сравниваются только id
	int friend operator<(const Lexem& obj1, const Lexem& obj2);
};

class Determ_analizer
{
protected:
	State m_cur_state;                                                          //Текущее состояние
	Symbolic_token m_cur_sym;                                                   //Текущий символ
	std::fstream m_input_file;                                                  //Файл
	std::map<std::string, Symbolic_token> m_sym_lexems;                         //Коллекция символьных лексем
	std::map<std::string, State> m_states;                                      //Коллекция состояний

	//Виртуальная функция транслитерации. По входному символу определяет класс лексемы и присваивает ей значение.
	virtual Symbolic_token transliterator(int sym) = 0; 
	//Виртуальная функция заполнения коллекции символьных лексем.
	virtual void fill_sym_lexems() = 0;
	//Виртуальная функция заполнения коллекции состояний.
	virtual void fill_states() = 0;
	//Виртуальная функция заполнения таблицы переходов.
	virtual void fill_transition_table() = 0;
public:
	virtual void parse() = 0;  //Функция парсинга
};

