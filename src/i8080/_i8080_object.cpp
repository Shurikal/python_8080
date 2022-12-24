#include <Python.h>
#include "_i8080_module.hpp"
#include "_i8080_constants.hpp"
#include "_i8080_object.hpp"

i8080oClass::i8080oClass(){
    #ifdef DEBUG
    printf("Constructor called\n");
    #endif

    A = 0;
    B = 0;
    C = 0;
    D = 0;
    E = 0;
    H = 0;
    L = 0;
    SP = 0;
    PC = 0;
    CC.z = 0;
    CC.s = 0;
    CC.p = 0;
    CC.cy = 0;
    CC.ac = 0;
    CC.halt = 0;
    CC.int_enable = 0;
    CC.pad = 0;
    memory = new uint8_t[MEMORY_SIZE];
    for (int i = 0; i < MEMORY_SIZE; i++){
        memory[i] = 0;
    }
    for (int i = 0; i < 256; i++){
        IO[i] = 0;
    }
}

i8080oClass::~i8080oClass(){
    #ifdef DEBUG
    printf("Destroying i8080 object\n");
    #endif
    delete[] memory;
}

static void
i8080o_dealloc(i8080oObject *self)
{
    #ifdef DEBUG
    printf("Destroying i8080 object\n");
    #endif
    Py_TYPE(self)->tp_free((PyObject*)self);
}

void i8080oClass::test(){
    printf("Test function called");
}

PyObject *i8080o_reset(i8080oObject *self, PyObject *args){
    #ifdef DEBUG
    printf("Resetting i8080 object");
    #endif
}


PyObject*
i8080o_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    #ifdef DEBUG
    printf("Creating new i8080 object\n");
    #endif

    i8080oObject *self;
    self = (i8080oObject *)type->tp_alloc(type, 0);
    if (self != NULL) {
       self->x_attr = PyLong_FromLong(0);
       if (self->x_attr == NULL) {
           Py_DECREF(self);
           return NULL;
       }
    }

    i8080oClass::test();

    self->cpu = new i8080oClass();

    return (PyObject *)self;
}
