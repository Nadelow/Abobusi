#pragma once
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <tuple>

class State {
public:
	std::string m_name;                //��� ��������� 
	size_t m_id;                       //������������� ���������(��� ������� ����� ����������)

	//����������� � ����������� �� ���������. name_ - ��� ���������, id_ - �������������
	State(const std::string& name_ = "", size_t id_ = 0) : m_name(name_),  m_id(id_){}

	//�������� <. ������������ ������ id
	int friend operator<(const State& obj1, const State& obj2);

	//�������� ������������
	State& operator=(const State& obj);
};

class Symbolic_token {
public:
	std::string m_sym_name;          //��� �������
	int m_sym_value;                 //�������� ���������� �������
	size_t m_sym_id;                 //id �������

	//����������� � ����������� �� ���������. name_ - ���, value_ - ��������, id_ - �������������
	Symbolic_token(std::string name_ = "", int value_ = -1, size_t id_ = 0) : m_sym_name(name_), m_sym_value(value_), m_sym_id(id_){};

	//�������� ������������
	Symbolic_token& operator=(const Symbolic_token& obj);

	//�������� <. ������������ ������ id
	int friend operator<(const Symbolic_token& obj1_, const Symbolic_token& obj2_) { return obj1_.m_sym_id < obj2_.m_sym_id; }
};

class Lexem {
public:
	size_t m_lex_id;                  //id �������
	std::string m_lex_class;          //����� �������

	//����������� � ����������� �� ���������. lex_class - ����� �������(std::string), id - �������������(���������� ��� ������� ������)
	Lexem(std::string lex_class = "", size_t id = 0) : m_lex_class(lex_class), m_lex_id(id) {}

	//�������� ������������
	Lexem& operator=(const Lexem& obj);
	
	//�������� <. ������������ ������ id
	int friend operator<(const Lexem& obj1, const Lexem& obj2);
};

class Determ_analizer
{
protected:
	State m_cur_state;                                                          //������� ���������
	Symbolic_token m_cur_sym;                                                   //������� ������
	std::fstream m_input_file;                                                  //����
	std::map<std::string, Symbolic_token> m_sym_lexems;                         //��������� ���������� ������
	std::map<std::string, State> m_states;                                      //��������� ���������

	//����������� ������� ��������������. �� �������� ������� ���������� ����� ������� � ����������� �� ��������.
	virtual Symbolic_token transliterator(int sym) = 0; 
	//����������� ������� ���������� ��������� ���������� ������.
	virtual void fill_sym_lexems() = 0;
	//����������� ������� ���������� ��������� ���������.
	virtual void fill_states() = 0;
	//����������� ������� ���������� ������� ���������.
	virtual void fill_transition_table() = 0;
public:
	virtual void parse() = 0;  //������� ��������
};

