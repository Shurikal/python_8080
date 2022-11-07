#include "Python.h"
#include "_i8080.h"


static PyTypeObject i8080o_Type;


// Initialize the i8080 object and set the default values
static i8080oObject *
newi8080oObject(PyObject *arg)
{
    i8080oObject *self;
    self = PyObject_New(i8080oObject, &i8080o_Type);
    if (self == NULL)
        return NULL;
    self->x_attr = NULL;

    // set SREG to 0
    self->sreg = 0;

    return self;
}

// deallocate memory
static void
i8080o_dealloc(i8080oObject *self)
{
    Py_XDECREF(self->x_attr);
    PyObject_Free(self);
}

// https://docs.python.org/3/c-api/typeobj.html
static PyTypeObject i8080o_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "_i8080.i8080oObject",
    .tp_basicsize = sizeof(i8080oObject),
    .tp_dealloc = (destructor)i8080o_dealloc,
    //.tp_getattr = (getattrfunc)0,
    //.tp_setattr = (setattrfunc)AVRo_setattr,
    //.tp_getattro = (getattrofunc)AVRo_getattro,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    //.tp_methods = AVRo_methods,
};


/* --------------------------------------------------------------------- */

/* Function of no arguments returning new AVRo object */

static PyObject *
i8080_new(PyObject *self, PyObject *args)
{
    i8080oObject *rv;

    if (!PyArg_ParseTuple(args, ":new"))
        return NULL;
    rv = newi8080oObject(args);
    if (rv == NULL)
        return NULL;
    return (PyObject *)rv;
}



/* ---------- */


/* List of functions defined in the module */

// https://docs.python.org/3/c-api/structures.html?highlight=pymethoddef#c.PyMethodDef
static PyMethodDef i8080_methods[] = {
    {"new",             i8080_new,         METH_VARARGS,           PyDoc_STR("new() -> new i8080 object")},
    {NULL,              NULL}           /* sentinel */
};


PyDoc_STRVAR(module_doc,
"This module is for emulating the Intel 8080 CPU.");


// Slot initialization
static int64_t
i8080_exec(PyObject *m)
{
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