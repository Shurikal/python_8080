#include "Python.h"
#include "_i8080_cpu.h"
#include "_i8080_object.h"
#include "stdio.h"

#define DEBUG


// checks the parity of a byte
static inline uint8_t parity_check(uint8_t byte)
{
    byte ^= byte >> 4;
    byte ^= byte >> 2;
    byte ^= byte >> 1;
    return (~byte) & 1;
}

/*
Carry Bit Instructions
*/

/*
CMC Complement Carry
Flags affected: CY
*/

void instr_0x3f(i8080oObject *self) {
    #ifdef DEBUG
    printf("CMC\n");
    #endif
    self->CC.cy = !self->CC.cy;
    self->PC++;
}

/*
STC Set Carry
*/

void instr_0x37(i8080oObject *self) {
    #ifdef DEBUG
    printf("STC\n");
    #endif
    self->CC.cy = 1;
    self->PC++;
}

/*
Single Register Instructions
*/



/*
INR Increment Register or Memory

Condition Bits Affected:
Zero Flag: Set if result is zero; reset otherwise.
Sign Flag: Set if bit 7 of result is set; reset otherwise.
Parity Flag: Set if result has even parity; reset otherwise.
Auxiliary Carry Flag: Set if carry from bit 3; reset otherwise.
*/

static inline void update_flags_inr(i8080oObject *self, uint8_t res) {
    self->CC.z = (res == 0); // set zero flag
    self->CC.s = res >> 7; // set sign flag
    self->CC.p = parity_check(res); // set parity flag
    self->CC.ac = (res & 0x0F) == 0; // set auxiliary carry flag
}


void instr_0x04(i8080oObject *self) {
    #ifdef DEBUG
    printf("INR B\n");
    #endif
    self->B++;
    update_flags_inr(self, self->B);
    self->PC++;
}

void instr_0x0c(i8080oObject *self) {
    #ifdef DEBUG
    printf("INR C\n");
    #endif
    self->C++;
    update_flags_inr(self, self->C);
    self->PC++;
}

void instr_0x14(i8080oObject *self) {
    #ifdef DEBUG
    printf("INR D\n");
    #endif
    self->D++;
    update_flags_inr(self, self->D);
    self->PC++;
}

void instr_0x1c(i8080oObject *self) {
    #ifdef DEBUG
    printf("INR E\n");
    #endif
    self->E++;
    update_flags_inr(self, self->E);
    self->PC++;
}

void instr_0x24(i8080oObject *self) {
    #ifdef DEBUG
    printf("INR H\n");
    #endif
    self->H++;
    update_flags_inr(self, self->H);
    self->PC++;
}

void instr_0x2c(i8080oObject *self) {
    #ifdef DEBUG
    printf("INR L\n");
    #endif
    self->L++;
    update_flags_inr(self, self->L);
    self->PC++;
}

void instr_0x34(i8080oObject *self) {
    #ifdef DEBUG
    printf("INR M\n");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    uint8_t res = self->memory[addr] + 1;
    self->memory[addr] = res;
    update_flags_inr(self, res);
    self->PC++;
}

void instr_0x3c(i8080oObject *self) {
    #ifdef DEBUG
    printf("INR A\n");
    #endif
    self->A++;
    update_flags_inr(self, self->A);
    self->PC++;
}

// END INR

/*
DCR Decrement Register or Memory

Condition Bits Affected:
Zero Flag: Set if result is zero; reset otherwise.
Sign Flag: Set if bit 7 of result is set; reset otherwise.
Parity Flag: Set if result has even parity; reset otherwise.
Auxiliary Carry Flag: Set if borrow from bit 4; reset otherwise.
*/

static inline void update_flags_dcr(i8080oObject *self, uint8_t res) {
    self->CC.z = (res == 0); // set zero flag
    self->CC.s = res >> 7; // set sign flag
    self->CC.p = parity_check(res); // set parity flag
    self->CC.ac = !((res & 0x0F) == 0x0F); // set auxiliary carry flag
}

void instr_0x05(i8080oObject *self) {
    #ifdef DEBUG
    printf("DCR B\n");
    #endif
    self->B--;
    update_flags_dcr(self, self->B);
    self->PC++;
}

void instr_0x0d(i8080oObject *self) {
    #ifdef DEBUG
    printf("DCR C\n");
    #endif
    self->C--;
    update_flags_dcr(self, self->C);
    self->PC++;
}

void instr_0x15(i8080oObject *self) {
    #ifdef DEBUG
    printf("DCR D\n");
    #endif
    self->D--;
    update_flags_dcr(self, self->D);
    self->PC++;
}

void instr_0x1d(i8080oObject *self) {
    #ifdef DEBUG
    printf("DCR E\n");
    #endif
    self->E--;
    update_flags_dcr(self, self->E);
    self->PC++;
}

void instr_0x25(i8080oObject *self) {
    #ifdef DEBUG
    printf("DCR H\n");
    #endif
    self->H--;
    update_flags_dcr(self, self->H);
    self->PC++;
}

void instr_0x2d(i8080oObject *self) {
    #ifdef DEBUG
    printf("DCR L\n");
    #endif
    self->L--;
    update_flags_dcr(self, self->H);
    self->PC++;
}

void instr_0x35(i8080oObject *self) {
    #ifdef DEBUG
    printf("DCR M\n");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    uint8_t res = self->memory[addr] - 1;
    self->memory[addr] = res;
    update_flags_dcr(self, res);
    self->PC++;
}

void instr_0x3d(i8080oObject *self) {
    #ifdef DEBUG
    printf("DCR A\n");
    #endif
    self->A--;
    update_flags_dcr(self, self->A);
    self->PC++;
}

// END DCR

/*
CMA Complement Accumulator
*/

void instr_0x2f(i8080oObject *self) {
    #ifdef DEBUG
    printf("CMA\n");
    #endif
    self->A = ~self->A;
    self->PC++;
}

/*
DAA Decimal Adjust Accumulator
*/

void instr_0x27(i8080oObject *self) {
    #ifdef DEBUG
    printf("DAA\n");
    #endif
    // check if lower nibble is greater than 9 or if AC is set
    if ((self->A & 0x0f) > 9 || self->CC.ac) {
        self->A += 6;
        self->CC.ac = 1;
    } else {
        self->CC.ac = 0;
    }

    // check if upper nibble is greater than 9 or if CY is set
    if ((self->A & 0xf0) > 0x90 || self->CC.cy) {
        self->A += 0x60;
        self->CC.cy = 1;
    } else {
        self->CC.cy = 0;
    }

    self->CC.z = (self->A == 0);
    self->CC.s = (0x80 == (self->A & 0x80));
    self->CC.p = parity_check(self->A);
    self->PC++;
}

/*
NOP Instruction
*/
void instr_0x00(i8080oObject *self) {
    #ifdef DEBUG
    printf("NOP\n");
    #endif
    self->PC++;
}

/*
*** Data Transfer Instructions ***
*/

/*
MOV Move Register or Memory to Register
*/

void instr_0x40(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV B,B\n");
    #endif
    self->PC++;
}

void instr_0x41(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV B,C\n");
    #endif
    self->B = self->C;
    self->PC++;
}

void instr_0x42(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV B,D\n");
    #endif
    self->B = self->D;
    self->PC++;
}

void instr_0x43(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV B,E\n");
    #endif
    self->B = self->E;
    self->PC++;
}

void instr_0x44(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV B,H\n");
    #endif
    self->B = self->H;
    self->PC++;
}

void instr_0x45(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV B,L\n");
    #endif
    self->B = self->L;
    self->PC++;
}

void instr_0x46(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV B,M\n");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    self->B = self->memory[addr];
    self->PC++;
}

void instr_0x47(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV B,A\n");
    #endif
    self->B = self->A;
    self->PC++;
}

void instr_0x48(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV C,B\n");
    #endif
    self->C = self->B;
    self->PC++;
}

void instr_0x49(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV C,C\n");
    #endif
    self->PC++;
}

void instr_0x4a(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV C,D\n");
    #endif
    self->C = self->D;
    self->PC++;
}

void instr_0x4b(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV C,E\n");
    #endif
    self->C = self->E;
    self->PC++;
}

void instr_0x4c(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV C,H\n");
    #endif
    self->C = self->H;
    self->PC++;
}

void instr_0x4d(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV C,L\n");
    #endif
    self->C = self->L;
    self->PC++;
}

void instr_0x4e(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV C,M\n");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    self->C = self->memory[addr];
    self->PC++;
}

void instr_0x4f(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV C,A\n");
    #endif
    self->C = self->A;
    self->PC++;
}

void instr_0x50(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV D,B\n");
    #endif
    self->D = self->B;
    self->PC++;
}

void instr_0x51(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV D,C\n");
    #endif
    self->D = self->C;
    self->PC++;
}

void instr_0x52(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV D,D\n");
    #endif
    self->PC++;
}

void instr_0x53(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV D,E\n");
    #endif
    self->D = self->E;
    self->PC++;
}

void instr_0x54(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV D,H\n");
    #endif
    self->D = self->H;
    self->PC++;
}

void instr_0x55(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV D,L\n");
    #endif
    self->D = self->L;
    self->PC++;
}

void instr_0x56(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV D,M\n");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    self->D = self->memory[addr];
    self->PC++;
}

