#pragma once

#include <vector>
#include "Permutation.h"
#include "SBlock.h"
#include "Splitter.h"
#include "Joiner.h"

class FeistelFunction
{
public:
   FeistelFunction()
   {
      const uint8_t cntExpP = 48;
      
      // ������� ����������� ������������ �����
      uint8_t tableEP[cntExpP] =
      {
         32, 1 , 2 , 3 , 4 , 5 , 4 , 5 , 6 , 7 , 8 , 9 ,
         8 , 9 , 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
         16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
         24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1 ,
      };
   
      const uint8_t cntLastPerm = 32;
      // ������� ����������� ������������ �����
      uint8_t tableLastPerm[cntLastPerm] = {
         16, 7 , 20, 21, 29, 12, 28, 17, 1 , 15, 23, 26, 
         5 , 18, 31, 10, 2 , 8 , 24, 14, 32, 27, 3 , 9 , 
         19, 13, 30, 6 , 22, 11, 4 , 25,
      };

      // ������� S-������
      uint8_t tableSBox[cntSBlocks][CNT_ROWS_SBOX][CNT_COL_SBOX] =
      {
         { // 0
            {14, 4 , 13, 1 , 2 , 15, 11, 8 , 3 , 10, 6 , 12, 5 , 9 , 0 , 7 },
            {0 , 15, 7 , 4 , 14, 2 , 13, 1 , 10, 6 , 12, 11, 9 , 5 , 3 , 8 },
            {4 , 1 , 14, 8 , 13, 6 , 2 , 11, 15, 12, 9 , 7 , 3 , 10, 5 , 0 },
            {15, 12, 8 , 2 , 4 , 9 , 1 , 7 , 5 , 11, 3 , 14, 10, 0 , 6 , 13},
         },
         { // 1
            {15, 1 , 8 , 14, 6 , 11, 3 , 4 , 9 , 7 , 2 , 13, 12, 0 , 5 , 10},
            {3 , 13, 4 , 7 , 15, 2 , 8 , 14, 12, 0 , 1 , 10, 6 , 9 , 11, 5 },
            {0 , 14, 7 , 11, 10, 4 , 13, 1 , 5 , 8 , 12, 6 , 9 , 3 , 2 , 15},
            {13, 8 , 10, 1 , 3 , 15, 4 , 2 , 11, 6 , 7 , 12, 0 , 5 , 14, 9 },
         },
         { // 2
            {10, 0 , 9 , 14, 6 , 3 , 15, 5 , 1 , 13, 12, 7 , 11, 4 , 2 , 8 },
            {13, 7 , 0 , 9 , 3 , 4 , 6 , 10, 2 , 8 , 5 , 14, 12, 11, 15, 1 },
            {13, 6 , 4 , 9 , 8 , 15, 3 , 0 , 11, 1 , 2 , 12, 5 , 10, 14, 7 },
            {1 , 10, 13, 0 , 6 , 9 , 8 , 7 , 4 , 15, 14, 3 , 11, 5 , 2 , 12},
         },
         { // 3
            {7 , 13, 14, 3 , 0 , 6 , 9 , 10, 1 , 2 , 8 , 5 , 11, 12, 4 , 15},
            {13, 8 , 11, 5 , 6 , 15, 0 , 3 , 4 , 7 , 2 , 12, 1 , 10, 14, 9 },
            {10, 6 , 9 , 0 , 12, 11, 7 , 13, 15, 1 , 3 , 14, 5 , 2 , 8 , 4 },
            {3 , 15, 0 , 6 , 10, 1 , 13, 8 , 9 , 4 , 5 , 11, 12, 7 , 2 , 14},
         },
         { // 4
            {2 , 12, 4 , 1 , 7 , 10, 11, 6 , 8 , 5 , 3 , 15, 13, 0 , 14, 9 },
            {14, 11, 2 , 12, 4 , 7 , 13, 1 , 5 , 0 , 15, 10, 3 , 9 , 8 , 6 },
            {4 , 2 , 1 , 11, 10, 13, 7 , 8 , 15, 9 , 12, 5 , 6 , 3 , 0 , 14},
            {11, 8 , 12, 7 , 1 , 14, 2 , 13, 6 , 15, 0 , 9 , 10, 4 , 5 , 3 },
         },
         { // 5
            {12, 1 , 10, 15, 9 , 2 , 6 , 8 , 0 , 13, 3 , 4 , 14, 7 , 5 , 11},
            {10, 15, 4 , 2 , 7 , 12, 9 , 5 , 6 , 1 , 13, 14, 0 , 11, 3 , 8 },
            {9 , 14, 15, 5 , 2 , 8 , 12, 3 , 7 , 0 , 4 , 10, 1 , 13, 11, 6 },
            {4 , 3 , 2 , 12, 9 , 5 , 15, 10, 11, 14, 1 , 7 , 6 , 0 , 8 , 13},
         },
         { // 6
            {4 , 11, 2 , 14, 15, 0 , 8 , 13, 3 , 12, 9 , 7 , 5 , 10, 6 , 1 },
            {13, 0 , 11, 7 , 4 , 9 , 1 , 10, 14, 3 , 5 , 12, 2 , 15, 8 , 6 },
            {1 , 4 , 11, 13, 12, 3 , 7 , 14, 10, 15, 6 , 8 , 0 , 5 , 9 , 2 },
            {6 , 11, 13, 8 , 1 , 4 , 10, 7 , 9 , 5 , 0 , 15, 14, 2 , 3 , 12},
         },
         { // 7
            {13, 2 , 8 , 4 , 6 , 15, 11, 1 , 10, 9 , 3 , 14, 5 , 0 , 12, 7 },
            {1 , 15, 13, 8 , 10, 3 , 7 , 4 , 12, 5 , 6 , 11, 0 , 14, 9 , 2 },
            {7 , 11, 4 , 1 , 9 , 12, 14, 2 , 0 , 6 , 10, 13, 15, 3 , 5 , 8 },
            {2 , 1 , 14, 7 , 4 , 10, 8 , 13, 15, 12, 9 , 0 , 3 , 5 , 6 , 11},
         },
      };

      _expP = new Permutation(tableEP, cntExpP);
      _lastPerm = new Permutation(tableLastPerm, 
         cntLastPerm);

      for (size_t i = 0; i < cntSBlocks; i++)
      {
         sblock[i] = new SBlock(tableSBox[i]);
      }
   }
   //------------------------------------------------------

