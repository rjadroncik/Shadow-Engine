#pragma once

#ifdef SECORE_EXPORTS
#define SECORE_API __declspec(dllexport)
#else
#define SECORE_API __declspec(dllimport)
#endif

#include <SCFObjectExtensions.h>
#include <SCFScripting.h>
#include <SCFXML.h>

#include "../Errors.h"

namespace System
{
    enum Namespace 
    {
        Resources,
        Rendering,
        Simulation,
        GUI
    };

    enum Category
    {
        Cameras,
        Forces,
        GPUPrograms,
        Lights,
        Materials,
        Meshes,
        Models,
        ParticleSystems,
        Scenes,
        Textures,

        Controls,
        Cursors,
        Carrets,
        Fonts,
        Layouts,
        GUIModules,
        Windows
    };

    class SECORE_API CNamespace : public CObject
    {
    public:
        Namespace Value() _GET { return m_eValue; }
        CString Name() _GET { return STRINGREF(m_Name); }

    public:
        CNamespace(_IN Namespace eValue, _IN CString name)
            : m_eValue(eValue), m_Name(name) {}

    private:
        Namespace m_eValue;
        CString m_Name;
    };

    class SECORE_API CCategory : public CObject
    {
    public:
        Category Value() _GET { return m_eValue; }
        CString Name() _GET { return STRINGREF(m_Name); }

    public:
        const CNamespace& Namespace() _GET { return m_Namespace; }

    public:
        CCategory(_IN Category eValue, _IN CString name, _IN _REF CNamespace& rNamespace) 
            : m_eValue(eValue), m_Name(name), m_Namespace(rNamespace) {}

    private: 
        Category m_eValue;
        CString m_Name;

        const CNamespace& m_Namespace;
    };

    #define CREATE_CATEGORY(eCategory, eNamespace) *(new CCategory(Category::eCategory, STRING(#eCategory), eNamespace))
    #define CREATE_NAMESPACE(eNamespace) *(new CNamespace(Namespace::eNamespace, STRING(#eNamespace)))

    class SECORE_API CSEObject : public SCFXML::CXMLObjectSerializable
    {
    public:
        void    ID(_IN CString& rID) _SET { m_ID = rID; }
        CString ID()                 _GET { return m_ID; }

    public:
        virtual Category Category() _GET = 0;

    protected:
        CSEObject() {};
        virtual ~CSEObject() {};

    protected:
        CString m_ID;
    };
};
