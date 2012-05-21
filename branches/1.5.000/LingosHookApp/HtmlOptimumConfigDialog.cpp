// -*- C++ -*- generated by wxGlade 0.6.5 (standalone edition) on Thu May 10 17:35:25 2012

#include "HtmlOptimumConfigDialog.h"

// begin wxGlade: ::extracode
// end wxGlade


const wxString HtmlOptimumConfigDialog::STR_TAG_DEFAULT	=	wxT("Default Configuration");
const wxString HtmlOptimumConfigDialog::STR_TAG_ALL		=	wxT("All Tag Elements");
const wxString HtmlOptimumConfigDialog::STR_TAG_LINK	=	wxT("A (Link Elements)");
const wxString HtmlOptimumConfigDialog::STR_TAG_IMG		=	wxT("IMG (Image/Picture Elements)");
const wxString HtmlOptimumConfigDialog::STR_TAG_OBJECT	=	wxT("OBJECT (Object Elements)");
const wxString HtmlOptimumConfigDialog::STR_TAG_EMBED	=	wxT("EMBED (Embedded Elemetns)");
const wxString HtmlOptimumConfigDialog::STR_TAG_PARAM	=	wxT("PARAM (Parameter Elements)");

const wxString HtmlOptimumConfigDialog::STR_ATTR_DEFAULT		=	wxT("Default Configuration");
const wxString HtmlOptimumConfigDialog::STR_ATTR_ALL			=	wxT("All Attribute Elements");
const wxString HtmlOptimumConfigDialog::STR_ATTR_CLASS			=	wxT("class");
const wxString HtmlOptimumConfigDialog::STR_ATTR_TITLE			=	wxT("title");
const wxString HtmlOptimumConfigDialog::STR_ATTR_ONCLICK		=	wxT("onclick");
const wxString HtmlOptimumConfigDialog::STR_ATTR_ONMOUSEOUT		=	wxT("onmouseout");
const wxString HtmlOptimumConfigDialog::STR_ATTR_ONMOUSEOVER	=	wxT("onmouseover");
const wxString HtmlOptimumConfigDialog::STR_ATTR_ONMOUSEUP		=	wxT("onmouseup");
const wxString HtmlOptimumConfigDialog::STR_ATTR_ONMOUSEDOWN	=	wxT("onmousedown");
const wxString HtmlOptimumConfigDialog::STR_ATTR_ONSELECTSTART	=	wxT("onselectstart");

const wxString HtmlOptimumConfigDialog::STR_VALUE_DEFAULT	=	wxT("Default Configuration");
const wxString HtmlOptimumConfigDialog::STR_VALUE_ALL		=	wxT("All Value Elements");

HtmlOptimumConfigDialog::HtmlOptimumConfigDialog(CDocumentOutputObject::TKeyMap& mapKey, wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):
    wxDialog(parent, id, title, pos, size, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxTHICK_FRAME)
