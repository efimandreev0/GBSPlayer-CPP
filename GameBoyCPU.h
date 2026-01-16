#pragma once

#include <vector>
#include <functional>
#include <cstdint>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <thread>

#include "GeneralMemory.h"
#include "Cartridge.h"
#include "Sound.h"

namespace OpTables {
    extern const uint8_t LengthTable[256];
    extern const uint8_t CycleArray[256];
    extern const uint8_t CycleCbArray[256];
}

class GameBoyCPU {
public:
    GeneralMemory* _memory;
    Cartridge* _cartridge;
    Sound* _gbSound;

    // Interrupt Master Enable Flag
    bool IME;

    // Registers
    uint8_t reg_A, reg_F;
    uint8_t reg_B, reg_C;
    uint8_t reg_D, reg_E;
    uint8_t reg_H, reg_L;

    uint16_t reg_SP;
    uint16_t reg_PC;

    // Pseudo-registers (Getters/Setters logic)
    uint16_t get_AF() const { return (uint16_t)((reg_A << 8) | reg_F); }
    void set_AF(uint16_t val) { reg_A = (uint8_t)(val >> 8); reg_F = (uint8_t)(val & 0xF0); }

    uint16_t get_BC() const { return (uint16_t)((reg_B << 8) | reg_C); }
    void set_BC(uint16_t val) { reg_B = (uint8_t)(val >> 8); reg_C = (uint8_t)(val & 0xFF); }

    uint16_t get_DE() const { return (uint16_t)((reg_D << 8) | reg_E); }
    void set_DE(uint16_t val) { reg_D = (uint8_t)(val >> 8); reg_E = (uint8_t)(val & 0xFF); }

    uint16_t get_HL() const { return (uint16_t)((reg_H << 8) | reg_L); }
    void set_HL(uint16_t val) { reg_H = (uint8_t)(val >> 8); reg_L = (uint8_t)(val & 0xFF); }

    // Flags
    bool get_Z() const { return (reg_F & 0x80) == 0x80; }
    void set_Z(bool val) { if (val) reg_F |= 0x80; else reg_F &= ~0x80; }

    bool get_N() const { return (reg_F & 0x40) == 0x40; }
    void set_N(bool val) { if (val) reg_F |= 0x40; else reg_F &= ~0x40; }

    bool get_H() const { return (reg_F & 0x20) == 0x20; }
    void set_H(bool val) { if (val) reg_F |= 0x20; else reg_F &= ~0x20; }

    bool get_C() const { return (reg_F & 0x10) == 0x10; }
    void set_C(bool val) { if (val) reg_F |= 0x10; else reg_F &= ~0x10; }

    uint8_t get_CBit() const { return get_C() ? 1 : 0; }

    // Helpers to access memory relative to PC
    uint8_t data8();
    uint16_t data16();

    // State
    bool CPUHalt;
    
    // Timing constants
    static const int Clockrate = 4194304;
    static const int UpdateRateInt = 60; // approx 59.73
    static const int UpdateStepSize = 10;
    static const int CycleTime = (int)(Clockrate / 1000.0f * UpdateStepSize);

    int cycleCount;
    int lastCycleCount;

    float soundCount;
    float maxSoundCycles; // 95.1089f

    float updateCycleCounter;
    float maxPlayCycles;
    float maxPlayCycles60;

    int divCounter;
    int timerCounter;
    int currentInstruction;

    bool _finishedInit;
    bool _calledPlay;
    uint16_t IdleAddress = 0xF00D;
    bool IsRunning;

    // Tables of function pointers
    typedef void (GameBoyCPU::*OpFunc)();
    OpFunc ops[256];
    OpFunc op_cb[256];

    GameBoyCPU(GeneralMemory* memory, Cartridge* cartridge, Sound* gbSound);
    
    void Init();
    void Update();
    void SkipBootROM();
    void SetPlaybackSpeed(float multiplier);

private:
    std::chrono::steady_clock::time_point _lastTime;
    double _accumulator;

    void CPUCycle();
    void ExecuteInstruction();
    void SetupOps(); // Fills the tables

