#include "Python.h"
#include "_i8080_module.h"
#include "_i8080_object.h"

#define DEBUG


/* ---------- 
i8080 Object methods
----------  */


/* ----------
get register
----------  */
static PyObject *
i8080o_get_reg(i8080oObject *self, PyObject *args)
{
    // return the value of the register
    char *reg;
    if (!PyArg_ParseTuple(args, "s", &reg)){
        PyErr_SetString(PyExc_Exception, "Parse error");
        return NULL;
    }

    if (strcmp(reg, "a") == 0)
        return Py_BuildValue("i", self->A);
    else if (strcmp(reg, "b") == 0)
        return Py_BuildValue("i", self->B);
    else if (strcmp(reg, "c") == 0)
        return Py_BuildValue("i", self->C);
    else if (strcmp(reg, "d") == 0)
        return Py_BuildValue("i", self->D);
    else if (strcmp(reg, "e") == 0)
        return Py_BuildValue("i", self->E);
    else if (strcmp(reg, "h") == 0)
        return Py_BuildValue("i", self->H);
    else if (strcmp(reg, "l") == 0)
        return Py_BuildValue("i", self->L);
    else if (strcmp(reg, "pc") == 0)
        return Py_BuildValue("i", self->PC);
    else if (strcmp(reg, "sp") == 0)
        return Py_BuildValue("i", self->SP);
    else if (strcmp(reg, "cc") == 0)
        return Py_BuildValue("i", self->CC);
    else {
        // https://docs.python.org/3/c-api/exceptions.html#standard-exceptions
        PyErr_SetString(PyExc_LookupError, "Invalid register");
        return NULL;
    }

}

/* ----------
set register
----------  */
static PyObject *
i8080o_set_reg(i8080oObject *self, PyObject *args, PyObject *keywds)
{
    // return the value of the register
    char *reg;
    uint16_t val;

    static char *kwlist[] = {"register", "value", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "sH", kwlist, &reg, &val)){
        PyErr_SetString(PyExc_Exception, "Parse error");
        return NULL;
    }

    // print the value
    printf("Setting register %s to %d", reg, val);


    if (strcmp(reg, "a") == 0){
        // check for overflow
        if (val > 0xFF){
            goto overflow;
        }
        self->A = val;
        return Py_BuildValue("i", self->A);
    }
    else if (strcmp(reg, "b") == 0){
        if (val > 0xFF){
            goto overflow;
        }
        self->B = val;
        return Py_BuildValue("i", self->B);
    }
    else if (strcmp(reg, "c") == 0){
        if (val > 0xFF){
            goto overflow;
        }
        self->C = val;
        return Py_BuildValue("i", self->C);
    }
    else if (strcmp(reg, "d") == 0){
        if (val > 0xFF){
            goto overflow;
        }
        self->D = val;
        return Py_BuildValue("i", self->D);
    }
    else if (strcmp(reg, "e") == 0){
        if (val > 0xFF){
            goto overflow;
        }
        self->E = val;
        return Py_BuildValue("i", self->E);
    }
    else if (strcmp(reg, "h") == 0){
        if (val > 0xFF){
            goto overflow;
        }
        self->H = val;
        return Py_BuildValue("i", self->H);
    }
    else if (strcmp(reg, "l") == 0){
        if (val > 0xFF){
            goto overflow;
        }
        self->L = val;
        return Py_BuildValue("i", self->L);
    }
    else if (strcmp(reg, "pc") == 0){
        self->PC = val;
        return Py_BuildValue("i", self->PC);
    }
    else if (strcmp(reg, "sp") == 0){
        self->SP = val;
        return Py_BuildValue("i", self->SP);
    }
    else if (strcmp(reg, "cc") == 0){
        if (val > 0xFF){
            goto overflow;
        }
        memcpy(&self->CC, &val, sizeof(ConditionCodes));
        return Py_BuildValue("i", self->CC);
    }
    else {
        // https://docs.python.org/3/c-api/exceptions.html#standard-exceptions
        PyErr_SetString(PyExc_LookupError, "Invalid register");
        return NULL;
    }

