#pragma once

#include <chrono>
#include <random>
#include <cstdint>
#include <array>

/*
    Reference document : https://austinmorlan.com/posts/chip8_emulator/
*/

class Chip8 {
public:
    Chip8();
    ~Chip8();

    void loadROM(const uint8_t* data, const size_t size);
    void emulateCycle();

    //chip8 screen resolution is 64x32
    const std::array<uint8_t, 64 * 32>& getDisplay() const;
private:
    static const uint32_t START_ADDRESS;
    static const uint32_t FONTSET_SIZE;
    static const uint32_t FONTSET_START_ADDRESS;
    static uint8_t fontset[];

    std::array<uint8_t, 4096> memory; // 4kb = 4096 bytes
    std::array<uint8_t, 16> registers{ }; // 16 8-bit registers
    
    std::default_random_engine              rand_generator;
    std::uniform_int_distribution<uint8_t>  rand_byte;

    uint16_t I = 0;
    uint16_t pc = START_ADDRESS; // program counter starts at 0x200
    
    std::array<uint8_t, 16> stack{ }; // 16 levels of stack
    uint8_t sp = 0; // stack pointer

    uint8_t delay_timer = 0;
    uint8_t sound_timer = 0;

    std::array<uint8_t, 64 * 32> video{ }; // 64x32 pixel display
    uint16_t opcode;

    /* 
        Set of opcodes
    */
    void OP_00E0();
    void OP_00EE();
    void OP_1NNN();
    void OP_2NNN();
    void OP_3XKK();
    void OP_4XKK();
    void OP_5XY0();
    void OP_6XKK();
    void OP_7XKK();
    void OP_8XY0();
    
    void OP_8XY1();
    void OP_8XY2();
    void OP_8XY3();
    void OP_8XY4();
    void OP_8XY5();
    void OP_8XY6();
    void OP_8XY7();
    
    void OP_8XYE();
    void OP_9XY0();
    void OP_ANNN();
    void OP_BNNN();
    void OP_CXKK();
    void OP_DXYN();
    void OP_EX9E();
    void OP_EXA1();
    void OP_FX07();
    void OP_FX0A();
    void OP_FX15();
    void OP_FX18();
    void OP_FX1E();
    void OP_FX29();
    void OP_FX33();
    void OP_FX55();
    void OP_FX65();
};