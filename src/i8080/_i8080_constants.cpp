#include "_i8080_constants.hpp"
#include <string>

// Opcodes from here, http://www.emulator101.com/reference/8080-by-opcode.html
const uint8_t opcodes_lengths[256] = {
/*      0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F  */
/* 0 */ 1, 3, 1, 1, 1, 1, 2, 1, 0, 1, 1, 1, 1, 1, 2, 1,
/* 1 */ 0, 3, 1, 1, 1, 1, 2, 1, 0, 1, 1, 1, 1, 1, 2, 1,
/* 2 */ 0, 3, 3, 1, 1, 1, 2, 1, 0, 1, 3, 1, 1, 1, 2, 1,
/* 3 */ 0, 3, 3, 1, 1, 1, 2, 1, 0, 1, 3, 1, 1, 1, 2, 1,
/* 4 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/* 5 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/* 6 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/* 7 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/* 8 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/* 9 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/* A */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/* B */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/* C */ 1, 1, 3, 3, 3, 1, 2, 1, 1, 1, 3, 0, 3, 3, 2, 1,
/* D */ 1, 1, 3, 2, 3, 1, 2, 1, 1, 0, 3, 2, 3, 0, 2, 1,
/* E */ 1, 1, 3, 1, 3, 1, 2, 1, 1, 1, 3, 1, 3, 0, 2, 1,
/* F */ 1, 1, 3, 1, 3, 1, 2, 1, 1, 1, 3, 1, 3, 0, 2, 1 };


// opcode cylces from https://github.com/mamedev/mame/blob/6c0fdfc5257ca20555fbc527203710d5af5401d1/src/devices/cpu/i8085/i8085.cpp
const uint8_t opcodes_cycles[256]={
/*      0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F  */
/* 0 */ 4, 10,7, 5, 5, 5, 7, 4, 4, 10,7, 5, 5, 5, 7, 4,
/* 1 */ 4, 10,7, 5, 5, 5, 7, 4, 4, 10,7, 5, 5, 5, 7, 4,
/* 2 */ 4, 10,16,5, 5, 5, 7, 4, 4, 10,16,5, 5, 5, 7, 4,
/* 3 */ 4, 10,13,5, 10,10,10,4, 4, 10,13,5, 5, 5, 7, 4,
/* 4 */ 5, 5, 5, 5, 5, 5, 7, 5, 5, 5, 5, 5, 5, 5, 7, 5,
/* 5 */ 5, 5, 5, 5, 5, 5, 7, 5, 5, 5, 5, 5, 5, 5, 7, 5,
/* 6 */ 5, 5, 5, 5, 5, 5, 7, 5, 5, 5, 5, 5, 5, 5, 7, 5,
/* 7 */ 7, 7, 7, 7, 7, 7, 7, 7, 5, 5, 5, 5, 5, 5, 7, 5,
/* 8 */ 4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
/* 9 */ 4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
/* A */ 4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
/* B */ 4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
/* C */ 5, 10,10,10,11,11,7, 11,5, 10,10,10,11,11,7, 11,
/* D */ 5, 10,10,10,11,11,7, 11,5, 10,10,10,11,11,7, 11,
/* E */ 5, 10,10,18,11,11,7, 11,5, 5, 10,5, 11,11,7, 11,
/* F */ 5, 10,10,4, 11,11,7, 11,5, 5, 10,4, 11,11,7, 11 };

