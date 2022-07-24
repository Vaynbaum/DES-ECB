#pragma once

#include <vector>

class Joiner
{
public:
	/*��������� ��� 28������ ����� � 64������ ����
	* 
	* ��������� ��� 28������ ����� left � right
	* 
	* ���������� 64������ ����*/
	uint64_t Join28bitsTo56bits(uint32_t left, 
		uint32_t right)
	{
		const uint8_t bitsSubBlock = 28;
		uint64_t joinedBlock = 0;
		uint32_t mask = (1 << bitsSubBlock) - 1;

		joinedBlock = left & mask;
		joinedBlock = (joinedBlock << bitsSubBlock) | 
			(right & mask);
		return joinedBlock;
	}
	//------------------------------------------------------

	/*��������� ��� 32������ ����� � 64������ ����
	*
	* ��������� ��� 32������ ����� left � right
	*
	* ���������� 64������ ����*/
	uint64_t Join32bitsTo64bits(uint32_t left, 
		uint32_t right)
	{
		uint64_t block64b = 0;
		block64b = (uint64_t)left;
		block64b = (uint64_t)(block64b << 32) | right;
		return block64b;
	}
	//------------------------------------------------------

	/*��������� �������� � ���� 32������ ����
	*
	* ��������� ������ ��������� parts � 
	* ���������� ����� � ��������
	*
	* ���������� 32������ ����*/
	uint32_t Join(std::vector<uint8_t> parts, 
		uint8_t cntBitsInPart)
	{
		uint32_t block = 0;
		for (size_t i = 0; i < parts.size(); i++)
		{
			block <<= cntBitsInPart;
			block |= parts[i];
		}
		return block;
	}
};