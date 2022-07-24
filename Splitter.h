#pragma once

#include <vector>

class Splitter
{
public:
	/*Разделяет 64битный блок на два 32битных блока
	* 
	* Принимает block64b 64битный блок и две ссылки 
	* на 32битных блока left и right
	* 
	* Ничего не возвращает*/
	void Split(uint64_t block64b, uint32_t& left, 
		uint32_t& right)
	{
		left = (uint32_t)(block64b >> 32);
		right = (uint32_t)(block64b);
	}
	//------------------------------------------------------

	/*Разделяет 64битный блок на несколько частей
	* 
	* Принимает 64битный блок block, 
	* количество подблоков cntPart, 
	* максимальное количество битов 64битового блока maxBit,
	* количество битов в подблоке cntBitsSubBlock
	* 
	* Возвращает вектор подблоков*/
	std::vector<uint8_t> Split(uint64_t block, 
		uint8_t cntPart, uint8_t maxBit, 
		uint8_t cntBitsSubBlock)
	{
		std::vector<uint8_t> subblocks;
		uint8_t mask = (1 << 6) - 1;
		for (size_t i = 0; i < cntPart; i++)
		{
			// Получение необходимого блока смещением
			uint64_t necPartBit = block >> 
				(maxBit - cntBitsSubBlock) - 
				i * cntBitsSubBlock;

			subblocks.push_back(necPartBit & mask);
		}
		return subblocks;
	}
};