, _mapKey(mapKey)
{
    // begin wxGlade: HtmlOptimumConfigDialog::HtmlOptimumConfigDialog
    label_1 = new wxStaticText(this, wxID_ANY, wxT("Tag Elements"));
    static_line_2 = new wxStaticLine(this, wxID_ANY);
    const wxString comboTag_choices[] = {
        STR_TAG_DEFAULT,
        STR_TAG_LINK,
        STR_TAG_IMG,
        STR_TAG_OBJECT,
        STR_TAG_EMBED,
        STR_TAG_PARAM,
        STR_TAG_ALL
    };
    comboTag = new wxComboBox(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 7, comboTag_choices, wxCB_DROPDOWN|wxCB_DROPDOWN);
    button_4 = new wxButton(this, 1000, wxT("Add"));
    const wxString *listTag_choices = NULL;
    listTag = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, listTag_choices, 0);
    button_5 = new wxButton(this, 1001, wxT("Remove"));
    button_6 = new wxButton(this, 1002, wxT("Remove All"));
    panel_2 = new wxPanel(this, wxID_ANY);
    label_1_copy = new wxStaticText(this, wxID_ANY, wxT("Attribute Elements"));
    static_line_2_copy = new wxStaticLine(this, wxID_ANY);
    const wxString comboAttrib_choices[] = {
        STR_ATTR_DEFAULT,
		STR_ATTR_CLASS,
		STR_ATTR_TITLE,
        STR_ATTR_ONCLICK,
        STR_ATTR_ONMOUSEOUT,
        STR_ATTR_ONMOUSEOVER,
        STR_ATTR_ONMOUSEUP,
        STR_ATTR_ONMOUSEDOWN,
        STR_ATTR_ONSELECTSTART,
        STR_ATTR_ALL
    };
    comboAttrib = new wxComboBox(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 10, comboAttrib_choices, wxCB_DROPDOWN|wxCB_DROPDOWN);
    button_4_copy = new wxButton(this, 2000, wxT("Add"));
    const wxString *listAttrib_choices = NULL;
    listAttrib = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, listAttrib_choices, 0);
    button_5_copy = new wxButton(this, 2001, wxT("Remove"));
    button_6_copy = new wxButton(this, 2002, wxT("Remove All"));
    panel_2_copy = new wxPanel(this, wxID_ANY);
    label_1_copy_1 = new wxStaticText(this, wxID_ANY, wxT("Value Elements"));
    static_line_2_copy_1 = new wxStaticLine(this, wxID_ANY);
    const wxString comboValue_choices[] = {
        STR_VALUE_DEFAULT,
        STR_VALUE_ALL
    };
    comboValue = new wxComboBox(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 2, comboValue_choices, wxCB_DROPDOWN|wxCB_DROPDOWN);
    button_4_copy_1 = new wxButton(this, 3000, wxT("Add"));
    const wxString *listValue_choices = NULL;
    listValue = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, listValue_choices, 0);
    button_5_copy_1 = new wxButton(this, 3001, wxT("Remove"));
    button_6_copy_1 = new wxButton(this, 3002, wxT("Remove All"));
    panel_2_copy_1 = new wxPanel(this, wxID_ANY);
    static_line_1 = new wxStaticLine(this, wxID_ANY);
    button_1 = new wxButton(this, 4000, wxT("Use Default Configuration"));
    panel_1 = new wxPanel(this, wxID_ANY);
    button_2 = new wxButton(this, wxID_CANCEL, wxT("Cancel"));
    button_3 = new wxButton(this, wxID_OK, wxEmptyString);

    set_properties();
    do_layout();
    // end wxGlade
}


BEGIN_EVENT_TABLE(HtmlOptimumConfigDialog, wxDialog)
    // begin wxGlade: HtmlOptimumConfigDialog::event_table
    EVT_BUTTON(1000, HtmlOptimumConfigDialog::OnBtnTagAdd)
    EVT_BUTTON(1001, HtmlOptimumConfigDialog::OnBtnTagRemove)
    EVT_BUTTON(1002, HtmlOptimumConfigDialog::OnBtnTagRemoveAll)
    EVT_BUTTON(2000, HtmlOptimumConfigDialog::OnBtnAttrAdd)
    EVT_BUTTON(2001, HtmlOptimumConfigDialog::OnBtnAttrRemove)
    EVT_BUTTON(2002, HtmlOptimumConfigDialog::OnBtnAttrRemoveAll)
    EVT_BUTTON(3000, HtmlOptimumConfigDialog::OnBtnValAdd)
    EVT_BUTTON(3001, HtmlOptimumConfigDialog::OnBtnValRemove)
    EVT_BUTTON(3002, HtmlOptimumConfigDialog::OnBtnValRemoveAll)
    EVT_BUTTON(4000, HtmlOptimumConfigDialog::OnBtnDefault)
    EVT_BUTTON(wxID_OK, HtmlOptimumConfigDialog::OnBtnOK)
    // end wxGlade
END_EVENT_TABLE();


