// -*- C++ -*- generated by wxGlade 0.6.3 on Mon Jan 31 14:54:10 2011

#include "PluginDetailDialog.h"

// begin wxGlade: ::extracode
// end wxGlade



PluginDetailDialog::PluginDetailDialog(const ActivityObject::PropertyData& data, wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
: wxDialog(parent, id, title, pos, size, wxDEFAULT_DIALOG_STYLE)
, _data(data)
{
    // begin wxGlade: PluginDetailDialog::PluginDetailDialog
    sizer_3_staticbox = new wxStaticBox(this, -1, wxEmptyString);
    label_1 = new wxStaticText(this, wxID_ANY, _("Name:"));
    m_labelName = new wxStaticText(this, wxID_ANY, wxEmptyString);
    label_3 = new wxStaticText(this, wxID_ANY, _("Label:"));
    m_labelLabel = new wxStaticText(this, wxID_ANY, wxEmptyString);
    label_5 = new wxStaticText(this, wxID_ANY, _("Version:"));
    m_labelVersion = new wxStaticText(this, wxID_ANY, wxEmptyString);
    label_7 = new wxStaticText(this, wxID_ANY, _("Author:"));
    m_labelAuthor = new wxStaticText(this, wxID_ANY, wxEmptyString);
    label_10 = new wxStaticText(this, wxID_ANY, _("Description:"));
    m_editDesc = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_LINEWRAP);
    panel_2 = new wxPanel(this, wxID_ANY);
    button_1 = new wxButton(this, wxID_CLOSE, wxEmptyString);
    panel_3 = new wxPanel(this, wxID_ANY);

    set_properties();
    do_layout();
    // end wxGlade
}


BEGIN_EVENT_TABLE(PluginDetailDialog, wxDialog)
    // begin wxGlade: PluginDetailDialog::event_table
    EVT_BUTTON(wxID_CLOSE, PluginDetailDialog::OnBtnClose)
    // end wxGlade
END_EVENT_TABLE();


void PluginDetailDialog::OnBtnClose(wxCommandEvent &event)
{
    Close();
}


// wxGlade: add PluginDetailDialog event handlers


void PluginDetailDialog::set_properties()
{
    // begin wxGlade: PluginDetailDialog::set_properties
    SetTitle(_("Plugins  Description - ") + _data.m_strLabel);
    SetSize(wxSize(289, 330));

    m_labelName->SetLabel(_data.m_strName);
    m_labelLabel->SetLabel(_data.m_strLabel);
    m_labelVersion->SetLabel(_data.m_strVersion);
    m_labelAuthor->SetLabel(_data.m_strAuthor);
    m_editDesc->SetValue(_data.m_strDescription);

    // end wxGlade
}


void PluginDetailDialog::do_layout()
{
    // begin wxGlade: PluginDetailDialog::do_layout
    wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_8 = new wxBoxSizer(wxHORIZONTAL);
    wxStaticBoxSizer* sizer_3 = new wxStaticBoxSizer(sizer_3_staticbox, wxVERTICAL);
    wxBoxSizer* sizer_7 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_6 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_5 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_4 = new wxBoxSizer(wxHORIZONTAL);
    sizer_4->Add(label_1, 1, wxALIGN_CENTER_VERTICAL, 0);
    sizer_4->Add(m_labelName, 4, wxALIGN_CENTER_VERTICAL, 0);
    sizer_3->Add(sizer_4, 1, wxEXPAND, 0);
    sizer_5->Add(label_3, 1, wxALIGN_CENTER_VERTICAL, 0);
    sizer_5->Add(m_labelLabel, 4, wxALIGN_CENTER_VERTICAL, 0);
    sizer_3->Add(sizer_5, 1, wxEXPAND, 0);
    sizer_6->Add(label_5, 1, wxALIGN_CENTER_VERTICAL, 0);
    sizer_6->Add(m_labelVersion, 4, wxALIGN_CENTER_VERTICAL, 0);
    sizer_3->Add(sizer_6, 1, wxEXPAND, 0);
    sizer_7->Add(label_7, 1, wxALIGN_CENTER_VERTICAL, 0);
    sizer_7->Add(m_labelAuthor, 4, wxALIGN_CENTER_VERTICAL, 0);
    sizer_3->Add(sizer_7, 1, wxEXPAND, 0);
    sizer_3->Add(label_10, 0, wxEXPAND, 0);
    sizer_3->Add(m_editDesc, 4, wxEXPAND, 0);
    sizer_1->Add(sizer_3, 1, wxALL|wxEXPAND, 4);
    sizer_8->Add(panel_2, 1, wxEXPAND, 0);
    sizer_8->Add(button_1, 0, 0, 0);
    sizer_8->Add(panel_3, 1, wxEXPAND, 0);
    sizer_1->Add(sizer_8, 0, wxEXPAND, 0);
    SetSizer(sizer_1);
    Layout();
    // end wxGlade
}

