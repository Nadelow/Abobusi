#include "lex_block_basic.h"

lex_block_basic::lex_block_basic(std::string filename)
{
	m_input_file = std::fstream(filename);
	if (!m_input_file)
	{
		std::cout << "In <lex_block_basic(std::string filename)> Couldn't open a file " << filename << std::endl;
		return;
	}

	fill_states();
	fill_sym_lexems();
	fill_transition_table();
	init_begin_vect();
	init_detect_table();
}

void lex_block_basic::parse()
{
}

Symbolic_token lex_block_basic::transliterator(int sym)
{
	Symbolic_token s_token;
	s_token.m_s_lex_value = 0;
	if (isdigit(sym))
	{
		s_token = Symbolic_token("s_l_digit", sym - '0', 0);
	}
	else if ((sym > 64 && sym < 91) || (sym > 96 && sym < 123))
	{
		s_token = Symbolic_token("s_l_letter", sym, 1);
	}
	else if (sym == '+' || sym == '-' || sym == '*' || sym == '/' || sym == '%')
	{
		s_token = Symbolic_token("s_l_aur_op", sym, 2);
	}
	else if (sym == '=' || sym == '!' || sym == '<' || sym == '>')
	{
		s_token = Symbolic_token("s_l_relat", sym, 3);
	}
	else if (sym == ' ' || sym == '\t')
	{
		s_token = Symbolic_token("s_l_space", -1, 4);
	}
	else if (sym == '\n')
	{
		s_token = Symbolic_token("s_l_lf", -1, 5);
	}
	else if (sym == EOF)
	{
		s_token = Symbolic_token("s_l_eof", -1, 6);
	}
	else if (sym == '(')
	{
		s_token = Symbolic_token("s_l_open_brace", sym, 7);
	}
	else if (sym == ')')
	{
		s_token = Symbolic_token("s_l_close_brace", sym, 8);
	}
	else if (sym == '.')
	{
		s_token = Symbolic_token("s_l_dot", sym, 9);
	}
	else
	{
		s_token = Symbolic_token("s_l_error", -1, 10);
	}
	return s_token;
}

void lex_block_basic::fill_states()
{
	m_states["A1"] = State("A1", 1);
	m_states["A2"] = State("A2", 2);
	m_states["A3"] = State("A3", 3);
	m_states["B1"] = State("B1", 4);
	m_states["C1"] = State("C1", 5);
	m_states["C2"] = State("C2", 6);
	m_states["D1"] = State("D1", 7);
	m_states["D2"] = State("D2", 8);
	m_states["D3"] = State("D3", 9);
	m_states["D4"] = State("D4", 10);
	m_states["D5"] = State("D5", 11);
	m_states["D6"] = State("D6", 12);
	m_states["E1"] = State("E1", 13);
	m_states["E2"] = State("E2", 14);
	m_states["F1"] = State("F1", 15);
	m_states["F2"] = State("F2", 16);
	m_states["F3"] = State("F3", 17);
	m_states["G1"] = State("G1", 18);
	m_states["H1"] = State("H1", 19);
	m_states["STOP"] = State("STOP", 0);
}

void lex_block_basic::fill_sym_lexems()
{
	m_sym_lexems["s_l_digit"] = Symbolic_token("s_l_digit", 0);
	m_sym_lexems["s_l_letter"] = Symbolic_token("s_l_letter", 1);
	m_sym_lexems["s_l_aur_op"] = Symbolic_token("s_l_aur_op",2);
	m_sym_lexems["s_l_relat"] = Symbolic_token("s_l_relat",3);
	m_sym_lexems["s_l_space"] = Symbolic_token("s_l_space",4);
	m_sym_lexems["s_l_lf"] = Symbolic_token("s_l_lf",5);
	m_sym_lexems["s_l_eof"] = Symbolic_token("s_l_eof",6);
	m_sym_lexems["s_l_open_brace"] = Symbolic_token("s_l_open_brace",7);
	m_sym_lexems["s_l_close_brace"] = Symbolic_token("s_l_close_brace",8);
	m_sym_lexems["s_l_dot"] = Symbolic_token("s_l_dot",9);
	m_sym_lexems["s_l_error"] = Symbolic_token("s_l_error",10);
}

