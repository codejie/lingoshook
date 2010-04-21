#ifndef __SPEAKOBJECT_H__
#define __SPEAKOBJECT_H__

//void Lock()
#include "wx/wx.h"
#include "wx/thread.h"

class CSpeakObject
{
public:
    CSpeakObject();
    virtual ~CSpeakObject();

    int Init();

    int Speak(const wxString& word);
protected:
    void Final();
protected:
    static DWORD WINAPI ThreadProc(LPVOID param);
private:
    static wxString _strCacheWord;
private:
    static HANDLE _hEvent[2];//exit and speak
    static HANDLE _hThread;
private:
    static wxMutex _stMutex;
};

#endif
