/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#ifndef __HOOKOBJECT_H__
#define __HOOKOBJECT_H__

#include "wx/wx.h"

#include "LingosHook.h"

class LingosHookFrame;
class CConfigData;

class CDllHookObject
{
public:
	CDllHookObject();
	virtual ~CDllHookObject();

    int Hook(HWND frame, HWND lgs, UINT param, UINT delay, UINT& msgid);
	int Unhook();
	
//	int MessageProc(WXUINT msg, WXWPARAM wparam, WXLPARAM lparam);
private:
	HINSTANCE _hDll;
	UINT _nMsgID;
};

class CHotkeyObject
{
public:
    CHotkeyObject();
    virtual ~CHotkeyObject();

    int Hook(HWND frame, HWND lgs, UINT mod, UINT key, UINT param, UINT& msgid);
    void Unhook();

    //int MessageProc(WXUINT msg, WXWPARAM wparam, WXLPARAM lparam);

    int GetResult(WXWPARAM wparam, WXLPARAM lparam);
protected:
    int GetIEDocResult(HWND hwnd);

    static int _wincount;
//    static BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lparam);

    BOOL GetWordData(HWND hwnd);
    BOOL GetHTMLData(HWND hwnd, HINSTANCE hinst);
    BOOL GetWordData(HWND hwnd, HINSTANCE hinst);

    static BOOL CALLBACK EnumIEChildProc(HWND hwnd, LPARAM lparam);
    static BOOL CALLBACK EnumEditChildProc(HWND hwnd, LPARAM lparam);   
    static BOOL CALLBACK EnumListChildProc(HWND hwnd, LPARAM lparam);

    int SendData(HookDataType type, const BSTR& str);
    int SendData(HookDataType type, const wchar_t* data, size_t size);	
private:
    HWND _hwndFrame;
    HWND _hwndLgs;
    int _id;
    UINT _nMsgID;
    UINT _nParam;
};


class CHookObject
{
private:
    enum HookStatus { HS_INIT = 0, HS_HOOK, HS_UNHOOK, HS_UNHOOK_USER };
public:
    CHookObject(LingosHookFrame* frame);
    virtual ~CHookObject();

    int Init(const CConfigData& conf);

    int SetHook();
    int SetUnhook();

    void SetStopRetrieve(bool stop);

	int MessageProc(WXUINT msg, WXWPARAM wparam, WXLPARAM lparam);

    static HWND GetLingoesHandle(int lang, bool strick = false);
	//static HWND GetLingoesHandle(int lang, bool strick, wchar_t*& buf, size_t bufsize);
protected:
    int Hook(HWND hwnd);
    int Unhook();
    void ShowInfo(const wxString& info);

    static DWORD WINAPI ThreadProc(LPVOID param);
    static int CheckLingoesStatus();
private:
    void Final();
    int CreateCheckThread();
    void ClearCheckThread();

	static int MatchClassName(const wchar_t* src, const wchar_t* tag, int totalLen, int skipPos, int skipLen);
private:
	LingosHookFrame* _objFrame;

    bool _bAutoHook;
    static int _iIfLanguage;
    bool _bStopRetrieve;
    bool _bOpenHotkey;
    bool _bHookCD;
    UINT _nDelay;
    UINT _nControlKey;
    UINT _nHotKey;
private:
    static HookStatus _eHookStatus;
    static HANDLE _hEvent[3];//exit, pause and continue;
    static HANDLE _hCheckThread;
    static HWND _hwndFrame;
    static UINT _nCheckMsgID;

    UINT _nHookMsgID;
    UINT _nHotkeyMsgID;
private:
    CDllHookObject _objHook;
    CHotkeyObject _objHotkey;
};


#endif
