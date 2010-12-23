/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/
#ifndef __TRAYICONOBJECT_H__
#define __TRAYICONOBJECT_H__

#include <string>

#include "wx/taskbar.h"


class CConfigData;
class CTagObject;
class LingosHookFrame;

class CTrayIconObject : public wxTaskBarIcon
{
public:
    enum TrayStatus { TS_UNKNOWN = -1, TS_HOOK = 0, TS_NORMAL };
    enum ContextMenuItemID { CMIID_EXIT = 15000, CMIID_SHOW, CMIID_SETHOOK, CMIID_RUNLINGOS, CMIID_TAGBASE = 15100 };
public:
    CTrayIconObject(LingosHookFrame* frame, const CConfigData* conf, CTagObject* tag);
    virtual ~CTrayIconObject();

    int Init();

    int ShowFrame(bool show);
    int SetStatus(TrayStatus status);

    virtual wxMenu *CreatePopupMenu();
protected:
    void OnLeftButtonDClick(wxTaskBarIconEvent& event);
    void OnMenuExit(wxCommandEvent& event);
    void OnMenuShow(wxCommandEvent& event);
    void OnMenuSetHook(wxCommandEvent& event);
    void OnMenuRunLingos(wxCommandEvent& event);
    void OnSubMenuTag(wxCommandEvent& event);
private:
    LingosHookFrame* _frame;
    bool _bIsHook;
    const CConfigData* _dataConfig;
    CTagObject* _objTag;
DECLARE_EVENT_TABLE()
};

#endif
