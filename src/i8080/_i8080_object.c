#include "Python.h"
#include "_i8080_module.h"
#include "_i8080_object.h"
#include "_i8080_constants.h"
#include "_i8080_cpu.h"

extern const uint8_t opcodes_cycles[256];
extern const char *opcodes_names[256];

// external defined in _i8080_cpu.c
extern void (*CPU_FUNCTIONS[256]) (i8080oObject *self);

/* ---------- 
i8080 Object methods
----------  */

/* ----------
run_instruction
---------- */

static PyObject *
i8080o_run_instruction(i8080oObject *self, PyObject *args)
{
	#ifdef DEBUG
	printf("%04x - ", self->PC);
	#endif

	uint8_t op_code = self->memory[self->PC];

	CPU_FUNCTIONS[op_code](self);
	
	Py_RETURN_NONE;	
}

/* ----------
disassemble instruction
----------  */
static PyObject *
i8080o_disassemble(i8080oObject *self, PyObject *args){

    if(self->PC >= MEMORY_SIZE){
        PyErr_SetString(PyExc_ValueError, "PC out of range");
        return NULL;
    }

    unsigned char *code = &self->memory[self->PC];
	int opbytes = 1;

    char* string;
    int64_t ret_val;

	switch (*code)
	{
		case 0x00: ret_val=asprintf(&string, "NOP"); break;
		case 0x01: ret_val=asprintf(&string, "LXI    B,#$%02x%02x", code[2], code[1]); opbytes=3; break;
		case 0x02: ret_val=asprintf(&string, "STAX   B"); break;
		case 0x03: ret_val=asprintf(&string, "INX    B"); break;
		case 0x04: ret_val=asprintf(&string, "INR    B"); break;
		case 0x05: ret_val=asprintf(&string, "DCR    B"); break;
		case 0x06: ret_val=asprintf(&string, "MVI    B,#$%02x", code[1]); opbytes=2; break;
		case 0x07: ret_val=asprintf(&string, "RLC"); break;
		case 0x08: ret_val=asprintf(&string, "NOP"); break;
		case 0x09: ret_val=asprintf(&string, "DAD    B"); break;
		case 0x0a: ret_val=asprintf(&string, "LDAX   B"); break;
		case 0x0b: ret_val=asprintf(&string, "DCX    B"); break;
		case 0x0c: ret_val=asprintf(&string, "INR    C"); break;
		case 0x0d: ret_val=asprintf(&string, "DCR    C"); break;
		case 0x0e: ret_val=asprintf(&string, "MVI    C,#$%02x", code[1]); opbytes = 2;	break;
		case 0x0f: ret_val=asprintf(&string, "RRC"); break;
			
		case 0x10: ret_val=asprintf(&string, "NOP"); break;
		case 0x11: ret_val=asprintf(&string, "LXI    D,#$%02x%02x", code[2], code[1]); opbytes=3; break;
		case 0x12: ret_val=asprintf(&string, "STAX   D"); break;
		case 0x13: ret_val=asprintf(&string, "INX    D"); break;
		case 0x14: ret_val=asprintf(&string, "INR    D"); break;
		case 0x15: ret_val=asprintf(&string, "DCR    D"); break;
		case 0x16: ret_val=asprintf(&string, "MVI    D,#$%02x", code[1]); opbytes=2; break;
		case 0x17: ret_val=asprintf(&string, "RAL"); break;
		case 0x18: ret_val=asprintf(&string, "NOP"); break;
		case 0x19: ret_val=asprintf(&string, "DAD    D"); break;
		case 0x1a: ret_val=asprintf(&string, "LDAX   D"); break;
		case 0x1b: ret_val=asprintf(&string, "DCX    D"); break;
		case 0x1c: ret_val=asprintf(&string, "INR    E"); break;
		case 0x1d: ret_val=asprintf(&string, "DCR    E"); break;
		case 0x1e: ret_val=asprintf(&string, "MVI    E,#$%02x", code[1]); opbytes = 2; break;
		case 0x1f: ret_val=asprintf(&string, "RAR"); break;
			
		case 0x20: ret_val=asprintf(&string, "NOP"); break;
		case 0x21: ret_val=asprintf(&string, "LXI    H,#$%02x%02x", code[2], code[1]); opbytes=3; break;
		case 0x22: ret_val=asprintf(&string, "SHLD   $%02x%02x", code[2], code[1]); opbytes=3; break;
		case 0x23: ret_val=asprintf(&string, "INX    H"); break;
		case 0x24: ret_val=asprintf(&string, "INR    H"); break;
		case 0x25: ret_val=asprintf(&string, "DCR    H"); break;
		case 0x26: ret_val=asprintf(&string, "MVI    H,#$%02x", code[1]); opbytes=2; break;
		case 0x27: ret_val=asprintf(&string, "DAA"); break;
		case 0x28: ret_val=asprintf(&string, "NOP"); break;
		case 0x29: ret_val=asprintf(&string, "DAD    H"); break;
		case 0x2a: ret_val=asprintf(&string, "LHLD   $%02x%02x", code[2], code[1]); opbytes=3; break;
		case 0x2b: ret_val=asprintf(&string, "DCX    H"); break;
		case 0x2c: ret_val=asprintf(&string, "INR    L"); break;
		case 0x2d: ret_val=asprintf(&string, "DCR    L"); break;
		case 0x2e: ret_val=asprintf(&string, "MVI    L,#$%02x", code[1]); opbytes = 2; break;
		case 0x2f: ret_val=asprintf(&string, "CMA"); break;
			
		case 0x30: ret_val=asprintf(&string, "NOP"); break;
		case 0x31: ret_val=asprintf(&string, "LXI    SP,#$%02x%02x", code[2], code[1]); opbytes=3; break;
		case 0x32: ret_val=asprintf(&string, "STA    $%02x%02x", code[2], code[1]); opbytes=3; break;
		case 0x33: ret_val=asprintf(&string, "INX    SP"); break;
		case 0x34: ret_val=asprintf(&string, "INR    M"); break;
		case 0x35: ret_val=asprintf(&string, "DCR    M"); break;
		case 0x36: ret_val=asprintf(&string, "MVI    M,#$%02x", code[1]); opbytes=2; break;
		case 0x37: ret_val=asprintf(&string, "STC"); break;
		case 0x38: ret_val=asprintf(&string, "NOP"); break;
		case 0x39: ret_val=asprintf(&string, "DAD    SP"); break;
		case 0x3a: ret_val=asprintf(&string, "LDA    $%02x%02x", code[2], code[1]); opbytes=3; break;
		case 0x3b: ret_val=asprintf(&string, "DCX    SP"); break;
		case 0x3c: ret_val=asprintf(&string, "INR    A"); break;
		case 0x3d: ret_val=asprintf(&string, "DCR    A"); break;
		case 0x3e: ret_val=asprintf(&string, "MVI    A,#$%02x", code[1]); opbytes = 2; break;
		case 0x3f: ret_val=asprintf(&string, "CMC"); break;
			
		case 0x40: ret_val=asprintf(&string, "MOV    B,B"); break;
		case 0x41: ret_val=asprintf(&string, "MOV    B,C"); break;
		case 0x42: ret_val=asprintf(&string, "MOV    B,D"); break;
		case 0x43: ret_val=asprintf(&string, "MOV    B,E"); break;
		case 0x44: ret_val=asprintf(&string, "MOV    B,H"); break;
		case 0x45: ret_val=asprintf(&string, "MOV    B,L"); break;
		case 0x46: ret_val=asprintf(&string, "MOV    B,M"); break;
		case 0x47: ret_val=asprintf(&string, "MOV    B,A"); break;
		case 0x48: ret_val=asprintf(&string, "MOV    C,B"); break;
		case 0x49: ret_val=asprintf(&string, "MOV    C,C"); break;
		case 0x4a: ret_val=asprintf(&string, "MOV    C,D"); break;
		case 0x4b: ret_val=asprintf(&string, "MOV    C,E"); break;
		case 0x4c: ret_val=asprintf(&string, "MOV    C,H"); break;
		case 0x4d: ret_val=asprintf(&string, "MOV    C,L"); break;
		case 0x4e: ret_val=asprintf(&string, "MOV    C,M"); break;
		case 0x4f: ret_val=asprintf(&string, "MOV    C,A"); break;
			
		case 0x50: ret_val=asprintf(&string, "MOV    D,B"); break;
		case 0x51: ret_val=asprintf(&string, "MOV    D,C"); break;
		case 0x52: ret_val=asprintf(&string, "MOV    D,D"); break;
		case 0x53: ret_val=asprintf(&string, "MOV    D.E"); break;
		case 0x54: ret_val=asprintf(&string, "MOV    D,H"); break;
		case 0x55: ret_val=asprintf(&string, "MOV    D,L"); break;
		case 0x56: ret_val=asprintf(&string, "MOV    D,M"); break;
		case 0x57: ret_val=asprintf(&string, "MOV    D,A"); break;
		case 0x58: ret_val=asprintf(&string, "MOV    E,B"); break;
		case 0x59: ret_val=asprintf(&string, "MOV    E,C"); break;
		case 0x5a: ret_val=asprintf(&string, "MOV    E,D"); break;
		case 0x5b: ret_val=asprintf(&string, "MOV    E,E"); break;
		case 0x5c: ret_val=asprintf(&string, "MOV    E,H"); break;
		case 0x5d: ret_val=asprintf(&string, "MOV    E,L"); break;
		case 0x5e: ret_val=asprintf(&string, "MOV    E,M"); break;
		case 0x5f: ret_val=asprintf(&string, "MOV    E,A"); break;

		case 0x60: ret_val=asprintf(&string, "MOV    H,B"); break;
		case 0x61: ret_val=asprintf(&string, "MOV    H,C"); break;
		case 0x62: ret_val=asprintf(&string, "MOV    H,D"); break;
		case 0x63: ret_val=asprintf(&string, "MOV    H.E"); break;
		case 0x64: ret_val=asprintf(&string, "MOV    H,H"); break;
		case 0x65: ret_val=asprintf(&string, "MOV    H,L"); break;
		case 0x66: ret_val=asprintf(&string, "MOV    H,M"); break;
		case 0x67: ret_val=asprintf(&string, "MOV    H,A"); break;
		case 0x68: ret_val=asprintf(&string, "MOV    L,B"); break;
		case 0x69: ret_val=asprintf(&string, "MOV    L,C"); break;
		case 0x6a: ret_val=asprintf(&string, "MOV    L,D"); break;
		case 0x6b: ret_val=asprintf(&string, "MOV    L,E"); break;
		case 0x6c: ret_val=asprintf(&string, "MOV    L,H"); break;
		case 0x6d: ret_val=asprintf(&string, "MOV    L,L"); break;
		case 0x6e: ret_val=asprintf(&string, "MOV    L,M"); break;
		case 0x6f: ret_val=asprintf(&string, "MOV    L,A"); break;

		case 0x70: ret_val=asprintf(&string, "MOV    M,B"); break;
		case 0x71: ret_val=asprintf(&string, "MOV    M,C"); break;
		case 0x72: ret_val=asprintf(&string, "MOV    M,D"); break;
		case 0x73: ret_val=asprintf(&string, "MOV    M.E"); break;
		case 0x74: ret_val=asprintf(&string, "MOV    M,H"); break;
		case 0x75: ret_val=asprintf(&string, "MOV    M,L"); break;
		case 0x76: ret_val=asprintf(&string, "HLT");        break;
		case 0x77: ret_val=asprintf(&string, "MOV    M,A"); break;
		case 0x78: ret_val=asprintf(&string, "MOV    A,B"); break;
		case 0x79: ret_val=asprintf(&string, "MOV    A,C"); break;
		case 0x7a: ret_val=asprintf(&string, "MOV    A,D"); break;
		case 0x7b: ret_val=asprintf(&string, "MOV    A,E"); break;
		case 0x7c: ret_val=asprintf(&string, "MOV    A,H"); break;
		case 0x7d: ret_val=asprintf(&string, "MOV    A,L"); break;
		case 0x7e: ret_val=asprintf(&string, "MOV    A,M"); break;
		case 0x7f: ret_val=asprintf(&string, "MOV    A,A"); break;

		case 0x80: ret_val=asprintf(&string, "ADD    B"); break;
		case 0x81: ret_val=asprintf(&string, "ADD    C"); break;
		case 0x82: ret_val=asprintf(&string, "ADD    D"); break;
		case 0x83: ret_val=asprintf(&string, "ADD    E"); break;
		case 0x84: ret_val=asprintf(&string, "ADD    H"); break;
		case 0x85: ret_val=asprintf(&string, "ADD    L"); break;
		case 0x86: ret_val=asprintf(&string, "ADD    M"); break;
		case 0x87: ret_val=asprintf(&string, "ADD    A"); break;
		case 0x88: ret_val=asprintf(&string, "ADC    B"); break;
		case 0x89: ret_val=asprintf(&string, "ADC    C"); break;
		case 0x8a: ret_val=asprintf(&string, "ADC    D"); break;
		case 0x8b: ret_val=asprintf(&string, "ADC    E"); break;
		case 0x8c: ret_val=asprintf(&string, "ADC    H"); break;
		case 0x8d: ret_val=asprintf(&string, "ADC    L"); break;
		case 0x8e: ret_val=asprintf(&string, "ADC    M"); break;
		case 0x8f: ret_val=asprintf(&string, "ADC    A"); break;

		case 0x90: ret_val=asprintf(&string, "SUB    B"); break;
		case 0x91: ret_val=asprintf(&string, "SUB    C"); break;
		case 0x92: ret_val=asprintf(&string, "SUB    D"); break;
		case 0x93: ret_val=asprintf(&string, "SUB    E"); break;
		case 0x94: ret_val=asprintf(&string, "SUB    H"); break;
		case 0x95: ret_val=asprintf(&string, "SUB    L"); break;
		case 0x96: ret_val=asprintf(&string, "SUB    M"); break;
		case 0x97: ret_val=asprintf(&string, "SUB    A"); break;
		case 0x98: ret_val=asprintf(&string, "SBB    B"); break;
		case 0x99: ret_val=asprintf(&string, "SBB    C"); break;
		case 0x9a: ret_val=asprintf(&string, "SBB    D"); break;
		case 0x9b: ret_val=asprintf(&string, "SBB    E"); break;
		case 0x9c: ret_val=asprintf(&string, "SBB    H"); break;
		case 0x9d: ret_val=asprintf(&string, "SBB    L"); break;
		case 0x9e: ret_val=asprintf(&string, "SBB    M"); break;
		case 0x9f: ret_val=asprintf(&string, "SBB    A"); break;

		case 0xa0: ret_val=asprintf(&string, "ANA    B"); break;
		case 0xa1: ret_val=asprintf(&string, "ANA    C"); break;
		case 0xa2: ret_val=asprintf(&string, "ANA    D"); break;
		case 0xa3: ret_val=asprintf(&string, "ANA    E"); break;
		case 0xa4: ret_val=asprintf(&string, "ANA    H"); break;
		case 0xa5: ret_val=asprintf(&string, "ANA    L"); break;
		case 0xa6: ret_val=asprintf(&string, "ANA    M"); break;
		case 0xa7: ret_val=asprintf(&string, "ANA    A"); break;
		case 0xa8: ret_val=asprintf(&string, "XRA    B"); break;
		case 0xa9: ret_val=asprintf(&string, "XRA    C"); break;
		case 0xaa: ret_val=asprintf(&string, "XRA    D"); break;
		case 0xab: ret_val=asprintf(&string, "XRA    E"); break;
		case 0xac: ret_val=asprintf(&string, "XRA    H"); break;
		case 0xad: ret_val=asprintf(&string, "XRA    L"); break;
		case 0xae: ret_val=asprintf(&string, "XRA    M"); break;
		case 0xaf: ret_val=asprintf(&string, "XRA    A"); break;

		case 0xb0: ret_val=asprintf(&string, "ORA    B"); break;
		case 0xb1: ret_val=asprintf(&string, "ORA    C"); break;
		case 0xb2: ret_val=asprintf(&string, "ORA    D"); break;
		case 0xb3: ret_val=asprintf(&string, "ORA    E"); break;
		case 0xb4: ret_val=asprintf(&string, "ORA    H"); break;
		case 0xb5: ret_val=asprintf(&string, "ORA    L"); break;
		case 0xb6: ret_val=asprintf(&string, "ORA    M"); break;
		case 0xb7: ret_val=asprintf(&string, "ORA    A"); break;
		case 0xb8: ret_val=asprintf(&string, "CMP    B"); break;
		case 0xb9: ret_val=asprintf(&string, "CMP    C"); break;
		case 0xba: ret_val=asprintf(&string, "CMP    D"); break;
		case 0xbb: ret_val=asprintf(&string, "CMP    E"); break;
		case 0xbc: ret_val=asprintf(&string, "CMP    H"); break;
		case 0xbd: ret_val=asprintf(&string, "CMP    L"); break;
		case 0xbe: ret_val=asprintf(&string, "CMP    M"); break;
		case 0xbf: ret_val=asprintf(&string, "CMP    A"); break;

		case 0xc0: ret_val=asprintf(&string, "RNZ"); break;
		case 0xc1: ret_val=asprintf(&string, "POP    B"); break;
		case 0xc2: ret_val=asprintf(&string, "JNZ    $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xc3: ret_val=asprintf(&string, "JMP    $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xc4: ret_val=asprintf(&string, "CNZ    $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xc5: ret_val=asprintf(&string, "PUSH   B"); break;
		case 0xc6: ret_val=asprintf(&string, "ADI    #$%02x",code[1]); opbytes = 2; break;
		case 0xc7: ret_val=asprintf(&string, "RST    0"); break;
		case 0xc8: ret_val=asprintf(&string, "RZ"); break;
		case 0xc9: ret_val=asprintf(&string, "RET"); break;
		case 0xca: ret_val=asprintf(&string, "JZ     $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xcb: ret_val=asprintf(&string, "JMP    $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xcc: ret_val=asprintf(&string, "CZ     $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xcd: ret_val=asprintf(&string, "CALL   $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xce: ret_val=asprintf(&string, "ACI    #$%02x",code[1]); opbytes = 2; break;
		case 0xcf: ret_val=asprintf(&string, "RST    1"); break;

		case 0xd0: ret_val=asprintf(&string, "RNC"); break;
		case 0xd1: ret_val=asprintf(&string, "POP    D"); break;
		case 0xd2: ret_val=asprintf(&string, "JNC    $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xd3: ret_val=asprintf(&string, "OUT    #$%02x",code[1]); opbytes = 2; break;
		case 0xd4: ret_val=asprintf(&string, "CNC    $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xd5: ret_val=asprintf(&string, "PUSH   D"); break;
		case 0xd6: ret_val=asprintf(&string, "SUI    #$%02x",code[1]); opbytes = 2; break;
		case 0xd7: ret_val=asprintf(&string, "RST    2"); break;
		case 0xd8: ret_val=asprintf(&string, "RC");  break;
		case 0xd9: ret_val=asprintf(&string, "RET"); break;
		case 0xda: ret_val=asprintf(&string, "JC     $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xdb: ret_val=asprintf(&string, "IN     #$%02x",code[1]); opbytes = 2; break;
		case 0xdc: ret_val=asprintf(&string, "CC     $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xdd: ret_val=asprintf(&string, "CALL   $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xde: ret_val=asprintf(&string, "SBI    #$%02x",code[1]); opbytes = 2; break;
		case 0xdf: ret_val=asprintf(&string, "RST    3"); break;

		case 0xe0: ret_val=asprintf(&string, "RPO"); break;
		case 0xe1: ret_val=asprintf(&string, "POP    H"); break;
		case 0xe2: ret_val=asprintf(&string, "JPO    $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xe3: ret_val=asprintf(&string, "XTHL");break;
		case 0xe4: ret_val=asprintf(&string, "CPO    $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xe5: ret_val=asprintf(&string, "PUSH   H"); break;
		case 0xe6: ret_val=asprintf(&string, "ANI    #$%02x",code[1]); opbytes = 2; break;
		case 0xe7: ret_val=asprintf(&string, "RST    4"); break;
		case 0xe8: ret_val=asprintf(&string, "RPE"); break;
		case 0xe9: ret_val=asprintf(&string, "PCHL");break;
		case 0xea: ret_val=asprintf(&string, "JPE    $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xeb: ret_val=asprintf(&string, "XCHG"); break;
		case 0xec: ret_val=asprintf(&string, "CPE     $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xed: ret_val=asprintf(&string, "CALL   $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xee: ret_val=asprintf(&string, "XRI    #$%02x",code[1]); opbytes = 2; break;
		case 0xef: ret_val=asprintf(&string, "RST    5"); break;

		case 0xf0: ret_val=asprintf(&string, "RP");  break;
		case 0xf1: ret_val=asprintf(&string, "POP    PSW"); break;
		case 0xf2: ret_val=asprintf(&string, "JP     $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xf3: ret_val=asprintf(&string, "DI");  break;
		case 0xf4: ret_val=asprintf(&string, "CP     $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xf5: ret_val=asprintf(&string, "PUSH   PSW"); break;
		case 0xf6: ret_val=asprintf(&string, "ORI    #$%02x",code[1]); opbytes = 2; break;
		case 0xf7: ret_val=asprintf(&string, "RST    6"); break;
		case 0xf8: ret_val=asprintf(&string, "RM");  break;
		case 0xf9: ret_val=asprintf(&string, "SPHL");break;
		case 0xfa: ret_val=asprintf(&string, "JM     $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xfb: ret_val=asprintf(&string, "EI");  break;
		case 0xfc: ret_val=asprintf(&string, "CM     $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xfd: ret_val=asprintf(&string, "CALL   $%02x%02x",code[2],code[1]); opbytes = 3; break;
		case 0xfe: ret_val=asprintf(&string, "CPI    #$%02x",code[1]); opbytes = 2; break;
		case 0xff: ret_val=asprintf(&string, "RST    7"); break;
	}

    if(0> ret_val){
        PyErr_SetString(PyExc_SystemError, "asprintf failed");
        return NULL;
    }

    self->PC += opbytes;

	PyObject *ret =Py_BuildValue("is", opbytes, string);

	free(string);

    return ret;
}


/* ----------
get register
----------  */
static PyObject *
i8080o_get_reg(i8080oObject *self, PyObject *args)
{
    // return the value of the register
    char *reg;
    if (!PyArg_ParseTuple(args, "s", &reg)){
        PyErr_SetString(PyExc_Exception, "Parse error\n");
        return NULL;
    }

    if (strcmp(reg, "a") == 0)
        return Py_BuildValue("i", self->A);
    else if (strcmp(reg, "b") == 0)
        return Py_BuildValue("i", self->B);
    else if (strcmp(reg, "c") == 0)
        return Py_BuildValue("i", self->C);
    else if (strcmp(reg, "d") == 0)
        return Py_BuildValue("i", self->D);
    else if (strcmp(reg, "e") == 0)
        return Py_BuildValue("i", self->E);
    else if (strcmp(reg, "h") == 0)
        return Py_BuildValue("i", self->H);
    else if (strcmp(reg, "l") == 0)
        return Py_BuildValue("i", self->L);
    else if (strcmp(reg, "pc") == 0)
        return Py_BuildValue("i", self->PC);
    else if (strcmp(reg, "sp") == 0)
        return Py_BuildValue("i", self->SP);
    else if (strcmp(reg, "cc") == 0)
        return Py_BuildValue("i", self->CC);
    else {
        // https://docs.python.org/3/c-api/exceptions.html#standard-exceptions
        PyErr_SetString(PyExc_LookupError, "Invalid register\n");
        return NULL;
    }

}

/* ----------
set register
----------  */
static PyObject *
i8080o_set_reg(i8080oObject *self, PyObject *args, PyObject *keywds)
{
    // return the value of the register
    char *reg;
    uint16_t val;

    static char *kwlist[] = {"register", "value", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "sH", kwlist, &reg, &val)){
        PyErr_SetString(PyExc_Exception, "Parse error\n");
        return NULL;
    }

    
    #ifdef DEBUG
	// print the value
    // printf("Setting register %s to %d\n	", reg, val);
    #endif

    if (strcmp(reg, "a") == 0){
        // check for overflow
        if (val > 0xFF){
            goto overflow;
        }
        self->A = val;
        return Py_BuildValue("i", self->A);
    }
    else if (strcmp(reg, "b") == 0){
        if (val > 0xFF){
            goto overflow;
        }
        self->B = val;
        return Py_BuildValue("i", self->B);
    }
    else if (strcmp(reg, "c") == 0){
        if (val > 0xFF){
            goto overflow;
        }
        self->C = val;
        return Py_BuildValue("i", self->C);
    }
    else if (strcmp(reg, "d") == 0){
        if (val > 0xFF){
            goto overflow;
        }
        self->D = val;
        return Py_BuildValue("i", self->D);
    }
    else if (strcmp(reg, "e") == 0){
        if (val > 0xFF){
            goto overflow;
        }
        self->E = val;
        return Py_BuildValue("i", self->E);
    }
    else if (strcmp(reg, "h") == 0){
        if (val > 0xFF){
            goto overflow;
        }
        self->H = val;
        return Py_BuildValue("i", self->H);
    }
    else if (strcmp(reg, "l") == 0){
        if (val > 0xFF){
            goto overflow;
        }
        self->L = val;
        return Py_BuildValue("i", self->L);
    }
    else if (strcmp(reg, "pc") == 0){
        self->PC = val;
        return Py_BuildValue("i", self->PC);
    }
    else if (strcmp(reg, "sp") == 0){
        self->SP = val;
        return Py_BuildValue("i", self->SP);
    }
    else if (strcmp(reg, "cc") == 0){
        if (val > 0xFF){
            goto overflow;
        }
        memcpy(&self->CC, &val, sizeof(ConditionCodes));
        return Py_BuildValue("i", self->CC);
    }
    else {
        // https://docs.python.org/3/c-api/exceptions.html#standard-exceptions
        PyErr_SetString(PyExc_LookupError, "Invalid register\n");
        return NULL;
    }

overflow:
        PyErr_SetString(PyExc_OverflowError, "Value too large\n");
        return NULL;
}


/* ----------
load rom
----------  */
static PyObject *
i8080o_load_rom(i8080oObject *self, PyObject *args, PyObject *keywds)
{
    char *file_path;
    uint32_t offset;
    static char *kwlist[] = {"file_path", "offset", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "sI", kwlist, &file_path, &offset)){
        PyErr_SetString(PyExc_Exception, "Parse error\n");
        return NULL;
    }

    FILE *f= fopen(file_path, "rb");
	if (f==NULL)
	{
        PyErr_SetString(PyExc_FileNotFoundError, "Could not open file\n");
        return NULL;
	}

	fseek(f, 0L, SEEK_END);
	int fsize = ftell(f);
	fseek(f, 0L, SEEK_SET);

    if (fsize + offset > MEMORY_SIZE){
        PyErr_SetString(PyExc_OverflowError, "File too large\n");
        return NULL;
    }
	
	uint8_t *buffer = &self->memory[offset];
	fread(buffer, fsize, 1, f);
	fclose(f);


    return Py_BuildValue("i", fsize);
}

/* ----------
read rom at address
----------  */
static PyObject *
i8080o_read_memory(i8080oObject *self, PyObject *args)
{
    uint32_t pos;
    if (!PyArg_ParseTuple(args, "I", &pos)){
        PyErr_SetString(PyExc_Exception, "Parse error\n");
        return NULL;
    }

    if (pos >= MEMORY_SIZE){
        PyErr_SetString(PyExc_IndexError, "Out of bounds\n");
        return NULL;
    }

    return Py_BuildValue("i", self->memory[pos]);
}

/*
set rom at address
*/
static PyObject *
i8080o_set_memory(i8080oObject *self, PyObject *args)
{
	uint32_t pos;
	uint8_t val;
	if (!PyArg_ParseTuple(args, "IB", &pos, &val)){
		PyErr_SetString(PyExc_Exception, "Parse error\n");
		return NULL;
	}

	if (pos >= MEMORY_SIZE){
		PyErr_SetString(PyExc_IndexError, "Out of bounds\n");
		return NULL;
	}

	self->memory[pos] = val;

	return Py_BuildValue("i", self->memory[pos]);
}

/* ---------- 
i8080 Object initialization
----------  */

//static i8080oObject *newi8080oObject();

// Initialize the i8080 object and set the default values
i8080oObject *
newi8080oObject(PyObject *arg)
{
    #ifdef DEBUG
    printf("Initialising i8080 object\n");
    #endif
    i8080oObject *self;
    self = PyObject_New(i8080oObject, &i8080o_Type);
    if (self == NULL)
        return NULL;
    self->x_attr = NULL;

    // Set the default values
    self->A = 0;
    self->B = 0;
    self->C = 0;
    self->D = 0;
    self->E = 0;
    self->H = 0;
    self->L = 0;
    self->PC = 0;
    self->SP = 0;
    memset(&self->CC, 0, sizeof(ConditionCodes));
    self->memory = malloc(MEMORY_SIZE);

    if (self->memory == NULL){
        PyErr_SetString(PyExc_MemoryError, "Could not allocate memory\n");
        return NULL;
    }

    return self;
}

// deallocate memory method
static void
i8080o_dealloc(i8080oObject *self)
{
    #ifdef DEBUG
    printf("Deallocating memory\n");
    #endif
    // free the memory
    if(self->memory != NULL){
        free(self->memory);
    }

    Py_XDECREF(self->x_attr);

    PyObject_Free(self);
}


static PyMethodDef i8080o_methods[] = {
	{"get_reg",                (PyCFunction)i8080o_get_reg,                             METH_VARARGS,                   PyDoc_STR("get register A")},
    {"set_reg",                (PyCFunction)(void(*)(void))i8080o_set_reg,              METH_VARARGS | METH_KEYWORDS,   PyDoc_STR("set register")},
    {"disassemble",            (PyCFunction)i8080o_disassemble,                         METH_VARARGS,                   PyDoc_STR("get register A")},
    {"load_rom",               (PyCFunction)(void(*)(void))i8080o_load_rom,             METH_VARARGS | METH_KEYWORDS,   PyDoc_STR("load rom")},
    {"read_memory",               (PyCFunction)i8080o_read_memory,                            METH_VARARGS,                   PyDoc_STR("read rom")},
    {"set_memory",                (PyCFunction)i8080o_set_memory,                             METH_VARARGS,                   PyDoc_STR("set rom")},
	{"run_instruction",        (PyCFunction)i8080o_run_instruction,                     METH_NOARGS,                    PyDoc_STR("run instruction")},
	{NULL,              NULL}           /* sentinel */
};

// i8080oObject basic methods
// https://docs.python.org/3/c-api/typeobj.html
// not static because it is used in the module init function
PyTypeObject i8080o_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "_i8080.i8080oObject",
    .tp_basicsize = sizeof(i8080oObject),
    .tp_dealloc = (destructor)i8080o_dealloc,
    //.tp_getattr = (getattrfunc)0,
    //.tp_setattr = (setattrfunc)AVRo_setattr,
    //.tp_getattro = (getattrofunc)AVRo_getattro,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_methods = i8080o_methods,
};