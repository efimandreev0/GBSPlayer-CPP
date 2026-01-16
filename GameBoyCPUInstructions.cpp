#include "GameBoyCPU.h"

// =============================================================
// Control / Misc
// =============================================================
void GameBoyCPU::NOP() { }
void GameBoyCPU::STOP() { } // Usually halts until button press
void GameBoyCPU::HALT() { CPUHalt = true; }
void GameBoyCPU::DI() { IME = false; }
void GameBoyCPU::EI() { IME = true; }

// =============================================================
// Loads (LD)
// =============================================================

void GameBoyCPU::LDH_a8_A() { _memory->Write(0xFF00 + data8(), reg_A); }
void GameBoyCPU::LDH_A_a8() { reg_A = _memory->Read(0xFF00 + data8()); }

void GameBoyCPU::LD_A_a16() { reg_A = _memory->Read(data16()); }

void GameBoyCPU::LD_BC_d16() { set_BC(data16()); }
void GameBoyCPU::LD_DE_d16() { set_DE(data16()); }
void GameBoyCPU::LD_HL_d16() { set_HL(data16()); }
void GameBoyCPU::LD_SP_d16() { reg_SP = data16(); }

void GameBoyCPU::LD_aHLp_A() { LD_aHL_A(); INC_HL(); }
void GameBoyCPU::LD_aHLm_A() { LD_aHL_A(); DEC_HL(); }
void GameBoyCPU::LD_aHL_d8() { _memory->Write(get_HL(), data8()); }
void GameBoyCPU::LD_aBC_A() { _memory->Write(get_BC(), reg_A); }
void GameBoyCPU::LD_aDE_A() { _memory->Write(get_DE(), reg_A); }
void GameBoyCPU::LD_a16_A() { _memory->Write(data16(), reg_A); }

void GameBoyCPU::LD_A_aC() { reg_A = _memory->Read(0xFF00 + reg_C); }
void GameBoyCPU::LD_A_aBC() { reg_A = _memory->Read(get_BC()); }
void GameBoyCPU::LD_A_aDE() { reg_A = _memory->Read(get_DE()); }
void GameBoyCPU::LD_A_aHLp() { LD_A_aHL(); INC_HL(); }
void GameBoyCPU::LD_A_aHLm() { LD_A_aHL(); DEC_HL(); }

void GameBoyCPU::LD_aC_A() { _memory->Write(0xFF00 + reg_C, reg_A); }

void GameBoyCPU::LD_a16_SP() { 
    uint16_t addr = data16();
    _memory->Write(addr, (uint8_t)(reg_SP & 0xFF)); 
    _memory->Write(addr + 1, (uint8_t)(reg_SP >> 8)); 
}

void GameBoyCPU::LD_HL_SPr8() {
    int8_t offset = (int8_t)data8();
    int res = reg_SP + offset;
    
    set_Z(false); 
    set_N(false);
    // H flag: carry from bit 3
    set_H(((reg_SP ^ offset ^ (res & 0xFFFF)) & 0x10) == 0x10);
    // C flag: carry from bit 7
    set_C(((reg_SP ^ offset ^ (res & 0xFFFF)) & 0x100) == 0x100);
    
    set_HL((uint16_t)res);
}

void GameBoyCPU::LD_SP_HL() { reg_SP = get_HL(); }

void GameBoyCPU::LD_A_d8() { reg_A = data8(); }
void GameBoyCPU::LD_B_d8() { reg_B = data8(); }
void GameBoyCPU::LD_C_d8() { reg_C = data8(); }
void GameBoyCPU::LD_D_d8() { reg_D = data8(); }
void GameBoyCPU::LD_E_d8() { reg_E = data8(); }
void GameBoyCPU::LD_H_d8() { reg_H = data8(); }
void GameBoyCPU::LD_L_d8() { reg_L = data8(); }

void GameBoyCPU::LD_aHL_A() { _memory->Write(get_HL(), reg_A); }
void GameBoyCPU::LD_aHL_B() { _memory->Write(get_HL(), reg_B); }
void GameBoyCPU::LD_aHL_C() { _memory->Write(get_HL(), reg_C); }
void GameBoyCPU::LD_aHL_D() { _memory->Write(get_HL(), reg_D); }
void GameBoyCPU::LD_aHL_E() { _memory->Write(get_HL(), reg_E); }
void GameBoyCPU::LD_aHL_H() { _memory->Write(get_HL(), reg_H); }
void GameBoyCPU::LD_aHL_L() { _memory->Write(get_HL(), reg_L); }

void GameBoyCPU::LD_A_A() { }
void GameBoyCPU::LD_A_B() { reg_A = reg_B; }
void GameBoyCPU::LD_A_C() { reg_A = reg_C; }
void GameBoyCPU::LD_A_D() { reg_A = reg_D; }
void GameBoyCPU::LD_A_E() { reg_A = reg_E; }
void GameBoyCPU::LD_A_H() { reg_A = reg_H; }
void GameBoyCPU::LD_A_L() { reg_A = reg_L; }
void GameBoyCPU::LD_A_aHL() { reg_A = _memory->Read(get_HL()); }

void GameBoyCPU::LD_B_A() { reg_B = reg_A; }
void GameBoyCPU::LD_B_B() { }
void GameBoyCPU::LD_B_C() { reg_B = reg_C; }
void GameBoyCPU::LD_B_D() { reg_B = reg_D; }
void GameBoyCPU::LD_B_E() { reg_B = reg_E; }
void GameBoyCPU::LD_B_H() { reg_B = reg_H; }
void GameBoyCPU::LD_B_L() { reg_B = reg_L; }
void GameBoyCPU::LD_B_aHL() { reg_B = _memory->Read(get_HL()); }

void GameBoyCPU::LD_C_A() { reg_C = reg_A; }
void GameBoyCPU::LD_C_B() { reg_C = reg_B; }
void GameBoyCPU::LD_C_C() { }
void GameBoyCPU::LD_C_D() { reg_C = reg_D; }
void GameBoyCPU::LD_C_E() { reg_C = reg_E; }
void GameBoyCPU::LD_C_H() { reg_C = reg_H; }
void GameBoyCPU::LD_C_L() { reg_C = reg_L; }
void GameBoyCPU::LD_C_aHL() { reg_C = _memory->Read(get_HL()); }

void GameBoyCPU::LD_D_A() { reg_D = reg_A; }
void GameBoyCPU::LD_D_B() { reg_D = reg_B; }
void GameBoyCPU::LD_D_C() { reg_D = reg_C; }
void GameBoyCPU::LD_D_D() { }
void GameBoyCPU::LD_D_E() { reg_D = reg_E; }
void GameBoyCPU::LD_D_H() { reg_D = reg_H; }
void GameBoyCPU::LD_D_L() { reg_D = reg_L; }
void GameBoyCPU::LD_D_aHL() { reg_D = _memory->Read(get_HL()); }

void GameBoyCPU::LD_E_A() { reg_E = reg_A; }
void GameBoyCPU::LD_E_B() { reg_E = reg_B; }
void GameBoyCPU::LD_E_C() { reg_E = reg_C; }
void GameBoyCPU::LD_E_D() { reg_E = reg_D; }
void GameBoyCPU::LD_E_E() { }
void GameBoyCPU::LD_E_H() { reg_E = reg_H; }
void GameBoyCPU::LD_E_L() { reg_E = reg_L; }
void GameBoyCPU::LD_E_aHL() { reg_E = _memory->Read(get_HL()); }

void GameBoyCPU::LD_H_A() { reg_H = reg_A; }
void GameBoyCPU::LD_H_B() { reg_H = reg_B; }
void GameBoyCPU::LD_H_C() { reg_H = reg_C; }
void GameBoyCPU::LD_H_D() { reg_H = reg_D; }
void GameBoyCPU::LD_H_E() { reg_H = reg_E; }
void GameBoyCPU::LD_H_H() { }
void GameBoyCPU::LD_H_L() { reg_H = reg_L; }
void GameBoyCPU::LD_H_aHL() { reg_H = _memory->Read(get_HL()); }

void GameBoyCPU::LD_L_A() { reg_L = reg_A; }
void GameBoyCPU::LD_L_B() { reg_L = reg_B; }
void GameBoyCPU::LD_L_C() { reg_L = reg_C; }
void GameBoyCPU::LD_L_D() { reg_L = reg_D; }
void GameBoyCPU::LD_L_E() { reg_L = reg_E; }
void GameBoyCPU::LD_L_H() { reg_L = reg_H; }
void GameBoyCPU::LD_L_L() { }
void GameBoyCPU::LD_L_aHL() { reg_L = _memory->Read(get_HL()); }

