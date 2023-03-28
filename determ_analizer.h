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
	std::string m_s_lex_name;          //��� �������
	int m_s_lex_value;                 //�������� ���������� �������
	size_t m_s_lex_id;                 //id �������
	Symbolic_token& operator=(const std::string& str) { m_s_lex_name = str; return *this; };
};

class Lexem {
public:
	size_t m_lex_id;                  //id �������
	std::string m_lex_class;          //����� �������
	Lexem& operator=(const std::string str) { m_lex_class = str; return *this; }
};

//���������� ��������� �� �������. ������������ �������� State - ���������, � ������� ������� ��������� ����� ���������� ���������
typedef State(*funct_ptr)();


class determ_analizer
{
protected:
	std::map<State, std::map<Symbolic_token, funct_ptr>> m_transition_table; //������� ��������� �����������
	State m_cur_state; //������� ���������
	Symbolic_token m_cur_sym; //������� ������
	std::fstream m_input_file; //����
	std::map<std::string, Symbolic_token> m_sym_lexems; //��������� ���������� ������
	std::map<std::string, State> m_states;

	virtual Symbolic_token transliterator(int sym) = 0; //����������� ������� ��������������. �� �������� ������� ���������� ����� ������� � ����������� �� ��������.
public:
	virtual void fill_sym_lexems() = 0;
	virtual void fill_states() = 0;
	virtual void fill_transition_table() = 0;
	determ_analizer(std::string filename); //����������� ��������� �����������
	virtual void parse() = 0;  //������� ��������
};