void instr_0x57(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV D,A\n");
    #endif
    self->D = self->A;
    self->PC++;
}

void instr_0x58(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV E,B\n");
    #endif
    self->E = self->B;
    self->PC++;
}

void instr_0x59(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV E,C\n");
    #endif
    self->E = self->C;
    self->PC++;
}

void instr_0x5a(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV E,D\n");
    #endif
    self->E = self->D;
    self->PC++;
}

void instr_0x5b(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV E,E\n");
    #endif
    self->PC++;
}

void instr_0x5c(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV E,H\n");
    #endif
    self->E = self->H;
    self->PC++;
}

void instr_0x5d(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV E,L\n");
    #endif
    self->E = self->L;
    self->PC++;
}

void instr_0x5e(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV E,M\n");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    self->E = self->memory[addr];
    self->PC++;
}

void instr_0x5f(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV E,A\n");
    #endif
    self->E = self->A;
    self->PC++;
}

void instr_0x60(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV H,B\n");
    #endif
    self->H = self->B;
    self->PC++;
}

void instr_0x61(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV H,C\n");
    #endif
    self->H = self->C;
    self->PC++;
}

void instr_0x62(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV H,D\n");
    #endif
    self->H = self->D;
    self->PC++;
}

void instr_0x63(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV H,E\n");
    #endif
    self->H = self->E;
    self->PC++;
}

void instr_0x64(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV H,H\n");
    #endif
    self->PC++;
}

void instr_0x65(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV H,L\n");
    #endif
    self->H = self->L;
    self->PC++;
}

void instr_0x66(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV H,M\n");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    self->H = self->memory[addr];
    self->PC++;
}

void instr_0x67(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV H,A\n");
    #endif
    self->H = self->A;
    self->PC++;
}

void instr_0x68(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV L,B\n");
    #endif
    self->L = self->B;
    self->PC++;
}

void instr_0x69(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV L,C\n");
    #endif
    self->L = self->C;
    self->PC++;
}

void instr_0x6a(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV L,D\n");
    #endif
    self->L = self->D;
    self->PC++;
}

void instr_0x6b(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV L,E\n");
    #endif
    self->L = self->E;
    self->PC++;
}

void instr_0x6c(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV L,H\n");
    #endif
    self->L = self->H;
    self->PC++;
}

void instr_0x6d(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV L,L\n");
    #endif
    self->PC++;
}

void instr_0x6e(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV L,M\n");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    self->L = self->memory[addr];
    self->PC++;
}

void instr_0x6f(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV L,A\n");
    #endif
    self->L = self->A;
    self->PC++;
}

void instr_0x70(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV M,B\n");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    self->memory[addr] = self->B;
    self->PC++;
}

void instr_0x71(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV M,C\n");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    self->memory[addr] = self->C;
    self->PC++;
}

void instr_0x72(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV M,D\n");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    self->memory[addr] = self->D;
    self->PC++;
}

void instr_0x73(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV M,E\n");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    self->memory[addr] = self->E;
    self->PC++;
}

void instr_0x74(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV M,H\n");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    self->memory[addr] = self->H;
    self->PC++;
}

void instr_0x75(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV M,L\n");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    self->memory[addr] = self->L;
    self->PC++;
}

void instr_0x77(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV M,A\n");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    self->memory[addr] = self->A;
    self->PC++;
}

void instr_0x78(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV A,B\n");
    #endif
    self->A = self->B;
    self->PC++;
}

void instr_0x79(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV A,C\n");
    #endif
    self->A = self->C;
    self->PC++;
}

void instr_0x7a(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV A,D\n");
    #endif
    self->A = self->D;
    self->PC++;
}

void instr_0x7b(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV A,E\n");
    #endif
    self->A = self->E;
    self->PC++;
}

void instr_0x7c(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV A,H\n");
    #endif
    self->A = self->H;
    self->PC++;
}

void instr_0x7d(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV A,L\n");
    #endif
    self->A = self->L;
    self->PC++;
}

void instr_0x7e(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV A,M\n");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    self->A = self->memory[addr];
    self->PC++;
}

void instr_0x7f(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV A,A\n");
    #endif
    self->PC++;
}

void instr_0x02(i8080oObject *self) {
    #ifdef DEBUG
    printf("STAX B\n");
    #endif
    uint16_t addr = (self->B << 8) | self->C;
    self->memory[addr] = self->A;
    self->PC++;
}

void instr_0x12(i8080oObject *self) {
    #ifdef DEBUG
    printf("STAX D\n");
    #endif
    uint16_t addr = (self->D << 8) | self->E;
    self->memory[addr] = self->A;
    self->PC++;
}

void instr_0x0a(i8080oObject *self) {
    #ifdef DEBUG
    printf("LDAX B\n");
    #endif
    uint16_t addr = (self->B << 8) | self->C;
    self->A = self->memory[addr];
    self->PC++;
}

void instr_0x1a(i8080oObject *self) {
    #ifdef DEBUG
    printf("LDAX D\n");
    #endif
    uint16_t addr = (self->D << 8) | self->E;
    self->A = self->memory[addr];
    self->PC++;
}

// END DATA TRANSFER INSTRUCTIONS

/*
*** Register or Memory to Accumulator Instructions ***
*/

/*
ADD ADD Register or Memory to Accumulator
Condition Bits Affected:
Carry Flag: Set if carry from bit 7; reset otherwise.
Zero Flag: Set if result is zero; reset otherwise.
Sign Flag: Set if bit 7 of result is set; reset otherwise.
Parity Flag: Set if result has even parity; reset otherwise.
Auxiliary Carry Flag: Set if borrow from bit 4; reset otherwise.
*/

static inline void update_flags_add(i8080oObject *self, uint16_t result){
    self->CC.cy = (result > 0xff);
    self->CC.z = (result == 0);
    self->CC.s = (result & 0x80) >> 7;
    self->CC.p = parity_check((uint8_t)(result & 0xff));
    self->CC.ac = (result & 0x10) >> 4;
}

void instr_0x80(i8080oObject *self) {
    #ifdef DEBUG
    printf("ADD B\n");
    #endif
    uint16_t result = self->A + self->B;
    self->A = result & 0xff;
    update_flags_add(self, result);
    self->PC++;
}

void instr_0x81(i8080oObject *self) {
    #ifdef DEBUG
    printf("ADD C\n");
    #endif
    uint16_t result = self->A + self->C;
    self->A = result & 0xff;
    update_flags_add(self, result);
    self->PC++;
}

void instr_0x82(i8080oObject *self) {
    #ifdef DEBUG
    printf("ADD D\n");
    #endif
    uint16_t result = self->A + self->D;
    self->A = result & 0xff;
    update_flags_add(self, result);
    self->PC++;
}

void instr_0x83(i8080oObject *self) {
    #ifdef DEBUG
    printf("ADD E\n");
    #endif
    uint16_t result = self->A + self->E;
    self->A = result & 0xff;
    update_flags_add(self, result);
    self->PC++;
}

void instr_0x84(i8080oObject *self) {
    #ifdef DEBUG
    printf("ADD H\n");
    #endif
    uint16_t result = self->A + self->H;
    self->A = result & 0xff;
    update_flags_add(self, result);
    self->PC++;
}

void instr_0x85(i8080oObject *self) {
    #ifdef DEBUG
    printf("ADD L\n");
    #endif
    uint16_t result = self->A + self->L;
    self->A = result & 0xff;
    update_flags_add(self, result);
    self->PC++;
}

void instr_0x86(i8080oObject *self) {
    #ifdef DEBUG
    printf("ADD M\n");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    uint16_t result = self->A + self->memory[addr];
    self->A = result & 0xff;
    update_flags_add(self, result);
    self->PC++;
}

void instr_0x87(i8080oObject *self) {
    #ifdef DEBUG
    printf("ADD A\n");
    #endif
    uint16_t result = self->A + self->A;
    self->A = result & 0xff;
    update_flags_add(self, result);
    self->PC++;
}

// END ADD

/*
ADC Add Register or Memory to Accumulator with Carry

Condition Bits Affected:
Carry Flag: Set if carry from bit 7; reset otherwise.
Zero Flag: Set if result is zero; reset otherwise.
Sign Flag: Set if bit 7 of result is set; reset otherwise.
Parity Flag: Set if result has even parity; reset otherwise.
Auxiliary Carry Flag: Set if borrow from bit 4; reset otherwise.
*/

void instr_0x88(i8080oObject *self) {
    #ifdef DEBUG
    printf("ADC B\n");
    #endif
    uint16_t result = self->A + self->B + self->CC.cy;
    self->A = result & 0xff;
    update_flags_add(self, result);
    self->PC++;
}

void instr_0x89(i8080oObject *self) {
    #ifdef DEBUG
    printf("ADC C\n");
    #endif
    uint16_t result = self->A + self->C + self->CC.cy;
    self->A = result & 0xff;
    update_flags_add(self, result);
    self->PC++;
}

void instr_0x8a(i8080oObject *self) {
    #ifdef DEBUG
    printf("ADC D\n");
    #endif
    uint16_t result = self->A + self->D + self->CC.cy;
    self->A = result & 0xff;
    update_flags_add(self, result);
    self->PC++;
}

