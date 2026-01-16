#include "Cartridge.h"

Cartridge::Cartridge() {
    SelectedRomBank = 1;
    RomOffset = 0;
    Version = 0;
    TrackCount = 0;
    FirstSong = 0;
    LoadAddress = 0;
    InitAddress = 0;
    PlayAddress = 0;
    StackPointer = 0;
    TimeModulo = 0;
    TimeControl = 0;
}

void Cartridge::Init() {
    // Reset values
    SelectedRomBank = 1;

    if (ROM.size() < 0x70) {
        std::cerr << "Error: ROM size is too small to contain a header." << std::endl;
        return;
    }

    // GBS Header parsing
    Version = ROM[0x03];
    TrackCount = ROM[0x04];
    FirstSong = ROM[0x05];

    // Little Endian combining
    LoadAddress = (uint16_t)(ROM[0x06] | (ROM[0x07] << 8));
    InitAddress = (uint16_t)(ROM[0x08] | (ROM[0x09] << 8));
    PlayAddress = (uint16_t)(ROM[0x0a] | (ROM[0x0b] << 8));

    StackPointer = (uint16_t)(ROM[0x0c] | (ROM[0x0d] << 8));

    TimeModulo = ROM[0x0e];
    TimeControl = ROM[0x0f];

    // Metadata reading
    Title = ReadString(0x10, 32);
    Author = ReadString(0x30, 32);
    Copyright = ReadString(0x50, 32);

    RomOffset = (uint16_t)(LoadAddress - 0x70);
}

std::string Cartridge::ReadString(int offset, int length) {
    if (offset + length > ROM.size()) return "";

    return std::string(reinterpret_cast<const char*>(&ROM[offset]), length);
}

uint8_t Cartridge::Read(int index) {
    // ROM Bank 00
    if (index < 0x4000) {
        int romIndex = index - RomOffset;
        if (romIndex >= 0 && romIndex < (int)ROM.size()) {
            return ROM[romIndex];
        }
    }
    // ROM Bank 01-..
    else if (index < 0x8000) {
        int romIndex = (SelectedRomBank * 0x4000) + (index - 0x4000) - RomOffset;
        if (romIndex >= 0 && romIndex < (int)ROM.size()) {
            return ROM[romIndex];
        }
    }

    // std::cout << "Cartridge Index unavailable: " << std::hex << index << std::dec << std::endl;
    return 0;
}

void Cartridge::Write(int index, uint8_t value) {
    // normally enable RAM
    if (index < 0x2000) {
        // std::cout << "Write to 0x" << std::hex << index << " not supported" << std::dec << std::endl;
    }
    // select ROM Bank
    else if (index < 0x4000) {
        // GBS: A page is selected into Bank 1 by writing the page number...
        SelectedRomBank = value;
        
        // MBC1 behavior: writing 0 usually selects bank 1.
        // GBS spec might differ slightly, but usually bank 0 is not accessible at 0x4000
        if (SelectedRomBank == 0) SelectedRomBank = 1; 
    }
    // RAM Bank Number - or - Upper Bits of ROM Bank Number
    else if (index < 0x6000) {
        // Player authors: you should disregard writes to $4000-$5fff...
    }
    else {
        // std::cout << "Write to 0x" << std::hex << index << " not supported" << std::dec << std::endl;
    }
}