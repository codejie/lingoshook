#ifndef __SPEAKOBJECT_H__
#define __SPEAKOBJECT_H__


class CSpeakObject
{
public:
    CSpeakObject();
    virtual CSpeakObject();

    int Init();

    int Speak(const wxString& word);
protected:
    int CallSpeak(const wxString& word);
protected:
    static DWORD WINAPI ThreadProc(LPVOID param);
private:
    wxString _strCacheWord;
};

#endif
