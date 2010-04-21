
#include "LHSpeak.h"

#include "SpeakObject.h"


HANDLE CSpeakObject::_hEvent[2] = { NULL, NULL };
HANDLE CSpeakObject::_hThread = NULL;
wxString CSpeakObject::_strCacheWord = wxEmptyString;
wxMutex CSpeakObject::_stMutex;

CSpeakObject::CSpeakObject()
{
}

CSpeakObject::~CSpeakObject()
{
    Final();
}

int CSpeakObject::Init()
{
    _hEvent[0] = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    if(_hEvent[0] == NULL)
        return -1;
    _hEvent[1] = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    if(_hEvent[1] == NULL)
        return -1;

    DWORD id = 0;
    _hThread = ::CreateThread(NULL, 0, ThreadProc, NULL, 0, &id);
    if(_hThread == NULL)
        return -1;
        
    return 0;
}

void CSpeakObject::Final()
{
    if(_hEvent[0] != NULL)
        ::SetEvent(_hEvent[0]);

    ::Sleep(200);
}

DWORD CSpeakObject::ThreadProc(LPVOID param)
{
    if(::Init() != TRUE)
        return 0;

    bool run = true;
    while(run)
    {
        DWORD ret = ::WaitForMultipleObjects(2, _hEvent, FALSE, INFINITE);
        switch(ret)
        {
        case WAIT_OBJECT_0 + 0://exit
            run = false;
            break;
        case WAIT_OBJECT_0 + 1://speak
            {
                wxMutexLocker lock(_stMutex);
                ::Speak(_strCacheWord.c_str());
            }
            break;
        default:
            ::ExitThread(1);
        }
    }

	if(_hEvent[0] != NULL)
	{
		::CloseHandle(_hEvent[0]);
		_hEvent[0] = NULL;
	}
	if(_hEvent[1] != NULL)
	{
		::CloseHandle(_hEvent[1]);
		_hEvent[1] = NULL;
	}

    ::Final();

    ::CloseHandle(_hThread);
    _hThread = NULL;

    return 0;
}

int CSpeakObject::Speak(const wxString &word)
{
    wxMutexLocker lock(_stMutex);

    _strCacheWord = word;
    ::SetEvent(_hEvent[1]);
    return 0;
}