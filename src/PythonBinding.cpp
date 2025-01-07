
#include "PythonBinding.h"

#include "generalHelpers.h"
#pragma push_macro("slots")
#undef slots
#include "Python.h"
#pragma pop_macro("slots")
using namespace zPyInt;

int PythonBinding::m_objCounter=0;

QVector<PyMethodDef> PythonBinding::CPPFUNCMethods;

PyModuleDef PythonBinding::CPPFUNCModule = {};

PythonBinding::PythonBinding()
{
}

PythonBinding::~PythonBinding()
{
    Py_DECREF(m_pModule);
    --m_objCounter;
    if(m_objCounter == 0){
        Py_Finalize();
    }
}

bool PythonBinding::init(QString p_module,QString p_path)
{
    bool retVal=false;
    if(m_objCounter == 0){
        if(CPPFUNCMethods.size() != 0){
            PythonBinding::CPPFUNCModule = {
                PyModuleDef_HEAD_INIT, "CPPFUNC", NULL, -1, PythonBinding::CPPFUNCMethods.data(),
                NULL, NULL, NULL, NULL
            };
            PyImport_AppendInittab("CPPFUNC", &PyInit_emb);
        }
        Py_Initialize();
        QString qs = QDir::currentPath();
        std::wstring ws = qs.toStdWString();
        PyObject *sys = PyImport_ImportModule("sys");
        PyObject *path = PyObject_GetAttrString(sys, "path");
        PyList_Append(path, PyUnicode_FromString(qs.toUtf8()));
        ++m_objCounter;
    }
    if(!p_path.isEmpty()){
        PyObject *sys = PyImport_ImportModule("sys");
        PyObject *path = PyObject_GetAttrString(sys, "path");
        PyList_Append(path, PyUnicode_FromString(p_path.toUtf8()));
    }


    m_pName = PyUnicode_DecodeFSDefault(p_module.toUtf8());
    m_pModule = PyImport_Import(m_pName);
    if (m_pModule !=  NULL){
        retVal = true;
    }
    Py_DECREF(m_pName);


    PyEval_InitThreads();
    PyEval_SaveThread();


    return retVal;
}

PyObject* PythonBinding::callFunction(QString p_name,QList<PyObject*> p_args)
{
    int i=0;
    int argSize = p_args.size();
    if (m_pModule != NULL) {
        m_pFunc = PyObject_GetAttrString(m_pModule, p_name.toUtf8());

        if (m_pFunc && PyCallable_Check(m_pFunc)) {
            m_pArgs = PyTuple_New(argSize);
            for (i = 0; i < argSize ; ++i) {
                m_pValue=p_args.at(i);
                //m_pValue = PyLong_FromLong(5);
                if (!m_pValue) {
                    Py_DECREF(m_pArgs);
                    fprintf(stderr, "Cannot convert argument\n");
                    return nullptr;
                }
                /* pValue reference stolen here: */
                PyTuple_SetItem(m_pArgs, i, m_pValue);
            }
            m_pValue = PyObject_CallObject(m_pFunc, m_pArgs);
            Py_DECREF(m_pArgs);
            if (m_pValue != NULL) {
                //printf("Result of call: %ld\n", PyLong_AsLong(m_pValue));
                Py_DECREF(m_pValue);
                return m_pValue;
            }
            else {
                Py_DECREF(m_pFunc);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return nullptr;
            }
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            qout << "Cannot find function" <<  p_name << endl;
        }
        Py_XDECREF(m_pFunc);
    }else{
        qout << "Module not available" << endl;
    }


    return nullptr;
}

PythonObject PythonBinding::createObject(const QString &p_name)
{
    PythonObject tmpPyP;
    tmpPyP.m_pClass = PyObject_GetAttrString(m_pModule, p_name.toUtf8());
    tmpPyP.m_pArgs  = Py_BuildValue("()");
    tmpPyP.m_pInst  = PyObject_CallObject(tmpPyP.m_pClass, tmpPyP.m_pArgs);
    Py_DECREF(tmpPyP.m_pClass);
    Py_DECREF(tmpPyP.m_pArgs);
    return tmpPyP;
}

PythonDictonary PythonBinding::createDictonary(const QString &p_name)
{
    PythonDictonary tmpPyDic;
    tmpPyDic.m_pDict = PyDict_New();
    if(tmpPyDic.m_pDict == NULL){
        //tmpPyDic=NULL;
    }else{
        Py_INCREF(tmpPyDic.m_pDict);
        if (PyModule_AddObject(m_pModule,p_name.toUtf8() , tmpPyDic.m_pDict) < 0){
        Py_DECREF(m_pModule);
        Py_XDECREF(tmpPyDic.m_pDict);
        }
        tmpPyDic.m_name=p_name;
    }
    return tmpPyDic;

}

bool PythonBinding::AddFunction(const char* name, PyObject *(*func)(PyObject *, PyObject *))
{
    if(m_objCounter == 0){
        CPPFUNCMethods.prepend({name, func, METH_VARARGS,
                                "Call a Function provided by C++."});
        return true;
    }
    return false;
}

PyObject *PythonBinding::PyInit_emb()
{

    return PyModule_Create(&PythonBinding::CPPFUNCModule);

}


