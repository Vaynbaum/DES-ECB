#pragma once
#include <string>
#include "FeistelNetwork.h"
#include "Permutation.h"
#include "Key.h"

class Cipher
{
public:
   /*���� - ������������� �����
   * � ���� ����������*/
   struct PairTxtKey
   {
      std::string key;
      std::string txt;
   };
   //------------------------------------------------------

   Cipher()
   {
      // ���������� ��������� ������������
      uint8_t ipTable[] =
      {
         58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36,
         28, 20, 12, 4, 62, 54, 46, 38, 30, 22, 14, 6,
         64, 56, 48, 40, 32, 24, 16, 8, 57, 49, 41, 33,
         25, 17, 9 , 1, 59, 51, 43, 35, 27, 19, 11, 3,
         61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39,
         31, 23, 15, 7,
      };

      // ���������� �������� ������������ 
      uint8_t epTable[] =
      {
         40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15,
         55, 23, 63, 31, 38, 6, 46, 14, 54, 22, 62, 30,
         37, 5, 45, 13, 53, 21, 61, 29, 36, 4, 44, 12,
         52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
         34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9 ,
         49, 17, 57, 25,
      };
      uint8_t cntBits = 64;      // ���������� ��� ��� 
                                 // �������������

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

   /*���������� ������
   *
   * ��������� �������� ����� srcTxt
   *
   * ���������� ����: �����-����*/
   PairTxtKey Encrypt(std::string srcTxt)
   {
      // ��������� ����� �� ��������� ������
      transformationText(srcTxt);

      // ��������� ������
      std::string encTxt, key = _key.GenerationKey();
      std::vector<uint64_t> keys = _key.KeyExpansion(
         convertStrToInt(key, 0));

      // ���������� ������ ������
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

   /*������������� ������
   *
   * ��������� ����: �����-���� pair
   *
   * ���������� �������������� ���������*/
   std::string Decrypt(PairTxtKey pair)
   {
      std::string decTxt;
      // ��������� ������
      std::vector<uint64_t> keys = _key.KeyExpansion(
         convertStrToInt(pair.key, 0));

      // ������������� ������ ������
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

   /*��������� ����� �� ��������� ������
   * 
   * ��������� ������ �� ����� src
   * 
   * ������ �� ����������*/
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

   /*����������� �������� ���� � ���������
   * 
   * ��������� �������� ���� block
   * 
   * ���������� ��������� ����*/
   std::string convertIntToStr(uint64_t block)
   {
      std::string txt;
      uint64_t mask = 0xFF00000000000000;
      
      // ���������� ������ 
      for (size_t i = 0; i < byte; i++)
      {
         // ���������� ������� �� �����
         unsigned char ch = uint64_t(block & mask) >>
            (byte * (byte - 1 - i));
         mask >>= byte;
         txt += ch;
      }
      return txt;
   }
   //------------------------------------------------------


   /*����������� ��������� ���� � ��������
   * 
   * ��������� ��������� ���� str � 
   * ��������� ������� start
   *
   * ���������� �������� ����*/
   uint64_t convertStrToInt(std::string str, size_t start)
   {
      uint64_t block = 0;
      // ���������� ������ � ��������� �������
      for (size_t i = start; i < start + byte; i++)
      {
         block <<= byte;
         unsigned char ch = str[i];
         block |= ch;
      }
      return block;
   }
};

