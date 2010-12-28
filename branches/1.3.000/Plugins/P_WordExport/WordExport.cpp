#include <windows.h>


#include "Exports.h"
#include "WordExport.h"

LINGOSHOOK_PLUGINS_API ActivityObject* GetActivity()
{
    _theActivity = new WordExport();
    return _theActivity;
}

int DLLInit()
{
    return 0;
}

void DLLFinal()
{
    if(_theActivity != NULL)
        delete _theActivity, _theActivity = NULL;
}

///////////
BOOL APIENTRY DllMain(HANDLE hModule, DWORD uCallReason, LPVOID pReserved)
{
    switch(uCallReason)
    {
	case DLL_PROCESS_ATTACH:
        if(DLLInit() != 0)
            return FALSE;
        break;
	case DLL_THREAD_ATTACH:
        break;
	case DLL_THREAD_DETACH:
        break;
	case DLL_PROCESS_DETACH:
        DLLFinal();
        break;
    default:
        return FALSE;
    };

    return TRUE;
}

////

WordExport* _theActivity = NULL;


WordExport::WordExport()
: ActivityObject()
{
}

WordExport::~WordExport()
{
}

int WordExport::LoadProperty(PropertyData& data) const
{
    data.m_Label = "WordExport";
    data.m_Name = "Word Export";
    data.m_Version = "0.1";
    data.m_Author = "codejie(codejie@gmail.com)";
    data.m_Description = "Export words from LingosHook to a Text file.";

    return 0;
}

int WordExport::Run()
{
    return -1;
}