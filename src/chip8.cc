#include "chip8.hh"

Chip8::Chip8() {

}

Chip8::~Chip8() {

}

void Chip8::loadROM(const uint8_t* data, size_t size) {
    std::memcpy(&memory[0x200], data, size);
}

void Chip8::emulateCycle() {

}