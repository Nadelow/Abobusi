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
	for (auto &it : m_rules)
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

void BF_grammar::print_BF_table()
{
	std::pair<Symbol, Symbol> pair;
	std::set<std::pair<Symbol, Symbol>> equal;
	std::set<std::pair<Symbol, Symbol>> less;
	std::set<std::pair<Symbol, Symbol>> more;

	for (auto& it : m_BF_table)
	{
		/*
		std::cout << it.first->m_name << " |";
		for (auto tmp = it.second.begin(); it == *m_BF_table.begin() && tmp != it.second.end(); ++tmp)
			std::cout << tmp->first->m_name << " |";
		for (auto that : it.second)
		{
			switch (that.second)
			{
			case Equal:
				std::cout << " = |";
				break;
			case Less:
				std::cout << " < |";
				break;
			case More:
				std::cout << " > |";
				break;
			case None:
				std::cout << "   |";
				break;
			}
		}
		std::cout << std::endl;*/

		for (auto& that : it.second)
		{
			pair.first = it.first;
			pair.second = that.first;
			switch (that.second)
			{
			case Equal:
				equal.insert(pair);
				break;
			case Less:
				less.insert(pair);
				break;
			case More:
				more.insert(pair);
				break;
			case None:
				break;
			}
		}
	}

	std::cout << "LESS" << std::endl;
	for (auto& it : less)
	{
		std::cout << it.first.m_name << " < " << it.second.m_name << std::endl;
	}
	std::cout << "MORE" << std::endl;
	for (auto& it : more)
	{
		std::cout << it.first.m_name << " > " << it.second.m_name << std::endl;
	}
	std::cout << "EQUAL" << std::endl;
	for (auto& it : equal)
	{
		std::cout << it.first.m_name << " = " << it.second.m_name << std::endl;
	}


}

BF_grammar::BF_RELATION BF_grammar::check_relation(std::shared_ptr<Symbol> A, std::shared_ptr<Symbol> B)
{
	bool wasrelat = false;
	BF_RELATION relat_code = None;

	if (EQUAL(A, B))
	{
		wasrelat = true;
		relat_code = Equal;
	}
	if (LESS(A, B))
	{
		if (wasrelat)
			return LESS_EQ;
		wasrelat = true;
		relat_code = Less;
	}
	if (MORE(A, B))
	{
		if (wasrelat)
		{
			if (relat_code == Equal)
				return MORE_EQ;
			else
				return MORE_LESS;
		}
		relat_code = More;
	}

	return relat_code;
}

