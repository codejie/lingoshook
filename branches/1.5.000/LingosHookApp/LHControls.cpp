
#include "wx/icon.h"


#include "LHControls.h"

DEFINE_EVENT_TYPE(wxEVT_COMMAND_LH_LISTBOX_DELETE)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LH_LISTBOX_FINDITEM)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LH_LISTBOX_CONTEXTMENU)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LH_LISTBOX_FOCUS)

BEGIN_EVENT_TABLE(CLHListBox, wxListBox)
    EVT_KEY_DOWN(CLHListBox::OnKeyDown)
    EVT_KEY_UP(CLHListBox::OnKeyUp)
    EVT_CONTEXT_MENU(CLHListBox::OnContextMenu)
    EVT_SET_FOCUS(CLHListBox::OnSetFocus)
    EVT_KILL_FOCUS(CLHListBox::OnKillFocus)
    EVT_LEAVE_WINDOW(CLHListBox::OnMouseLeave)
//    EVT_RIGHT_UP(CLHListBox::OnMouseRightUp)
END_EVENT_TABLE()

CLHListBox::CLHListBox(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, int n, const wxString choices[], long style, const wxValidator &validator, const wxString &name)
: wxListBox(parent, id, pos, size, n, choices, style, validator, name)
{
}

void CLHListBox::OnKeyDown(wxKeyEvent& event)
{
    if(event.ControlDown() && event.GetKeyCode() == 127)
    {
        int sel = this->GetSelection();
        if(sel != -1)
        {
//            this->SetSelection(sel, false);
            SendEvent(wxEVT_COMMAND_LH_LISTBOX_DELETE, sel);
//            this->Delete(sel);
            //if(this->GetCount() > 0)
            //{
            //    if(sel >= this->GetCount())
            //        sel = this->GetCount() - 1;
            //    SendEvent(wxEVT_COMMAND_LISTBOX_SELECTED, sel);
            //}
        }
    }
    else
    {
        event.Skip();
    }
}

void CLHListBox::OnKeyUp(wxKeyEvent& event)
{
    event.Skip();
}

int CLHListBox::FindItem(const wxString &item) 
{
    if(item.size() == 0)
        return -1;

    wxString str;
    for(size_t i = 0; i < this->GetCount(); ++ i)
    {
        size_t sz = this->GetString(i).size();
        while(sz >= item.size())
        {
            str = this->GetString(i).substr(0, sz);
            if(str == item)
            {
                //SendEvent(wxEVT_COMMAND_LH_LISTBOX_FINDITEM, i);
                SendEvent(wxEVT_COMMAND_LISTBOX_SELECTED, i);
                return 0;
            }
            -- sz;
        }
    }
    return -1;
}

int CLHListBox::DeleteItem(int data)
{
    for(size_t i = 0; i < this->GetCount(); ++ i)
    {
        if((int)this->GetClientData(i) == data)
        {
            this->Delete(i);

            if(this->GetCount() > 0)
            {
                if(i >= this->GetCount())
                    i = this->GetCount() - 1;
                SendEvent(wxEVT_COMMAND_LISTBOX_SELECTED, i);
            }

            return 0;
        }
    }
    return -1;
}

void CLHListBox::SendEvent(wxEventType cmdtype, int selected)
{
    wxCommandEvent event(cmdtype, GetId());
    event.SetEventObject(this);
    event.SetExtraLong(1);
    event.SetString(this->GetString(selected));
    event.SetInt(selected);
    event.SetClientData(this->GetClientData(selected));

    //this->GetParent()->GetEventHandler()->ProcessEvent(event);
    GetEventHandler()->ProcessEvent(event);
}

void CLHListBox::OnContextMenu(wxContextMenuEvent& event)
{
    wxPoint pos = event.GetPosition();
    pos = ScreenToClient(pos);
    int sel = this->HitTest(pos);
    if(sel !=  wxNOT_FOUND)
    {
        //this->SetSelection(sel);
        SendEvent(wxEVT_COMMAND_LISTBOX_SELECTED, sel);
        SendEvent(wxEVT_COMMAND_LH_LISTBOX_CONTEXTMENU, sel);
    }
}

void CLHListBox::OnMouseRightUp(wxMouseEvent &event)
{
    return;
}

void CLHListBox::OnSetFocus(wxFocusEvent &event)
{
    wxCommandEvent ev(wxEVT_COMMAND_LH_LISTBOX_FOCUS,  GetId());
    ev.SetInt(1);
    GetEventHandler()->ProcessEvent(ev);
    event.Skip();
}