const std::string opcodes_names[256] = {
    "NOP",          // 0x00
    "LXI B,D16",    // 0x01
    "STAX B",       // 0x02
    "INX B",        // 0x03
    "INR B",        // 0x04
    "DCR B",        // 0x05
    "MVI B, D8",    // 0x06
    "RLC",          // 0x07
    "-",            // 0x08
    "DAD B",        // 0x09
    "LDAX B",       // 0x0a
    "DCX B",        // 0x0b
    "INR C",        // 0x0c
    "DCR C",        // 0x0d
    "MVI C,D8",     // 0x0e
    "RRC",          // 0x0f
    "-",            // 0x10
    "LXI D,D16",    // 0x11
    "STAX D",       // 0x12
    "INX D",        // 0x13
    "INR D",        // 0x14
    "DCR D",        // 0x15
    "MVI D, D8",    // 0x16
    "RAL",          // 0x17
    "-",            // 0x18
    "DAD D",        // 0x19
    "LDAX D",       // 0x1a
    "DCX D",        // 0x1b
    "INR E",        // 0x1c
    "DCR E",        // 0x1d
    "MVI E,D8",     // 0x1e
    "RAR",          // 0x1f
    "-",            // 0x20
    "LXI H,D16",    // 0x21
    "SHLD adr",     // 0x22
    "INX H",        // 0x23
    "INR H",        // 0x24
    "DCR H",        // 0x25
    "MVI H,D8",     // 0x26
    "DAA",          // 0x27
    "-",            // 0x28
    "DAD H",        // 0x29
    "LHLD adr",     // 0x2a
    "DCX H",        // 0x2b
    "INR L",        // 0x2c
    "DCR L",        // 0x2d
    "MVI L, D8",    // 0x2e
    "CMA",          // 0x2f
    "-",            // 0x30
    "LXI SP, D16",  // 0x31
    "STA adr",      // 0x32
    "INX SP",       // 0x33
    "INR M",        // 0x34
    "DCR M",        // 0x35
    "MVI M,D8",     // 0x36
    "STC",          // 0x37
    "-",            // 0x38
    "DAD SP",       // 0x39
    "LDA adr",      // 0x3a
    "DCX SP",       // 0x3b
    "INR A",        // 0x3c
    "DCR A",        // 0x3d
    "MVI A,D8",     // 0x3e
    "CMC",          // 0x3f
    "MOV B,B",      // 0x40
    "MOV B,C",      // 0x41
    "MOV B,D",      // 0x42
    "MOV B,E",      // 0x43
    "MOV B,H",      // 0x44
    "MOV B,L",      // 0x45
    "MOV B,M",      // 0x46
    "MOV B,A",      // 0x47
    "MOV C,B",      // 0x48
    "MOV C,C",      // 0x49
    "MOV C,D",      // 0x4a
    "MOV C,E",      // 0x4b
    "MOV C,H",      // 0x4c
    "MOV C,L",      // 0x4d
    "MOV C,M",      // 0x4e
    "MOV C,A",      // 0x4f
    "MOV D,B",      // 0x50
    "MOV D,C",      // 0x51
    "MOV D,D",      // 0x52
    "MOV D,E",      // 0x53
    "MOV D,H",      // 0x54
    "MOV D,L",      // 0x55
    "MOV D,M",      // 0x56
    "MOV D,A",      // 0x57
    "MOV E,B",      // 0x58
    "MOV E,C",      // 0x59
    "MOV E,D",      // 0x5a
    "MOV E,E",      // 0x5b
    "MOV E,H",      // 0x5c
    "MOV E,L",      // 0x5d
    "MOV E,M",      // 0x5e
    "MOV E,A",      // 0x5f
    "MOV H,B",      // 0x60
    "MOV H,C",      // 0x61
    "MOV H,D",      // 0x62
    "MOV H,E",      // 0x63
    "MOV H,H",      // 0x64
    "MOV H,L",      // 0x65
    "MOV H,M",      // 0x66
    "MOV H,A",      // 0x67
    "MOV L,B",      // 0x68
    "MOV L,C",      // 0x69
    "MOV L,D",      // 0x6a
    "MOV L,E",      // 0x6b
    "MOV L,H",      // 0x6c
    "MOV L,L",      // 0x6d
    "MOV L,M",      // 0x6e
    "MOV L,A",      // 0x6f
    "MOV M,B",      // 0x70
    "MOV M,C",      // 0x71
    "MOV M,D",      // 0x72
    "MOV M,E",      // 0x73
    "MOV M,H",      // 0x74
    "MOV M,L",      // 0x75
    "HLT",          // 0x76
    "MOV M,A",      // 0x77
    "MOV A,B",      // 0x78
    "MOV A,C",      // 0x79
    "MOV A,D",      // 0x7a
    "MOV A,E",      // 0x7b
    "MOV A,H",      // 0x7c
    "MOV A,L",      // 0x7d
    "MOV A,M",      // 0x7e
    "MOV A,A",      // 0x7f
    "ADD B",        // 0x80
    "ADD C",        // 0x81
    "ADD D",        // 0x82
    "ADD E",        // 0x83
    "ADD H",        // 0x84
    "ADD L",        // 0x85
    "ADD M",        // 0x86
    "ADD A",        // 0x87
    "ADC B",        // 0x88
    "ADC C",        // 0x89
    "ADC D",        // 0x8a
    "ADC E",        // 0x8b
    "ADC H",        // 0x8c
    "ADC L",        // 0x8d
    "ADC M",        // 0x8e
    "ADC A",        // 0x8f
    "SUB B",        // 0x90
    "SUB C",        // 0x91
    "SUB D",        // 0x92
    "SUB E",        // 0x93
    "SUB H",        // 0x94
    "SUB L",        // 0x95
    "SUB M",        // 0x96
    "SUB A",        // 0x97
    "SBB B",        // 0x98
    "SBB C",        // 0x99
    "SBB D",        // 0x9a
    "SBB E",        // 0x9b
    "SBB H",        // 0x9c
    "SBB L",        // 0x9d
    "SBB M",        // 0x9e
    "SBB A",        // 0x9f
    "ANA B",        // 0xa0
    "ANA C",        // 0xa1
    "ANA D",        // 0xa2
    "ANA E",        // 0xa3
    "ANA H",        // 0xa4
    "ANA L",        // 0xa5
    "ANA M",        // 0xa6
    "ANA A",        // 0xa7
    "XRA B",        // 0xa8
    "XRA C",        // 0xa9
    "XRA D",        // 0xaa
    "XRA E",        // 0xab
    "XRA H",        // 0xac
    "XRA L",        // 0xad
    "XRA M",        // 0xae
    "XRA A",        // 0xaf
    "ORA B",        // 0xb0
    "ORA C",        // 0xb1
    "ORA D",        // 0xb2
    "ORA E",        // 0xb3
    "ORA H",        // 0xb4
    "ORA L",        // 0xb5
    "ORA M",        // 0xb6
    "ORA A",        // 0xb7
    "CMP B",        // 0xb8
    "CMP C",        // 0xb9
    "CMP D",        // 0xba
    "CMP E",        // 0xbb
    "CMP H",        // 0xbc
    "CMP L",        // 0xbd
    "CMP M",        // 0xbe
    "CMP A",        // 0xbf
    "RNZ",          // 0xc0
    "POP B",        // 0xc1
    "JNZ adr",      // 0xc2
    "JMP adr",      // 0xc3
    "CNZ adr",      // 0xc4
    "PUSH B",       // 0xc5
    "ADI D8",       // 0xc6
    "RST 0",        // 0xc7
    "RZ",           // 0xc8
    "RET",          // 0xc9
    "JZ adr",       // 0xca
    "-",            // 0xcb
    "CZ adr",       // 0xcc
    "CALL adr",     // 0xcd
    "ACI D8",       // 0xce
    "RST 1",        // 0xcf
    "RNC",          // 0xd0
    "POP D",        // 0xd1
    "JNC adr",      // 0xd2
    "OUT D8",       // 0xd3
    "CNC adr",      // 0xd4
    "PUSH D",       // 0xd5
    "SUI D8",       // 0xd6
    "RST 2",        // 0xd7
    "RC",           // 0xd8
    "-",            // 0xd9
    "JC adr",       // 0xda
    "IN D8",        // 0xdb
    "CC adr",       // 0xdc
    "-",            // 0xdd
    "SBI D8",       // 0xde
    "RST 3",        // 0xdf
    "RPO",          // 0xe0
    "POP H",        // 0xe1
    "JPO adr",      // 0xe2
    "XTHL",         // 0xe3
    "CPO adr",      // 0xe4
    "PUSH H",       // 0xe5
    "ANI D8",       // 0xe6
    "RST 4",        // 0xe7
    "RPE",          // 0xe8
    "PCHL",         // 0xe9
    "JPE adr",      // 0xea
    "XCHG",         // 0xeb
    "CPE adr",      // 0xec
    "-",            // 0xed
    "XRI D8",       // 0xee
    "RST 5",        // 0xef
    "RP",           // 0xf0
    "POP PSW",      // 0xf1
    "JP adr",       // 0xf2
    "DI",           // 0xf3
    "CP adr",       // 0xf4
    "PUSH PSW",     // 0xf5
    "ORI D8",       // 0xf6
    "RST 6",        // 0xf7
    "RM",           // 0xf8
    "SPHL",         // 0xf9
    "JM adr",       // 0xfa
    "EI",           // 0xfb
    "CM adr",       // 0xfc
    "-",            // 0xfd
    "CPI D8",       // 0xfe
    "RST 7"         // 0xff
};