void HtmlOptimumConfigDialog::do_layout()
{
    // begin wxGlade: HtmlOptimumConfigDialog::do_layout
    wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_2 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_3_copy_1 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_6_copy_1 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_7_copy_1 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_5_copy_1 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_4_copy_1 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_3_copy = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_6_copy = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_7_copy = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_5_copy = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_4_copy = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_3 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_6 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_7 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_5 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_4 = new wxBoxSizer(wxHORIZONTAL);
    sizer_4->Add(label_1, 0, wxLEFT|wxRIGHT|wxALIGN_CENTER_VERTICAL, 4);
    sizer_4->Add(static_line_2, 1, wxALL|wxALIGN_CENTER_VERTICAL, 8);
    sizer_3->Add(sizer_4, 0, wxEXPAND, 0);
    sizer_5->Add(comboTag, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 4);
    sizer_5->Add(button_4, 0, wxALL|wxALIGN_CENTER_VERTICAL, 4);
    sizer_3->Add(sizer_5, 0, wxLEFT|wxRIGHT|wxEXPAND, 32);
    sizer_6->Add(listTag, 1, wxALL|wxEXPAND, 4);
    sizer_7->Add(button_5, 0, wxBOTTOM, 4);
    sizer_7->Add(button_6, 0, wxTOP|wxBOTTOM, 0);
    sizer_7->Add(panel_2, 1, wxEXPAND, 0);
    sizer_6->Add(sizer_7, 0, wxALL|wxEXPAND, 4);
    sizer_3->Add(sizer_6, 1, wxLEFT|wxRIGHT|wxEXPAND, 32);
    sizer_1->Add(sizer_3, 1, wxALL|wxEXPAND, 2);
    sizer_4_copy->Add(label_1_copy, 0, wxLEFT|wxRIGHT|wxALIGN_CENTER_VERTICAL, 4);
    sizer_4_copy->Add(static_line_2_copy, 1, wxALL|wxALIGN_CENTER_VERTICAL, 8);
    sizer_3_copy->Add(sizer_4_copy, 0, wxEXPAND, 0);
    sizer_5_copy->Add(comboAttrib, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 4);
    sizer_5_copy->Add(button_4_copy, 0, wxALL|wxALIGN_CENTER_VERTICAL, 4);
    sizer_3_copy->Add(sizer_5_copy, 0, wxLEFT|wxRIGHT|wxEXPAND, 32);
    sizer_6_copy->Add(listAttrib, 1, wxALL|wxEXPAND, 4);
    sizer_7_copy->Add(button_5_copy, 0, wxBOTTOM, 4);
    sizer_7_copy->Add(button_6_copy, 0, wxTOP|wxBOTTOM, 0);
    sizer_7_copy->Add(panel_2_copy, 1, wxEXPAND, 0);
    sizer_6_copy->Add(sizer_7_copy, 0, wxALL|wxEXPAND, 4);
    sizer_3_copy->Add(sizer_6_copy, 1, wxLEFT|wxRIGHT|wxEXPAND, 32);
    sizer_1->Add(sizer_3_copy, 1, wxALL|wxEXPAND, 2);
    sizer_4_copy_1->Add(label_1_copy_1, 0, wxLEFT|wxRIGHT|wxALIGN_CENTER_VERTICAL, 4);
    sizer_4_copy_1->Add(static_line_2_copy_1, 1, wxALL|wxALIGN_CENTER_VERTICAL, 8);
    sizer_3_copy_1->Add(sizer_4_copy_1, 0, wxEXPAND, 0);
    sizer_5_copy_1->Add(comboValue, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 4);
    sizer_5_copy_1->Add(button_4_copy_1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 4);
    sizer_3_copy_1->Add(sizer_5_copy_1, 0, wxLEFT|wxRIGHT|wxEXPAND, 32);
    sizer_6_copy_1->Add(listValue, 1, wxALL|wxEXPAND, 4);
    sizer_7_copy_1->Add(button_5_copy_1, 0, wxBOTTOM, 4);
    sizer_7_copy_1->Add(button_6_copy_1, 0, wxTOP|wxBOTTOM, 0);
    sizer_7_copy_1->Add(panel_2_copy_1, 1, wxEXPAND, 0);
    sizer_6_copy_1->Add(sizer_7_copy_1, 0, wxALL|wxEXPAND, 4);
    sizer_3_copy_1->Add(sizer_6_copy_1, 1, wxLEFT|wxRIGHT|wxEXPAND, 32);
    sizer_1->Add(sizer_3_copy_1, 1, wxALL|wxEXPAND, 2);
    sizer_1->Add(static_line_1, 0, wxALL|wxEXPAND, 8);
    sizer_2->Add(button_1, 0, wxALL, 4);
    sizer_2->Add(panel_1, 1, wxEXPAND, 0);
    sizer_2->Add(button_2, 0, wxALL, 4);
    sizer_2->Add(button_3, 0, wxALL, 4);
    sizer_1->Add(sizer_2, 0, wxALL|wxEXPAND, 4);
    SetSizer(sizer_1);
    Layout();
	Center();
    // end wxGlade
}