    // --- INSTRUCTION DECLARATIONS ---
    void NOP(); void STOP(); void HALT(); void DI(); void EI();
    
    // Loads
    void LD_BC_d16(); void LD_aBC_A(); void LD_B_d8(); void LD_a16_SP(); 
    void LD_A_aBC(); void LD_C_d8(); void LD_DE_d16(); void LD_aDE_A(); 
    void LD_D_d8(); void LD_A_aDE(); void LD_E_d8(); void LD_HL_d16(); 
    void LD_aHLp_A(); void LD_H_d8(); void LD_A_aHLp(); void LD_L_d8(); 
    void LD_SP_d16(); void LD_aHLm_A(); void LD_aHL_d8(); void LD_A_aHLm(); 
    void LD_A_d8();
    
    void LD_B_B(); void LD_B_C(); void LD_B_D(); void LD_B_E(); void LD_B_H(); void LD_B_L(); void LD_B_aHL(); void LD_B_A();
    void LD_C_B(); void LD_C_C(); void LD_C_D(); void LD_C_E(); void LD_C_H(); void LD_C_L(); void LD_C_aHL(); void LD_C_A();
    void LD_D_B(); void LD_D_C(); void LD_D_D(); void LD_D_E(); void LD_D_H(); void LD_D_L(); void LD_D_aHL(); void LD_D_A();
    void LD_E_B(); void LD_E_C(); void LD_E_D(); void LD_E_E(); void LD_E_H(); void LD_E_L(); void LD_E_aHL(); void LD_E_A();
    void LD_H_B(); void LD_H_C(); void LD_H_D(); void LD_H_E(); void LD_H_H(); void LD_H_L(); void LD_H_aHL(); void LD_H_A();
    void LD_L_B(); void LD_L_C(); void LD_L_D(); void LD_L_E(); void LD_L_H(); void LD_L_L(); void LD_L_aHL(); void LD_L_A();
    void LD_aHL_B(); void LD_aHL_C(); void LD_aHL_D(); void LD_aHL_E(); void LD_aHL_H(); void LD_aHL_L(); void LD_aHL_A();
    void LD_A_B(); void LD_A_C(); void LD_A_D(); void LD_A_E(); void LD_A_H(); void LD_A_L(); void LD_A_aHL(); void LD_A_A();

    // ALU
    void INC_BC(); void INC_B(); void DEC_B(); void ADD_HL_BC(); void DEC_BC(); void INC_C(); void DEC_C();
    void INC_DE(); void INC_D(); void DEC_D(); void ADD_HL_DE(); void DEC_DE(); void INC_E(); void DEC_E();
    void INC_HL(); void INC_H(); void DEC_H(); void ADD_HL_HL(); void DEC_HL(); void INC_L(); void DEC_L();
    void INC_SP(); void INC_aHL(); void DEC_aHL(); void ADD_HL_SP(); void DEC_SP(); void INC_A(); void DEC_A();
    
    void RLCA(); void RRCA(); void RLA(); void RRA(); void DAA(); void CPL(); void SCF(); void CCF();

    void ADD_A_B(); void ADD_A_C(); void ADD_A_D(); void ADD_A_E(); void ADD_A_H(); void ADD_A_L(); void ADD_A_aHL(); void ADD_A_A();
    void ADC_A_B(); void ADC_A_C(); void ADC_A_D(); void ADC_A_E(); void ADC_A_H(); void ADC_A_L(); void ADC_A_aHL(); void ADC_A_A();
    void SUB_B(); void SUB_C(); void SUB_D(); void SUB_E(); void SUB_H(); void SUB_L(); void SUB_aHL(); void SUB_A();
    void SBC_A_B(); void SBC_A_C(); void SBC_A_D(); void SBC_A_E(); void SBC_A_H(); void SBC_A_L(); void SBC_A_aHL(); void SBC_A_A();
    void AND_B(); void AND_C(); void AND_D(); void AND_E(); void AND_H(); void AND_L(); void AND_aHL(); void AND_A();
    void XOR_B(); void XOR_C(); void XOR_D(); void XOR_E(); void XOR_H(); void XOR_L(); void XOR_aHL(); void XOR_A();
    void OR_B(); void OR_C(); void OR_D(); void OR_E(); void OR_H(); void OR_L(); void OR_aHL(); void OR_A();
    void CP_B(); void CP_C(); void CP_D(); void CP_E(); void CP_H(); void CP_L(); void CP_aHL(); void CP_A();
    
