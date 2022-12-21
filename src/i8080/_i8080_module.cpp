#include <Python.h>
#include "_i8080_module.hpp"
#include "_i8080_constants.hpp"

#include <iostream>
#include <string>

extern const uint8_t opcodes_lengths[256];
extern const std::string opcodes_names[256];


PyDoc_STRVAR(module_doc, "This module is for emulating the Intel 8080 CPU.");


/* ---------- 
Module Methods
----------  */


static PyObject *
get_instruction_size(PyObject *self, PyObject *args)
{
    uint8_t instruction;
    // parse the argument to a uint8_t
    if (!PyArg_ParseTuple(args, "b", &instruction)){
        // throw parse error
        PyErr_SetString(PyExc_Exception, "Parse error");
        return NULL;
    }
    return Py_BuildValue("H", opcodes_lengths[instruction]);
}

static PyObject *
get_instruction_name(PyObject *self, PyObject *args)
{
    uint8_t instruction;
    // parse the argument to a uint8_t
    if (!PyArg_ParseTuple(args, "b", &instruction)){
        PyErr_SetString(PyExc_Exception, "Parse error");
        return NULL;
    }
    #ifdef DEBUG
    printf("Instruction: %d", instruction);
    #endif
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
    std::cout << "i8080_exec\n";
    #endif
    /*
    if (PyType_Ready(&i8080oMemory_Type) < 0)
        goto fail;

    // don't add the i8080oMemory_Type to the module
    // it's a private type only used by the i8080o_Type
    // PyModule_AddType(m, &i8080oMemory_Type);

    if (PyType_Ready(&i8080o_Type) < 0)
        goto fail;

    // add the i8080o_Type to the module
    PyModule_AddType(m, &i8080o_Type);
    */
    return 0;
 fail:
    Py_XDECREF(m);
    return -1;
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