void instr_0x8b(i8080oObject *self) {
    #ifdef DEBUG
    printf("ADC E\n");
    #endif
    uint16_t result = self->A + self->E + self->CC.cy;
    self->A = result & 0xff;
    update_flags_add(self, result);
    self->PC++;
}

void instr_0x8c(i8080oObject *self) {
    #ifdef DEBUG
    printf("ADC H\n");
    #endif
    uint16_t result = self->A + self->H + self->CC.cy;
    self->A = result & 0xff;
    update_flags_add(self, result);
    self->PC++;
}

void instr_0x8d(i8080oObject *self) {
    #ifdef DEBUG
    printf("ADC L\n");
    #endif
    uint16_t result = self->A + self->L + self->CC.cy;
    self->A = result & 0xff;
    update_flags_add(self, result);
    self->PC++;
}

void instr_0x8e(i8080oObject *self) {
    #ifdef DEBUG
    printf("ADC M\n");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    uint16_t result = self->A + self->memory[addr] + self->CC.cy;
    self->A = result & 0xff;
    update_flags_add(self, result);
    self->PC++;
}

void instr_0x8f(i8080oObject *self) {
    #ifdef DEBUG
    printf("ADC A\n");
    #endif
    uint16_t result = self->A + self->A + self->CC.cy;
    self->A = result & 0xff;
    update_flags_add(self, result);
    self->PC++;
}

// END ADC

/*
SUB Subtract Register or Memory from Accumulator

Condition Bits Affected:
Carry Flag: Reset if borrow from bit 7; set otherwise.
Zero Flag: Set if result is zero; reset otherwise.
Sign Flag: Set if bit 7 of result is set; reset otherwise.
Parity Flag: Set if result has even parity; reset otherwise.
Auxiliary Carry Flag: Set if borrow from bit 4; reset otherwise.
*/

static inline void update_flags_sub(i8080oObject *self, uint16_t result){
    self->CC.cy = !(result > 0xff); // reset carry if result is greater than 0xff
    self->CC.z = ((result & 0xff) == 0);
    self->CC.s = ((result & 0x80) != 0);
    self->CC.p = parity_check(result & 0xff);
    self->CC.ac = (result & 0x10) >> 4;
    self->PC++;
}

void instr_0x90(i8080oObject *self) {
    #ifdef DEBUG
    printf("SUB B\n");
    #endif
    uint16_t result = self->A + (~self->B) + 1; // add the 2's complement of B and 1
    self->A = result & 0xff;
    update_flags_sub(self, result);
    self->PC++;
}

void instr_0x91(i8080oObject *self) {
    #ifdef DEBUG
    printf("SUB C\n");
    #endif
    uint16_t result = self->A + (~self->C) + 1; // add the 2's complement of C and 1
    self->A = result & 0xff;
    update_flags_sub(self, result);
    self->PC++;
}

void instr_0x92(i8080oObject *self) {
    #ifdef DEBUG
    printf("SUB D\n");
    #endif
    uint16_t result = self->A + (~self->D) + 1; // add the 2's complement of D and 1
    self->A = result & 0xff;
    update_flags_sub(self, result);
    self->PC++;
}

void instr_0x93(i8080oObject *self) {
    #ifdef DEBUG
    printf("SUB E\n");
    #endif
    uint16_t result = self->A + (~self->E) + 1; // add the 2's complement of E and 1
    self->A = result & 0xff;
    update_flags_sub(self, result);
    self->PC++;
}

void instr_0x94(i8080oObject *self) {
    #ifdef DEBUG
    printf("SUB H\n");
    #endif
    uint16_t result = self->A + (~self->H) + 1; // add the 2's complement of H and 1
    self->A = result & 0xff;
    update_flags_sub(self, result);
    self->PC++;
}

void instr_0x95(i8080oObject *self) {
    #ifdef DEBUG
    printf("SUB L\n");
    #endif
    uint16_t result = self->A + (~self->L) + 1; // add the 2's complement of L and 1
    self->A = result & 0xff;
    update_flags_sub(self, result);
    self->PC++;
}

void instr_0x96(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("SUB M\n");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    uint16_t result = self->A + (~self->memory[addr]) + 1; // add the 2's complement of M and 1
    self->A = result & 0xff;
    update_flags_sub(self, result);
    self->PC++;
}

void instr_0x97(i8080oObject *self) {
    #ifdef DEBUG
    printf("SUB A\n");
    #endif
    uint16_t result = self->A + (~self->A) + 1; // add the 2's complement of A and 1
    self->A = result & 0xff;
    update_flags_sub(self, result);
    self->PC++;
}

// END SUB

/*
SBB Subtract Register or Memory from Accumulator with Borrow

Condition Bits Affected:
Carry Flag: Reset if borrow from bit 7; set otherwise.
Sign Flag: Set if bit 7 of result is set; reset otherwise.
Zero Flag: Set if result is zero; reset otherwise.
Parity Flag: Set if result has even parity; reset otherwise.
Auxiliary Carry Flag: Set if borrow from bit 4; reset otherwise.
*/

void instr_0x98(i8080oObject *self) {
    #ifdef DEBUG
    printf("SBB B\n");
    #endif
    uint16_t result = self->A + ~(self->B + self->CC.cy) +  1; // no idea what happens if B + 1 overflows
    self->A = result & 0xff;
    update_flags_sub(self, result);
    self->PC++;
}

void instr_0x99(i8080oObject *self) {
    #ifdef DEBUG
    printf("SBB C\n");
    #endif
    uint16_t result = self->A + ~(self->C + self->CC.cy) +  1;
    self->A = result & 0xff;
    update_flags_sub(self, result);
    self->PC++;
}

void instr_0x9a(i8080oObject *self) {
    #ifdef DEBUG
    printf("SBB D\n");
    #endif
    uint16_t result = self->A + ~(self->D + self->CC.cy) +  1;
    self->A = result & 0xff;
    update_flags_sub(self, result);
    self->PC++;
}

void instr_0x9b(i8080oObject *self) {
    #ifdef DEBUG
    printf("SBB E\n");
    #endif
    uint16_t result = self->A + ~(self->E + self->CC.cy) +  1;
    self->A = result & 0xff;
    update_flags_sub(self, result);
    self->PC++;
}

void instr_0x9c(i8080oObject *self) {
    #ifdef DEBUG
    printf("SBB H\n");
    #endif
    uint16_t result = self->A + ~(self->H + self->CC.cy) +  1;
    self->A = result & 0xff;
    update_flags_sub(self, result);
    self->PC++;
}

void instr_0x9d(i8080oObject *self) {
    #ifdef DEBUG
    printf("SBB L\n");
    #endif
    uint16_t result = self->A + ~(self->L + self->CC.cy) +  1;
    self->A = result & 0xff;
    update_flags_sub(self, result);
    self->PC++;
}

void instr_0x9e(i8080oObject *self) {
    #ifdef DEBUG
    printf("SBB M\n");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    uint16_t result = self->A + ~(self->memory[addr] + self->CC.cy) +  1;
    self->A = result & 0xff;
    update_flags_sub(self, result);
    self->PC++;
}

void instr_0x9f(i8080oObject *self) {
    #ifdef DEBUG
    printf("SBB A\n");
    #endif
    uint16_t result = self->A + ~(self->A + self->CC.cy) +  1;
    self->A = result & 0xff;
    update_flags_sub(self, result);
    self->PC++;
}

// END SBB

/*
ANA Logical AND Register or Memory with Accumulator

Condition Bits Affected:
Carry Flag: Reset.
Zero Flag: Set if result is zero; reset otherwise.
Sign Flag: Set if bit 7 of result is set; reset otherwise.
Parity Flag: Set if result has even parity; reset otherwise.
*/

static inline void update_flags_ana(i8080oObject *self) {
    self->CC.z = (self->A) == 0;
    self->CC.s = (self->A & 0x80) != 0;
    self->CC.p = parity_check(self->A);
    self->CC.cy = 0;
}

void instr_0xa0(i8080oObject *self) {
    #ifdef DEBUG
    printf("ANA B\n");
    #endif
    self->A = self->A & self->B;
    update_flags_ana(self);
    self->PC++;
}

void instr_0xa1(i8080oObject *self) {
    #ifdef DEBUG
    printf("ANA C\n");
    #endif
    self->A = self->A & self->C;
    update_flags_ana(self);
    self->PC++;
}

void instr_0xa2(i8080oObject *self) {
    #ifdef DEBUG
    printf("ANA D\n");
    #endif
    self->A = self->A & self->D;
    update_flags_ana(self);
    self->PC++;
}

void instr_0xa3(i8080oObject *self) {
    #ifdef DEBUG
    printf("ANA E\n");
    #endif
    self->A = self->A & self->E;
    update_flags_ana(self);
    self->PC++;
}

void instr_0xa4(i8080oObject *self) {
    #ifdef DEBUG
    printf("ANA H\n");
    #endif
    self->A = self->A & self->H;
    update_flags_ana(self);
    self->PC++;
}

void instr_0xa5(i8080oObject *self) {
    #ifdef DEBUG
    printf("ANA L\n");
    #endif
    self->A = self->A & self->L;
    update_flags_ana(self);
    self->PC++;
}

void instr_0xa6(i8080oObject *self) {
    #ifdef DEBUG
    printf("ANA M\n");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    self->A = self->A & self->memory[addr];
    update_flags_ana(self);
    self->PC++;
}

