#ifndef __EXPORTS_H__
#define __EXPORTS_H__

//http://wxforum.shadonet.com/viewtopic.php?t=13178&highlight=plugin

#include "wx/wx.h"
#include "wx/msw/private.h"

#define IMPLEMENT_PLUGIN(name)                                                              \
    extern "C" __declspec(dllexport) ActivityObject* GetActivity() { return new name; }     \
                                                                                            \
    class name##DLLApp : public wxApp                                                       \
    {                                                                                       \
    };                                                                                      \
    IMPLEMENT_APP_NO_MAIN(name##DLLApp)                                                     \
                                                                                            \
    BOOL APIENTRY DllMain(HANDLE hModule, DWORD uCallReason, LPVOID pReserved)              \
    {                                                                                       \
        switch(uCallReason)                                                                 \
        {                                                                                   \
	    case DLL_PROCESS_ATTACH:                                                            \
            {                                                                               \
                wxSetInstance((HINSTANCE)hModule);                                          \
                int argc = 0; char* argv = NULL;                                            \
                wxEntryStart(argc, &argv);                                                  \
                return TRUE;                                                                \
            }                                                                               \
            break;                                                                          \
	    case DLL_THREAD_ATTACH:                                                             \
            break;                                                                          \
	    case DLL_THREAD_DETACH:                                                             \
            break;                                                                          \
	    case DLL_PROCESS_DETACH:                                                            \
            wxEntryCleanup();	                                                            \
            break;                                                                          \
        default:                                                                            \
            return FALSE;                                                                   \
        };                                                                                  \
                                                                                            \
        return TRUE;                                                                        \
    }                                                                                       \
    

#endif
