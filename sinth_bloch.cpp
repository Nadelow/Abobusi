#include "sinth_block.h"

template<class T> inline
    std::list<T> operator+(const std::list<T>& first, const std::list<T>& second)
{
    std::list<T> newList(first.cbegin(), first.cend());
    newList.insert(newList.end(), second.cbegin(), second.cend());
    return newList;
}

bool BF_grammar::EQUAL(std::shared_ptr<Symbol> A, std::shared_ptr<Symbol> B)
{
    for (auto &it : m_rules)
    {
        for (auto first = it.second.m_right_part.begin(), second = (++it.second.m_right_part.begin()); second != it.second.m_right_part.end() && first != it.second.m_right_part.end(); ++first, ++second)
        {
            if (*A == *(*first) && *B == *(* second))
                return true;
        }
    }

    return false;
}

bool BF_grammar::MORE(std::shared_ptr<Symbol> A, std::shared_ptr<Symbol> B)
{
    bool res = false;

    for (auto &it : m_rules)
        for (auto first = it.second.m_right_part.begin(), second = (++it.second.m_right_part.begin()); second != it.second.m_right_part.end() && first != it.second.m_right_part.end(); ++first, ++second)
        {
            bool B_in_FIRST1_of_second = false;                  //Флаг говорит, что В входит в FIRST1 от second
            auto FIRST1_of_second = FIRST1(* second);             //FIRST1(*ыусщтв)
            for (auto& that : FIRST1_of_second)
            {
                if (that->m_id == B->m_id)
                    B_in_FIRST1_of_second = true;
            }

            if (((dynamic_cast<Terminal*>(B.get())) && *B == *(*second)) || B_in_FIRST1_of_second && (dynamic_cast<Non_terminal*>(first->get())))
            {
                auto first_end = END1(*first);
                for (auto& that : first_end)
                {
                    if (that->m_id == A->m_id)
                        res = true;
                }
            }
        }

    return res;
}

bool BF_grammar::LESS(std::shared_ptr<Symbol> A, std::shared_ptr<Symbol> B)
{
   bool res = false;
   for (auto& it : m_rules)
      for (auto first = it.second.m_right_part.begin(), second = (++it.second.m_right_part.begin()); second != it.second.m_right_part.end() && first != it.second.m_right_part.end(); ++first, ++second)
         if (*A == *(*first) && (dynamic_cast<Non_terminal*>(second->get())))
         {
            auto sec_start1 = START1(*second);
            for (auto& that : sec_start1)
               if (that->m_id == B->m_id)
                  res = true;
         }

   return res;
}

void BF_grammar::sort_by_right_part()
{
	for (auto& it : m_rules)
	{
		if (!m_sorted_by_right_part.insert(it.second).second)
			std::cout << "rules are not unique" << std::endl;
	}
}

void BF_grammar::fill_symbol_list(std::list<std::tuple<Lexem, long long int, size_t>> lexem_list)
{
	std::shared_ptr<Symbol> curr_sym;
	long long int converted_ptr;
	std::list<std::tuple<Lexem, long long int, size_t>>::iterator that;
	for (auto it = lexem_list.begin(); it != lexem_list.end(); ++it)
	{
		curr_sym = std::make_shared<Terminal>();
		curr_sym->m_id = std::get<0>(*it).m_lex_id;
		converted_ptr = std::get<1>(*it);
		curr_sym->m_atributes.push_back(converted_ptr);
		switch (std::get<0>(*it).m_lex_id)
		{
		case 0:
			that = it;
			if (std::get<0>(*(++that)).m_lex_id == 4)
				continue;
			curr_sym->m_name = "LINE_NUM";
			break;
		case 1:
			curr_sym->m_name = "OPERAND";
			break;
		case 2:
			switch (std::get<1>(*it))
			{
			case '+':
				curr_sym->m_name = '+';
				curr_sym->m_id = 19;
				break;
			case '-':
				curr_sym->m_name = '-';
				curr_sym->m_id = 20;
				break;
			case '*':
				curr_sym->m_name = '*';
				curr_sym->m_id = 21;
				break;
			case '/':
				curr_sym->m_name = '/';
				curr_sym->m_id = 22;
				break;
			case '^':
				curr_sym->m_name = '^';
				curr_sym->m_id = 2;
				break;
			}
			break;
		case 3:
			curr_sym->m_name = "REL";
			break;
		case 4:
			curr_sym->m_name = "NEXT";
			curr_sym->m_atributes.push_front(-1);
			curr_sym->m_atributes.push_front(std::get<2>(*it));
			break;
		case 5:
			curr_sym->m_name = "LET";
			break;
		case 6:
			curr_sym->m_name = "FOR";
			break;
		case 7:
			curr_sym->m_name = "GOTO";
			break;
		case 8:
			curr_sym->m_name = "GOSUB";
			break;
		case 9:
			curr_sym->m_name = "(";
			break;
		case 10:
			curr_sym->m_name = ")";
			break;
		case 11:
			curr_sym->m_name = "IF";
			break;
		case 12:
			curr_sym->m_name = "RETURN";
			break;
		case 13:
			curr_sym->m_name = "END";
			break;
		case 14:
			curr_sym->m_name = "TO";
			break;
		case 15:
			curr_sym->m_name = "STEP";
			break;
		case 16:
			curr_sym->m_name = "REM";
			break;
		case 17:
			curr_sym->m_name = "!error!";
			break;
		case 18:
			curr_sym->m_name = "EOF";
			continue;
			break;
		}
		m_in_word.push_back(curr_sym);
	}
	m_in_word.push_back(Dollar);
}
