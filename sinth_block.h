#pragma once
#include"CF_grammar.h"
#include "lex_block_basic.h"
#include <stack>

struct ATOM {
    enum ATOM_TYPE{ LET, IF_GOTO, FOR_STEP, FOR, LINE, GOTO, GOSUB, RETURN, END, REM, NEXT } type;
    std::list<long long int> atributs;
};

class BF_grammar : public CF_grammar
{

   void create_BF_table();
   std::shared_ptr<Symbol> Dollar;
   enum BF_RELATION { More, Less, Equal, None, MORE_LESS, MORE_EQ, LESS_EQ };
   std::map<Symbol, std::map<Symbol, BF_RELATION>> m_BF_table;
   std::set<Grammar_rule> m_sorted_by_right_part;
   std::list<std::shared_ptr<Symbol>> m_in_word;
   std::map<std::string, Lexem> m_lexems;
   std::map<size_t, ATOM> m_atom_output;
   BF_RELATION check_relation(std::shared_ptr<Symbol> A, std::shared_ptr<Symbol> B);
   void create_BF_table();
public:
   BF_grammar(const std::string filename, std::list<std::tuple<Lexem, long long int, size_t>> lexem_list = std::list<std::tuple<Lexem, long long int, size_t>>()) : CF_grammar(filename)
   {
      sort_by_right_part();
      Dollar = std::make_shared<Symbol>();
      Dollar->m_id = -1;
      Dollar->m_name = "$";
      create_BF_table();
      fill_symbol_list(lexem_list);
   }

private:
   bool EQUAL(std::shared_ptr<Symbol> A, std::shared_ptr<Symbol> B);
   bool MORE(std::shared_ptr<Symbol> A, std::shared_ptr<Symbol> B);
   bool LESS(std::shared_ptr<Symbol> A, std::shared_ptr<Symbol> B);
};
