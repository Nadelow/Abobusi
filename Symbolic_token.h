#pragma once
#include <string>
class Symbolic_token {
public:
	std::string m_sym_name;          //имя лексемы
	int m_sym_value;                 //значение символьной лексемы
	size_t m_sym_id;                 //id лексемы

	//Конструктор с параметрами по умолчанию. name_ - имя, value_ - значение, id_ - идентификатор.
	Symbolic_token(std::string name_ = "", int value_ = -1, size_t id_ = 1); 

	//Оператор присваивания.
	Symbolic_token& operator=(const Symbolic_token& obj);

	//Оператор <. Сравниваются только id.
	int friend operator<(const Symbolic_token& obj1_, const Symbolic_token& obj2_);

	//Оператор сравнения. Сравниваются только id.
	int friend operator==(const Symbolic_token& obj1_, const Symbolic_token& obj2_);
};
