#include <Python.h>
#include <iostream>
#include <cstring>
#include <ostream>
#include <sstream>
#include "methodobject.h"
#include "modsupport.h"
#include "object.h"
#include "pyport.h"
#include "tupleobject.h"
#include "unicodeobject.h"
#include <structmember.h>

#include "_i8080_module.hpp"
#include "_i8080_constants.hpp"
#include "_i8080_object.hpp"

#include <iostream>
#include <string>
#include <cstdint>

extern const uint8_t opcodes_lengths[256];
extern const std::string opcodes_names[256];


PyDoc_STRVAR(module_doc, "This module is for emulating the Intel 8080 CPU.");


/* ---------- 
Module Methods
----------  */


static PyObject *
get_instruction_size(PyObject *self, PyObject *args)
{
    uint64_t instruction;
    if (!PyArg_ParseTuple(args, "K", &instruction)){
        PyErr_SetString(PyExc_Exception, "Parse error");
        return NULL;
    }
    if (instruction > 255){
        PyErr_SetString(PyExc_IndexError, "Out of bounds\n");
        return NULL;
    }
    return Py_BuildValue("H", opcodes_lengths[instruction]);
}

static PyObject *
get_instruction_name(PyObject *self, PyObject *args)
{
    uint64_t instruction;
    if (!PyArg_ParseTuple(args, "K", &instruction)){
        PyErr_SetString(PyExc_Exception, "Parse error");
        return NULL;
    }
    #ifdef DEBUG
    std::cout << "Instruction: " << instruction << std::endl;
    #endif
    if (instruction > 255){
        PyErr_SetString(PyExc_IndexError, "Out of bounds\n");
        return NULL;
    }

    return Py_BuildValue("s", opcodes_names[instruction].c_str());
}

static PyMethodDef i8080_module_methods[] = {
    {"get_instruction_size", (PyCFunction)get_instruction_size, METH_VARARGS, "Get the size of an instruction in bytes."},
    {"get_instruction_name", (PyCFunction)get_instruction_name, METH_VARARGS, "Get the name of an instruction."},
	{NULL, NULL}
};

// Slot initialization
static int64_t
i8080_exec(PyObject *m)
{
    #ifdef DEBUG
    std::cout << "Initializing i8080 module" << std::endl;
    #endif
    PyObject *myclass = PyType_FromSpec(&spec_i8080C);
    if (myclass == NULL){
        return -1;
    }
    Py_INCREF(myclass);

    if(PyModule_AddObject(m, "i8080C", myclass) < 0){
        Py_DECREF(myclass);
        return -1;
    }

    return 0;
}


static struct PyModuleDef_Slot i8080_module_slots[] = {
    {Py_mod_exec, (void *)i8080_exec},
    {0, NULL},
};

static struct PyModuleDef i8080module = {
    PyModuleDef_HEAD_INIT,
    "intel_8080",
    module_doc,
    0,
    i8080_module_methods,
    i8080_module_slots,
    NULL,
    NULL,
    NULL
};

PyMODINIT_FUNC PyInit__i8080(void){
	return PyModuleDef_Init(&i8080module);
}