// -*- C++ -*- generated by wxGlade 0.6.3 on Mon Jun 28 10:09:51 2010

#include "DictChoiceDialog.h"

// begin wxGlade: ::extracode
// end wxGlade



CHtmlDictChoiceDialog::CHtmlDictChoiceDialog(wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):
    wxDialog(parent, id, title, pos, size, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxTHICK_FRAME)
{
    // begin wxGlade: CHtmlDictChoiceDialog::CHtmlDictChoiceDialog
    sizer_1_staticbox = new wxStaticBox(this, -1, wxEmptyString);
    m_listDict = new CLHCheckBoxList(this, 9000, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_SINGLE_SEL|wxSUNKEN_BORDER);
    panel_1 = new wxPanel(this, wxID_ANY);
    m_btnDictTop = new wxButton(this, 9003, wxT("Move Top"));
    m_btnDictUp = new wxButton(this, 9004, wxT("Move Up"));
    m_btnDictDown = new wxButton(this, 9005, wxT("Move Down"));
    m_btnDictBottom = new wxButton(this, 9006, wxT("Move Bottom"));
    static_line_4 = new wxStaticLine(this, wxID_ANY);
    m_radioDictChoose = new wxRadioButton(this, 9001, wxT("Show the Selected Dictionary Result Only."));
    m_radioDictAuto = new wxRadioButton(this, 9007, wxT("Show HTML Data Result While the Selected Dictionary Result is Empty."));
    m_radioDictNo = new wxRadioButton(this, 9002, wxT("Show HTML Data Result Directly."));
    static_line_1 = new wxStaticLine(this, wxID_ANY);
    m_btnDictCancel = new wxButton(this, wxID_CANCEL, wxEmptyString);
    panel_2 = new wxPanel(this, wxID_ANY);
    m_btnDictOK = new wxButton(this, wxID_OK, wxEmptyString);

    set_properties();
    do_layout();
    // end wxGlade
}


BEGIN_EVENT_TABLE(CHtmlDictChoiceDialog, wxDialog)
    // begin wxGlade: CHtmlDictChoiceDialog::event_table
    EVT_BUTTON(9003, CHtmlDictChoiceDialog::OnBtnDictTop)
    EVT_BUTTON(9004, CHtmlDictChoiceDialog::OnBtnDictUp)
    EVT_BUTTON(9005, CHtmlDictChoiceDialog::OnBtnDictDown)
    EVT_BUTTON(9006, CHtmlDictChoiceDialog::OnBtnDictBottom)
    EVT_RADIOBUTTON(9001, CHtmlDictChoiceDialog::OnRadioDict)
    EVT_RADIOBUTTON(9002, CHtmlDictChoiceDialog::OnRadioNoDict)
    EVT_RADIOBUTTON(9007, CHtmlDictChoiceDialog::OnRadioDictAuto)	
    // end wxGlade
END_EVENT_TABLE();


void CHtmlDictChoiceDialog::OnBtnDictTop(wxCommandEvent &event)
{
    long item = m_listDict->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_FOCUSED | wxLIST_STATE_SELECTED);
    if(item != -1)
    {
        if(item == 0)
            return;

        wxListItem info;
        info.SetMask(wxLIST_MASK_TEXT | wxLIST_MASK_IMAGE);
        info.SetId(item);

        info.SetColumn(0);
        m_listDict->GetItem(info);
        wxString title = info.GetText();
        info.SetColumn(1);
        m_listDict->GetItem(info);
        wxString id = info.GetText();

        bool check = m_listDict->IsChecked(item);
        int index = m_listDict->GetItemData(item);

        m_listDict->DeleteItem(item);

        item = m_listDict->InsertItem(0, title, check ? 0 : 1);
        m_listDict->SetItem(item, 1, id);
        m_listDict->SetItemData(item, index);

        m_listDict->SetItemState(item, wxLIST_STATE_FOCUSED | wxLIST_STATE_SELECTED, wxLIST_STATE_FOCUSED | wxLIST_STATE_SELECTED);

    }
}


