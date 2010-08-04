/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#include <mshtml.h>
#include <atlbase.h>
#include <oleacc.h>
#include <tchar.h>
#include <comutil.h>
#include <commctrl.h>

//#include "wx/wfstream.h"
//#include "wx/txtstrm.h"

#include "LingosHookApp.h"
#include "ConfigData.h"
#include "HookObject.h"


typedef BOOL (*CreateHookThreadPtr)(HWND, LPCTSTR, LPCTSTR, UINT, UINT*);
typedef BOOL (*CreateHookThreadByHWNDPtr)(HWND, HWND, UINT, UINT, UINT*);
typedef BOOL (*RemoveHookThreadPtr)(void);


CDllHookObject::CDllHookObject()
: _hDll(NULL)
, _nMsgID(0)
{
}

CDllHookObject::~CDllHookObject()
{
	Unhook();
}

int CDllHookObject::Hook(HWND frame, HWND lgs, UINT param, UINT delay, UINT& msgid)
{
	if(frame == NULL || lgs == NULL)
		return -1;

	_hDll = ::LoadLibrary(_T("LingosHook.dll"));
	if(_hDll != NULL)
	{
		CreateHookThreadByHWNDPtr pch = (CreateHookThreadByHWNDPtr)GetProcAddress(_hDll, "CreateHookThreadByHWND");
		if(pch != NULL)
		{
			if(pch(frame, lgs, param, delay, &msgid) != TRUE)
			{
				wxLogDebug(_("call CreateHookThread() failed."));
				::FreeLibrary(_hDll);
				return -1;
			}
		}
		else
		{
			wxLogDebug(_("Get CreateHookThread address failed."));
			::FreeLibrary(_hDll);
			return -1;
		}
	}
	else
	{
		wxLogDebug(_("Load LingosHook dll failed."));
		return -1;
	}
	return 0;
}

int CDllHookObject::Unhook()
{
	if(_hDll != NULL)
	{
		RemoveHookThreadPtr prh = (RemoveHookThreadPtr)GetProcAddress(_hDll, "RemoveHookThread");
		if(prh != NULL)
		{
			if(prh() != TRUE)
			{
				wxLogDebug(_("call RemoveHookThread() failed."));
				return -1;
			}
		}
		else
		{
			wxLogDebug(_("Get RemoveHookThread address failed."));
		}
		::FreeLibrary(_hDll);
		_hDll = NULL;
	}
	return 0;
}

//int CDllHookObject::MessageProc(WXUINT msg, WXWPARAM wparam, WXLPARAM lparam)
//{
//	if(msg == _nMsgID)
//	{
//		const struct _HookData_t* hd = (reinterpret_cast<const struct _HookData_t*>(lparam));
//		wxString str;
//		if(hd != NULL && hd->data != NULL)
//		{
//			str.append(hd->data, hd->size);
//		}		
//
//		if(wparam == HKT_RESULT_TEXT)
//		{
//			wxLogDebug(_T("get RESULT_TEXT message."));
//			
//			if(_objFrame != NULL)
//				_objFrame->HookTextProc(str);
//		}
//		else if(wparam == HKT_RESULT_HTML)
//		{
//			wxLogDebug(_T("get RESULT_HTLM message."));
//			if(_objFrame != NULL)
//				_objFrame->HookHTMLProc(str);
//		}
//		else
//		{
//			wxLogDebug(_T("get UNKNOWN message."));
//		}
//	}
//	return 0;
//}
/////////////////////////////////////////////////////////
int CHotkeyObject::_wincount = 0;

CHotkeyObject::CHotkeyObject()
: _hwndFrame(NULL)
, _hwndLgs(NULL)
, _id(0)
, _nMsgID(0)
, _nParam(0)
{
}

CHotkeyObject::~CHotkeyObject()
{
    Unhook();
}

int CHotkeyObject::Hook(HWND frame, HWND lgs, UINT mod, UINT key, UINT param, UINT& msgid)
{
    _hwndFrame = frame;

    _nMsgID = ::RegisterWindowMessage(_T("JIE_HOOKOBJECT_HOTKEY_MSG_DEF"));

    if(_hwndFrame == NULL || _nMsgID == 0)
        return -1;
    
    _id = ::GlobalAddAtom(_T("LingosHook"));
    if(_id == 0)
        return -1;
    if(::RegisterHotKey(frame, _id, mod, key) != TRUE)
        return -1;

    _hwndLgs = lgs;
    msgid = _nMsgID;
    _nParam = param;

    return 0;
}

