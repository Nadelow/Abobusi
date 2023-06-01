#pragma once
#include "sinth_block.h"

class Code_generator
{
	std::map<size_t, ATOM> m_atom_sequence;
	std::map<std::string, operand> m_operands;
public:
	Code_generator(std::map<std::string, operand> name_table, std::map<size_t, ATOM> atoms) : m_atom_sequence(atoms), m_operands(name_table) {	}
	~Code_generator() {	}
	void Generate(std::string filename = "output.txt");
private:
	void translate_expression(std::ofstream& out_file, std::list<long long int> atributes);
};