void CHtmlDictChoiceDialog::OnBtnDictUp(wxCommandEvent &event)
{
    long item = m_listDict->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_FOCUSED | wxLIST_STATE_SELECTED);
    if(item != -1)
    {
        if(item == 0)
            return;

        wxListItem info;
        info.SetMask(wxLIST_MASK_TEXT | wxLIST_MASK_IMAGE);
        info.SetId(item);

        info.SetColumn(0);
        m_listDict->GetItem(info);
        wxString title = info.GetText();
        info.SetColumn(1);
        m_listDict->GetItem(info);
        wxString id = info.GetText();

        bool check = m_listDict->IsChecked(item);
        int index = m_listDict->GetItemData(item);

        m_listDict->DeleteItem(item);
        
        //if(item > 0)
        item = item - 1;

        item = m_listDict->InsertItem(item , title, check ? 0 : 1);
        m_listDict->SetItem(item , 1, id);
        m_listDict->SetItemData(item , index);

        m_listDict->SetItemState(item, wxLIST_STATE_FOCUSED | wxLIST_STATE_SELECTED, wxLIST_STATE_FOCUSED | wxLIST_STATE_SELECTED);
    }
}


void CHtmlDictChoiceDialog::OnBtnDictDown(wxCommandEvent &event)
{
    long item = m_listDict->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_FOCUSED | wxLIST_STATE_SELECTED);
    if(item != -1)
    {
        if(item == m_listDict->GetItemCount() - 1)
            return;

        wxListItem info;
        info.SetMask(wxLIST_MASK_TEXT | wxLIST_MASK_IMAGE);
        info.SetId(item);

        info.SetColumn(0);
        m_listDict->GetItem(info);
        wxString title = info.GetText();
        info.SetColumn(1);
        m_listDict->GetItem(info);
        wxString id = info.GetText();

        bool check = m_listDict->IsChecked(item);
        int index = m_listDict->GetItemData(item);

        m_listDict->DeleteItem(item);

        //if(item == m_listDict->GetItemCount())
         item = item + 1; 

        item = m_listDict->InsertItem(item, title, check ? 0 : 1);
        m_listDict->SetItem(item, 1, id);
        m_listDict->SetItemData(item, index);

        m_listDict->SetItemState(item, wxLIST_STATE_FOCUSED | wxLIST_STATE_SELECTED, wxLIST_STATE_FOCUSED | wxLIST_STATE_SELECTED);
    }
}


void CHtmlDictChoiceDialog::OnBtnDictBottom(wxCommandEvent &event)
{
    long item = m_listDict->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_FOCUSED | wxLIST_STATE_SELECTED);
    if(item != -1)
    {
        if(item == m_listDict->GetItemCount() - 1)
            return;

        wxListItem info;
        info.SetMask(wxLIST_MASK_TEXT | wxLIST_MASK_IMAGE);
        info.SetId(item);

        info.SetColumn(0);
        m_listDict->GetItem(info);
        wxString title = info.GetText();
        info.SetColumn(1);
        m_listDict->GetItem(info);
        wxString id = info.GetText();

        bool check = m_listDict->IsChecked(item);
        int index = m_listDict->GetItemData(item);

        m_listDict->DeleteItem(item);

        item = m_listDict->GetItemCount();

        item = m_listDict->InsertItem(item, title, check ? 0 : 1);
        m_listDict->SetItem(item, 1, id);
        m_listDict->SetItemData(item, index);

        m_listDict->SetItemState(item, wxLIST_STATE_FOCUSED | wxLIST_STATE_SELECTED, wxLIST_STATE_FOCUSED | wxLIST_STATE_SELECTED);
    }
}

void CHtmlDictChoiceDialog::OnRadioDict(wxCommandEvent &event)
{
    m_listDict->Enable(true);
    m_btnDictTop->Enable(true);
    m_btnDictUp->Enable(true);
    m_btnDictDown->Enable(true);
    m_btnDictBottom->Enable(true);
}