void CHotkeyObject::Unhook()
{
    if(_id != 0)
    {
        ::UnregisterHotKey(_hwndFrame, _id);
        _id = 0;
    }
}

//int CHotkeyObject::MessageProc(WXUINT msg, WXWPARAM wparam, WXLPARAM lparam)
//{
//    GetResult(wparam, lparam);
//
//    return 0;
//}

int CHotkeyObject::GetResult(WXWPARAM wparam, WXLPARAM lparam)
{
    //FindWindows
//    HWND hwnd = ::FindWindow(_T("Afx:400000:0"), _T("Lingoes"));
    if(_hwndLgs == NULL)
        return -1;
    if(::IsWindowVisible(_hwndLgs) == FALSE)
        return -1;

    if(GetIEDocResult(_hwndLgs) != TRUE)
        return -1;
    
    return 0;
}

BOOL CHotkeyObject::GetIEDocResult(HWND hwnd)
{
    if(hwnd == NULL)
        return FALSE;
    if(_nParam == 1)
    {
        if(GetWordData(hwnd) != TRUE)
            return FALSE;
    }

	::CoInitialize(NULL);

	HINSTANCE hinst = ::LoadLibrary( _T("OLEACC.DLL") );	// Explicitly load MSAA so we know if it's installed
	if(hinst != NULL)
	{
        GetWordData(hwnd, hinst);
        GetHTMLData(hwnd, hinst);
		::FreeLibrary(hinst);
	}
	::CoUninitialize();
	return TRUE;
}

BOOL CALLBACK CHotkeyObject::EnumIEChildProc(HWND hwnd, LPARAM lparam)
{
    ++ _wincount;

    if(_wincount == 0x24)
    {
		*(HWND*)lparam = hwnd;
        return FALSE;
    }
    return TRUE;
}

BOOL CHotkeyObject::GetHTMLData(HWND hwnd, HINSTANCE hinst)
{
    HWND hc = NULL;
    _wincount = 0;
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
	            SendData(HKT_RESULT_HTML, html);
			    ::SysFreeString(html);

			    //BSTR text;
			    //pbody->get_innerText(&text);
	            //_objFrame->HookTextProc(text);
			    //::SysFreeString(text);							
			    //
			    pbody->Release();
			    pdoc->Release();
			    pwin->Release();
			    pdisp->Release();
		    }
	    }
    }

	return TRUE;
}

BOOL CHotkeyObject::EnumEditChildProc(HWND hwnd, LPARAM lparam)
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

BOOL CHotkeyObject::EnumListChildProc(HWND hwnd, LPARAM lparam)
{
 //   HWND h = hwnd;
	//TCHAR bu[128];
	//::GetClassName(h, (LPWSTR)&bu, 128);

 //   int c = (int)::SendMessage(h, LB_GETCOUNT, (WPARAM)0, (LPARAM)0);
 //   if(c > 0)
 //   {
 //       return FALSE;
 //   }
 //   return TRUE;

	TCHAR buf[128];
	::GetClassName(hwnd, (LPWSTR)&buf, 128);
	if (::_tcscmp(buf, _T("Lingoes_ListKey")) == 0 )
	{
		*(HWND*)lparam = hwnd;
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

BOOL CHotkeyObject::GetWordData(HWND hwnd)
{
    HWND hedit = NULL, hlist = NULL;

    ::EnumChildWindows(hwnd, EnumEditChildProc, (LPARAM)&hedit);
    if(hedit == NULL)
        return FALSE;

    wchar_t buf[64];
    wmemset(buf, 0, 64);
    ::SendMessage(hedit, WM_GETTEXT, 64, (LPARAM)buf);

    size_t size = wcslen(buf);
    if(size == 0)
        return FALSE;

    SendData(HKT_CATCH, buf, size);

    //::EnumChildWindows(hwnd, EnumListChildProc, (LPARAM)&hlist);
    //if(hlist == NULL)
    //    return FALSE;
    //BOOL v = ::IsWindowVisible(hlist);
    //if(v)
    //{
    //    int pos = (int)::SendMessage(hlist, LB_GETCOUNT, (WPARAM)0, (LPARAM)0);
    //    int l = pos;
    //}

    return TRUE;
}

BOOL CHotkeyObject::GetWordData(HWND hwnd, HINSTANCE hinst)
{
    HWND hlist = NULL;
    ::EnumChildWindows(hwnd, EnumListChildProc, (LPARAM)&hlist);
    if(hlist == NULL)
        return FALSE;
    BOOL v = ::IsWindowVisible(hlist);
    if(v)
    {
	    CComPtr<IHTMLDocument2> cpDoc;
	    UINT msg = ::RegisterWindowMessage(_T("WM_HTML_GETOBJECT"));
	    LRESULT res = 0;
	    ::SendMessageTimeout(hlist, msg, 0L, 0L, SMTO_ABORTIFHUNG, 1000, (PDWORD)&res);
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
	            SendData(HKT_RESULT_HTML, html);
			    ::SysFreeString(html);

			    //BSTR text;
			    //pbody->get_innerText(&text);
	            //_objFrame->HookTextProc(text);
			    //::SysFreeString(text);							
			    //
			    pbody->Release();
			    pdoc->Release();
			    pwin->Release();
			    pdisp->Release();
		    }
	    }
    }

    return TRUE;
}


