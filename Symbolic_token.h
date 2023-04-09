#pragma once
#include <string>
class Symbolic_token {
public:
	std::string m_sym_name;          //имя лексемы
	int m_sym_value;                 //значение символьной лексемы
	size_t m_sym_id;                 //id лексемы

	/*
	@brief Конструктор с параметрами по умолчанию.
	@param name_ - имя, 
	@param value_ - значение, 
	@param id_ - идентификатор.
	*/
	Symbolic_token(std::string name_ = "", int value_ = -1, size_t id_ = 1); 

	//Оператор присваивания.
	Symbolic_token& operator=(const Symbolic_token& obj_);

	//Оператор <. Сравниваются только id.
	int friend operator<(const Symbolic_token& obj1_, const Symbolic_token& obj2_);

	//Оператор сравнения. Сравниваются только id.
	int friend operator==(const Symbolic_token& obj1_, const Symbolic_token& obj2_);
};
