#pragma once

#include <Python.h>
#include <iostream>
#include <cstring>
#include <cstdint>


class i8080C
{
public:
    i8080C();

    virtual ~i8080C();

    unsigned long addOne();

    // getters and setters
    unsigned long getCnt() const { return m_cnt; }
    void setCnt(unsigned long val) { m_cnt = val; }

    uint8_t getA() const { return A; }
    void setA(uint8_t val) { A = val; }

    uint8_t getB() const { return B; }
    void setB(uint8_t val) { B = val; }

    uint8_t getC() const { return C; }
    void setC(uint8_t val) { C = val; }

    uint8_t getD() const { return D; }
    void setD(uint8_t val) { D = val; }

    uint8_t getE() const { return E; }
    void setE(uint8_t val) { E = val; }

    uint8_t getH() const { return H; }
    void setH(uint8_t val) { H = val; }

    uint8_t getL() const { return L; }
    void setL(uint8_t val) { L = val; }

    uint16_t getSP() const { return SP; }
    void setSP(uint16_t val) { SP = val; }

    uint16_t getPC() const { return PC; }
    void setPC(uint16_t val) { PC = val; }

    

private:
    unsigned long m_cnt;

   typedef struct ConditionCodes {
        uint8_t		z:1;
        uint8_t		s:1;
        uint8_t		p:1;
        uint8_t		cy:1;
        uint8_t		ac:1;
        // Custom added flags
        uint8_t     halt:1;
        uint8_t     int_enable:1;
        uint8_t		pad:1;
    } ConditionCodes;

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
    uint8_t    IO[256];   

 
};