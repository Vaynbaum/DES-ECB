#pragma once

#include <vector>
#include "Permutation.h"
#include "Joiner.h"

class Key
{
public:
   Key()
   {
      // Таблица перестановок левой части ключа
      uint8_t k1Table[cntBitsPartKey] =
      {
         57, 49, 41, 33, 25, 17, 9 , 1 , 58, 50, 
         42, 34, 26, 18, 10, 2 , 59, 51, 43, 35,
         27, 19, 11, 3 , 60, 52, 44, 36,
      };
      // Таблица перестановок правой части ключа
      uint8_t k2Table[cntBitsPartKey] =
      {
         63, 55, 47, 39, 31, 23, 15, 7 , 62, 54,
         46, 38, 30, 22, 14, 6 , 61, 53, 45, 37, 
         29, 21, 13, 5 , 28, 20, 12, 4 ,
      };

      const uint8_t cntBitsKeyComp = 48;
      
      // Сжимающая таблица перестановок ключа
      uint8_t keyCompTable[cntBitsKeyComp] =
      {
         14, 17, 11, 24, 1 , 5 , 3 , 28, 15, 6 , 21, 10,
         23, 19, 12, 4 , 26, 8 , 16, 7 , 27, 20, 13, 2 ,
         41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
         44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32,
      };

      _leftp = new Permutation(k1Table, cntBitsPartKey);
      _rightp = new Permutation(k2Table, cntBitsPartKey);
      _keyp = new Permutation(keyCompTable, 
         cntBitsKeyComp);
   }
   //------------------------------------------------------

   ~Key()
   {
      delete _leftp;
      delete _rightp;
      delete _keyp;
   }
   //------------------------------------------------------

   /*Получение ключей из одного ключа
   *
   * Принимает ключ key
   * 
   * Возвращает вектор ключей*/
   std::vector<uint64_t> KeyExpansion(uint64_t key)
   {
      std::vector<uint64_t> keys;
      uint32_t left = 0, right = 0;
      
      // Осуществляем перестановки в частях ключа
      left = _leftp->Process(key);
      right = _rightp->Process(key);

      // Получаем ключи для раундов сети Фейстеля
      for (size_t i = 0; i < cntRounds; i++)
      {
         left = leftCycleShift(left, _table[i]);
         right = leftCycleShift(right, _table[i]);
         uint64_t joinKey = joiner.
            Join28bitsTo56bits(left, right);

         keys.push_back(_keyp->Process(joinKey));
      }
      return keys;
   }
   //------------------------------------------------------

   /*Генерирует ключ длиной в 8 байт
   * 
   * Ничего не принимает
   * 
   * Возвращает ключ*/
   std::string GenerationKey()
   {
      const size_t maxBorder = 255, minBorder = 32;
      std::string key;

      for (size_t i = 0; i < byte; i++)
      {
         unsigned char ch = minBorder + rand() % 
            maxBorder;
         if (checkEven(ch))
         {
            ch += 1;
         }

         key += ch;
      }
      return key;
   }
   //------------------------------------------------------

private:
   Permutation* _leftp, * _rightp, * _keyp;
   Joiner joiner;
   static const size_t byte = 8;
   static const size_t cntRounds = 16;
   static const uint8_t cntBitsPartKey = 28;

   const uint8_t _table[cntRounds] = {
     1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
   };

   /*Проверка четности количества бит символа
   * 
   * Принимает символ ch
   * 
   * Возвращает true, если количество бит четно*/
   bool checkEven(unsigned char ch)
   {
      uint8_t cntOnes = 0;
      for (size_t i = 0; i < byte; i++)
      {
         if ((ch >> i) & 1)
         {
            cntOnes++;
         }
      }
      return cntOnes % 2 == 0;
   }
   //------------------------------------------------------

   /*Циклический свдиг числа влево
   * 
   * Принимает число num и количество шагов сдвига step
   * 
   * Возвращает смещенное число */
   uint32_t leftCycleShift(uint32_t num, uint8_t step)
   {
      uint32_t part1 = num << step;
      uint32_t part2 = num >> (cntBitsPartKey - step);
      return part1 | part2;
   }
};