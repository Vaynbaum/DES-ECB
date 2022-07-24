#pragma once

#include <vector>

// Перестановки битов в блоке
class Permutation
{
public:
   Permutation(uint8_t table[], uint8_t countBits)
   {
      bits = countBits;
      for (uint8_t i = 0; i < bits; i++)
      {
         _table.push_back(table[i]);
      }
   }
   //------------------------------------------------------

   ~Permutation()
   {
      _table.clear();
   }
   //------------------------------------------------------

   /*Осуществляет перестановку битов по инструкционной 
   * таблице
   * 
   * Принимает 64битный блок block
   * 
   * Возвраащет измененный блок*/
   uint64_t Process(uint64_t block)
   {
      uint64_t rearrangedBlock = 0;
      for (uint8_t i = 0; i < bits; i++)
      {
         uint64_t pos = uint64_t(1) << (_table[i] - 1);
         if (block & pos)
         {
            rearrangedBlock |= (uint64_t(1) << i);
         }
      }
      return rearrangedBlock;
   }
   //------------------------------------------------------

private:
   uint8_t bits;
   std::vector<uint8_t> _table;
};