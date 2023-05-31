#pragma once
#include <string>

class BaseSymbol
{
public:

   size_t m_id;
   std::string m_name;

   BaseSymbol(size_t id_ = 0, std::string name_ = "");

   virtual ~BaseSymbol()
   {
   };

   friend bool operator ==(const BaseSymbol& A_, const BaseSymbol& B_);
   friend bool operator <(const BaseSymbol& A_, const BaseSymbol& B_);
};