void instr_0xa7(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("ANA A\n");
    #endif
    self->A = self->A & self->A;
    update_flags_ana(self);
    self->PC++;
}

// END ANA

/*
XRA Logical Exclusive OR Register or Memory with Accumulator

Condition Bits Affected:
Carry Flag: Reset.
Zero Flag: Set if result is zero; reset otherwise.
Sign Flag: Set if bit 7 of result is set; reset otherwise.
Parity Flag: Set if result has even parity; reset otherwise.
Auxiliary Carry Flag: Reset. (Undocumented, not sure)
*/

static inline void update_flags_xra(i8080oObject *self) {
    self->CC.z = (self->A) == 0;
    self->CC.s = (self->A & 0x80) != 0;
    self->CC.p = parity_check(self->A);
    self->CC.cy = 0;
    self->CC.ac = 0;
}

void instr_0xa8(i8080oObject *self) {
    #ifdef DEBUG
    printf("XRA B\n");
    #endif
    self->A = self->A ^ self->B;
    update_flags_xra(self);
    self->PC++;
}

void instr_0xa9(i8080oObject *self) {
    #ifdef DEBUG
    printf("XRA C\n");
    #endif
    self->A = self->A ^ self->C;
    update_flags_xra(self);
    self->PC++;
}

void instr_0xaa(i8080oObject *self) {
    #ifdef DEBUG
    printf("XRA D\n");
    #endif
    self->A = self->A ^ self->D;
    update_flags_xra(self);
    self->PC++;
}

void instr_0xab(i8080oObject *self) {
    #ifdef DEBUG
    printf("XRA E\n");
    #endif
    self->A = self->A ^ self->E;
    update_flags_xra(self);
    self->PC++;
}

void instr_0xac(i8080oObject *self) {
    #ifdef DEBUG
    printf("XRA H\n");
    #endif
    self->A = self->A ^ self->H;
    update_flags_xra(self);
    self->PC++;
}

void instr_0xad(i8080oObject *self) {
    #ifdef DEBUG
    printf("XRA L\n");
    #endif
    self->A = self->A ^ self->L;
    update_flags_xra(self);
    self->PC++;
}

void instr_0xae(i8080oObject *self) {
    #ifdef DEBUG
    printf("XRA M\n");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    self->A = self->A ^ self->memory[addr];
    update_flags_xra(self);
    self->PC++;
}

void instr_0xaf(i8080oObject *self) {
    #ifdef DEBUG
    printf("XRA A\n");
    #endif
    self->A = self->A ^ self->A;
    update_flags_xra(self);
    self->PC++;
}

// END XRA

/*
ORA Logical OR Register or Memory with Accumulator

Condition Bits Affected:
Carry Flag: Reset.
Zero Flag: Set if result is zero; reset otherwise.
Sign Flag: Set if bit 7 of result is set; reset otherwise.
Parity Flag: Set if result has even parity; reset otherwise.
*/

static inline void update_flags_ora(i8080oObject *self) {
    self->CC.z = (self->A) == 0;
    self->CC.s = (self->A & 0x80) != 0;
    self->CC.p = parity_check(self->A);
    self->CC.cy = 0;
}

void instr_0xb0(i8080oObject *self) {
    #ifdef DEBUG
    printf("ORA B\n");
    #endif
    self->A = self->A | self->B;
    update_flags_ora(self);
    self->PC++;
}

void instr_0xb1(i8080oObject *self) {
    #ifdef DEBUG
    printf("ORA C\n");
    #endif
    self->A = self->A | self->C;
    update_flags_ora(self);
    self->PC++;
}

void instr_0xb2(i8080oObject *self) {
    #ifdef DEBUG
    printf("ORA D\n");
    #endif
    self->A = self->A | self->D;
    update_flags_ora(self);
    self->PC++;
}

void instr_0xb3(i8080oObject *self) {
    #ifdef DEBUG
    printf("ORA E\n");
    #endif
    self->A = self->A | self->E;
    update_flags_ora(self);
    self->PC++;
}

void instr_0xb4(i8080oObject *self) {
    #ifdef DEBUG
    printf("ORA H\n");
    #endif
    self->A = self->A | self->H;
    update_flags_ora(self);
    self->PC++;
}

void instr_0xb5(i8080oObject *self) {
    #ifdef DEBUG
    printf("ORA L\n");
    #endif
    self->A = self->A | self->L;
    update_flags_ora(self);
    self->PC++;
}

void instr_0xb6(i8080oObject *self) {
    #ifdef DEBUG
    printf("ORA M\n");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    self->A = self->A | self->memory[addr];
    update_flags_ora(self);
    self->PC++;
}

void instr_0xb7(i8080oObject *self) {
    #ifdef DEBUG
    printf("ORA A\n");
    #endif
    self->A = self->A | self->A;
    update_flags_ora(self);
    self->PC++;
}

// END ORA

/*
CMP Compare Register or Memory with Accumulator

Condition Bits Affected:
Carry Flag: Set if accumulator is less than source operand; reset otherwise.
Zero Flag: Set if accumulator is equal to source operand; reset otherwise.
Sign Flag: Set if bit 7 of accumulator is set; reset otherwise.
Parity Flag: Set if accumulator has even parity; reset otherwise.
Auxiliary Carry Flag: Set if borrow from bit 4; reset otherwise. (Undocumented, not sure)
*/

static inline void update_flags_cmp(i8080oObject *self, uint8_t reg) {
    self->CC.z = (self->A) == reg;
    self->CC.cy = (self->A) < reg;
    self->CC.s = (self->A & 0x80) != 0;
    self->CC.p = parity_check(self->A); // Undocumented, not sure
    self->CC.ac = (self->A & 0x0f) < (reg & 0x0f); // Undocumented, not sure
}

void instr_0xb8(i8080oObject *self) {
    #ifdef DEBUG
    printf("CMP B\n");
    #endif
    update_flags_cmp(self, self->B);
    self->PC++;
}

void instr_0xb9(i8080oObject *self) {
    #ifdef DEBUG
    printf("CMP C\n");
    #endif
    update_flags_cmp(self, self->C);
    self->PC++;
}

void instr_0xba(i8080oObject *self) {
    #ifdef DEBUG
    printf("CMP D\n");
    #endif
    update_flags_cmp(self, self->D);
    self->PC++;
}

void instr_0xbb(i8080oObject *self) {
    #ifdef DEBUG
    printf("CMP E\n");
    #endif
    update_flags_cmp(self, self->E);
    self->PC++;
}

void instr_0xbc(i8080oObject *self) {
    #ifdef DEBUG
    printf("CMP H\n");
    #endif
    update_flags_cmp(self, self->H);
    self->PC++;
}

void instr_0xbd(i8080oObject *self) {
    #ifdef DEBUG
    printf("CMP L\n");
    #endif
    update_flags_cmp(self, self->L);
    self->PC++;
}

void instr_0xbe(i8080oObject *self) {
    #ifdef DEBUG
    printf("CMP M\n");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    update_flags_cmp(self, self->memory[addr]);
    self->PC++;
}

void instr_0xbf(i8080oObject *self) {
    #ifdef DEBUG
    printf("CMP A\n");
    #endif
    update_flags_cmp(self, self->A);
    self->PC++;
}

// END CMP

/*
Rotate Accumulator Instructions
*/

/*
RLC Rotate Accumulator Left

Condition Bits Affected:
Carry Flag: Set to value of bit 7 of accumulator before operation.
*/

void instr_0x07(i8080oObject *self) {
    #ifdef DEBUG
    printf("RLC\n");
    #endif
    uint8_t result = self->A << 1;
    self->CC.cy = (0x80 == (self->A & 0x80));
    self->A = result | self->CC.cy;
    self->PC++;
}

// END RLC

/*
RRC Rotate Accumulator Right

Condition Bits Affected:
Carry Flag: Set to value of bit 0 of accumulator before operation.
*/

void instr_0x0f(i8080oObject *self) {
    #ifdef DEBUG
    printf("RRC\n");
    #endif
    uint8_t result = self->A;
    self->A = ((result & 1) << 7) | (result >> 1);
    self->CC.cy = (1 == (result&1));
    self->PC++;
}

// END RRC

/*
RAL Rotate Accumulator Left Through Carry

Condition Bits Affected:
Carry Flag: Set to value of bit 7 of accumulator before operation.
*/

void instr_0x17(i8080oObject *self) {
    #ifdef DEBUG
    printf("RAL\n");
    #endif
    uint8_t result = self->A << 1;
    result |= self->CC.cy;
    self->CC.cy = (0x80 == (self->A & 0x80));
    self->A = result;
    self->PC++;
}

// END RAL

/*
RAR Rotate Accumulator Right Through Carry

Condition Bits Affected:
Carry Flag: Set to value of bit 0 of accumulator before operation.
*/

void instr_0x1f(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("RAR\n");
    #endif
    uint8_t result = self->A >> 1;
    result |= (self->CC.cy << 7);
    self->CC.cy = (0x01 == (self->A & 0x01));
    self->A = result;
    self->PC++;
}

// END RAR

/*
Register Pair Instructions
*/

/*
PUSH Push Data Onto Stack

Condition Bits Affected:
All condition bits are unaffected.
*/