int CHotkeyObject::SendData(HookDataType type, const BSTR& str)
{
	size_t size = ::SysStringLen(str);
	struct _HookData_t* hd = new struct _HookData_t;

	hd->size = size + 1;
	hd->data = new wchar_t[hd->size];
	wcsncpy_s(hd->data, hd->size, str, size);

	::SendMessage(_hwndFrame, _nMsgID, (WPARAM)type, (LPARAM)hd);

	delete [] hd->data;
	delete hd;

	return 0;
}

int CHotkeyObject::SendData(HookDataType type, const wchar_t* data, size_t size)
{
    if(data == NULL || size == 0)
        return -1;

	struct _HookData_t* hd = new struct _HookData_t;

	hd->size = size + 1;
	hd->data = new wchar_t[hd->size];
	wcsncpy_s(hd->data, hd->size, data, size);

	::SendMessage(_hwndFrame, _nMsgID, (WPARAM)type, (LPARAM)hd);

	delete [] hd->data;
	delete hd;

	return 0;
}

//////////////////////////////////////////////////////////

int CHookObject::_iIfLanguage = 0;
UINT CHookObject::_nCheckMsgID = 0;
HWND CHookObject::_hwndFrame = NULL;
CHookObject::HookStatus CHookObject::_eHookStatus = CHookObject::HS_INIT;
HANDLE CHookObject::_hCheckThread = NULL;
HANDLE CHookObject::_hEvent[3] = { NULL, NULL, NULL };

CHookObject::CHookObject(LingosHookFrame *frame)
: _objFrame(frame)
{
}

CHookObject::~CHookObject()
{
    Final();
}

int CHookObject::Init(const CConfigData &conf)
{
    _bAutoHook = conf.m_iAutoHook == 1 ? true : false;
    _iIfLanguage = conf.m_iIfLanguage;
    _bOpenHotkey = conf.m_iOpenHotkey == 1 ? true : false;
    _bHookCD = (conf.m_iSkipError == 1 || (conf.m_iSkipDict == 1 && conf.m_iSkipHtml == 1));//  true;//(conf.m_iDataProcFlag == 2 || conf.m_iDataProcFlag == 3) ? true : false;
    _nDelay = conf.m_iRetrieveDelay;

    if(_bOpenHotkey)
    {
        _nControlKey = conf.GetContolKey();
        _nHotKey = conf.GetHotKey();
    }

    if(_objFrame == NULL)
        return -1;

    _hwndFrame = (HWND)_objFrame->GetHWND();

    _nCheckMsgID = ::RegisterWindowMessage(_T("JIE_HOOKOBJECT_CHECKTHREAD_MSG_DEF"));
    if(_nCheckMsgID == 0)
        return -1;

    if(CreateCheckThread() != 0)
        return -1;

    if(_bAutoHook != true)
    {
        ::SetEvent(_hEvent[1]);
    }
    return 0;
}

void CHookObject::Final()
{
    ClearCheckThread();
    Unhook();
}

int CHookObject::CreateCheckThread()
{
    _hEvent[0] = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    if(_hEvent[0] == NULL)
        return -1;
    _hEvent[1] = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    if(_hEvent[1] == NULL)
        return -1;
    _hEvent[2] = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    if(_hEvent[2] == NULL)
        return -1;

    DWORD id = 0;
    _hCheckThread = ::CreateThread(NULL, 0, ThreadProc, NULL, 0, &id);
    if(_hCheckThread == NULL)
        return -1;
    return 0;
}

