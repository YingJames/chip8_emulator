#include <iostream>
#include "Chip8.h"

Chip8 Chip8;

int main()
{
//    std::cout << "Hello, World!" << std::endl;
    Chip8.initialize();
    Chip8.emulateCycle();

    return 0;
}