// =============================================================
// Increment / Decrement
// =============================================================

void GameBoyCPU::INC_A() { reg_A++; set_Z(reg_A == 0); set_N(false); set_H((reg_A & 0x0F) == 0x00); }
void GameBoyCPU::INC_B() { reg_B++; set_Z(reg_B == 0); set_N(false); set_H((reg_B & 0x0F) == 0x00); }
void GameBoyCPU::INC_C() { reg_C++; set_Z(reg_C == 0); set_N(false); set_H((reg_C & 0x0F) == 0x00); }
void GameBoyCPU::INC_D() { reg_D++; set_Z(reg_D == 0); set_N(false); set_H((reg_D & 0x0F) == 0x00); }
void GameBoyCPU::INC_E() { reg_E++; set_Z(reg_E == 0); set_N(false); set_H((reg_E & 0x0F) == 0x00); }
void GameBoyCPU::INC_H() { reg_H++; set_Z(reg_H == 0); set_N(false); set_H((reg_H & 0x0F) == 0x00); }
void GameBoyCPU::INC_L() { reg_L++; set_Z(reg_L == 0); set_N(false); set_H((reg_L & 0x0F) == 0x00); }
void GameBoyCPU::INC_aHL() { 
    uint8_t val = _memory->Read(get_HL()); 
    val++; 
    _memory->Write(get_HL(), val); 
    set_Z(val == 0); set_N(false); set_H((val & 0x0F) == 0x00); 
}

void GameBoyCPU::INC_BC() { set_BC(get_BC() + 1); }
void GameBoyCPU::INC_DE() { set_DE(get_DE() + 1); }
void GameBoyCPU::INC_HL() { set_HL(get_HL() + 1); }
void GameBoyCPU::INC_SP() { reg_SP++; }

void GameBoyCPU::DEC_A() { reg_A--; set_Z(reg_A == 0); set_N(true); set_H((reg_A & 0x0F) == 0x0F); }
void GameBoyCPU::DEC_B() { reg_B--; set_Z(reg_B == 0); set_N(true); set_H((reg_B & 0x0F) == 0x0F); }
void GameBoyCPU::DEC_C() { reg_C--; set_Z(reg_C == 0); set_N(true); set_H((reg_C & 0x0F) == 0x0F); }
void GameBoyCPU::DEC_D() { reg_D--; set_Z(reg_D == 0); set_N(true); set_H((reg_D & 0x0F) == 0x0F); }
void GameBoyCPU::DEC_E() { reg_E--; set_Z(reg_E == 0); set_N(true); set_H((reg_E & 0x0F) == 0x0F); }
void GameBoyCPU::DEC_H() { reg_H--; set_Z(reg_H == 0); set_N(true); set_H((reg_H & 0x0F) == 0x0F); }
void GameBoyCPU::DEC_L() { reg_L--; set_Z(reg_L == 0); set_N(true); set_H((reg_L & 0x0F) == 0x0F); }
void GameBoyCPU::DEC_aHL() { 
    uint8_t val = _memory->Read(get_HL()); 
    val--; 
    _memory->Write(get_HL(), val);
    set_Z(val == 0); set_N(true); set_H((val & 0x0F) == 0x0F); 
}

void GameBoyCPU::DEC_BC() { set_BC(get_BC() - 1); }
void GameBoyCPU::DEC_DE() { set_DE(get_DE() - 1); }
void GameBoyCPU::DEC_HL() { set_HL(get_HL() - 1); }
void GameBoyCPU::DEC_SP() { reg_SP--; }

// =============================================================
// Arithmetic (ADD/ADC/SUB/SBC)
// =============================================================

void GameBoyCPU::ADD_A_A() { set_H(((reg_A & 0xF) + (reg_A & 0xF)) > 0xF); int sum = reg_A + reg_A; set_C(sum > 0xFF); reg_A = (uint8_t)sum; set_Z(reg_A == 0); set_N(false); }
void GameBoyCPU::ADD_A_B() { set_H(((reg_A & 0xF) + (reg_B & 0xF)) > 0xF); int sum = reg_A + reg_B; set_C(sum > 0xFF); reg_A = (uint8_t)sum; set_Z(reg_A == 0); set_N(false); }
void GameBoyCPU::ADD_A_C() { set_H(((reg_A & 0xF) + (reg_C & 0xF)) > 0xF); int sum = reg_A + reg_C; set_C(sum > 0xFF); reg_A = (uint8_t)sum; set_Z(reg_A == 0); set_N(false); }
void GameBoyCPU::ADD_A_D() { set_H(((reg_A & 0xF) + (reg_D & 0xF)) > 0xF); int sum = reg_A + reg_D; set_C(sum > 0xFF); reg_A = (uint8_t)sum; set_Z(reg_A == 0); set_N(false); }
void GameBoyCPU::ADD_A_E() { set_H(((reg_A & 0xF) + (reg_E & 0xF)) > 0xF); int sum = reg_A + reg_E; set_C(sum > 0xFF); reg_A = (uint8_t)sum; set_Z(reg_A == 0); set_N(false); }
void GameBoyCPU::ADD_A_H() { set_H(((reg_A & 0xF) + (reg_H & 0xF)) > 0xF); int sum = reg_A + reg_H; set_C(sum > 0xFF); reg_A = (uint8_t)sum; set_Z(reg_A == 0); set_N(false); }
void GameBoyCPU::ADD_A_L() { set_H(((reg_A & 0xF) + (reg_L & 0xF)) > 0xF); int sum = reg_A + reg_L; set_C(sum > 0xFF); reg_A = (uint8_t)sum; set_Z(reg_A == 0); set_N(false); }
void GameBoyCPU::ADD_A_d8() { uint8_t d = data8(); set_H(((reg_A & 0xF) + (d & 0xF)) > 0xF); int sum = reg_A + d; set_C(sum > 0xFF); reg_A = (uint8_t)sum; set_Z(reg_A == 0); set_N(false); }
void GameBoyCPU::ADD_A_aHL() { uint8_t val = _memory->Read(get_HL()); set_H(((reg_A & 0xF) + (val & 0xF)) > 0xF); int sum = reg_A + val; set_C(sum > 0xFF); reg_A = (uint8_t)sum; set_Z(reg_A == 0); set_N(false); }

void GameBoyCPU::ADD_SP_r8() {
    int8_t d = (int8_t)data8();
    set_Z(false); set_N(false);
    set_H(((reg_SP ^ d ^ ((reg_SP + d) & 0xFFFF)) & 0x10) == 0x10);
    set_C(((reg_SP ^ d ^ ((reg_SP + d) & 0xFFFF)) & 0x100) == 0x100);
    reg_SP = (uint16_t)(reg_SP + d);
}

void GameBoyCPU::ADC_A_A() { int sum = reg_A + reg_A + get_CBit(); set_H(((reg_A & 0xF) + (reg_A & 0xF) + get_CBit()) > 0xF); set_C(sum > 0xFF); reg_A = (uint8_t)sum; set_Z(reg_A == 0); set_N(false); }
void GameBoyCPU::ADC_A_B() { int sum = reg_A + reg_B + get_CBit(); set_H(((reg_A & 0xF) + (reg_B & 0xF) + get_CBit()) > 0xF); set_C(sum > 0xFF); reg_A = (uint8_t)sum; set_Z(reg_A == 0); set_N(false); }
void GameBoyCPU::ADC_A_C() { int sum = reg_A + reg_C + get_CBit(); set_H(((reg_A & 0xF) + (reg_C & 0xF) + get_CBit()) > 0xF); set_C(sum > 0xFF); reg_A = (uint8_t)sum; set_Z(reg_A == 0); set_N(false); }
void GameBoyCPU::ADC_A_D() { int sum = reg_A + reg_D + get_CBit(); set_H(((reg_A & 0xF) + (reg_D & 0xF) + get_CBit()) > 0xF); set_C(sum > 0xFF); reg_A = (uint8_t)sum; set_Z(reg_A == 0); set_N(false); }
void GameBoyCPU::ADC_A_E() { int sum = reg_A + reg_E + get_CBit(); set_H(((reg_A & 0xF) + (reg_E & 0xF) + get_CBit()) > 0xF); set_C(sum > 0xFF); reg_A = (uint8_t)sum; set_Z(reg_A == 0); set_N(false); }
void GameBoyCPU::ADC_A_H() { int sum = reg_A + reg_H + get_CBit(); set_H(((reg_A & 0xF) + (reg_H & 0xF) + get_CBit()) > 0xF); set_C(sum > 0xFF); reg_A = (uint8_t)sum; set_Z(reg_A == 0); set_N(false); }
void GameBoyCPU::ADC_A_L() { int sum = reg_A + reg_L + get_CBit(); set_H(((reg_A & 0xF) + (reg_L & 0xF) + get_CBit()) > 0xF); set_C(sum > 0xFF); reg_A = (uint8_t)sum; set_Z(reg_A == 0); set_N(false); }
void GameBoyCPU::ADC_A_d8() { uint8_t d = data8(); int sum = reg_A + d + get_CBit(); set_H(((reg_A & 0xF) + (d & 0xF) + get_CBit()) > 0xF); set_C(sum > 0xFF); reg_A = (uint8_t)sum; set_Z(reg_A == 0); set_N(false); }
void GameBoyCPU::ADC_A_aHL() { uint8_t val = _memory->Read(get_HL()); int sum = reg_A + val + get_CBit(); set_H(((reg_A & 0xF) + (val & 0xF) + get_CBit()) > 0xF); set_C(sum > 0xFF); reg_A = (uint8_t)sum; set_Z(reg_A == 0); set_N(false); }

