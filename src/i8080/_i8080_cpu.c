#include "Python.h"
#include "_i8080_cpu.h"
#include "_i8080_object.h"
#include "stdio.h"

#define DEBUG


// checks the parity of a byte
uint8_t parity_check(uint8_t byte)
{
    byte ^= byte >> 4;
    byte ^= byte >> 2;
    byte ^= byte >> 1;
    return (~byte) & 1;
}

int parity(int x, int size)
{
	int i;
	int p = 0;
	x = (x & ((1<<size)-1));
	for (i=0; i<size; i++)
	{
		if (x & 0x1) p++;
		x = x >> 1;
	}
	return (0 == (p & 0x1));
}

void LogicFlagsA(i8080oObject *self)
{
	self->CC.cy = self->CC.ac = 0;
	self->CC.z = (self->A == 0);
	self->CC.s = (0x80 == (self->A & 0x80));
	self->CC.p = parity(self->A, 8);
}

void ArithFlagsA(i8080oObject *self, uint16_t res)
{
	self->CC.cy = (res > 0xff);
	self->CC.z = ((res&0xff) == 0);
	self->CC.s = (0x80 == (res & 0x80));
	self->CC.p = parity(res&0xff, 8);
}

void update_flags_inr_dcr(i8080oObject *self, uint8_t res) {
    self->CC.z = (res == 0);
    self->CC.s = (0x80 == (res & 0x80));
    self->CC.p = parity(res, 8);
    self->CC.cy =(0x8 == (res & 0x8));
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
    printf("CMC");
    #endif
    self->CC.cy = !self->CC.cy;
}

/*
STC Set Carry
*/

