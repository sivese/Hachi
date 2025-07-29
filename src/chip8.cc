#include "chip8.hh"

const uint32_t Chip8::START_ADDRESS = 0x200;
const uint32_t Chip8::FONTSET_START_ADDRESS = 0x50;
const uint32_t Chip8::FONTSET_SIZE = 80;

uint8_t Chip8::fontset[FONTSET_SIZE] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip8::Chip8() {
    pc = START_ADDRESS;
}

Chip8::~Chip8() {

}

void Chip8::loadROM(const uint8_t* data, size_t size) {
    std::memcpy(&memory[0x200], data, size);
}

void Chip8::emulateCycle() {

}

void Chip8::OP_00E0() {
	this->video.fill(0); // CLS, Clear the display
}

void Chip8::OP_00EE() {
	--sp;
	pc = stack[sp]; // RET, Return from subroutine
}

void Chip8::OP_1NNN() {
	uint16_t address = opcode & 0x0FFFu;
	pc = address; // JP addr, Jump to location nnn
}

void Chip8::OP_2NNN() {
	uint16_t address = opcode & 0x0FFFu;
	
	stack[sp] = pc;
	++sp;
	pc = address; // CALL addr, Call subroutine at nnn
}

void Chip8::OP_3XKK() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	if(registers[Vx] == byte) {
		pc += 2;
	}
}

void Chip8::OP_4XKK() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	if(registers[Vx] != byte) {
		pc += 2; // SNE Vx, byte, Skip next instruction if Vx != byte
	}
}

void Chip8::OP_5XY0() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if(registers[Vx] == registers[Vy]) {
		pc += 2;
	} // SE Vx, Vy, Skip next instruction if Vx == Vy
}

void Chip8::OP_6XKK() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	registers[Vx] = byte; // LD Vx, byte, Set Vx = byte
}

void Chip8::OP_7XKK() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	registers[Vx] += byte; // ADD Vx, byte, Set Vx = Vx + byte
}

void Chip8::OP_8XY0() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	registers[Vx] = registers[Vy]; // LD Vx, Vy, Set Vx = Vy
}

void Chip8::OP_8XY1() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	registers[Vx] |= registers[Vy]; // OR Vx, Vy, Set Vx = Vx OR Vy
}

void Chip8::OP_8XY2() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	registers[Vx] &= registers[Vy]; // AND Vx, Vy, Set Vx = Vx AND Vy
}

void Chip8::OP_8XY3() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	registers[Vx] ^= registers[Vy]; // XOR Vx, Vy, Set Vx = Vx XOR Vy
}

void Chip8::OP_8XY4() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	uint16_t sum = registers[Vx] + registers[Vy];

	if (sum > 255U) {
		registers[0xF] = 1;
	}
	else {
		registers[0xF] = 0;
	}

	registers[Vx] = sum & 0xFFu;
}

void Chip8::OP_8XY5() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (registers[Vx] > registers[Vy])
	{
		registers[0xF] = 1;
	}
	else
	{
		registers[0xF] = 0;
	}

	registers[Vx] -= registers[Vy];
}

void Chip8::OP_8XY6() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	registers[0xF] = (registers[Vx] & 0x1u);

	registers[Vx] >>= 1;
}

void Chip8::OP_8XY7() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (registers[Vy] > registers[Vx])
	{
		registers[0xF] = 1;
	}
	else
	{
		registers[0xF] = 0;
	}

	registers[Vx] = registers[Vy] - registers[Vx];
}

void Chip8::OP_8XYE() {

}

void Chip8::OP_9XY0() {
	
}