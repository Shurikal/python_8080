#ifndef _I8080_H
#define _I8080_H

#include <stdint.h>
#include <string.h>


typedef struct ConditionCodes {
	uint8_t		z:1;
	uint8_t		s:1;
	uint8_t		p:1;
	uint8_t		cy:1;
	uint8_t		ac:1;
	uint8_t		pad:3;
} ConditionCodes;

typedef struct {
    PyObject_HEAD
    uint8_t     A;
    uint8_t     B;
    uint8_t     C;
    uint8_t     D;
    uint8_t     E;
    uint8_t     H;
    uint8_t     L;
    uint16_t    SP;
    uint16_t    PC;
    struct ConditionCodes  CC;
    uint32_t    rom_size;
    uint8_t     *rom_data;
    PyObject    *x_attr;        /* Attributes dictionary */
} i8080oObject;


#endif