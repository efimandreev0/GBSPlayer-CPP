#include "GeneralMemory.h"
#include "Cartridge.h"
#include "Sound.h"

GeneralMemory::GeneralMemory(Cartridge* cartridge, Sound* gbSound) 
    : _cartridge(cartridge), _gbSound(gbSound)
{
    Memory.resize(65536, 0);
}

void GeneralMemory::Init()
{
    std::fill(Memory.begin(), Memory.end(), 0);
}

uint8_t GeneralMemory::Read(int index)
{
    // Cartridge (ROM)
    if (index < 0x8000)
    {
        return _cartridge->Read(index);
    }
    
    // GBS Main RAM
    // Player authors: disregard writes to $4000-$5fff and $ff70, implement main RAM from $a000 to $dfff.
    if (index >= 0xA000 && index < 0xE000)
    {
        return Memory[index];
    }

    // Shadow RAM
    if (index >= 0xE000 && index < 0xFE00)
    {
        return Memory[index - 0x2000];
    }

    // Sound registries
    if (index >= 0xFF10 && index <= 0xFF3F)
    {
        return _gbSound->Read(index);
    }

    // High RAM (HRAM) & Interrupt Enable (0xFFFF)
    if (index >= 0xFF80 && index <= 0xFFFF)
    {
        return Memory[index];
    }

    std::cout << "Read at 0x" << std::hex << index << " not supported" << std::dec << std::endl;
    return 0;
}

void GeneralMemory::Write(int index, uint8_t value)
{
    // ROM (MBC Control)
    if (index < 0x8000)
    {
        _cartridge->Write(index, value);
    }
    // 8KB Video RAM (VRAM) - Not supported in GBS player
    else if (index < 0xA000)
    {
        // std::cout << "VRAM not supported 0x" << std::hex << index << std::dec << std::endl;
    }
    // Unit Working RAM
    else if (index < 0xE000)
    {
        Memory[index] = value;
    }
    // Shadow memory
    else if (index < 0xFE00)
    {
        Memory[index - 0x2000] = value;
    }
    // IO Registers < FF10
    else if (index < 0xFF10)
    {
        if (index == 0xFF05) {
            // std::cout << "TIMA currently not supported" << std::endl;
        }
        else if (index == 0xFF06) {
            // std::cout << "TMA currently not supported" << std::endl;
        }
        else if (index == 0xFF07) {
            // std::cout << "TAC currently not supported" << std::endl;
        }
        else {
             // std::cout << "Write to 0x" << std::hex << index << " not supported" << std::dec << std::endl;
        }
    }
    // Sound registries
    else if (index >= 0xFF10 && index <= 0xFF3F)
    {
        _gbSound->Write(index, value);
    }
    // Other IO
    else if (index < 0xFF80)
    {
        // std::cout << "Write to 0x" << std::hex << index << " not supported" << std::dec << std::endl;
    }
    // High RAM (HRAM) & Interrupt Enable
    else if (index <= 0xFFFF)
    {
        Memory[index] = value;
    }
    else
    {
        std::cerr << "Error writing to high address: " << std::hex << index << std::dec << std::endl;
    }
}