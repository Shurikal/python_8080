#include "Python.h"
#include "_i8080_module.h"
#include "_i8080_object.h"
#include "_i8080_constants.h"
#include "_i8080_cpu.h"
#include "_i8080_memory.h"


/*
Methods to access the memory with the subscript operator
*/


static Py_ssize_t i8080oMemory_len(i8080oMemory* self) {
	return (Py_ssize_t) MEMORY_SIZE;
}

static PyObject* i8080oMemory_sq_item(i8080oMemory* self, Py_ssize_t index) {
	if (index >= MEMORY_SIZE) {
		PyErr_SetString(PyExc_IndexError, "Out of bounds\n");
		return NULL;
	}
	if (index < 0 && index >= -MEMORY_SIZE) {
		index += MEMORY_SIZE;
	}

	return Py_BuildValue("i", self->memory[index]);
}


static int i8080oMemory_sq_setitem(i8080oMemory* self, Py_ssize_t index, PyObject* value) {
	if (index >= MEMORY_SIZE) {
		PyErr_SetString(PyExc_IndexError, "Out of bounds\n");
		return NULL;
	}
	if (index < 0 && index >= -MEMORY_SIZE) {
		index += MEMORY_SIZE;
	}

	uint64_t val = PyLong_AsUnsignedLong(value);
    if (PyErr_Occurred()) {
        return -1;
    }
	if (val > 0xFF){
		PyErr_SetString(PyExc_ValueError, "Value out of range");
		return -1;
	}

	self->memory[index] = val;
	return 0;
}


static PySequenceMethods i8080oMemory_SeqMethods = {
	/* PySequenceMethods, implementing the sequence protocol
	 * references:
	 * https://docs.python.org/3/c-api/typeobj.html#c.PySequenceMethods
	 * https://docs.python.org/3/c-api/sequence.html
	 */
	(lenfunc)i8080oMemory_len, // sq_length
	0, // sq_concat
	0, // sq_repeat
	(ssizeargfunc)i8080oMemory_sq_item, // sq_item
	0,
	(ssizeobjargproc)i8080oMemory_sq_setitem, // sq_ass_item
	0,
	0, // sq_contains
	0, // sq_inplace_concat
	0, // sq_inplace_repeat
};

// Memory Management

static PyObject*
i80800Memory_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	#ifdef DEBUG
	printf("Creating new i8080 memory object\n");
	#endif
	i8080oMemory *self;
	self = (i8080oMemory *)type->tp_alloc(type, 0);
	if (self != NULL) {
		self->x_attr = PyLong_FromLong(0);
		if (self->x_attr == NULL) {
			Py_DECREF(self);
			return NULL;
		}
	}

	// Set the default values
    self->memory = malloc(MEMORY_SIZE);

    if (self->memory == NULL){
        PyErr_SetString(PyExc_MemoryError, "Could not allocate memory\n");
        return NULL;
    }

    memset(self->memory, 0, MEMORY_SIZE);

	return (PyObject *)self;
}


// deallocate memory method
static void
i8080oMemory_dealloc(i8080oMemory *self)
{
    #ifdef DEBUG
    printf("Deallocating i8080oMemory\n");
    #endif
    // free the memory
    if(self->memory != NULL){
        free(self->memory);
    }

    Py_XDECREF(self->x_attr);

    PyObject_Free(self);
}

// i8080oObject basic methods
// https://docs.python.org/3/c-api/typeobj.html
// not static because it is used in the module init function
PyTypeObject i8080oMemory_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "_i8080.i8080_Memory",
    .tp_basicsize = sizeof(i8080oMemory),
    .tp_dealloc = (destructor)i8080oMemory_dealloc,
	.tp_new = i80800Memory_new,
    //.tp_getattr = (getattrfunc)0,
    //.tp_setattr = (setattrfunc)AVRo_setattr,
    //.tp_getattro = (getattrofunc)AVRo_getattro,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    //.tp_methods = i8080o_methods,
	//.tp_getset = getsets,
	.tp_as_sequence = &i8080oMemory_SeqMethods,
//	.tp_members = i8080o_members,
};