void CHtmlDictChoiceDialog::OnRadioNoDict(wxCommandEvent &event)
{
    m_listDict->Enable(false);
    m_btnDictTop->Enable(false);
    m_btnDictUp->Enable(false);
    m_btnDictDown->Enable(false);
    m_btnDictBottom->Enable(false);
}

void CHtmlDictChoiceDialog::OnRadioDictAuto(wxCommandEvent &event)
{
    m_listDict->Enable(true);
    m_btnDictTop->Enable(true);
    m_btnDictUp->Enable(true);
    m_btnDictDown->Enable(true);
    m_btnDictBottom->Enable(true);
}

// wxGlade: add CHtmlDictChoiceDialog event handlers


void CHtmlDictChoiceDialog::set_properties()
{
    // begin wxGlade: CHtmlDictChoiceDialog::set_properties
    SetTitle(wxT("Dictionary Chooser.."));
    m_listDict->SetMinSize(wxSize(378, 214));
    // end wxGlade
}


void CHtmlDictChoiceDialog::do_layout()
{
    // begin wxGlade: CHtmlDictChoiceDialog::do_layout
    wxStaticBoxSizer* sizer_1 = new wxStaticBoxSizer(sizer_1_staticbox, wxHORIZONTAL);
    wxBoxSizer* sizer_2 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_4 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_3 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_5 = new wxBoxSizer(wxHORIZONTAL);
    sizer_3->Add(m_listDict, 1, wxEXPAND, 0);
    sizer_5->Add(panel_1, 1, wxEXPAND, 0);
    sizer_5->Add(m_btnDictTop, 0, 0, 0);
    sizer_5->Add(m_btnDictUp, 0, 0, 0);
    sizer_5->Add(m_btnDictDown, 0, 0, 0);
    sizer_5->Add(m_btnDictBottom, 0, 0, 0);
    sizer_3->Add(sizer_5, 0, wxALL|wxEXPAND, 4);
    sizer_3->Add(static_line_4, 0, wxALL|wxEXPAND, 8);
    sizer_2->Add(sizer_3, 1, wxEXPAND, 0);
    sizer_2->Add(m_radioDictChoose, 0, wxALL, 4);
    sizer_2->Add(m_radioDictAuto, 0, wxALL, 4);
    sizer_2->Add(m_radioDictNo, 0, wxALL, 4);
    sizer_2->Add(static_line_1, 0, wxALL|wxEXPAND, 8);
    sizer_4->Add(m_btnDictCancel, 0, 0, 0);
    sizer_4->Add(panel_2, 1, wxEXPAND, 0);
    sizer_4->Add(m_btnDictOK, 0, 0, 0);
    sizer_2->Add(sizer_4, 0, wxALL|wxEXPAND, 8);
    sizer_1->Add(sizer_2, 1, wxEXPAND, 0);
    SetSizer(sizer_1);
    sizer_1->Fit(this);
    Layout();
    // end wxGlade
}

void CHtmlDictChoiceDialog::SetUseDictRadio(int usedict)
{
    if(usedict == 1)
        m_radioDictChoose->SetValue(true);
    else if(usedict == 2)
        m_radioDictAuto->SetValue(true);
    else
        m_radioDictNo->SetValue(true);

    m_listDict->Enable(usedict != 0);
    m_btnDictTop->Enable(usedict != 0);
    m_btnDictUp->Enable(usedict != 0);
    m_btnDictDown->Enable(usedict != 0);
    m_btnDictBottom->Enable(usedict != 0);
}

int CHtmlDictChoiceDialog::GetUseDictRadio(int &usedict) const
{
    if(m_radioDictChoose->GetValue())
        usedict = 1;
    else if(m_radioDictAuto->GetValue())
        usedict = 2;
    else
        usedict = 0;
    return  0;
}