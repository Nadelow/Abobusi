#include "lex_block_basic.h"

/*
/ В конструкторе происходит заполнение всех таблиц, необходимых для работы автомата.
/ Также сразу же при создании лекс блока происходит парсинг входного файла
*/
Lex_block::Lex_block(std::string filename)
{
	m_input_file = std::fstream(filename);
	if (!m_input_file)
	{
		std::cout << "In <lex_block_basic(std::string filename)> Couldn't open a file " << filename << std::endl;
		return;
	}

	fill_states();
	fill_lexems();
	fill_sym_lexems();
	init_begin_vect();
	init_detect_table();
	fill_transition_table();

	parse();
}

Symbolic_token Lex_block::transliterator(int sym)
{
	Symbolic_token s_token;
	s_token.m_sym_value = 0;
	if (isdigit(sym))
	{
		s_token = Symbolic_token("sym_digit", sym - '0', 1);
	}
	else if (sym >= 'A' && sym <= 'Z')
	{
		s_token = Symbolic_token("sym_letter", sym, 0);
	}
	else if (sym == '+' || sym == '-' || sym == '*' || sym == '/' || sym == '^')
	{
		s_token = Symbolic_token("sym_aur_op", sym, 2);
	}
	else if (sym == '=' || sym == '!' || sym == '<' || sym == '>')
	{
		s_token = Symbolic_token("sym_relat", sym, 3);
	}
	else if (sym == ' ' || sym == '\t')
	{
		s_token = Symbolic_token("sym_space", -1, 7);
	}
	else if (sym == '\n')
	{
		s_token = Symbolic_token("sym_lf", -1, 8);
	}
	else if (sym == EOF)
	{
		s_token = Symbolic_token("sym_eof", -1, 9);
	}
	else if (sym == '(')
	{
		s_token = Symbolic_token("sym_lbrace", sym, 4);
	}
	else if (sym == ')')
	{
		s_token = Symbolic_token("sym_rbrace", sym, 5);
	}
	else if (sym == '.')
	{
		s_token = Symbolic_token("sym_dot", sym, 6);
	}
	else
	{
		s_token = Symbolic_token("sym_error", -1, 10);
	}
	return s_token;
}

void Lex_block::fill_states()
{
	m_states["A1"] = State("A1", 0);
	m_states["A2"] = State("A2", 1);
	m_states["A3"] = State("A3", 2);
	m_states["B1"] = State("B1", 3);
	m_states["C1"] = State("C1", 4);
	m_states["C2"] = State("C2", 5);
	m_states["D1"] = State("D1", 6);
	m_states["D2"] = State("D2", 7);
	m_states["D3"] = State("D3", 8);
	m_states["D4"] = State("D4", 9);
	m_states["D5"] = State("D5", 10);
	m_states["D6"] = State("D6", 11);
	m_states["E1"] = State("E1", 12);
	m_states["E2"] = State("E2", 13);
	m_states["F1"] = State("F1", 14);
	m_states["F2"] = State("F2", 15);
	m_states["F3"] = State("F3", 16);
	m_states["G1"] = State("G1", 17);
	m_states["H1"] = State("H1", 18);
	m_states["STOP"] = State("STOP", 19);
}

void Lex_block::fill_sym_lexems()
{
	m_sym_lexems["sym_letter"] = Symbolic_token("sym_letter", 0, 0);
	m_sym_lexems["sym_digit"] = Symbolic_token("sym_digit", 0, 1);
	m_sym_lexems["sym_aur_op"] = Symbolic_token("sym_aur_op", 0, 2);
	m_sym_lexems["sym_relat"] = Symbolic_token("sym_relat", 0, 3);
	m_sym_lexems["sym_lbrace"] = Symbolic_token("sym_lbrace", 0, 4);
	m_sym_lexems["sym_rbrace"] = Symbolic_token("sym_rbrace", 0, 5);
	m_sym_lexems["sym_dot"] = Symbolic_token("sym_dot", 0, 6);
	m_sym_lexems["sym_space"] = Symbolic_token("sym_space", 0, 7);
	m_sym_lexems["sym_lf"] = Symbolic_token("sym_lf", 0, 8);
	m_sym_lexems["sym_eof"] = Symbolic_token("sym_eof", 0, 9);
	m_sym_lexems["sym_error"] = Symbolic_token("sym_error", 0, 10);
}

void Lex_block::fill_lexems()
{
	m_lexems["l_line_num"] = Lexem("l_line_num", 0);
	m_lexems["l_operand"] = Lexem("l_operand", 1);
	m_lexems["l_aur_op"] = Lexem("l_aur_op", 2);
	m_lexems["l_relation"] = Lexem("l_relation", 3);
	m_lexems["l_next"] = Lexem("l_next", 4);
	m_lexems["l_let"] = Lexem("l_let", 5);
	m_lexems["l_for"] = Lexem("l_for", 6);
	m_lexems["l_goto"] = Lexem("l_goto", 7);
	m_lexems["l_gosub"] = Lexem("l_gosub", 8);
	m_lexems["l_lbrace"] = Lexem("l_lbrace", 9);
	m_lexems["l_rbrace"] = Lexem("l_rbrace", 10);
	m_lexems["l_if"] = Lexem("l_if", 11);
	m_lexems["l_return"] = Lexem("l_return", 12);
	m_lexems["l_end"] = Lexem("l_end", 13);
	m_lexems["l_to"] = Lexem("l_to", 14);
	m_lexems["l_step"] = Lexem("l_step", 15);
	m_lexems["l_rem"] = Lexem("l_rem", 16);
	m_lexems["l_error"] = Lexem("l_error", 17);
	m_lexems["l_eof"] = Lexem("l_eof", 18);
}

