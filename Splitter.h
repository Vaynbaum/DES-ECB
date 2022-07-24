#pragma once

#include <vector>

class Splitter
{
public:
	/*��������� 64������ ���� �� ��� 32������ �����
	* 
	* ��������� block64b 64������ ���� � ��� ������ 
	* �� 32������ ����� left � right
	* 
	* ������ �� ����������*/
	void Split(uint64_t block64b, uint32_t& left, 
		uint32_t& right)
	{
		left = (uint32_t)(block64b >> 32);
		right = (uint32_t)(block64b);
	}
	//------------------------------------------------------

	/*��������� 64������ ���� �� ��������� ������
	* 
	* ��������� 64������ ���� block, 
	* ���������� ��������� cntPart, 
	* ������������ ���������� ����� 64�������� ����� maxBit,
	* ���������� ����� � �������� cntBitsSubBlock
	* 
	* ���������� ������ ���������*/
	std::vector<uint8_t> Split(uint64_t block, 
		uint8_t cntPart, uint8_t maxBit, 
		uint8_t cntBitsSubBlock)
	{
		std::vector<uint8_t> subblocks;
		uint8_t mask = (1 << 6) - 1;
		for (size_t i = 0; i < cntPart; i++)
		{
			// ��������� ������������ ����� ���������
			uint64_t necPartBit = block >> 
				(maxBit - cntBitsSubBlock) - 
				i * cntBitsSubBlock;

			subblocks.push_back(necPartBit & mask);
		}
		return subblocks;
	}
};