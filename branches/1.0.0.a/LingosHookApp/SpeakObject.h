#ifndef __SPEAKOBJECT_H__
#define __SPEAKOBJECT_H__

#include <queue>
#include "wx/wx.h"
#include "wx/thread.h"

class CSpeakObject
{
protected:
    typedef std::deque<wxString> TCacheQueue;
public:
    CSpeakObject();
    virtual ~CSpeakObject();

    int Init();

    int Speak(const wxString& word);
protected:
    void Final();
protected:
    static DWORD WINAPI ThreadProc(LPVOID param);
    static int GetNextWord(wxString& word);
private:
    static TCacheQueue _queWord;
//    static wxString _strCacheWord;
private:
    static HANDLE _hEvent[2];//exit and speak
    static HANDLE _hThread;
private:
    static wxMutex _stMutex;
};

#endif
