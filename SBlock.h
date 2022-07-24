#pragma once

#include <vector>
#define CNT_ROWS_SBOX 4
#define CNT_COL_SBOX 16

// S-блок для функции Фейстеля
class SBlock
{
public:
   SBlock(uint8_t table[CNT_ROWS_SBOX][CNT_COL_SBOX])
   {
      for (size_t i = 0; i < CNT_ROWS_SBOX; i++)
      {
         std::vector<uint8_t> tmp;
         for (size_t j = 0; j < CNT_COL_SBOX; j++)
         {
            tmp.push_back(table[i][j]);
         }
         _table.push_back(tmp);
      }
   }
   //------------------------------------------------------

   ~SBlock()
   {
      for (size_t i = 0; i < CNT_ROWS_SBOX; i++)
      {
         _table[i].clear();
      }
      _table.clear();
   }
   //------------------------------------------------------

   /*Изменение 6-битового блока в 4-битовый блок
   * 
   * Получает 6-битовый блок block6b
   * 
   * Возвращает 4-битовый блок*/
   uint8_t Action(uint8_t block6b)
   {
      uint8_t posRow = extremeBits(block6b);
      uint8_t posCol = middleBits(block6b);
      return _table[posRow][posCol];
   }
   //------------------------------------------------------

private:
   std::vector<std::vector<uint8_t>> _table;
   
   /*Получение крайних битов блока
   * 
   * Получает 6-битовый блок block6b
   * 
   * Возвращает крайние биты*/
   uint8_t extremeBits(uint8_t block6b)
   {
      return  (block6b >> 5) | (block6b & 1);
   }
   //------------------------------------------------------

   /*Получение средних битов блока
   *
   * Получает 6-битовый блок block6b
   *
   * Возвращает средние биты*/
   uint8_t middleBits(uint8_t block6b)
   {
      uint8_t mask = 30;
      return (block6b & mask) >> 1;
   }
   //------------------------------------------------------
};