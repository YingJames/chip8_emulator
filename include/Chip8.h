//
// Created by james on 5/8/24.
//

#ifndef CHIP8_EMULATOR_CHIP8_H
#define CHIP8_EMULATOR_CHIP8_H


class Chip8
{
public:
    unsigned char memory[4096];
    unsigned char V[16];
    unsigned short stack[16];
    unsigned char gfx[64 * 32];
    unsigned short opcode;
    unsigned short I;
    unsigned short pc;
    unsigned short sp;


    unsigned char delay_timer;
    unsigned char sound_timer;

    void initialize();
    void emulateCycle();
    void execOpcode0xANNN();

};


#endif //CHIP8_EMULATOR_CHIP8_H
