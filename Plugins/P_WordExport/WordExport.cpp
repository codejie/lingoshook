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
        DLLFinal();
        break;
	case DLL_PROCESS_DETACH:
        break;
    default:
        return FALSE;
    };

    return TRUE;
}

////

WordExport* _theActivity = NULL;