#ifndef MY_CLASS_PY_TYPE_H
#define MY_CLASS_PY_TYPE_H

#include <Python.h>
#include <structmember.h>
#include <iostream>
#include <cstring>

#include "_i8080_class.hpp"


typedef struct {
    PyObject_HEAD
    int         m_value;
    i8080C*    m_i8080C;
} i8080CObject;

int i8080C_init(PyObject *self, PyObject *args, PyObject *kwds);
PyObject *i8080C_new(PyTypeObject *subtype, PyObject *args, PyObject *kwds);
void i8080C_dealloc(i8080CObject *self);
PyObject* i8080C_test(PyObject *self, PyObject *args);

static PyMethodDef i8080C_methods[] = {
    {"test", (PyCFunction)i8080C_test, METH_NOARGS,  PyDoc_STR("Test function")},
    {NULL, NULL} /* Sentinel */
};

// Getter and setter for the object

PyObject *i8080C_getA(i8080CObject *self, void *closure);
int i8080C_setA(i8080CObject *self, PyObject *value, void *closure);

PyObject *i8080C_getB(i8080CObject *self, void *closure);
int i8080C_setB(i8080CObject *self, PyObject *value, void *closure);

PyObject *i8080C_getC(i8080CObject *self, void *closure);
int i8080C_setC(i8080CObject *self, PyObject *value, void *closure);

PyObject *i8080C_getD(i8080CObject *self, void *closure);
int i8080C_setD(i8080CObject *self, PyObject *value, void *closure);

PyObject *i8080C_getE(i8080CObject *self, void *closure);
int i8080C_setE(i8080CObject *self, PyObject *value, void *closure);

PyObject *i8080C_getH(i8080CObject *self, void *closure);
int i8080C_setH(i8080CObject *self, PyObject *value, void *closure);

PyObject *i8080C_getL(i8080CObject *self, void *closure);
int i8080C_setL(i8080CObject *self, PyObject *value, void *closure);

PyObject *i8080C_getSP(i8080CObject *self, void *closure);
int i8080C_setSP(i8080CObject *self, PyObject *value, void *closure);

PyObject *i8080C_getPC(i8080CObject *self, void *closure);
int i8080C_setPC(i8080CObject *self, PyObject *value, void *closure);

PyObject *i8080C_getCC(i8080CObject *self, void *closure);
int i8080C_setCC(i8080CObject *self, PyObject *value, void *closure);


static PyGetSetDef i8080C_getseters[] = {
    {"a"    , (getter)i8080C_getA,      (setter)i8080C_setA,    PyDoc_STR("A register"), NULL},
    {"b"    , (getter)i8080C_getB,      (setter)i8080C_setB,    PyDoc_STR("B register"), NULL},
    {"c"    , (getter)i8080C_getC,      (setter)i8080C_setC,    PyDoc_STR("C register"), NULL},
    {"d"    , (getter)i8080C_getD,      (setter)i8080C_setD,    PyDoc_STR("D register"), NULL},
    {"e"    , (getter)i8080C_getE,      (setter)i8080C_setE,    PyDoc_STR("E register"), NULL},
    {"h"    , (getter)i8080C_getH,      (setter)i8080C_setH,    PyDoc_STR("H register"), NULL},
    {"l"    , (getter)i8080C_getL,      (setter)i8080C_setL,    PyDoc_STR("L register"), NULL},
    {"sp"   , (getter)i8080C_getSP,     (setter)i8080C_setSP,   PyDoc_STR("Stack pointer"), NULL},
    {"pc"   , (getter)i8080C_getPC,     (setter)i8080C_setPC,   PyDoc_STR("Program counter"), NULL},
  /*  {"cc"   , (getter)i8080C_getCC,     (setter)i8080C_setCC,   PyDoc_STR("Condition codes"), NULL},*/
    {NULL} /* Sentinel */
};

static PyType_Slot i8080C_slots[] = {
    {Py_tp_doc,         (void*)PyDoc_STR("i8080C docstring")},
    {Py_tp_new,         (void*)i8080C_new},
    {Py_tp_init,        (void*)i8080C_init},
    {Py_tp_dealloc,     (void*)i8080C_dealloc},
    {Py_tp_getset,      i8080C_getseters},
    {Py_tp_methods,     i8080C_methods},
    {0, 0}
};

static PyType_Spec spec_i8080C = {
    "i8080uC",                                   // name
    sizeof(i8080CObject) + sizeof(i8080C),      // basicsize
    0,                                          // itemsize
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,   // flags
    i8080C_slots                                // slots
};

#endif