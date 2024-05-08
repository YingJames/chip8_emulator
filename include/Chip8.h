//
// Created by james on 5/8/24.
//

#ifndef CHIP8_EMULATOR_CHIP8_H
#define CHIP8_EMULATOR_CHIP8_H


class Chip8
{
public:
    unsigned short opcode;
    unsigned char memory[4096];
    unsigned char V[16];
    unsigned short I;
    unsigned short pc;

    void initialize();
    void emulateCycle();

};


#endif //CHIP8_EMULATOR_CHIP8_H