void CLHListBox::OnKillFocus(wxFocusEvent &event)
{
    wxCommandEvent ev(wxEVT_COMMAND_LH_LISTBOX_FOCUS,  GetId());
    ev.SetInt(0);
    GetEventHandler()->ProcessEvent(ev);
    event.Skip();
}

void CLHListBox::OnMouseLeave(wxMouseEvent &event)
{
    wxCommandEvent ev(wxEVT_COMMAND_LH_LISTBOX_FOCUS,  GetId());
    ev.SetInt(0);

    GetEventHandler()->ProcessEvent(ev);
    event.Skip();
}

///////////////////////
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LH_COMBOBOX_FOCUS)

BEGIN_EVENT_TABLE(CLHComboBox, wxComboBox)
    EVT_SET_FOCUS(CLHComboBox::OnSetFocus)
    EVT_KILL_FOCUS(CLHComboBox::OnKillFocus)
    EVT_LEAVE_WINDOW(CLHComboBox::OnMouseLeave)
END_EVENT_TABLE()

CLHComboBox::CLHComboBox(wxWindow *parent, wxWindowID id, const wxString &value, const wxPoint& pos, const wxSize &size , int n, const wxString choices[], long style, const wxValidator &validator, const wxString &name)
: wxComboBox(parent, id, value, pos, size, n, choices, style, validator, name)
{
}

void CLHComboBox::OnSetFocus(wxFocusEvent &event)
{
    wxCommandEvent ev(wxEVT_COMMAND_LH_COMBOBOX_FOCUS,  GetId());
    ev.SetInt(1);
    GetEventHandler()->ProcessEvent(ev);
    event.Skip();
}

void CLHComboBox::OnKillFocus(wxFocusEvent &event)
{
    wxCommandEvent ev(wxEVT_COMMAND_LH_COMBOBOX_FOCUS,  GetId());
    ev.SetInt(0);
    GetEventHandler()->ProcessEvent(ev);
    event.Skip();
}

void CLHComboBox::OnMouseLeave(wxMouseEvent &event)
{
    wxCommandEvent ev(wxEVT_COMMAND_LH_COMBOBOX_FOCUS,  GetId());
    ev.SetInt(0);

    GetEventHandler()->ProcessEvent(ev);
    event.Skip();
}

/////////////////////

DEFINE_EVENT_TYPE(wxEVT_COMMAND_LH_TREECTRL_CONTEXTMENU)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LH_TREECTRL_FOCUS)

BEGIN_EVENT_TABLE(CLHTreeCtrl, wxTreeCtrl)
    EVT_CONTEXT_MENU(CLHTreeCtrl::OnContextMenu)
    EVT_SET_FOCUS(CLHTreeCtrl::OnSetFocus)
    EVT_KILL_FOCUS(CLHTreeCtrl::OnKillFocus)
    EVT_LEAVE_WINDOW(CLHTreeCtrl::OnMouseLeave)
END_EVENT_TABLE()

CLHTreeCtrl::CLHTreeCtrl(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style, const wxValidator &validator, const wxString &name)
: wxTreeCtrl(parent, id, pos, size, style, validator, name)
{
}

void CLHTreeCtrl::OnSetFocus(wxFocusEvent &event)
{
    wxCommandEvent ev(wxEVT_COMMAND_LH_TREECTRL_FOCUS,  GetId());
    ev.SetInt(1);
    GetEventHandler()->ProcessEvent(ev);
    event.Skip();
}

void CLHTreeCtrl::OnKillFocus(wxFocusEvent &event)
{
    wxCommandEvent ev(wxEVT_COMMAND_LH_TREECTRL_FOCUS,  GetId());
    ev.SetInt(0);
    GetEventHandler()->ProcessEvent(ev);
    event.Skip();
}

void CLHTreeCtrl::OnMouseLeave(wxMouseEvent &event)
{
    wxCommandEvent ev(wxEVT_COMMAND_LH_TREECTRL_FOCUS,  GetId());
    ev.SetInt(0);

    GetEventHandler()->ProcessEvent(ev);
    event.Skip();
}

///////////

CLHFilterTreeCtrl::CLHFilterTreeCtrl(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style, const wxValidator &validator, const wxString &name)
: CLHTreeCtrl(parent, id, pos, size, style, validator, name)
, _eFilterType(FT_UNKNOWN)
{
}

