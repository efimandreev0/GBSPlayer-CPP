#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <iostream>
#include <cstring>

class Cartridge {
public:
    // Regular data buffer
    std::vector<uint8_t> ROM;

    uint8_t SelectedRomBank;

    // GBS Header Info
    uint8_t Version;
    uint8_t TrackCount;
    uint8_t FirstSong;

    uint16_t LoadAddress;
    uint16_t InitAddress;
    uint16_t PlayAddress;

    uint16_t StackPointer;

    uint8_t TimeModulo;
    uint8_t TimeControl;

    std::string Title;
    std::string Author;
    std::string Copyright;

    uint16_t RomOffset;

    Cartridge();

    void Init();

    uint8_t Read(int index);
    void Write(int index, uint8_t value);

private:
    std::string ReadString(int offset, int length);
};