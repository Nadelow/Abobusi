#include "code_generator.h"

void Code_generator::Generate(std::string filename)
{
	std::ofstream out_file(filename);

	out_file << "section .bss\n";
	for (auto& var : m_operands)
	{
		if (var.first[0] < 'A' && var.first[0] > 'Z')
			continue;

		out_file << var.first << " DQ " << "?\n";
	}

	out_file << "section .text\n global _start:\n" << "_start:\n";

	out_file << "POW proc\n pop ebx\n pop ecx\n mov eax, 1\n";
	out_file << "POW_CYCLE:\n cmp ecx, 0\n jle POW_STOP\n mul ebx\n";
	out_file << "dec ecx\n jmp POW_CYCLE\n POW_STOP:\n ret\n endp\n";

	long long int temp;

	for (auto& curr_atom : m_atom_sequence)
	{
		switch (curr_atom.second.type)
		{
		case(ATOM::LINE):
			out_file << "LINE" << curr_atom.second.atributs.front() << ":\n";
			break;
		case(ATOM::LET):
			temp = curr_atom.second.atributs.front();
			curr_atom.second.atributs.pop_front();
			curr_atom.second.atributs.pop_front();
			translate_expression(out_file, curr_atom.second.atributs);
			out_file << "pop " << "eax " << std::endl;
			out_file << "mov " << reinterpret_cast<operand*>(temp)->op_name << " eax " << std::endl;
			break;
		case(ATOM::END):
			break;
		case(ATOM::FOR):
			break;
		case(ATOM::FOR_STEP):
			break;
		case(ATOM::GOTO):
			break;
		case(ATOM::GOSUB):
			break;
		case(ATOM::IF_GOTO):
			break;
		case(ATOM::NEXT):
			break;
		case(ATOM::REM):
			break;
		case(ATOM::RETURN):
			break;
		}
	}

}

void Code_generator::translate_expression(std::ofstream &out_file, std::list<long long int> atributes)
{
	for (auto& it : atributes)
	{
		switch (it)
		{
		case '-':
			out_file << "pop ebx \npop eax\n";
			out_file << "sub eax,ebx \npush eax\n";
			break;
		case '+':
			out_file << "pop ebx \npop eax\n";
			out_file << "add eax,ebx \npush eax\n";
			break;
		case '*':
			out_file << "pop ebx \npop eax\n";
			out_file << "mul eax,ebx \npush eax\n";
			break;
		case '/':
			out_file << "pop ebx \npop eax\n";
			out_file << "div eax,ebx \npush eax\n";
			break;
		case '^':
			out_file << "call POW \npush eax\n";
			break;
		default:
			out_file << "push " << reinterpret_cast<operand*>(it)->op_name << "\n";
			break;
		}
	}
}