void lex_block_basic::fill_lexems()
{
	m_lexems["lex_line"] = Lexem("lex_line", 0);
	m_lexems["lex_operand"] = Lexem("lex_operand", 1);
	m_lexems["lex_aur_oper"] = Lexem("lex_aur_oper", 2);
	m_lexems["lex_relation"] = Lexem("lex_relation", 3);
	m_lexems["lex_next"] = Lexem("lex_next", 4);
	m_lexems["lex_let"] = Lexem("lex_let", 5);
	m_lexems["lex_for"] = Lexem("lex_for", 6);
	m_lexems["lex_goto"] = Lexem("lex_goto", 7);
	m_lexems["lex_gosub"] = Lexem("lex_gosub", 8);
	m_lexems["lex_op_brace"] = Lexem("lex_op_brace", 9);
	m_lexems["lex_cls_brace"] = Lexem("lex_cls_brace", 10);
	m_lexems["lex_if"] = Lexem("lex_if", 11);
	m_lexems["lex_return"] = Lexem("lex_return", 12);
	m_lexems["lex_end"] = Lexem("lex_end", 13);
	m_lexems["lex_to"] = Lexem("lex_to", 14);
	m_lexems["lex_step"] = Lexem("lex_step", 15);
	m_lexems["lex_rem"] = Lexem("lex_rem", 16);
	m_lexems["lex_error"] = Lexem("lex_error", 17);
	m_lexems["lex_eof"] = Lexem("lex_eof", 18);
}

void lex_block_basic::init_begin_vect()
{
	m_begin_vector['e'] = 1;
	m_begin_vector['f'] = 3;
	m_begin_vector['g'] = 5;
	m_begin_vector['i'] = 11;
	m_begin_vector['l'] = 12;
	m_begin_vector['n'] = 14;
	m_begin_vector['r'] = 17;
	m_begin_vector['s'] = 23;
	m_begin_vector['t'] = 26;
}

