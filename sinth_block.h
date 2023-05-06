#pragma once
#include"CF_grammar.h"
#include"lex_block_basic.h"

class BF_grammar : public CF_grammar
{
	std::shared_ptr<Symbol> Dollar;
	enum BF_RELATION{More, Less, Equal, None, Error};
	std::map<std::shared_ptr<Symbol>, std::map<std::shared_ptr<Symbol>, BF_RELATION>> m_BF_table;
public:
	BF_grammar(std::string filename) : CF_grammar(filename) 
	{ 
		Dollar = std::make_shared<Symbol>();
		Dollar->m_id = -1;
		Dollar->m_name = "$";
		create_BF_table(); 
	}
	bool EQUAL(std::shared_ptr<Symbol> A, std::shared_ptr<Symbol> B);
	bool MORE(std::shared_ptr<Symbol> A, std::shared_ptr<Symbol> B);
	bool LESS(std::shared_ptr<Symbol> A, std::shared_ptr<Symbol> B);

	void print_BF_table();

private:
	void create_BF_table();
	BF_RELATION check_relation(std::shared_ptr<Symbol> A, std::shared_ptr<Symbol> B);
};