void instr_0xc5(i8080oObject *self) {
    #ifdef DEBUG
    printf("PUSH B\n");
    #endif
    self->memory[self->SP-1] = self->B;
    self->memory[self->SP-2] = self->C;
    self->SP -= 2;
    self->PC++;
}

void instr_0xd5(i8080oObject *self) {
    #ifdef DEBUG
    printf("PUSH D\n");
    #endif
    self->memory[self->SP-1] = self->D;
    self->memory[self->SP-2] = self->E;
    self->SP -= 2;
    self->PC++;
}

void instr_0xe5(i8080oObject *self) {
    #ifdef DEBUG
    printf("PUSH H\n");
    #endif
    self->memory[self->SP-1] = self->H;
    self->memory[self->SP-2] = self->L;
    self->SP -= 2;
    self->PC++;
}

void instr_0xf5(i8080oObject *self) {
    #ifdef DEBUG
    printf("PUSH PSW\n");
    #endif
    self->memory[self->SP-1] = self->A;
    self->memory[self->SP-2] = (self->CC.z << 0) | (self->CC.s << 1) | (self->CC.p << 2) | (self->CC.cy << 3) | (self->CC.ac << 4);
    self->SP -= 2;
    self->PC++;
}

// END PUSH

/*
POP Pop Data Off Stack

Condition Bits Affected:
All condition bits are unaffected.
*/

void instr_0xc1(i8080oObject *self) {
    #ifdef DEBUG
    printf("POP B\n");
    #endif
    self->C = self->memory[self->SP];
    self->B = self->memory[self->SP+1];
    self->SP += 2;
    self->PC++;
}

void instr_0xd1(i8080oObject *self) {
    #ifdef DEBUG
    printf("POP D\n");
    #endif
    self->E = self->memory[self->SP];
    self->D = self->memory[self->SP+1];
    self->SP += 2;
    self->PC++;
}

void instr_0xe1(i8080oObject *self) {
    #ifdef DEBUG
    printf("POP H\n");
    #endif
    self->L = self->memory[self->SP];
    self->H = self->memory[self->SP+1];
    self->SP += 2;
    self->PC++;
}

void instr_0xf1(i8080oObject *self) {
    #ifdef DEBUG
    printf("POP PSW\n");
    #endif
    self->A = self->memory[self->SP+1];
    uint8_t flags = self->memory[self->SP];
    self->CC.z = (flags & 0x01) ? 1 : 0;
    self->CC.s = (flags & 0x02) ? 1 : 0;
    self->CC.p = (flags & 0x04) ? 1 : 0;
    self->CC.cy = (flags & 0x08) ? 1 : 0;
    self->CC.ac = (flags & 0x10) ? 1 : 0;
    self->SP += 2;
    self->PC++;
}

// END POP

/*
DAD Double Add

Condition Bits Affected:
Carry Flag: Set if carry from bit 15
*/

void instr_0x09(i8080oObject * self){
    #ifdef DEBUG
    printf("DAD B\n");
    #endif
    uint32_t hl = (self->H << 8) | self->L;
    uint32_t bc = (self->B << 8) | self->C;
    uint32_t res = hl + bc;
    self->H = (res & 0xff00) >> 8;
    self->L = res & 0xff;
    self->CC.cy = ((res & 0xffff0000) != 0);
    self->PC++;
}

void instr_0x19(i8080oObject * self){
    #ifdef DEBUG
    printf("DAD D\n");
    #endif
    uint32_t hl = (self->H << 8) | self->L;
    uint32_t de = (self->D << 8) | self->E;
    uint32_t res = hl + de;
    self->H = (res & 0xff00) >> 8;
    self->L = res & 0xff;
    //self->CC.cy = ((res & 0xffff0000) > 0);
    self->CC.cy = ((res & 0xffff0000) != 0);
    self->PC++;
}

void instr_0x29(i8080oObject * self){
    #ifdef DEBUG
    printf("DAD H\n");
    #endif
    uint32_t hl = (self->H << 8) | self->L;
    uint32_t res = hl + hl;
    self->H = (res & 0xff00) >> 8;
    self->L = res & 0xff;
    self->CC.cy = ((res & 0xffff0000) != 0);
    self->PC++;
}

void instr_0x39(i8080oObject *self) {
    #ifdef DEBUG
    printf("DAD SP\n");
    #endif
    uint32_t sp = self->SP;
    uint32_t hl = (self->H << 8) | self->L;
    uint32_t res = sp + hl;
    self->H = (res & 0xff00) >> 8;
    self->L = res & 0xff;
    self->CC.cy = ((res & 0xffff0000) != 0);
    self->PC++;
}

// END DAD

/*
INX Increment Register Pair

Condition Bits Affected:
All condition bits are unaffected.
*/

void instr_0x03(i8080oObject *self) {
    #ifdef DEBUG
    printf("INX B\n");
    #endif
    self->C++;
    if (self->C == 0) {
        self->B++;
    }
    self->PC++;
}

void instr_0x13(i8080oObject *self) {
    #ifdef DEBUG
    printf("INX D\n");
    #endif
    self->E++;
    if (self->E == 0) {
        self->D++;
    }
    self->PC++;
}

void instr_0x23(i8080oObject *self) {
    #ifdef DEBUG
    printf("INX H\n");
    #endif
    self->L++;
    if (self->L == 0) {
        self->H++;
    }
    self->PC++;
}

void instr_0x33(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("INX SP\n");
    #endif
    self->SP++;
    self->PC++;
}

// END INX

/*
DCX Decrement Register Pair

Condition Bits Affected:
All condition bits are unaffected.
*/

void instr_0x0b(i8080oObject *self) {
    #ifdef DEBUG
    printf("DCX B\n");
    #endif
    self->C--;
    if (self->C == 0xff) {
        self->B--;
    }
    self->PC++;
}

void instr_0x1b(i8080oObject *self) {
    #ifdef DEBUG
    printf("DCX D\n");
    #endif
    self->E--;
    if (self->E == 0xff) {
        self->D--;
    }
    self->PC++;
}

void instr_0x2b(i8080oObject *self) {
    #ifdef DEBUG
    printf("DCX H\n");
    #endif
    self->L--;
    if (self->L == 0xff) {
        self->H--;
    }
    self->PC++;
}

void instr_0x3b(i8080oObject *self) {
    #ifdef DEBUG
    printf("DCX SP\n");
    #endif
    self->SP--;
    self->PC++;
}

// END DCX

/*
XCHG Exchange Registers

Condition Bits Affected:
All condition bits are unaffected.
*/

void instr_0xeb(i8080oObject *self) {
    #ifdef DEBUG
    printf("XCHG\n");
    #endif
    uint8_t save1 = self->D;
    uint8_t save2 = self->E;
    self->D = self->H;
    self->E = self->L;
    self->H = save1;
    self->L = save2;
    self->PC++;
}

// END XCHG

/*
XTHL Exchange Stack

Condition Bits Affected:
All condition bits are unaffected.
*/

void instr_0xe3(i8080oObject *self) {
    #ifdef DEBUG
    printf("XTHL\n");
    #endif
    uint8_t save1 = self->memory[self->SP];
    uint8_t save2 = self->memory[self->SP+1];
    self->memory[self->SP] = self->L;
    self->memory[self->SP+1] = self->H;
    self->L = save1;
    self->H = save2;
    self->PC++;
}

// END XTHL

/*
SPHL Load SP From H And L

Condition Bits Affected:
All condition bits are unaffected.
*/

void instr_0xf9(i8080oObject *self) {
    #ifdef DEBUG
    printf("SPHL\n");
    #endif
    self->SP = (self->H << 8) | self->L;
    self->PC++;
}

// END SPHL

/*
Immediate Instructions
*/

/*
LXI Load Immediate Data

Condition Bits Affected:
All condition bits are unaffected.
*/

void instr_0x01(i8080oObject *self) {
    #ifdef DEBUG
    printf("LXI B\n");
    #endif
    self->C = self->memory[self->PC + 1];
    self->B = self->memory[self->PC + 2];
    self->PC+=3;
}

void instr_0x11(i8080oObject *self) {
    #ifdef DEBUG
    printf("LXI D\n");
    #endif
    self->E = self->memory[self->PC + 1];
    self->D = self->memory[self->PC + 2];
    self->PC+=3;
}

void instr_0x21(i8080oObject *self) {
    #ifdef DEBUG
    printf("LXI H\n");
    #endif
    self->L = self->memory[self->PC + 1];
    self->H = self->memory[self->PC + 2];
    self->PC+=3;
}

void instr_0x31(i8080oObject *self) {
    #ifdef DEBUG
    printf("LXI SP\n");
    #endif
    self->SP = (self->memory[self->PC + 2] << 8) + self->memory[self->PC + 1];
    self->PC+=3;
}

// END LXI

/*
MVI Move Immediate Data

Condition Bits Affected:
All condition bits are unaffected.
*/

void instr_0x06(i8080oObject *self) {
    #ifdef DEBUG
    printf("MVI B\n");
    #endif
    self->B = self->memory[self->PC + 1];
    self->PC+=2;
}

void instr_0x0e(i8080oObject *self) {
    #ifdef DEBUG
    printf("MVI C\n");
    #endif
    self->C = self->memory[self->PC + 1];
    self->PC+=2;
}

