#define PY_SSIZE_T_CLEAN
// Python.h has all the required function definitions to manipulate the Python objects
#include <Python.h>

#if SIZEOF_PY_UHASH_T > 4
#define _PyHASH_XXPRIME_1 ((Py_uhash_t)11400714785074694791ULL)
#define _PyHASH_XXPRIME_2 ((Py_uhash_t)14029467366897019727ULL)
#define _PyHASH_XXPRIME_5 ((Py_uhash_t)2870177450012600261ULL)
#define _PyHASH_XXROTATE(x) ((x << 31) | (x >> 33))  /* Rotate left 31 bits */
#else
#define _PyHASH_XXPRIME_1 ((Py_uhash_t)2654435761UL)
#define _PyHASH_XXPRIME_2 ((Py_uhash_t)2246822519UL)
#define _PyHASH_XXPRIME_5 ((Py_uhash_t)374761393UL)
#define _PyHASH_XXROTATE(x) ((x << 13) | (x >> 19))  /* Rotate left 13 bits */
#endif

//This is the function that is called from the python code
static PyObject* cpathpy_tuplehash(PyObject* self, PyObject* args){

  PyObject * tupleObj;

  //The input arguments come as a tuple, we parse the args to get the various variables
  //In this case it's only one tuple variable, which will now be referenced by tupleObj
  if (! PyArg_ParseTuple( args, "O", &tupleObj))
    return NULL;

  // Length of the tuple
  Py_ssize_t i,len = PyTuple_Size(tupleObj);

  // Hash value if an error in the calculation occurs
  int errorhash  = 1546275796;

  // Initialization of the hash variable
  Py_uhash_t acc = _PyHASH_XXPRIME_5;

  //iterate over all the elements an generate the hash
  for(i = 0; i < len; i++){
    //get an element out of the list - the element is also a python objects
    PyObject* temp = PyTuple_GetItem(tupleObj, i);
    //we know that object represents an integer - so convert it into C long
    Py_uhash_t lane = PyObject_Hash(temp);
    if (lane == (Py_uhash_t)-1) {
      return NULL;
    }
    acc += lane * _PyHASH_XXPRIME_2;
    acc = _PyHASH_XXROTATE(acc);
    acc *= _PyHASH_XXPRIME_1;
  }

  /* Add input length, mangled to keep the historical value of hash(()). */
  acc += len ^ (_PyHASH_XXPRIME_5 ^ 3527539UL);

  if (acc == (Py_uhash_t)-1) {
    return Py_BuildValue("n", errorhash);
  }
  // Return a Python Object with the hash as undirected long int
  return Py_BuildValue("n", acc);
}

//This is the docstring that corresponds to our 'tuplehash' function.
static char tuplehash_docs[] =
    "tuplehash( ): calculate the hash value of a tuple\n";

/* This table contains the relavent info mapping -
  <function-name in python module>, <actual-function>,
  <type-of-args the function expects>, <docstring associated with the function>
*/
static PyMethodDef cpathpy_funcs[] = {
    {"tuplehash", (PyCFunction)cpathpy_tuplehash, METH_VARARGS, tuplehash_docs},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef cpathpy = {
    PyModuleDef_HEAD_INIT,
    "cpathpy",                             /* name of module */
    "Experimental C functions for pathpy", /* module documentation, may be NULL */
    -1, /* size of per-interpreter state of the module,
           or -1 if the module keeps state in global variables. */
    cpathpy_funcs
};

/*
addList is the module name, and this is the initialization block of the module.
<desired module name>, <the-info-table>, <module's-docstring>
*/
PyMODINIT_FUNC PyInit_cpathpy(void){
  return PyModule_Create(&cpathpy);
}
