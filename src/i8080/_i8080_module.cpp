#include <iostream>
#include <Python.h>


PyDoc_STRVAR(module_doc, "This module is for emulating the Intel 8080 CPU.");

static PyMethodDef i8080_module_methods[] = {
	{NULL, NULL}
};




// Slot initialization
static int64_t
i8080_exec(PyObject *m)
{
    #ifdef DEBUG
    printf("i8080_exec\n");
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