void GameBoyCPU::ADD_HL_BC() { uint16_t bc = get_BC(); uint16_t hl = get_HL(); set_N(false); set_H(((hl & 0xFFF) + (bc & 0xFFF)) > 0xFFF); set_C((hl + bc) > 0xFFFF); set_HL(hl + bc); }
void GameBoyCPU::ADD_HL_DE() { uint16_t de = get_DE(); uint16_t hl = get_HL(); set_N(false); set_H(((hl & 0xFFF) + (de & 0xFFF)) > 0xFFF); set_C((hl + de) > 0xFFFF); set_HL(hl + de); }
void GameBoyCPU::ADD_HL_HL() { uint16_t hl = get_HL(); set_N(false); set_H(((hl & 0xFFF) + (hl & 0xFFF)) > 0xFFF); set_C((hl + hl) > 0xFFFF); set_HL(hl + hl); }
void GameBoyCPU::ADD_HL_SP() { uint16_t hl = get_HL(); set_N(false); set_H(((hl & 0xFFF) + (reg_SP & 0xFFF)) > 0xFFF); set_C((hl + reg_SP) > 0xFFFF); set_HL(hl + reg_SP); }

void GameBoyCPU::SUB_A() { set_H(false); set_C(false); reg_A = 0; set_Z(true); set_N(true); }
void GameBoyCPU::SUB_B() { set_H((reg_A & 0xF) < (reg_B & 0xF)); set_C(reg_A < reg_B); reg_A -= reg_B; set_Z(reg_A == 0); set_N(true); }
void GameBoyCPU::SUB_C() { set_H((reg_A & 0xF) < (reg_C & 0xF)); set_C(reg_A < reg_C); reg_A -= reg_C; set_Z(reg_A == 0); set_N(true); }
void GameBoyCPU::SUB_D() { set_H((reg_A & 0xF) < (reg_D & 0xF)); set_C(reg_A < reg_D); reg_A -= reg_D; set_Z(reg_A == 0); set_N(true); }
void GameBoyCPU::SUB_E() { set_H((reg_A & 0xF) < (reg_E & 0xF)); set_C(reg_A < reg_E); reg_A -= reg_E; set_Z(reg_A == 0); set_N(true); }
void GameBoyCPU::SUB_H() { set_H((reg_A & 0xF) < (reg_H & 0xF)); set_C(reg_A < reg_H); reg_A -= reg_H; set_Z(reg_A == 0); set_N(true); }
void GameBoyCPU::SUB_L() { set_H((reg_A & 0xF) < (reg_L & 0xF)); set_C(reg_A < reg_L); reg_A -= reg_L; set_Z(reg_A == 0); set_N(true); }
void GameBoyCPU::SUB_d8() { uint8_t d = data8(); set_H((reg_A & 0xF) < (d & 0xF)); set_C(reg_A < d); reg_A -= d; set_Z(reg_A == 0); set_N(true); }
void GameBoyCPU::SUB_aHL() { uint8_t val = _memory->Read(get_HL()); set_H((reg_A & 0xF) < (val & 0xF)); set_C(reg_A < val); reg_A -= val; set_Z(reg_A == 0); set_N(true); }

void GameBoyCPU::SBC_A_A() { int temp = reg_A + get_CBit(); set_H((reg_A & 0xF) < ((reg_A & 0xF) + get_CBit())); set_C(reg_A < temp); reg_A = (uint8_t)(reg_A - temp); set_Z(reg_A == 0); set_N(true); }
void GameBoyCPU::SBC_A_B() { int temp = reg_B + get_CBit(); set_H((reg_A & 0xF) < ((reg_B & 0xF) + get_CBit())); set_C(reg_A < temp); reg_A = (uint8_t)(reg_A - temp); set_Z(reg_A == 0); set_N(true); }
void GameBoyCPU::SBC_A_C() { int temp = reg_C + get_CBit(); set_H((reg_A & 0xF) < ((reg_C & 0xF) + get_CBit())); set_C(reg_A < temp); reg_A = (uint8_t)(reg_A - temp); set_Z(reg_A == 0); set_N(true); }
void GameBoyCPU::SBC_A_D() { int temp = reg_D + get_CBit(); set_H((reg_A & 0xF) < ((reg_D & 0xF) + get_CBit())); set_C(reg_A < temp); reg_A = (uint8_t)(reg_A - temp); set_Z(reg_A == 0); set_N(true); }
void GameBoyCPU::SBC_A_E() { int temp = reg_E + get_CBit(); set_H((reg_A & 0xF) < ((reg_E & 0xF) + get_CBit())); set_C(reg_A < temp); reg_A = (uint8_t)(reg_A - temp); set_Z(reg_A == 0); set_N(true); }
void GameBoyCPU::SBC_A_H() { int temp = reg_H + get_CBit(); set_H((reg_A & 0xF) < ((reg_H & 0xF) + get_CBit())); set_C(reg_A < temp); reg_A = (uint8_t)(reg_A - temp); set_Z(reg_A == 0); set_N(true); }
void GameBoyCPU::SBC_A_L() { int temp = reg_L + get_CBit(); set_H((reg_A & 0xF) < ((reg_L & 0xF) + get_CBit())); set_C(reg_A < temp); reg_A = (uint8_t)(reg_A - temp); set_Z(reg_A == 0); set_N(true); }
void GameBoyCPU::SBC_A_d8() { int temp = data8() + get_CBit(); set_H((reg_A & 0xF) < ((data8() & 0xF) + get_CBit())); set_C(reg_A < temp); reg_A = (uint8_t)(reg_A - temp); set_Z(reg_A == 0); set_N(true); }
void GameBoyCPU::SBC_A_aHL() { uint8_t val = _memory->Read(get_HL()); int temp = val + get_CBit(); set_H((reg_A & 0xF) < ((val & 0xF) + get_CBit())); set_C(reg_A < temp); reg_A = (uint8_t)(reg_A - temp); set_Z(reg_A == 0); set_N(true); }

// =============================================================
// Logic (AND/OR/XOR/CP)
// =============================================================

void GameBoyCPU::AND_A() { reg_A &= reg_A; set_Z(reg_A == 0); set_N(false); set_H(true); set_C(false); }
void GameBoyCPU::AND_B() { reg_A &= reg_B; set_Z(reg_A == 0); set_N(false); set_H(true); set_C(false); }
void GameBoyCPU::AND_C() { reg_A &= reg_C; set_Z(reg_A == 0); set_N(false); set_H(true); set_C(false); }
void GameBoyCPU::AND_D() { reg_A &= reg_D; set_Z(reg_A == 0); set_N(false); set_H(true); set_C(false); }
void GameBoyCPU::AND_E() { reg_A &= reg_E; set_Z(reg_A == 0); set_N(false); set_H(true); set_C(false); }
void GameBoyCPU::AND_H() { reg_A &= reg_H; set_Z(reg_A == 0); set_N(false); set_H(true); set_C(false); }
void GameBoyCPU::AND_L() { reg_A &= reg_L; set_Z(reg_A == 0); set_N(false); set_H(true); set_C(false); }
void GameBoyCPU::AND_d8() { reg_A &= data8(); set_Z(reg_A == 0); set_N(false); set_H(true); set_C(false); }
void GameBoyCPU::AND_aHL() { reg_A &= _memory->Read(get_HL()); set_Z(reg_A == 0); set_N(false); set_H(true); set_C(false); }

