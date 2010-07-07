/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#include "LingosHookApp.h"
#include "TrayIconObject.h"


BEGIN_EVENT_TABLE(CTrayIconObject, wxTaskBarIcon)
    EVT_MENU(CTrayIconObject::CMIID_EXIT, CTrayIconObject::OnMenuExit)
    EVT_MENU(CTrayIconObject::CMIID_SHOW, CTrayIconObject::OnMenuShow)
    EVT_MENU(CTrayIconObject::CMIID_SETHOOK, CTrayIconObject::OnMenuSetHook)
    EVT_MENU(CTrayIconObject::CMIID_RUNLINGOS, CTrayIconObject::OnMenuRunLingos)
    EVT_TASKBAR_LEFT_DCLICK(CTrayIconObject::OnLeftButtonDClick)
END_EVENT_TABLE()

CTrayIconObject::CTrayIconObject(LingosHookFrame *frame)
: _frame(frame)
{
}

CTrayIconObject::~CTrayIconObject()
{
    if(IsIconInstalled())
        RemoveIcon();
}

int CTrayIconObject::Init()
{
    return SetStatus(TS_UNHOOK);
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
    switch(status)
    {
    case TS_HOOK:
        {
            SetIcon(wxICON(ICON_CHECKED), wxT("HOOK"));
        }
        break;
    case TS_UNHOOK:
        {
            SetIcon(wxICON(ICON_UNCHECKED), wxT("UNHOOK"));
        }
        break;
    default:
        return -1;
    }
    return 0;
}