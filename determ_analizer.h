#pragma once
#include "State.h"
#include "Symbolic_token.h"
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <tuple>

class Determ_analizer
{
protected:
	//State m_cur_state = State("A1", 0);                                         //Текущее состояние
	//Symbolic_token m_cur_sym;                                                   //Текущий символ
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

