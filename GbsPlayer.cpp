#include "GbsPlayer.h"
#include <fstream>
#include <chrono>

void GbsPlayer::AudioCallback(void* udata, uint8_t* stream, int len) {
    AudioRingBuffer* buffer = (AudioRingBuffer*)udata;

    int16_t* out16 = (int16_t*)stream;
    int samplesRequest = len / 2;

    buffer->Read(out16, samplesRequest);
}

GbsPlayer::GbsPlayer() {
    AudioBuffer = new AudioRingBuffer(65536);

    SoundGenerator = new Sound(AudioBuffer);
    CartridgeData = new Cartridge();
    Memory = new GeneralMemory(CartridgeData, SoundGenerator);
    Cpu = new GameBoyCPU(Memory, CartridgeData, SoundGenerator);

    CurrentTrack = 0;
    GbsLoaded = false;
    _exitThread = false;
}

GbsPlayer::~GbsPlayer() {
    OnExit();

    delete Cpu;
    delete Memory;
    delete CartridgeData;
    delete SoundGenerator;
    delete AudioBuffer;

    Mix_CloseAudio();
    SDL_Quit();
}

bool GbsPlayer::Init() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cout << "SDL Init Failed: " << SDL_GetError() << std::endl;
        return false;
    }

    if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 4096) < 0) {
        std::cout << "Mixer Init Failed: " << Mix_GetError() << std::endl;
        return false;
    }

    Mix_HookMusic(AudioCallback, AudioBuffer);

    return true;
}

void GbsPlayer::OnExit() {
    _exitThread = true;
    if (_updateThread.joinable()) {
        _updateThread.join();
    }
}

void GbsPlayer::LoadFile(const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);

    if (!file.is_open()) {
        std::cout << "Failed to open file: " << path << std::endl;
        return;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    CartridgeData->ROM.resize(size);
    if (!file.read((char*)CartridgeData->ROM.data(), size)) {
        std::cout << "Failed to read file" << std::endl;
        return;
    }

    CartridgeData->Init();
    Cpu->Init();

    GbsLoaded = true;
    std::cout << "Finished loading file: " << path << std::endl;
}

void GbsPlayer::ChangeTrack(int offset) {
    int newTrack = CurrentTrack + offset;

    while (newTrack < 0)
        newTrack += CartridgeData->TrackCount;

    if (CartridgeData->TrackCount > 0)
        newTrack %= CartridgeData->TrackCount;
    else
        newTrack = 0;

    StartTrack((uint8_t)newTrack);
}

void GbsPlayer::StartTrack(uint8_t trackNr) {
    std::lock_guard<std::mutex> lock(_updateLock);
    CurrentTrack = trackNr;
    SoundGenerator->Stop();
    GbsInit(trackNr);
    SoundGenerator->SetStopTime(0);
}

void GbsPlayer::GbsInit(uint8_t trackNumber) {
    CartridgeData->Init();
    Cpu->SkipBootROM();
    Cpu->Init();
    Cpu->SetPlaybackSpeed(1.0f);

    Cpu->reg_A = trackNumber;

    Cpu->reg_PC = CartridgeData->InitAddress;
    Cpu->reg_SP = CartridgeData->StackPointer;

    // Push idle address to stack
    // Stack grows down: --SP
    uint16_t sp = Cpu->reg_SP;
    Memory->Write(--sp, (uint8_t)(Cpu->IdleAddress >> 8));
    Memory->Write(--sp, (uint8_t)(Cpu->IdleAddress & 0xFF));
    Cpu->reg_SP = sp;

    std::cout << "Finished GBS Init for track " << (int)trackNumber << std::endl;
}

void GbsPlayer::Play() {
    Cpu->IsRunning = true;
}

void GbsPlayer::Pause() {
    SoundGenerator->Pause();
    Cpu->IsRunning = false;
}

void GbsPlayer::Resume() {
    SoundGenerator->Resume();
    // Mix_ResumeMusic();
    Cpu->IsRunning = true;
}

void GbsPlayer::Stop() {
    SoundGenerator->Stop();
    Cpu->IsRunning = false;
}

float GbsPlayer::GetVolume() {
    return _volume;
}

void GbsPlayer::SetVolume(float volume) {
    _volume = volume;
    SoundGenerator->SetVolume(_volume * _volumeMultiplier);
}

float GbsPlayer::GetVolumeMultiplier() {
    return _volumeMultiplier;
}

void GbsPlayer::SetVolumeMultiplier(float multiplier) {
    _volumeMultiplier = multiplier;
    SoundGenerator->SetVolume(_volume * _volumeMultiplier);
}

void GbsPlayer::Update(float deltaTime) {
    Cpu->Update();
}

void GbsPlayer::StartThread() {
    _updateThread = std::thread(&GbsPlayer::UpdateThreadFunc, this);
}

void GbsPlayer::UpdateThreadFunc() {
    while (!_exitThread) {

        if (GbsLoaded) {
            std::lock_guard<std::mutex> lock(_updateLock);
            Cpu->Update();
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        std::this_thread::yield(); // Get CPU one quant of time
    }
}