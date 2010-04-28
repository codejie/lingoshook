#include <mshtml.h>
#include <atlbase.h>
#include <oleacc.h>
#include <tchar.h>
#include <comutil.h>

#include "LingosHookApp.h"

#include "HotkeyObject.h"

int CHotkeyObject::_wincount = 0;

CHotkeyObject::CHotkeyObject(LingosHookFrame* frame)
: _objFrame(frame)
, _id(0)
{
}

CHotkeyObject::~CHotkeyObject()
{
    Unregister();
}

int CHotkeyObject::Register(HWND hwnd, UINT mod, UINT key)
{
    _id = ::GlobalAddAtom(_T("LingosHook"));
    if(_id == 0)
        return -1;
    if(::RegisterHotKey((HWND)_objFrame->GetHWND(), _id, mod, key) != TRUE)
        return -1;
    _hwnd = hwnd;
    return 0;
}

void CHotkeyObject::Unregister()
{
    if(_id != 0)
    {
        ::UnregisterHotKey((HWND)_objFrame->GetHWND(), _id);
        _id = 0;
    }
}

int CHotkeyObject::MessageProc(WXUINT msg, WXWPARAM wparam, WXLPARAM lparam)
{
    GetResult(wparam, lparam);

    return 0;
}

int CHotkeyObject::GetResult(WXWPARAM wparam, WXLPARAM lparam)
{
    //FindWindows
//    HWND hwnd = ::FindWindow(_T("Afx:400000:0"), _T("Lingoes"));
    if(_hwnd == NULL)
        return -1;
    if(::IsWindowVisible(_hwnd) == FALSE)
        return -1;

    if(GetIEDocResult(_hwnd) != TRUE)
        return -1;
    
    return 0;
}

BOOL CALLBACK CHotkeyObject::EnumChildProc(HWND hwnd, LPARAM lparam)
{
    ++ _wincount;

    if(_wincount == 0x24)
    {
		*(HWND*)lparam = hwnd;
        return FALSE;
    }
    return TRUE;
}

BOOL CHotkeyObject::GetIEDocResult(HWND hwnd)
{
	::CoInitialize(NULL);

	HINSTANCE hinst = ::LoadLibrary( _T("OLEACC.DLL") );	// Explicitly load MSAA so we know if it's installed
	if(hinst != NULL)
	{
		if(hwnd != NULL)
		{
			HWND hc = NULL;
            _wincount = 0;
			::EnumChildWindows(hwnd, EnumChildProc, (LPARAM)&hc);
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
				        _objFrame->HookHTMLProc(html);
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
		}
		::FreeLibrary(hinst);
	}
	::CoUninitialize();
	return TRUE;
}
