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

void BF_grammar::sinth_analize()
{
	std::stack<std::shared_ptr<Symbol>> stack;
	Grammar_rule ready_to_wrap;
	stack.push(Dollar);
	auto curr_symb = m_in_word.begin();
	while (curr_symb != m_in_word.end())
	{
		switch (m_BF_table[*stack.top()][**curr_symb])
		{
		case Less:
			stack.push(*curr_symb);
			std::cout << "COPY " << (*curr_symb)->m_name << std::endl;
			++curr_symb;
			break;
		case Equal:
			stack.push(*curr_symb);
			std::cout << "COPY " << (*curr_symb)->m_name << std::endl;
			++curr_symb;
			break;
		case More:
			if (stack.top()->m_id == get_id("[S]") && *(*curr_symb) == *Dollar)
			{
				std::cout << "GOOD";
				return;
			}
			while (true)
			{
				auto top = stack.top();
				ready_to_wrap.m_right_part.insert(ready_to_wrap.m_right_part.begin(), top);
				stack.pop();
				if (m_BF_table[*stack.top()][*top] == Less)
				{
					auto find = m_sorted_by_right_part.find(ready_to_wrap);
					std::cout << find->rule_number << ")" << find->m_non_terminal->m_name << " -> ";
					for (auto& vect : ready_to_wrap.m_right_part)
						std::cout << vect->m_name << " ";
					std::cout << std::endl;
					ready_to_wrap.m_non_terminal = std::make_shared<Symbol>(find->m_non_terminal->m_name, find->m_non_terminal->m_id);
					ready_to_wrap.rule_number = find->rule_number;
					stack.push(process_wrap(ready_to_wrap));
					ready_to_wrap.m_right_part.clear();
					break;
				}
			}
			break;
		case None:
			std::cout << "error" << std::endl;
			return;
		}
	}
}