void Lex_block::init_begin_vect()
{
	m_begin_vector['E'] = 1;
	m_begin_vector['F'] = 3;
	m_begin_vector['G'] = 5;
	m_begin_vector['I'] = 11;
	m_begin_vector['L'] = 12;
	m_begin_vector['N'] = 14;
	m_begin_vector['R'] = 17;
	m_begin_vector['S'] = 23;
	m_begin_vector['T'] = 26;
}

void Lex_block::init_detect_table()
{
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('+', 0, &Lex_block::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('N', 0, &Lex_block::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('D', 0, &Lex_block::A2q));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('O', 0, &Lex_block::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('R', 0, &Lex_block::F1b));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('O', 0, &Lex_block::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('T', 8, &Lex_block::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('O', 0, &Lex_block::E1a));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('S', 0, &Lex_block::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('U', 0, &Lex_block::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('B', 0, &Lex_block::E1b));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('F', 0, &Lex_block::A2r));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('E', 0, &Lex_block::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('T', 0, &Lex_block::F1a));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('E', 0, &Lex_block::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('X', 0, &Lex_block::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('T', 0, &Lex_block::C1a));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('E', 0, &Lex_block::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('T', 22, &Lex_block::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('U', 0, &Lex_block::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('R', 0, &Lex_block::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('N', 0, &Lex_block::A2s));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('M', 0, &Lex_block::G1a));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('T', 0, &Lex_block::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('E', 0, &Lex_block::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('P', 0, &Lex_block::A2t));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('O', 0, &Lex_block::A2u));
}

void Lex_block::fill_transition_table()
{
	//Заполним сначала всю таблицу ошибками, ибо вручную заполнять её довольно-таки лениво
	for (auto it = m_states.begin(); it != m_states.end(); it++)
		for (auto it_sym = m_sym_lexems.begin(); it_sym != m_sym_lexems.end(); it_sym++)
			(m_transition_table[it->second])[it_sym->second] = &Lex_block::ERROR1;

	//А теперь уже заполним все необходимые места процедурами автомата
	(m_transition_table[m_states["A1"]])[m_sym_lexems["sym_digit"]] = &Lex_block::E2a;
	(m_transition_table[m_states["A1"]])[m_sym_lexems["sym_space"]] = &Lex_block::A1;
	(m_transition_table[m_states["A1"]])[m_sym_lexems["sym_lf"]] = &Lex_block::A1;
	(m_transition_table[m_states["A1"]])[m_sym_lexems["sym_eof"]] = &Lex_block::EXIT1;

	(m_transition_table[m_states["A2"]])[m_sym_lexems["sym_letter"]] = &Lex_block::C2a;
	(m_transition_table[m_states["A2"]])[m_sym_lexems["sym_digit"]] = &Lex_block::D1a;
	(m_transition_table[m_states["A2"]])[m_sym_lexems["sym_aur_op"]] = &Lex_block::A2a;
	(m_transition_table[m_states["A2"]])[m_sym_lexems["sym_relat"]] = &Lex_block::H1a;
	(m_transition_table[m_states["A2"]])[m_sym_lexems["sym_lbrace"]] = &Lex_block::A2h;
	(m_transition_table[m_states["A2"]])[m_sym_lexems["sym_rbrace"]] = &Lex_block::A3b;
	(m_transition_table[m_states["A2"]])[m_sym_lexems["sym_dot"]] = &Lex_block::D6;
	(m_transition_table[m_states["A2"]])[m_sym_lexems["sym_space"]] = &Lex_block::A2;
	(m_transition_table[m_states["A2"]])[m_sym_lexems["sym_lf"]] = &Lex_block::A1;
	(m_transition_table[m_states["A2"]])[m_sym_lexems["sym_eof"]] = &Lex_block::EXIT1;

	(m_transition_table[m_states["A3"]])[m_sym_lexems["sym_letter"]] = &Lex_block::B1a;
	(m_transition_table[m_states["A3"]])[m_sym_lexems["sym_digit"]] = &Lex_block::D1a;
	(m_transition_table[m_states["A3"]])[m_sym_lexems["sym_aur_op"]] = &Lex_block::A2a;
	(m_transition_table[m_states["A3"]])[m_sym_lexems["sym_relat"]] = &Lex_block::H1a;
	(m_transition_table[m_states["A3"]])[m_sym_lexems["sym_lbrace"]] = &Lex_block::A2h;
	(m_transition_table[m_states["A3"]])[m_sym_lexems["sym_rbrace"]] = &Lex_block::A3b;
	(m_transition_table[m_states["A3"]])[m_sym_lexems["sym_dot"]] = &Lex_block::D6;
	(m_transition_table[m_states["A3"]])[m_sym_lexems["sym_space"]] = &Lex_block::A3;
	(m_transition_table[m_states["A3"]])[m_sym_lexems["sym_lf"]] = &Lex_block::A1;
	(m_transition_table[m_states["A3"]])[m_sym_lexems["sym_eof"]] = &Lex_block::EXIT1;

	(m_transition_table[m_states["B1"]])[m_sym_lexems["sym_letter"]] = &Lex_block::M1;
	(m_transition_table[m_states["B1"]])[m_sym_lexems["sym_space"]] = &Lex_block::B1;

	(m_transition_table[m_states["C1"]])[m_sym_lexems["sym_letter"]] = &Lex_block::C2d;
	(m_transition_table[m_states["C1"]])[m_sym_lexems["sym_space"]] = &Lex_block::C1;

	(m_transition_table[m_states["C2"]])[m_sym_lexems["sym_letter"]] = &Lex_block::B1b;
	(m_transition_table[m_states["C2"]])[m_sym_lexems["sym_digit"]] = &Lex_block::A3a;
	(m_transition_table[m_states["C2"]])[m_sym_lexems["sym_aur_op"]] = &Lex_block::A2g;
	(m_transition_table[m_states["C2"]])[m_sym_lexems["sym_relat"]] = &Lex_block::H1b;
	(m_transition_table[m_states["C2"]])[m_sym_lexems["sym_lbrace"]] = &Lex_block::A2k;
	(m_transition_table[m_states["C2"]])[m_sym_lexems["sym_rbrace"]] = &Lex_block::A3c;
	(m_transition_table[m_states["C2"]])[m_sym_lexems["sym_space"]] = &Lex_block::C2;
	(m_transition_table[m_states["C2"]])[m_sym_lexems["sym_lf"]] = &Lex_block::A1a;
	(m_transition_table[m_states["C2"]])[m_sym_lexems["sym_eof"]] = &Lex_block::EXIT2;

	(m_transition_table[m_states["D1"]])[m_sym_lexems["sym_letter"]] = &Lex_block::M2;
	(m_transition_table[m_states["D1"]])[m_sym_lexems["sym_digit"]] = &Lex_block::D1b;
	(m_transition_table[m_states["D1"]])[m_sym_lexems["sym_aur_op"]] = &Lex_block::A2c;
	(m_transition_table[m_states["D1"]])[m_sym_lexems["sym_relat"]] = &Lex_block::H1c;
	(m_transition_table[m_states["D1"]])[m_sym_lexems["sym_lbrace"]] = &Lex_block::A2l;
	(m_transition_table[m_states["D1"]])[m_sym_lexems["sym_rbrace"]] = &Lex_block::A3d;
	(m_transition_table[m_states["D1"]])[m_sym_lexems["sym_dot"]] = &Lex_block::D2c;
	(m_transition_table[m_states["D1"]])[m_sym_lexems["sym_space"]] = &Lex_block::D1;
	(m_transition_table[m_states["D1"]])[m_sym_lexems["sym_lf"]] = &Lex_block::A1b;
	(m_transition_table[m_states["D1"]])[m_sym_lexems["sym_eof"]] = &Lex_block::EXIT3;

	(m_transition_table[m_states["D2"]])[m_sym_lexems["sym_letter"]] = &Lex_block::M3;
	(m_transition_table[m_states["D2"]])[m_sym_lexems["sym_digit"]] = &Lex_block::D2a;
	(m_transition_table[m_states["D2"]])[m_sym_lexems["sym_aur_op"]] = &Lex_block::A2d;
	(m_transition_table[m_states["D2"]])[m_sym_lexems["sym_relat"]] = &Lex_block::H1d;
	(m_transition_table[m_states["D2"]])[m_sym_lexems["sym_lbrace"]] = &Lex_block::A2m;
	(m_transition_table[m_states["D2"]])[m_sym_lexems["sym_rbrace"]] = &Lex_block::A3e;
	(m_transition_table[m_states["D2"]])[m_sym_lexems["sym_space"]] = &Lex_block::D2;
	(m_transition_table[m_states["D2"]])[m_sym_lexems["sym_lf"]] = &Lex_block::A1c;
	(m_transition_table[m_states["D2"]])[m_sym_lexems["sym_eof"]] = &Lex_block::EXIT4;

	(m_transition_table[m_states["D3"]])[m_sym_lexems["sym_digit"]] = &Lex_block::D5a;
	(m_transition_table[m_states["D3"]])[m_sym_lexems["sym_aur_op"]] = &Lex_block::D4a;
	(m_transition_table[m_states["D3"]])[m_sym_lexems["sym_space"]] = &Lex_block::D3;

	(m_transition_table[m_states["D4"]])[m_sym_lexems["sym_digit"]] = &Lex_block::D5b;
	(m_transition_table[m_states["D4"]])[m_sym_lexems["sym_space"]] = &Lex_block::D4;

	(m_transition_table[m_states["D5"]])[m_sym_lexems["sym_letter"]] = &Lex_block::B1c;
	(m_transition_table[m_states["D5"]])[m_sym_lexems["sym_digit"]] = &Lex_block::D5c;
	(m_transition_table[m_states["D5"]])[m_sym_lexems["sym_aur_op"]] = &Lex_block::A2e;
	(m_transition_table[m_states["D5"]])[m_sym_lexems["sym_relat"]] = &Lex_block::H1e;
	(m_transition_table[m_states["D5"]])[m_sym_lexems["sym_lbrace"]] = &Lex_block::A2n;
	(m_transition_table[m_states["D5"]])[m_sym_lexems["sym_rbrace"]] = &Lex_block::A3f;
	(m_transition_table[m_states["D5"]])[m_sym_lexems["sym_space"]] = &Lex_block::D5;
	(m_transition_table[m_states["D5"]])[m_sym_lexems["sym_lf"]] = &Lex_block::A1d;
	(m_transition_table[m_states["D5"]])[m_sym_lexems["sym_eof"]] = &Lex_block::EXIT5;

	(m_transition_table[m_states["D6"]])[m_sym_lexems["sym_digit"]] = &Lex_block::D2b;
	(m_transition_table[m_states["D6"]])[m_sym_lexems["sym_space"]] = &Lex_block::D6;

	(m_transition_table[m_states["E1"]])[m_sym_lexems["sym_digit"]] = &Lex_block::E2b;
	(m_transition_table[m_states["E1"]])[m_sym_lexems["sym_space"]] = &Lex_block::E1;

	(m_transition_table[m_states["E2"]])[m_sym_lexems["sym_letter"]] = &Lex_block::B1e;
	(m_transition_table[m_states["E2"]])[m_sym_lexems["sym_digit"]] = &Lex_block::E2c;
	(m_transition_table[m_states["E2"]])[m_sym_lexems["sym_aur_op"]] = &Lex_block::A2f;
	(m_transition_table[m_states["E2"]])[m_sym_lexems["sym_relat"]] = &Lex_block::H1f;
	(m_transition_table[m_states["E2"]])[m_sym_lexems["sym_lbrace"]] = &Lex_block::A2j;
	(m_transition_table[m_states["E2"]])[m_sym_lexems["sym_rbrace"]] = &Lex_block::A3g;
	(m_transition_table[m_states["E2"]])[m_sym_lexems["sym_space"]] = &Lex_block::E2;
	(m_transition_table[m_states["E2"]])[m_sym_lexems["sym_lf"]] = &Lex_block::A1e;
	(m_transition_table[m_states["E2"]])[m_sym_lexems["sym_eof"]] = &Lex_block::EXIT6;

	(m_transition_table[m_states["F1"]])[m_sym_lexems["sym_letter"]] = &Lex_block::F2a;
	(m_transition_table[m_states["F1"]])[m_sym_lexems["sym_space"]] = &Lex_block::F1;

	(m_transition_table[m_states["F2"]])[m_sym_lexems["sym_digit"]] = &Lex_block::F3a;
	(m_transition_table[m_states["F2"]])[m_sym_lexems["sym_relat"]] = &Lex_block::A2o;
	(m_transition_table[m_states["F2"]])[m_sym_lexems["sym_space"]] = &Lex_block::F2;

	(m_transition_table[m_states["F3"]])[m_sym_lexems["sym_relat"]] = &Lex_block::A2o;
	(m_transition_table[m_states["F3"]])[m_sym_lexems["sym_space"]] = &Lex_block::F3;

	(m_transition_table[m_states["G1"]])[m_sym_lexems["sym_letter"]] = &Lex_block::G1;
	(m_transition_table[m_states["G1"]])[m_sym_lexems["sym_digit"]] = &Lex_block::G1;
	(m_transition_table[m_states["G1"]])[m_sym_lexems["sym_aur_op"]] = &Lex_block::G1;
	(m_transition_table[m_states["G1"]])[m_sym_lexems["sym_relat"]] = &Lex_block::G1;
	(m_transition_table[m_states["G1"]])[m_sym_lexems["sym_lbrace"]] = &Lex_block::G1;
	(m_transition_table[m_states["G1"]])[m_sym_lexems["sym_rbrace"]] = &Lex_block::G1;
	(m_transition_table[m_states["G1"]])[m_sym_lexems["sym_space"]] = &Lex_block::G1;
	(m_transition_table[m_states["G1"]])[m_sym_lexems["sym_error"]] = &Lex_block::G1;
	(m_transition_table[m_states["G1"]])[m_sym_lexems["sym_dot"]] = &Lex_block::G1;
	(m_transition_table[m_states["G1"]])[m_sym_lexems["sym_lf"]] = &Lex_block::A1;
	(m_transition_table[m_states["G1"]])[m_sym_lexems["sym_eof"]] = &Lex_block::EXIT1;

	(m_transition_table[m_states["H1"]])[m_sym_lexems["sym_letter"]] = &Lex_block::C2b;
	(m_transition_table[m_states["H1"]])[m_sym_lexems["sym_digit"]] = &Lex_block::D1c;
	(m_transition_table[m_states["H1"]])[m_sym_lexems["sym_aur_op"]] = &Lex_block::A2g;
	(m_transition_table[m_states["H1"]])[m_sym_lexems["sym_relat"]] = &Lex_block::A2p;
	(m_transition_table[m_states["H1"]])[m_sym_lexems["sym_lbrace"]] = &Lex_block::A2k;
	(m_transition_table[m_states["H1"]])[m_sym_lexems["sym_rbrace"]] = &Lex_block::A3c;
	(m_transition_table[m_states["H1"]])[m_sym_lexems["sym_dot"]] = &Lex_block::D6a;
	(m_transition_table[m_states["H1"]])[m_sym_lexems["sym_space"]] = &Lex_block::H1;
	(m_transition_table[m_states["H1"]])[m_sym_lexems["sym_lf"]] = &Lex_block::A1a;
	(m_transition_table[m_states["H1"]])[m_sym_lexems["sym_eof"]] = &Lex_block::EXIT2;
}

void Lex_block::DA1E()
{
	m_reg_value = m_reg_line_num;
}

void Lex_block::DA1D()
{
	m_reg_order -= (long)m_reg_counter;
	calc_constant();
}

void Lex_block::DA2D()
{
	m_reg_order -= (long)m_reg_counter;
	m_reg_class = m_lexems["l_operand"];
	calc_constant();
}

void Lex_block::DA3D()
{
	if (m_reg_sign == -1)
	{
		m_reg_order *= -1;
	}
	m_reg_order -= (long)m_reg_counter;
	calc_constant();
}

State Lex_block::ERROR1()
{
	m_curr_sym = transliterator(m_input_file.get());
	if (m_reg_class != m_lexems["l_error"])
	{
		m_reg_class = m_lexems["l_error"];
		create_lexem();
	}

	if (m_curr_sym.m_sym_id == 9) // eof
	{
		return EXIT1();
	}

	if (m_curr_sym.m_sym_id == 10)
	{
		return m_states["A1"];
	}

	return m_curr_state;
}

State Lex_block::A1() { return m_states["A1"]; }

State Lex_block::A2() { return m_states["A2"]; }

State Lex_block::A3() { return m_states["A3"]; }

State Lex_block::B1() { return m_states["B1"]; }

State Lex_block::C1() { return m_states["C1"]; }

State Lex_block::C2() { return m_states["C2"]; }

State Lex_block::D1() { return m_states["D1"]; }

State Lex_block::D2() { return m_states["D2"]; }

State Lex_block::D3() { return m_states["D3"]; }

State Lex_block::D4() { return m_states["D4"]; }

State Lex_block::D5() { return m_states["D5"]; }

State Lex_block::D6() { return m_states["D6"]; }

State Lex_block::E1() { return m_states["E1"]; }

State Lex_block::E2() { return m_states["E2"]; }

State Lex_block::F1() { return m_states["F1"]; }

State Lex_block::F2() { return m_states["F2"]; }

State Lex_block::F3() { return m_states["F3"]; }

State Lex_block::G1() { return m_states["G1"]; }

State Lex_block::H1() { return m_states["H1"]; }

State Lex_block::EXIT1()
{
	m_reg_class = m_lexems["l_eof"];
	create_lexem();
	return m_states["STOP"];
}

State Lex_block::EXIT2()
{
	create_lexem();
	return EXIT1();
}

State Lex_block::EXIT3()
{
	DA1D();
	return EXIT2();
}
State Lex_block::EXIT4()
{
	DA2D();
	return EXIT2();
}
State Lex_block::EXIT5()
{
	DA3D();
	return EXIT2();
}
State Lex_block::EXIT6()
{
	DA1E();
	return EXIT2();
}


State Lex_block::A1a()
{
	if (m_reg_var_name != "")
		add_variable();
	create_lexem();
	if (m_curr_sym.m_sym_id == 3 && m_curr_sym.m_sym_value == '=')
		m_reg_relation = 1;
	else if (m_curr_sym.m_sym_value == '<')
		m_reg_relation = 3;
	else if (m_curr_sym.m_sym_value == '>')
		m_reg_relation = 4;
	return A1();
}

State Lex_block::A1b()
{
	DA1D();
	return A1a();
}

State Lex_block::A1c()
{
	DA2D();
	return A1a();
}

State Lex_block::A1d()
{
	DA3D();
	return A1a();
}

State Lex_block::A1e()
{
	DA1E();
	return A1a();
}

State Lex_block::A2a()
{
	m_reg_class = m_lexems["l_aur_op"];
	m_reg_value = m_curr_sym.m_sym_value;
	return A2b();
}

State Lex_block::A2b()
{
	create_lexem();
	return A2();
}

State Lex_block::A2c()
{
	DA1D();
	return A2g();
}

State Lex_block::A2d()
{
	DA2D();
	return A2g();
}

State Lex_block::A2e()
{
	DA3D();
	return A2g();
}

State Lex_block::A2f()
{
	DA1E();
	return A2g();
}

State Lex_block::A2g()
{
	if (m_reg_var_name != "")
		add_variable();
	create_lexem();
	if (m_curr_sym.m_sym_value == '=')
		m_reg_relation = 1;
	else if (m_curr_sym.m_sym_value == '<')
		m_reg_relation = 3;
	else if (m_curr_sym.m_sym_value == '>')
		m_reg_relation = 4;
	return A2a();
}

State Lex_block::A2h()
{
	m_reg_class = m_lexems["l_lbrace"];
	create_lexem();
	return A2();
}

State Lex_block::A2j()
{
	DA1E();
	return A2k();
}

State Lex_block::A2k()
{
	if (m_reg_var_name != "")
		add_variable();
	create_lexem();
	if (m_curr_sym.m_sym_value == '=')
		m_reg_relation = 1;
	else if (m_curr_sym.m_sym_value == '<')
		m_reg_relation = 3;
	else if (m_curr_sym.m_sym_value == '>')
		m_reg_relation = 4;
	return A2h();
}

State Lex_block::A2l()
{
	DA1D();
	return A2k();
}

State Lex_block::A2m()
{
	DA2D();
	return A2k();
}

State Lex_block::A2n()
{
	DA3D();
	return A2k();
}

State Lex_block::A2o()
{
	if (m_curr_sym.m_sym_value != '=')
	{
		return ERROR1();
	}
	else
	{
		add_variable();
		return A2b();
	}

}

State Lex_block::A2p()
{

	if (m_curr_sym.m_sym_value == '=')
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
	else// вторым символом отношения может быть только =, иначе ошибка
	{
		return ERROR1();
	}
	return A2b();
}

State Lex_block::A2q()
{
	m_reg_class = m_lexems["l_end"];
	return A2b();
}

State Lex_block::A2r()
{
	m_reg_class = m_lexems["l_if"];
	return A2b();
}

State Lex_block::A2s()
{
	m_reg_class = m_lexems["l_return"];
	return A2b();
}

State Lex_block::A2t()
{
	m_reg_class = m_lexems["l_step"];
	return A2b();
}

State Lex_block::A2u()
{
	m_reg_class = m_lexems["l_to"];
	return A2b();
}

State Lex_block::A3a()
{
	m_reg_var_name += m_curr_sym.m_sym_value + '0';
	add_variable();
	create_lexem();
	return A3();
}

State Lex_block::A3b()
{
	m_reg_class = m_lexems["l_rbrace"];
	create_lexem();
	return A3();
}

State Lex_block::A3c()
{

	if (m_reg_var_name != "")
		add_variable();
	create_lexem();
	if (m_curr_sym.m_sym_value == '=')
		m_reg_relation = 1;
	else if (m_curr_sym.m_sym_value == '<')
		m_reg_relation = 3;
	else if (m_curr_sym.m_sym_value == '>')
		m_reg_relation = 4;
	return A3b();
}

State Lex_block::A3d()
{
	DA1D();
	return A3c();
}

State Lex_block::A3e()
{
	DA2D();
	return A3c();
}

State Lex_block::A3f()
{
	DA3D();
	return A3c();
}

State Lex_block::A3g()
{
	DA1E();
	return A3c();
}

State Lex_block::B1a()
{
	m_reg_detection = m_begin_vector[m_curr_sym.m_sym_value];
	if (m_reg_detection == 0)
	{
		return ERROR1();
	}
	return B1();
}

State Lex_block::B1b()
{
	if (m_reg_var_name != "")
		add_variable();
	create_lexem();
	return B1a();
}

State Lex_block::B1c()
{
	DA3D();
	return B1b();
}

State Lex_block::B1d()
{
	m_reg_detection++;
	return B1();
}

State Lex_block::B1e()
{
	DA1E();
	return B1b();
}

State Lex_block::C1a()
{
	m_reg_class = m_lexems["l_next"];
	return C1();
}

State Lex_block::C2a()
{
	m_reg_class = m_lexems["l_operand"];
	return C2d();
}

State Lex_block::C2b()
{
	create_lexem();
	return C2a();
}

State Lex_block::C2d()
{
	m_reg_var_name = m_curr_sym.m_sym_value;

	return C2();
}

State Lex_block::H1a()
{
	m_reg_class = m_lexems["l_relation"];
	if (m_curr_sym.m_sym_value == '=')
		m_reg_relation = 1;
	else if (m_curr_sym.m_sym_value == '<')
		m_reg_relation = 3;
	else if (m_curr_sym.m_sym_value == '>')
		m_reg_relation = 4;
	return H1();
}

State Lex_block::H1b()
{
	if (m_reg_var_name != "")
		add_variable();
	create_lexem();
	return H1a();
}

State Lex_block::H1c()
{
	DA1D();
	return H1b();
}

State Lex_block::H1d()
{
	DA2D();
	if (m_curr_sym.m_sym_value == '=')
		m_reg_relation = 1;
	else if (m_curr_sym.m_sym_value == '<')
		m_reg_relation = 3;
	else if (m_curr_sym.m_sym_value == '>')
		m_reg_relation = 4;
	return H1b();
}

State Lex_block::H1e()
{
	DA3D();
	if (m_curr_sym.m_sym_value == '=')
		m_reg_relation = 1;
	else if (m_curr_sym.m_sym_value == '<')
		m_reg_relation = 3;
	else if (m_curr_sym.m_sym_value == '>')
		m_reg_relation = 4;
	return H1b();
}

State Lex_block::H1f()
{
	DA1E();
	return H1b();
}

State Lex_block::G1a()
{
	m_reg_class = m_lexems["l_rem"];
	create_lexem();
	return G1();
}

State Lex_block::E1a()
{
	m_reg_class = m_lexems["l_goto"];
	return E1();
}

State Lex_block::E1b()
{
	m_reg_class = m_lexems["l_gosub"];
	return E1();
}

State Lex_block::E2a()
{
	m_reg_class = m_lexems["l_line_num"];
	m_reg_line_num = m_curr_sym.m_sym_value;
	return E2b();
}

State Lex_block::E2b()
{
	m_reg_line_num = m_curr_sym.m_sym_value;
	return E2();
}

State Lex_block::E2c()
{
	m_reg_line_num = m_reg_line_num * 10 + m_curr_sym.m_sym_value;
	return E2();
}

State Lex_block::D1a()
{
	m_reg_class = m_lexems["l_operand"];
	m_reg_number = m_curr_sym.m_sym_value;
	return D1();
}

State Lex_block::D1b()
{
	m_reg_number = m_reg_number * 10 + m_curr_sym.m_sym_value;
	return D1();
}

State Lex_block::D1c()
{
	create_lexem();
	return D1a();
}

State Lex_block::D2a()
{
	m_reg_counter++;
	m_reg_number = m_reg_number * 10 + m_curr_sym.m_sym_value;
	return D2();
}

State Lex_block::D2b()
{
	m_reg_class = m_lexems["l_operand"];
	m_reg_counter = 1;
	m_reg_number = m_curr_sym.m_sym_value;
	return D2();
}

State Lex_block::D2c()
{
	m_reg_counter = 0;
	return D2();
}

State Lex_block::D3a()
{
	m_reg_counter = 0;
	return D3();
}

State Lex_block::D4a()
{
	if (m_curr_sym.m_sym_value == '-')
	{
		m_reg_sign = -1;
	}
	else if (m_curr_sym.m_sym_value == '+')
	{
		m_reg_sign = +1;
	}
	else
	{
		return ERROR1();
	}
	return D4();
}

State Lex_block::D5a()
{
	m_reg_sign = +1;
	return D5b();
}

State Lex_block::D5b()
{
	m_reg_order = m_curr_sym.m_sym_value;
	return D5();
}

State Lex_block::D5c()
{
	m_reg_order = m_reg_order * 10 + m_curr_sym.m_sym_value;
	return D5();
}

State Lex_block::D6a()
{
	if (m_curr_sym.m_sym_value == '=')
		m_reg_relation = 1;
	else if (m_curr_sym.m_sym_value == '<')
		m_reg_relation = 3;
	else if (m_curr_sym.m_sym_value == '>')
		m_reg_relation = 4;
	create_lexem();
	return D6();
}

State Lex_block::F1a()
{
	m_reg_class = m_lexems["l_let"];
	return F1();
}

State Lex_block::F1b()
{
	m_reg_class = m_lexems["l_for"];
	return F1();
}

State Lex_block::F2a()
{
	m_reg_var_name = static_cast<char>(m_curr_sym.m_sym_value);
	return F2();
}

State Lex_block::F3a()
{
	m_reg_var_name += m_curr_sym.m_sym_value + '0';
	return F3();
}

State Lex_block::M1()
{
	if (m_reg_detection == 0)
		return ERROR1();
	if (m_curr_sym.m_sym_value == std::get<0>(m_detect_table[m_reg_detection]))
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

State Lex_block::M2()
{
	if (m_curr_sym.m_sym_value != 'E')
	{
		DA1D();
		return B1b();
	}
	else
	{
		return D3a();
	}
}

State Lex_block::M3()
{
	if (m_curr_sym.m_sym_value != 'E')
	{
		DA2D();
		return B1b();
	}
	else
	{
		return D3();
	}
}

std::string Lex_block::relation_table(long long int ch)
{
	switch (ch)
	{
	case(1): {return "="; }
	case(2): {return "!="; }
	case(3): {return "<"; }
	case(4): {return ">"; }
	case(5): {return "<="; }
	case(6): {return ">="; }
	default: {return ""; }
	}
}

void Lex_block::print_lexem_list()
{
	for (auto it = m_lexem_list.begin(); it != m_lexem_list.end(); ++it)
	{
		switch (std::get<0>(*it).m_lex_id)
		{
		case 0: //line
		{
			std::cout << std::endl << std::get<2>(*it) << " ";
			break;
		}
		case 1: //operand
		{
			std::cout << (reinterpret_cast<operand*>(std::get<1>(*it))->op_name) << " ";
			break;
		}
		case 2: // ar op
		{
			std::cout << ((char)std::get<1>(*it)) << " ";
			break;
		}
		case 3: // relation
		{
			std::cout << (relation_table(std::get<1>(*it))) << " ";
			break;
		}
		case 4: //next
		{
			std::cout << "NEXT " << (reinterpret_cast<operand*>(std::get<1>(*it))->op_name) << " ";
			break;
		}
		case 5: //let
		{
			std::cout << "LET " << (reinterpret_cast<operand*>(std::get<1>(*it))->op_name) << " =" << " ";
			break;
		}
		case 6: //for
		{
			std::cout << "FOR " << (reinterpret_cast<operand*>(std::get<1>(*it))->op_name) << " =" << " ";
			break;
		}
		case 7: //goto
		{
			std::cout << "GOTO " << (std::get<1>(*it)) << " {" << std::get<2>(*reinterpret_cast<std::tuple<Lexem, long long int, size_t>*>(std::get<1>(*it))) << "} ";
			break;
		}
		case 8: //gosub
		{
			std::cout << "GOSUB " << (std::get<1>(*it)) << " {" << std::get<2>(*reinterpret_cast<std::tuple<Lexem, long long int, size_t>*>(std::get<1>(*it))) << "} ";
			break;
		}
		case 9:// (
		{
			std::cout << "(" << " ";
			break;
		}
		case 10:// )
		{
			std::cout << ")" << " ";
			break;
		}
		case 11:// if
		{
			std::cout << "IF" << " ";
			break;
		}
		case 12:// return
		{
			std::cout << "RETURN" << " ";
			break;
		}
		case 13:// end
		{
			std::cout << "END" << " ";
			break;
		}
		case 14:// to
		{
			std::cout << "TO" << " ";
			break;
		}
		case 15:// step
		{
			std::cout << "STEP" << " ";
			break;
		}
		case 16:// rem
		{
			std::cout << "REM" << " ";
			break;
		}
		case 17:// err
		{
			std::cout << "!ERROR!" << " ";
			break;
		}
		case 18:// return
		{
			std::cout << std::endl << "EOF" << " ";
			break;
		}


		};
	}
	std::cout << std::endl;

}

void Lex_block::calc_constant()
{
	m_reg_number = m_reg_number * std::powl(10.0, (long double)m_reg_order);
	add_constant();
	m_reg_number = 0;
	m_reg_counter = 0;
	m_reg_order = 0;
}

void Lex_block::add_constant()
{
	std::ostringstream str_stream;
	str_stream << m_reg_number;
	std::string const_id = str_stream.str();
	operand var;
	var.op_name = const_id;
	var.op_value = m_reg_number;
	m_name_table.emplace(std::pair<std::string, operand>(const_id, var));
	m_reg_pointer = reinterpret_cast<long long int>(&m_name_table[const_id]);

}

void Lex_block::add_variable()
{
	operand var;
	var.op_name = m_reg_var_name;
	var.op_value = 0;
	m_name_table.emplace(std::pair<std::string, operand>(m_reg_var_name, var));
	m_reg_pointer = reinterpret_cast<long long int>(&m_name_table[m_reg_var_name]);
	m_reg_var_name = "";
}

void Lex_block::parse()
{
	while (m_curr_state.m_id != m_states["STOP"].m_id)
	{
		m_curr_sym = transliterator(m_input_file.get());

		m_curr_state = (this->*m_transition_table[m_curr_state][m_curr_sym])();
	}
}

void Lex_block::create_lexem()
{
	if (m_reg_class.m_lex_id == 0 /*line*/)
	{
		if (m_lines.contains(m_reg_line_num))
		{
			ERROR1();
			return;
		}
		std::tuple<Lexem, long long int, int> tup(m_reg_class, m_reg_line_num, m_reg_line_num);
		m_lexem_list.push_back(tup);

		auto it = &(*(--m_lexem_list.end()));
		m_lines[m_reg_line_num] = reinterpret_cast<long long>(it);
	}
	else if (m_reg_class.m_lex_id == 7 /*goto*/ || m_reg_class.m_lex_id == 8 /*gosub*/)
	{
		if (!m_lines.contains(m_reg_line_num))
		{
			ERROR1();
			return;
		}
		m_reg_pointer = m_lines[m_reg_line_num];
		std::tuple<Lexem, long long int, int> tup(m_reg_class, m_reg_pointer, m_reg_line_num);
		m_lexem_list.push_back(tup);
	}
	else if (m_reg_class.m_lex_id == 2 /*aur oper*/)
	{
		std::tuple<Lexem, long long int, int> tup(m_reg_class, m_reg_value, m_reg_line_num);
		m_lexem_list.push_back(tup);
	}
	else if (m_reg_class.m_lex_id == 1 /*operand*/ || m_reg_class.m_lex_id == 4 /*next*/ || m_reg_class.m_lex_id == 5 /*let*/ || \
		m_reg_class.m_lex_id == 6 /*for*/)
	{
		std::tuple<Lexem, long long int, int> tup(m_reg_class, m_reg_pointer, m_reg_line_num);
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

std::map<std::string, Lexem> Lex_block::get_lexems()
{
	return m_lexems;
}
std::list<std::tuple<Lexem, long long int, size_t>> Lex_block::get_lexem_list()
{
	return m_lexem_list;
}