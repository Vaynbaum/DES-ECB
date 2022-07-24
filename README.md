# Cryptographic algorithm DES Electronic Code Book mode

The algorithm allows you to encrypt and decrypt the message.

***

# How to use

You need to import the `Cipher.h` file, create an `instance` of the `Cipher` class.

**File: main.cpp**
```c++
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
```

<img width="230" alt="image" src="https://user-images.githubusercontent.com/78900834/180643240-d28ebf79-407d-4bb7-a0a6-1eccc5d3d09b.png">
