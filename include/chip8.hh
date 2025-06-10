#pragma once
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
    static const uint32_t START_ADDRESS = 0x200;

    std::array<uint8_t, 4096> memory; // 4kb = 4096 bytes
    std::array<uint8_t, 16> V{ }; // 16 8-bit registers
    
    uint16_t I = 0;
    uint16_t pc = START_ADDRESS; // program counter starts at 0x200
    
    std::array<uint8_t, 16> stack{ }; // 16 levels of stack
    uint8_t sp = 0; // stack pointer

    uint8_t delay_timer = 0;
    uint8_t sound_timer = 0;

    std::array<uint8_t, 64 * 32> display{ }; // 64x32 pixel display
};