#pragma once

#include <vector>
#include "Splitter.h"
#include "Joiner.h"
#include "FeistelFunction.h"

class FeistelNetwork
{
public:
   //Режимы преобразования сети Фейстеля
   static enum Mode
   {
      Direct,
      Reverse
   };
   //------------------------------------------------------

   /*Преобразует блок сетью Фейстеля
   * 
   * Принимает блок block, вектор ключей keys 
   * и режим преобразования mode
   * 
   * Возвращает измененный блок*/
   uint64_t Conversion(uint64_t block, 
      std::vector<uint64_t> keys, Mode mode)
   {
      uint32_t left, right;
      splitter.Split(block, left, right);

      if (mode == Direct)
      {
         for (int8_t i = 0; i < cntRounds; i++)
         {
            roundFeistelCipher(left, right, keys[i]);
         }
      }
      else
      {
         for (int8_t i = cntRounds - 1; i >= 0; i--)
         {
            roundFeistelCipher(left, right, keys[i]);
         }
      }
      swap(left, right);
      return joiner.Join32bitsTo64bits(left, right);
   }
   //------------------------------------------------------

private:
   static const size_t cntRounds = 16;
   Splitter splitter;
   Joiner joiner;
   FeistelFunction func;

   /*Итерация сетью Фейстеля
   * 
   * Принимает ссылки на левый left и правый right 
   * блоки и ключ key 
   * 
   * Ничего не возвращает*/
   void roundFeistelCipher(uint32_t& left, 
      uint32_t& right, uint64_t key)
   {
      uint32_t temp = right;
      right = func.Action(right, key) ^ left;
      left = temp;
   }
   //------------------------------------------------------

   /*Меняет блоки местами
   * 
   * Принимает ссылки на блоки left и right
   * 
   * Ничего не возвращает*/
   void swap(uint32_t& left, uint32_t& right)
   {
      uint32_t temp = left;
      left = right;
      right = temp;
   }
};