void instr_0x16(i8080oObject *self) {
    #ifdef DEBUG
    printf("MVI D\n");
    #endif
    self->D = self->memory[self->PC + 1];
    self->PC+=2;
}

void instr_0x1e(i8080oObject *self) {
    #ifdef DEBUG
    printf("MVI E\n");
    #endif
    self->E = self->memory[self->PC + 1];
    self->PC+=2;
}

void instr_0x26(i8080oObject *self) {
    #ifdef DEBUG
    printf("MVI H\n");
    #endif
    self->H = self->memory[self->PC + 1];
    self->PC+=2;
}

void instr_0x2e(i8080oObject *self) {
    #ifdef DEBUG
    printf("MVI L\n");
    #endif
    self->L = self->memory[self->PC + 1];
    self->PC+=2;
}

void instr_0x36(i8080oObject *self) {
    #ifdef DEBUG
    printf("MVI M\n");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    self->memory[addr] = self->memory[self->PC + 1];
    self->PC+=2;
}

void instr_0x3e(i8080oObject *self) {
    #ifdef DEBUG
    printf("MVI A\n");
    #endif
    self->A = self->memory[self->PC + 1];
    self->PC+=2;
}

// END MVI

/*
ADI Add Immediate To Accumulator

Condition Bits Affected:
Carry Flag: Set if carry from bit 7; otherwise, reset.
Sign Flag: Set if result is negative; otherwise, reset.
Zero Flag: Set if result is zero; otherwise, reset.
Parity Flag: Set if result is even parity; otherwise, reset.
Auxiliary Carry Flag: Set if carry from bit 3; otherwise, reset.
*/

void instr_0xc6(i8080oObject *self) {
    #ifdef DEBUG
    printf("ADI\n");
    #endif
    uint16_t result = self->A + self->memory[self->PC+1];
    self->CC.z = ((result & 0xff) == 0);
    self->CC.s = (0x80 == (result & 0x80));
    self->CC.p = parity_check((result&0xff));
    self->CC.cy = (result > 0xff);
    self->A = (uint8_t) (result & 0xff);
    self->PC+=2;
}

// END ADI

/*
ACI Add Immediate With Carry To Accumulator

Condition Bits Affected:
Carry Flag: Set if carry from bit 7; otherwise, reset.
Sign Flag: Set if result is negative; otherwise, reset.
Zero Flag: Set if result is zero; otherwise, reset.
Parity Flag: Set if result is even parity; otherwise, reset.
Auxiliary Carry Flag: Set if carry from bit 3; otherwise, reset.
*/

void instr_0xce(i8080oObject *self) {
    #ifdef DEBUG
    printf("ACI\n");
    #endif
    uint16_t result = self->A + self->memory[self->PC+1] + self->CC.cy;
    self->CC.z = ((result & 0xff) == 0);
    self->CC.s = (0x80 == (result & 0x80));
    self->CC.p = parity_check((result&0xff));
    self->CC.cy = (result > 0xff);
    self->A = (uint8_t) (result & 0xff);
    self->PC+=2;
}

// END ACI

/*
SUI Subtract Immediate From Accumulator

Condition Bits Affected:
Carry Flag: Set if borrow from bit 7; otherwise, reset.
Sign Flag: Set if result is negative; otherwise, reset.
Zero Flag: Set if result is zero; otherwise, reset.
Parity Flag: Set if result is even parity; otherwise, reset.
Auxiliary Carry Flag: Set if borrow from bit 3; otherwise, reset.
*/

void instr_0xd6(i8080oObject *self) {
    #ifdef DEBUG
    printf("SUI\n");
    #endif
    uint16_t result = self->A + (~self->memory[self->PC+1]) + 1;
    self->A = result & 0xff;
    update_flags_sub(self, result);
    self->PC+=2;
}

// END SUI

/*
SBI Subtract Immediate from Accumulator with Borrow

Condition Bits Affected:
Carry Flag: Set if borrow from bit 7; otherwise, reset.
Sign Flag: Set if result is negative; otherwise, reset.
Zero Flag: Set if result is zero; otherwise, reset.
Parity Flag: Set if result is even parity; otherwise, reset.
Auxiliary Carry Flag: Set if borrow from bit 3; otherwise, reset.
*/

void instr_0xde(i8080oObject *self) {
    #ifdef DEBUG
    printf("SBI\n");
    #endif
    uint16_t result = self->A + ~(self->memory[self->PC+1] + self->CC.cy) +  1;
    self->A = result & 0xff;
    update_flags_sub(self, result);
    self->PC+=2;
}

// END SBI

/*
ANI And Immediate With Accumulator

Condition Bits Affected:
Carry Flag: Reset.
Zero Flag: Set if result is zero; otherwise, reset.
Sign Flag: Set if result is negative; otherwise, reset.
Parity Flag: Set if result is even parity; otherwise, reset.
*/

void instr_0xe6(i8080oObject *self) {
    #ifdef DEBUG
    printf("ANI\n");
    #endif
    self->A = self->A & self->memory[self->PC+1];
    update_flags_ana(self);
    self->PC+=2;
}

// END ANI

/*
XRI Exclusive Or Immediate With Accumulator

Condition Bits Affected:
Carry Flag: Reset.
Zero Flag: Set if result is zero; otherwise, reset.
Sign Flag: Set if result is negative; otherwise, reset.
Parity Flag: Set if result is even parity; otherwise, reset.
*/

void instr_0xee(i8080oObject *self) {
    #ifdef DEBUG
    printf("XRI\n");
    #endif
    self->A = self->A ^ self->memory[self->PC+1];
    update_flags_ana(self);
    self->PC+=2;
}

// END XRI

/*
ORI Or Immediate With Accumulator

Condition Bits Affected:
Carry Flag: Reset.
Zero Flag: Set if result is zero; otherwise, reset.
Sign Flag: Set if result is negative; otherwise, reset.
Parity Flag: Set if result is even parity; otherwise, reset.
*/

void instr_0xf6(i8080oObject *self) {
    #ifdef DEBUG
    printf("ORI\n");
    #endif
    self->A = self->A | self->memory[self->PC+1];
    update_flags_ora(self);
    self->PC+=2;
}

// END ORI

/*
CPI Compare Immediate With Accumulator

Condition Bits Affected:
Carry Flag: Set if borrow from bit 7; otherwise, reset.
Sign Flag: Set if result is negative; otherwise, reset.
Zero Flag: Set if result is zero; otherwise, reset.
Parity Flag: Set if result is even parity; otherwise, reset.
Auxiliary Carry Flag: Set if borrow from bit 3; otherwise, reset.
*/

void instr_0xfe(i8080oObject *self) {
    #ifdef DEBUG
    printf("CPI\n");
    #endif
    update_flags_cmp(self, self->memory[self->PC+1]);
    self->PC+=2;
}

// END CPI

/*
Direct Addressing Instructions
*/

/*
STA Store Accumulator Direct

Condition Bits Affected: None.
*/

void instr_0x32(i8080oObject *self) {
    #ifdef DEBUG
    printf("STA\n");
    #endif
    uint16_t addr = (self->memory[self->PC + 2] << 8) + self->memory[self->PC + 1];
    self->memory[addr] = self->A;
    self->PC+=3;
}

// END STA

/*
LDA Load Accumulator Direct

Condition Bits Affected: None.
*/

void instr_0x3a(i8080oObject *self) {
    #ifdef DEBUG
    printf("LDA\n");
    #endif
    uint16_t addr = (self->memory[self->PC + 2] << 8) + self->memory[self->PC + 1];
    self->A = self->memory[addr];
    self->PC+=3;
}

// END LDA

/*
SHLD Store H and L Direct

Condition Bits Affected: None.
*/

void instr_0x22(i8080oObject *self) {
    #ifdef DEBUG
    printf("SHLD\n");
    #endif
    uint16_t addr = (self->memory[self->PC + 2] << 8)+ self->memory[self->PC + 1];
    self->memory[addr] = self->L;
    self->memory[addr + 1] = self->H;
    self->PC+=3;
}

// END SHLD

/*
LHLD Load H and L Direct

Condition Bits Affected: None.
*/

void instr_0x2a(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("LHLD\n");
    #endif
    uint16_t addr = (self->memory[self->PC + 2] << 8) + self->memory[self->PC + 1];
    self->L = self->memory[addr];
    self->H = self->memory[addr + 1];
    self->PC+=3;
}

// END LHLD

/*
JUMP Instructions
*/

/*
PCHL Load Program Counter

Condition Bits Affected: None.
*/

void instr_0xe9(i8080oObject *self) {
    #ifdef DEBUG
    printf("PCHL\n");
    #endif
    self->PC = (self->H << 8) | self->L;
    self->PC++;
}

// END PCHL

/*
JMP Jump

Condition Bits Affected: None.
*/

void instr_0xc3(i8080oObject *self) {
    #ifdef DEBUG
    printf("JMP\n");
    #endif
    self->PC = (self->memory[self->PC+2] << 8) | self->memory[self->PC+1];
}

// END JMP

/*
JC Jump if Carry

Condition Bits Affected: None.
*/

