#pragma once
#include "determ_analizer.h"
#include "Lexem.h"
#include <list>
#include <vector>
#include <sstream>


class lex_block_basic : public Determ_analizer
{
protected:
	struct variable 
	{
		std::string var_name;
		double var_value = 0;
	};
	
	typedef State(lex_block_basic::* lex_func_ptr)();                    //Указатель на методы класса
	
	std::map<std::string, Lexem> m_lexems;                               //Коллекция лексем
	std::map<std::string, variable> m_name_table;                          //Таблица имён
	std::map<State, std::map<Symbolic_token, lex_func_ptr>> m_transition_table;
	std::list<std::tuple<Lexem, long long int, size_t>> m_lexem_list;    //Список лексем. Элементы -- кортежи лексема-значение-номер_строки
	std::map<char, int> m_begin_vector;

	std::vector<std::tuple<char, int, lex_func_ptr>> m_detect_table;        //Таблица обнаружения
	
	const size_t m_state_number = 23;
	const size_t m_symbols_number = 10;

	size_t m_reg_relation;                   //Регистр, хранящий первый  символ отношения
	size_t m_reg_counter;                    //Регистр счётчика
	size_t m_reg_line_num = 1;               //Регистр текущего номера строки
	size_t m_reg_detection = 0;              //Регистр таблицы обнаружения
	size_t m_reg_value;                      //Регистр значения
	
	Lexem m_reg_class;                       //Регистр класса лексемы
	
	long long int m_reg_name_table_pointer;  //Регистр указателя на таблицу имён, является значением PUSH и POP
	long long int m_reg_order;               //Регистр порядка
	
	double m_reg_number;                     //Регистр числа
	
	int m_reg_sign;                          //Регистр знака
	
	std::string m_reg_var_name;              //Регистр имени переменной
	
	Symbolic_token m_curr_sym;               //Текущий символ
	
	State m_curr_state = State("A1", 0);                      //Текущее состояние

public:
	//Конструктор, принимающий имя файла для анализа в качестве аргумента
	lex_block_basic(std::string filename);

	~lex_block_basic() { m_input_file.close(); }

	void print_lexem_list();
private:
	//Функция парсинга, формирующая список лексем на основе входного файла
	virtual void parse() override;

	//Определяет, к какому классу принадлежит входной символ. В соответствие с этим присваивает id и значение.
	Symbolic_token transliterator(int sym) override;

	//Функция заполняет коллекцию состояний.
	void fill_states() override;

	//Функция заполняет коллекцию символьных лексем.
	void fill_sym_lexems() override;

	//Заполнение таблицы переходов
	void fill_transition_table() override;

	//Заполнение коллекции лексем
	void fill_lexems();
	
	//Инициализация начального вектора
	void init_begin_vect();

	//Инициализация таблицы обнаружения
	void init_detect_table();

	//Добавление константы в таблицу имён
	void add_constant();

	//Добавление переменной в таблицу имён
	void add_variable();

	//Создание лексемы
	void create_lexem();

	void calc_constant();

	std::string relation_table(long long int sym);

	///////////////////////////////////////Procedures////////////////////////////////////////////////
	void DA1E();
	void DA1D();
	void DA2D();
	void DA3D();

	State ERROR1();

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

