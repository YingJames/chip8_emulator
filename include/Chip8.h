//
// Created by james on 5/8/24.
//

#ifndef CHIP8_EMULATOR_CHIP8_H
#define CHIP8_EMULATOR_CHIP8_H


class Chip8
{
public:
    uint8_t memory[4096];
    uint8_t V[16];
    uint16_t stack[16];
    uint8_t gfx[64 * 32];
    uint16_t opcode;
    uint16_t I;
    uint16_t pc;
    uint16_t sp;


    uint8_t delay_timer;
    uint8_t sound_timer;

    void initialize();
    void emulateCycle();
    void loadROM(std::string filename);

    // return 1 if true, 0 if false
    int isKeyPressed(uint8_t key);
    bool isKeyPressed(uint8_t key);

    void execOpcode0x0NNN();

    // Clear the screen
    void execOpcode0x00E0();

    // Return from a subroutine
    void execOpcode0x00EE();

    // Jump to address NNN
    void execOpcode0x1NNN();

    // Execute subroutine at address NNN
    void execOpcode0x2NNN();

    // Skip the following (or next) instruction if the VX == NN
    void execOpcode0x3XNN();

    // Skip the following (or next) instruction if the VX != NN
    void execOpcode0x4XNN();

    // Skip the following (or next) instruction if the VX == VY
    void execOpcode0x5XY0();

    // Store value of NN in register VX register
    void execOpcode0x6XNN();

    // Add value of NN to VX register
    void execOpcode0x7XNN();

    // Store the value of VY in VX register
    void execOpcode0x8XY0();

    // Set register VX to VX OR VY
    void execOpcode0x8XY1();

    // Set register VX to VX AND VY
    void execOpcode0x8XY2();

    // Set register VX to VX XOR VY
    void execOpcode0x8XY3();

    // Add VY value to register VX with carry value set to VF
    void execOpcode0x8XY4();

    // Subtract VY value to register VX with borrow value set to VF
    void execOpcode0x8XY5();

    // store right shifted VY to register VX with lsbit of VY set to VF
    void execOpcode0x8XY6();

    // store VY - VX to register VX with borrow value set to VF
    void execOpcode0x8XY7();

    // store left shifted VY to register VX with msbit of VY set to VF
    void execOpcode0x8XYE();

    // Skip the following (or next) instruction if the VX == VY
    void execOpcode0x9XY0();

    // Store memory address NNN in register I
    void execOpcode0xANNN();

    // Jump to address NNN + V0
    void execOpcode0xBNNN();

    // Set VX to a random number with a mask of NN
    void execOpcode0xCXNN();

    // Draw a sprite at VX, VY with N bytes and VF for collision
    void execOpcode0xDXYN();

    //
    void execOpcode0xEX9E();
    void execOpcode0xEXA1();
    void execOpcode0xFX07();
    void execOpcode0xFX0A();
    void execOpcode0xFX15();
    void execOpcode0xFX18();
    void execOpcode0xFX1E();
    void execOpcode0xFX29();
    void execOpcode0xFX33();
    void execOpcode0xFX55();
    void execOpcode0xFX65();

};


#endif //CHIP8_EMULATOR_CHIP8_H