// wxGlade: add HtmlOptimumConfigDialog event handlers


void HtmlOptimumConfigDialog::set_properties()
{
    // begin wxGlade: HtmlOptimumConfigDialog::set_properties
    SetTitle(wxT("HTML Optimum Configuration"));
    SetSize(wxSize(451, 564));
    label_1->SetFont(wxFont(10, wxDEFAULT, wxNORMAL, wxBOLD, 0, wxT("")));
    comboTag->SetSelection(0);
	comboTag->SetToolTip(wxT("Input and add the tag element of HTML want to be deleted, such as 'IMG'"));
    label_1_copy->SetFont(wxFont(10, wxDEFAULT, wxNORMAL, wxBOLD, 0, wxT("")));
    comboAttrib->SetSelection(0);
	comboAttrib->SetToolTip(wxT("Input and add the attribute element of HTML want to be deleted, such as 'height'"));
    label_1_copy_1->SetFont(wxFont(10, wxDEFAULT, wxNORMAL, wxBOLD, 0, wxT("")));
    comboValue->SetSelection(0);
	comboValue->SetToolTip(wxT("Input and add the value element of HTML want to be deleted"));
    // end wxGlade

	InitConfig();
}

void HtmlOptimumConfigDialog::OnBtnTagAdd(wxCommandEvent &event)
{
	if(comboTag->GetStringSelection() == STR_TAG_ALL)
	{
		listTag->AppendString(STR_TAG_ALL);
	}
	else if(comboTag->GetStringSelection() == STR_TAG_DEFAULT)
	{
		OnBtnTagRemoveAll(event);

		listTag->AppendString(wxT("A"));
		listTag->AppendString(wxT("OBJECT"));
		listTag->AppendString(wxT("EMBED"));
		listTag->AppendString(wxT("PARAM"));
	}
	else if(comboTag->GetStringSelection() == STR_TAG_LINK)
	{
		listTag->AppendString(wxT("A"));
	}
	else if(comboTag->GetStringSelection() == STR_TAG_IMG)
	{
		listTag->AppendString(wxT("IMG"));
	}
	else if(comboTag->GetStringSelection() == STR_TAG_OBJECT)
	{
		listTag->AppendString(wxT("OBJECT"));
	}
	else if(comboTag->GetStringSelection() == STR_TAG_EMBED)
	{
		listTag->AppendString(wxT("EMBED"));
	}
	else if(comboTag->GetStringSelection() == STR_TAG_PARAM)
	{
		listTag->AppendString(wxT("PARAM"));
	}
	else if(comboTag->GetLabelText() != wxEmptyString)
	{
		listTag->AppendString(comboTag->GetLabelText());
	}
}


void HtmlOptimumConfigDialog::OnBtnTagRemove(wxCommandEvent &event)
{
	int idx = listTag->GetSelection();
	if(idx == -1)
		return;

	listTag->Delete(idx);
}


