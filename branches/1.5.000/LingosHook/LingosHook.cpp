/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#include <mshtml.h>
#include <atlbase.h>
#include <oleacc.h>
#include <tchar.h>
#include <comutil.h>

#include "LingosHook.h"


#pragma data_seg(".JIE")

HWND _hWnd = NULL;
HHOOK _hHook = NULL;
UINT _nMsgID = 0;
HANDLE _hEvent[2] = { NULL, NULL };

UINT _nHookParam = 0;//0:none, 1:hook copy data
UINT _nDelayParam = 0;
//LingosCD _stLingosCD = LingosCD();
HWND _hLingoesHwnd = NULL;
#pragma data_seg()
#pragma comment(linker, "/section:.JIE,rws")

HINSTANCE _hInst = NULL;
HANDLE _hThread = NULL;

LPCTSTR HOOK_NOTIFY_MSG_DEF = _T("JIE_HOOK_NOTIFY_MSG_DEF");

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD reason, LPVOID reserved)
{
	switch(reason)
	{
	case DLL_PROCESS_ATTACH:
		_hInst = hinst;
		break;
	case DLL_PROCESS_DETACH:
		RemoveHookThread();
		break;
	default:;
	}
    return TRUE;
}


//

BOOL CreateHookThread(HWND hwnd, LPCTSTR classname, LPCTSTR wintitle, UINT hookparam, UINT delay, UINT* msgid)
{
	if(_hWnd != NULL)
		return FALSE;

	_hWnd = hwnd;

	LPCTSTR name = classname;
	if(name == NULL)
		name = LINGOES_HANDLE_LONG;
	LPCTSTR title = wintitle;
	if(title == NULL)
		title = LINGOES_TITLE_ENG;

	HWND h = ::FindWindow(name, title);
	if(h == NULL)
		return FALSE;

	_nMsgID = ::RegisterWindowMessage(HOOK_NOTIFY_MSG_DEF);
	if(_nMsgID == 0)
		return FALSE;
	*msgid = _nMsgID;

	if(SetThread() != TRUE)
		return FALSE;

	if(SetHook(h) != TRUE)
		return FALSE;

    _nHookParam = hookparam;
    _nDelayParam = delay;

	return TRUE;
}

BOOL CreateHookThreadByHWND(HWND hwnd, HWND hwndLingoes, UINT hookparam, UINT delay, UINT* msgid)
{
    if(hwnd == NULL || hwndLingoes == NULL)
        return FALSE;

	if(_hWnd != NULL)
		return FALSE;

	_hWnd = hwnd;

	_nMsgID = ::RegisterWindowMessage(HOOK_NOTIFY_MSG_DEF);
	if(_nMsgID == 0)
		return FALSE;
	*msgid = _nMsgID;

	if(SetThread() != TRUE)
		return FALSE;

	if(SetHook(hwndLingoes) != TRUE)
		return FALSE;

    _nHookParam = hookparam;
    _nDelayParam = delay;

    _hLingoesHwnd = hwndLingoes;
	return TRUE;
}

BOOL RemoveHookThread()
{
	if(ClearThread() != TRUE)
		return FALSE;

	if(ClearHook() != TRUE)
		return FALSE;

	_hWnd = NULL;

	return TRUE;
}

///

BOOL SetThread()
{
	_hEvent[0] = ::CreateEvent(NULL, FALSE, FALSE, _T("LingosDllCatchEvent"));
	if(_hEvent[0] == NULL)
		return FALSE;
	_hEvent[1] = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	if(_hEvent[1] == NULL)
		return FALSE;

	DWORD id = 0;
	_hThread = ::CreateThread(NULL, 0, ThreadProc, NULL, 0, &id);
	if(_hThread == NULL)
		return FALSE;

	return TRUE;
}

BOOL ClearThread()
{
	if(_hEvent[1] != NULL)
		::SetEvent(_hEvent[1]);

    ::Sleep(500);

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

	return TRUE;
}

BOOL SetHook(HWND hwnd)
{
	DWORD tid = ::GetWindowThreadProcessId(hwnd, &tid);
	if(tid == 0)
		return FALSE;
	
	_hHook = ::SetWindowsHookEx(WH_CALLWNDPROCRET, (HOOKPROC)HookProc, _hInst, tid);
	if(_hHook == NULL)
		return FALSE;

	return TRUE;
}

BOOL ClearHook()
{
	if(_hHook != NULL)
	{
		if(::UnhookWindowsHookEx(_hHook) != TRUE)
			return FALSE;
		_hHook = NULL;
	}
	return TRUE;
}

