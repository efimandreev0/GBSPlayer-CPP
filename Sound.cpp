#include "Sound.h"
#include <cmath>
#include <iostream>
#include <cstring>
#include <algorithm>

Sound::Sound(AudioRingBuffer* buffer) : _outputBuffer(buffer)
{
    std::memset(_soundRegister, 0, sizeof(_soundRegister));
    std::memset(_waveNibbles, 0, sizeof(_waveNibbles));

    Init();
}

void Sound::Init()
{
    _frameSequencerTimer = 0;
    _frameSequencerCounter = 0;

    _square1LengthCounter = 0;
    _square2LengthCounter = 0;
    _waveLengthCounter = 0;
    _noiseLengthCounter = 0;

    _modeOneFreqCounter = 0;
    _modeTwoFreqCounter = 0;
    _modeThreeFreqCounter = 0;

    _square1EnvelopeCounter = 0;
    _square2EnvelopeCounter = 0;
    _noiseEnvelopeCounter = 0;

    _highPassFilterCapacitor = 0;

    _bufferCount = 0;
    WasStopped = false;

    // if (_outputBuffer) _outputBuffer->Clear();
}

bool Sound::IsPlaying()
{
    return !WasStopped;
}

bool Sound::FinishedPlaying()
{
    return WasStopped && (_outputBuffer->GetCount() == 0);
}

void Sound::SetStopTime(float length)
{
    _endBufferCount = (int)(length * OutputRate);
}

void Sound::Play()
{
    WasStopped = false;
}

void Sound::Pause()
{
}

void Sound::Resume()
{
}

void Sound::Stop()
{
    _bufferCount = 0;
    WasStopped = true;
}

void Sound::SetVolume(float volume)
{
    _volumeMultiplier = volume;
}

uint8_t Sound::Read(int index)
{
    // Square 1
    if (index == 0xFF10) return _soundRegister[index - 0xFF10];
    if (index == 0xFF11) return (uint8_t)(_soundRegister[index - 0xFF10] & 0xC0);
    if (index == 0xFF12) return _soundRegister[index - 0xFF10];
    if (index == 0xFF13) return 0;
    if (index == 0xFF14) return _soundRegister[index - 0xFF10];

    // Square 2
    if (index == 0xFF15) return 0;
    if (index == 0xFF16) return (uint8_t)(_soundRegister[index - 0xFF10] & 0xC0);
    if (index == 0xFF17) return _soundRegister[index - 0xFF10];
    if (index == 0xFF18) return 0;
    if (index == 0xFF19) return _soundRegister[index - 0xFF10];

    // Wave
    if (index == 0xFF1A) return (uint8_t)(_waveDACpower ? 0x80 : 0x00);
    if (index == 0xFF1B) return 0;
    if (index == 0xFF1C) return (uint8_t)(_waveVolumeCode << 5);
    if (index == 0xFF1D) return 0;
    if (index == 0xFF1E) return (uint8_t)(_waveLengthEnable ? 0x40 : 0x00);

    // Stuff
    if (index == 0xFF24) return _soundOutputLevel;
    if (index == 0xFF25) return _channelLeftRightControl;
    if (index == 0xFF26) return _soundOnOff;

    if (index >= 0xFF10 && index <= 0xFF3F)
        return _soundRegister[index - 0xFF10];

    return 0;
}