void lex_block_basic::init_detect_table()
{ 
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('c', 0, &lex_block_basic::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('n', 0, &lex_block_basic::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('d', 0, &lex_block_basic::A2q));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('o', 0, &lex_block_basic::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('r', 0, &lex_block_basic::F1b));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('o', 0, &lex_block_basic::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('t', 7, &lex_block_basic::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('o', 0, &lex_block_basic::E1a));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('s', 0, &lex_block_basic::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('u', 0, &lex_block_basic::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('b', 0, &lex_block_basic::E1b));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('f', 0, &lex_block_basic::A2r));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('e', 0, &lex_block_basic::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('t', 0, &lex_block_basic::F1a));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('e', 0, &lex_block_basic::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('x', 0, &lex_block_basic::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('t', 0, &lex_block_basic::C1a));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('e', 0, &lex_block_basic::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('t', 21, &lex_block_basic::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('u', 0, &lex_block_basic::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('r', 0, &lex_block_basic::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('n', 0, &lex_block_basic::A2s));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('m', 0, &lex_block_basic::G1a));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('t', 0, &lex_block_basic::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('e', 0, &lex_block_basic::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('p', 0, &lex_block_basic::A2t));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('o', 0, &lex_block_basic::A2u));
}

void lex_block_basic::add_constant()
{
	std::string const_name = std::to_string(m_reg_number);
	m_name_table.emplace(std::pair<std::string, double>(const_name, m_reg_number));
	m_reg_name_table_pointer = reinterpret_cast<long long int>(&m_name_table[const_name]);
}

void lex_block_basic::add_variable()
{
	m_name_table.emplace(std::pair<std::string, double>(m_reg_var_name, m_reg_number));
	m_reg_name_table_pointer = reinterpret_cast<long long int>(&m_name_table[m_reg_var_name]);
}

void lex_block_basic::create_lexem()
{
	if (m_reg_class.m_lex_id == 0 /*line*/ || m_reg_class.m_lex_id == 2 /*aur oper*/)
	{
		std::tuple<Lexem, long long int, int> tup(m_reg_class, m_reg_value, m_reg_line_num);
		m_lexem_list.push_back(tup);
	}
	else if (m_reg_class.m_lex_id == 1 /*operand*/ || m_reg_class.m_lex_id == 4 /*next*/ || \
		m_reg_class.m_lex_id == 5 /*let*/ || m_reg_class.m_lex_id == 6 /*for*/ || m_reg_class.m_lex_id == 7 /*goto*/ || \
		m_reg_class.m_lex_id == 8 /*gosub*/)
	{
		std::tuple<Lexem, long long int, int> tup(m_reg_class, m_reg_name_table_pointer, m_reg_line_num);
		m_lexem_list.push_back(tup);
	}
	else if (m_reg_class.m_lex_id == 3 /*relation*/)
	{
		std::tuple<Lexem, long long int, int> tup(m_reg_class, m_reg_relation, m_reg_line_num);
		m_lexem_list.push_back(tup);
	}
	else if (m_reg_class.m_lex_id == 9 /*open_br*/ || m_reg_class.m_lex_id == 10 /*close_br*/ || \
		m_reg_class.m_lex_id == 11 /*if*/ || m_reg_class.m_lex_id == 12 /*return*/ || m_reg_class.m_lex_id == 13 /*end*/ || \
		m_reg_class.m_lex_id == 14 /*to*/ || m_reg_class.m_lex_id == 15 /*step*/ || m_reg_class.m_lex_id == 16 /*rem*/ || \
		m_reg_class.m_lex_id == 17 /*error*/ || m_reg_class.m_lex_id == 18 /*eof*/)
	{
		std::tuple<Lexem, long long int, int> tup(m_reg_class, -1, m_reg_line_num);
		m_lexem_list.push_back(tup);
	}
}

void lex_block_basic::fill_transition_table()
{
	(m_transition_table[m_states["A1"]])[m_sym_lexems["s_l_digit"]] = &lex_block_basic::E2a;
	(m_transition_table[m_states["A1"]])[m_sym_lexems["s_l_space"]] = &lex_block_basic::A1;
	(m_transition_table[m_states["A1"]])[m_sym_lexems["s_l_lf"]] = &lex_block_basic::A1;
	(m_transition_table[m_states["A1"]])[m_sym_lexems["s_l_eof"]] = &lex_block_basic::EXIT1;

	(m_transition_table[m_states["A2"]])[m_sym_lexems["s_l_letter"]] = &lex_block_basic::C2a;
	(m_transition_table[m_states["A2"]])[m_sym_lexems["s_l_digit"]] = &lex_block_basic::D1a;
	(m_transition_table[m_states["A2"]])[m_sym_lexems["s_l_aur_op"]] = &lex_block_basic::A2a;
	(m_transition_table[m_states["A2"]])[m_sym_lexems["s_l_relat"]] = &lex_block_basic::H1a;
	(m_transition_table[m_states["A2"]])[m_sym_lexems["s_l_op_brace"]] = &lex_block_basic::A2h;
	(m_transition_table[m_states["A2"]])[m_sym_lexems["s_l_cls_brace"]] = &lex_block_basic::A3b;
	(m_transition_table[m_states["A2"]])[m_sym_lexems["s_l_dot"]] = &lex_block_basic::D6;
	(m_transition_table[m_states["A2"]])[m_sym_lexems["s_l_space"]] = &lex_block_basic::A2;
	(m_transition_table[m_states["A2"]])[m_sym_lexems["s_l_lf"]] = &lex_block_basic::A1;
	(m_transition_table[m_states["A2"]])[m_sym_lexems["s_l_eof"]] = &lex_block_basic::EXIT1;

	(m_transition_table[m_states["A3"]])[m_sym_lexems["s_l_letter"]] = &lex_block_basic::B1a;
	(m_transition_table[m_states["A3"]])[m_sym_lexems["s_l_digit"]] = &lex_block_basic::D1a;
	(m_transition_table[m_states["A3"]])[m_sym_lexems["s_l_aur_op"]] = &lex_block_basic::A2a;
	(m_transition_table[m_states["A3"]])[m_sym_lexems["s_l_relat"]] = &lex_block_basic::H1a;
	(m_transition_table[m_states["A3"]])[m_sym_lexems["s_l_op_brace"]] = &lex_block_basic::A2h;
	(m_transition_table[m_states["A3"]])[m_sym_lexems["s_l_cls_brace"]] = &lex_block_basic::A3b;
	(m_transition_table[m_states["A3"]])[m_sym_lexems["s_l_dot"]] = &lex_block_basic::D6;
	(m_transition_table[m_states["A3"]])[m_sym_lexems["s_l_space"]] = &lex_block_basic::A3;
	(m_transition_table[m_states["A3"]])[m_sym_lexems["s_l_lf"]] = &lex_block_basic::A1;
	(m_transition_table[m_states["A3"]])[m_sym_lexems["s_l_eof"]] = &lex_block_basic::EXIT1;

	(m_transition_table[m_states["B1"]])[m_sym_lexems["s_l_letter"]] = &lex_block_basic::M1;
	(m_transition_table[m_states["B1"]])[m_sym_lexems["s_l_space"]] = &lex_block_basic::B1;

	(m_transition_table[m_states["C1"]])[m_sym_lexems["s_l_letter"]] = &lex_block_basic::C2d;
	(m_transition_table[m_states["C1"]])[m_sym_lexems["s_l_space"]] = &lex_block_basic::C1;

	(m_transition_table[m_states["C2"]])[m_sym_lexems["s_l_letter"]] = &lex_block_basic::B1b;
	(m_transition_table[m_states["C2"]])[m_sym_lexems["s_l_digit"]] = &lex_block_basic::A3a;
	(m_transition_table[m_states["C2"]])[m_sym_lexems["s_l_aur_op"]] = &lex_block_basic::A2g;
	(m_transition_table[m_states["C2"]])[m_sym_lexems["s_l_relat"]] = &lex_block_basic::H1b;
	(m_transition_table[m_states["C2"]])[m_sym_lexems["s_l_op_brace"]] = &lex_block_basic::A2k;
	(m_transition_table[m_states["C2"]])[m_sym_lexems["s_l_cls_brace"]] = &lex_block_basic::A3c;
	(m_transition_table[m_states["C2"]])[m_sym_lexems["s_l_space"]] = &lex_block_basic::C2;
	(m_transition_table[m_states["C2"]])[m_sym_lexems["s_l_lf"]] = &lex_block_basic::A1a;
	(m_transition_table[m_states["C2"]])[m_sym_lexems["s_l_eof"]] = &lex_block_basic::EXIT2;

	(m_transition_table[m_states["D1"]])[m_sym_lexems["s_l_letter"]] = &lex_block_basic::M2;
	(m_transition_table[m_states["D1"]])[m_sym_lexems["s_l_digit"]] = &lex_block_basic::D1b;
	(m_transition_table[m_states["D1"]])[m_sym_lexems["s_l_aur_op"]] = &lex_block_basic::A2c;
	(m_transition_table[m_states["D1"]])[m_sym_lexems["s_l_relat"]] = &lex_block_basic::H1c;
	(m_transition_table[m_states["D1"]])[m_sym_lexems["s_l_op_brace"]] = &lex_block_basic::A2i;
	(m_transition_table[m_states["D1"]])[m_sym_lexems["s_l_cls_brace"]] = &lex_block_basic::A3d;
	(m_transition_table[m_states["D1"]])[m_sym_lexems["s_l_dot"]] = &lex_block_basic::D2c;
	(m_transition_table[m_states["D1"]])[m_sym_lexems["s_l_space"]] = &lex_block_basic::D1;
	(m_transition_table[m_states["D1"]])[m_sym_lexems["s_l_lf"]] = &lex_block_basic::A1b;
	(m_transition_table[m_states["D1"]])[m_sym_lexems["s_l_eof"]] = &lex_block_basic::EXIT3;

	(m_transition_table[m_states["D2"]])[m_sym_lexems["s_l_letter"]] = &lex_block_basic::M3;
	(m_transition_table[m_states["D2"]])[m_sym_lexems["s_l_digit"]] = &lex_block_basic::D2a;
	(m_transition_table[m_states["D2"]])[m_sym_lexems["s_l_aur_op"]] = &lex_block_basic::A2d;
	(m_transition_table[m_states["D2"]])[m_sym_lexems["s_l_relat"]] = &lex_block_basic::H1d;
	(m_transition_table[m_states["D2"]])[m_sym_lexems["s_l_op_brace"]] = &lex_block_basic::A2m;
	(m_transition_table[m_states["D2"]])[m_sym_lexems["s_l_cls_brace"]] = &lex_block_basic::A3e;
	(m_transition_table[m_states["D2"]])[m_sym_lexems["s_l_space"]] = &lex_block_basic::D2;
	(m_transition_table[m_states["D2"]])[m_sym_lexems["s_l_lf"]] = &lex_block_basic::A1c;
	(m_transition_table[m_states["D2"]])[m_sym_lexems["s_l_eof"]] = &lex_block_basic::EXIT4;

	(m_transition_table[m_states["D3"]])[m_sym_lexems["s_l_digit"]] = &lex_block_basic::D5a;
	(m_transition_table[m_states["D3"]])[m_sym_lexems["s_l_aur_op"]] = &lex_block_basic::D4a;
	(m_transition_table[m_states["D3"]])[m_sym_lexems["s_l_space"]] = &lex_block_basic::D3;

	(m_transition_table[m_states["D4"]])[m_sym_lexems["s_l_digit"]] = &lex_block_basic::D5b;
	(m_transition_table[m_states["D4"]])[m_sym_lexems["s_l_space"]] = &lex_block_basic::D4;

	(m_transition_table[m_states["D5"]])[m_sym_lexems["s_l_letter"]] = &lex_block_basic::B1c;
	(m_transition_table[m_states["D5"]])[m_sym_lexems["s_l_digit"]] = &lex_block_basic::D5c;
	(m_transition_table[m_states["D5"]])[m_sym_lexems["s_l_aur_op"]] = &lex_block_basic::A2e;
	(m_transition_table[m_states["D5"]])[m_sym_lexems["s_l_relat"]] = &lex_block_basic::H1e;
	(m_transition_table[m_states["D5"]])[m_sym_lexems["s_l_op_brace"]] = &lex_block_basic::A2n;
	(m_transition_table[m_states["D5"]])[m_sym_lexems["s_l_cls_brace"]] = &lex_block_basic::A3f;
	(m_transition_table[m_states["D5"]])[m_sym_lexems["s_l_space"]] = &lex_block_basic::D5;
	(m_transition_table[m_states["D5"]])[m_sym_lexems["s_l_lf"]] = &lex_block_basic::A1d;
	(m_transition_table[m_states["D5"]])[m_sym_lexems["s_l_eof"]] = &lex_block_basic::EXIT5;

	(m_transition_table[m_states["D6"]])[m_sym_lexems["s_l_digit"]] = &lex_block_basic::D2b;
	(m_transition_table[m_states["D6"]])[m_sym_lexems["s_l_space"]] = &lex_block_basic::D6;

	(m_transition_table[m_states["E1"]])[m_sym_lexems["s_l_digit"]] = &lex_block_basic::E2b;
	(m_transition_table[m_states["E1"]])[m_sym_lexems["s_l_space"]] = &lex_block_basic::E1;

	(m_transition_table[m_states["E2"]])[m_sym_lexems["s_l_letter"]] = &lex_block_basic::B1e;
	(m_transition_table[m_states["E2"]])[m_sym_lexems["s_l_digit"]] = &lex_block_basic::E2c;
	(m_transition_table[m_states["E2"]])[m_sym_lexems["s_l_aur_op"]] = &lex_block_basic::A2f;
	(m_transition_table[m_states["E2"]])[m_sym_lexems["s_l_relat"]] = &lex_block_basic::H1f;
	(m_transition_table[m_states["E2"]])[m_sym_lexems["s_l_op_brace"]] = &lex_block_basic::A2j;
	(m_transition_table[m_states["E2"]])[m_sym_lexems["s_l_cls_brace"]] = &lex_block_basic::A3g;
	(m_transition_table[m_states["E2"]])[m_sym_lexems["s_l_space"]] = &lex_block_basic::E2;
	(m_transition_table[m_states["E2"]])[m_sym_lexems["s_l_lf"]] = &lex_block_basic::A1e;
	(m_transition_table[m_states["E2"]])[m_sym_lexems["s_l_eof"]] = &lex_block_basic::EXIT6;

	(m_transition_table[m_states["F1"]])[m_sym_lexems["s_l_letter"]] = &lex_block_basic::F2a;
	(m_transition_table[m_states["F1"]])[m_sym_lexems["s_l_space"]] = &lex_block_basic::F1;

	(m_transition_table[m_states["F2"]])[m_sym_lexems["s_l_digit"]] = &lex_block_basic::F3a;
	(m_transition_table[m_states["F2"]])[m_sym_lexems["s_l_relat"]] = &lex_block_basic::A2o;
	(m_transition_table[m_states["F2"]])[m_sym_lexems["s_l_space"]] = &lex_block_basic::F2;

	(m_transition_table[m_states["F3"]])[m_sym_lexems["s_l_relat"]] = &lex_block_basic::A2o;
	(m_transition_table[m_states["F3"]])[m_sym_lexems["s_l_space"]] = &lex_block_basic::F3;

	(m_transition_table[m_states["G1"]])[m_sym_lexems["s_l_letter"]] = &lex_block_basic::G1;
	(m_transition_table[m_states["G1"]])[m_sym_lexems["s_l_digit"]] = &lex_block_basic::G1;
	(m_transition_table[m_states["G1"]])[m_sym_lexems["s_l_aur_op"]] = &lex_block_basic::G1;
	(m_transition_table[m_states["G1"]])[m_sym_lexems["s_l_relat"]] = &lex_block_basic::G1;
	(m_transition_table[m_states["G1"]])[m_sym_lexems["s_l_op_brace"]] = &lex_block_basic::G1;
	(m_transition_table[m_states["G1"]])[m_sym_lexems["s_l_cls_brace"]] = &lex_block_basic::G1;
	(m_transition_table[m_states["G1"]])[m_sym_lexems["s_l_space"]] = &lex_block_basic::G1;
	(m_transition_table[m_states["G1"]])[m_sym_lexems["s_l_dot"]] = &lex_block_basic::G1;
	(m_transition_table[m_states["G1"]])[m_sym_lexems["s_l_lf"]] = &lex_block_basic::A1;
	(m_transition_table[m_states["G1"]])[m_sym_lexems["s_l_eof"]] = &lex_block_basic::EXIT1;

	(m_transition_table[m_states["H1"]])[m_sym_lexems["s_l_letter"]] = &lex_block_basic::C2b;
	(m_transition_table[m_states["H1"]])[m_sym_lexems["s_l_digit"]] = &lex_block_basic::D1c;
	(m_transition_table[m_states["H1"]])[m_sym_lexems["s_l_aur_op"]] = &lex_block_basic::A2g;
	(m_transition_table[m_states["H1"]])[m_sym_lexems["s_l_relat"]] = &lex_block_basic::A2p;
	(m_transition_table[m_states["H1"]])[m_sym_lexems["s_l_op_brace"]] = &lex_block_basic::A2k;
	(m_transition_table[m_states["H1"]])[m_sym_lexems["s_l_cls_brace"]] = &lex_block_basic::A3c;
	(m_transition_table[m_states["H1"]])[m_sym_lexems["s_l_dot"]] = &lex_block_basic::D6a;
	(m_transition_table[m_states["H1"]])[m_sym_lexems["s_l_space"]] = &lex_block_basic::H1;
	(m_transition_table[m_states["H1"]])[m_sym_lexems["s_l_lf"]] = &lex_block_basic::A1a;
	(m_transition_table[m_states["H1"]])[m_sym_lexems["s_l_eof"]] = &lex_block_basic::EXIT2;
}

State lex_block_basic::A1() { return m_states["A1"]; }

State lex_block_basic::A2() { return m_states["A2"]; }

State lex_block_basic::A3() { return m_states["A3"]; }

State lex_block_basic::B1() { return m_states["B1"]; }

State lex_block_basic::C1() { return m_states["C1"]; }

State lex_block_basic::C2() { return m_states["C2"]; }

State lex_block_basic::D1() { return m_states["D1"]; }

State lex_block_basic::D2() { return m_states["D2"]; }

State lex_block_basic::D3() { return m_states["D3"]; }

State lex_block_basic::D4() { return m_states["D4"]; }

State lex_block_basic::D5() { return m_states["D5"]; }

State lex_block_basic::D6() { return m_states["D6"]; }

State lex_block_basic::E1() { return m_states["E1"]; }

State lex_block_basic::E2() { return m_states["E2"]; }

State lex_block_basic::F1() { return m_states["F1"]; }

State lex_block_basic::F2() { return m_states["F2"]; }

State lex_block_basic::F3() { return m_states["F3"]; }

State lex_block_basic::G1() { return m_states["G1"]; }

State lex_block_basic::H1() { return m_states["H1"]; }

State lex_block_basic::EXIT2()
{
	create_lexem();
	return EXIT1();
}

State lex_block_basic::EXIT1()
{
	m_reg_class = m_lexems["lex_eof"];
	create_lexem();
	return m_states["STOP"];
}

State lex_block_basic::EXIT3()
{
	DA1D();
	return EXIT2();
}
State lex_block_basic::EXIT4()
{
	DA2D();
	return EXIT2();
}
State lex_block_basic::EXIT5()
{
	DA3D();
	return EXIT2();
}
State lex_block_basic::EXIT6()
{
	DA1E();
	return EXIT2();
}

void lex_block_basic::DA1E()
{
	m_reg_value = m_reg_line_num;
}

void lex_block_basic::DA1D()
{
	m_reg_order -= m_reg_counter;
	calc_constant();
}

void lex_block_basic::DA2D()
{
	m_reg_order -= m_reg_counter;
	m_reg_class = m_lexems["lex_operand"];
	calc_constant();
}

void lex_block_basic::DA3D()
{
	if (m_reg_sign == -1)
	{
		m_reg_order *= -1;
	}
	m_reg_order -= m_reg_counter;
	calc_constant();
}

State lex_block_basic::A1a()
{
	if (m_reg_var_name != "")
		add_variable();
	create_lexem();
	if (m_curr_sym.m_s_lex_id == 3 && m_curr_sym.m_s_lex_value == '=')
		m_reg_relation = 1;
	else if (m_curr_sym.m_s_lex_value == '<')
		m_reg_relation = 3;
	else if (m_curr_sym.m_s_lex_value == '>')
		m_reg_relation = 4;
	return A1();
}

State lex_block_basic::A1b()
{
	DA1D();
	return A1a();
}

State lex_block_basic::A1c()
{
	DA2D();
	return A1a();
}

State lex_block_basic::A1d()
{
	DA3D();
	return A1a();
}

State lex_block_basic::A1e()
{
	DA1E();
	return A1a();
}

State lex_block_basic::A2a()
{
	m_reg_class = m_lexems["lex_aur_oper"];
	m_reg_value = m_curr_sym.m_s_lex_value;
	return A2b();
}

State lex_block_basic::A2b()
{
	create_lexem();
	return A2();
}

State lex_block_basic::A2c()
{
	DA1D();
	return A2g();
}

State lex_block_basic::A2d()
{
	DA2D();
	return A2g();
}

State lex_block_basic::A2e()
{
	DA3D();
	return A2g();
}

State lex_block_basic::A2f()
{
	DA1E();
	return A2g();
}

State lex_block_basic::A2j()
{
	DA1E();
	return A2k();
}

State lex_block_basic::A2h()
{
	m_reg_class = m_lexems["lex_op_brace"];
	create_lexem();
	return A2();
}

State lex_block_basic::A2i()
{
	DA1D();
	return A2k();
}

State lex_block_basic::A2m()
{
	DA2D();
	return A2k();
}

State lex_block_basic::A2n()
{
	DA3D();
	return A2k();
}

State lex_block_basic::A2o()
{
	if (m_curr_sym.m_s_lex_value != '=')
	{
		return ERROR1();
	}
	else
	{
		add_variable();
		return A2b();
	}

}

State lex_block_basic::A2q()
{
	m_reg_class = m_lexems["lex_end"];
	return A2b();
}

State lex_block_basic::A2r()
{
	m_reg_class = m_lexems["lex_if"];
	return A2b();
}

State lex_block_basic::A2s()
{
	m_reg_class = m_lexems["lex_return"];
	return A2b();
}

State lex_block_basic::A2g()
{
	if (m_reg_var_name != "")
		add_variable();
	create_lexem();
	if (m_curr_sym.m_s_lex_value == '=')
		m_reg_relation = 1;
	else if (m_curr_sym.m_s_lex_value == '<')
		m_reg_relation = 3;
	else if (m_curr_sym.m_s_lex_value == '>')
		m_reg_relation = 4;
	return A2a();
}

State lex_block_basic::A2p()
{

	if (m_curr_sym.m_s_lex_value == '=')
	{
		if (m_reg_relation == '!')
		{
			m_reg_relation = 2;
		}
		else if (m_reg_relation == '<')
		{
			m_reg_relation = 5;
		}
		else if (m_reg_relation == '>')
		{
			m_reg_relation = 6;
		}
		else
		{
			return ERROR1();
		}
	}
	else// здесь после одного знака встретили не = а что угодно - это ошибка
	{
		return ERROR1();
	}
	return A2b();
}

State lex_block_basic::A2k()
{
	if (m_reg_var_name != "")
		add_variable();
	create_lexem();
	if (m_curr_sym.m_s_lex_value == '=')
		m_reg_relation = 1;
	else if (m_curr_sym.m_s_lex_value == '<')
		m_reg_relation = 3;
	else if (m_curr_sym.m_s_lex_value == '>')
		m_reg_relation = 4;
	return A2h();
}

State lex_block_basic::A2t()
{
	m_reg_class = m_lexems["lex_step"];
	return A2b();
}

State lex_block_basic::A2u()
{
	m_reg_class = m_lexems["lex_to"];
	return A2b();
}

State lex_block_basic::A3a()
{
	m_reg_var_name += m_curr_sym.m_s_lex_value + '0';
	add_variable();
	create_lexem();
	return A3();
}

State lex_block_basic::A3c()
{

	if (m_reg_var_name != "")
		add_variable();
	create_lexem();
	if (m_curr_sym.m_s_lex_value == '=')
		m_reg_relation = 1;
	else if (m_curr_sym.m_s_lex_value == '<')
		m_reg_relation = 3;
	else if (m_curr_sym.m_s_lex_value == '>')
		m_reg_relation = 4;
	return A3b();
}

State lex_block_basic::A3d()
{
	DA1D();
	return A3c();
}

State lex_block_basic::A3b()
{
	m_reg_class = m_lexems["lex_cls_brace"];
	create_lexem();
	return A3();
}

State lex_block_basic::A3e()
{
	DA2D();
	return A3c();
}

State lex_block_basic::A3f()
{
	DA3D();
	return A3c();
}

State lex_block_basic::A3g()
{
	DA1E();
	return A3c();
}

State lex_block_basic::B1c()
{
	DA3D();
	return B1b();
}

State lex_block_basic::B1b()
{
	if (m_reg_var_name != "")
		add_variable();
	create_lexem();
	return B1a();
}

State lex_block_basic::B1a()
{
	m_reg_detection = m_begin_vector[m_curr_sym.m_s_lex_value];
	if (m_reg_detection == 0)
	{
		return ERROR1();
	}
	return B1();
}

State lex_block_basic::B1d()
{
	m_reg_detection++;
	return B1();
}

State lex_block_basic::B1e()
{
	DA1E();
	return B1b();
}

State lex_block_basic::C1a()
{
	m_reg_class = m_lexems["lex_next"];
	return C1();
}

State lex_block_basic::C2a()
{
	m_reg_class = m_lexems["lex_operand"];
	return C2d();
}

State lex_block_basic::C2d()
{
	m_reg_var_name = m_curr_sym.m_s_lex_value;

	return C2();
}


State lex_block_basic::C2b()
{
	create_lexem();
	return C2a();
}

State lex_block_basic::M1()
{
	if (m_reg_detection == 0)
		return ERROR1();
	if (m_curr_sym.m_s_lex_value == std::get<0>(m_detect_table[m_reg_detection]))
	{
		return (this->*std::get<2>(m_detect_table[m_reg_detection]))();
	}
	else
	{
		m_reg_detection = std::get<1>(m_detect_table[m_reg_detection]);
		if (m_reg_detection == 0)
			return ERROR1();
		return M1();
	}

}

State lex_block_basic::M2()
{
	if (m_curr_sym.m_s_lex_value != 'E')
	{
		DA1D();
		return B1b();
	}
	else
	{
		return D3a();
	}
}

State lex_block_basic::M3()
{
	if (m_curr_sym.m_s_lex_value != 'E')
	{
		DA1D();
		return B1b();
	}
	else
	{
		return D3();
	}

}

std::string lex_block_basic::relation_table(long long int ch)
{
	switch (ch)
	{
	case(1): {return "="; }
	case(2): {return "!="; }
	case(3): {return "<"; }
	case(4): {return ">"; }
	case(5): {return "<="; }
	case(6): {return ">="; }
	default:
		break;
	}
	return 0;
}
