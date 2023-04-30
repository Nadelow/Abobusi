#pragma once
#include <string>
#include "BaseSymbolClass.h"

class Non_terminal : public BaseSymbol
{
public:
	Non_terminal(size_t id_, std::string name_) : BaseSymbol(id_, name_) {};

	bool operator== (const Non_terminal& other);

	bool operator< (const Non_terminal& other);
};