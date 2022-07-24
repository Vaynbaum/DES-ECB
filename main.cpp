#include <ctime>
#include <iostream>
#include <cstdlib>

#include "Cipher.h"


int main()
{
   system("chcp 1251");
   srand(time(nullptr));

   Cipher cipher;
   std::cout << "Input text: ";
   std::string text;
   std::getline(std::cin, text);
   Cipher::PairTxtKey pair = cipher.Encrypt(text);
   std::cout << "Encrypted text: " << pair.txt << std::endl;
   std::string srcText = cipher.Decrypt(pair);
   std::cout << "Decrypted text: " << srcText;
}
