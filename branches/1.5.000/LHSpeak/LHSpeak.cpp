#include <tchar.h>
#include <sapi.h>

#include "LHSpeak.h"


BOOL WINAPI DllMain(HINSTANCE hinst, DWORD reason, LPVOID reserved)
{
	switch(reason)
	{
	case DLL_PROCESS_ATTACH:
//        Init();
		break;
	case DLL_PROCESS_DETACH:
//        Final();
		break;
	default:;
	}
    return TRUE;
}

BOOL Init()
{
    if(FAILED(CoInitialize(NULL)))
    {
        return FALSE;
    }
    return TRUE;
}

VOID Final()
{
    CoUninitialize();
}

BOOL Speak(LPCTSTR word)
{
    ISpVoice * pSpeak = NULL;

    HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pSpeak);
    if(FAILED(hr))
    {
        return FALSE;
    }

    pSpeak->Speak(word, 0, NULL);

    pSpeak->Release();
    pSpeak = NULL;
    
    return TRUE;
}

