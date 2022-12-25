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

PyObject *i8080C_getvalue(i8080CObject *self, void *closure){
    return PyLong_FromLong(self->m_value);
}

int i8080C_setvalue(i8080CObject *self, PyObject *value, void *closure){
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