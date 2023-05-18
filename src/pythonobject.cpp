#include "pythonobject.h"
#include "PythonBinding.h"
#include "generalHelpers.h"

using namespace zPyInt;

PythonObject::PythonObject()
{

}

PythonObject::~PythonObject()
{
    Py_DECREF(m_pClass);
    Py_DECREF(m_pInst);

}

PyObject* PythonObject::callFunction(const QString &p_name, QList<PyObject *> p_args)
{
    int i=0;
    int argSize = p_args.size();
    if (m_pClass != NULL) {
        m_pFunc = PyObject_GetAttrString(m_pInst, p_name.toUtf8());

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
                return m_pValue;
            }
            else {
                Py_DECREF(m_pFunc);
                Py_DECREF(m_pClass);
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
        Py_DECREF(m_pClass);
    }else{
        qout << "Module not available" << endl;
    }


    return nullptr;
}

PyObject *PythonObject::getAttribute(const QString p_name)
{
    m_pFunc = PyObject_GetAttrString(m_pInst, p_name.toUtf8());
    if(m_pFunc == NULL){

        throw std::runtime_error(QString("Calling of undefined python variable ").append(p_name).append(" from C").toUtf8());
    }
    return m_pFunc;
}

void PythonObject::setAttribute(const QString &p_name, PyObject *p_obj)
{
    PyObject_SetAttrString(m_pInst, p_name.toUtf8(),p_obj);
}

PyObject *PythonObject::getPInst() const
{
    return m_pInst;
}

void PythonObject::setPInst(PyObject *pInst)
{
    m_pInst = pInst;
}

PythonObject &PythonObject::operator=(const PythonObject& obj)
{
    this->m_pInst=obj.m_pInst;
    this->m_pClass=obj.m_pInst;
    Py_INCREF(this->m_pClass);
    Py_INCREF(this->m_pInst);
    return *this;
}