void GameBoyCPU::OR_A() { reg_A |= reg_A; set_Z(reg_A == 0); set_N(false); set_H(false); set_C(false); }
void GameBoyCPU::OR_B() { reg_A |= reg_B; set_Z(reg_A == 0); set_N(false); set_H(false); set_C(false); }
void GameBoyCPU::OR_C() { reg_A |= reg_C; set_Z(reg_A == 0); set_N(false); set_H(false); set_C(false); }
void GameBoyCPU::OR_D() { reg_A |= reg_D; set_Z(reg_A == 0); set_N(false); set_H(false); set_C(false); }
void GameBoyCPU::OR_E() { reg_A |= reg_E; set_Z(reg_A == 0); set_N(false); set_H(false); set_C(false); }
void GameBoyCPU::OR_H() { reg_A |= reg_H; set_Z(reg_A == 0); set_N(false); set_H(false); set_C(false); }
void GameBoyCPU::OR_L() { reg_A |= reg_L; set_Z(reg_A == 0); set_N(false); set_H(false); set_C(false); }
void GameBoyCPU::OR_d8() { reg_A |= data8(); set_Z(reg_A == 0); set_N(false); set_H(false); set_C(false); }
void GameBoyCPU::OR_aHL() { reg_A |= _memory->Read(get_HL()); set_Z(reg_A == 0); set_N(false); set_H(false); set_C(false); }

void GameBoyCPU::XOR_A() { reg_A ^= reg_A; set_Z(reg_A == 0); set_N(false); set_H(false); set_C(false); }
void GameBoyCPU::XOR_B() { reg_A ^= reg_B; set_Z(reg_A == 0); set_N(false); set_H(false); set_C(false); }
void GameBoyCPU::XOR_C() { reg_A ^= reg_C; set_Z(reg_A == 0); set_N(false); set_H(false); set_C(false); }
void GameBoyCPU::XOR_D() { reg_A ^= reg_D; set_Z(reg_A == 0); set_N(false); set_H(false); set_C(false); }
void GameBoyCPU::XOR_E() { reg_A ^= reg_E; set_Z(reg_A == 0); set_N(false); set_H(false); set_C(false); }
void GameBoyCPU::XOR_H() { reg_A ^= reg_H; set_Z(reg_A == 0); set_N(false); set_H(false); set_C(false); }
void GameBoyCPU::XOR_L() { reg_A ^= reg_L; set_Z(reg_A == 0); set_N(false); set_H(false); set_C(false); }
void GameBoyCPU::XOR_d8() { reg_A ^= data8(); set_Z(reg_A == 0); set_N(false); set_H(false); set_C(false); }
void GameBoyCPU::XOR_aHL() { reg_A ^= _memory->Read(get_HL()); set_Z(reg_A == 0); set_N(false); set_H(false); set_C(false); }

void GameBoyCPU::CP_A() { set_Z(true); set_N(true); set_H(false); set_C(false); }
void GameBoyCPU::CP_B() { set_Z(reg_A == reg_B); set_N(true); set_H((reg_A & 0xF) < (reg_B & 0xF)); set_C(reg_A < reg_B); }
void GameBoyCPU::CP_C() { set_Z(reg_A == reg_C); set_N(true); set_H((reg_A & 0xF) < (reg_C & 0xF)); set_C(reg_A < reg_C); }
void GameBoyCPU::CP_D() { set_Z(reg_A == reg_D); set_N(true); set_H((reg_A & 0xF) < (reg_D & 0xF)); set_C(reg_A < reg_D); }
void GameBoyCPU::CP_E() { set_Z(reg_A == reg_E); set_N(true); set_H((reg_A & 0xF) < (reg_E & 0xF)); set_C(reg_A < reg_E); }
void GameBoyCPU::CP_H() { set_Z(reg_A == reg_H); set_N(true); set_H((reg_A & 0xF) < (reg_H & 0xF)); set_C(reg_A < reg_H); }
void GameBoyCPU::CP_L() { set_Z(reg_A == reg_L); set_N(true); set_H((reg_A & 0xF) < (reg_L & 0xF)); set_C(reg_A < reg_L); }
void GameBoyCPU::CP_d8() { uint8_t d = data8(); set_Z(reg_A == d); set_N(true); set_H((reg_A & 0xF) < (d & 0xF)); set_C(reg_A < d); }
void GameBoyCPU::CP_aHL() { uint8_t val = _memory->Read(get_HL()); set_Z(reg_A == val); set_N(true); set_H((reg_A & 0xF) < (val & 0xF)); set_C(reg_A < val); }

// =============================================================
// Misc / DAA / SCF / CPL
// =============================================================

void GameBoyCPU::DAA() {
    int val = reg_A;
    if (!get_N()) {
        if (get_H() || (val & 0xF) > 9) val += 0x06;
        if (get_C() || val > 0x9F) val += 0x60;
    } else {
        if (get_H()) val = (val - 6) & 0xFF;
        if (get_C()) val -= 0x60;
    }
    set_H(false);
    set_Z((val & 0xFF) == 0);
    if ((val & 0x100) == 0x100) set_C(true);
    reg_A = (uint8_t)(val & 0xFF);
}

void GameBoyCPU::CPL() { reg_A = ~reg_A; set_N(true); set_H(true); }
void GameBoyCPU::SCF() { set_N(false); set_H(false); set_C(true); }
void GameBoyCPU::CCF() { set_N(false); set_H(false); set_C(!get_C()); }

// =============================================================
// Jumps / Calls
// =============================================================

void GameBoyCPU::JP_a16() { reg_PC = data16(); }
void GameBoyCPU::JP_aHL() { reg_PC = get_HL(); }
void GameBoyCPU::JP_NZ_a16() { if (!get_Z()) { JP_a16(); cycleCount += 4; } }
void GameBoyCPU::JP_Z_a16() { if (get_Z()) { JP_a16(); cycleCount += 4; } }
void GameBoyCPU::JP_NC_a16() { if (!get_C()) { JP_a16(); cycleCount += 4; } }
void GameBoyCPU::JP_C_a16() { if (get_C()) { JP_a16(); cycleCount += 4; } }

void GameBoyCPU::JR_d8() { reg_PC = (uint16_t)(reg_PC + (int8_t)data8()); }
void GameBoyCPU::JR_NZ_a8() { if (!get_Z()) { JR_d8(); cycleCount += 4; } }
void GameBoyCPU::JR_Z_a8() { if (get_Z()) { JR_d8(); cycleCount += 4; } }
void GameBoyCPU::JR_NC_a8() { if (!get_C()) { JR_d8(); cycleCount += 4; } }
void GameBoyCPU::JR_C_a8() { if (get_C()) { JR_d8(); cycleCount += 4; } }

void GameBoyCPU::RET()
{
    uint8_t lo = _memory->Read(reg_SP++);
    uint8_t hi = _memory->Read(reg_SP++);

    reg_PC = (uint16_t)((hi << 8) | lo);
}
void GameBoyCPU::RET_NZ() { if (!get_Z()) { RET(); cycleCount += 12; } }
void GameBoyCPU::RET_Z() { if (get_Z()) { RET(); cycleCount += 12; } }
void GameBoyCPU::RET_NC() { if (!get_C()) { RET(); cycleCount += 12; } }
void GameBoyCPU::RET_C() { if (get_C()) { RET(); cycleCount += 12; } }
void GameBoyCPU::RETI() { RET(); EI(); }

void GameBoyCPU::CALL_a16() { 
    uint16_t target = data16(); 
    _memory->Write(--reg_SP, (uint8_t)(reg_PC >> 8)); 
    _memory->Write(--reg_SP, (uint8_t)(reg_PC & 0xFF)); 
    reg_PC = target; 
}
void GameBoyCPU::CALL_NZ_a16() { if (!get_Z()) { CALL_a16(); cycleCount += 12; } }
void GameBoyCPU::CALL_Z_a16() { if (get_Z()) { CALL_a16(); cycleCount += 12; } }
void GameBoyCPU::CALL_NC_a16() { if (!get_C()) { CALL_a16(); cycleCount += 12; } }
void GameBoyCPU::CALL_C_a16() { if (get_C()) { CALL_a16(); cycleCount += 12; } }

