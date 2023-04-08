#pragma once
#include <string>

class State {
public:
	std::string m_name;                //Имя состояния 
	size_t m_id;                       //Идентификатор состояния(для каждого имени уникальный)

	//Конструктор с параметрами по умолчанию. name_ - имя состояния, id_ - идентификатор
	State(const std::string& name_ = "", size_t id_ = 0) : m_name(name_), m_id(id_) {}

	//Оператор <. Сравниваются только id
	int friend operator<(const State& obj1, const State& obj2);

	//Оператор присваивания
	State& operator=(const State& obj);
};