void CLHFilterTreeCtrl::OnContextMenu(wxContextMenuEvent& event)
{
    wxPoint pos = event.GetPosition();
    pos = ScreenToClient(pos);
    wxTreeItemId id = this->HitTest(pos);
    if(id.IsOk())
    {
        this->SelectItem(id, true);
        CLHFilterTreeItemData* idata = (CLHFilterTreeItemData*)(this->GetItemData(id));
        if(idata != NULL)// && idata->Type() == CLHFilterTreeItemData::IT_WORD)
        {
            wxCommandEvent ev(wxEVT_COMMAND_LH_TREECTRL_CONTEXTMENU, GetId());
            ev.SetEventObject(this);
            ev.SetExtraLong(1);
            ev.SetString(this->GetItemText(id));
            ev.SetInt(idata->Type());
            ev.SetClientData(idata);

            GetEventHandler()->ProcessEvent(ev);
        }
    }    
}

//
CLHResultTreeCtrl::CLHResultTreeCtrl(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxValidator& validator, const wxString& name)
: CLHTreeCtrl(parent, id, pos, size, style, validator, name)
{
}

void CLHResultTreeCtrl::OnContextMenu(wxContextMenuEvent& event)
{
    wxTreeItemId root = this->GetRootItem();
    if(root.IsOk())
    {
        wxCommandEvent ev(wxEVT_COMMAND_LH_TREECTRL_CONTEXTMENU, GetId());
        ev.SetEventObject(this);
        ev.SetExtraLong(1);
        ev.SetString(this->GetItemText(root));

        GetEventHandler()->ProcessEvent(ev);
    }
}

//////////////

DEFINE_EVENT_TYPE(wxEVT_COMMAND_LH_TEXTCTRL_KEYDOWN)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LH_TEXTCTRL_FOCUS)

BEGIN_EVENT_TABLE(CLHTextCtrl, wxTextCtrl)
    EVT_KEY_DOWN(CLHTextCtrl::OnKeyDown)

    EVT_SET_FOCUS(CLHTextCtrl::OnSetFocus)
    EVT_KILL_FOCUS(CLHTextCtrl::OnKillFocus)
    EVT_LEAVE_WINDOW(CLHTextCtrl::OnMouseLeave)
END_EVENT_TABLE()

CLHTextCtrl::CLHTextCtrl(wxWindow *parent, wxWindowID id, const wxString &value, const wxPoint &pos, const wxSize &size, long style, const wxValidator &validator, const wxString &name)
: wxTextCtrl(parent, id, value, pos, size, style, validator, name)
{
}

void CLHTextCtrl::OnSetFocus(wxFocusEvent &event)
{
    wxCommandEvent ev(wxEVT_COMMAND_LH_TEXTCTRL_FOCUS,  GetId());
    ev.SetInt(1);
    GetEventHandler()->ProcessEvent(ev);
    event.Skip();
}

void CLHTextCtrl::OnKillFocus(wxFocusEvent &event)
{
    wxCommandEvent ev(wxEVT_COMMAND_LH_TEXTCTRL_FOCUS,  GetId());
    ev.SetInt(0);
    GetEventHandler()->ProcessEvent(ev);
    event.Skip();
}

void CLHTextCtrl::OnMouseLeave(wxMouseEvent &event)
{
    wxCommandEvent ev(wxEVT_COMMAND_LH_TEXTCTRL_FOCUS,  GetId());
    ev.SetInt(0);

    GetEventHandler()->ProcessEvent(ev);
    event.Skip();
}

void CLHTextCtrl::OnKeyDown(wxKeyEvent &event)
{
    int key = event.GetKeyCode();
    
    if(::wxIsdigit(key) || (key >= (VK_NUMPAD0 + 0xE4) && (key <= VK_NUMPAD9 + 0xE4))|| key == 126 || key == VK_ESCAPE)
    {
        wxCommandEvent ev(wxEVT_COMMAND_LH_TEXTCTRL_KEYDOWN);
        ev.SetEventObject(this);
        ev.SetId(this->GetId());
        ev.SetInt(key);

        GetEventHandler()->ProcessEvent(ev);
    }
    else
    {
        event.Skip();
    }
}

////////////////////////////////////////////////////////

//#include "res/checked.xpm"
//#include "res/unchecked.xpm"
 
//IMPLEMENT_CLASS(CLHCheckBoxList, wxListCtrl)
 