void GameBoyCPU::RST_00H() { _memory->Write(--reg_SP, (uint8_t)(reg_PC >> 8)); _memory->Write(--reg_SP, (uint8_t)(reg_PC & 0xFF)); reg_PC = 0x0000 + _cartridge->LoadAddress; }
void GameBoyCPU::RST_08H() { _memory->Write(--reg_SP, (uint8_t)(reg_PC >> 8)); _memory->Write(--reg_SP, (uint8_t)(reg_PC & 0xFF)); reg_PC = 0x0008 + _cartridge->LoadAddress; }
void GameBoyCPU::RST_10H() { _memory->Write(--reg_SP, (uint8_t)(reg_PC >> 8)); _memory->Write(--reg_SP, (uint8_t)(reg_PC & 0xFF)); reg_PC = 0x0010 + _cartridge->LoadAddress; }
void GameBoyCPU::RST_18H() { _memory->Write(--reg_SP, (uint8_t)(reg_PC >> 8)); _memory->Write(--reg_SP, (uint8_t)(reg_PC & 0xFF)); reg_PC = 0x0018 + _cartridge->LoadAddress; }
void GameBoyCPU::RST_20H() { _memory->Write(--reg_SP, (uint8_t)(reg_PC >> 8)); _memory->Write(--reg_SP, (uint8_t)(reg_PC & 0xFF)); reg_PC = 0x0020 + _cartridge->LoadAddress; }
void GameBoyCPU::RST_28H() { _memory->Write(--reg_SP, (uint8_t)(reg_PC >> 8)); _memory->Write(--reg_SP, (uint8_t)(reg_PC & 0xFF)); reg_PC = 0x0028 + _cartridge->LoadAddress; }
void GameBoyCPU::RST_30H() { _memory->Write(--reg_SP, (uint8_t)(reg_PC >> 8)); _memory->Write(--reg_SP, (uint8_t)(reg_PC & 0xFF)); reg_PC = 0x0030 + _cartridge->LoadAddress; }
void GameBoyCPU::RST_38H() { _memory->Write(--reg_SP, (uint8_t)(reg_PC >> 8)); _memory->Write(--reg_SP, (uint8_t)(reg_PC & 0xFF)); reg_PC = 0x0038 + _cartridge->LoadAddress; }

// =============================================================
// Stack
// =============================================================

void GameBoyCPU::POP_AF() { reg_F = (uint8_t)(_memory->Read(reg_SP++) & 0xF0); reg_A = _memory->Read(reg_SP++); }
void GameBoyCPU::POP_BC() { reg_C = _memory->Read(reg_SP++); reg_B = _memory->Read(reg_SP++); }
void GameBoyCPU::POP_DE() { reg_E = _memory->Read(reg_SP++); reg_D = _memory->Read(reg_SP++); }
void GameBoyCPU::POP_HL() { reg_L = _memory->Read(reg_SP++); reg_H = _memory->Read(reg_SP++); }

void GameBoyCPU::PUSH_AF() { _memory->Write(--reg_SP, reg_A); _memory->Write(--reg_SP, reg_F); }
void GameBoyCPU::PUSH_BC() { _memory->Write(--reg_SP, reg_B); _memory->Write(--reg_SP, reg_C); }
void GameBoyCPU::PUSH_DE() { _memory->Write(--reg_SP, reg_D); _memory->Write(--reg_SP, reg_E); }
void GameBoyCPU::PUSH_HL() { _memory->Write(--reg_SP, reg_H); _memory->Write(--reg_SP, reg_L); }

// =============================================================
// Rotates / Shifts / CB Prefix
// =============================================================

void GameBoyCPU::RLCA() { set_C((reg_A & 0x80) == 0x80); reg_A = (reg_A << 1) | (reg_A >> 7); set_Z(false); set_N(false); set_H(false); }
void GameBoyCPU::RRCA() { set_C((reg_A & 0x01) == 0x01); reg_A = (reg_A >> 1) | ((reg_A & 0x01) << 7); set_Z(false); set_N(false); set_H(false); }
void GameBoyCPU::RLA() { uint8_t temp = reg_A; reg_A = (reg_A << 1) | get_CBit(); set_Z(false); set_N(false); set_H(false); set_C((temp & 0x80) == 0x80); }
void GameBoyCPU::RRA() { uint8_t temp = reg_A; reg_A = (reg_A >> 1) | (get_C() ? 0x80 : 0); set_Z(false); set_N(false); set_H(false); set_C((temp & 0x01) == 0x01); }

void GameBoyCPU::RLC_A() { set_C((reg_A & 0x80) == 0x80); reg_A = (reg_A << 1) | (reg_A >> 7); set_Z(reg_A == 0); set_N(false); set_H(false); }
void GameBoyCPU::RLC_B() { set_C((reg_B & 0x80) == 0x80); reg_B = (reg_B << 1) | (reg_B >> 7); set_Z(reg_B == 0); set_N(false); set_H(false); }
void GameBoyCPU::RLC_C() { set_C((reg_C & 0x80) == 0x80); reg_C = (reg_C << 1) | (reg_C >> 7); set_Z(reg_C == 0); set_N(false); set_H(false); }
void GameBoyCPU::RLC_D() { set_C((reg_D & 0x80) == 0x80); reg_D = (reg_D << 1) | (reg_D >> 7); set_Z(reg_D == 0); set_N(false); set_H(false); }
void GameBoyCPU::RLC_E() { set_C((reg_E & 0x80) == 0x80); reg_E = (reg_E << 1) | (reg_E >> 7); set_Z(reg_E == 0); set_N(false); set_H(false); }
void GameBoyCPU::RLC_H() { set_C((reg_H & 0x80) == 0x80); reg_H = (reg_H << 1) | (reg_H >> 7); set_Z(reg_H == 0); set_N(false); set_H(false); }
void GameBoyCPU::RLC_L() { set_C((reg_L & 0x80) == 0x80); reg_L = (reg_L << 1) | (reg_L >> 7); set_Z(reg_L == 0); set_N(false); set_H(false); }
void GameBoyCPU::RLC_aHL() { uint8_t val = _memory->Read(get_HL()); set_C((val & 0x80) == 0x80); val = (val << 1) | (val >> 7); _memory->Write(get_HL(), val); set_Z(val == 0); set_N(false); set_H(false); }

void GameBoyCPU::RRC_A() { set_C((reg_A & 0x01) == 0x01); reg_A = (reg_A >> 1) | ((reg_A & 0x01) << 7); set_Z(reg_A == 0); set_N(false); set_H(false); }
void GameBoyCPU::RRC_B() { set_C((reg_B & 0x01) == 0x01); reg_B = (reg_B >> 1) | ((reg_B & 0x01) << 7); set_Z(reg_B == 0); set_N(false); set_H(false); }
void GameBoyCPU::RRC_C() { set_C((reg_C & 0x01) == 0x01); reg_C = (reg_C >> 1) | ((reg_C & 0x01) << 7); set_Z(reg_C == 0); set_N(false); set_H(false); }
void GameBoyCPU::RRC_D() { set_C((reg_D & 0x01) == 0x01); reg_D = (reg_D >> 1) | ((reg_D & 0x01) << 7); set_Z(reg_D == 0); set_N(false); set_H(false); }
void GameBoyCPU::RRC_E() { set_C((reg_E & 0x01) == 0x01); reg_E = (reg_E >> 1) | ((reg_E & 0x01) << 7); set_Z(reg_E == 0); set_N(false); set_H(false); }
void GameBoyCPU::RRC_H() { set_C((reg_H & 0x01) == 0x01); reg_H = (reg_H >> 1) | ((reg_H & 0x01) << 7); set_Z(reg_H == 0); set_N(false); set_H(false); }
void GameBoyCPU::RRC_L() { set_C((reg_L & 0x01) == 0x01); reg_L = (reg_L >> 1) | ((reg_L & 0x01) << 7); set_Z(reg_L == 0); set_N(false); set_H(false); }
void GameBoyCPU::RRC_aHL() { uint8_t val = _memory->Read(get_HL()); set_C((val & 0x01) == 0x01); val = (val >> 1) | ((val & 0x01) << 7); _memory->Write(get_HL(), val); set_Z(val == 0); set_N(false); set_H(false); }

