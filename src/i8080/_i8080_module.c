#include "Python.h"
#include "_i8080_module.h"
#include "_i8080_object.h"
#include "_i8080_constants.h"


extern const uint8_t opcodes_lengths[256];
extern const char *opcodes_names[256];


/* ---------- 
Module Methods
----------  */


// Function of no arguments returning new i8080oObject object
static PyObject *
i8080_new(PyObject *self, PyObject *args)
{
    i8080oObject *rv;

    if (!PyArg_ParseTuple(args, ":new")){
        return NULL;
    }
    rv = newi8080oObject(args);
    if (rv == NULL){
        return NULL;
    }
    return (PyObject *)rv;
}

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
    return Py_BuildValue("s", opcodes_names[instruction]);
}

/* ---------- 
Module Initialization
---------- */

/* List of functions defined in the module */

// https://docs.python.org/3/c-api/structures.html?highlight=pymethoddef#c.PyMethodDef
static PyMethodDef i8080_methods[] = {
    {"i8080_new",               i8080_new,                         METH_VARARGS, PyDoc_STR("new() -> new i8080 object")},
    {"get_instruction_size",    (PyCFunction)get_instruction_size, METH_VARARGS, PyDoc_STR("get_instruction_size")},
    {"get_instruction_name",    (PyCFunction)get_instruction_name, METH_VARARGS, PyDoc_STR("get_instruction_name")},
    {NULL,              NULL}           /* sentinel */
};


PyDoc_STRVAR(module_doc,
"This module is for emulating the Intel 8080 CPU.");


// Slot initialization
static int64_t
i8080_exec(PyObject *m)
{
    #ifdef DEBUG
    printf("i8080_exec\n");
    #endif
    if (PyType_Ready(&i8080o_Type) < 0)
        goto fail;

    return 0;
 fail:
    Py_XDECREF(m);
    return -1;
}

// https://docs.python.org/3/c-api/module.html?highlight=pymoduledef_slot#c.PyModuleDef_Slot
static struct PyModuleDef_Slot i8080_slots[] = {
    {Py_mod_exec, i8080_exec},
    {0, NULL},
};

// https://docs.python.org/3/c-api/module.html#c.PyModuleDef
static struct PyModuleDef i8080module = {
    PyModuleDef_HEAD_INIT,
    "intel_8080",
    module_doc,
    0,
    i8080_methods,
    i8080_slots,
    NULL,
    NULL,
    NULL
};

/* 
Export function for the module (*must* be called PyInit_xx) 
module name is _i8080, from setup.py Extension
*/

PyMODINIT_FUNC
PyInit__i8080(void)
{
    return PyModuleDef_Init(&i8080module);
}