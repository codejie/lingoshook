#ifndef __LHCONTROLS_H__
#define __LHCONTROLS_H__

#include "wx/wx.h"
#include "wx/listbox.h"
#include "wx/treectrl.h"
#include "wx/clntdata.h"

#include "Consts.h"

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

    virtual void OnSetFocus(wxFocusEvent& event);
    virtual void OnKillFocus(wxFocusEvent& event);
    virtual void OnMouseLeave(wxMouseEvent& event);
private:
    void SendEvent(wxEventType cmdtype, int selected);
private:
    DECLARE_EVENT_TABLE()
};

DECLARE_EVENT_TYPE(wxEVT_COMMAND_LH_LISTBOX_DELETE, -1)
DECLARE_EVENT_TYPE(wxEVT_COMMAND_LH_LISTBOX_FINDITEM, -1)
DECLARE_EVENT_TYPE(wxEVT_COMMAND_LH_LISTBOX_CONTEXTMENU, -1)
DECLARE_EVENT_TYPE(wxEVT_COMMAND_LH_LISTBOX_FOCUS, -1)

//
class CLHComboBox : public wxComboBox
{
public:
    CLHComboBox(wxWindow *parent, wxWindowID id,
            const wxString& value = wxEmptyString,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            int n = 0, const wxString choices[] = NULL,
            long style = 0,
            const wxValidator& validator = wxDefaultValidator,
            const wxString& name = wxComboBoxNameStr);
protected:
    virtual void OnSetFocus(wxFocusEvent& event);
    virtual void OnKillFocus(wxFocusEvent& event);
    virtual void OnMouseLeave(wxMouseEvent& event);
private:
    DECLARE_EVENT_TABLE()
};

DECLARE_EVENT_TYPE(wxEVT_COMMAND_LH_COMBOBOX_FOCUS, -1)

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

    virtual void OnSetFocus(wxFocusEvent& event);
    virtual void OnKillFocus(wxFocusEvent& event);
    virtual void OnMouseLeave(wxMouseEvent& event);
private:
    DECLARE_EVENT_TABLE()
};

DECLARE_EVENT_TYPE(wxEVT_COMMAND_LH_TREECTRL_CONTEXTMENU, -1)
DECLARE_EVENT_TYPE(wxEVT_COMMAND_LH_TREECTRL_FOCUS, -1)

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
    void SetFilterType(FilterType type) { _eFilterType = type; }
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
    enum ItemType { IT_WORD, IT_TAG, IT_DATE_DAY, IT_DATE_WEEK, IT_DATE_MONTH, IT_SCORE };
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

//class CLHCheckListClientData : public wxClientData
//{
//public:
//    CLHCheckListClientData(int index, int param)
//        : wxClientData()
//        , _index(index)
//        , _param(param)
//    {
//    }
//    int Index() const { return _index; }
//    int Param() const { return _param; }
//
//    void Index(int index) { _index = index; }
//    void Param(int param) { _param = param; }
//protected:
//    int _index;
//    int _param;
//};

/////////////
class CLHTextCtrl : public wxTextCtrl
{
public:
    CLHTextCtrl(wxWindow *parent, wxWindowID id,
               const wxString& value = wxEmptyString,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = 0,
               const wxValidator& validator = wxDefaultValidator,
               const wxString& name = wxTextCtrlNameStr);
protected:
    virtual void OnSetFocus(wxFocusEvent& event);
    virtual void OnKillFocus(wxFocusEvent& event);
    virtual void OnMouseLeave(wxMouseEvent& event);
    virtual void OnKeyDown(wxKeyEvent& event);
private:
    DECLARE_EVENT_TABLE()
};

DECLARE_EVENT_TYPE(wxEVT_COMMAND_LH_TEXTCTRL_KEYDOWN, -1)
DECLARE_EVENT_TYPE(wxEVT_COMMAND_LH_TEXTCTRL_FOCUS, -1)

///

#ifdef __LH_USE_WXIE__

#include "IEHtmlWin.h"

class CLHHtmlWindow : public wxIEHtmlWin
{
public:
    CLHHtmlWindow(wxWindow * parent, wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = 0, const wxString& name = wxPanelNameStr)
    : wxIEHtmlWin(parent, id, pos, size, style, name)
    {
    }
    virtual ~CLHHtmlWindow() {}

public:
    void LoadBlankPage() 
    {
        //wxWindow* fw = this->FindFocus();
        wxIEHtmlWin::LoadWString(wxT("<HTML><!-- created by Codejie(codejie@gmail.com) --></HTML>"));
        //fw->SetFocus();
    }
    bool LoadString(const wxString& html)
    { 
        //wxWindow* fw = this->FindFocus();
        wxIEHtmlWin::LoadWString(html); 
        //fw->SetFocus();
        return true;
    }
    void SetCharset(const wxString& charset) { wxIEHtmlWin::SetCharset(charset); }
};

#else

#include <wx/html/htmlwin.h>

class CLHHtmlWindow : public wxHtmlWindow
{
public:
    CLHHtmlWindow(wxWindow * parent, wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = 0, const wxString& name = wxPanelNameStr)
    : wxHtmlWindow(parent, id, pos, size, style, name)
    {
    }
    virtual ~CLHHtmlWindow() {}

public:
    void LoadBlankPage() { wxHtmlWindow::SetPage(wxT("<HTML><HTML><!-- created by Codejie(codejie@gmail.com) --></HTML></HTML>")); }
    bool LoadString(const wxString& html)
    { 
        wxString str = html;
        str.Replace(_("file:///"), _(""), true);
        return wxHtmlWindow::SetPage(str); 
    }
    void SetCharset(const wxString& charset) {}
};

#endif

// 
#include "wx/listctrl.h"

class CLHCheckBoxList : public wxListCtrl
{
public:
    CLHCheckBoxList(wxWindow* parent, wxWindowID id, const wxPoint& pt, const wxSize& sz, long style);

    void OnMouseEvent(wxMouseEvent& event);
    bool IsChecked(long item) const;
    void SetChecked(long item, bool checked);
protected:
    wxImageList m_imageList;
private:
    DECLARE_EVENT_TABLE()
};


#include "wx/panel.h"

class CLHPanel : public wxPanel
{
public:
    CLHPanel(wxWindow *parent, wxWindowID winid = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxNO_BORDER, const wxString& name = wxPanelNameStr)
        : wxPanel(parent, winid, pos, size, style, name)
//        , _winChild(NULL)
    {
    }

    void OnMouseEnter(wxMouseEvent& event);
    void OnMouseLeave(wxMouseEvent& event);
private:
    DECLARE_EVENT_TABLE()
};

///
#include "wx/radiobut.h"

class CLHRadioButton : public wxRadioButton
{
public:
    CLHRadioButton(wxWindow *parent,
                  wxWindowID id,
                  const wxString& label,
                  const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize,
                  long style = 0,
                  const wxValidator& validator = wxDefaultValidator,
                  const wxString& name = wxRadioButtonNameStr); 
protected:
    virtual void OnSetFocus(wxFocusEvent& event);
    virtual void OnKillFocus(wxFocusEvent& event);
    virtual void OnMouseLeave(wxMouseEvent& event);
private:
    DECLARE_EVENT_TABLE()
};

DECLARE_EVENT_TYPE(wxEVT_COMMAND_LH_RADIOBUTTON_FOCUS, -1)

#endif