void GameBoyCPU::RL_A() { uint8_t t = reg_A; reg_A = (reg_A << 1) | get_CBit(); set_Z(reg_A == 0); set_N(false); set_H(false); set_C((t & 0x80) == 0x80); }
void GameBoyCPU::RL_B() { uint8_t t = reg_B; reg_B = (reg_B << 1) | get_CBit(); set_Z(reg_B == 0); set_N(false); set_H(false); set_C((t & 0x80) == 0x80); }
void GameBoyCPU::RL_C() { uint8_t t = reg_C; reg_C = (reg_C << 1) | get_CBit(); set_Z(reg_C == 0); set_N(false); set_H(false); set_C((t & 0x80) == 0x80); }
void GameBoyCPU::RL_D() { uint8_t t = reg_D; reg_D = (reg_D << 1) | get_CBit(); set_Z(reg_D == 0); set_N(false); set_H(false); set_C((t & 0x80) == 0x80); }
void GameBoyCPU::RL_E() { uint8_t t = reg_E; reg_E = (reg_E << 1) | get_CBit(); set_Z(reg_E == 0); set_N(false); set_H(false); set_C((t & 0x80) == 0x80); }
void GameBoyCPU::RL_H() { uint8_t t = reg_H; reg_H = (reg_H << 1) | get_CBit(); set_Z(reg_H == 0); set_N(false); set_H(false); set_C((t & 0x80) == 0x80); }
void GameBoyCPU::RL_L() { uint8_t t = reg_L; reg_L = (reg_L << 1) | get_CBit(); set_Z(reg_L == 0); set_N(false); set_H(false); set_C((t & 0x80) == 0x80); }
void GameBoyCPU::RL_aHL() { uint8_t val = _memory->Read(get_HL()); uint8_t t = val; val = (val << 1) | get_CBit(); _memory->Write(get_HL(), val); set_Z(val == 0); set_N(false); set_H(false); set_C((t & 0x80) == 0x80); }

void GameBoyCPU::RR_A() { uint8_t t = reg_A; reg_A = (reg_A >> 1) | (get_C() ? 0x80 : 0); set_Z(reg_A == 0); set_N(false); set_H(false); set_C((t & 0x01) == 0x01); }
void GameBoyCPU::RR_B() { uint8_t t = reg_B; reg_B = (reg_B >> 1) | (get_C() ? 0x80 : 0); set_Z(reg_B == 0); set_N(false); set_H(false); set_C((t & 0x01) == 0x01); }
void GameBoyCPU::RR_C() { uint8_t t = reg_C; reg_C = (reg_C >> 1) | (get_C() ? 0x80 : 0); set_Z(reg_C == 0); set_N(false); set_H(false); set_C((t & 0x01) == 0x01); }
void GameBoyCPU::RR_D() { uint8_t t = reg_D; reg_D = (reg_D >> 1) | (get_C() ? 0x80 : 0); set_Z(reg_D == 0); set_N(false); set_H(false); set_C((t & 0x01) == 0x01); }
void GameBoyCPU::RR_E() { uint8_t t = reg_E; reg_E = (reg_E >> 1) | (get_C() ? 0x80 : 0); set_Z(reg_E == 0); set_N(false); set_H(false); set_C((t & 0x01) == 0x01); }
void GameBoyCPU::RR_H() { uint8_t t = reg_H; reg_H = (reg_H >> 1) | (get_C() ? 0x80 : 0); set_Z(reg_H == 0); set_N(false); set_H(false); set_C((t & 0x01) == 0x01); }
void GameBoyCPU::RR_L() { uint8_t t = reg_L; reg_L = (reg_L >> 1) | (get_C() ? 0x80 : 0); set_Z(reg_L == 0); set_N(false); set_H(false); set_C((t & 0x01) == 0x01); }
void GameBoyCPU::RR_aHL() { uint8_t val = _memory->Read(get_HL()); uint8_t t = val; val = (val >> 1) | (get_C() ? 0x80 : 0); _memory->Write(get_HL(), val); set_Z(val == 0); set_N(false); set_H(false); set_C((t & 0x01) == 0x01); }

void GameBoyCPU::SLA_A() { set_C((reg_A & 0x80) == 0x80); reg_A <<= 1; set_Z(reg_A == 0); set_N(false); set_H(false); }
void GameBoyCPU::SLA_B() { set_C((reg_B & 0x80) == 0x80); reg_B <<= 1; set_Z(reg_B == 0); set_N(false); set_H(false); }
void GameBoyCPU::SLA_C() { set_C((reg_C & 0x80) == 0x80); reg_C <<= 1; set_Z(reg_C == 0); set_N(false); set_H(false); }
void GameBoyCPU::SLA_D() { set_C((reg_D & 0x80) == 0x80); reg_D <<= 1; set_Z(reg_D == 0); set_N(false); set_H(false); }
void GameBoyCPU::SLA_E() { set_C((reg_E & 0x80) == 0x80); reg_E <<= 1; set_Z(reg_E == 0); set_N(false); set_H(false); }
void GameBoyCPU::SLA_H() { set_C((reg_H & 0x80) == 0x80); reg_H <<= 1; set_Z(reg_H == 0); set_N(false); set_H(false); }
void GameBoyCPU::SLA_L() { set_C((reg_L & 0x80) == 0x80); reg_L <<= 1; set_Z(reg_L == 0); set_N(false); set_H(false); }
void GameBoyCPU::SLA_aHL() { uint8_t v = _memory->Read(get_HL()); set_C((v & 0x80) == 0x80); v <<= 1; _memory->Write(get_HL(), v); set_Z(v == 0); set_N(false); set_H(false); }

void GameBoyCPU::SRA_A() { set_C((reg_A & 0x01) == 0x01); reg_A = (reg_A >> 1) | (reg_A & 0x80); set_Z(reg_A == 0); set_N(false); set_H(false); }
void GameBoyCPU::SRA_B() { set_C((reg_B & 0x01) == 0x01); reg_B = (reg_B >> 1) | (reg_B & 0x80); set_Z(reg_B == 0); set_N(false); set_H(false); }
void GameBoyCPU::SRA_C() { set_C((reg_C & 0x01) == 0x01); reg_C = (reg_C >> 1) | (reg_C & 0x80); set_Z(reg_C == 0); set_N(false); set_H(false); }
void GameBoyCPU::SRA_D() { set_C((reg_D & 0x01) == 0x01); reg_D = (reg_D >> 1) | (reg_D & 0x80); set_Z(reg_D == 0); set_N(false); set_H(false); }
void GameBoyCPU::SRA_E() { set_C((reg_E & 0x01) == 0x01); reg_E = (reg_E >> 1) | (reg_E & 0x80); set_Z(reg_E == 0); set_N(false); set_H(false); }
void GameBoyCPU::SRA_H() { set_C((reg_H & 0x01) == 0x01); reg_H = (reg_H >> 1) | (reg_H & 0x80); set_Z(reg_H == 0); set_N(false); set_H(false); }
void GameBoyCPU::SRA_L() { set_C((reg_L & 0x01) == 0x01); reg_L = (reg_L >> 1) | (reg_L & 0x80); set_Z(reg_L == 0); set_N(false); set_H(false); }
void GameBoyCPU::SRA_aHL() { uint8_t v = _memory->Read(get_HL()); set_C((v & 0x01) == 0x01); v = (v >> 1) | (v & 0x80); _memory->Write(get_HL(), v); set_Z(v == 0); set_N(false); set_H(false); }

void GameBoyCPU::SWAP_A() { reg_A = (reg_A >> 4) | (reg_A << 4); set_Z(reg_A == 0); set_N(false); set_H(false); set_C(false); }
void GameBoyCPU::SWAP_B() { reg_B = (reg_B >> 4) | (reg_B << 4); set_Z(reg_B == 0); set_N(false); set_H(false); set_C(false); }
void GameBoyCPU::SWAP_C() { reg_C = (reg_C >> 4) | (reg_C << 4); set_Z(reg_C == 0); set_N(false); set_H(false); set_C(false); }
void GameBoyCPU::SWAP_D() { reg_D = (reg_D >> 4) | (reg_D << 4); set_Z(reg_D == 0); set_N(false); set_H(false); set_C(false); }
void GameBoyCPU::SWAP_E() { reg_E = (reg_E >> 4) | (reg_E << 4); set_Z(reg_E == 0); set_N(false); set_H(false); set_C(false); }
void GameBoyCPU::SWAP_H() { reg_H = (reg_H >> 4) | (reg_H << 4); set_Z(reg_H == 0); set_N(false); set_H(false); set_C(false); }
void GameBoyCPU::SWAP_L() { reg_L = (reg_L >> 4) | (reg_L << 4); set_Z(reg_L == 0); set_N(false); set_H(false); set_C(false); }
void GameBoyCPU::SWAP_aHL() { uint8_t v = _memory->Read(get_HL()); v = (v >> 4) | (v << 4); _memory->Write(get_HL(), v); set_Z(v == 0); set_N(false); set_H(false); set_C(false); }