void instr_0xda(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("JC\n");
    #endif
    if (1 == self->CC.cy)
        self->PC = (self->memory[self->PC+2] << 8) | self->memory[self->PC+1];
    else
        self->PC += 3;
}

// END JC

/*
JNC Jump if No Carry

Condition Bits Affected: None.
*/

void instr_0xd2(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("JNC\n");
    #endif
    if (0 == self->CC.cy)
        self->PC = (self->memory[self->PC+2] << 8) | self->memory[self->PC+1];
    else
        self->PC += 3;
}

// END JNC

/*
JZ Jump if Zero

Condition Bits Affected: None.
*/

void instr_0xca(i8080oObject *self) {
    #ifdef DEBUG
    printf("JZ\n");
    #endif
    if (1 == self->CC.z)
        self->PC = (self->memory[self->PC+2] << 8) | self->memory[self->PC+1];
    else
        self->PC += 3;
}

// END JZ

/*
JNZ Jump if Not Zero

Condition Bits Affected: None.
*/

void instr_0xc2(i8080oObject *self) {
    #ifdef DEBUG
    printf("JNZ\n");
    #endif
    if (0 == self->CC.z)
        self->PC = (self->memory[self->PC+2] << 8) | self->memory[self->PC+1];
    else
        self->PC += 3;
}

// END JNZ

/*
JM Jump if Minus

Condition Bits Affected: None.
*/

void instr_0xfa(i8080oObject *self) {
    #ifdef DEBUG
    printf("JM\n");
    #endif
    if (1 == self->CC.s)
        self->PC = (self->memory[self->PC+2] << 8) | self->memory[self->PC+1];
    else
        self->PC += 3;
}

// END JM

/*
JP Jump if Positive

Condition Bits Affected: None.
*/

void instr_0xf2(i8080oObject *self) {
    #ifdef DEBUG
    printf("JP\n");
    #endif
    if (0 == self->CC.s)
        self->PC = (self->memory[self->PC+2] << 8) | self->memory[self->PC+1];
    else
        self->PC += 3;
}

// END JP

/*
JPE Jump if Parity Even

Condition Bits Affected: None.
*/

void instr_0xea(i8080oObject *self) {
    #ifdef DEBUG
    printf("JPE\n");
    #endif
    if (1 == self->CC.p)
        self->PC = (self->memory[self->PC+2] << 8) | self->memory[self->PC+1];
    else
        self->PC += 3;
}

// END JPE

/*
JPO Jump if Parity Odd

Condition Bits Affected: None.
*/

void instr_0xe2(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("JPO\n");
    #endif
    if (0 == self->CC.p)
        self->PC = (self->memory[self->PC+2] << 8) | self->memory[self->PC+1];
    else
        self->PC += 3;
}

// END JPO

/*
CALL Subroutine Instructions
*/

/*
CALL Call

Condition Bits Affected: None.
*/

void call(i8080oObject *self, uint16_t ret) {
    self->memory[self->SP-1] = (ret >> 8) & 0xff;
    self->memory[self->SP-2] = (ret & 0xff);
    self->SP = self->SP - 2;
    self->PC = (self->memory[self->PC+2] << 8) | self->memory[self->PC+1];
}

void instr_0xcd(i8080oObject *self) {
    #ifdef DEBUG
    printf("CALL\n");
    #endif
    call(self, self->PC+3);
}

/*
CC Call if Carry

Condition Bits Affected: None.
*/

void instr_0xdc(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("CC\n");
    #endif
    if (1 == self->CC.cy) {
        call(self, self->PC+3);
    } else {
        self->PC += 3;
    }
}

/*
CNC Call if No Carry

Condition Bits Affected: None.
*/

void instr_0xd4(i8080oObject *self) {
    #ifdef DEBUG
    printf("CNC\n");
    #endif
    if (0 == self->CC.cy) {
        call(self, self->PC+3);
    } else {
        self->PC += 3;
    }
}

/*
CZ Call if Zero

Condition Bits Affected: None.
*/

void instr_0xcc(i8080oObject *self) {
    #ifdef DEBUG
    printf("CZ\n");
    #endif
    if (1 == self->CC.z) {
        call(self, self->PC+3);
    } else {
        self->PC += 3;
    }
}

/*
CNZ Call if Not Zero

Condition Bits Affected: None.
*/

void instr_0xc4(i8080oObject *self) {
    #ifdef DEBUG
    printf("CNZ\n");
    #endif
    if (0 == self->CC.z) {
        call(self, self->PC+3);
    } else {
        self->PC += 3;
    }
}

/*
CM Call if Minus

Condition Bits Affected: None.
*/

void instr_0xfc(i8080oObject *self) {
    #ifdef DEBUG
    printf("CM\n");
    #endif
    if (1 == self->CC.s) {
        call(self, self->PC+3);
    } else {
        self->PC += 3;
    }
}

/*
CP Call if Plus

Condition Bits Affected: None.
*/

void instr_0xf4(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("CP\n");
    #endif
    if (0 == self->CC.s) {
        call(self, self->PC+3);
    } else {
        self->PC += 3;
    }
}

/*
CPE Call if Parity Even

Condition Bits Affected: None.
*/

void instr_0xec(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("CPE\n");
    #endif
    if (1 == self->CC.p) {
        call(self, self->PC+3);
    } else {
        self->PC += 3;
    }
}

/*
CPO Call if Parity Odd

Condition Bits Affected: None.
*/

void instr_0xe4(i8080oObject *self) {
    #ifdef DEBUG
    printf("CPO\n");
    #endif
    if (0 == self->CC.p) {
        call(self, self->PC+3);
    } else {
        self->PC += 3;
    }
}

// END CALL

/*
RET Return Instructions
*/

void ret(i8080oObject *self) {
    self->PC = (self->memory[self->SP+1] << 8) | self->memory[self->SP];
    self->SP = self->SP + 2;
}

void instr_0xc9(i8080oObject *self) {
    #ifdef DEBUG
    printf("RET\n");
    #endif
    ret(self);
}

/*
RC Return if Carry

Condition Bits Affected: None.
*/

void instr_0xd8(i8080oObject *self) {
    #ifdef DEBUG
    printf("RC\n");
    #endif
    
    if (1 == self->CC.cy) {
        ret(self);
    } else {
        self->PC += 1;
    }
}

/*
RNC Return if No Carry

Condition Bits Affected: None.
*/

void instr_0xd0(i8080oObject *self) {
    #ifdef DEBUG
    printf("RNC\n");
    #endif
    if (0 == self->CC.cy) {
        ret(self);
    } else {
        self->PC += 1;
    }

}

/*
RZ Return if Zero

Condition Bits Affected: None.
*/

void instr_0xc8(i8080oObject *self) {
    #ifdef DEBUG
    printf("RZ\n");
    #endif
    if (1 == self->CC.z) {
        ret(self);
    } else {
        self->PC += 1;
    }
}

/*
RNZ Return if Not Zero

Condition Bits Affected: None.
*/

void instr_0xc0(i8080oObject *self) {
    #ifdef DEBUG
    printf("RNZ\n");
    #endif
    if (0 == self->CC.z) {
        ret(self);
    } else {
        self->PC += 1;
    }
}

/*
RM Return if Minus

Condition Bits Affected: None.
*/

void instr_0xf8(i8080oObject *self) {
    #ifdef DEBUG
    printf("RM\n");
    #endif
    if (1 == self->CC.s) {
        ret(self);
    } else {
        self->PC += 1;
    }
}

/*
RP Return if Plus

Condition Bits Affected: None.
*/

void instr_0xf0(i8080oObject *self) {
    #ifdef DEBUG
    printf("RP\n");
    #endif
    if (0 == self->CC.s) {
        ret(self);
    } else {
        self->PC += 1;
    }
}

/*
RPE Return if Parity Even

Condition Bits Affected: None.
*/

void instr_0xe8(i8080oObject *self) {
    #ifdef DEBUG
    printf("RPE\n");
    #endif
    if (1 == self->CC.p) {
        ret(self);
    } else {
        self->PC += 1;
    }
}

/*
RPO Return if Parity Odd

Condition Bits Affected: None.
*/

void instr_0xe0(i8080oObject *self) {
    #ifdef DEBUG
    printf("RPO\n");
    #endif
    if (0 == self->CC.p) {
        ret(self);
    } else {
        self->PC += 1;
    }
}

// END RET

/*
RST Restart Instructions
*/

void rst(i8080oObject *self, uint8_t addr) {
    self->memory[self->SP-1] = (self->PC >> 8) & 0xff;
    self->memory[self->SP-2] = self->PC & 0xff;
    self->SP = self->SP - 2;
    self->PC = addr;
}

/*
RST 0

Condition Bits Affected: None.
*/

void instr_0xc7(i8080oObject *self) {
    #ifdef DEBUG
    printf("RST 0\n");
    #endif
    rst(self, 0x00);
}

/*
RST 1

Condition Bits Affected: None.
*/

void instr_0xcf(i8080oObject *self) {
    #ifdef DEBUG
    printf("RST 1\n");
    #endif
    rst(self, 0x08);
}

/*
RST 2

Condition Bits Affected: None.
*/

void instr_0xd7(i8080oObject *self) {
    #ifdef DEBUG
    printf("RST 2\n");
    #endif
    rst(self, 0x10); // not sure
}

/*
RST 3

Condition Bits Affected: None.
*/