void HtmlOptimumConfigDialog::OnBtnTagRemoveAll(wxCommandEvent &event)
{
	int num = listTag->GetCount();
	while(num > 0)
	{
		listTag->Delete(0);
		-- num;
	}
}

void HtmlOptimumConfigDialog::OnBtnAttrAdd(wxCommandEvent &event)
{
	if(comboAttrib->GetStringSelection() == STR_ATTR_ALL)
	{
		listAttrib->AppendString(STR_ATTR_ALL);
	}
	else if(comboAttrib->GetStringSelection() == STR_ATTR_DEFAULT)
	{
		OnBtnAttrRemoveAll(event);

		listAttrib->AppendString(wxT("class"));
		listAttrib->AppendString(wxT("title"));
		listAttrib->AppendString(wxT("onclick"));
		listAttrib->AppendString(wxT("onmouseout"));
		listAttrib->AppendString(wxT("onmouseover"));
		listAttrib->AppendString(wxT("onmouseup"));
		listAttrib->AppendString(wxT("onmousedown"));
		listAttrib->AppendString(wxT("onselectstart"));
		listAttrib->AppendString(wxT("lingoes_pendfind"));
		listAttrib->AppendString(wxT("lingoes_pend"));
		listAttrib->AppendString(wxT("lingoes_find"));

	}
	else if(comboAttrib->GetStringSelection() == STR_ATTR_CLASS)
	{
		listAttrib->AppendString(wxT("class"));
	}
	else if(comboAttrib->GetStringSelection() == STR_ATTR_ONCLICK)
	{
		listAttrib->AppendString(wxT("onclick"));
	}
	else if(comboAttrib->GetStringSelection() == STR_ATTR_ONMOUSEOUT)
	{
		listAttrib->AppendString(wxT("onmouseout"));
	}
	else if(comboAttrib->GetStringSelection() == STR_ATTR_ONMOUSEOVER)
	{
		listAttrib->AppendString(wxT("onmouseover"));
	}
	else if(comboAttrib->GetStringSelection() == STR_ATTR_ONMOUSEUP)
	{
		listAttrib->AppendString(wxT("onmouseup"));
	}
	else if(comboAttrib->GetStringSelection() == STR_ATTR_ONMOUSEDOWN)
	{
		listAttrib->AppendString(wxT("onmousedown"));
	}
	else if(comboAttrib->GetStringSelection() == STR_ATTR_ONSELECTSTART)
	{
		listAttrib->AppendString(wxT("onselectstart"));
	}
	else if(comboAttrib->GetLabelText() != wxEmptyString)
	{
		listAttrib->AppendString(comboAttrib->GetLabelText());
	}
}


void HtmlOptimumConfigDialog::OnBtnAttrRemove(wxCommandEvent &event)
{
	int idx = listAttrib->GetSelection();
	if(idx == -1)
		return;

	listAttrib->Delete(idx);
}


void HtmlOptimumConfigDialog::OnBtnAttrRemoveAll(wxCommandEvent &event)
{
	int num = listAttrib->GetCount();
	while(num > 0)
	{
		listAttrib->Delete(0);
		-- num;
	}
}


void HtmlOptimumConfigDialog::OnBtnValAdd(wxCommandEvent &event)
{
	if(comboValue->GetStringSelection() == STR_VALUE_ALL)
	{
		listValue->AppendString(STR_VALUE_ALL);
	}
	else if(comboValue->GetStringSelection() == STR_VALUE_DEFAULT)
	{
		OnBtnValRemoveAll(event);
	}
	else if(comboValue->GetLabelText() != wxEmptyString)
	{
		listValue->AppendString(comboValue->GetLabelText());
	}
}


void HtmlOptimumConfigDialog::OnBtnValRemove(wxCommandEvent &event)
{
	int idx = listValue->GetSelection();
	if(idx == -1)
		return;

	listValue->Delete(idx);
}


void HtmlOptimumConfigDialog::OnBtnValRemoveAll(wxCommandEvent &event)
{
	int num = listValue->GetCount();
	while(num > 0)
	{
		listValue->Delete(0);
		-- num;
	}
}