   ~FeistelFunction()
   {
      delete _lastPerm;
      delete _expP;
   }
   //------------------------------------------------------

   /*���������� ������� ��������
   * 
   * ��������� ���� block � ���� key
   * 
   * ��������� ���������� ����*/
   uint32_t Action(uint32_t block, uint64_t key)
   {
      uint64_t expBlock = _expP->Process(block);
      expBlock ^= key;
      block = substitutions(expBlock);
      return _lastPerm->Process(block);
   }
   //------------------------------------------------------

private:
   static const uint8_t cntSBlocks = 8;
   Permutation* _expP, * _lastPerm;
   SBlock* sblock[cntSBlocks];
   Splitter splitter;
   Joiner joiner;

   /*�������������� ����� S-�������
   * 
   * ��������� ���� block
   * 
   * ���������� ����, ����� ���������� S-������*/
   uint32_t substitutions(uint64_t block)
   {
      const uint8_t maxBit = 48, cntBitsSubBlock = 6,
         cntBitsAfterSBlock = 4;
      std::vector<uint8_t> subblocks = splitter.
         Split(block, cntSBlocks, maxBit, cntBitsSubBlock);

      std::vector<uint8_t>comprBlocks;
      for (size_t i = 0; i < cntSBlocks; i++)
      {
         comprBlocks.push_back(sblock[i]->Action(subblocks[i]));
      }

      return joiner.Join(comprBlocks, cntBitsAfterSBlock);
   }
};