/////
DWORD WINAPI ThreadProc(LPVOID param)
{
	BOOL run = TRUE;
	int cnt = 0;
	while(run)
	{
		DWORD ret = ::WaitForMultipleObjects(2, _hEvent, FALSE, INFINITE);
		switch(ret)
		{
		case WAIT_OBJECT_0 + 0:
			{
                //int i = _stLingosCD._size;
                //const wchar_t* str = (const wchar_t*)_stLingosCD._data;
                ////i = Len;
                //if(_nHookParam == HOOKPARAM_HOOKCOPYDATA)
                //    SendCDData(HKT_CATCH, &_stLingosCD);

                wchar_t buf[64];
                size_t s = 0;
                HWND hwnd = ::FindWindowEx(NULL, NULL, NULL, NULL);
                while(hwnd != NULL)
                {
//                    wcsnset(buf, 0, 64);
                    ::GetClassName(hwnd, buf, 64);
                    s = wcslen(buf);
                    if(s > 17)
                    {
                        if(wcsncmp(buf, _T("Afx:400000:2400b:"), 17) == 0)
                        {
				            if(hwnd != NULL)
				            {
					            BOOL v = ::IsWindowVisible(hwnd);
					            while(v == FALSE && (++ cnt < 10))
					            {
						            ::Sleep(100);
						            v = ::IsWindowVisible(hwnd);
					            }
					            if(v == TRUE)
					            {
                                    if(_nDelayParam == 0)
                                    {
						                GetIEDocResult(hwnd);
                                    }
                                    else
                                    {
                                        cnt = 0;
                                        do
                                        {
                                            ::Sleep(200);
                                            if(::IsWindowVisible(hwnd) == FALSE)
                                            {
                                                break;
                                            }
                                        }while(++ cnt < (_nDelayParam / 200));

                                        if(cnt >= (_nDelayParam / 200))
                                        {
                                            GetIEDocResult(hwnd);
                                        }
                                    }
					            }
					            cnt = 0;					
				            }
                            break;
                        }
                    }

                    hwnd = ::FindWindowEx(NULL, hwnd, NULL, NULL);
                }

				//HWND hwnd = ::FindWindow(_T("Afx:400000:2400b:10011:0:0"), NULL);
				//if(hwnd != NULL)
				//{
				//	BOOL v = ::IsWindowVisible(hwnd);
				//	while(v == FALSE && (++ cnt < 10))
				//	{
				//		::Sleep(100);
				//		v = ::IsWindowVisible(hwnd);
				//	}
				//	if(v == TRUE)
				//	{
				//		GetIEDocResult(hwnd);
				//	}
				//	cnt = 0;					
				//}
			}
			break;
		case WAIT_OBJECT_0 + 1:
			{
				run = FALSE;
			}
			break;
		default:
			::ExitProcess(1);
		}
	}

	if(_hThread != NULL)
	{
		::CloseHandle(_hThread);
		_hThread = NULL;
	}
	
	return 0;
}

BOOL GetIEDocResult(HWND hwnd)
{
	::CoInitialize(NULL);

	HINSTANCE hinst = ::LoadLibrary( _T("OLEACC.DLL") );	// Explicitly load MSAA so we know if it's installed
	if(hinst != NULL)
	{
		if(hwnd != NULL)
		{
            if(_nHookParam == HOOKPARAM_HOOKCOPYDATA)
            {
                GetWordData(hwnd, hinst);
            }

            GetHTMLData(hwnd, hinst);
        }
		::FreeLibrary(hinst);
	}

	::CoUninitialize();

    return TRUE;
}

BOOL GetHTMLData(HWND hwnd, HINSTANCE hinst)
{
    HWND hc = NULL;
    ::EnumChildWindows(hwnd, EnumIEChildProc, (LPARAM)&hc);
    if(hc != NULL)
    {
	    CComPtr<IHTMLDocument2> cpDoc;
	    UINT msg = ::RegisterWindowMessage(_T("WM_HTML_GETOBJECT"));
	    LRESULT res = 0;
	    ::SendMessageTimeout(hc, msg, 0L, 0L, SMTO_ABORTIFHUNG, 1000, (PDWORD)&res);
	    LPFNOBJECTFROMLRESULT pfres = (LPFNOBJECTFROMLRESULT)::GetProcAddress(hinst, "ObjectFromLresult");
	    if(pfres != NULL)
	    {
		    HRESULT hr = (*pfres)(res, IID_IHTMLDocument, 0, (void**)&cpDoc);
		    if(SUCCEEDED(hr))
		    {
			    IDispatch* pdisp = NULL;
			    cpDoc->get_Script(&pdisp);
			    IHTMLWindow2* pwin = NULL;
			    pdisp->QueryInterface(IID_IHTMLWindow2, (void**)&pwin);
			    IHTMLDocument2* pdoc = NULL;
			    pwin->get_document(&pdoc);
			    IHTMLElement* pbody = NULL;
			    pdoc->get_body(&pbody);

			    BSTR html;
			    pbody->get_innerHTML(&html);
			    SendBSTRData(HKT_RESULT_HTML, html);
			    ::SysFreeString(html);

			    //BSTR text;
			    //pbody->get_innerText(&text);
			    //SendBSTRData(HKT_RESULT_TEXT, text);
			    //::SysFreeString(text);	

			    pbody->Release();
			    pdoc->Release();
			    pwin->Release();
			    pdisp->Release();
		    }
	    }
    }
    return TRUE;
}

