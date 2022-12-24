#ifndef MY_CLASS_PY_TYPE_H
#define MY_CLASS_PY_TYPE_H

#include <Python.h>
#include <structmember.h>


class i8080C
{
public:
    i8080C() : m_cnt(0) {std::cout << "i8080C::i8080C() called!" << std::endl;};

    virtual ~i8080C() {std::cout << "i8080C::~i8080C() called" << std::endl;};

    unsigned long addOne() { return ++m_cnt;};

private:
    unsigned long m_cnt;
};

typedef struct {
    PyObject_HEAD
    int         m_value;
    i8080C*    m_i8080C;
} i8080CObject;

int i8080C_init(PyObject *self, PyObject *args, PyObject *kwds);
PyObject *i8080C_new(PyTypeObject *subtype, PyObject *args, PyObject *kwds);
void i8080C_dealloc(i8080CObject *self);
PyObject* i8080C_addOne(PyObject *self, PyObject *args);

static PyMethodDef i8080C_methods[] = {
    {"addOne", (PyCFunction)i8080C_addOne, METH_NOARGS,  PyDoc_STR("Return an incrmented integer")},
    {NULL, NULL} /* Sentinel */
};

static struct PyMemberDef i8080C_members[] = {
    {"value", T_INT, offsetof(i8080CObject, m_value)},
    {NULL} /* Sentinel */
};

static PyObject *i8080C_getvalue(i8080CObject *self, void *closure){
    return PyLong_FromLong(self->m_value);
}

static int i8080C_setvalue(i8080CObject *self, PyObject *value, void *closure){
    if(!value){
        PyErr_SetString(PyExc_TypeError, "Cannot delete the value attribute");
        return -1;
    }

    if(!PyLong_Check(value)){
        PyErr_SetString(PyExc_TypeError, "The value attribute value must be an integer");
        return -1;
    }

    self->m_value = PyLong_AsLong(value);
    return 0;
}

static PyGetSetDef i8080C_getseters[] = {
    {"value", (getter)i8080C_getvalue, (setter)i8080C_setvalue, PyDoc_STR("Value of the object"), NULL},
    {NULL} /* Sentinel */
};

static PyType_Slot i8080C_slots[] = {
    {Py_tp_doc, (void*)PyDoc_STR("i8080C objects")},
    {Py_tp_new, (void*)i8080C_new},
    {Py_tp_init, (void*)i8080C_init},
    {Py_tp_dealloc, (void*)i8080C_dealloc},
    {Py_tp_getset, i8080C_getseters},
    //{Py_tp_members,  i8080C_members},
    {Py_tp_methods, i8080C_methods},
    {0, 0}
};

static PyType_Spec spec_i8080C = {
    "i8080C",                                  // name
    sizeof(i8080CObject) + sizeof(i8080C),    // basicsize
    0,                                          // itemsize
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,   // flags
    i8080C_slots                               // slots
};

#endif