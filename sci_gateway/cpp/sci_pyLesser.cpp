#include "function.hxx"
#include "alltypes.hxx"
#include "PyVar.hxx"


extern "C" {
#include "Scierror.h"
#include "localization.h"
#include "PythonInstance.h"
}

using namespace types;

types::Function::ReturnValue sci_pyLesser(types::typed_list& in, int _iRetCount, types::typed_list& out) {
    if (!Py_IsInitialized()) {
        Scierror(999, "pyLesser: No instance of python is currently running! Did you forget to call startPy?");
        return types::Function::Error;
    }

    if (_iRetCount > 1) {
        Scierror(999, "pyLesser: Wrong number of output arguements, 1 expected");
        return types::Function::Error;
    }

    if (in.size() != 2) {
        Scierror(999, "pyLesser: Wrong number of input arguements, 2 expected");
        return types::Function::Error;
    }

    PyVar pIn1 = PyVar(in[0]);
    PyVar pIn2 = PyVar(in[1]);

    PyObject *result = PyObject_CallMethod(pIn1.get(), "__lt__", "(O)", pIn2.get());
    if (!PyBool_Check(result)) {
        PyErr_Clear();
        Scierror(999, "pyLesser: Incompatible types for operation");
        return types::Function::Error;
    } else if (PyObject_IsTrue(result)) {
        out.push_back(new Bool(true));
    } else {
        out.push_back(new Bool(false));
    }

    Py_DECREF(result);
    return types::Function::OK;
}