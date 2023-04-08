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
	fill_lexems();
	fill_sym_lexems();
	init_begin_vect();
	init_detect_table();
	fill_transition_table();
	
	parse();
}

Symbolic_token lex_block_basic::transliterator(int sym)
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

void lex_block_basic::fill_states()
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

void lex_block_basic::fill_sym_lexems()
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

void lex_block_basic::fill_lexems()
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

void lex_block_basic::init_begin_vect()
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

void lex_block_basic::init_detect_table()
{ 
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('+', 0, &lex_block_basic::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('N', 0, &lex_block_basic::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('D', 0, &lex_block_basic::A2q));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('O', 0, &lex_block_basic::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('R', 0, &lex_block_basic::F1b));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('O', 0, &lex_block_basic::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('T', 8, &lex_block_basic::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('O', 0, &lex_block_basic::E1a));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('S', 0, &lex_block_basic::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('U', 0, &lex_block_basic::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('B', 0, &lex_block_basic::E1b));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('F', 0, &lex_block_basic::A2r));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('E', 0, &lex_block_basic::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('T', 0, &lex_block_basic::F1a));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('E', 0, &lex_block_basic::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('X', 0, &lex_block_basic::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('T', 0, &lex_block_basic::C1a));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('E', 0, &lex_block_basic::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('T', 22, &lex_block_basic::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('U', 0, &lex_block_basic::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('R', 0, &lex_block_basic::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('N', 0, &lex_block_basic::A2s));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('M', 0, &lex_block_basic::G1a));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('T', 0, &lex_block_basic::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('E', 0, &lex_block_basic::B1d));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('P', 0, &lex_block_basic::A2t));
	m_detect_table.push_back(std::make_tuple<char, int, lex_func_ptr>('O', 0, &lex_block_basic::A2u));
}

void lex_block_basic::fill_transition_table()
{
	for (auto it = m_states.begin(); it != m_states.end(); it++)
	{
		for (auto it_sym = m_sym_lexems.begin(); it_sym != m_sym_lexems.end(); it_sym++)
		{

			(m_transition_table[it->second])[it_sym->second] = &lex_block_basic::ERROR1;

		}
	}

	(m_transition_table[m_states["A1"]])[m_sym_lexems["sym_digit"]] = &lex_block_basic::E2a;
	(m_transition_table[m_states["A1"]])[m_sym_lexems["sym_space"]] = &lex_block_basic::A1;
	(m_transition_table[m_states["A1"]])[m_sym_lexems["sym_lf"]] = &lex_block_basic::A1;
	(m_transition_table[m_states["A1"]])[m_sym_lexems["sym_eof"]] = &lex_block_basic::EXIT1;

	(m_transition_table[m_states["A2"]])[m_sym_lexems["sym_letter"]] = &lex_block_basic::C2a;
	(m_transition_table[m_states["A2"]])[m_sym_lexems["sym_digit"]] = &lex_block_basic::D1a;
	(m_transition_table[m_states["A2"]])[m_sym_lexems["sym_aur_op"]] = &lex_block_basic::A2a;
	(m_transition_table[m_states["A2"]])[m_sym_lexems["sym_relat"]] = &lex_block_basic::H1a;
	(m_transition_table[m_states["A2"]])[m_sym_lexems["sym_lbrace"]] = &lex_block_basic::A2h;
	(m_transition_table[m_states["A2"]])[m_sym_lexems["sym_rbrace"]] = &lex_block_basic::A3b;
	(m_transition_table[m_states["A2"]])[m_sym_lexems["sym_dot"]] = &lex_block_basic::D6;
	(m_transition_table[m_states["A2"]])[m_sym_lexems["sym_space"]] = &lex_block_basic::A2;
	(m_transition_table[m_states["A2"]])[m_sym_lexems["sym_lf"]] = &lex_block_basic::A1;
	(m_transition_table[m_states["A2"]])[m_sym_lexems["sym_eof"]] = &lex_block_basic::EXIT1;

	(m_transition_table[m_states["A3"]])[m_sym_lexems["sym_letter"]] = &lex_block_basic::B1a;
	(m_transition_table[m_states["A3"]])[m_sym_lexems["sym_digit"]] = &lex_block_basic::D1a;
	(m_transition_table[m_states["A3"]])[m_sym_lexems["sym_aur_op"]] = &lex_block_basic::A2a;
	(m_transition_table[m_states["A3"]])[m_sym_lexems["sym_relat"]] = &lex_block_basic::H1a;
	(m_transition_table[m_states["A3"]])[m_sym_lexems["sym_lbrace"]] = &lex_block_basic::A2h;
	(m_transition_table[m_states["A3"]])[m_sym_lexems["sym_rbrace"]] = &lex_block_basic::A3b;
	(m_transition_table[m_states["A3"]])[m_sym_lexems["sym_dot"]] = &lex_block_basic::D6;
	(m_transition_table[m_states["A3"]])[m_sym_lexems["sym_space"]] = &lex_block_basic::A3;
	(m_transition_table[m_states["A3"]])[m_sym_lexems["sym_lf"]] = &lex_block_basic::A1;
	(m_transition_table[m_states["A3"]])[m_sym_lexems["sym_eof"]] = &lex_block_basic::EXIT1;

	(m_transition_table[m_states["B1"]])[m_sym_lexems["sym_letter"]] = &lex_block_basic::M1;
	(m_transition_table[m_states["B1"]])[m_sym_lexems["sym_space"]] = &lex_block_basic::B1;

	(m_transition_table[m_states["C1"]])[m_sym_lexems["sym_letter"]] = &lex_block_basic::C2d;
	(m_transition_table[m_states["C1"]])[m_sym_lexems["sym_space"]] = &lex_block_basic::C1;

	(m_transition_table[m_states["C2"]])[m_sym_lexems["sym_letter"]] = &lex_block_basic::B1b;
	(m_transition_table[m_states["C2"]])[m_sym_lexems["sym_digit"]] = &lex_block_basic::A3a;
	(m_transition_table[m_states["C2"]])[m_sym_lexems["sym_aur_op"]] = &lex_block_basic::A2g;
	(m_transition_table[m_states["C2"]])[m_sym_lexems["sym_relat"]] = &lex_block_basic::H1b;
	(m_transition_table[m_states["C2"]])[m_sym_lexems["sym_lbrace"]] = &lex_block_basic::A2k;
	(m_transition_table[m_states["C2"]])[m_sym_lexems["sym_rbrace"]] = &lex_block_basic::A3c;
	(m_transition_table[m_states["C2"]])[m_sym_lexems["sym_space"]] = &lex_block_basic::C2;
	(m_transition_table[m_states["C2"]])[m_sym_lexems["sym_lf"]] = &lex_block_basic::A1a;
	(m_transition_table[m_states["C2"]])[m_sym_lexems["sym_eof"]] = &lex_block_basic::EXIT2;

	(m_transition_table[m_states["D1"]])[m_sym_lexems["sym_letter"]] = &lex_block_basic::M2;
	(m_transition_table[m_states["D1"]])[m_sym_lexems["sym_digit"]] = &lex_block_basic::D1b;
	(m_transition_table[m_states["D1"]])[m_sym_lexems["sym_aur_op"]] = &lex_block_basic::A2c;
	(m_transition_table[m_states["D1"]])[m_sym_lexems["sym_relat"]] = &lex_block_basic::H1c;
	(m_transition_table[m_states["D1"]])[m_sym_lexems["sym_lbrace"]] = &lex_block_basic::A2l;
	(m_transition_table[m_states["D1"]])[m_sym_lexems["sym_rbrace"]] = &lex_block_basic::A3d;
	(m_transition_table[m_states["D1"]])[m_sym_lexems["sym_dot"]] = &lex_block_basic::D2c;
	(m_transition_table[m_states["D1"]])[m_sym_lexems["sym_space"]] = &lex_block_basic::D1;
	(m_transition_table[m_states["D1"]])[m_sym_lexems["sym_lf"]] = &lex_block_basic::A1b;
	(m_transition_table[m_states["D1"]])[m_sym_lexems["sym_eof"]] = &lex_block_basic::EXIT3;

	(m_transition_table[m_states["D2"]])[m_sym_lexems["sym_letter"]] = &lex_block_basic::M3;
	(m_transition_table[m_states["D2"]])[m_sym_lexems["sym_digit"]] = &lex_block_basic::D2a;
	(m_transition_table[m_states["D2"]])[m_sym_lexems["sym_aur_op"]] = &lex_block_basic::A2d;
	(m_transition_table[m_states["D2"]])[m_sym_lexems["sym_relat"]] = &lex_block_basic::H1d;
	(m_transition_table[m_states["D2"]])[m_sym_lexems["sym_lbrace"]] = &lex_block_basic::A2m;
	(m_transition_table[m_states["D2"]])[m_sym_lexems["sym_rbrace"]] = &lex_block_basic::A3e;
	(m_transition_table[m_states["D2"]])[m_sym_lexems["sym_space"]] = &lex_block_basic::D2;
	(m_transition_table[m_states["D2"]])[m_sym_lexems["sym_lf"]] = &lex_block_basic::A1c;
	(m_transition_table[m_states["D2"]])[m_sym_lexems["sym_eof"]] = &lex_block_basic::EXIT4;

	(m_transition_table[m_states["D3"]])[m_sym_lexems["sym_digit"]] = &lex_block_basic::D5a;
	(m_transition_table[m_states["D3"]])[m_sym_lexems["sym_aur_op"]] = &lex_block_basic::D4a;
	(m_transition_table[m_states["D3"]])[m_sym_lexems["sym_space"]] = &lex_block_basic::D3;

	(m_transition_table[m_states["D4"]])[m_sym_lexems["sym_digit"]] = &lex_block_basic::D5b;
	(m_transition_table[m_states["D4"]])[m_sym_lexems["sym_space"]] = &lex_block_basic::D4;

	(m_transition_table[m_states["D5"]])[m_sym_lexems["sym_letter"]] = &lex_block_basic::B1c;
	(m_transition_table[m_states["D5"]])[m_sym_lexems["sym_digit"]] = &lex_block_basic::D5c;
	(m_transition_table[m_states["D5"]])[m_sym_lexems["sym_aur_op"]] = &lex_block_basic::A2e;
	(m_transition_table[m_states["D5"]])[m_sym_lexems["sym_relat"]] = &lex_block_basic::H1e;
	(m_transition_table[m_states["D5"]])[m_sym_lexems["sym_lbrace"]] = &lex_block_basic::A2n;
	(m_transition_table[m_states["D5"]])[m_sym_lexems["sym_rbrace"]] = &lex_block_basic::A3f;
	(m_transition_table[m_states["D5"]])[m_sym_lexems["sym_space"]] = &lex_block_basic::D5;
	(m_transition_table[m_states["D5"]])[m_sym_lexems["sym_lf"]] = &lex_block_basic::A1d;
	(m_transition_table[m_states["D5"]])[m_sym_lexems["sym_eof"]] = &lex_block_basic::EXIT5;

	(m_transition_table[m_states["D6"]])[m_sym_lexems["sym_digit"]] = &lex_block_basic::D2b;
	(m_transition_table[m_states["D6"]])[m_sym_lexems["sym_space"]] = &lex_block_basic::D6;

	(m_transition_table[m_states["E1"]])[m_sym_lexems["sym_digit"]] = &lex_block_basic::E2b;
	(m_transition_table[m_states["E1"]])[m_sym_lexems["sym_space"]] = &lex_block_basic::E1;

	(m_transition_table[m_states["E2"]])[m_sym_lexems["sym_letter"]] = &lex_block_basic::B1e;
	(m_transition_table[m_states["E2"]])[m_sym_lexems["sym_digit"]] = &lex_block_basic::E2c;
	(m_transition_table[m_states["E2"]])[m_sym_lexems["sym_aur_op"]] = &lex_block_basic::A2f;
	(m_transition_table[m_states["E2"]])[m_sym_lexems["sym_relat"]] = &lex_block_basic::H1f;
	(m_transition_table[m_states["E2"]])[m_sym_lexems["sym_lbrace"]] = &lex_block_basic::A2j;
	(m_transition_table[m_states["E2"]])[m_sym_lexems["sym_rbrace"]] = &lex_block_basic::A3g;
	(m_transition_table[m_states["E2"]])[m_sym_lexems["sym_space"]] = &lex_block_basic::E2;
	(m_transition_table[m_states["E2"]])[m_sym_lexems["sym_lf"]] = &lex_block_basic::A1e;
	(m_transition_table[m_states["E2"]])[m_sym_lexems["sym_eof"]] = &lex_block_basic::EXIT6;

	(m_transition_table[m_states["F1"]])[m_sym_lexems["sym_letter"]] = &lex_block_basic::F2a;
	(m_transition_table[m_states["F1"]])[m_sym_lexems["sym_space"]] = &lex_block_basic::F1;

	(m_transition_table[m_states["F2"]])[m_sym_lexems["sym_digit"]] = &lex_block_basic::F3a;
	(m_transition_table[m_states["F2"]])[m_sym_lexems["sym_relat"]] = &lex_block_basic::A2o;
	(m_transition_table[m_states["F2"]])[m_sym_lexems["sym_space"]] = &lex_block_basic::F2;

	(m_transition_table[m_states["F3"]])[m_sym_lexems["sym_relat"]] = &lex_block_basic::A2o;
	(m_transition_table[m_states["F3"]])[m_sym_lexems["sym_space"]] = &lex_block_basic::F3;

	(m_transition_table[m_states["G1"]])[m_sym_lexems["sym_letter"]] = &lex_block_basic::G1;
	(m_transition_table[m_states["G1"]])[m_sym_lexems["sym_digit"]] = &lex_block_basic::G1;
	(m_transition_table[m_states["G1"]])[m_sym_lexems["sym_aur_op"]] = &lex_block_basic::G1;
	(m_transition_table[m_states["G1"]])[m_sym_lexems["sym_relat"]] = &lex_block_basic::G1;
	(m_transition_table[m_states["G1"]])[m_sym_lexems["sym_lbrace"]] = &lex_block_basic::G1;
	(m_transition_table[m_states["G1"]])[m_sym_lexems["sym_rbrace"]] = &lex_block_basic::G1;
	(m_transition_table[m_states["G1"]])[m_sym_lexems["sym_space"]] = &lex_block_basic::G1;
	(m_transition_table[m_states["G1"]])[m_sym_lexems["sym_error"]] = &lex_block_basic::G1;
	(m_transition_table[m_states["G1"]])[m_sym_lexems["sym_dot"]] = &lex_block_basic::G1;
	(m_transition_table[m_states["G1"]])[m_sym_lexems["sym_lf"]] = &lex_block_basic::A1;
	(m_transition_table[m_states["G1"]])[m_sym_lexems["sym_eof"]] = &lex_block_basic::EXIT1;

	(m_transition_table[m_states["H1"]])[m_sym_lexems["sym_letter"]] = &lex_block_basic::C2b;
	(m_transition_table[m_states["H1"]])[m_sym_lexems["sym_digit"]] = &lex_block_basic::D1c;
	(m_transition_table[m_states["H1"]])[m_sym_lexems["sym_aur_op"]] = &lex_block_basic::A2g;
	(m_transition_table[m_states["H1"]])[m_sym_lexems["sym_relat"]] = &lex_block_basic::A2p;
	(m_transition_table[m_states["H1"]])[m_sym_lexems["sym_lbrace"]] = &lex_block_basic::A2k;
	(m_transition_table[m_states["H1"]])[m_sym_lexems["sym_rbrace"]] = &lex_block_basic::A3c;
	(m_transition_table[m_states["H1"]])[m_sym_lexems["sym_dot"]] = &lex_block_basic::D6a;
	(m_transition_table[m_states["H1"]])[m_sym_lexems["sym_space"]] = &lex_block_basic::H1;
	(m_transition_table[m_states["H1"]])[m_sym_lexems["sym_lf"]] = &lex_block_basic::A1a;
	(m_transition_table[m_states["H1"]])[m_sym_lexems["sym_eof"]] = &lex_block_basic::EXIT2;
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
	m_reg_order -= (long)m_reg_counter;
	calc_constant();
}

void lex_block_basic::DA2D()
{
	m_reg_order -= (long)m_reg_counter;
	m_reg_class = m_lexems["l_operand"];
	calc_constant();
}

void lex_block_basic::DA3D()
{
	if (m_reg_sign == -1)
	{
		m_reg_order *= -1;
	}
	m_reg_order -= (long)m_reg_counter;
	calc_constant();
}

State lex_block_basic::A1a()
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
	m_reg_class = m_lexems["l_aur_op"];
	m_reg_value = m_curr_sym.m_sym_value;
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
	m_reg_class = m_lexems["l_lbrace"];
	create_lexem();
	return A2();
}

State lex_block_basic::A2l()
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

State lex_block_basic::A2q()
{
	m_reg_class = m_lexems["l_end"];
	return A2b();
}

State lex_block_basic::A2r()
{
	m_reg_class = m_lexems["l_if"];
	return A2b();
}

State lex_block_basic::A2s()
{
	m_reg_class = m_lexems["l_return"];
	return A2b();
}

State lex_block_basic::A2g()
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

State lex_block_basic::A2p()
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
	if (m_curr_sym.m_sym_value == '=')
		m_reg_relation = 1;
	else if (m_curr_sym.m_sym_value == '<')
		m_reg_relation = 3;
	else if (m_curr_sym.m_sym_value == '>')
		m_reg_relation = 4;
	return A2h();
}

State lex_block_basic::A2t()
{
	m_reg_class = m_lexems["l_step"];
	return A2b();
}

State lex_block_basic::A2u()
{
	m_reg_class = m_lexems["l_to"];
	return A2b();
}

State lex_block_basic::A3a()
{
	m_reg_var_name += m_curr_sym.m_sym_value + '0';
	add_variable();
	create_lexem();
	return A3();
}

State lex_block_basic::A3c()
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

State lex_block_basic::A3d()
{
	DA1D();
	return A3c();
}

State lex_block_basic::A3b()
{
	m_reg_class = m_lexems["l_rbrace"];
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
	m_reg_detection = m_begin_vector[m_curr_sym.m_sym_value];
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
	m_reg_class = m_lexems["l_next"];
	return C1();
}

State lex_block_basic::C2a()
{
	m_reg_class = m_lexems["l_operand"];
	return C2d();
}

State lex_block_basic::C2d()
{
	m_reg_var_name = m_curr_sym.m_sym_value;

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

State lex_block_basic::M2()
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

State lex_block_basic::M3()
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

State lex_block_basic::EXIT1()
{
	m_reg_class = m_lexems["l_eof"];
	create_lexem();
	return m_states["STOP"];
}

State lex_block_basic::EXIT2()
{
	create_lexem();
	return EXIT1();
}

State lex_block_basic::H1c()
{
	DA1D();
	return H1b();
}

State lex_block_basic::H1b()
{
	if (m_reg_var_name != "")
		add_variable();
	create_lexem();
	return H1a();
}

State lex_block_basic::H1a()
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

State lex_block_basic::H1d()
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

State lex_block_basic::H1e()
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

State lex_block_basic::H1f()
{
	DA1E();
	return H1b();
}

State lex_block_basic::G1a()
{
	m_reg_class = m_lexems["l_rem"];
	create_lexem();
	return G1();
}

State lex_block_basic::E1a()
{
	m_reg_class = m_lexems["l_goto"];
	return E1();
}

State lex_block_basic::E1b()
{
	m_reg_class = m_lexems["l_gosub"];
	return E1();
}

State lex_block_basic::E2a()
{
	m_reg_class = m_lexems["l_line_num"];
	m_reg_line_num = m_curr_sym.m_sym_value;
	return E2b();
}

State lex_block_basic::E2b()
{
	m_reg_line_num = m_curr_sym.m_sym_value;
	return E2();
}

State lex_block_basic::E2c()
{
	m_reg_line_num = m_reg_line_num * 10 + m_curr_sym.m_sym_value;
	return E2();
}

State lex_block_basic::D1c()
{
	create_lexem();
	return D1a();
}

State lex_block_basic::D1a()
{
	m_reg_class = m_lexems["l_operand"];
	m_reg_number = m_curr_sym.m_sym_value;
	return D1();
}

State lex_block_basic::D1b()
{
	m_reg_number = m_reg_number * 10 + m_curr_sym.m_sym_value;
	return D1();
}

State lex_block_basic::D2a()
{
	m_reg_counter++;
	m_reg_number = m_reg_number * 10 + m_curr_sym.m_sym_value;
	return D2();
}

State lex_block_basic::D2b()
{
	m_reg_class = m_lexems["l_operand"];
	m_reg_counter = 1;
	m_reg_number = m_curr_sym.m_sym_value;
	return D2();
}

State lex_block_basic::D2c()
{
	m_reg_counter = 0;
	return D2();
}

State lex_block_basic::D3a()
{
	m_reg_counter = 0;
	return D3();
}

State lex_block_basic::D4a()
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

State lex_block_basic::D5a()
{
	m_reg_sign = +1;
	return D5b();
}

State lex_block_basic::D5b()
{
	m_reg_order = m_curr_sym.m_sym_value;
	return D5();
}

State lex_block_basic::D5c()
{
	m_reg_order = m_reg_order * 10 + m_curr_sym.m_sym_value;
	return D5();
}

State lex_block_basic::D6a()
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


State lex_block_basic::F1a()
{
	m_reg_class = m_lexems["l_let"];
	return F1();
}
State lex_block_basic::F1b()
{
	m_reg_class = m_lexems["l_for"];
	return F1();
}

State lex_block_basic::F2a()
{
	m_reg_var_name = static_cast<char>(m_curr_sym.m_sym_value);
	return F2();
}
State lex_block_basic::F3a()
{
	m_reg_var_name += m_curr_sym.m_sym_value + '0';
	return F3();
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
	default: {return ""; }
	}
}


State lex_block_basic::ERROR1()
{
	m_curr_sym = transliterator(m_input_file.get());

	m_reg_class = m_lexems["l_error"];
	create_lexem();
	if (m_curr_sym.m_sym_id == 9) // eof
	{
		return EXIT1();
	}

	if (m_curr_sym.m_sym_id == 10)
	{
		return m_states["A1"];
	}

	return m_curr_state;
	//while (m_curr_sym.m_sym_id != 9 && m_curr_sym.m_sym_id != 10)
	//{
	//	m_curr_sym = transliterator(m_input_file.get());
	//}
	//if (m_curr_sym.m_sym_id == 9) // eof
	//{
	//	return EXIT1();
	//}
	//if (m_curr_sym.m_sym_id == 10)
	//{
	//	return m_states["A1"];
	//}
}

void lex_block_basic::print_lexem_list()
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
			std::cout << (reinterpret_cast<variable*>(std::get<1>(*it))->var_name) << " ";
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
			std::cout << "NEXT " << (reinterpret_cast<variable*>(std::get<1>(*it))->var_name) << " ";
			break;
		}
		case 5: //let
		{
			std::cout << "LET " << (reinterpret_cast<variable*>(std::get<1>(*it))->var_name) << " =" << " ";
			break;
		}
		case 6: //for
		{
			std::cout << "FOR " << (reinterpret_cast<variable*>(std::get<1>(*it))->var_name) << " =" << " ";
			break;
		}
		case 7: //goto
		{
			std::cout << "GOTO " << (std::get<1>(*it)) << " ";
			break;
		}
		case 8: //gosub
		{
			std::cout << "GOSUB " << (std::get<1>(*it)) << " ";
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

void lex_block_basic::calc_constant()
{
	m_reg_number = m_reg_number * std::powl(10.0, (long double)m_reg_order);
	add_constant();
	m_reg_number = 0;
	m_reg_counter = 0;
	m_reg_order = 0;
}

void lex_block_basic::add_constant()
{
	std::ostringstream str_stream;
	str_stream << m_reg_number;
	std::string const_id = str_stream.str();
	variable var;
	var.var_name = const_id;
	var.var_value = m_reg_number;
	m_name_table.emplace(std::pair<std::string, variable>(const_id, var));
	m_reg_name_table_pointer = reinterpret_cast<long long int>(&m_name_table[const_id]);

}

void lex_block_basic::add_variable()
{
	variable var;
	var.var_name = m_reg_var_name;
	var.var_value = 0;
	m_name_table.emplace(std::pair<std::string, variable>(m_reg_var_name, var));
	m_reg_name_table_pointer = reinterpret_cast<long long int>(&m_name_table[m_reg_var_name]);
	m_reg_var_name = "";
}

void lex_block_basic::parse()
{
	while (m_curr_state.m_id != m_states["STOP"].m_id)
	{
		m_curr_sym = transliterator(m_input_file.get());

		m_curr_state = (this->*m_transition_table[m_curr_state][m_curr_sym])();
	}
}

void lex_block_basic::create_lexem()
{
	if (m_reg_class.m_lex_id == 0 /*line*/ || m_reg_class.m_lex_id == 2 /*aur oper*/ || m_reg_class.m_lex_id == 7 /*goto*/ || m_reg_class.m_lex_id == 8 /*gosub*/)
	{
		std::tuple<Lexem, long long int, int> tup(m_reg_class, m_reg_value, m_reg_line_num);
		m_lexem_list.push_back(tup);
	}
	else if (m_reg_class.m_lex_id == 1 /*operand*/ || m_reg_class.m_lex_id == 4 /*next*/ || m_reg_class.m_lex_id == 5 /*let*/ || \
		m_reg_class.m_lex_id == 6 /*for*/)
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