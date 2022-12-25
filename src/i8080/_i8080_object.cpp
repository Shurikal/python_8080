#include <Python.h>
#include <iostream>
#include "_i8080_module.hpp"
#include "_i8080_object.hpp"
#include <exception>
#include <string>
#include "_i8080_class.hpp"


PyObject *i8080C_new(PyTypeObject *type, PyObject *args, PyObject *kwds){
    #ifdef DEBUG
    std::cout << "i8080C_new() called!" << std::endl;
    #endif
    i8080CObject *self;
    self = (i8080CObject*) type->tp_alloc(type, 0);
    if(self == NULL){ 
        PyErr_SetString(PyExc_RuntimeError, "i8080C_new() failed");
        return NULL;
    }
    self->m_value   = 0;
    self->m_i8080C = NULL; 
    return (PyObject*) self;
}

int i8080C_init(PyObject *self, PyObject *args, PyObject *kwds){
    #ifdef DEBUG
    std::cout << "i8080C_init() called!" << std::endl;
    #endif
    i8080CObject* m = (i8080CObject*)self;

    m->m_value = 123;
    m->m_i8080C = (i8080C*)PyObject_Malloc(sizeof(i8080C));

    if(!m->m_i8080C){
        PyErr_SetString(PyExc_RuntimeError, "Memory allocation failed");
        return -1;
    }

    try {
        new (m->m_i8080C) i8080C();
    } catch (const std::exception& ex) {
        PyObject_Free(m->m_i8080C);
        m->m_i8080C = NULL;
        m->m_value   = 0;
        PyErr_SetString(PyExc_RuntimeError, ex.what());
        return -1;
    } catch(...) {
        PyObject_Free(m->m_i8080C);
        m->m_i8080C = NULL;
        m->m_value   = 0;
        PyErr_SetString(PyExc_RuntimeError, "Initialization failed");
        return -1;
    }

    return 0;
}

void i8080C_dealloc(i8080CObject *self){
    #ifdef DEBUG
    std::cout << "i8080C_dealloc() called!" << std::endl;
    #endif
    PyTypeObject *tp = Py_TYPE(self);

    i8080CObject* m = reinterpret_cast<i8080CObject*>(self);

    if(m->m_i8080C){
        m->m_i8080C->~i8080C();
        PyObject_Free(m->m_i8080C);
    }

    tp->tp_free(self);
    Py_DECREF(tp);
};

PyObject* i8080C_test(PyObject *self, PyObject *args){
    assert(self);

    i8080CObject* _self = reinterpret_cast<i8080CObject*>(self);

    Py_RETURN_NONE;
}

#define get_register(getter) \
    assert(self);\
    i8080CObject* _self = reinterpret_cast<i8080CObject*>(self);\
    return Py_BuildValue("i", _self->m_i8080C->getter());\

#define set_register(setter) \
    assert(self);\
    uint64_t val = PyLong_AsUnsignedLong(value);\
    if (PyErr_Occurred()) {\
        return -1;\
    }\
	if (val > 0xFF){\
		PyErr_SetString(PyExc_ValueError, "Value out of range");\
		return -1;\
	}\
    i8080CObject* _self = reinterpret_cast<i8080CObject*>(self);\
    _self->m_i8080C->setter(val);\
    return 0;\

PyObject *i8080C_getA(i8080CObject *self, void *closure){
    get_register(getA)
}

int i8080C_setA(i8080CObject *self, PyObject *value, void *closure){
    set_register(setA)
}

PyObject *i8080C_getB(i8080CObject *self, void *closure){
    get_register(getB)
}

int i8080C_setB(i8080CObject *self, PyObject *value, void *closure){
    set_register(setB)
}

PyObject *i8080C_getC(i8080CObject *self, void *closure){
    get_register(getC)
}

int i8080C_setC(i8080CObject *self, PyObject *value, void *closure){
    set_register(setC)
}

PyObject *i8080C_getD(i8080CObject *self, void *closure){
    get_register(getD)
}

int i8080C_setD(i8080CObject *self, PyObject *value, void *closure){
    set_register(setD)
}

PyObject *i8080C_getE(i8080CObject *self, void *closure){
    get_register(getE)
}

int i8080C_setE(i8080CObject *self, PyObject *value, void *closure){
    set_register(setE)
}

PyObject *i8080C_getH(i8080CObject *self, void *closure){
    get_register(getH)
}

int i8080C_setH(i8080CObject *self, PyObject *value, void *closure){
    set_register(setH)
}

PyObject *i8080C_getL(i8080CObject *self, void *closure){
    get_register(getL)
}

int i8080C_setL(i8080CObject *self, PyObject *value, void *closure){
    set_register(setL)
}