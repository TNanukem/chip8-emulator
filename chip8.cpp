#include "chip8.h"
// Chip-8 Class Methods Definition

unsigned char chip8_fontset[80] = {
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

void chip8::initialize(){

	// Clears the memory of the system
	for(int i = 0; i < 4096; i++){
		this->memory[i] = 0;
	}

	// Clears all registers and the stack
	for(int i = 0; i < 16; i++){
		this->v[i] = 0;
		this->stack[i] = 0;
	}

	// Clears the display
	for(int i = 0; i < 2048; i++){
		this->display[i] = 0;
	}

	// Uploads the fontset to the Memory
	for(int i = 0; i < 80; i++){
		this->memory[i] = chip8_fontset[i];
	}

	this->opcode = 0;
	this->pc = 0x200;
	this->sp = 0;
	this->I = 0;

	this->soundTimer = 0;
	this->delayTimer = 0;

	this->drawFlag = 1;

}

bool chip8::loadGame(char *game){
    printf("Loading the game....\n");
	FILE *gameP = fopen(game, "rb");
	if(gameP == NULL){
        printf("ERROR! Game file not found!\n");
		return false;
	}

	// Finds the size of the game
	fseek(gameP, 0, SEEK_END);
	long int gameSize = ftell(gameP);
	fseek(gameP, 0, SEEK_SET);

	// Reads the data from the game into the buffer
	char *buffer = (char*) malloc(sizeof(char)*gameSize);
	if(buffer == NULL){
		return false;
	}
	fread(buffer, 1, gameSize, gameP);

	// Writes the data on the memory
	for(int i = 0; i < gameSize; i++){
		this->memory[i + 512] = buffer[i];
        printf("Memory[%d] = %d\n", i+512,memory[i+512]);
	}

	fclose(gameP);
	free(buffer);

}

void chip8::nextCycle(){

	// Fetching the instruction
	this->opcode = this->memory[this->pc] << 8 | this->memory[this->pc+1];

	// Decoding the instruction
	switch(opcode & GET_4BITS){					// Decode the instruction based on the first 4 bits

		// Decodification of the 0x0 opcodes
		case 0x0000:
			switch(opcode & GET_8BIT_CONSTANT){

				// Clears the screen.
				case 0x00E0:
					for(int i = 0; i < 2048; i++){
						this->display[i] = 0;
					}
					this->pc += 2;
					this->drawFlag = 1;
					break;

				// Returns from a subroutine.
				case 0x00EE:
					this->sp--;
					this->pc = this->stack[this->sp];
					break;
			}
			break;

		// Jumps to the address NNN
		case 0x1000:
			this->pc = (opcode & GET_ADDRESS);
			break;

        // Calls subroutine at NNN.
		case 0x2000:
			this->stack[this->sp] = pc;
			this->sp++;
			pc = opcode & GET_ADDRESS;
			break;

		// Skips the next instruction if v[X] is equal to NN
		case 0x3000:
			if(v[(opcode & GET_X) >> 8] == (opcode & GET_8BIT_CONSTANT)){
					this->pc += 4;
			}
			else this->pc += 2;
			break;

		// Skips the next instruction if v[X] is not equal to NN
		case 0x4000:
			if(v[(opcode & GET_X) >> 8] != (opcode & GET_8BIT_CONSTANT)){
					this->pc += 4;
			}
			else this->pc += 2;
			break;

		// Skips the next instruction if v[X] is equal to v[Y]
		case 0x5000:
			if(v[(opcode & GET_X) >> 8] == v[(opcode & GET_Y) >> 4]){
				this->pc += 4;
			}
			else
				this->pc += 2;
			break;

		// Sets v[X] to NN
		case 0x6000:
			v[(opcode & GET_X) >> 8] = (opcode & GET_8BIT_CONSTANT);
			this->pc += 2;
			break;

		// Adds NN to v[X]
		case 0x7000:
			v[(opcode & GET_X) >> 8]  += (opcode & GET_8BIT_CONSTANT);
			this->pc += 2;
			break;

		// Skips the next instruction if v[X] is not equal to v[Y]
		case 0x9000:
			if(v[opcode & GET_X >> 8] != v[(opcode & GET_Y) >> 4]){
				this->pc += 4;
			}
			else this->pc += 2;
			break;

		case 0x8000:
			switch(opcode & GET_4BIT_CONSTANT){

				// Sets v[X] to v[Y]
				case 0x0000:
					v[(opcode & GET_X) >> 8] = v[(opcode & GET_Y) >> 4];
					this->pc += 2;
					break;

				// Sets v[X] to v[X] OR v[Y]
				case 0x0001:
					v[(opcode & GET_X) >> 8] |= v[(opcode & GET_Y) >> 4];
					this->pc += 2;
					break;

				// Sets v[X] to v[X] AND v[Y]
				case 0x0002:
					v[(opcode & GET_X) >> 8] &= v[(opcode & GET_Y) >> 4];
					this->pc += 2;
					break;

				// Sets v[X] to v[X] XOR v[Y]
				case 0x0003:
					v[(opcode & GET_X) >> 8] ^= v[(opcode & GET_Y) >> 4];
					this->pc += 2;
					break;

				// Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
				case 0x0004:
					if(v[(opcode & GET_X) >> 8] > 0xFF - v[(opcode & GET_Y) >> 4])
						v[0xF] = 1;
					else
						v[0xF] = 0;
					v[(opcode & GET_X) >> 8] += v[(opcode & GET_Y) >> 4];
					this->pc += 2;

					break;

				// VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
				case 0x0005:
					if(v[(opcode & GET_Y) >> 4] > v[(opcode & GET_X) >> 8])
						v[0xF] = 0;
					else
						v[0xF] = 1;
					v[(opcode & GET_X) >> 8] -= v[(opcode & GET_Y) >> 4];
					this->pc += 2;
					break;

				// Stores the least significant bit of VX in VF and then shifts VX to the right by 1.
				case 0x0006:
					v[0xF] = v[(opcode & GET_X) >> 8] >> 7;
					v[(opcode & GET_X) >> 8] >> 1;
					this->pc += 2;
					break;

				// Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
				case 0x0007:
					if(v[(opcode & GET_X) >> 8] > v[(opcode & GET_Y) >> 4])
						v[0xF] = 0;
					else
						v[0xF] = 1;
					v[(opcode & GET_X) >> 8] = v[(opcode & GET_Y) >> 4] - v[(opcode & GET_X) >> 8];
					this->pc += 2;
					break;

				// Stores the most significant bit of VX in VF and then shifts VX to the left by 1.
				case 0x000E:
					v[0xF] = v[(opcode & GET_X) >> 8] >> 7;
					v[(opcode & GET_X) >> 8] << 1;
					this->pc += 2;
					break;
			}
			break;

		// Sets the I address to NNN
		case 0xA000:
			this->I = opcode & GET_ADDRESS;
			this->pc += 2;
			break;

		// Jumps to the address V0 + NNN
		case 0xB000:
			this->pc = (opcode & GET_ADDRESS) + this->v[0];
			this->pc += 2;
			break;

		// Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.
		case 0xC000:
			v[opcode & GET_X] = (rand() % 256) & (opcode & GET_8BIT_CONSTANT);
			this->pc += 2;
			break;

		// Draws a sprite at (vx,vy) with size height of N pixels
		case 0xD000:
			//draw(v[opcode & GET_X],v[opcode & GET_Y], (opcode & GET_4BIT_CONSTANT));
			this->pc += 2;
			break;

		case 0xE000:
			switch(opcode & GET_4BIT_CONSTANT){

				// Skips the next instruction if the key stored in VX is pressed.
				case 0x000E:
					if(this->keyboard[v[(opcode & GET_X) >> 8]] != 0)
						this->pc += 4;
					else
						this->pc += 2;
					break;

				// Skips the next instruction if the key stored in VX isn't pressed.
				case 0x0001:
					if(this->keyboard[v[(opcode & GET_X) >> 8]] == 0)
						this->pc += 4;
					else
						this->pc += 2;
					break;
			}

			break;

		case 0xF000:
			switch(opcode & GET_8BIT_CONSTANT){

				// Sets VX to the value of the delay timer.
				case 0x0007:
					v[(opcode & GET_X) >> 8] = this->delayTimer;
					this->pc += 2;
					break;

                // A key press is awaited, and then stored in VX
				case 0x000A:
					bool pressedKey = false;
					for(int i = 0; i < 16; i++){
						if(this->keyboard[i] != 0){
							v[(opcode & GET_X) >> 8] = i;
							pressedKey = true;
						}
						if(!pressedKey)
							//return

						this->pc += 2;
                        break;

				// Sets the delay timer to the value of v[X]
				case 0x0015:
					this->delayTimer = v[(opcode & GET_X) >> 8];
					this->pc += 2;
					break;

				// Sets the sound timer to the value of v[X]
				case 0x0018:
					this->soundTimer = v[(opcode & GET_X) >> 8];
					this->pc += 2;
					break;

				// Adds v[X] to I
				case 0x001E:
					this->I += v[(opcode & GET_X) >> 8];
					this->pc += 2;
					break;

				// Sets I to the location of the sprite for the character in VX.
				case 0x0029:
					this->I = v[(opcode & GET_X) >> 8] * 0x5;
					this->pc += 2;
					break;

				// Stores the Binary-coded decimal representation of VX at the addresses I, I plus 1, and I plus 2
				// Implementation by TJA
				case 0x0033:
						this->memory[I]     = V[(opcode & 0x0F00) >> 8] / 100;
						this->memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
						this->memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
						this->pc += 2;
					break;

				//Stores V0 to VX (including VX) in memory starting at address I.
				case 0x0055:
					for(int i = 0; i <= v[(opcode & GET_X) >> 8]; i++){
						this->memory[I+i] = v[i];
					}
					this->pc += 2;
					break;

				// Fills V0 to VX (including VX) with values from memory starting at address I.
				case 0x0065:
					for(int i = 0; i <= v[(opcode & GET_X) >> 8]; i++){
						v[i] = this->memory[I+i];
					}
					this->pc += 2;
					break;

			}
	}

	// Decrement the timers if they are greater than zero
	if(this->delayTimer > 0)
		this->delayTimer--;

	if(this->soundTimer > 0)
		this->soundTimer--;
}
