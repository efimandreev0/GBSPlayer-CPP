#pragma once

#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "GbsAudio.h"
#include "GameBoyCPU.h"
#include "Cartridge.h"
#include "GeneralMemory.h"
#include "Sound.h"

class GbsPlayer {
public:
    GameBoyCPU* Cpu;
    Cartridge* CartridgeData;
    GeneralMemory* Memory;
    Sound* SoundGenerator;
    AudioRingBuffer* AudioBuffer;

    uint8_t CurrentTrack;
    bool GbsLoaded;

private:
    float _volume = 1.0f;
    float _volumeMultiplier = 1.0f;

    std::mutex _updateLock;
    std::atomic<bool> _exitThread;
    std::thread _updateThread;

public:
    GbsPlayer();
    ~GbsPlayer();

    bool Init();

    void OnExit();

    void LoadFile(const std::string& path);
    void ChangeTrack(int offset);
    void StartTrack(uint8_t trackNr);

    void Play();
    void Pause();
    void Resume();
    void Stop();

    float GetVolume();
    void SetVolume(float volume);
    float GetVolumeMultiplier();
    void SetVolumeMultiplier(float multiplier);

    void Update(float deltaTime);
    void StartThread();

private:
    void GbsInit(uint8_t trackNumber);
    void UpdateThreadFunc();

    // Callback for SDL2
    static void AudioCallback(void* udata, uint8_t* stream, int len);
};