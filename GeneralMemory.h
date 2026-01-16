#pragma once

#include <vector>
#include <cstdint>
#include <iostream>

class Cartridge;
class Sound;

class GeneralMemory {
private:
    Cartridge* _cartridge;
    Sound* _gbSound;

public:
    std::vector<uint8_t> Memory;

    GeneralMemory(Cartridge* cartridge, Sound* gbSound);

    void Init();

    uint8_t Read(int index);
    void Write(int index, uint8_t value);
};