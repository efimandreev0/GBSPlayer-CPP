#pragma once

#include <vector>
#include <mutex>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <algorithm>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

class AudioRingBuffer {
private:
    std::vector<int16_t> buffer;
    size_t head = 0;
    size_t tail = 0;
    size_t capacity;
    std::mutex mtx;

public:
    AudioRingBuffer(size_t size = 65536 * 3) : capacity(size) {
        buffer.resize(size, 0);
    }

    void Write(int16_t sample) {
        std::lock_guard<std::mutex> lock(mtx);
        size_t next_head = (head + 1) % capacity;
        if (next_head != tail) { // Если есть место
            buffer[head] = sample;
            head = next_head;
        }
    }

    // Читаем массив данных для SDL
    void Read(int16_t* output, int count) {
        std::lock_guard<std::mutex> lock(mtx);
        for (int i = 0; i < count; i++) {
            if (tail != head) {
                output[i] = buffer[tail];
                tail = (tail + 1) % capacity;
            } else {
                // Если данных нет (эмулятор не успел), заполняем тишиной
                output[i] = 0;
            }
        }
    }

    int GetCount() {
        std::lock_guard<std::mutex> lock(mtx);
        if (head >= tail) return head - tail;
        return capacity - (tail - head);
    }
    int GetCapacity() { return capacity; }
    void Clear() {
        std::lock_guard<std::mutex> lock(mtx);
        head = tail = 0;
    }
};