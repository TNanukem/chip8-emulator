#include "chip8.h"

class emulator {

    public:
    chip8 Chip8;
    display Display;
    uint8_t SaveStateSlot = 0;

    void saveGame(uint8_t SaveStateSlot, chip8* Chip8);
    chip8 loadGame(uint8_t SaveStateSlot, chip8* Chip8);
    void emulate();
};