void BF_grammar::create_BF_table()
{
	for (auto& it : m_terminals)
		for (auto& that : m_terminals)
		{
			BF_RELATION rel = check_relation(it.second, that.second);
			/*if (rel == Error)
			{
				std::cout << "Relation conflict between " << it.second->m_name << " and " << that.second->m_name << std::endl;
			}*/

			switch (rel)
			{
			case LESS_EQ:
				std::cout << "Relation conflict between " << it.second->m_name << " and " << that.second->m_name << " < and == " << std::endl;
				break;
			case MORE_EQ:
				std::cout << "Relation conflict between " << it.second->m_name << " and " << that.second->m_name << " > and == " << std::endl;
				break;
			case MORE_LESS:
				std::cout << "Relation conflict between " << it.second->m_name << " and " << that.second->m_name << " > and < " << std::endl;
				break;
			default:
				m_BF_table[*it.second][*that.second] = rel;
				break;
			}
		}
	for (auto& it : m_non_terminals)
		for (auto& that : m_terminals)
		{
			BF_RELATION rel = check_relation(it.second, that.second);
			switch (rel)
			{
			case LESS_EQ:
				std::cout << "Relation conflict between " << it.second->m_name << " and " << that.second->m_name << " < and == " << std::endl;
				break;
			case MORE_EQ:
				std::cout << "Relation conflict between " << it.second->m_name << " and " << that.second->m_name << " > and == " << std::endl;
				break;
			case MORE_LESS:
				std::cout << "Relation conflict between " << it.second->m_name << " and " << that.second->m_name << " > and < " << std::endl;
				break;
			default:
				m_BF_table[*it.second][*that.second] = rel;
				break;
			}
		}
	for (auto& it : m_terminals)
		for (auto& that : m_non_terminals)
		{
			BF_RELATION rel = check_relation(it.second, that.second);
			switch (rel)
			{
			case LESS_EQ:
				std::cout << "Relation conflict between " << it.second->m_name << " and " << that.second->m_name << " < and == " << std::endl;
				break;
			case MORE_EQ:
				std::cout << "Relation conflict between " << it.second->m_name << " and " << that.second->m_name << " > and == " << std::endl;
				break;
			case MORE_LESS:
				std::cout << "Relation conflict between " << it.second->m_name << " and " << that.second->m_name << " > and < " << std::endl;
				break;
			default:
				m_BF_table[*it.second][*that.second] = rel;
				break;
			}
		}
	for (auto& it : m_non_terminals)
		for (auto& that : m_non_terminals)
		{
			BF_RELATION rel = check_relation(it.second, that.second);
			switch (rel)
			{
			case LESS_EQ:
				std::cout << "Relation conflict between " << it.second->m_name << " and " << that.second->m_name << " < and == " << std::endl;
				break;
			case MORE_EQ:
				std::cout << "Relation conflict between " << it.second->m_name << " and " << that.second->m_name << " > and == " << std::endl;
				break;
			case MORE_LESS:
				std::cout << "Relation conflict between " << it.second->m_name << " and " << that.second->m_name << " > and < " << std::endl;
				break;
			default:
				m_BF_table[*it.second][*that.second] = rel;
				break;
			}
		}
	

	/////////////////////////////Заполнение отношений с долларом//////////////////////////////////
	for (auto& it : m_terminals)
	{
		auto START_S = START1(m_non_terminals["[S]"]);
		bool in_start_s = false;
		for (auto& that : START_S)
			if (*that == *it.second)
				m_BF_table[*Dollar][*that] = Less;
	}
	for (auto& it : m_non_terminals)
	{
		auto START_S = START1(m_non_terminals["[S]"]);
		bool in_start_s = false;
		for (auto& that : START_S)
			if (*that == *it.second)
				m_BF_table[*Dollar][*that] = Less;
	}
	for (auto& it : m_terminals)
	{
		auto END_S = END1(m_non_terminals["[S]"]);
		bool in_END_s = false;
		for (auto that : END_S)
			if (*that == *it.second)
				m_BF_table[*that][*Dollar] = More;
	}
	for (auto& it : m_non_terminals)
	{
		auto END_S = END1(m_non_terminals["[S]"]);
		bool in_END_s = false;
		for (auto that : END_S)
			if (*that == *it.second)
				m_BF_table[*that][*Dollar] = More;
	}
}

void BF_grammar::sort_by_right_part()
{
	for (auto& it : m_rules)
	{
		if (!m_sorted_by_right_part.insert(it.second).second)
			std::cout << "rules are not unique" << std::endl;
	}

	
}


void BF_grammar::print_sorted_rules()
{
	for (auto& it : m_sorted_by_right_part)
	{
		std::cout << it.rule_number << ") " << it.m_non_terminal->m_name << " ";
		for (auto& that : it.m_right_part)
			std::cout << that->m_name << " ";
		std::cout << std::endl;
	}
}

void BF_grammar::print_FIRST_START_END()
{
	for (auto& it : m_non_terminals)
	{
		std::cout << "\n\n\nFIRST1 " << it.second->m_name << "\n";
		auto FIRST = this->FIRST1(it.second);
		for (auto& that : FIRST)
			std::cout << that->m_name << std::endl;

		std::cout << "\nSTART1 " << it.second->m_name << "\n";
		auto START = this->START1(it.second);
		for (auto& that : START)
			std::cout << that->m_name << std::endl;

		std::cout << "\nEND1 " << it.second->m_name << "\n";
		auto END = this->END1(it.second);
		for (auto& that : END)
			std::cout << that->m_name << std::endl;
	}
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
			/*std::cout << reinterpret_cast<operand*>(rule.m_right_part[0]->m_atributes.front())->op_name << "\n = \n";
			for (auto it : rule.m_right_part[1]->m_atributes)
			{
				if (it != 43 && it != 42)
					std::cout << reinterpret_cast<operand*>(it)->op_name << std::endl;
				else
					std::cout << (char)it << std::endl;
			}*/
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
			/*atom.type = ATOM::FOR;
			atom.atributs.push_back(rule.m_right_part[0]->m_atributes.front());
			atom.atributs = atom.atributs + rule.m_right_part[1]->m_atributes;
			atom.atributs.push_back(-1);
			atom.atributs = atom.atributs + rule.m_right_part[3]->m_atributes;*/
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



/*
Дополнить проверкой на наличие ровно одного end с наибольшим номером

*/

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