void GameBoyCPU::SRL_A() { set_C((reg_A & 0x01) == 0x01); reg_A >>= 1; set_Z(reg_A == 0); set_N(false); set_H(false); }
void GameBoyCPU::SRL_B() { set_C((reg_B & 0x01) == 0x01); reg_B >>= 1; set_Z(reg_B == 0); set_N(false); set_H(false); }
void GameBoyCPU::SRL_C() { set_C((reg_C & 0x01) == 0x01); reg_C >>= 1; set_Z(reg_C == 0); set_N(false); set_H(false); }
void GameBoyCPU::SRL_D() { set_C((reg_D & 0x01) == 0x01); reg_D >>= 1; set_Z(reg_D == 0); set_N(false); set_H(false); }
void GameBoyCPU::SRL_E() { set_C((reg_E & 0x01) == 0x01); reg_E >>= 1; set_Z(reg_E == 0); set_N(false); set_H(false); }
void GameBoyCPU::SRL_H() { set_C((reg_H & 0x01) == 0x01); reg_H >>= 1; set_Z(reg_H == 0); set_N(false); set_H(false); }
void GameBoyCPU::SRL_L() { set_C((reg_L & 0x01) == 0x01); reg_L >>= 1; set_Z(reg_L == 0); set_N(false); set_H(false); }
void GameBoyCPU::SRL_aHL() { uint8_t v = _memory->Read(get_HL()); set_C((v & 0x01) == 0x01); v >>= 1; _memory->Write(get_HL(), v); set_Z(v == 0); set_N(false); set_H(false); }

// =============================================================
// BIT
// =============================================================

#define BIT_OP(bit, reg) set_Z((reg & (1 << bit)) == 0); set_N(false); set_H(true);
#define BIT_HL(bit) set_Z((_memory->Read(get_HL()) & (1 << bit)) == 0); set_N(false); set_H(true);

void GameBoyCPU::BIT_0_A() { BIT_OP(0, reg_A); } void GameBoyCPU::BIT_0_B() { BIT_OP(0, reg_B); } void GameBoyCPU::BIT_0_C() { BIT_OP(0, reg_C); } void GameBoyCPU::BIT_0_D() { BIT_OP(0, reg_D); } void GameBoyCPU::BIT_0_E() { BIT_OP(0, reg_E); } void GameBoyCPU::BIT_0_H() { BIT_OP(0, reg_H); } void GameBoyCPU::BIT_0_L() { BIT_OP(0, reg_L); } void GameBoyCPU::BIT_0_aHL() { BIT_HL(0); }
void GameBoyCPU::BIT_1_A() { BIT_OP(1, reg_A); } void GameBoyCPU::BIT_1_B() { BIT_OP(1, reg_B); } void GameBoyCPU::BIT_1_C() { BIT_OP(1, reg_C); } void GameBoyCPU::BIT_1_D() { BIT_OP(1, reg_D); } void GameBoyCPU::BIT_1_E() { BIT_OP(1, reg_E); } void GameBoyCPU::BIT_1_H() { BIT_OP(1, reg_H); } void GameBoyCPU::BIT_1_L() { BIT_OP(1, reg_L); } void GameBoyCPU::BIT_1_aHL() { BIT_HL(1); }
void GameBoyCPU::BIT_2_A() { BIT_OP(2, reg_A); } void GameBoyCPU::BIT_2_B() { BIT_OP(2, reg_B); } void GameBoyCPU::BIT_2_C() { BIT_OP(2, reg_C); } void GameBoyCPU::BIT_2_D() { BIT_OP(2, reg_D); } void GameBoyCPU::BIT_2_E() { BIT_OP(2, reg_E); } void GameBoyCPU::BIT_2_H() { BIT_OP(2, reg_H); } void GameBoyCPU::BIT_2_L() { BIT_OP(2, reg_L); } void GameBoyCPU::BIT_2_aHL() { BIT_HL(2); }
void GameBoyCPU::BIT_3_A() { BIT_OP(3, reg_A); } void GameBoyCPU::BIT_3_B() { BIT_OP(3, reg_B); } void GameBoyCPU::BIT_3_C() { BIT_OP(3, reg_C); } void GameBoyCPU::BIT_3_D() { BIT_OP(3, reg_D); } void GameBoyCPU::BIT_3_E() { BIT_OP(3, reg_E); } void GameBoyCPU::BIT_3_H() { BIT_OP(3, reg_H); } void GameBoyCPU::BIT_3_L() { BIT_OP(3, reg_L); } void GameBoyCPU::BIT_3_aHL() { BIT_HL(3); }
void GameBoyCPU::BIT_4_A() { BIT_OP(4, reg_A); } void GameBoyCPU::BIT_4_B() { BIT_OP(4, reg_B); } void GameBoyCPU::BIT_4_C() { BIT_OP(4, reg_C); } void GameBoyCPU::BIT_4_D() { BIT_OP(4, reg_D); } void GameBoyCPU::BIT_4_E() { BIT_OP(4, reg_E); } void GameBoyCPU::BIT_4_H() { BIT_OP(4, reg_H); } void GameBoyCPU::BIT_4_L() { BIT_OP(4, reg_L); } void GameBoyCPU::BIT_4_aHL() { BIT_HL(4); }
void GameBoyCPU::BIT_5_A() { BIT_OP(5, reg_A); } void GameBoyCPU::BIT_5_B() { BIT_OP(5, reg_B); } void GameBoyCPU::BIT_5_C() { BIT_OP(5, reg_C); } void GameBoyCPU::BIT_5_D() { BIT_OP(5, reg_D); } void GameBoyCPU::BIT_5_E() { BIT_OP(5, reg_E); } void GameBoyCPU::BIT_5_H() { BIT_OP(5, reg_H); } void GameBoyCPU::BIT_5_L() { BIT_OP(5, reg_L); } void GameBoyCPU::BIT_5_aHL() { BIT_HL(5); }
void GameBoyCPU::BIT_6_A() { BIT_OP(6, reg_A); } void GameBoyCPU::BIT_6_B() { BIT_OP(6, reg_B); } void GameBoyCPU::BIT_6_C() { BIT_OP(6, reg_C); } void GameBoyCPU::BIT_6_D() { BIT_OP(6, reg_D); } void GameBoyCPU::BIT_6_E() { BIT_OP(6, reg_E); } void GameBoyCPU::BIT_6_H() { BIT_OP(6, reg_H); } void GameBoyCPU::BIT_6_L() { BIT_OP(6, reg_L); } void GameBoyCPU::BIT_6_aHL() { BIT_HL(6); }
void GameBoyCPU::BIT_7_A() { BIT_OP(7, reg_A); } void GameBoyCPU::BIT_7_B() { BIT_OP(7, reg_B); } void GameBoyCPU::BIT_7_C() { BIT_OP(7, reg_C); } void GameBoyCPU::BIT_7_D() { BIT_OP(7, reg_D); } void GameBoyCPU::BIT_7_E() { BIT_OP(7, reg_E); } void GameBoyCPU::BIT_7_H() { BIT_OP(7, reg_H); } void GameBoyCPU::BIT_7_L() { BIT_OP(7, reg_L); } void GameBoyCPU::BIT_7_aHL() { BIT_HL(7); }

// =============================================================
// RES
// =============================================================

#define RES_OP(bit, reg) reg &= ~(1 << bit)
#define RES_HL(bit) _memory->Write(get_HL(), _memory->Read(get_HL()) & ~(1 << bit))

