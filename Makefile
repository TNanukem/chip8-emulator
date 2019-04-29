all: chip8

chip8: chip8.cpp display.cpp emulator.cpp main.cpp
	g++ chip8.cpp display.cpp emulator.cpp main.cpp -lSDL2 -o chip8