    void ADD_A_d8(); void ADC_A_d8(); void SUB_d8(); void SBC_A_d8(); void AND_d8(); void XOR_d8(); void OR_d8(); void CP_d8();

    // Jumps / Calls
    void JR_d8(); void JR_NZ_a8(); void JR_Z_a8(); void JR_NC_a8(); void JR_C_a8();
    void JP_NZ_a16(); void JP_a16(); void JP_Z_a16(); void JP_NC_a16(); void JP_C_a16(); void JP_aHL();
    void RET_NZ(); void RET_Z(); void RET(); void RET_NC(); void RET_C(); void RETI();
    void CALL_NZ_a16(); void CALL_Z_a16(); void CALL_NC_a16(); void CALL_C_a16(); void CALL_a16();
    void RST_00H(); void RST_08H(); void RST_10H(); void RST_18H(); void RST_20H(); void RST_28H(); void RST_30H(); void RST_38H();

    // Stack
    void POP_BC(); void PUSH_BC(); void POP_DE(); void PUSH_DE(); void POP_HL(); void PUSH_HL(); void POP_AF(); void PUSH_AF();
    
    // Misc
    void LDH_a8_A(); void LD_aC_A(); void LDH_A_a8(); void LD_A_aC(); 
    void ADD_SP_r8(); void LD_a16_A(); void LD_HL_SPr8(); void LD_SP_HL(); void LD_A_a16();

    // CB Prefix Operations
    void RLC_B(); void RLC_C(); void RLC_D(); void RLC_E(); void RLC_H(); void RLC_L(); void RLC_aHL(); void RLC_A();
    void RRC_B(); void RRC_C(); void RRC_D(); void RRC_E(); void RRC_H(); void RRC_L(); void RRC_aHL(); void RRC_A();
    void RL_B(); void RL_C(); void RL_D(); void RL_E(); void RL_H(); void RL_L(); void RL_aHL(); void RL_A();
    void RR_B(); void RR_C(); void RR_D(); void RR_E(); void RR_H(); void RR_L(); void RR_aHL(); void RR_A();
    void SLA_B(); void SLA_C(); void SLA_D(); void SLA_E(); void SLA_H(); void SLA_L(); void SLA_aHL(); void SLA_A();
    void SRA_B(); void SRA_C(); void SRA_D(); void SRA_E(); void SRA_H(); void SRA_L(); void SRA_aHL(); void SRA_A();
    void SWAP_B(); void SWAP_C(); void SWAP_D(); void SWAP_E(); void SWAP_H(); void SWAP_L(); void SWAP_aHL(); void SWAP_A();
    void SRL_B(); void SRL_C(); void SRL_D(); void SRL_E(); void SRL_H(); void SRL_L(); void SRL_aHL(); void SRL_A();

