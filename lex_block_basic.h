#pragma once
#include "determ_analizer.h"
#include <list>
#include <vector>
class lex_block_basic : public determ_analizer
{
protected:
	std::map<std::string, Lexem> m_lexems;                               //��������� ������
	std::map<std::string, double> m_name_table;                          //������� ���
	std::list<std::tuple<Lexem, long long int, size_t>> m_lexem_list;    //������ ������. �������� -- ������� �������-��������-�����_������
	std::vector<std::tuple<char, int, funct_ptr>> m_detect_table;        //������� �����������

	const size_t m_state_number = 23;
	const size_t m_symbols_number = 10;

	Lexem m_reg_class;               //������� ������ �������
	long long int m_reg_name_table_pointer;  //������� ��������� �� ������� ���, �������� ��������� PUSH � POP
	size_t m_reg_relation;           //�������, �������� ������  ������ ���������
	double m_reg_number;             //������� �����
	size_t m_reg_order;              //������� �������
	size_t m_reg_counter;            //������� ��������
	size_t m_reg_is_negative;        //����
	size_t m_reg_line_num = 1;       //������� �������� ������ ������
	std::string m_reg_var_name;      //������� ����� ����������
	size_t m_reg_detection = 0;      //������� ������� �����������
	size_t m_reg_value;              //������� ��������

	std::map<char, int> m_begin_vector;
public:
	lex_block_basic(std::string filename);
	virtual void parse() override;
	~lex_block_basic() { m_input_file.close(); }
private:
	Symbolic_token transliterator(int sym) override;

	void fill_states() override;

	void fill_sym_lexems() override;

	void fill_transition_table() override;

	void fill_lexems();
	
	void init_begin_vect();

	void init_detect_table();

	void add_constant();

	void add_variable();

	void create_lexem();

	///////////////////////////////////////Procedures////////////////////////////////////////////////
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
	State A2j();
	State A2k();
	State A2l();
	State A2m();
	State A2n();
	State A2o();
	State A2p();
	State A2q();
	State A2r();
	State A2s();
	State A2t();
	State A2u();

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
	State C2c();
	State C2d();
	
	State D1();
	State D1a();
	State D1b();
	
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

