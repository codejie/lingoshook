/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#include "Consts.h"
#include "ConfigData.h"
#include "HookObject.h"
#include "LingosHookApp.h"
#include "TrayIconObject.h"


BEGIN_EVENT_TABLE(CTrayIconObject, wxTaskBarIcon)
    EVT_MENU(CTrayIconObject::CMIID_EXIT, CTrayIconObject::OnMenuExit)
    EVT_MENU(CTrayIconObject::CMIID_SHOW, CTrayIconObject::OnMenuShow)
    EVT_MENU(CTrayIconObject::CMIID_SETHOOK, CTrayIconObject::OnMenuSetHook)
    EVT_MENU(CTrayIconObject::CMIID_RUNLINGOS, CTrayIconObject::OnMenuRunLingos)
    EVT_TASKBAR_LEFT_DCLICK(CTrayIconObject::OnLeftButtonDClick)
END_EVENT_TABLE()

CTrayIconObject::CTrayIconObject(LingosHookFrame *frame, const CConfigData* conf)
: _frame(frame)
, _bIsHook(false)
, _dataConfig(conf)
{
}

CTrayIconObject::~CTrayIconObject()
{
    if(IsIconInstalled())
        RemoveIcon();

    _frame = NULL;
}

int CTrayIconObject::Init()
{
    return SetStatus(TS_NORMAL);
}

int CTrayIconObject::ShowFrame(bool show)
{
    if(_frame == NULL)
        return -1;

    _frame->Show(true);

    return 0;
}

int CTrayIconObject::SetStatus(CTrayIconObject::TrayStatus status)
{
    if(_frame == NULL)
        return -1;

    switch(status)
    {
    case TS_HOOK:
        {
            _bIsHook = true;
            SetIcon(wxICON(ICON_HOOK), APP_TITLE + wxT(" - HOOK"));
        }
        break;
    case TS_NORMAL:
        {
            _bIsHook = false;
            SetIcon(wxICON(ICON_NORMAL), APP_TITLE + wxT(" - UNHOOK"));
        }
        break;
    default:
        return -1;
    }
    return 0;
}

wxMenu* CTrayIconObject::CreatePopupMenu()
{
    wxMenu* menu = new wxMenu;

    menu->Append(CMIID_SHOW, wxT("Show LingosHook"));
    menu->Append(CMIID_RUNLINGOS, wxT("Run Lingoes"));

    HWND hwnd = CHookObject::GetLingoesHandle(0);

    if(_dataConfig->m_strLingoesExec.empty() || _dataConfig->m_strLingoesPath.empty() || hwnd != NULL)
    {
        menu->Enable(CMIID_RUNLINGOS, false);
    }
    else
    {
        menu->Enable(CMIID_RUNLINGOS, true);
    }

//    menu->AppendSeparator();

    if(_bIsHook)
    {
        menu->Append(CMIID_SETHOOK, wxT("Unhook"));        
    }
    else
    {
        menu->Append(CMIID_SETHOOK, wxT("Hook"));
    }
    menu->AppendSeparator();
    menu->Append(CMIID_EXIT, wxT("Exit"));

    return menu;
}

void CTrayIconObject::OnLeftButtonDClick(wxTaskBarIconEvent &event)
{
    _frame->Restore();
    _frame->Show(true);
    _frame->SetFocus();
}

void CTrayIconObject::OnMenuExit(wxCommandEvent &event)
{
    _frame->SetCloseFlag(true);
    //::SendMessage((HWND)_frame->GetHWND(), WM_CLOSE, 0, 0);
    _frame->Close();
}

void CTrayIconObject::OnMenuShow(wxCommandEvent &event)
{
    _frame->Restore();
    _frame->Show(true);
    _frame->SetFocus();
}

void CTrayIconObject::OnMenuSetHook(wxCommandEvent &event)
{
    if(_bIsHook)
    {//unhook
        ::PostMessage((HWND)_frame->GetHWND(), WM_SET_HOOK, 0, 0);
    }
    else
    {
        ::PostMessage((HWND)_frame->GetHWND(), WM_SET_HOOK, 1, 0);
    }
}

void CTrayIconObject::OnMenuRunLingos(wxCommandEvent &event)
{
    ::ShellExecute(NULL, _("open"), _dataConfig->m_strLingoesExec.c_str(), NULL, _dataConfig->m_strLingoesPath.c_str(), SW_SHOWNORMAL);
}