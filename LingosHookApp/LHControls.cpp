
#include "LHControls.h"

DEFINE_EVENT_TYPE(wxEVT_COMMAND_LH_LISTBOX_DELETE)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LH_LISTBOX_FINDITEM)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LH_LISTBOX_CONTEXTMENU)

BEGIN_EVENT_TABLE(CLHListBox, wxListBox)
    EVT_KEY_DOWN(CLHListBox::OnKeyDown)
    EVT_KEY_UP(CLHListBox::OnKeyUp)
    EVT_CONTEXT_MENU(CLHListBox::OnContextMenu)
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

///

DEFINE_EVENT_TYPE(wxEVT_COMMAND_LH_TREECTRL_CONTEXTMENU)

BEGIN_EVENT_TABLE(CLHTreeCtrl, wxTreeCtrl)
    EVT_CONTEXT_MENU(CLHTreeCtrl::OnContextMenu)
END_EVENT_TABLE()

CLHTreeCtrl::CLHTreeCtrl(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style, const wxValidator &validator, const wxString &name)
: wxTreeCtrl(parent, id, pos, size, style, validator, name)
{
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

BEGIN_EVENT_TABLE(CLHTextCtrl, wxTextCtrl)
    EVT_KEY_DOWN(CLHTextCtrl::OnKeyDown)
END_EVENT_TABLE()

void CLHTextCtrl::OnKeyDown(wxKeyEvent &event)
{
    int key = event.GetKeyCode();
    
    if(::wxIsdigit(key) || key == 126 || key == VK_ESCAPE)
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