void instr_0x37(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("STC");
    #endif
    self->CC.cy = 1;
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

void instr_0x04(i8080oObject *self) {
    #ifdef DEBUG
    printf("INR B");
    #endif
    self->B++;
    update_flags_inr_dcr(self, self->B);
}

void instr_0x0c(i8080oObject *self) {
    #ifdef DEBUG
    printf("INR C");
    #endif
    self->C++;
    update_flags_inr_dcr(self, self->C);
}

void instr_0x14(i8080oObject *self) {
    #ifdef DEBUG
    printf("INR D");
    #endif
    self->D++;
    update_flags_inr_dcr(self, self->D);
}

void instr_0x1c(i8080oObject *self) {
    #ifdef DEBUG
    printf("INR E");
    #endif
    self->E++;
    update_flags_inr_dcr(self, self->E);
}

void instr_0x24(i8080oObject *self) {
    #ifdef DEBUG
    printf("INR H");
    #endif
    self->H++;
    update_flags_inr_dcr(self, self->H);
}

void instr_0x2c(i8080oObject *self) {
    #ifdef DEBUG
    printf("INR L");
    #endif
    self->L++;
    update_flags_inr_dcr(self, self->L);
}

void instr_0x34(i8080oObject *self) {
    #ifdef DEBUG
    printf("INR M");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    uint8_t res = self->memory[addr] + 1;
    self->memory[addr] = res;
    update_flags_inr_dcr(self, res);
}

void instr_0x3c(i8080oObject *self) {
    #ifdef DEBUG
    printf("INR A");
    #endif
    self->A++;
    update_flags_inr_dcr(self, self->A);
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

void instr_0x05(i8080oObject *self) {
    #ifdef DEBUG
    printf("DCR B");
    #endif
    self->B--;
    update_flags_inr_dcr(self, self->B);
}

void instr_0x0d(i8080oObject *self) {
    #ifdef DEBUG
    printf("DCR C");
    #endif
    self->C--;
    update_flags_inr_dcr(self, self->C);
}

void instr_0x15(i8080oObject *self) {
    #ifdef DEBUG
    printf("DCR D");
    #endif
    self->D--;
    update_flags_inr_dcr(self, self->D);
}

void instr_0x1d(i8080oObject *self) {
    #ifdef DEBUG
    printf("DCR E");
    #endif
    self->E--;
    update_flags_inr_dcr(self, self->E);
}

void instr_0x25(i8080oObject *self) {
    #ifdef DEBUG
    printf("DCR H");
    #endif
    self->H--;
    update_flags_inr_dcr(self, self->H);
}

void instr_0x2d(i8080oObject *self) {
    #ifdef DEBUG
    printf("DCR L");
    #endif
    uint8_t res = self->L - 1;
    self->CC.z = (res == 0);
    self->CC.s = (0x80 == (res & 0x80));
    self->CC.p = parity(res, 8);
    self->L = res;
}

void instr_0x35(i8080oObject *self) {
    #ifdef DEBUG
    printf("DCR M");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    uint8_t res = self->memory[addr] - 1;
    self->memory[addr] = res;
    update_flags_inr_dcr(self, res);
}

void instr_0x3d(i8080oObject *self) {
    #ifdef DEBUG
    printf("DCR A");
    #endif
    self->A--;
    update_flags_inr_dcr(self, self->A);
}

// END DCR

/*
CMA Complement Accumulator
*/

void instr_0x2f(i8080oObject *self) {
    #ifdef DEBUG
    printf("CMA");
    #endif
    self->A = ~self->A;
}

/*
DAA Decimal Adjust Accumulator
*/

void instr_0x27(i8080oObject *self) {
    #ifdef DEBUG
    printf("DAA");
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
}

/*
NOP Instruction
*/
void instr_0x00(i8080oObject *self) {
    #ifdef DEBUG
    printf("NOP");
    #endif
}

/*
*** Data Transfer Instructions ***
*/

/*
MOV Move Register or Memory to Register
*/

void instr_0x40(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV B,B");
    #endif
}

void instr_0x41(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV B,C");
    #endif
    self->B = self->C;
}

void instr_0x42(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV B,D");
    #endif
    self->B = self->D;
}

void instr_0x43(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV B,E");
    #endif
    self->B = self->E;
}

void instr_0x44(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV B,H");
    #endif
    self->B = self->H;
}

void instr_0x45(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV B,L");
    #endif
    self->B = self->L;
}

void instr_0x46(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV B,M");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    self->B = self->memory[addr];
}

void instr_0x47(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV B,A");
    #endif
    self->B = self->A;
}

void instr_0x48(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV C,B");
    #endif
    self->C = self->B;
}

void instr_0x49(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV C,C");
    #endif
}

void instr_0x4a(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV C,D");
    #endif
    self->C = self->D;
}

void instr_0x4b(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV C,E");
    #endif
    self->C = self->E;
}

void instr_0x4c(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV C,H");
    #endif
    self->C = self->H;
}

void instr_0x4d(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV C,L");
    #endif
    self->C = self->L;
}

void instr_0x4e(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV C,M");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    self->C = self->memory[addr];
}

void instr_0x4f(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV C,A");
    #endif
    self->C = self->A;
}

void instr_0x50(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV D,B");
    #endif
    self->D = self->B;
}

void instr_0x51(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV D,C");
    #endif
    self->D = self->C;
}

void instr_0x52(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV D,D");
    #endif
}

void instr_0x53(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV D,E");
    #endif
    self->D = self->E;
}

void instr_0x54(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV D,H");
    #endif
    self->D = self->H;
}

void instr_0x55(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV D,L");
    #endif
    self->D = self->L;
}

void instr_0x56(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV D,M");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    self->D = self->memory[addr];
}

void instr_0x57(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV D,A");
    #endif
    self->D = self->A;
}

void instr_0x58(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV E,B");
    #endif
    self->E = self->B;
}

void instr_0x59(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV E,C");
    #endif
    self->E = self->C;
}

void instr_0x5a(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV E,D");
    #endif
    self->E = self->D;
}

void instr_0x5b(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV E,E");
    #endif
}

void instr_0x5c(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV E,H");
    #endif
    self->E = self->H;
}

void instr_0x5d(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV E,L");
    #endif
    self->E = self->L;
}

void instr_0x5e(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV E,M");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    self->E = self->memory[addr];
}

void instr_0x5f(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV E,A");
    #endif
    self->E = self->A;
}

void instr_0x60(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV H,B");
    #endif
    self->H = self->B;
}

void instr_0x61(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV H,C");
    #endif
    self->H = self->C;
}

void instr_0x62(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV H,D");
    #endif
    self->H = self->D;
}

void instr_0x63(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV H,E");
    #endif
    self->H = self->E;
}

void instr_0x64(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV H,H");
    #endif
}

void instr_0x65(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV H,L");
    #endif
    self->H = self->L;
}

void instr_0x66(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV H,M");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    self->H = self->memory[addr];
}

void instr_0x67(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV H,A");
    #endif
    self->H = self->A;
}

void instr_0x68(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV L,B");
    #endif
    self->L = self->B;
}

void instr_0x69(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV L,C");
    #endif
    self->L = self->C;
}

void instr_0x6a(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV L,D");
    #endif
    self->L = self->D;
}

void instr_0x6b(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV L,E");
    #endif
    self->L = self->E;
}

void instr_0x6c(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV L,H");
    #endif
    self->L = self->H;
}

void instr_0x6d(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV L,L");
    #endif
}

void instr_0x6e(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV L,M");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    self->L = self->memory[addr];
}

void instr_0x6f(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV L,A");
    #endif
    self->L = self->A;
}

void instr_0x70(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV M,B");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    self->memory[addr] = self->B;
}

void instr_0x71(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV M,C");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    self->memory[addr] = self->C;
}

void instr_0x72(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV M,D");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    self->memory[addr] = self->D;
}

void instr_0x73(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV M,E");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    self->memory[addr] = self->E;
}

void instr_0x74(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV M,H");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    self->memory[addr] = self->H;
}

void instr_0x75(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV M,L");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    self->memory[addr] = self->L;
}

void instr_0x77(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV M,A");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    self->memory[addr] = self->A;
}

void instr_0x78(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV A,B");
    #endif
    self->A = self->B;
}

void instr_0x79(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV A,C");
    #endif
    self->A = self->C;
}

void instr_0x7a(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV A,D");
    #endif
    self->A = self->D;
}

void instr_0x7b(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV A,E");
    #endif
    self->A = self->E;
}

void instr_0x7c(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV A,H");
    #endif
    self->A = self->H;
}

void instr_0x7d(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV A,L");
    #endif
    self->A = self->L;
}

void instr_0x7e(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV A,M");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    self->A = self->memory[addr];
}

void instr_0x7f(i8080oObject *self) {
    #ifdef DEBUG
    printf("MOV A,A");
    #endif
}

void instr_0x02(i8080oObject *self) {
    #ifdef DEBUG
    printf("STAX B");
    #endif
    uint16_t addr = (self->B << 8) | self->C;
    self->memory[addr] = self->A;
}

void instr_0x12(i8080oObject *self) {
    #ifdef DEBUG
    printf("STAX D");
    #endif
    uint16_t addr = (self->D << 8) | self->E;
    self->memory[addr] = self->A;
}

void instr_0x0a(i8080oObject *self) {
    #ifdef DEBUG
    printf("LDAX B");
    #endif
    uint16_t addr = (self->B << 8) | self->C;
    self->A = self->memory[addr];
}

void instr_0x1a(i8080oObject *self) {
    #ifdef DEBUG
    printf("LDAX D");
    #endif
    uint16_t addr = (self->D << 8) | self->E;
    self->A = self->memory[addr];
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

void update_flags_add(i8080oObject *self, uint16_t result){
    self->CC.cy = (result & 0x100) >> 8;
    self->CC.z = (result == 0);
    self->CC.s = (result & 0x80) >> 7;
    self->CC.p = parity_check((uint8_t)(result & 0xff));
    self->CC.ac = (result & 0x10) >> 4;
}

void instr_0x80(i8080oObject *self) {
    #ifdef DEBUG
    printf("ADD B");
    #endif
    uint16_t result = self->A + self->B;
    self->A = result & 0xff;
    update_flags_add(self, result);
}

void instr_0x81(i8080oObject *self) {
    #ifdef DEBUG
    printf("ADD C");
    #endif
    uint16_t result = self->A + self->C;
    self->A = result & 0xff;
    update_flags_add(self, result);
}

void instr_0x82(i8080oObject *self) {
    #ifdef DEBUG
    printf("ADD D");
    #endif
    uint16_t result = self->A + self->D;
    self->A = result & 0xff;
    update_flags_add(self, result);
}

void instr_0x83(i8080oObject *self) {
    #ifdef DEBUG
    printf("ADD E");
    #endif
    uint16_t result = self->A + self->E;
    self->A = result & 0xff;
    update_flags_add(self, result);
}

void instr_0x84(i8080oObject *self) {
    #ifdef DEBUG
    printf("ADD H");
    #endif
    uint16_t result = self->A + self->H;
    self->A = result & 0xff;
    update_flags_add(self, result);
}

void instr_0x85(i8080oObject *self) {
    #ifdef DEBUG
    printf("ADD L");
    #endif
    uint16_t result = self->A + self->L;
    self->A = result & 0xff;
    update_flags_add(self, result);
}

void instr_0x86(i8080oObject *self) {
    #ifdef DEBUG
    printf("ADD M");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    uint16_t result = self->A + self->memory[addr];
    self->A = result & 0xff;
    update_flags_add(self, result);
}

void instr_0x87(i8080oObject *self) {
    #ifdef DEBUG
    printf("ADD A");
    #endif
    uint16_t result = self->A + self->A;
    self->A = result & 0xff;
    update_flags_add(self, result);
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
    printf("ADC B");
    #endif
    uint16_t result = self->A + self->B + self->CC.cy;
    self->A = result & 0xff;
    update_flags_add(self, result);
}

void instr_0x89(i8080oObject *self) {
    #ifdef DEBUG
    printf("ADC C");
    #endif
    uint16_t result = self->A + self->C + self->CC.cy;
    self->A = result & 0xff;
    update_flags_add(self, result);
}

void instr_0x8a(i8080oObject *self) {
    #ifdef DEBUG
    printf("ADC D");
    #endif
    uint16_t result = self->A + self->D + self->CC.cy;
    self->A = result & 0xff;
    update_flags_add(self, result);
}

void instr_0x8b(i8080oObject *self) {
    #ifdef DEBUG
    printf("ADC E");
    #endif
    uint16_t result = self->A + self->E + self->CC.cy;
    self->A = result & 0xff;
    update_flags_add(self, result);
}

void instr_0x8c(i8080oObject *self) {
    #ifdef DEBUG
    printf("ADC H");
    #endif
    uint16_t result = self->A + self->H + self->CC.cy;
    self->A = result & 0xff;
    update_flags_add(self, result);
}

void instr_0x8d(i8080oObject *self) {
    #ifdef DEBUG
    printf("ADC L");
    #endif
    uint16_t result = self->A + self->L + self->CC.cy;
    self->A = result & 0xff;
    update_flags_add(self, result);
}

void instr_0x8e(i8080oObject *self) {
    #ifdef DEBUG
    printf("ADC M");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    uint16_t result = self->A + self->memory[addr] + self->CC.cy;
    self->A = result & 0xff;
    update_flags_add(self, result);
}

void instr_0x8f(i8080oObject *self) {
    #ifdef DEBUG
    printf("ADC A");
    #endif
    uint16_t result = self->A + self->A + self->CC.cy;
    self->A = result & 0xff;
    update_flags_add(self, result);
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

void update_flags_sub(i8080oObject self, uint16_t result){
    self->CC.cy = !(result > 0xff); // reset carry if result is greater than 0xff
    self->CC.z = ((result & 0xff) == 0);
    self->CC.s = ((result & 0x80) != 0);
    self->CC.p = parity_check(result & 0xff);
    self->CC.ac = (result & 0x10) >> 4;
}

void instr_0x90(i8080oObject *self) {
    #ifdef DEBUG
    printf("SUB B");
    #endif
    uint16_t result = self->A + (~self->B) + 1; // add the 2's complement of B and 1
    self->A = result & 0xff;
    update_flags_sub(self, result);
}

void instr_0x91(i8080oObject *self) {
    #ifdef DEBUG
    printf("SUB C");
    #endif
    uint16_t result = self->A + (~self->C) + 1; // add the 2's complement of C and 1
    self->A = result & 0xff;
    update_flags_sub(self, result);
}

void instr_0x92(i8080oObject *self) {
    #ifdef DEBUG
    printf("SUB D");
    #endif
    uint16_t result = self->A + (~self->D) + 1; // add the 2's complement of D and 1
    self->A = result & 0xff;
    update_flags_sub(self, result);
}

void instr_0x93(i8080oObject *self) {
    #ifdef DEBUG
    printf("SUB E");
    #endif
    uint16_t result = self->A + (~self->E) + 1; // add the 2's complement of E and 1
    self->A = result & 0xff;
    update_flags_sub(self, result);
}

void instr_0x94(i8080oObject *self) {
    #ifdef DEBUG
    printf("SUB H");
    #endif
    uint16_t result = self->A + (~self->H) + 1; // add the 2's complement of H and 1
    self->A = result & 0xff;
    update_flags_sub(self, result);
}

void instr_0x95(i8080oObject *self) {
    #ifdef DEBUG
    printf("SUB L");
    #endif
    uint16_t result = self->A + (~self->L) + 1; // add the 2's complement of L and 1
    self->A = result & 0xff;
    update_flags_sub(self, result);
}

void instr_0x96(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("SUB M");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    uint16_t result = self->A + (~self->memory[addr]) + 1; // add the 2's complement of M and 1
    self->A = result & 0xff;
    update_flags_sub(self, result);
}

void instr_0x97(i8080oObject *self) {
    #ifdef DEBUG
    printf("SUB A");
    #endif
    uint16_t result = self->A + (~self->A) + 1; // add the 2's complement of A and 1
    self->A = result & 0xff;
    update_flags_sub(self, result);
}

// END SUB



void instr_0x01(i8080oObject *self) {
    #ifdef DEBUG
    printf("LXI B");
    #endif
    self->C = self->memory[self->PC + 1];
    self->B = self->memory[self->PC + 2];
    self->PC+=2;
}



void instr_0x03(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("INX B");
    #endif
    self->C++;
    if (self->C == 0) {
        self->B++;
    }
}

void instr_0x06(i8080oObject *self) {
    #ifdef DEBUG
    printf("MVI B");
    #endif
    self->B = self->memory[self->PC + 1];
    self->PC+=1;
}

void instr_0x07(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("RLC");
    #endif
    uint8_t res = self->A << 1;
    self->CC.cy = (0x80 == (self->A & 0x80));
    self->A = res;
}

void instr_0x08(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("NOP");
    #endif
}

void instr_0x09(i8080oObject * self){
    uint32_t hl = (self->H << 8) | self->L;
    uint32_t bc = (self->B << 8) | self->C;
    uint32_t res = hl + bc;
    self->H = (res & 0xff00) >> 8;
    self->L = res & 0xff;
    self->CC.cy = ((res & 0xffff0000) > 0);
}



void instr_0x0b(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("DCX B");
    #endif
    self->C--;
    if (self->C == 0xff) {
        self->B--;
    }
}





void instr_0x0e(i8080oObject *self) {
    #ifdef DEBUG
    printf("MVI C");
    #endif
    self->C = self->memory[self->PC + 1];
    self->PC+=1;
}

void instr_0x0f(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("RRC");
    #endif
    uint8_t x = self->A;
    self->A = ((x & 1) << 7) | (x >> 1);
    self->CC.cy = (1 == (x&1));
}

void instr_0x10(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("NOP");
    #endif
}

void instr_0x11(i8080oObject *self) {
    #ifdef DEBUG
    printf("LXI D");
    #endif
    self->E = self->memory[self->PC + 1];
    self->D = self->memory[self->PC + 2];
    self->PC+=2;
}



void instr_0x13(i8080oObject *self) {
    #ifdef DEBUG
    printf("INX D");
    #endif
    self->E++;
    if (self->E == 0) {
        self->D++;
    }
}





void instr_0x16(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("MVI D");
    #endif
    self->D = self->memory[self->PC + 1];
    self->PC+=1;
}

void instr_0x17(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("RAL");
    #endif
    uint8_t res = self->A << 1;
    res |= self->CC.cy;
    self->CC.cy = (0x80 == (self->A & 0x80));
    self->A = res;
}

void instr_0x18(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("NOP");
    #endif
}

void instr_0x19(i8080oObject * self){
    #ifdef DEBUG
    printf("DAD D");
    #endif
    uint32_t hl = (self->H << 8) | self->L;
    uint32_t de = (self->D << 8) | self->E;
    uint32_t res = hl + de;
    self->H = (res & 0xff00) >> 8;
    self->L = res & 0xff;
    //self->CC.cy = ((res & 0xffff0000) > 0);
    self->CC.cy = ((res & 0xffff0000) != 0);
}



void instr_0x1b(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("DCX D");
    #endif
    self->E--;
    if (self->E == 0xff) {
        self->D--;
    }
}




void instr_0x1e(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("MVI E");
    #endif
    self->E = self->memory[self->PC + 1];
    self->PC+=1;
}

void instr_0x1f(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("RAR");
    #endif
    uint8_t res = self->A >> 1;
    res |= (self->CC.cy << 7);
    self->CC.cy = (0x01 == (self->A & 0x01));
    self->A = res;
}

void instr_0x20(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("NOP");
    #endif
}

void instr_0x21(i8080oObject *self) {
    #ifdef DEBUG
    printf("LXI H");
    #endif
    self->L = self->memory[self->PC + 1];
    self->H = self->memory[self->PC + 2];
    self->PC+=2;
}

void instr_0x22(i8080oObject *self) {
    #ifdef DEBUG
    printf("SHLD");
    #endif
    uint16_t addr = self->memory[self->PC + 2] * 256 + self->memory[self->PC + 1];
    self->memory[addr] = self->L;
    self->memory[addr + 1] = self->H;
    self->PC+=2;
}

void instr_0x23(i8080oObject *self) {
    #ifdef DEBUG
    printf("INX H");
    #endif
    self->L++;
    if (self->L == 0) {
        self->H++;
    }
}





void instr_0x26(i8080oObject *self) {
    #ifdef DEBUG
    printf("MVI H");
    #endif
    self->H = self->memory[self->PC + 1];
    self->PC+=1;
}



void instr_0x28(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("NOP");
    #endif
}

void instr_0x29(i8080oObject * self){
    #ifdef DEBUG
    printf("DAD H");
    #endif
    uint32_t hl = (self->H << 8) | self->L;
    uint32_t res = hl + hl;
    self->H = (res & 0xff00) >> 8;
    self->L = res & 0xff;
    self->CC.cy = ((res & 0xffff0000) != 0);
}

void instr_0x2a(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("LHLD");
    #endif
    uint16_t addr = self->memory[self->PC + 2] * 256 + self->memory[self->PC + 1];
    self->L = self->memory[addr];
    self->H = self->memory[addr + 1];
    self->PC+=2;
}

void instr_0x2b(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("DCX H");
    #endif
    self->L--;
    if (self->L == 0xff) {
        self->H--;
    }
}





void instr_0x2e(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("MVI L");
    #endif
    self->L = self->memory[self->PC + 1];
    self->PC+=1;
}



void instr_0x30(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("NOP");
    #endif
}

void instr_0x31(i8080oObject *self) {
    #ifdef DEBUG
    printf("LXI SP");
    #endif
    self->SP = self->memory[self->PC + 2] * 256 + self->memory[self->PC + 1];
    self->PC+=2;
}

void instr_0x32(i8080oObject *self) {
    #ifdef DEBUG
    printf("STA");
    #endif
    uint16_t addr = self->memory[self->PC + 2] * 256 + self->memory[self->PC + 1];
    self->memory[addr] = self->A;
    self->PC+=2;
}

void instr_0x33(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("INX SP");
    #endif
    self->SP++;
}





void instr_0x36(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("MVI M");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    self->memory[addr] = self->memory[self->PC + 1];
    self->PC+=1;
}


void instr_0x38(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("NOP");
    #endif
}

void instr_0x39(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("DAD SP");
    #endif
    uint32_t sp = self->SP;
    uint32_t hl = (self->H << 8) | self->L;
    uint32_t res = sp + hl;
    self->H = (res & 0xff00) >> 8;
    self->L = res & 0xff;
    self->CC.cy = ((res & 0xffff0000) != 0);
}

void instr_0x3a(i8080oObject *self) {
    #ifdef DEBUG
    printf("LDA");
    #endif
    uint16_t addr = self->memory[self->PC + 2] * 256 + self->memory[self->PC + 1];
    self->A = self->memory[addr];
    self->PC+=2;
}

void instr_0x3b(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("DCX SP");
    #endif
    self->SP--;
}





void instr_0x3e(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("MVI A");
    #endif
    self->A = self->memory[self->PC + 1];
    self->PC+=1;
}



void instr_0x76(i8080oObject *self) {
    #ifdef DEBUG
    printf("HLT");
    #endif
    self->CC.halt = 1;
}



void instr_0x98(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("SBB B");
    #endif
    uint16_t result = self->A - self->B - self->CC.cy;
    self->A = result & 0xff;
    ArithFlagsA(self, result);
}

void instr_0x99(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("SBB C");
    #endif
    uint16_t result = self->A - self->C - self->CC.cy;
    self->A = result & 0xff;
    ArithFlagsA(self, result);
}

void instr_0x9a(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("SBB D");
    #endif
    uint16_t result = self->A - self->D - self->CC.cy;
    self->A = result & 0xff;
    ArithFlagsA(self, result);
}

void instr_0x9b(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("SBB E");
    #endif
    uint16_t result = self->A - self->E - self->CC.cy;
    self->A = result & 0xff;
    ArithFlagsA(self, result);
}

void instr_0x9c(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("SBB H");
    #endif
    uint16_t result = self->A - self->H - self->CC.cy;
    self->A = result & 0xff;
    ArithFlagsA(self, result);
}

void instr_0x9d(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("SBB L");
    #endif
    uint16_t result = self->A - self->L - self->CC.cy;
    self->A = result & 0xff;
    ArithFlagsA(self, result);
}

void instr_0x9e(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("SBB M");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    uint16_t result = self->A - self->memory[addr] - self->CC.cy;
    self->A = result & 0xff;
    ArithFlagsA(self, result);
}

void instr_0x9f(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("SBB A");
    #endif
    uint16_t result = self->A - self->A - self->CC.cy;
    self->A = result & 0xff;
    ArithFlagsA(self, result);
}

void instr_0xa0(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("ANA B");
    #endif
    uint16_t result = self->A & self->B;
    self->A = result & 0xff;
    LogicFlagsA(self);
}

void instr_0xa1(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("ANA C");
    #endif
    uint16_t result = self->A & self->C;
    self->A = result & 0xff;
    LogicFlagsA(self);
}

void instr_0xa2(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("ANA D");
    #endif
    uint16_t result = self->A & self->D;
    self->A = result & 0xff;
    LogicFlagsA(self);
}

void instr_0xa3(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("ANA E");
    #endif
    uint16_t result = self->A & self->E;
    self->A = result & 0xff;
    LogicFlagsA(self);
}

void instr_0xa4(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("ANA H");
    #endif
    uint16_t result = self->A & self->H;
    self->A = result & 0xff;
    LogicFlagsA(self);
}

void instr_0xa5(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("ANA L");
    #endif
    uint16_t result = self->A & self->L;
    self->A = result & 0xff;
    LogicFlagsA(self);
}

void instr_0xa6(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("ANA M");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    uint16_t result = self->A & self->memory[addr];
    self->A = result & 0xff;
    LogicFlagsA(self);
}

void instr_0xa7(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("ANA A");
    #endif
    self->A = self->A & self->A;
    LogicFlagsA(self);
}

void instr_0xa8(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("XRA B");
    #endif
    self->A = self->A ^ self->B;
    LogicFlagsA(self);
}

void instr_0xa9(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("XRA C");
    #endif
    self->A = self->A ^ self->C;
    LogicFlagsA(self);
}

void instr_0xaa(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("XRA D");
    #endif
    self->A = self->A ^ self->D;
    LogicFlagsA(self);
}

void instr_0xab(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("XRA E");
    #endif
    self->A = self->A ^ self->E;
    LogicFlagsA(self);
}

void instr_0xac(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("XRA H");
    #endif
    self->A = self->A ^ self->H;
    LogicFlagsA(self);
}

void instr_0xad(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("XRA L");
    #endif
    self->A = self->A ^ self->L;
    LogicFlagsA(self);
}

void instr_0xae(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("XRA M");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    self->A = self->A ^ self->memory[addr];
    LogicFlagsA(self);
}

void instr_0xaf(i8080oObject *self) {
    #ifdef DEBUG
    printf("XRA A");
    #endif
    self->A = self->A ^ self->A;
    LogicFlagsA(self);
}

void instr_0xb0(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("ORA B");
    #endif
    self->A = self->A | self->B;
    LogicFlagsA(self);
}

void instr_0xb1(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("ORA C");
    #endif
    self->A = self->A | self->C;
    LogicFlagsA(self);
}

void instr_0xb2(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("ORA D");
    #endif
    self->A = self->A | self->D;
    LogicFlagsA(self);
}

void instr_0xb3(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("ORA E");
    #endif
    self->A = self->A | self->E;
    LogicFlagsA(self);
}

void instr_0xb4(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("ORA H");
    #endif
    self->A = self->A | self->H;
    LogicFlagsA(self);
}

void instr_0xb5(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("ORA L");
    #endif
    self->A = self->A | self->L;
    LogicFlagsA(self);
}

void instr_0xb6(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("ORA M");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    self->A = self->A | self->memory[addr];
    LogicFlagsA(self);
}

void instr_0xb7(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("ORA A");
    #endif
    self->A = self->A | self->A;
    LogicFlagsA(self);
}

void instr_0xb8(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("CMP B");
    #endif
    uint16_t result = self->A - self->B;
    LogicFlagsA(self);
}

void instr_0xb9(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("CMP C");
    #endif
    uint16_t result = self->A - self->C;
    LogicFlagsA(self);
}

void instr_0xba(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("CMP D");
    #endif
    uint16_t result = self->A - self->D;
    LogicFlagsA(self);
}

void instr_0xbb(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("CMP E");
    #endif
    uint16_t result = self->A - self->E;
    LogicFlagsA(self);
}

void instr_0xbc(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("CMP H");
    #endif
    uint16_t result = self->A - self->H;
    LogicFlagsA(self);
}

void instr_0xbd(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("CMP L");
    #endif
    uint16_t result = self->A - self->L;
    LogicFlagsA(self);
}

void instr_0xbe(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("CMP M");
    #endif
    uint16_t addr = (self->H << 8) | self->L;
    uint16_t result = self->A - self->memory[addr];
    LogicFlagsA(self);
}

void instr_0xbf(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("CMP A");
    #endif
    uint16_t result = self->A - self->A;
    LogicFlagsA(self);
}

void instr_0xc0(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("RNZ");
    #endif
    printf("Not implemented");
    /*
    if (self->flags & FLAG_Z) {
        self->PC += 1;
    } else {
        self->PC = pop(self);
    }*/
}

void instr_0xc1(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("POP B");
    #endif
    self->C = self->memory[self->SP];
    self->B = self->memory[self->SP+1];
    self->SP += 2;
}

void instr_0xc2(i8080oObject *self) {
    #ifdef DEBUG
    printf("JNZ");
    #endif
    if (0 == self->CC.z)
        self->PC = (self->memory[self->PC+2] << 8) | self->memory[self->PC+1];
    else
        self->PC += 2;
}

void instr_0xc3(i8080oObject *self) {
    #ifdef DEBUG
    printf("JMP");
    #endif
    self->PC = (self->memory[self->PC+2] << 8) | self->memory[self->PC+1];
}

void instr_0xc4(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("CNZ");
    #endif
    if (0 == self->CC.z) {
        //push(self, self->PC+2);
        self->PC = (self->memory[self->PC+2] << 8) | self->memory[self->PC+1];
    } else {
        self->PC += 2;
    }
}

void instr_0xc5(i8080oObject *self) {
    #ifdef DEBUG
    printf("PUSH B");
    #endif
    self->memory[self->SP-1] = self->B;
    self->memory[self->SP-2] = self->C;
    self->SP -= 2;
}

void instr_0xc6(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("ADI");
    #endif
    uint16_t result = self->A + self->memory[self->PC+1];
    self->CC.z = ((result & 0xff) == 0);
    self->CC.s = (0x80 == (result & 0x80));
    self->CC.p = parity((result&0xff), 8);
    self->CC.cy = (result > 0xff);
    self->A = (uint8_t) (result & 0xff);
    self->PC++;
}

void instr_0xc7(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("RST 0");
    #endif
    printf("Not implemented");
    //push(self, self->PC);
    //self->PC = 0;
}

void instr_0xc8(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("RZ");
    #endif
    printf("Not implemented");
    /*
    if (self->flags & FLAG_Z) {
        self->PC = pop(self);
    } else {
        self->PC += 1;
    }*/
}

void instr_0xc9(i8080oObject *self) {
    #ifdef DEBUG
    printf("RET");
    #endif
    self->PC = self->memory[self->SP] | (self->memory[self->SP+1] << 8);
    self->SP += 2;    
}

void instr_0xca(i8080oObject *self) {
    #ifdef DEBUG
    printf("JZ");
    #endif
    if (1 == self->CC.z)
        self->PC = (self->memory[self->PC+2] << 8) | self->memory[self->PC+1];
    else
        self->PC += 2;
}

void instr_0xcb(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("JMP");
    #endif
    printf("Not implemented");
    //self->PC = (self->memory[self->PC+2] << 8) | self->memory[self->PC+1];
}

void instr_0xcc(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("CZ");
    #endif
    if (1 == self->CC.z) {
        //push(self, self->PC+2);
        self->PC = (self->memory[self->PC+2] << 8) | self->memory[self->PC+1];
    } else {
        self->PC += 2;
    }
}

void instr_0xcd(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("CALL");
    #endif
    uint16_t ret = self->PC+2;
    self->memory[self->SP-1] = (ret >> 8) & 0xff;
    self->memory[self->SP-2] = (ret & 0xff);
    self->SP = self->SP - 2;
    self->PC = (self->memory[self->PC+2] << 8) | self->memory[self->PC+1];
}

void instr_0xce(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("ACI");
    #endif
    uint16_t result = self->A + self->memory[self->PC+1] + self->CC.cy;
    self->CC.z = ((result & 0xff) == 0);
    self->CC.s = (0x80 == (result & 0x80));
    self->CC.p = parity((result&0xff), 8);
    self->CC.cy = (result > 0xff);
    self->A = (uint8_t) (result & 0xff);
    self->PC++;
}

void instr_0xcf(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("RST 1");
    #endif
    printf("Not implemented");
    //push(self, self->PC);
    //self->PC = 8;
}

void instr_0xd0(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("RNC");
    #endif
    printf("Not implemented");
    /*
    if (self->flags & FLAG_C) {
        self->PC += 1;
    } else {
        self->PC = pop(self);
    }*/
}

void instr_0xd1(i8080oObject *self) {
    #ifdef DEBUG
    printf("POP D");
    #endif
    self->E = self->memory[self->SP];
    self->D = self->memory[self->SP+1];
    self->SP += 2;
}

void instr_0xd2(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("JNC");
    #endif
    if (0 == self->CC.cy)
        self->PC = (self->memory[self->PC+2] << 8) | self->memory[self->PC+1];
    else
        self->PC += 2;
}

void instr_0xd3(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("OUT");
    #endif
    printf("Not implemented");
    //self->PC += 1;
}

void instr_0xd4(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("CNC");
    #endif
    if (0 == self->CC.cy) {
        //push(self, self->PC+2);
        self->PC = (self->memory[self->PC+2] << 8) | self->memory[self->PC+1];
    } else {
        self->PC += 2;
    }
}

void instr_0xd5(i8080oObject *self) {
    #ifdef DEBUG
    printf("PUSH D");
    #endif
    self->memory[self->SP-1] = self->D;
    self->memory[self->SP-2] = self->E;
    self->SP -= 2;
}

void instr_0xd6(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("SUI");
    #endif
    uint16_t result = self->A - self->memory[self->PC+1];
    self->CC.z = ((result & 0xff) == 0);
    self->CC.s = (0x80 == (result & 0x80));
    self->CC.p = parity((result&0xff), 8);
    self->CC.cy = (result > 0xff);
    self->A = (uint8_t) (result & 0xff);
    self->PC++;
}

void instr_0xd7(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("RST 2");
    #endif
    printf("Not implemented");
    //push(self, self->PC);
    //self->PC = 16;
}

void instr_0xd8(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("RC");
    #endif
    printf("Not implemented");
    /*
    if (self->flags & FLAG_C) {
        self->PC = pop(self);
    } else {
        self->PC += 1;
    }*/
}

void instr_0xd9(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("Not implemented");
    #endif
    printf("Not implemented");
    //self->PC += 1;
}

void instr_0xda(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("JC");
    #endif
    if (1 == self->CC.cy)
        self->PC = (self->memory[self->PC+2] << 8) | self->memory[self->PC+1];
    else
        self->PC += 2;
}

void instr_0xdb(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("IN");
    #endif
    printf("Not implemented");
    //self->PC += 1;
}

void instr_0xdc(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("CC");
    #endif
    if (1 == self->CC.cy) {
        //push(self, self->PC+2);
        self->PC = (self->memory[self->PC+2] << 8) | self->memory[self->PC+1];
    } else {
        self->PC += 2;
    }
}

void instr_0xdd(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("Not implemented");
    #endif
    printf("Not implemented");
    //self->PC += 1;
}

void instr_0xde(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("SBI");
    #endif
    uint16_t result = self->A - self->memory[self->PC+1] - self->CC.cy;
    self->CC.z = ((result & 0xff) == 0);
    self->CC.s = (0x80 == (result & 0x80));
    self->CC.p = parity((result&0xff), 8);
    self->CC.cy = (result > 0xff);
    self->A = (uint8_t) (result & 0xff);
    self->PC++;
}

void instr_0xdf(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("RST 3");
    #endif
    printf("Not implemented");
    //push(self, self->PC);
    //self->PC = 24;
}

void instr_0xe0(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("RPO");
    #endif
    printf("Not implemented");
    /*
    if (self->flags & FLAG_P) {
        self->PC += 1;
    } else {
        self->PC = pop(self);
    }*/
}

void instr_0xe1(i8080oObject *self) {
    #ifdef DEBUG
    printf("POP H");
    #endif
    self->L = self->memory[self->SP];
    self->H = self->memory[self->SP+1];
    self->SP += 2;
}

void instr_0xe2(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("JPO");
    #endif
    if (0 == self->CC.p)
        self->PC = (self->memory[self->PC+2] << 8) | self->memory[self->PC+1];
    else
        self->PC += 2;
}

void instr_0xe3(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("XTHL");
    #endif
    printf("Not implemented");
    /*
    uint8_t tmp = self->memory[self->SP];
    self->memory[self->SP] = self->L;
    self->L = tmp;
    tmp = self->memory[self->SP+1];
    self->memory[self->SP+1] = self->H;
    self->H = tmp;
    */
}

void instr_0xe4(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("CPO");
    #endif
    if (0 == self->CC.p) {
        //push(self, self->PC+2);
        self->PC = (self->memory[self->PC+2] << 8) | self->memory[self->PC+1];
    } else {
        self->PC += 2;
    }
}

void instr_0xe5(i8080oObject *self) {
    #ifdef DEBUG
    printf("PUSH H");
    #endif
    self->memory[self->SP-1] = self->H;
    self->memory[self->SP-2] = self->L;
    self->SP -= 2;
}

void instr_0xe6(i8080oObject *self) {
    #ifdef DEBUG
    printf("ANI");
    #endif
    self->A = self->A & self->memory[self->PC+1];
    LogicFlagsA(self);
    self->PC++;
}

void instr_0xe7(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("RST 4");
    #endif
    printf("Not implemented");
    //push(self, self->PC);
    //self->PC = 32;
}

void instr_0xe8(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("RPE");
    #endif
    printf("Not implemented");
    /*
    if (self->flags & FLAG_P) {
        self->PC = pop(self);
    } else {
        self->PC += 1;
    }*/
}

void instr_0xe9(i8080oObject *self) {
    #ifdef DEBUG
    printf("PCHL");
    #endif
    self->PC = (self->H << 8) | self->L;
}

void instr_0xea(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("JPE");
    #endif
    if (1 == self->CC.p)
        self->PC = (self->memory[self->PC+2] << 8) | self->memory[self->PC+1];
    else
        self->PC += 2;
}

void instr_0xeb(i8080oObject *self) {
    #ifdef DEBUG
    printf("XCHG");
    #endif
    uint8_t save1 = self->D;
    uint8_t save2 = self->E;
    self->D = self->H;
    self->E = self->L;
    self->H = save1;
    self->L = save2;
}

void instr_0xec(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("CPE");
    #endif
    if (1 == self->CC.p) {
        //push(self, self->PC+2);
        self->PC = (self->memory[self->PC+2] << 8) | self->memory[self->PC+1];
    } else {
        self->PC += 2;
    }
}

void instr_0xed(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("Not implemented");
    #endif
    printf("Not implemented");
    //self->PC += 1;
}

void instr_0xee(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("XRI");
    #endif
    self->A = self->A ^ self->memory[self->PC+1];
    LogicFlagsA(self);
    self->PC++;
}

void instr_0xef(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("RST 5");
    #endif
    printf("Not implemented");
    //push(self, self->PC);
    //self->PC = 40;
}

void instr_0xf0(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("RP");
    #endif
    printf("Not implemented");
    /*
    if (self->flags & FLAG_S) {
        self->PC += 1;
    } else {
        self->PC = pop(self);
    }*/
}

void instr_0xf1(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("POP PSW");
    #endif
    self->A = self->memory[self->SP+1];
    uint8_t flags = self->memory[self->SP];
    self->CC.z = (flags & 0x01) ? 1 : 0;
    self->CC.s = (flags & 0x02) ? 1 : 0;
    self->CC.p = (flags & 0x04) ? 1 : 0;
    self->CC.cy = (flags & 0x08) ? 1 : 0;
    self->CC.ac = (flags & 0x10) ? 1 : 0;
    self->SP += 2;
}

void instr_0xf2(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("JP");
    #endif
    if (0 == self->CC.s)
        self->PC = (self->memory[self->PC+2] << 8) | self->memory[self->PC+1];
    else
        self->PC += 2;
}

void instr_0xf3(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("DI");
    #endif
    printf("Not implemented");
    //self->flags &= ~FLAG_I;
}

void instr_0xf4(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("CP");
    #endif
    if (0 == self->CC.s) {
        //push(self, self->PC+2);
        self->PC = (self->memory[self->PC+2] << 8) | self->memory[self->PC+1];
    } else {
        self->PC += 2;
    }
}

void instr_0xf5(i8080oObject *self) {
    #ifdef DEBUG
    printf("PUSH PSW");
    #endif
    self->memory[self->SP-1] = self->A;
    self->memory[self->SP-2] = (self->CC.z << 0) | (self->CC.s << 1) | (self->CC.p << 2) | (self->CC.cy << 3) | (self->CC.ac << 4);
    self->SP -= 2;
}

void instr_0xf6(i8080oObject *self) {
    #ifdef DEBUG
    printf("ORI");
    #endif
    self->A = self->A | self->memory[self->PC+1];
    LogicFlagsA(self);
    self->PC++;
}

void instr_0xf7(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("RST 6");
    #endif
    printf("Not implemented");
    //push(self, self->PC);
    //self->PC = 48;
}

void instr_0xf8(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("RM");
    #endif
    printf("Not implemented");
    /*
    if (self->flags & FLAG_S) {
        self->PC = pop(self);
    } else {
        self->PC += 1;
    }*/
}

void instr_0xf9(i8080oObject *self) {
    #ifdef DEBUG
    printf("SPHL");
    #endif
    self->SP = (self->H << 8) | self->L;
}

void instr_0xfa(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("JM");
    #endif
    if (1 == self->CC.s)
        self->PC = (self->memory[self->PC+2] << 8) | self->memory[self->PC+1];
    else
        self->PC += 2;
}

void instr_0xfb(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("EI");
    #endif
    printf("Not implemented");
    //self->flags |= FLAG_I;
}

void instr_0xfc(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("CM");
    #endif
    if (1 == self->CC.s) {
        //push(self, self->PC+2);
        self->PC = (self->memory[self->PC+2] << 8) | self->memory[self->PC+1];
    } else {
        self->PC += 2;
    }
}

void instr_0xfd(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("Not implemented");
    #endif
    printf("Not implemented");
    //self->PC += 1;
}

void instr_0xfe(i8080oObject *self) {
    #ifdef DEBUG
    printf("CPI");
    #endif
    uint8_t val = self->memory[self->PC+1];
    self->CC.cy = (self->A < val) ? 1 : 0;
    self->CC.z = (self->A == val) ? 1 : 0;
    self->CC.s = ((self->A - val) & 0x80) ? 1 : 0;
    self->CC.p = parity(self->A - val, 8);
    self->PC++;
}

void instr_0xff(i8080oObject *self) {
    // not verified
    #ifdef DEBUG
    printf("RST 7");
    #endif
    printf("Not implemented");
    //push(self, self->PC);
    //self->PC = 56;
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