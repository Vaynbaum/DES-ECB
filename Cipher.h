#pragma once
#include <string>
#include "FeistelNetwork.h"
#include "Permutation.h"
#include "Key.h"

class Cipher
{
public:
   /*Пара - зашифрованный текст
   * и ключ шифрования*/
   struct PairTxtKey
   {
      std::string key;
      std::string txt;
   };
   //------------------------------------------------------

   Cipher()
   {
      // Инструкции начальной перестановки
      uint8_t ipTable[] =
      {
         58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36,
         28, 20, 12, 4, 62, 54, 46, 38, 30, 22, 14, 6,
         64, 56, 48, 40, 32, 24, 16, 8, 57, 49, 41, 33,
         25, 17, 9 , 1, 59, 51, 43, 35, 27, 19, 11, 3,
         61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39,
         31, 23, 15, 7,
      };

      // Инструкции конечной перестановки 
      uint8_t epTable[] =
      {
         40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15,
         55, 23, 63, 31, 38, 6, 46, 14, 54, 22, 62, 30,
         37, 5, 45, 13, 53, 21, 61, 29, 36, 4, 44, 12,
         52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
         34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9 ,
         49, 17, 57, 25,
      };
      uint8_t cntBits = 64;      // Количество бит при 
                                 // перестановках

      _ip = new Permutation(ipTable, cntBits);
      _ep = new Permutation(epTable, cntBits);
   }
   //------------------------------------------------------

   ~Cipher()
   {
      delete _ip;
      delete _ep;
   }
   //------------------------------------------------------

   /*Шифрование текста
   *
   * Принимает исходный текст srcTxt
   *
   * Возвращает пару: текст-ключ*/
   PairTxtKey Encrypt(std::string srcTxt)
   {
      // Дополняет текст до кратности байтам
      transformationText(srcTxt);

      // Генерация ключей
      std::string encTxt, key = _key.GenerationKey();
      std::vector<uint64_t> keys = _key.KeyExpansion(
         convertStrToInt(key, 0));

      // Шифрование блоков текста
      for (size_t i = 0; i < srcTxt.size(); i += byte)
      {
         uint64_t tmpBlock = _ip->Process(
            convertStrToInt(srcTxt, i));
         tmpBlock = _network.Conversion(tmpBlock, keys,
            FeistelNetwork::Direct);
         tmpBlock = _ep->Process(tmpBlock);
         encTxt += convertIntToStr(tmpBlock);
      }
      PairTxtKey pair = { key, encTxt };
      return pair;
   }
   //------------------------------------------------------

   /*Расшифрование текста
   *
   * Принимает пару: текст-ключ pair
   *
   * Возвращает расшифрованное сообщение*/
   std::string Decrypt(PairTxtKey pair)
   {
      std::string decTxt;
      // Генерация ключей
      std::vector<uint64_t> keys = _key.KeyExpansion(
         convertStrToInt(pair.key, 0));

      // Расшифрование блоков текста
      for (size_t i = 0; i < pair.txt.size(); i += byte)
      {
         uint64_t tmpBlock = _ip->Process(
            convertStrToInt(pair.txt, i));

         tmpBlock = _network.Conversion(tmpBlock, keys,
            FeistelNetwork::Reverse);
         tmpBlock = _ep->Process(tmpBlock);
         decTxt += convertIntToStr(tmpBlock);
      }
      return decTxt;
   }
   //------------------------------------------------------

private:
   Permutation* _ip, * _ep;
   FeistelNetwork _network;
   Key _key;
   static const size_t byte = 8;

   /*Дополняет текст до кратности байтам
   * 
   * Принимает ссылку на текст src
   * 
   * Ничего не возвращает*/
   void transformationText(std::string& src)
   {
      size_t len = src.size();
      while (len % byte != 0)
      {
         src += unsigned char(0);
         len = src.size();
      }
   }
   //------------------------------------------------------

   /*Преобразует числовой блок в строковый
   * 
   * Принимает числовой блок block
   * 
   * Возвращает строковый блок*/
   std::string convertIntToStr(uint64_t block)
   {
      std::string txt;
      uint64_t mask = 0xFF00000000000000;
      
      // Побайтовый проход 
      for (size_t i = 0; i < byte; i++)
      {
         // Извлечение символа из блока
         unsigned char ch = uint64_t(block & mask) >>
            (byte * (byte - 1 - i));
         mask >>= byte;
         txt += ch;
      }
      return txt;
   }
   //------------------------------------------------------


   /*Преобразует строковый блок в числовой
   * 
   * Принимает строковый блок str и 
   * начальную позицию start
   *
   * Возвращает числовой блок*/
   uint64_t convertStrToInt(std::string str, size_t start)
   {
      uint64_t block = 0;
      // Побайтовый проход с начальной позиции
      for (size_t i = start; i < start + byte; i++)
      {
         block <<= byte;
         unsigned char ch = str[i];
         block |= ch;
      }
      return block;
   }
};

