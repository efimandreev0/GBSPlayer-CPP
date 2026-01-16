#pragma once

#include <vector>
#include <mutex>
#include <algorithm>
#include <cstdint>
#include <climits>

class ByteBufferPool {
private:
    int _minBufferSize;
    int _maxBuffers;

    std::vector<std::vector<uint8_t>> _freeBuffers;
    std::mutex _mutex;
    int FirstLargerThan(int size) {
        if (_freeBuffers.empty()) return -1;

        int l = 0;
        int r = (int)_freeBuffers.size() - 1;

        while (l <= r) {
            int m = (l + r) / 2;
            size_t len = _freeBuffers[m].size();

            if (len < (size_t)size) {
                l = m + 1;
            }
            else if (len > (size_t)size) {
                r = m;
                if (l == r) return l;
            }
            else {
                return m;
            }
        }

        return -1;
    }

public:
    ByteBufferPool(int minBufferSize = 0, int maxBuffers = INT_MAX) 
        : _minBufferSize(minBufferSize), _maxBuffers(maxBuffers) {}

    int FreeAmount() {
        std::lock_guard<std::mutex> lock(_mutex);
        return (int)_freeBuffers.size();
    }

    std::vector<uint8_t> Get(int size) {
        if (size < _minBufferSize)
            size = _minBufferSize;

        std::vector<uint8_t> result;

        std::lock_guard<std::mutex> lock(_mutex);
        
        int index = FirstLargerThan(size);

        if (index == -1) {
            if (!_freeBuffers.empty()) {
                _freeBuffers.erase(_freeBuffers.begin());
            }
            result.resize(size);
        }
        else {
            result = std::move(_freeBuffers[index]);
            _freeBuffers.erase(_freeBuffers.begin() + index);
        }
        
        return result;
    }

    void Return(std::vector<uint8_t> buffer) {
        std::lock_guard<std::mutex> lock(_mutex);

        if ((int)_freeBuffers.size() >= _maxBuffers)
            return;

        int index = FirstLargerThan((int)buffer.size());
        
        if (index == -1)
            _freeBuffers.push_back(std::move(buffer));
        else
            _freeBuffers.insert(_freeBuffers.begin() + index, std::move(buffer));
    }
};