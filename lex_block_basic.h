#pragma once
#include "determ_analizer.h"
#include "Lexem.h"
#include <list>
#include <map>
#include <tuple>
#include <vector>
#include <sstream>


class Lex_block : public Determ_analizer
{
protected:
	/*
	/ ��������� ��� ������������� ���������
	/ @param op_name - ��� ��������
	/ @param op_value - �������� ��������
	*/
	struct operand
	{
		std::string op_name;
		double op_value = 0;
	};

	typedef State(Lex_block::* lex_func_ptr)();                                   // ��������� �� ������ ������

	std::map<size_t, long long int> m_lines;                                      // ��������� ���������� �� ������� �����
	std::map<std::string, Lexem> m_lexems;                                        // ��������� ������
	std::map<std::string, operand> m_name_table;                                  // ������� ���
	std::map<State, std::map<Symbolic_token, lex_func_ptr>> m_transition_table;   // ������� ��������� ��������. ��� ������ ��������� m_transition_table[State][Symbolic_token] ����� ���������� ��������� ��������
	std::map<char, int> m_begin_vector;                                           // ��������� ������ ��� ����������� �������� ����

	std::list<std::tuple<Lexem, long long int, size_t>> m_lexem_list;             // ������ ������. �������� -- ������� �������-��������-�����_������

	std::vector<std::tuple<char, int, lex_func_ptr>> m_detect_table;              //������� �����������

	size_t m_reg_relation;                   //�������, �������� ������  ������ ���������
	size_t m_reg_counter;                    //������� ��������
	size_t m_reg_line_num = 1;               //������� �������� ������ ������
	size_t m_reg_detection = 0;              //������� ������� ������� �����������
	size_t m_reg_value;                      //������� ��������

	Lexem m_reg_class;                       //������� ������ �������

	long long int m_reg_pointer;             //������� ���������, ����������� � ���� long long
	long long int m_reg_order;               //������� �������

	double m_reg_number;                     //������� �����

	int m_reg_sign;                          //������� �����

	std::string m_reg_var_name;              //������� ����� ����������

	Symbolic_token m_curr_sym;               //������� ������

	State m_curr_state = State("A1", 0);     //������� ���������

public:
	//���������� ���� ������
	std::map<std::string, Lexem> get_lexems();

	//���������� ������ ������
	std::list<std::tuple<Lexem, long long int, size_t>> get_lexem_list();

	//�����������, ����������� ��� ����� ��� ������� � �������� ���������
	Lex_block(std::string filename);

	~Lex_block() { m_input_file.close(); }

	void print_lexem_list();
private:	

	//������� ��������, ����������� ������ ������ �� ������ �������� �����
	virtual void parse() override;

	//����������, � ������ ������ ����������� ������� ������. � ������������ � ���� ����������� id � ��������.
	Symbolic_token transliterator(int sym) override;

	//������� ��������� ��������� ���������.
	void fill_states() override;

	//������� ��������� ��������� ���������� ������.
	void fill_sym_lexems() override;

	//���������� ������� ���������
	void fill_transition_table() override;

	//���������� ��������� ������
	void fill_lexems();

	//������������� ���������� �������
	void init_begin_vect();

	//������������� ������� �����������
	void init_detect_table();

	//���������� ��������� � ������� ���
	void add_constant();

	//���������� ���������� � ������� ���
	void add_variable();

	//�������� �������
	void create_lexem();

	//������� �������� ���������, ��������� �������� �����, �������, ����� � ��������
	void calc_constant();

	//�� ������ ��������� ����� ������, �������������� ���. ����� ��� ������ ������
	std::string relation_table(long long int sym);

	///////////////////////////////////////Procedures////////////////////////////////////////////////
	State ERROR1();

	void DA1E();
	void DA1D();
	void DA2D();
	void DA3D();

	State A1();
	State A1a();
	State A1b();
	State A1c();
	State A1d();
	State A1e();

	State A2();
	State A2a();
	State A2b();
	State A2c();
	State A2d();
	State A2e();
	State A2f();
	State A2g();
	State A2h();
	State A2j();
	State A2k();
	State A2m();
	State A2n();
	State A2o();
	State A2p();
	State A2q();
	State A2r();
	State A2s();
	State A2t();
	State A2u();
	State A2l();

	State A3();
	State A3a();
	State A3b();
	State A3c();
	State A3d();
	State A3e();
	State A3f();
	State A3g();

	State B1();
	State B1a();
	State B1b();
	State B1c();
	State B1d();
	State B1e();

	State C1();
	State C1a();

	State C2();
	State C2a();
	State C2b();
	State C2d();

	State D1();
	State D1a();
	State D1b();
	State D1c();

	State D2();
	State D2a();
	State D2b();
	State D2c();

	State D3();
	State D3a();

	State D4();
	State D4a();

	State D5();
	State D5a();
	State D5b();
	State D5c();


	State D6();
	State D6a();

	State E1();
	State E1a();
	State E1b();

	State E2();
	State E2a();
	State E2b();
	State E2c();

	State F1();
	State F1a();
	State F1b();

	State F2();
	State F2a();

	State F3();
	State F3a();

	State G1();
	State G1a();

	State H1();
	State H1a();
	State H1b();
	State H1c();
	State H1d();
	State H1e();
	State H1f();

	State M1();
	State M2();
	State M3();

	State EXIT1();
	State EXIT2();
	State EXIT3();
	State EXIT4();
	State EXIT5();
	State EXIT6();

};