void HtmlOptimumConfigDialog::OnBtnDefault(wxCommandEvent &event)
{
	comboTag->SetSelection(0);
	OnBtnTagAdd(event);
	comboAttrib->SetSelection(0);
	OnBtnAttrAdd(event);
	comboValue->SetSelection(0);
	OnBtnValAdd(event);
}


void HtmlOptimumConfigDialog::OnBtnOK(wxCommandEvent &event)
{
	UpdateConfig();

	wxDialog::EndModal(wxID_OK);
//	wxDialog::Close();
}



int HtmlOptimumConfigDialog::InitConfig()
{
	//if(_config == NULL)
	//	return -1;

	const CDocumentOutputObject::TKeyMap& mapKey = _mapKey;//_config->m_mapHtmlOptimumKey;

	CDocumentOutputObject::TKeyMap::const_iterator it = mapKey.begin();
	while(it != mapKey.end())
	{
		if(it->first == CDocumentOutputObject::KT_ALL_TAG)
		{
			listTag->AppendString(STR_TAG_ALL);
		}
		else if(it->first == CDocumentOutputObject::KT_ALL_ATTRIB)
		{
			listAttrib->AppendString(STR_ATTR_ALL);
		}
		else if(it->first == CDocumentOutputObject::KT_ALL_VALUE)
		{
			listValue->AppendString(STR_VALUE_ALL);
		}
		else if(it->first == CDocumentOutputObject::KT_TAG)
		{
			CDocumentOutputObject::TKeySet::const_iterator i = it->second.begin();
			while(i != it->second.end())
			{
				listTag->AppendString(*i);
				++ i;
			}
		}
		else if(it->first == CDocumentOutputObject::KT_ATTRIB)
		{
			CDocumentOutputObject::TKeySet::const_iterator i = it->second.begin();
			while(i != it->second.end())
			{
				listAttrib->AppendString(*i);
				++ i;
			}
		}
		else if(it->first == CDocumentOutputObject::KT_VALUE)
		{
			CDocumentOutputObject::TKeySet::const_iterator i = it->second.begin();
			while(i != it->second.end())
			{
				listValue->AppendString(*i);
				++ i;
			}
		}

		++ it;
	}

	return 0;
}

int HtmlOptimumConfigDialog::UpdateConfig()
{
	//if(_config == NULL)
	//	return -1;

	CDocumentOutputObject::TKeyMap& mapKey = _mapKey;//_config->m_mapHtmlOptimumKey;

	mapKey.clear();

	unsigned int num = 0;
	wxString str = wxEmptyString;
	while(num < listTag->GetCount())
	{
		str = listTag->GetString(num);
		if(str == STR_TAG_ALL)
		{
			CDocumentOutputObject::AddKey(&mapKey, CDocumentOutputObject::KT_ALL_TAG);
		}
		else
		{
			CDocumentOutputObject::AddKey(&mapKey, CDocumentOutputObject::KT_TAG, str);
		}
		++ num;
	}

	num = 0;
	while(num < listAttrib->GetCount())
	{
		str = listAttrib->GetString(num);
		if(str == STR_ATTR_ALL)
		{
			CDocumentOutputObject::AddKey(&mapKey, CDocumentOutputObject::KT_ALL_ATTRIB);
		}
		else
		{
			CDocumentOutputObject::AddKey(&mapKey, CDocumentOutputObject::KT_ATTRIB, str);
		}
		++ num;
	}

	num = 0;
	while(num < listValue->GetCount())
	{
		str = listValue->GetString(num);
		if(str == STR_VALUE_ALL)
		{
			CDocumentOutputObject::AddKey(&mapKey, CDocumentOutputObject::KT_ALL_VALUE);
		}
		else
		{
			CDocumentOutputObject::AddKey(&mapKey, CDocumentOutputObject::KT_VALUE, str);
		}
		++ num;
	}

	return 0;//_config->SaveHtmlOptimumConfig();
}