    void BIT_0_B(); void BIT_0_C(); void BIT_0_D(); void BIT_0_E(); void BIT_0_H(); void BIT_0_L(); void BIT_0_aHL(); void BIT_0_A();
    void BIT_1_B(); void BIT_1_C(); void BIT_1_D(); void BIT_1_E(); void BIT_1_H(); void BIT_1_L(); void BIT_1_aHL(); void BIT_1_A();
    void BIT_2_B(); void BIT_2_C(); void BIT_2_D(); void BIT_2_E(); void BIT_2_H(); void BIT_2_L(); void BIT_2_aHL(); void BIT_2_A();
    void BIT_3_B(); void BIT_3_C(); void BIT_3_D(); void BIT_3_E(); void BIT_3_H(); void BIT_3_L(); void BIT_3_aHL(); void BIT_3_A();
    void BIT_4_B(); void BIT_4_C(); void BIT_4_D(); void BIT_4_E(); void BIT_4_H(); void BIT_4_L(); void BIT_4_aHL(); void BIT_4_A();
    void BIT_5_B(); void BIT_5_C(); void BIT_5_D(); void BIT_5_E(); void BIT_5_H(); void BIT_5_L(); void BIT_5_aHL(); void BIT_5_A();
    void BIT_6_B(); void BIT_6_C(); void BIT_6_D(); void BIT_6_E(); void BIT_6_H(); void BIT_6_L(); void BIT_6_aHL(); void BIT_6_A();
    void BIT_7_B(); void BIT_7_C(); void BIT_7_D(); void BIT_7_E(); void BIT_7_H(); void BIT_7_L(); void BIT_7_aHL(); void BIT_7_A();

    void RES_0_B(); void RES_0_C(); void RES_0_D(); void RES_0_E(); void RES_0_H(); void RES_0_L(); void RES_0_aHL(); void RES_0_A();
    void RES_1_B(); void RES_1_C(); void RES_1_D(); void RES_1_E(); void RES_1_H(); void RES_1_L(); void RES_1_aHL(); void RES_1_A();
    void RES_2_B(); void RES_2_C(); void RES_2_D(); void RES_2_E(); void RES_2_H(); void RES_2_L(); void RES_2_aHL(); void RES_2_A();
    void RES_3_B(); void RES_3_C(); void RES_3_D(); void RES_3_E(); void RES_3_H(); void RES_3_L(); void RES_3_aHL(); void RES_3_A();
    void RES_4_B(); void RES_4_C(); void RES_4_D(); void RES_4_E(); void RES_4_H(); void RES_4_L(); void RES_4_aHL(); void RES_4_A();
    void RES_5_B(); void RES_5_C(); void RES_5_D(); void RES_5_E(); void RES_5_H(); void RES_5_L(); void RES_5_aHL(); void RES_5_A();
    void RES_6_B(); void RES_6_C(); void RES_6_D(); void RES_6_E(); void RES_6_H(); void RES_6_L(); void RES_6_aHL(); void RES_6_A();
    void RES_7_B(); void RES_7_C(); void RES_7_D(); void RES_7_E(); void RES_7_H(); void RES_7_L(); void RES_7_aHL(); void RES_7_A();

    void SET_0_B(); void SET_0_C(); void SET_0_D(); void SET_0_E(); void SET_0_H(); void SET_0_L(); void SET_0_aHL(); void SET_0_A();
    void SET_1_B(); void SET_1_C(); void SET_1_D(); void SET_1_E(); void SET_1_H(); void SET_1_L(); void SET_1_aHL(); void SET_1_A();
    void SET_2_B(); void SET_2_C(); void SET_2_D(); void SET_2_E(); void SET_2_H(); void SET_2_L(); void SET_2_aHL(); void SET_2_A();
    void SET_3_B(); void SET_3_C(); void SET_3_D(); void SET_3_E(); void SET_3_H(); void SET_3_L(); void SET_3_aHL(); void SET_3_A();
    void SET_4_B(); void SET_4_C(); void SET_4_D(); void SET_4_E(); void SET_4_H(); void SET_4_L(); void SET_4_aHL(); void SET_4_A();
    void SET_5_B(); void SET_5_C(); void SET_5_D(); void SET_5_E(); void SET_5_H(); void SET_5_L(); void SET_5_aHL(); void SET_5_A();
    void SET_6_B(); void SET_6_C(); void SET_6_D(); void SET_6_E(); void SET_6_H(); void SET_6_L(); void SET_6_aHL(); void SET_6_A();
    void SET_7_B(); void SET_7_C(); void SET_7_D(); void SET_7_E(); void SET_7_H(); void SET_7_L(); void SET_7_aHL(); void SET_7_A();

    void InvalidOp() { std::cerr << "Invalid Opcode at PC=" << std::hex << reg_PC << std::dec << std::endl; }
};