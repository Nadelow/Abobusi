#include "BaseSymbolClass.h"


BaseSymbol::BaseSymbol(size_t id_, std::string name_)
{
   m_id = id_;
   m_name = name_;
}

bool operator==(const BaseSymbol& A_, const BaseSymbol& B_)
{
   if (A_.m_id == B_.m_id)
   {
      return true;
   }

   return false;
}

bool operator<(const BaseSymbol& A_, const BaseSymbol& B_)
{
   if (A_.m_id < B_.m_id)
   {
      return true;
   }

   return false;
}


int main()
{
   BaseSymbol MEM;
   BaseSymbol MEM2;

   MEM.m_id = 64;
   MEM2.m_id = 5;

   int cool = 0;

   if (MEM < MEM2)
   {
      cool = 1;
   }

   printf("%d", cool);

   return 0;
}