BEGIN_EVENT_TABLE(CLHCheckBoxList, wxListCtrl)
  EVT_LEFT_DOWN(CLHCheckBoxList::OnMouseEvent)
END_EVENT_TABLE()
 
CLHCheckBoxList::CLHCheckBoxList(wxWindow* parent, wxWindowID id, const wxPoint& pt, const wxSize& sz, long style)
: wxListCtrl(parent, id, pt, sz, style)
, m_imageList(16, 16, true)
{
   //m_imageList.Add(wxICON(checked), wxBITMAP_TYPE_ICO_RESOURCE);
    m_imageList.Add(wxIcon(wxT("ICON_ALLINON")), wxBITMAP_TYPE_ICO_RESOURCE);
    //m_imageList.Add(wxIcon(wxT("ICON_UNCHECKED")), wxBITMAP_TYPE_ICO_RESOURCE);

    //SetImageList(&m_imageList, wxIMAGE_LIST_NORMAL);
    SetImageList(&m_imageList, wxIMAGE_LIST_SMALL);
}

void CLHCheckBoxList::OnMouseEvent(wxMouseEvent& event)
{
  if (event.LeftDown())
  {
     int flags;
     long item = HitTest(event.GetPosition(), flags);
     if (item > -1 && (flags & wxLIST_HITTEST_ONITEMICON))
     {
         SetChecked(item, !IsChecked(item));
     }
     else
        event.Skip();
  }
  else
  {
     event.Skip();
  }
}

bool CLHCheckBoxList::IsChecked(long item) const
{
   wxListItem info;
   info.m_mask = wxLIST_MASK_IMAGE ;
   info.m_itemId = item;
 
   if (GetItem(info))
   {
      return (info.m_image == 0);
   }
   else
      return FALSE;
}
 
void CLHCheckBoxList::SetChecked(long item, bool checked)
{
   SetItemImage(item, (checked ? 0 : 1), -1);
}
//0-p-[pdfsdpp00000.00/.'0'/0[00.0'0[]
///[[[[[[...........[0'''''''''''''''''''''''0.0'[[[[[[,9;8p78m,o,oa9/99


/////////////////////////////////////////////
BEGIN_EVENT_TABLE(CLHPanel, wxPanel)
  EVT_ENTER_WINDOW(CLHPanel::OnMouseEnter)
  EVT_LEAVE_WINDOW(CLHPanel::OnMouseLeave)
END_EVENT_TABLE()

void CLHPanel::OnMouseEnter(wxMouseEvent &event)
{
    int i = event.GetId();
    this->Enable(true);
}

void CLHPanel::OnMouseLeave(wxMouseEvent &event)
{
    this->Enable(false);
}

///////////////////////////////////////////////

DEFINE_EVENT_TYPE(wxEVT_COMMAND_LH_RADIOBUTTON_FOCUS)

BEGIN_EVENT_TABLE(CLHRadioButton, wxRadioButton)
    EVT_SET_FOCUS(CLHRadioButton::OnSetFocus)
    EVT_KILL_FOCUS(CLHRadioButton::OnKillFocus)
    EVT_LEAVE_WINDOW(CLHRadioButton::OnMouseLeave)
END_EVENT_TABLE()

CLHRadioButton::CLHRadioButton(wxWindow *parent, wxWindowID id, const wxString &value, const wxPoint &pos, const wxSize &size, long style, const wxValidator &validator, const wxString &name)
: wxRadioButton(parent, id, value, pos, size, style, validator, name)
{
}

void CLHRadioButton::OnSetFocus(wxFocusEvent &event)
{
    wxCommandEvent ev(wxEVT_COMMAND_LH_RADIOBUTTON_FOCUS,  GetId());
    ev.SetInt(1);
    GetEventHandler()->ProcessEvent(ev);
    event.Skip();
}

void CLHRadioButton::OnKillFocus(wxFocusEvent &event)
{
    wxCommandEvent ev(wxEVT_COMMAND_LH_RADIOBUTTON_FOCUS,  GetId());
    ev.SetInt(0);
    GetEventHandler()->ProcessEvent(ev);
    event.Skip();
}

void CLHRadioButton::OnMouseLeave(wxMouseEvent &event)
{
    wxCommandEvent ev(wxEVT_COMMAND_LH_RADIOBUTTON_FOCUS,  GetId());
    ev.SetInt(0);

    GetEventHandler()->ProcessEvent(ev);
    event.Skip();
}