void Sound::Write(int index, uint8_t value)
{
    // Square 1
    if (index == 0xFF10)
    {
        _soundRegister[index - 0xFF10] = (uint8_t)(value & 0x7F);
        _modeOneNumberOfSweepShifts = (uint8_t)(value & 0x07);
        _modeOneSweepSubtraction = (value & 0x08) == 0x08;
        _modeOneSweepTime = (uint8_t)((value & 0x70) >> 4);
        _modeOneSweepCounter = 0;
    }
    else if (index == 0xFF11)
    {
        _soundRegister[index - 0xFF10] = (uint8_t)(value & 0xC7);
        _square1LengthLoad = 64 - (uint8_t)(value & 0x3F);
        auto waveDuty = (uint8_t)(value >> 6);
        _modeOneWavePatternDutyPercentage = waveDuty == 0 ? 1.0f : (float)(waveDuty * 2);
    }
    else if (index == 0xFF12)
    {
        _soundRegister[index - 0xFF10] = value;
        _square1StartVolume = (uint8_t)(value >> 4);
        _square1EnvelopeAddMode = (value & 0x08) == 0x08;
        _square1EnvelopePeriod = (uint8_t)(value & 0x07);
    }
    else if (index == 0xFF13)
    {
        _soundRegister[index - 0xFF10] = value;
        _square1Frequency = (int16_t)((_square1Frequency & 0x700) + value);
        UpdateSquare1FrequencyTime();
    }
    else if (index == 0xFF14)
    {
        _soundRegister[index - 0xFF10] = (uint8_t)(value & 0x40);
        _square1Frequency = (int16_t)(((value & 0x07) << 8) | (_square1Frequency & 0xFF));
        UpdateSquare1FrequencyTime();

        _modeOneCounterEnable = (value & 0x40) == 0x40;

        if ((value & 0x80) == 0x80)
        {
            _square1Running = true;
            _soundOnOff |= 0x01;
            _square1LengthCounter = 0;
            _square1Volume = _square1StartVolume;
            _square1EnvelopeCounter = _square1EnvelopePeriod;
        }
    }

    // Square 2
    else if (index == 0xFF15) { }
    else if (index == 0xFF16)
    {
        _soundRegister[index - 0xFF10] = (uint8_t)(value & 0xC7);
        _modeTwoSoundLength = 64 - (uint8_t)(value & 0x3F);
        auto waveDuty = (uint8_t)(value >> 6);
        _modeTwoWavePatternDutyPercentage = waveDuty == 0 ? 1.0f : (float)(waveDuty * 2);
    }
    else if (index == 0xFF17)
    {
        _soundRegister[index - 0xFF10] = value;
        _square2StartVolume = (uint8_t)(value >> 4);
        _square2EnvelopeAddMode = (value & 0x08) == 0x08;
        _square2EnvelopePeriod = (uint8_t)(value & 0x07);
    }
    else if (index == 0xFF18)
    {
        _soundRegister[index - 0xFF10] = value;
        _square2Frequency = (int16_t)((_square2Frequency & 0x700) + value);
        UpdateSquare2FrequencyTime();
    }
    else if (index == 0xFF19)
    {
        _soundRegister[index - 0xFF10] = (uint8_t)(value & 0x40);
        _square2Frequency = (int16_t)(((value & 0x07) << 8) + (_square2Frequency & 0xFF));
        UpdateSquare2FrequencyTime();

        _modeTwoCounterEnable = (value & 0x40) == 0x40;

        if ((value & 0x80) == 0x80)
        {
            _square2Running = true;
            _soundOnOff |= 0x02;
            _square2LengthCounter = 0;
            _square2Volume = _square2StartVolume;
            _square2EnvelopeCounter = _square2EnvelopePeriod;
        }
    }

    // Wave
    else if (index == 0xFF1A)
    {
        _waveDACpower = (value & 0x80) == 0x80;
    }
    else if (index == 0xFF1B)
    {
        _modeThreeSoundLength = 256 - value;
    }
    else if (index == 0xFF1C)
    {
        _waveVolumeCode = (uint8_t)((value >> 5) & 0x03);
        if (_waveVolumeCode == 0x00) _modeThreeFreqCounter = 0;
    }
    else if (index == 0xFF1D)
    {
        _modeThreeFrequency = (int16_t)((_modeThreeFrequency & 0x700) + value);
        UpdateWaveFrequencyTime();
        _modeThreeFreqCounter = 0;
    }
    else if (index == 0xFF1E)
    {
        _modeThreeFrequency = (int16_t)((_modeThreeFrequency & 0xFF) + ((value & 0x07) << 8));
        UpdateWaveFrequencyTime();

        _waveTrigger = (value & 0x80) == 0x80;
        _waveLengthEnable = (value & 0x40) == 0x40;

        if (_waveDACpower && _waveTrigger)
        {
            _soundOnOff |= 0x04;
            _waveLengthCounter = 0;
            _modeThreeFreqCounter = 0;
        }
    }

    // Noise
    else if (index == 0xFF20)
    {
        _noiseLengthLoad = (uint8_t)(64 - (value & 0x3F));
    }
    else if (index == 0xFF21)
    {
        _noiseInitVolume = (uint8_t)(value >> 4);
        _noiseEnvelopeAddMode = (value & 0x08) == 0x08;
        _noiseEnvelopePeriod = (uint8_t)(value & 0x07);
    }
    else if (index == 0xFF22)
    {
        _noiseClockShift = (uint8_t)(value >> 4);
        _noiseWidthMode = (value & 0x08) == 0x08;
        _noiseDivisorCode = (uint8_t)(value & 0x07);
    }
    else if (index == 0xFF23)
    {
        _noiseTrigger = (value & 0x80) == 0x80;
        _noiseLengthEnable = (value & 0x40) == 0x40;

        if (_noiseTrigger)
        {
            _soundOnOff |= 0x08;
            _noiseLengthCounter = 0;
            _noiseVolume = _noiseInitVolume;
            _noiseEnvelopeCounter = _noiseEnvelopePeriod;
            _noiseLfsr = _noiseWidthMode ? 0x7F : 0x7FFF;
        }
        else
        {
            _soundOnOff &= 0xF7;
        }
    }
    else if (index == 0xFF24)
    {
        _soundOutputLevel = value;
    }
    else if (index == 0xFF25)
        _channelLeftRightControl = value;
    else if (index == 0xFF26)
        _soundOnOff = value;

    // wave data
    else if (0xFF30 <= index && index <= 0xFF3F)
    {
        _soundRegister[index - 0xFF10] = value;
        _waveNibbles[(index - 0xFF30) * 2] = (uint8_t)(value >> 4);
        _waveNibbles[(index - 0xFF30) * 2 + 1] = (uint8_t)(value & 0x0F);
    }
}