DWORD CHookObject::ThreadProc(LPVOID param)
{
    bool run = true;
    while(run)
    {
        DWORD ret = ::WaitForMultipleObjects(3, _hEvent, FALSE, 500);
        switch(ret)
        {
        case WAIT_OBJECT_0 + 0://exit
            run = false;
            break;
        case WAIT_OBJECT_0 + 1://pause
            {
                bool r = true;
                while(r)
                {
                    ret = ::WaitForMultipleObjects(3, _hEvent, FALSE, INFINITE);
                    switch(ret)
                    {
                        case WAIT_OBJECT_0 + 0://exit
                            r = false;
                            run = false;
                            break;
                        case WAIT_OBJECT_0 + 1://pause
                            break;
                        case WAIT_OBJECT_0 + 2://cont
                            r = false;
                            break;
                        default:
                            ::ExitThread(1);
                    }
                }
            }
            break;
        case WAIT_OBJECT_0 + 2://continue
            break;
        case WAIT_TIMEOUT:
            {
                CheckLingoesStatus();
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

	if(_hEvent[2] != NULL)
	{
		::CloseHandle(_hEvent[2]);
		_hEvent[2] = NULL;
	}

    ::CloseHandle(_hCheckThread);
    _hCheckThread = NULL;

    return 0;
}

HWND CHookObject::GetLingoesHandle(int lang, bool strick)
{
    HWND hwnd = NULL;
    if(_iIfLanguage == 0)
    {
        hwnd = ::FindWindow(_T("Afx:400000:0"), _T("Lingoes"));
        if(hwnd == NULL)
            hwnd = ::FindWindow(_T("Afx:400000:0"), _T("Lingoes 灵格斯"));
    }
    else if(_iIfLanguage == 1)
    {
        hwnd = ::FindWindow(_T("Afx:400000:0"), _T("Lingoes"));
    }
    else
    {
        hwnd = ::FindWindow(_T("Afx:400000:0"), _T("Lingoes 灵格斯"));
    }
    
    wchar_t buf[64];
    if(strick == false)
    {
        if(_iIfLanguage == 0)
        {
            hwnd = ::FindWindowEx(NULL, NULL, NULL, _T("Lingoes"));
            while(hwnd != NULL)
            {
                ::GetClassName(hwnd, buf, 64);
                if(wcsncmp(buf, _T("Afx:400000"), 10) == 0)
                    return hwnd;
                hwnd = ::FindWindowEx(NULL, hwnd, NULL, _T("Lingoes"));
            }

            hwnd = ::FindWindowEx(NULL, NULL, NULL, _T("Lingoes 灵格斯"));
            while(hwnd != NULL)
            {
                ::GetClassName(hwnd, buf, 64);
                if(wcsncmp(buf, _T("Afx:400000"), 10) == 0)
                    return hwnd;
                hwnd = ::FindWindowEx(NULL, hwnd, NULL, _T("Lingoes"));
            }
        }
        else if(_iIfLanguage == 1)
        {
            hwnd = ::FindWindowEx(NULL, NULL, NULL, _T("Lingoes"));
            while(hwnd != NULL)
            {
                ::GetClassName(hwnd, buf, 64);
                if(wcsncmp(buf, _T("Afx:400000"), 10) == 0)
                    return hwnd;
                hwnd = ::FindWindowEx(NULL, hwnd, NULL, _T("Lingoes"));
            }
        }
        else
        {
            hwnd = ::FindWindowEx(NULL, NULL, NULL, _T("Lingoes 灵格斯"));
            while(hwnd != NULL)
            {
                ::GetClassName(hwnd, buf, 64);
                if(wcsncmp(buf, _T("Afx:400000"), 10) == 0)
                    return hwnd;
                hwnd = ::FindWindowEx(NULL, hwnd, NULL, _T("Lingoes"));
            }
        }        
    }
    return hwnd;
}

int CHookObject::CheckLingoesStatus()
{
    HWND hwnd = GetLingoesHandle(_iIfLanguage, false);
    if(hwnd == NULL)
    {
        if(_eHookStatus == HS_HOOK)
        {
            ::SendMessage(_hwndFrame, _nCheckMsgID, HS_UNHOOK, (LPARAM)(hwnd));
        }
    }
    else
    {
        if(_eHookStatus == HS_UNHOOK || _eHookStatus == HS_INIT)
        {
            ::SendMessage(_hwndFrame, _nCheckMsgID, HS_HOOK, (LPARAM)(hwnd));
        }
    }

    return 0;
}

void CHookObject::ClearCheckThread()
{
    if(_hEvent[0] != NULL)
        ::SetEvent(_hEvent[0]);

    ::Sleep(200);
}

int CHookObject::MessageProc(WXUINT msg, WXWPARAM wparam, WXLPARAM lparam)
{
    if(msg == _nHookMsgID)
    {
		const struct _HookData_t* hd = (reinterpret_cast<const struct _HookData_t*>(lparam));
		wxString str;
		if(hd != NULL && hd->data != NULL)
		{   
			str.append(hd->data, hd->size);
            //wxFileOutputStream output(wxT("C:\\T2.html"));
            //wxTextOutputStream ofs(output);
            //ofs.WriteString(str);
		}		

        if(wparam == HKT_CATCH)
        {
            wxLogDebug(_T("get HKT_CATCH message."));
            
            if(_objFrame != NULL)
                _objFrame->HookCDProc(str);
        }
		else if(wparam == HKT_RESULT_TEXT)
		{
			wxLogDebug(_T("get RESULT_TEXT message."));
			
			if(_objFrame != NULL)
				_objFrame->HookTextProc(str);
		}
		else if(wparam == HKT_RESULT_HTML)
		{
			wxLogDebug(_T("get RESULT_HTLM message."));
			if(_objFrame != NULL)
				_objFrame->HookHTMLProc(str);
		}
		else
		{
			wxLogDebug(_T("get UNKNOWN hook message."));
		}
    }
    else if(msg == _nHotkeyMsgID)
    {
		const struct _HookData_t* hd = (reinterpret_cast<const struct _HookData_t*>(lparam));
		wxString str;
		if(hd != NULL && hd->data != NULL)
		{
			str.append(hd->data, hd->size);
		}		

        HookDataType type = (HookDataType)wparam;
        if(type == HKT_CATCH)
        {
            if(_objFrame != NULL)
                _objFrame->HookCDProc(str);
        }
        else if(type == HKT_RESULT_HTML)
        {
		    if(_objFrame != NULL)
			    _objFrame->HookHTMLProc(str);
        }
        else
        {
			wxLogDebug(_T("get UNKNOWN hotkey message."));
        }
    }
    else if(msg == WM_HOTKEY)
    {
        _objHotkey.GetResult(wparam, lparam);
    }
    else if(msg == _nCheckMsgID)
    {
        if((HookStatus)(wparam) == HS_HOOK)
        {
            Hook((HWND)(lparam));
        }
        else if((HookStatus)(wparam) == HS_UNHOOK)
        {
            Unhook();
        }
        else
        {
			wxLogDebug(_T("get UNKNOWN check message."));
        }
    }

    return 0;
}

int CHookObject::Hook(HWND hwnd)
{
    if(_objHook.Hook(_hwndFrame, hwnd, (_bHookCD ? 1 : 0), _nDelay, _nHookMsgID) != 0)
    {
        ShowInfo(_("Hook hook failed."));
        return -1;
    }

    if(_bOpenHotkey == true)
    {
        if(_objHotkey.Hook(_hwndFrame, hwnd, _nControlKey, _nHotKey, (_bHookCD ? 1 : 0), _nHotkeyMsgID) != 0)
        {
            ShowInfo(_("Set Hotkey failed."));
            return -1;
        }
    }

    _eHookStatus = HS_HOOK;

    if(_objFrame != NULL)
    {
        _objFrame->SetHookButton(true);
    }

	return 0;
}

int CHookObject::Unhook()
{
    _objHook.Unhook();
    _objHotkey.Unhook();

    _eHookStatus = HS_UNHOOK;

    if(_objFrame != NULL)
    {
        _objFrame->SetHookButton(false);
    }

    return 0;
}

int CHookObject::SetHook()
{
    if(_eHookStatus == HS_HOOK)
        return 0;

    if(_bAutoHook == true)
    {
        ::SetEvent(_hEvent[2]);
    }
    else
    {
        CheckLingoesStatus();
    }
 
    return 0;
}

int CHookObject::SetUnhook()
{
    if(_eHookStatus == HS_UNHOOK)
        return 0;

    Unhook();

    if(_bAutoHook == true)
    {
        ::SetEvent(_hEvent[1]);
    }

    return 0;
}

void CHookObject::ShowInfo(const wxString& info)
{
    if(_objFrame != NULL)
        _objFrame->ShowHint(info);
}