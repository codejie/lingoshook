#ifndef __LHCONTROLS_H__
#define __LHCONTROLS_H__

#include "wx/wx.h"
#include "wx/listbox.h"
#include "wx/treectrl.h"
#include "wx/clntdata.h"

class CLHListBox : public wxListBox
{
public:
    CLHListBox(wxWindow *parent, wxWindowID id,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            int n = 0, const wxString choices[] = NULL,
            long style = 0,
            const wxValidator& validator = wxDefaultValidator,
            const wxString& name = wxListBoxNameStr);
public:
    int FindItem(const wxString& item);
    int DeleteItem(int data);
protected:
    virtual void OnKeyDown(wxKeyEvent& event);
    virtual void OnKeyUp(wxKeyEvent& event);
    virtual void OnContextMenu(wxContextMenuEvent& event);
    virtual void OnMouseRightUp(wxMouseEvent& event);
private:
    void SendEvent(wxEventType cmdtype, int selected);
private:
    DECLARE_EVENT_TABLE()
};

DECLARE_EVENT_TYPE(wxEVT_COMMAND_LH_LISTBOX_DELETE, -1)
DECLARE_EVENT_TYPE(wxEVT_COMMAND_LH_LISTBOX_FINDITEM, -1)
DECLARE_EVENT_TYPE(wxEVT_COMMAND_LH_LISTBOX_CONTEXTMENU, -1)

//
class CLHTreeCtrl : public wxTreeCtrl
{
public:
    CLHTreeCtrl(wxWindow *parent, wxWindowID id = wxID_ANY,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT,
               const wxValidator& validator = wxDefaultValidator,
               const wxString& name = wxTreeCtrlNameStr);
protected:
    virtual void OnContextMenu(wxContextMenuEvent& event) {};
private:
    DECLARE_EVENT_TABLE()
};

DECLARE_EVENT_TYPE(wxEVT_COMMAND_LH_TREECTRL_CONTEXTMENU, -1)


class CLHFilterTreeCtrl : public CLHTreeCtrl
{
public:
    enum FilterType { FT_UNKNOWN = -1, FT_TAG = 1, FT_DATE, FT_SCORE, FT_CLOSE };
public:
    CLHFilterTreeCtrl(wxWindow *parent, wxWindowID id = wxID_ANY,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT,
               const wxValidator& validator = wxDefaultValidator,
               const wxString& name = wxTreeCtrlNameStr);
public:
    FilterType GetFilterType() const { return _eFilterType; }
    void SetFilterType(FilterType type) { _eFilterType; }
protected:
    virtual void OnContextMenu(wxContextMenuEvent& event);
protected:
    FilterType _eFilterType;
};

class CLHResultTreeCtrl : public CLHTreeCtrl
{
public:
    CLHResultTreeCtrl(wxWindow *parent, wxWindowID id = wxID_ANY,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT,
               const wxValidator& validator = wxDefaultValidator,
               const wxString& name = wxTreeCtrlNameStr);
protected:
    virtual void OnContextMenu(wxContextMenuEvent& event);
};

//////////////////////

class CLHFilterTreeItemData : public wxTreeItemData
{
public:
    enum ItemType { IT_TAG, IT_WORD };
public:
    CLHFilterTreeItemData(ItemType type, int id, int pid = -1)
        : _type(type), _id(id), _pid(pid)
    {
    }
    ItemType Type() const { return _type; }
    int ID() const { return _id; }
    int PID() const { return _pid; }
protected:
    ItemType _type;
    int _id;
    int _pid;
};

class CLHResultTreeItemData : public wxTreeItemData
{
public:
    CLHResultTreeItemData(int id)
        : _id(id)
    {
    }
    int ID() const { return _id; }
protected:
    int _id;
};

class CLHComboBoxClientData : public wxClientData
{
public:
    CLHComboBoxClientData(int index, const wxString& str)
        :wxClientData()
        , _index(index)
        , _str(str)
    {
    }
    int Index() const { return _index; }
    const wxString& Str() const { return _str; }
protected:
    int _index;
    wxString _str;
};

#endif