void instr_0xdf(i8080oObject *self) {
    #ifdef DEBUG
    printf("RST 3\n");
    #endif
    rst(self, 18);
}

/*
RST 4

Condition Bits Affected: None.
*/

void instr_0xe7(i8080oObject *self) {
    #ifdef DEBUG
    printf("RST 4\n");
    #endif
    rst(self, 0x20);
}

/*
RST 5

Condition Bits Affected: None.
*/

void instr_0xef(i8080oObject *self) {
    #ifdef DEBUG
    printf("RST 5\n");
    #endif
    rst(self, 0x28);
}

/*
RST 6

Condition Bits Affected: None.
*/

void instr_0xf7(i8080oObject *self) {
    #ifdef DEBUG
    printf("RST 6\n");
    #endif
    rst(self, 0x30);
}

/*
RST 7

Condition Bits Affected: None.
*/

void instr_0xff(i8080oObject *self) {
    #ifdef DEBUG
    printf("RST 7\n");
    #endif
    rst(self, 0x38);
}

/*
Interrupt flip-flop instructions
*/

/*
EI Enable Interrupts

Condition Bits Affected: None.
*/

void instr_0xfb(i8080oObject *self) {
    #ifdef DEBUG
    printf("EI\n");
    #endif
    self->CC.int_enable = 1;
    self->PC += 1;
}

/*
DI Disable Interrupts

Condition Bits Affected: None.
*/

void instr_0xf3(i8080oObject *self) {
    #ifdef DEBUG
    printf("DI\n");
    #endif
    self->CC.int_enable = 0;
    self->PC += 1;
}

/*
Input/Output Instructions
*/

// dummy function for now
uint8_t in(uint8_t port){
    return 0;
}

/*
IN Input

Condition Bits Affected: None.
*/

void instr_0xdb(i8080oObject *self) {
    #ifdef DEBUG
    printf("IN\n");
    #endif
    uint8_t port = self->memory[self->PC+1];
    self->A = in(port);
    self->PC += 2;
}

// dummy function for now
void out(uint8_t port, uint8_t data){
    return;
}

/*
OUT Output

Condition Bits Affected: None.
*/

void instr_0xd3(i8080oObject *self) {
    #ifdef DEBUG
    printf("OUT\n");
    #endif
    uint8_t port = self->memory[self->PC+1];
    out(port, self->A);
    self->PC += 2;
}

/*
HLT Halt

Condition Bits Affected: None.
*/

void instr_0x76(i8080oObject *self) {
    #ifdef DEBUG
    printf("HLT\n");
    #endif
    self->CC.halt = 1;
}


/*
Invalid Instruction
*/

void instr_0x08(i8080oObject *self) {
    #ifdef DEBUG
    printf("Invalid Instruction\n");
    #endif
}

void instr_0x10(i8080oObject *self) {
    #ifdef DEBUG
    printf("Invalid Instruction\n");
    #endif
}

void instr_0x18(i8080oObject *self) {
    #ifdef DEBUG
    printf("Invalid Instruction\n");
    #endif
}

void instr_0x20(i8080oObject *self) {
    #ifdef DEBUG
    printf("Invalid Instruction\n");
    #endif
}

void instr_0x28(i8080oObject *self) {
    #ifdef DEBUG
    printf("Invalid Instruction\n");
    #endif
}

void instr_0x30(i8080oObject *self) {
    #ifdef DEBUG
    printf("Invalid Instruction\n");
    #endif
}

void instr_0x38(i8080oObject *self) {
    #ifdef DEBUG
    printf("Invalid Instruction\n");
    #endif
}

void instr_0xd9(i8080oObject *self) {
    #ifdef DEBUG
    printf("Invalid Instruction\n");
    #endif
}

void instr_0xdd(i8080oObject *self) {
    #ifdef DEBUG
    printf("Invalid Instruction\n");
    #endif
}

void instr_0xed(i8080oObject *self) {
    #ifdef DEBUG
    printf("Invalid Instruction\n");
    #endif
}

void instr_0xcb(i8080oObject *self) {
    #ifdef DEBUG
    printf("Invalid Instruction\n");
    #endif
}

void instr_0xfd(i8080oObject *self) {
    #ifdef DEBUG
    printf("Invalid Instruction\n");
    #endif
}

void (*CPU_FUNCTIONS[256]) (i8080oObject *self) = {
    instr_0x00, instr_0x01, instr_0x02, instr_0x03, instr_0x04, instr_0x05, instr_0x06, instr_0x07,
    instr_0x08, instr_0x09, instr_0x0a, instr_0x0b, instr_0x0c, instr_0x0d, instr_0x0e, instr_0x0f,
    instr_0x10, instr_0x11, instr_0x12, instr_0x13, instr_0x14, instr_0x15, instr_0x16, instr_0x17,
    instr_0x18, instr_0x19, instr_0x1a, instr_0x1b, instr_0x1c, instr_0x1d, instr_0x1e, instr_0x1f,
    instr_0x20, instr_0x21, instr_0x22, instr_0x23, instr_0x24, instr_0x25, instr_0x26, instr_0x27,
    instr_0x28, instr_0x29, instr_0x2a, instr_0x2b, instr_0x2c, instr_0x2d, instr_0x2e, instr_0x2f,
    instr_0x30, instr_0x31, instr_0x32, instr_0x33, instr_0x34, instr_0x35, instr_0x36, instr_0x37,
    instr_0x38, instr_0x39, instr_0x3a, instr_0x3b, instr_0x3c, instr_0x3d, instr_0x3e, instr_0x3f,
    instr_0x40, instr_0x41, instr_0x42, instr_0x43, instr_0x44, instr_0x45, instr_0x46, instr_0x47,
    instr_0x48, instr_0x49, instr_0x4a, instr_0x4b, instr_0x4c, instr_0x4d, instr_0x4e, instr_0x4f,
    instr_0x50, instr_0x51, instr_0x52, instr_0x53, instr_0x54, instr_0x55, instr_0x56, instr_0x57,
    instr_0x58, instr_0x59, instr_0x5a, instr_0x5b, instr_0x5c, instr_0x5d, instr_0x5e, instr_0x5f,
    instr_0x60, instr_0x61, instr_0x62, instr_0x63, instr_0x64, instr_0x65, instr_0x66, instr_0x67,
    instr_0x68, instr_0x69, instr_0x6a, instr_0x6b, instr_0x6c, instr_0x6d, instr_0x6e, instr_0x6f,
    instr_0x70, instr_0x71, instr_0x72, instr_0x73, instr_0x74, instr_0x75, instr_0x76, instr_0x77,
    instr_0x78, instr_0x79, instr_0x7a, instr_0x7b, instr_0x7c, instr_0x7d, instr_0x7e, instr_0x7f,
    instr_0x80, instr_0x81, instr_0x82, instr_0x83, instr_0x84, instr_0x85, instr_0x86, instr_0x87,
    instr_0x88, instr_0x89, instr_0x8a, instr_0x8b, instr_0x8c, instr_0x8d, instr_0x8e, instr_0x8f,
    instr_0x90, instr_0x91, instr_0x92, instr_0x93, instr_0x94, instr_0x95, instr_0x96, instr_0x97,
    instr_0x98, instr_0x99, instr_0x9a, instr_0x9b, instr_0x9c, instr_0x9d, instr_0x9e, instr_0x9f,
    instr_0xa0, instr_0xa1, instr_0xa2, instr_0xa3, instr_0xa4, instr_0xa5, instr_0xa6, instr_0xa7,
    instr_0xa8, instr_0xa9, instr_0xaa, instr_0xab, instr_0xac, instr_0xad, instr_0xae, instr_0xaf,
    instr_0xb0, instr_0xb1, instr_0xb2, instr_0xb3, instr_0xb4, instr_0xb5, instr_0xb6, instr_0xb7,
    instr_0xb8, instr_0xb9, instr_0xba, instr_0xbb, instr_0xbc, instr_0xbd, instr_0xbe, instr_0xbf,
    instr_0xc0, instr_0xc1, instr_0xc2, instr_0xc3, instr_0xc4, instr_0xc5, instr_0xc6, instr_0xc7,
    instr_0xc8, instr_0xc9, instr_0xca, instr_0xcb, instr_0xcc, instr_0xcd, instr_0xce, instr_0xcf,
    instr_0xd0, instr_0xd1, instr_0xd2, instr_0xd3, instr_0xd4, instr_0xd5, instr_0xd6, instr_0xd7,
    instr_0xd8, instr_0xd9, instr_0xda, instr_0xdb, instr_0xdc, instr_0xdd, instr_0xde, instr_0xdf,
    instr_0xe0, instr_0xe1, instr_0xe2, instr_0xe3, instr_0xe4, instr_0xe5, instr_0xe6, instr_0xe7,
    instr_0xe8, instr_0xe9, instr_0xea, instr_0xeb, instr_0xec, instr_0xed, instr_0xee, instr_0xef,
    instr_0xf0, instr_0xf1, instr_0xf2, instr_0xf3, instr_0xf4, instr_0xf5, instr_0xf6, instr_0xf7,
    instr_0xf8, instr_0xf9, instr_0xfa, instr_0xfb, instr_0xfc, instr_0xfd, instr_0xfe, instr_0xff
};