overflow:
        PyErr_SetString(PyExc_OverflowError, "Value too large");
        return NULL;
}

static PyObject *
i8080o_load_rom(i8080oObject *self, PyObject *args)
{
    char *file_path;
    if (!PyArg_ParseTuple(args, "s", &file_path)){
        PyErr_SetString(PyExc_Exception, "Parse error");
        return NULL;
    }

    FILE *f= fopen(file_path, "rb");    
    if (f==NULL){    
        PyErr_SetString(PyExc_FileNotFoundError, "Could not open file");
        return NULL;
    }

    fseek(f, 0L, SEEK_END);    
    uint32_t size = ftell(f);    
    fseek(f, 0L, SEEK_SET);

    self->rom_data = malloc(size);
    self->rom_size = size;

    fread(self->rom_data, size, 1, f);    
    fclose(f);

    return Py_BuildValue("i", size);
}


static PyObject *
i8080o_read_rom(i8080oObject *self, PyObject *args)
{
    uint32_t pos;
    if (!PyArg_ParseTuple(args, "I", &pos)){
        PyErr_SetString(PyExc_Exception, "Parse error");
        return NULL;
    }

    if (pos >= self->rom_size){
        PyErr_SetString(PyExc_IndexError, "Out of bounds");
        return NULL;
    }

    return Py_BuildValue("i", self->rom_data[pos]);
}

/* ---------- 
i8080 Object initialization
----------  */

//static i8080oObject *newi8080oObject();

// Initialize the i8080 object and set the default values
i8080oObject *
newi8080oObject(PyObject *arg)
{
    #ifdef DEBUG
    printf("Initialising i8080 object\n");
    #endif
    i8080oObject *self;
    self = PyObject_New(i8080oObject, &i8080o_Type);
    if (self == NULL)
        return NULL;
    self->x_attr = NULL;

    // Set the default values
    self->A = 0;
    self->B = 0;
    self->C = 0;
    self->D = 0;
    self->E = 0;
    self->H = 0;
    self->L = 0;
    self->PC = 0;
    self->SP = 0;
    memset(&self->CC, 0, sizeof(ConditionCodes));
    self->rom_data = NULL;

    return self;
}

// deallocate memory method
static void
i8080o_dealloc(i8080oObject *self)
{
    #ifdef DEBUG
    printf("Deallocating memory\n");
    #endif
    // free the memory
    if(self->rom_data != NULL){
        free(self->rom_data);
    }

    Py_XDECREF(self->x_attr);

    PyObject_Free(self);
}


static PyMethodDef i8080o_methods[] = {
    {"get_reg",                (PyCFunction)i8080o_get_reg,                             METH_VARARGS,                   PyDoc_STR("get register A")},
    {"set_reg",                 (PyCFunction)(void(*)(void))i8080o_set_reg,            METH_VARARGS | METH_KEYWORDS, PyDoc_STR("set register")},
    {"load_rom",               (PyCFunction)i8080o_load_rom,                            METH_VARARGS,                   PyDoc_STR("load rom")},
    {"read_rom",               (PyCFunction)i8080o_read_rom,                            METH_VARARGS,                   PyDoc_STR("read rom")},
    {NULL,              NULL}           /* sentinel */
};

// i8080oObject basic methods
// https://docs.python.org/3/c-api/typeobj.html
PyTypeObject i8080o_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "_i8080.i8080oObject",
    .tp_basicsize = sizeof(i8080oObject),
    .tp_dealloc = (destructor)i8080o_dealloc,
    //.tp_getattr = (getattrfunc)0,
    //.tp_setattr = (setattrfunc)AVRo_setattr,
    //.tp_getattro = (getattrofunc)AVRo_getattro,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_methods = i8080o_methods,
};