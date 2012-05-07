/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#ifndef __LINGOSHOOK_H__
#define __LINGOSHOOK_H__

#include <windows.h>

#define DllExport __declspec(dllexport)

#if defined(__cplusplus)
extern "C"
{
#endif

DllExport BOOL CreateHookThread(HWND hwnd, LPCTSTR classname, LPCTSTR wintitle, UINT param, UINT delay, UINT* msgid);
DllExport BOOL CreateHookThreadByHWND(HWND hwnd, HWND hwndLingoes, UINT param, UINT delay, UINT* msgid);
DllExport BOOL RemoveHookThread();

#if defined(__cplusplus)
}
#endif

#define HOOKPARAM_HOOKCOPYDATA  1

#define SIZE_LINGOSCD       536
#define SIZE_LINGOSCD_DATA  256

typedef struct _LingosCD_t
{
	DWORD _unkwn1;
	DWORD _unkwn2;
	DWORD _unkwn3;
	DWORD _unkwn4;
	DWORD _size;
	DWORD _unkwn5;
	wchar_t _data[SIZE_LINGOSCD_DATA];
} LingosCD;

typedef enum _HookDataType_t
{ 
	HKT_CATCH = 0, 
	HKT_RESULT_TEXT,
	HKT_RESULT_HTML
} HookDataType;

typedef struct _HookData_t
{
	UINT size;
	wchar_t* data;
} HookData;

#define LINGOES_HANDLE_LONG			_T("Afx:400000:0")
#define LINGOES_HANDLE_SHORT		_T("Afx:400000")
#define LINGOES_TITLE_ENG			_T("Lingoes")
#define LINGOES_TITLE_CHN			_T("Lingoes ¡È∏ÒÀπ")

/////////////

BOOL SetThread();
BOOL ClearThread();
BOOL SetHook(HWND hwnd);
BOOL ClearHook();

DWORD WINAPI ThreadProc(LPVOID param);
LRESULT CALLBACK HookProc(UINT code, WPARAM wparam, LPARAM lparam);

BOOL GetIEDocResult(HWND hwnd);

BOOL GetWordData(HWND hwnd, HINSTANCE hinst);
BOOL GetHTMLData(HWND hwnd, HINSTANCE hinst);

//BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lparam);

BOOL CALLBACK EnumIEChildProc(HWND hwnd, LPARAM lparam);
BOOL CALLBACK EnumEditChildProc(HWND hwnd, LPARAM lparam);


BOOL SendBSTRData(enum _HookDataType_t type, const BSTR& str);
BOOL SendCDData(enum _HookDataType_t type, const LingosCD* data);
BOOL SendWCharData(enum _HookDataType_t type, const wchar_t* data, size_t size);
///
extern HINSTANCE _hInst;
extern HANDLE _hThread;


#endif
