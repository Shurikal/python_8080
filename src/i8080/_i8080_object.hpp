#ifndef _I8080_OBJECT_HPP
#define _I8080_OBJECT_HPP


#include "Python.h"

#include <cstdint>



class i8080oClass {
    public:

    struct ConditionCodes {
    uint8_t		z:1;
    uint8_t		s:1;
    uint8_t		p:1;
    uint8_t		cy:1;
    uint8_t		ac:1;
    // Custom added flags
    uint8_t     halt:1;
    uint8_t     int_enable:1;
    uint8_t		pad:1;
};

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
        uint8_t     *memory;        // Todo, make this a separate object for access like cpu.memory[] and not cpu[]
        //i8080oMemory *memory_obj;

        i8080oClass();
        ~i8080oClass();

        //static PyObject* newi8080(PyTypeObject *type, PyObject *args, PyObject *kwds);
        static void test();
};

typedef struct {
    PyObject_HEAD
    i8080oClass *cpu;
    PyObject    *x_attr;        /* Attributes dictionary */
} i8080oObject;

PyObject*
i8080o_new(PyTypeObject *type, PyObject *args, PyObject *kwds);

PyObject *i8080o_reset(i8080oObject *self, PyObject *args);


static PyMethodDef i8080o_methods[] = {
	{"reset",                  		(PyCFunction)i8080o_reset,                               METH_NOARGS, 					 PyDoc_STR("Reset the i8080")},
	{NULL,              NULL}           /* sentinel */
};


// i8080oObject basic methods
// https://docs.python.org/3/c-api/typeobj.html
// not static because it is used in the module init function
static PyTypeObject i8080o_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "_i8080.i8080uC",
    .tp_basicsize = sizeof(i8080oObject),
    .tp_new = i8080o_new,
    .tp_methods = i8080o_methods,
};



#endif