std::shared_ptr<Symbol> BF_grammar::process_wrap(Grammar_rule& rule)
{
	ATOM atom;
	switch (rule.rule_number)
	{
	case 1: //[S] -> [PROGRAM]

		break;
	case 2: //[PROGRAM] -> LINE_NUM [OPERATOR] [PROGRAM]
		atom.type = (ATOM::ATOM_TYPE)rule.m_right_part[1]->m_atributes.front();
		rule.m_right_part[1]->m_atributes.pop_front();
		rule.m_right_part[1]->m_atributes.pop_front();
		atom.atributs = rule.m_right_part[1]->m_atributes;
		if (atom.type == ATOM::FOR || atom.type == ATOM::FOR_STEP)
		{
			ATOM next;
			next.type = ATOM::NEXT;
			next.atributs.push_back(atom.atributs.back());
			atom.atributs.pop_back();
			atom.atributs.pop_back();
			m_atom_output.emplace(atom.atributs.back(), next);
			atom.atributs.pop_back();
		}
		m_atom_output.emplace(rule.m_right_part[0]->m_atributes.front(), atom);
		break;
	case 3: //[PROGRAM] -> LINE_NUM [OPERATOR]
		atom.type = (ATOM::ATOM_TYPE)rule.m_right_part[1]->m_atributes.front();
		rule.m_right_part[1]->m_atributes.pop_front();
		rule.m_right_part[1]->m_atributes.pop_front();
		atom.atributs = rule.m_right_part[1]->m_atributes;
		if (atom.type == ATOM::FOR || atom.type == ATOM::FOR_STEP)
		{
			ATOM next;
			next.type = ATOM::NEXT;
			next.atributs.push_back(atom.atributs.back());
			atom.atributs.pop_back();
			atom.atributs.pop_back();
			m_atom_output.emplace(atom.atributs.back(), next);
			atom.atributs.pop_back();
		}
		m_atom_output.emplace(rule.m_right_part[0]->m_atributes.front(), atom);
		break;
	case 4: //[LINE_NUM] -> LINE_NUM
		rule.m_non_terminal->m_atributes = rule.m_right_part[0]->m_atributes;
		break;
	case 5: //[OPERATOR] -> LET [E]			
		rule.m_non_terminal->m_atributes.push_back(ATOM::LET);
		rule.m_non_terminal->m_atributes.push_back(-1);
		rule.m_non_terminal->m_atributes.push_back(rule.m_right_part[0]->m_atributes.front());
		rule.m_non_terminal->m_atributes.push_back(-1);
		rule.m_non_terminal->m_atributes = rule.m_non_terminal->m_atributes + rule.m_right_part[1]->m_atributes;
		break;
	case 6: //[OPERATOR] -> [GOTO]
		rule.m_non_terminal->m_atributes.push_back(ATOM::GOTO);
		rule.m_non_terminal->m_atributes.push_back(-1);
		rule.m_non_terminal->m_atributes = rule.m_right_part[0]->m_atributes;
		break;
	case 7: //[OPERATOR] -> IF [TEST] GOTO
		rule.m_non_terminal->m_atributes.push_back(ATOM::IF_GOTO);
		rule.m_non_terminal->m_atributes.push_back(-1);
		rule.m_non_terminal->m_atributes = rule.m_non_terminal->m_atributes + rule.m_right_part[1]->m_atributes;
		rule.m_non_terminal->m_atributes.push_back(-1);
		rule.m_non_terminal->m_atributes = rule.m_non_terminal->m_atributes + rule.m_right_part[2]->m_atributes;
		break;
	case 8: //[TEST] -> [E] REL [E]
		rule.m_non_terminal->m_atributes = rule.m_right_part[0]->m_atributes;
		rule.m_non_terminal->m_atributes.push_back(-1);
		rule.m_non_terminal->m_atributes = rule.m_non_terminal->m_atributes + rule.m_right_part[1]->m_atributes;
		rule.m_non_terminal->m_atributes.push_back(-1);
		rule.m_non_terminal->m_atributes = rule.m_non_terminal->m_atributes + rule.m_right_part[2]->m_atributes;
		break;
	case 9: //[OPERATOR] -> [FOR1] [NEXT]
		rule.m_non_terminal->m_atributes.push_back(ATOM::FOR_STEP);
		rule.m_non_terminal->m_atributes.push_back(-1);
		rule.m_non_terminal->m_atributes = rule.m_non_terminal->m_atributes + rule.m_right_part[0]->m_atributes;
		rule.m_non_terminal->m_atributes.push_back(-1);
		rule.m_non_terminal->m_atributes = rule.m_non_terminal->m_atributes + rule.m_right_part[1]->m_atributes;
		break;
	case 10: //[NEXT] -> NEXT
		rule.m_non_terminal->m_atributes = rule.m_right_part[0]->m_atributes;
		break;
	case 11: //[FOR1] -> [FOR3] [PROGRAM]
		rule.m_non_terminal->m_atributes = rule.m_right_part[0]->m_atributes;
		break;
	case 12: //[FOR3] -> FOR [E] TO [E] STEP [E]
		rule.m_non_terminal->m_atributes = rule.m_right_part[0]->m_atributes;
		rule.m_non_terminal->m_atributes.push_back(-1);
		rule.m_non_terminal->m_atributes = rule.m_non_terminal->m_atributes + rule.m_right_part[1]->m_atributes;
		rule.m_non_terminal->m_atributes.push_back(-1);
		rule.m_non_terminal->m_atributes = rule.m_non_terminal->m_atributes + rule.m_right_part[3]->m_atributes;
		rule.m_non_terminal->m_atributes.push_back(-1);
		rule.m_non_terminal->m_atributes = rule.m_non_terminal->m_atributes + rule.m_right_part[5]->m_atributes;
		break;
	case 13: //[OPERATOR] -> [FOR2] [NEXT]
		rule.m_non_terminal->m_atributes.push_back(ATOM::FOR);
		rule.m_non_terminal->m_atributes.push_back(-1);
		rule.m_non_terminal->m_atributes = rule.m_non_terminal->m_atributes + rule.m_right_part[0]->m_atributes;
		rule.m_non_terminal->m_atributes.push_back(-1);
		rule.m_non_terminal->m_atributes = rule.m_non_terminal->m_atributes + rule.m_right_part[1]->m_atributes;
		break;
	case 14: //[FOR2] -> [FOR4] [PROGRAM]
		rule.m_non_terminal->m_atributes = rule.m_right_part[0]->m_atributes;
		break;
	case 15://[FOR4] -> FOR [E] TO [E]			
		rule.m_non_terminal->m_atributes = rule.m_right_part[0]->m_atributes;
		rule.m_non_terminal->m_atributes.push_back(-1);
		rule.m_non_terminal->m_atributes = rule.m_non_terminal->m_atributes + rule.m_right_part[1]->m_atributes;
		rule.m_non_terminal->m_atributes.push_back(-1);
		rule.m_non_terminal->m_atributes = rule.m_non_terminal->m_atributes + rule.m_right_part[3]->m_atributes;
		break;
	case 16: //[OPERAND] -> OPERAND
		rule.m_non_terminal->m_atributes = rule.m_right_part[0]->m_atributes;
		break;
	case 17: //[OPERATOR] -> REM
		rule.m_non_terminal->m_atributes.push_back(ATOM::REM);
		rule.m_non_terminal->m_atributes.push_back(-1);
		break;
	case 18: //[OPERATOR] -> GOSUB
		rule.m_non_terminal->m_atributes.push_back(ATOM::GOSUB);
		rule.m_non_terminal->m_atributes.push_back(-1);
		rule.m_non_terminal->m_atributes.push_back(rule.m_right_part[0]->m_atributes.front());
		break;
	case 19: //[OPERATOR] -> RETURN
		rule.m_non_terminal->m_atributes.push_back(ATOM::RETURN);
		rule.m_non_terminal->m_atributes.push_back(-1);
		rule.m_non_terminal->m_atributes.push_back(rule.m_right_part[0]->m_atributes.front());
		break;
	case 20: //[OPERATOR] -> END
		rule.m_non_terminal->m_atributes.push_back(ATOM::END);
		rule.m_non_terminal->m_atributes.push_back(-1);
		rule.m_non_terminal->m_atributes.push_back(rule.m_right_part[0]->m_atributes.front());
		break;
	case 21: //[E] ->[T]
		rule.m_non_terminal->m_atributes = rule.m_right_part[0]->m_atributes;
		break;
	case 22: //[E] ->[T][E']
		rule.m_non_terminal->m_atributes = rule.m_right_part[0]->m_atributes;
		rule.m_non_terminal->m_atributes = rule.m_non_terminal->m_atributes + rule.m_right_part[1]->m_atributes;
		break;
	case 23: //[E'] -> + [T] [E']
		rule.m_non_terminal->m_atributes = rule.m_right_part[1]->m_atributes;
		rule.m_non_terminal->m_atributes.push_back(rule.m_right_part[0]->m_atributes.front());
		rule.m_non_terminal->m_atributes = rule.m_non_terminal->m_atributes + rule.m_right_part[2]->m_atributes;
		break;
	case 24: //[E'] -> - [T] [E']
		rule.m_non_terminal->m_atributes = rule.m_right_part[1]->m_atributes;
		rule.m_non_terminal->m_atributes.push_back(rule.m_right_part[0]->m_atributes.front());
		rule.m_non_terminal->m_atributes = rule.m_non_terminal->m_atributes + rule.m_right_part[2]->m_atributes;
		break;
	case 25: //[E'] -> + [T]
		rule.m_non_terminal->m_atributes = rule.m_right_part[1]->m_atributes;
		rule.m_non_terminal->m_atributes.push_back(rule.m_right_part[0]->m_atributes.front());
		break;
	case 26: //[E'] -> - [T]
		rule.m_non_terminal->m_atributes = rule.m_right_part[1]->m_atributes;
		rule.m_non_terminal->m_atributes.push_back(rule.m_right_part[0]->m_atributes.front());
		break;
	case 27: //[T] -> [F]
		rule.m_non_terminal->m_atributes = rule.m_right_part[0]->m_atributes;
		break;
	case 28: //[T] -> [F] [T']
		rule.m_non_terminal->m_atributes = rule.m_right_part[0]->m_atributes;
		rule.m_non_terminal->m_atributes = rule.m_non_terminal->m_atributes + rule.m_right_part[1]->m_atributes;
		break;
	case 29: //[T'] -> * [F] [T']
		rule.m_non_terminal->m_atributes = rule.m_right_part[1]->m_atributes;
		rule.m_non_terminal->m_atributes.push_back(rule.m_right_part[0]->m_atributes.front());
		rule.m_non_terminal->m_atributes = rule.m_non_terminal->m_atributes + rule.m_right_part[2]->m_atributes;
		break;
	case 30: //[T'] -> / [F] [T']
		rule.m_non_terminal->m_atributes = rule.m_right_part[1]->m_atributes;
		rule.m_non_terminal->m_atributes.push_back(rule.m_right_part[0]->m_atributes.front());
		rule.m_non_terminal->m_atributes = rule.m_non_terminal->m_atributes + rule.m_right_part[2]->m_atributes;
		break;
	case 31: //[T'] -> ^ [F] [T']
		rule.m_non_terminal->m_atributes = rule.m_right_part[1]->m_atributes;
		rule.m_non_terminal->m_atributes.push_back(rule.m_right_part[0]->m_atributes.front());
		rule.m_non_terminal->m_atributes = rule.m_non_terminal->m_atributes + rule.m_right_part[2]->m_atributes;
		break;
	case 32: //[T'] -> * [F]
		rule.m_non_terminal->m_atributes = rule.m_right_part[1]->m_atributes;
		rule.m_non_terminal->m_atributes.push_back(rule.m_right_part[0]->m_atributes.front());
		break;
	case 33: //[T'] -> / [F]
		rule.m_non_terminal->m_atributes = rule.m_right_part[1]->m_atributes;
		rule.m_non_terminal->m_atributes.push_back(rule.m_right_part[0]->m_atributes.front());
		break;
	case 34: //[T'] -> ^ [F]
		rule.m_non_terminal->m_atributes = rule.m_right_part[1]->m_atributes;
		rule.m_non_terminal->m_atributes.push_back(rule.m_right_part[0]->m_atributes.front());
		break;
	case 35: //[F] -> ( [E] )
		rule.m_non_terminal->m_atributes = rule.m_right_part[1]->m_atributes;
		break;

	case 36: //[F] -> [OPERAND]
		rule.m_non_terminal->m_atributes = rule.m_right_part[0]->m_atributes;
		break;
	default:
		break;
	}

	return rule.m_non_terminal;
}
