#pragma once
#include <string>

class Lexem {
public:
	size_t m_lex_id;                  //id лексемы
	std::string m_lex_class;          //класс лексемы

	//Конструктор с параметрами по умолчанию. lex_class - класс лексемы(std::string), id - идентификатор(уникальный для каждого класса)
	Lexem(std::string lex_class = "", size_t id = 0) : m_lex_class(lex_class), m_lex_id(id) {}

	//Конструктор копирования
	Lexem(const Lexem& obj);

	//Оператор присваивания
	Lexem& operator=(const Lexem& obj);

	//Оператор <. Сравниваются только id
	int friend operator<(const Lexem& obj1, const Lexem& obj2);

	bool friend operator==(const Lexem& obj1, const Lexem& obj2);
};