void Sound::UpdateSquare1FrequencyTime()
{
    _modeOneFreqTime = OutputRate / (4194304.0 / (4 * (2048 - _square1Frequency)));
}

void Sound::UpdateSquare2FrequencyTime()
{
    _modeTwoFreqTime = OutputRate / (4194304.0 / (4 * (2048 - _square2Frequency)));
}

void Sound::UpdateWaveFrequencyTime()
{
    _modeThreeFreqTime = OutputRate / (4194304.0 / (2 * (2048 - _modeThreeFrequency)));
}

void Sound::UpdateBuffer()
{
    DebugCounter++;
    _frameSequencerTimer++;

    // 44100/512 = 86
    if (_frameSequencerTimer >= 86)
    {
        _frameSequencerTimer = 0;

        // 256Hz Length Ctr Clock
        if (_frameSequencerCounter % 2 == 0)
        {
            _square1LengthCounter++;
            _square2LengthCounter++;
            _waveLengthCounter++;
            _noiseLengthCounter++;

            if (_square1LengthCounter >= _square1LengthLoad && _modeOneCounterEnable) _soundOnOff &= 0xFE;
            if (_square2LengthCounter >= _modeTwoSoundLength && _modeTwoCounterEnable) _soundOnOff &= 0xFD;
            if (_waveLengthCounter >= _modeThreeSoundLength && _waveLengthEnable) { _waveTrigger = false; _soundOnOff &= 0xFB; }
            if (_noiseLengthCounter >= _noiseLengthLoad && _noiseLengthEnable) _soundOnOff &= 0xF7;
        }

        // 64Hz Volume Envelope Clock
        if ((_frameSequencerCounter + 1) % 8 == 0)
        {
            // Square 1
            _square1EnvelopeCounter--;
            if (_square1EnvelopePeriod != 0 && _square1EnvelopeCounter <= 0 &&
               ((!_square1EnvelopeAddMode && _square1Volume > 0) || (_square1EnvelopeAddMode && _square1Volume < 15)))
            {
                _square1EnvelopeCounter = _square1EnvelopePeriod;
                _square1Volume += _square1EnvelopeAddMode ? 1 : -1;
            }

            // Square 2
            _square2EnvelopeCounter--;
            if (_square2EnvelopePeriod != 0 && _square2EnvelopeCounter <= 0 &&
               ((!_square2EnvelopeAddMode && _square2Volume > 0) || (_square2EnvelopeAddMode && _square2Volume < 15)))
            {
                _square2EnvelopeCounter = _square2EnvelopePeriod;
                _square2Volume += _square2EnvelopeAddMode ? 1 : -1;
            }

            // Noise
            _noiseEnvelopeCounter--;
            if (_noiseEnvelopePeriod != 0 && _noiseEnvelopeCounter <= 0 &&
                ((!_noiseEnvelopeAddMode && _noiseVolume > 0) || (_noiseEnvelopeAddMode && _noiseVolume < 15)))
            {
                _noiseEnvelopeCounter = _noiseEnvelopePeriod;
                _noiseVolume += (uint8_t)(_noiseEnvelopeAddMode ? 1 : -1);
            }
        }

        // 128Hz Sweep Clock
        if ((_frameSequencerCounter + 2) % 4 == 0)
        {
            _modeOneSweepCounter++;
            if (_modeOneSweepTime != 0 && _modeOneNumberOfSweepShifts != 0 && _modeOneSweepCounter >= _modeOneSweepTime)
            {
                _modeOneSweepCounter = 0;
                int16_t newFreq = (int16_t)(_square1Frequency + (_modeOneSweepSubtraction ? -1 : 1) * (_square1Frequency >> _modeOneNumberOfSweepShifts));

                if ((newFreq & 0x7FF) != newFreq) _soundOnOff &= 0xFE;
                else if (newFreq > 0)
                {
                    _square1Frequency = newFreq;
                    _soundRegister[0xFF13 - 0xFF10] = (uint8_t)(_square1Frequency & 0xFF);
                    _soundRegister[0xFF14 - 0xFF10] = (uint8_t)((_soundRegister[0xFF14 - 0xFF10] & 0xF8) + (_square1Frequency >> 8));
                    UpdateSquare1FrequencyTime();
                }
            }
        }

        _frameSequencerCounter++;
        if (_frameSequencerCounter >= 8) _frameSequencerCounter = 0;
    }

    // Square 1
    int16_t channel0 = 0;
    if (_square1Running) {
        _modeOneFreqCounter++;
        if (_modeOneFreqCounter >= _modeOneFreqTime) {
            _modeOneFreqCounter -= _modeOneFreqTime;
            _waveOne = _square1WaveCounter < _modeOneWavePatternDutyPercentage ? 1 : 0;
            _square1WaveCounter = (uint8_t)((_square1WaveCounter + 1) % 8);
        }
        channel0 = (int16_t)(_waveOne * _square1Volume);
        if ((_soundOnOff & 0x01) == 0x00) channel0 = 0;
    }

    // Square 2
    int16_t channel1 = 0;
    if (_square2Running) {
        _modeTwoFreqCounter++;
        if (_modeTwoFreqCounter >= _modeTwoFreqTime) {
            _modeTwoFreqCounter -= _modeTwoFreqTime;
            _waveTwo = _square2WaveCounter < _modeTwoWavePatternDutyPercentage ? 1 : 0;
            _square2WaveCounter = (uint8_t)((_square2WaveCounter + 1) % 8);
        }
        channel1 = (int16_t)(_waveTwo * _square2Volume);
        if ((_soundOnOff & 0x02) == 0x00) channel1 = 0;
    }

    // Wave
    double channel2 = 0;
    {
        _modeThreeFreqCounter++;
        if (_modeThreeFreqCounter >= _modeThreeFreqTime)
        {
            _modeThreeFreqCounter -= _modeThreeFreqTime;
            _waveIndex = (_waveIndex + 1) % 32;

            // Fix for C++ negative shift
            int shift = (_waveVolumeCode > 0) ? (_waveVolumeCode - 1) : 0;
            _waveThree = (uint8_t)(_waveNibbles[_waveIndex] >> shift);
        }
        channel2 = _waveThree;
        if ((_soundOnOff & 0x04) == 0x00 || _waveVolumeCode == 0x00 || !_waveDACpower)
            channel2 = 0;
    }

    // Noise
    double channel3 = 0;
    if ((_soundOnOff & 0x08) != 0x00)
    {
        channel3 = (double)((~_noiseLfsr & 0x01) * _noiseVolume);

        auto s = _noiseClockShift;
        auto r = _noiseDivisorCode == 0 ? 0.5 : _noiseDivisorCode;
        double divisor = (262144.0 / r / std::pow(2, s));
        double freq = 4194304.0 / divisor;
        double stepSize = 4194304.0 / OutputRate;

        _noiseTimeSteps += stepSize;
        int stepCount = (int)(_noiseTimeSteps / freq);
        if (stepCount > 1) channel3 /= (double)stepCount;

        while (_noiseTimeSteps >= freq)
        {
            _noiseTimeSteps -= freq;
            int16_t lsb = (int16_t)((_noiseLfsr & 0x01) ^ ((_noiseLfsr >> 1) & 0x01));
            _noiseLfsr >>= 1;
            _noiseLfsr |= (int16_t)(lsb << 14);
            if (_noiseWidthMode) _noiseLfsr |= (int16_t)(lsb << 6);

            if (_noiseTimeSteps > freq)
                channel3 += (double)((~_noiseLfsr & 0x01) * _noiseVolume) / (double)stepCount;
        }
    }

    // Mix
    double mixerOutput = ((channel0 / 15.0) + (channel1 / 15.0) + (channel2 / 15.0) + (channel3 / 15.0)) / 4.0;

    if ((_soundOnOff & 0x80) != 0x80) mixerOutput = 0;

    auto output = HighPass(mixerOutput, (_soundOnOff & 0x0F) != 0x00);

    // Convert to 16-bit signed integer (Scale 0..1 to -32768..32767)
    int16_t sample16 = (int16_t)(output * 32767 * _volumeMultiplier);

    if (_outputBuffer)
    {
        _outputBuffer->Write(sample16); // Left Channel
        _outputBuffer->Write(sample16); // Right Channel
    }

    _bufferCount++;
    if (_endBufferCount > 0 && _bufferCount > _endBufferCount)
        WasStopped = true;
}

double Sound::HighPass(double input, bool dacsEnabled)
{
    if (!dacsEnabled) return 0.0;
    auto output = input - _highPassFilterCapacitor;
    _highPassFilterCapacitor = input - output * 0.996013308910;
    return output;
}