void GameBoyCPU::RES_0_A() { RES_OP(0, reg_A); } void GameBoyCPU::RES_0_B() { RES_OP(0, reg_B); } void GameBoyCPU::RES_0_C() { RES_OP(0, reg_C); } void GameBoyCPU::RES_0_D() { RES_OP(0, reg_D); } void GameBoyCPU::RES_0_E() { RES_OP(0, reg_E); } void GameBoyCPU::RES_0_H() { RES_OP(0, reg_H); } void GameBoyCPU::RES_0_L() { RES_OP(0, reg_L); } void GameBoyCPU::RES_0_aHL() { RES_HL(0); }
void GameBoyCPU::RES_1_A() { RES_OP(1, reg_A); } void GameBoyCPU::RES_1_B() { RES_OP(1, reg_B); } void GameBoyCPU::RES_1_C() { RES_OP(1, reg_C); } void GameBoyCPU::RES_1_D() { RES_OP(1, reg_D); } void GameBoyCPU::RES_1_E() { RES_OP(1, reg_E); } void GameBoyCPU::RES_1_H() { RES_OP(1, reg_H); } void GameBoyCPU::RES_1_L() { RES_OP(1, reg_L); } void GameBoyCPU::RES_1_aHL() { RES_HL(1); }
void GameBoyCPU::RES_2_A() { RES_OP(2, reg_A); } void GameBoyCPU::RES_2_B() { RES_OP(2, reg_B); } void GameBoyCPU::RES_2_C() { RES_OP(2, reg_C); } void GameBoyCPU::RES_2_D() { RES_OP(2, reg_D); } void GameBoyCPU::RES_2_E() { RES_OP(2, reg_E); } void GameBoyCPU::RES_2_H() { RES_OP(2, reg_H); } void GameBoyCPU::RES_2_L() { RES_OP(2, reg_L); } void GameBoyCPU::RES_2_aHL() { RES_HL(2); }
void GameBoyCPU::RES_3_A() { RES_OP(3, reg_A); } void GameBoyCPU::RES_3_B() { RES_OP(3, reg_B); } void GameBoyCPU::RES_3_C() { RES_OP(3, reg_C); } void GameBoyCPU::RES_3_D() { RES_OP(3, reg_D); } void GameBoyCPU::RES_3_E() { RES_OP(3, reg_E); } void GameBoyCPU::RES_3_H() { RES_OP(3, reg_H); } void GameBoyCPU::RES_3_L() { RES_OP(3, reg_L); } void GameBoyCPU::RES_3_aHL() { RES_HL(3); }
void GameBoyCPU::RES_4_A() { RES_OP(4, reg_A); } void GameBoyCPU::RES_4_B() { RES_OP(4, reg_B); } void GameBoyCPU::RES_4_C() { RES_OP(4, reg_C); } void GameBoyCPU::RES_4_D() { RES_OP(4, reg_D); } void GameBoyCPU::RES_4_E() { RES_OP(4, reg_E); } void GameBoyCPU::RES_4_H() { RES_OP(4, reg_H); } void GameBoyCPU::RES_4_L() { RES_OP(4, reg_L); } void GameBoyCPU::RES_4_aHL() { RES_HL(4); }
void GameBoyCPU::RES_5_A() { RES_OP(5, reg_A); } void GameBoyCPU::RES_5_B() { RES_OP(5, reg_B); } void GameBoyCPU::RES_5_C() { RES_OP(5, reg_C); } void GameBoyCPU::RES_5_D() { RES_OP(5, reg_D); } void GameBoyCPU::RES_5_E() { RES_OP(5, reg_E); } void GameBoyCPU::RES_5_H() { RES_OP(5, reg_H); } void GameBoyCPU::RES_5_L() { RES_OP(5, reg_L); } void GameBoyCPU::RES_5_aHL() { RES_HL(5); }
void GameBoyCPU::RES_6_A() { RES_OP(6, reg_A); } void GameBoyCPU::RES_6_B() { RES_OP(6, reg_B); } void GameBoyCPU::RES_6_C() { RES_OP(6, reg_C); } void GameBoyCPU::RES_6_D() { RES_OP(6, reg_D); } void GameBoyCPU::RES_6_E() { RES_OP(6, reg_E); } void GameBoyCPU::RES_6_H() { RES_OP(6, reg_H); } void GameBoyCPU::RES_6_L() { RES_OP(6, reg_L); } void GameBoyCPU::RES_6_aHL() { RES_HL(6); }
void GameBoyCPU::RES_7_A() { RES_OP(7, reg_A); } void GameBoyCPU::RES_7_B() { RES_OP(7, reg_B); } void GameBoyCPU::RES_7_C() { RES_OP(7, reg_C); } void GameBoyCPU::RES_7_D() { RES_OP(7, reg_D); } void GameBoyCPU::RES_7_E() { RES_OP(7, reg_E); } void GameBoyCPU::RES_7_H() { RES_OP(7, reg_H); } void GameBoyCPU::RES_7_L() { RES_OP(7, reg_L); } void GameBoyCPU::RES_7_aHL() { RES_HL(7); }

// =============================================================
// SET
// =============================================================

#define SET_OP(bit, reg) reg |= (1 << bit)
#define SET_HL(bit) _memory->Write(get_HL(), _memory->Read(get_HL()) | (1 << bit))

void GameBoyCPU::SET_0_A() { SET_OP(0, reg_A); } void GameBoyCPU::SET_0_B() { SET_OP(0, reg_B); } void GameBoyCPU::SET_0_C() { SET_OP(0, reg_C); } void GameBoyCPU::SET_0_D() { SET_OP(0, reg_D); } void GameBoyCPU::SET_0_E() { SET_OP(0, reg_E); } void GameBoyCPU::SET_0_H() { SET_OP(0, reg_H); } void GameBoyCPU::SET_0_L() { SET_OP(0, reg_L); } void GameBoyCPU::SET_0_aHL() { SET_HL(0); }
void GameBoyCPU::SET_1_A() { SET_OP(1, reg_A); } void GameBoyCPU::SET_1_B() { SET_OP(1, reg_B); } void GameBoyCPU::SET_1_C() { SET_OP(1, reg_C); } void GameBoyCPU::SET_1_D() { SET_OP(1, reg_D); } void GameBoyCPU::SET_1_E() { SET_OP(1, reg_E); } void GameBoyCPU::SET_1_H() { SET_OP(1, reg_H); } void GameBoyCPU::SET_1_L() { SET_OP(1, reg_L); } void GameBoyCPU::SET_1_aHL() { SET_HL(1); }
void GameBoyCPU::SET_2_A() { SET_OP(2, reg_A); } void GameBoyCPU::SET_2_B() { SET_OP(2, reg_B); } void GameBoyCPU::SET_2_C() { SET_OP(2, reg_C); } void GameBoyCPU::SET_2_D() { SET_OP(2, reg_D); } void GameBoyCPU::SET_2_E() { SET_OP(2, reg_E); } void GameBoyCPU::SET_2_H() { SET_OP(2, reg_H); } void GameBoyCPU::SET_2_L() { SET_OP(2, reg_L); } void GameBoyCPU::SET_2_aHL() { SET_HL(2); }
void GameBoyCPU::SET_3_A() { SET_OP(3, reg_A); } void GameBoyCPU::SET_3_B() { SET_OP(3, reg_B); } void GameBoyCPU::SET_3_C() { SET_OP(3, reg_C); } void GameBoyCPU::SET_3_D() { SET_OP(3, reg_D); } void GameBoyCPU::SET_3_E() { SET_OP(3, reg_E); } void GameBoyCPU::SET_3_H() { SET_OP(3, reg_H); } void GameBoyCPU::SET_3_L() { SET_OP(3, reg_L); } void GameBoyCPU::SET_3_aHL() { SET_HL(3); }
void GameBoyCPU::SET_4_A() { SET_OP(4, reg_A); } void GameBoyCPU::SET_4_B() { SET_OP(4, reg_B); } void GameBoyCPU::SET_4_C() { SET_OP(4, reg_C); } void GameBoyCPU::SET_4_D() { SET_OP(4, reg_D); } void GameBoyCPU::SET_4_E() { SET_OP(4, reg_E); } void GameBoyCPU::SET_4_H() { SET_OP(4, reg_H); } void GameBoyCPU::SET_4_L() { SET_OP(4, reg_L); } void GameBoyCPU::SET_4_aHL() { SET_HL(4); }
void GameBoyCPU::SET_5_A() { SET_OP(5, reg_A); } void GameBoyCPU::SET_5_B() { SET_OP(5, reg_B); } void GameBoyCPU::SET_5_C() { SET_OP(5, reg_C); } void GameBoyCPU::SET_5_D() { SET_OP(5, reg_D); } void GameBoyCPU::SET_5_E() { SET_OP(5, reg_E); } void GameBoyCPU::SET_5_H() { SET_OP(5, reg_H); } void GameBoyCPU::SET_5_L() { SET_OP(5, reg_L); } void GameBoyCPU::SET_5_aHL() { SET_HL(5); }
void GameBoyCPU::SET_6_A() { SET_OP(6, reg_A); } void GameBoyCPU::SET_6_B() { SET_OP(6, reg_B); } void GameBoyCPU::SET_6_C() { SET_OP(6, reg_C); } void GameBoyCPU::SET_6_D() { SET_OP(6, reg_D); } void GameBoyCPU::SET_6_E() { SET_OP(6, reg_E); } void GameBoyCPU::SET_6_H() { SET_OP(6, reg_H); } void GameBoyCPU::SET_6_L() { SET_OP(6, reg_L); } void GameBoyCPU::SET_6_aHL() { SET_HL(6); }
void GameBoyCPU::SET_7_A() { SET_OP(7, reg_A); } void GameBoyCPU::SET_7_B() { SET_OP(7, reg_B); } void GameBoyCPU::SET_7_C() { SET_OP(7, reg_C); } void GameBoyCPU::SET_7_D() { SET_OP(7, reg_D); } void GameBoyCPU::SET_7_E() { SET_OP(7, reg_E); } void GameBoyCPU::SET_7_H() { SET_OP(7, reg_H); } void GameBoyCPU::SET_7_L() { SET_OP(7, reg_L); } void GameBoyCPU::SET_7_aHL() { SET_HL(7); }