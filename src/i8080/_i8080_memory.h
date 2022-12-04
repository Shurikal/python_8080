#ifndef _I8080_MEMORY_H
#define _I8080_MEMORY_H

#include "Python.h"

typedef struct {
    PyObject_HEAD
    uint8_t     *memory;
    PyObject    *x_attr;        /* Attributes dictionary */
} i8080oMemory;

// Definition of the i8080oMemory type
PyTypeObject i8080oMemory_Type;

#endif