BOOL CALLBACK EnumIEChildProc(HWND hwnd, LPARAM lparam)
{
	TCHAR buf[128];
	::GetClassName(hwnd, (LPWSTR)&buf, 128);
	if (::_tcscmp(buf, _T("Internet Explorer_Server")) == 0 )
	{
		*(HWND*)lparam = hwnd;
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

BOOL GetWordData(HWND hwnd, HINSTANCE hinst)
{
    HWND hc = NULL;
    ::EnumChildWindows(hwnd, EnumEditChildProc, (LPARAM)&hc);
    if(hc != NULL)
    {
        wchar_t buf[64];
        wmemset(buf, 0, 64);
        ::SendMessage(hc, WM_GETTEXT, 64, (LPARAM)buf);
        SendWCharData(HKT_CATCH, buf, wcslen(buf));
    }
    
    return TRUE;
}

BOOL CALLBACK EnumEditChildProc(HWND hwnd, LPARAM lparam)
{
	TCHAR buf[128];
	::GetClassName(hwnd, (LPWSTR)&buf, 128);
	if (::_tcscmp(buf, _T("Edit")) == 0 )
	{
		*(HWND*)lparam = hwnd;
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

LRESULT CALLBACK HookProc(UINT code, WPARAM wparam, LPARAM lparam)
{
	if(code < 0)
		return CallNextHookEx(_hHook, code, wparam, lparam);

	if(code == HC_ACTION)
	{
		PCWPRETSTRUCT pst = (PCWPRETSTRUCT)(lparam);

		if(pst->message == WM_COPYDATA)
		{
       //     if(_nHookParam == HOOKPARAM_HOOKCOPYDATA)
       //     {
			    //const PCOPYDATASTRUCT pcd = (const PCOPYDATASTRUCT)(pst->lParam);

       //         if(pcd->cbData == SIZE_LINGOSCD)
       //         {
       //             memcpy((void*)&_stLingosCD, (void*)pcd->lpData, pcd->cbData);
       //         }
       //         else
       //         {
       //             memset(&_stLingosCD, 0, sizeof(_stLingosCD));
       //         }
       //     }

			HANDLE event = ::OpenEvent(EVENT_ALL_ACCESS, FALSE, _T("LingosDllCatchEvent"));
			if(event != NULL)
				::SetEvent(event);
		}
	}
	return CallNextHookEx(_hHook, code, wparam, lparam);
}

BOOL SendBSTRData(enum _HookDataType_t type, const BSTR& str)
{
	size_t size = ::SysStringLen(str);
	struct _HookData_t* hd = new struct _HookData_t;

	hd->size = size + 1;
	hd->data = new wchar_t[hd->size];
	wcsncpy_s(hd->data, hd->size, str, size);

	::SendMessage(_hWnd, _nMsgID, (WPARAM)type, (LPARAM)hd);

	delete [] hd->data;
	delete hd;

	return TRUE;
}

BOOL SendCDData(enum _HookDataType_t type, const LingosCD* data)
{
    if(data == NULL || data->_size == 0)
        return FALSE;

	struct _HookData_t* hd = new struct _HookData_t;

    hd->size = data->_size + 1;
    hd->data = new wchar_t[hd->size];
    wcsncpy_s(hd->data, hd->size, (const wchar_t*)data->_data, data->_size);

	::SendMessage(_hWnd, _nMsgID, (WPARAM)type, (LPARAM)hd);

	delete [] hd->data;
	delete hd;

	return TRUE;
}

BOOL SendWCharData(enum _HookDataType_t type, const wchar_t* data, size_t size)
{
    if(data == NULL || size == 0)
        return FALSE;

	struct _HookData_t* hd = new struct _HookData_t;

    hd->size = size + 1;
    hd->data = new wchar_t[hd->size];
    wcsncpy_s(hd->data, hd->size, data, size);

	::SendMessage(_hWnd, _nMsgID, (WPARAM)type, (LPARAM)hd);

	delete [] hd->data;
	delete hd;

	return TRUE;
}