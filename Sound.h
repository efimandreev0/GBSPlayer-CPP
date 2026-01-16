#pragma once

#include <vector>
#include <cstdint>
#include <cmath>
#include <cstring>
#include <iostream>
#include <algorithm>

#include "GbsAudio.h" 

class Sound {
public:
    AudioRingBuffer* _outputBuffer;

    bool WasStopped;
    int _endBufferCount;
    int _bufferCount;

    static const int OutputRate = 44100;

    // FF10-FF3F
    uint8_t _soundRegister[0x30];

    // frame sequencer
    int _frameSequencerTimer;
    uint8_t _frameSequencerCounter;

    // Square 1
    uint8_t _modeOneNumberOfSweepShifts;
    bool _modeOneSweepSubtraction;
    uint8_t _modeOneSweepTime;
    uint8_t _modeOneSweepCounter;

    float _modeOneWavePatternDutyPercentage;

    int _square1LengthLoad;
    uint8_t _square1LengthCounter;

    // envelope
    uint8_t _square1StartVolume;
    bool _square1EnvelopeAddMode;
    uint8_t _square1EnvelopePeriod;
    int _square1EnvelopeCounter;
    int _square1Volume;

    int16_t _square1Frequency;
    double _modeOneFreqCounter;
    double _modeOneFreqTime;
    bool _modeOneFreqDuty;
    uint8_t _square1WaveCounter;

    bool _modeOneCounterEnable;
    bool _square1Running;
    int _waveOne = 1;

    // Square 2
    float _modeTwoWavePatternDutyPercentage;

    uint8_t _square2StartVolume;
    bool _square2EnvelopeAddMode;
    uint8_t _square2EnvelopePeriod;
    int _square2EnvelopeCounter;
    int _square2Volume;

    uint8_t _square2LengthCounter;
    int _modeTwoSoundLength;

    int16_t _square2Frequency;
    double _modeTwoFreqTime;
    double _modeTwoFreqCounter;
    uint8_t _square2WaveCounter;

    bool _modeTwoCounterEnable;
    bool _square2Running;
    int _waveTwo = 1;

    // Wave
    bool _waveDACpower;
    uint8_t _waveLengthCounter;
    int _modeThreeSoundLength;
    uint8_t _waveVolumeCode;
    int16_t _modeThreeFrequency;
    double _modeThreeFreqCounter;
    double _modeThreeFreqTime;
    bool _waveTrigger;
    bool _waveLengthEnable;

    uint8_t _waveNibbles[32];
    int _waveIndex;
    int16_t _waveThree;

    // Noise
    uint8_t _noiseLengthLoad;
    uint8_t _noiseVolume;
    uint8_t _noiseInitVolume;
    bool _noiseEnvelopeAddMode;
    uint8_t _noiseEnvelopePeriod;

    uint8_t _noiseClockShift;
    bool _noiseWidthMode;
    uint8_t _noiseDivisorCode;

    bool _noiseTrigger;
    bool _noiseLengthEnable;

    int16_t _noiseLfsr;
    double _noiseTimeSteps;
    int _noiseEnvelopeCounter;
    uint8_t _noiseLengthCounter;

    // FF24
    uint8_t _soundOutputLevel;
    // FF25
    uint8_t _channelLeftRightControl;
    // FF26
    uint8_t _soundOnOff;

    double _highPassFilterCapacitor;
    int DebugCounter;

    // Volume mixer
    float _volumeMultiplier = 1.0f;

    Sound(AudioRingBuffer* buffer);

    void Init();
    bool IsPlaying();
    bool FinishedPlaying();
    void SetStopTime(float length);
    void Play();
    void Pause();
    void Resume();
    void Stop();
    void SetVolume(float volume);

    uint8_t Read(int index);
    void Write(int index, uint8_t value);

    void UpdateBuffer();

private:
    void UpdateSquare1FrequencyTime();
    void UpdateSquare2FrequencyTime();
    void UpdateWaveFrequencyTime();
    double HighPass(double input, bool dacsEnabled);
};