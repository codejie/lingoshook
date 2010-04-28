#ifndef __HOTKEYOBJECT_H__
#define __HOTKEYOBJECT_H__

#include "wx/wx.h"

class LingosHookFrame;

class CHotkeyObject
{
public:
    CHotkeyObject(LingosHookFrame* frame);
    virtual ~CHotkeyObject();

    int Register(HWND hwnd, UINT mod, UINT key);
    void Unregister();

    int MessageProc(WXUINT msg, WXWPARAM wparam, WXLPARAM lparam);
protected:
    int GetResult(WXWPARAM wparam, WXLPARAM lparam);
    int GetIEDocResult(HWND hwnd);

    static int _wincount;
    static BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lparam);
private:
    LingosHookFrame* _objFrame;
    HWND _hwnd;
    int _id;
};

#endif

