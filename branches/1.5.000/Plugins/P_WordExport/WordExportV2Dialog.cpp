// -*- C++ -*- generated by wxGlade 0.6.5 (standalone edition) on Mon May 21 15:48:48 2012

#include <wx/wfstream.h>
#include <wx/txtstrm.h>

#include "ResHelper.h"
#include "TinyHtmlParser.h"

#include "SeparatorDialog.h"

#include "WordExportV2Dialog.h"

using namespace TinyHtmlParser;


// begin wxGlade: ::extracode
// end wxGlade
const wxString WordExportV2Dialog::PLUGINS_TITLE		=	wxT("LingosHook Plugin - Word Export");
const wxString WordExportV2Dialog::EXPORT_FILENAME		=	wxT("LH_Export");
const wxString WordExportV2Dialog::INDEX_FILENAME		=	wxT("i.html");
const wxString WordExportV2Dialog::DATA_FILENAME		=	wxT("d.html");

WordExportV2Dialog::WordExportV2Dialog(CDBAccess* dbaccess, wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):
    wxDialog(parent, id, title, pos, size, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxTHICK_FRAME)
, _objDBAccess(dbaccess)
, _strSeparator(wxT("\n"))
, _eExportType(ET_HTML_INDEX_S)
{
    // begin wxGlade: WordExportV2Dialog::WordExportV2Dialog
    label_1 = new wxStaticText(this, wxID_ANY, wxT("Word Range Setting"));
    static_line_1 = new wxStaticLine(this, wxID_ANY);
    radioWordAll = new wxRadioButton(this, 7000, wxT("All Words"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
    checkWordPrefix = new wxCheckBox(this, 7001, wxT("Prefix"));
    textWordPrefix = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    checkWordSuffix = new wxCheckBox(this, 7002, wxT("Suffix"));
    textWordSuffix = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    label_3 = new wxStaticText(this, wxID_ANY, wxT("Tag Setting"));
    static_line_2 = new wxStaticLine(this, wxID_ANY);
    radioTagAll = new wxRadioButton(this, 7100, wxT("Any Tag"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
    radioTagSpecific = new wxRadioButton(this, 7101, wxT("Under Specific Tag"));
    const wxString *comboTagSpecific_choices = NULL;
    comboTagSpecific = new wxComboBox(this, 7102, wxT(""), wxDefaultPosition, wxDefaultSize, 0, comboTagSpecific_choices, wxCB_DROPDOWN|wxCB_READONLY);
    label_4 = new wxStaticText(this, wxID_ANY, wxT("Dictionary Setting"));
    static_line_3 = new wxStaticLine(this, wxID_ANY);
    radioDictAll = new wxRadioButton(this, 7200, wxT("Any Dictionary"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
    radioDictSpecific = new wxRadioButton(this, 7201, wxT("Under Specific Dictionary"));
    const wxString *comboDictSpecific_choices = NULL;
    comboDictSpecific = new wxComboBox(this, 7202, wxT(""), wxDefaultPosition, wxDefaultSize, 0, comboDictSpecific_choices, wxCB_DROPDOWN);
    label_5 = new wxStaticText(this, wxID_ANY, wxT("Output format Setting"));
    static_line_4 = new wxStaticLine(this, wxID_ANY);
    radioOutputHtmlSingle = new wxRadioButton(this, 7300, wxT("Output all data with index using HTML format to single file"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
    btnOutputOptimumSingle = new wxButton(this, 7301, wxT("Optimizer"));
    radioOutputHtmlMulti = new wxRadioButton(this, 7302, wxT("Output all data with index using HTML format to multi-files"));
    btnOutputOptimumMulti = new wxButton(this, 7303, wxT("Optimizer"));
    radioOutputHtml = new wxRadioButton(this, 7304, wxT("Output all data with HTML format to single file"));
    btnOutputOptimum = new wxButton(this, 7305, wxT("Optimizer"));
    radioOutputLAC = new wxRadioButton(this, 7306, wxT("Output all data with LAC format"));
    radioOutputWord = new wxRadioButton(this, 7307, wxT("Output word data with TEXT format"));
    btnOutputWord = new wxButton(this, 7308, wxT("Separator"));
    label_6 = new wxStaticText(this, wxID_ANY, wxT("Export Directory"));
    static_line_5 = new wxStaticLine(this, wxID_ANY);
    textExport = new wxTextCtrl(this, 7400, wxEmptyString);
    btnExportBrowse = new wxButton(this, 7401, wxT("Browse.."));
    static_line_6 = new wxStaticLine(this, wxID_ANY);
    btnExport = new wxButton(this, 7500, wxT("Export"));
    panel_1 = new wxPanel(this, wxID_ANY);
    btnClose = new wxButton(this, wxID_CLOSE, wxEmptyString);

    set_properties();
    do_layout();
    // end wxGlade
}


BEGIN_EVENT_TABLE(WordExportV2Dialog, wxDialog)
    // begin wxGlade: WordExportV2Dialog::event_table
	EVT_CHECKBOX(7000, WordExportV2Dialog::OnCheckWordClick)
    EVT_CHECKBOX(7001, WordExportV2Dialog::OnCheckWordClick)
    EVT_CHECKBOX(7002, WordExportV2Dialog::OnCheckWordClick)
	EVT_RADIOBUTTON(7100, WordExportV2Dialog::OnRadioTagClick)
    EVT_RADIOBUTTON(7101, WordExportV2Dialog::OnRadioTagClick)
	EVT_RADIOBUTTON(7200, WordExportV2Dialog::OnRadioDictClick)
    EVT_RADIOBUTTON(7201, WordExportV2Dialog::OnRadioDictClick)
    EVT_RADIOBUTTON(7300, WordExportV2Dialog::OnRadioOutputClick)
    EVT_RADIOBUTTON(7302, WordExportV2Dialog::OnRadioOutputClick)
    EVT_RADIOBUTTON(7304, WordExportV2Dialog::OnRadioOutputClick)
	EVT_RADIOBUTTON(7306, WordExportV2Dialog::OnRadioOutputClick)
	EVT_RADIOBUTTON(7307, WordExportV2Dialog::OnRadioOutputClick)
    EVT_BUTTON(7301, WordExportV2Dialog::OnBtnOptimizer)
	EVT_BUTTON(7303, WordExportV2Dialog::OnBtnOptimizer)
	EVT_BUTTON(7305, WordExportV2Dialog::OnBtnOptimizer)
    EVT_BUTTON(7308, WordExportV2Dialog::OnBtnSeparator)
    EVT_TEXT(7400, WordExportV2Dialog::OnTextExport)
    EVT_BUTTON(7401, WordExportV2Dialog::OnBtnBrowse)
    EVT_BUTTON(7500, WordExportV2Dialog::OnBtnExport)
	EVT_BUTTON(wxID_CLOSE, WordExportV2Dialog::OnBtnClose)
    // end wxGlade
END_EVENT_TABLE();


void WordExportV2Dialog::OnCheckWordClick(wxCommandEvent &event)
{
	textWordPrefix->Enable(checkWordPrefix->IsChecked());
	textWordSuffix->Enable(checkWordSuffix->IsChecked());
}


void WordExportV2Dialog::OnRadioTagClick(wxCommandEvent &event)
{
	comboTagSpecific->Enable(radioTagSpecific->GetValue());

	if(radioTagSpecific->GetValue())
	{
		InitTag();
	}
}


void WordExportV2Dialog::OnRadioDictClick(wxCommandEvent &event)
{
	comboDictSpecific->Enable(radioDictSpecific->GetValue());
	if(radioDictSpecific->GetValue() && comboDictSpecific->GetCount() == 0)
	{
		InitDict();
	}
}


void WordExportV2Dialog::OnRadioOutputClick(wxCommandEvent &event)
{
	btnOutputOptimumSingle->Enable(radioOutputHtmlSingle->GetValue());
	btnOutputOptimumMulti->Enable(radioOutputHtmlMulti->GetValue());
	btnOutputOptimum->Enable(radioOutputHtml->GetValue());
	btnOutputWord->Enable(radioOutputWord->GetValue());
	if(radioOutputWord->GetValue())
	{
		if(radioDictSpecific->GetValue())
			radioDictAll->SetValue(true);

		radioDictSpecific->SetValue(false);
		radioDictSpecific->Enable(false);
		comboDictSpecific->Enable(false);
	}
	else
	{
		radioDictSpecific->Enable(true);
	}
}


void WordExportV2Dialog::OnBtnOptimizer(wxCommandEvent &event)
{
	HtmlOptimumConfigDialog dlg(_mapOptimumKey, this, wxID_ANY, wxEmptyString);
    if(dlg.ShowModal() == wxID_OK)
    {
    }
}

void WordExportV2Dialog::OnBtnSeparator(wxCommandEvent &event)
{
	SeparatorDialog dlg(this, wxID_ANY, wxEmptyString);
	if(dlg.ShowModal() == wxID_OK)
	{
		_strSeparator = dlg.m_strSeparator;
	}
}


void WordExportV2Dialog::OnTextExport(wxCommandEvent &event)
{
	btnExport->Enable(!textExport->GetValue().IsEmpty());
}


void WordExportV2Dialog::OnBtnBrowse(wxCommandEvent &event)
{
	wxDirDialog dlg(this, wxT("Select output directory"), wxEmptyString);
    //wxFileDialog dlg(this, wxT("Select output file"), wxEmptyString, wxT("LingosHook_WordExport.txt"), wxT("Text Files(*.txt)|*.txt|CSV Files(*.csv)|*.csv|All Files(*.*)|*.*"));
    if(dlg.ShowModal() == wxID_OK)
    {
		textExport->SetValue(dlg.GetPath());
    }
}


void WordExportV2Dialog::OnBtnExport(wxCommandEvent &event)
{
	if(radioOutputHtmlSingle->GetValue())
	{
		_eExportType = ET_HTML_INDEX_S;
	}
	if(radioOutputHtmlMulti->GetValue())
	{
		_eExportType = ET_HTML_INDEX_M;
	}
	else if(radioOutputHtml->GetValue())
	{
		_eExportType = ET_HTML;
	}
	else if(radioOutputLAC->GetValue())
	{
		_eExportType = ET_LAC;
	}
	else if(radioOutputWord->GetValue())
	{
		_eExportType = ET_TEXT;
	}

	if(Export() == 0)
	{
		wxMessageBox(wxT("Export completed successfully."), PLUGINS_TITLE, wxCENTRE | wxOK);
	}
	else
	{
		wxMessageBox(wxT("Export Failed."), PLUGINS_TITLE, wxCENTRE | wxOK | wxICON_ERROR);
	}
}

void WordExportV2Dialog::OnBtnClose(wxCommandEvent &event)
{
    Close();
}

// wxGlade: add WordExportV2Dialog event handlers


void WordExportV2Dialog::set_properties()
{
    // begin wxGlade: WordExportV2Dialog::set_properties
    SetTitle(PLUGINS_TITLE);
    label_1->SetFont(wxFont(9, wxDEFAULT, wxNORMAL, wxBOLD, 0, wxT("")));
    textWordPrefix->Enable(false);
    textWordSuffix->Enable(false);
    label_3->SetFont(wxFont(9, wxDEFAULT, wxNORMAL, wxBOLD, 0, wxT("")));
    comboTagSpecific->Enable(false);
    label_4->SetFont(wxFont(9, wxDEFAULT, wxNORMAL, wxBOLD, 0, wxT("")));
    comboDictSpecific->SetToolTip(wxT("Input the ID of dictionary"));
    comboDictSpecific->Enable(false);
    label_5->SetFont(wxFont(9, wxDEFAULT, wxNORMAL, wxBOLD, 0, wxT("")));
    btnOutputOptimumMulti->Enable(false);
    btnOutputOptimum->Enable(false);
    btnOutputWord->Enable(false);
    label_6->SetFont(wxFont(9, wxDEFAULT, wxNORMAL, wxBOLD, 0, wxT("")));
    btnExport->Enable(false);
    // end wxGlade

	InitOptimumKey();
}


void WordExportV2Dialog::do_layout()
{
    // begin wxGlade: WordExportV2Dialog::do_layout
    wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_26 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_23 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_27 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_25 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_24 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_17 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_19 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_21 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_20 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_20_copy_copy = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_20_copy = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_18 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_13 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_15 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_16 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_14 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_9 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_11 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_12 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_10 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_2 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_4 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_8 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_7 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_5 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_3 = new wxBoxSizer(wxHORIZONTAL);
    sizer_3->Add(label_1, 0, 0, 0);
    sizer_3->Add(static_line_1, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_VERTICAL, 4);
    sizer_2->Add(sizer_3, 0, wxALL|wxEXPAND, 8);
    sizer_5->Add(radioWordAll, 0, wxALIGN_CENTER_VERTICAL, 0);
    sizer_4->Add(sizer_5, 0, wxTOP|wxBOTTOM|wxEXPAND, 4);
    sizer_7->Add(checkWordPrefix, 0, wxALIGN_CENTER_VERTICAL, 0);
    sizer_7->Add(textWordPrefix, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 16);
    sizer_4->Add(sizer_7, 0, wxBOTTOM|wxEXPAND, 4);
    sizer_8->Add(checkWordSuffix, 0, wxALIGN_CENTER_VERTICAL, 0);
    sizer_8->Add(textWordSuffix, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 16);
    sizer_4->Add(sizer_8, 0, wxBOTTOM|wxEXPAND, 4);
    sizer_2->Add(sizer_4, 0, wxLEFT|wxEXPAND, 32);
    sizer_1->Add(sizer_2, 0, wxALL|wxEXPAND, 4);
    sizer_10->Add(label_3, 0, 0, 0);
    sizer_10->Add(static_line_2, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_VERTICAL, 4);
    sizer_9->Add(sizer_10, 0, wxALL|wxEXPAND, 4);
    sizer_11->Add(radioTagAll, 0, wxTOP|wxBOTTOM, 4);
    sizer_12->Add(radioTagSpecific, 0, wxALIGN_CENTER_VERTICAL, 0);
    sizer_12->Add(comboTagSpecific, 1, wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_CENTER_VERTICAL, 16);
    sizer_11->Add(sizer_12, 0, wxTOP|wxBOTTOM|wxEXPAND, 4);
    sizer_9->Add(sizer_11, 0, wxLEFT|wxEXPAND, 32);
    sizer_1->Add(sizer_9, 0, wxALL|wxEXPAND, 4);
    sizer_14->Add(label_4, 0, 0, 0);
    sizer_14->Add(static_line_3, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_VERTICAL, 4);
    sizer_13->Add(sizer_14, 0, wxALL|wxEXPAND, 4);
    sizer_15->Add(radioDictAll, 0, wxTOP|wxBOTTOM, 4);
    sizer_16->Add(radioDictSpecific, 0, wxALIGN_CENTER_VERTICAL, 0);
    sizer_16->Add(comboDictSpecific, 1, wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_CENTER_VERTICAL, 16);
    sizer_15->Add(sizer_16, 0, wxTOP|wxBOTTOM|wxEXPAND, 4);
    sizer_13->Add(sizer_15, 0, wxLEFT|wxRIGHT|wxEXPAND, 32);
    sizer_1->Add(sizer_13, 0, wxALL|wxEXPAND, 4);
    sizer_18->Add(label_5, 0, 0, 0);
    sizer_18->Add(static_line_4, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_VERTICAL, 4);
    sizer_17->Add(sizer_18, 0, wxALL|wxEXPAND, 4);
    sizer_20_copy->Add(radioOutputHtmlSingle, 1, wxALIGN_CENTER_VERTICAL, 0);
    sizer_20_copy->Add(btnOutputOptimumSingle, 0, wxLEFT|wxRIGHT|wxALIGN_CENTER_VERTICAL, 4);
    sizer_19->Add(sizer_20_copy, 0, wxTOP|wxBOTTOM|wxEXPAND, 4);
    sizer_20_copy_copy->Add(radioOutputHtmlMulti, 1, wxALIGN_CENTER_VERTICAL, 0);
    sizer_20_copy_copy->Add(btnOutputOptimumMulti, 0, wxLEFT|wxRIGHT|wxALIGN_CENTER_VERTICAL, 4);
    sizer_19->Add(sizer_20_copy_copy, 0, wxTOP|wxBOTTOM|wxEXPAND, 4);
    sizer_20->Add(radioOutputHtml, 1, wxALIGN_CENTER_VERTICAL, 0);
    sizer_20->Add(btnOutputOptimum, 0, wxLEFT|wxRIGHT|wxALIGN_CENTER_VERTICAL, 4);
    sizer_19->Add(sizer_20, 0, wxTOP|wxBOTTOM|wxEXPAND, 4);
    sizer_19->Add(radioOutputLAC, 0, wxTOP|wxBOTTOM|wxALIGN_CENTER_VERTICAL, 4);
    sizer_21->Add(radioOutputWord, 1, wxALIGN_CENTER_VERTICAL, 0);
    sizer_21->Add(btnOutputWord, 0, wxLEFT|wxRIGHT|wxALIGN_CENTER_VERTICAL, 4);
    sizer_19->Add(sizer_21, 0, wxTOP|wxBOTTOM|wxEXPAND, 4);
    sizer_17->Add(sizer_19, 0, wxLEFT|wxRIGHT|wxEXPAND, 32);
    sizer_1->Add(sizer_17, 0, wxALL|wxEXPAND, 4);
    sizer_24->Add(label_6, 0, 0, 0);
    sizer_24->Add(static_line_5, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_VERTICAL, 4);
    sizer_23->Add(sizer_24, 0, wxALL|wxEXPAND, 4);
    sizer_25->Add(textExport, 1, wxTOP|wxBOTTOM|wxEXPAND|wxALIGN_CENTER_VERTICAL, 4);
    sizer_25->Add(btnExportBrowse, 0, wxALL, 4);
    sizer_27->Add(sizer_25, 0, wxTOP|wxBOTTOM|wxEXPAND, 4);
    sizer_23->Add(sizer_27, 0, wxLEFT|wxRIGHT|wxEXPAND, 32);
    sizer_1->Add(sizer_23, 0, wxALL|wxEXPAND, 4);
    sizer_1->Add(static_line_6, 0, wxALL|wxEXPAND, 4);
    sizer_26->Add(btnExport, 1, wxTOP|wxBOTTOM|wxALIGN_CENTER_VERTICAL, 0);
    sizer_26->Add(panel_1, 3, wxEXPAND, 0);
    sizer_26->Add(btnClose, 0, wxTOP|wxBOTTOM|wxALIGN_CENTER_VERTICAL, 0);
    sizer_1->Add(sizer_26, 0, wxALL|wxEXPAND, 16);
    SetSizer(sizer_1);
	sizer_1->Fit(this);
    Layout();
    Centre();
    // end wxGlade
}

void WordExportV2Dialog::InitOptimumKey()
{
    CDocumentOutputObject::AddKey(&_mapOptimumKey, CDocumentOutputObject::KT_TAG, wxT("A"));
    CDocumentOutputObject::AddKey(&_mapOptimumKey, CDocumentOutputObject::KT_TAG, wxT("PARAM"));
    CDocumentOutputObject::AddKey(&_mapOptimumKey, CDocumentOutputObject::KT_TAG, wxT("EMBED"));
    CDocumentOutputObject::AddKey(&_mapOptimumKey, CDocumentOutputObject::KT_TAG, wxT("OBJECT"));
    CDocumentOutputObject::AddKey(&_mapOptimumKey, CDocumentOutputObject::KT_ATTRIB, wxT("class"));	
	CDocumentOutputObject::AddKey(&_mapOptimumKey, CDocumentOutputObject::KT_ATTRIB, wxT("title"));
	CDocumentOutputObject::AddKey(&_mapOptimumKey, CDocumentOutputObject::KT_ATTRIB, wxT("onclick"));
	CDocumentOutputObject::AddKey(&_mapOptimumKey, CDocumentOutputObject::KT_ATTRIB, wxT("onmouseout"));
	CDocumentOutputObject::AddKey(&_mapOptimumKey, CDocumentOutputObject::KT_ATTRIB, wxT("onmouseup"));
	CDocumentOutputObject::AddKey(&_mapOptimumKey, CDocumentOutputObject::KT_ATTRIB, wxT("onmousedown"));
	CDocumentOutputObject::AddKey(&_mapOptimumKey, CDocumentOutputObject::KT_ATTRIB, wxT("onselectstart"));
	CDocumentOutputObject::AddKey(&_mapOptimumKey, CDocumentOutputObject::KT_ATTRIB, wxT("lingoes_pendfind"));
	CDocumentOutputObject::AddKey(&_mapOptimumKey, CDocumentOutputObject::KT_ATTRIB, wxT("lingoes_pend"));
	CDocumentOutputObject::AddKey(&_mapOptimumKey, CDocumentOutputObject::KT_ATTRIB, wxT("lingoes_find"));
}

int WordExportV2Dialog::InitTag()
{
	if(_objDBAccess == NULL)
		return -1;
	
	try
	{
		CDBAccess::TResult res = _objDBAccess->Database().ExecuteQuery("SELECT TagID, Title FROM TagTable");
        if(!res.IsOk())
            return -1;
		while(res.NextRow())
		{
			comboTagSpecific->AppendString(wxString::Format(wxT("%d:%s"), res.GetInt(0), res.GetAsString(1)));
		}
		comboTagSpecific->SetSelection(0);

	}
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

int WordExportV2Dialog::InitDict()
{
	if(_objDBAccess == NULL)
		return -1;
	
	try
	{
		CDBAccess::TResult res = _objDBAccess->Database().ExecuteQuery("SELECT DictID, Title FROM DictTable");
        if(!res.IsOk())
            return -1;
		while(res.NextRow())
		{
			comboDictSpecific->AppendString(wxString::Format(wxT("%s:%s"), res.GetAsString(0), res.GetAsString(1)));
		}
		
		comboDictSpecific->AppendString(wxT("0895DFE8DB67F9409DB285590D870EDD:Dict.CN online"));
		comboDictSpecific->AppendString(wxT("D4722835273E184582F2D24696A738EA:Longman Dictionary of Contemporary English"));
		comboDictSpecific->AppendString(wxT("25D62D261B9B6943BE86B7DCF8F9D255:Concise English Synonym & Antonym Dictionary"));
		comboDictSpecific->AppendString(wxT("74BF7ECCE3A9AC43B5E8902261A19B29:Hudong Wiki"));
		comboDictSpecific->AppendString(wxT("C68363F3F2F8F04997C0552720B30742:Google Web Definition"));
		comboDictSpecific->AppendString(wxT("3A259F0851B07D4AAA30FBFA43686AB3:Dr.eye Online"));
		comboDictSpecific->AppendString(wxT("9455286C1F1BC84985523D2608B6A913:Web Translation"));

		comboDictSpecific->SetSelection(0);

	}
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

const wxString WordExportV2Dialog::MakeExportPath() const
{
	wxString file = textExport->GetValue();
	if(file.Right(1) != wxT("\\"))
	{
		file += wxT("\\");
	}
	return file;
}

const wxString WordExportV2Dialog::GetExportFilename() const
{
	return MakeExportPath() + EXPORT_FILENAME;
}

const wxString WordExportV2Dialog::GetIndexFilename() const
{
	return MakeExportPath() + INDEX_FILENAME;
}

const wxString WordExportV2Dialog::GetDataFilename() const
{
	return MakeExportPath() + DATA_FILENAME;
}

const wxString WordExportV2Dialog::MakeExportSql()
{
	wxString sql = wxEmptyString;
	
	wxArrayString cond;

	if(_eExportType != ET_TEXT)
	{
		sql = wxT("SELECT WordTable.Word, WordTable.WordID, WordTable.SrcID, SrcDataTable.HTML FROM WordTable,SrcDataTable");
		cond.Add(wxT("(WordTable.SrcID=SrcDataTable.SrcID)"));
	}
	else
	{
		sql = wxT("SELECT WordTable.Word FROM WordTable");
	}	

	if(radioTagSpecific->GetValue())
	{
		sql += wxT(",TagIndexTable");

		wxString tagid = comboTagSpecific->GetStringSelection();
		tagid = tagid.substr(0, tagid.find_first_of(wxT(":")));

		cond.Add(wxString::Format(wxT("(WordTable.WordID=TagIndexTable.WordID AND TagIndexTable.TagID=%s)"), tagid));
	}
			
	if(checkWordPrefix->GetValue() && !textWordPrefix->GetValue().IsEmpty())
	{
		cond.Add(wxString::Format(wxT("(WordTable.Word LIKE '%s%%')"), textWordPrefix->GetValue()));
	}

	if(checkWordSuffix->GetValue() && !textWordSuffix->GetValue().IsEmpty())
	{
		cond.Add(wxString::Format(wxT("(WordTable.Word LIKE '%%%s')"), textWordSuffix->GetValue()));
	}

	if(radioDictSpecific->GetValue() && !comboDictSpecific->GetValue().IsEmpty())
	{
		wxString str = comboDictSpecific->GetValue();
		wxString::size_type pos = str.find(wxT(":"));
		if(pos != wxString::npos)
		{
			str = str.substr(0, pos);
		}
		_strFilterDict = wxT("\"") + str + wxT("\"");;
	}
	else
	{
		_strFilterDict.Empty();
	}

	if(!cond.IsEmpty())
	{
		sql += wxT(" WHERE ");
		for(size_t sz = 0; sz < cond.GetCount(); ++ sz)
		{
			sql += cond.Item(sz);
			if(sz != cond.GetCount() -1)
			{
				sql += wxT(" AND ");
			}
		}
	}

	sql += wxT(" ORDER BY UPPER(WordTable.Word)");

	return sql;
}

int WordExportV2Dialog::Export()
{
	if(_objDBAccess == NULL)
		return -1;

	wxString sql = MakeExportSql();

    CDBAccess::TDatabase& db = _objDBAccess->Database();

    CDBAccess::TQuery query = db.PrepareStatement(sql);
    CDBAccess::TResult res = query.ExecuteQuery();
    if(!res.IsOk())
        return -1;

	switch(_eExportType)
	{
	case ET_HTML_INDEX_S:
		return ExportHtmlWithIndexSingle(res);
	case ET_HTML_INDEX_M:
		return ExportHtmlWithIndexMulti(res);
	case ET_HTML:
		return ExportHtml(res);
	case ET_LAC:
		return ExportLAC(res);
	case ET_TEXT:
		return ExportText(res);
	default:
		return -1;
	}

	return -1;
}

int WordExportV2Dialog::ExportHtmlWithIndexSingle(CDBAccess::TResult& res)
{
	//make sub-directory
	wxString filename = GetExportFilename();
	//export frame
	wxFileOutputStream ofs1(filename + wxT(".html"));
	if(!ofs1.IsOk())
		return -1;	
	wxTextOutputStream tos1(ofs1);
	
	tos1 << wxT("<html><head><title>LingosHook Export</title></head><frameset cols=\"30%,70%\"><frame src=\"i.html\" name=\"i\"></frame><frame src=\"d.html\" name=\"d\"></frame></frameset>");

	ofs1.Close();

	//export index & data

	wxFileOutputStream ofs2(GetIndexFilename());// wxT("i.html"));
	if(!ofs2.IsOk())
		return -1;	
	wxTextOutputStream tos2(ofs2);

	wxFileOutputStream ofs3(GetDataFilename());//wxT("d.html"));
	if(!ofs3.IsOk())
		return -1;	
	wxTextOutputStream tos3(ofs3);

	CDocumentOutputObject::TKeyMap mapKey = _mapOptimumKey;

	CDocumentOutputObject::AddKey(&mapKey, CDocumentOutputObject::KT_TAG, wxT("HTML"));
	CDocumentOutputObject::AddKey(&mapKey, CDocumentOutputObject::KT_TAG, wxT("TITLE"));
	CDocumentOutputObject::AddKey(&mapKey, CDocumentOutputObject::KT_TAG, wxT("HEAD"));
	CDocumentOutputObject::AddKey(&mapKey, CDocumentOutputObject::KT_TAG, wxT("BODY"));

	tos2 << wxT("<HTML><HEAD><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"/><STYLE>A:link{TEXT-DECORATION:none;}</STYLE></HEAD><BODY>");
	tos3 << wxT("<HTML><HEAD><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"/></HEAD><BODY>");

	tos2 << wxT("<DIV style=\"PADDING-TOP: 5px; PADDING-LEFT: 10px; FONT-FAMILY: 'Tahoma';\"><FONT style=\"FONT-SIZE:13pt\"><B>");
	wxString ret = wxEmptyString;
	int srcid = -1, psrcid = -1;
	while(res.NextRow())
	{
		srcid = res.GetInt(2);
		if(srcid != psrcid)
		{
			ret = wxEmptyString;
			FilterHtml(res.GetString(3), ret, mapKey);
			if(ret.empty())
			{
				continue;
			}

			tos3 << wxT("<A name=\"") << srcid << wxT("\">") << ret << wxT("</A>");//res.GetString(3);
			tos3 << wxT("<DIV style=\"BORDER-TOP:#7070dd 2px solid; PADDING-TOP:5px\"></DIV>");

			psrcid = srcid;
		}

		tos2 << wxT("<A href=\"d.html#") << srcid << wxT("\" target=\"d\">") << res.GetString(0) << wxT("</A><BR/>");
	}

	tos2 << wxT("</B></FONT></DIV><DIV style=\"position:absolute;right:10px;bottom:25px;FONT-FAMILY:'Tahoma';\"><FONT sytle=\"FONT-SIZE:12\"><B>Powered by <A href=\"http://codejie.tk\" target=\"d\">LingosHook</A></B></FONT></DIV></BODY></HTML>");
	ofs2.Close();
	tos3 << wxT("</BODY></HTML>");
	ofs3.Close();

	return 0;
}

int WordExportV2Dialog::ExportHtmlWithIndexMulti(CDBAccess::TResult& res)
{
	//make sub-directory
	wxString filename = GetExportFilename();
	//export frame
	wxFileOutputStream ofs1(filename + wxT(".html"));
	if(!ofs1.IsOk())
		return -1;	
	wxTextOutputStream tos1(ofs1);
	
	tos1 << wxT("<html><head><title>LingosHook Export</title></head><frameset cols=\"30%,70%\"><frame src=\"i.html\" name=\"i\"></frame><frame src=\"d.html\" name=\"d\"></frame></frameset>");

	ofs1.Close();

	//export index & data

	wxFileOutputStream ofs2(GetIndexFilename());//wxT("i.html"));
	if(!ofs2.IsOk())
		return -1;	
	wxTextOutputStream tos2(ofs2);

	wxFileOutputStream ofs3(GetDataFilename());//wxT("d.html"));
	if(!ofs3.IsOk())
		return -1;	
	wxTextOutputStream tos3(ofs3);

	tos3 << ResHelper::MakeHomepageString();//STR_HOMEPAGE;//wxT("<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"/><title>LingosHook Export</title></head><body><div align=\"center\"><FONT sytle=\"FONT-SIZE:12\"><B>Power by <A href=\"http://codejie.tk\" target=\"d\">LingosHook</A></B></FONT></div></body></html>");
	ofs3.Close();

	CDocumentOutputObject::TKeyMap mapKey = _mapOptimumKey;

	CDocumentOutputObject::AddKey(&mapKey, CDocumentOutputObject::KT_TAG, wxT("HTML"));
	CDocumentOutputObject::AddKey(&mapKey, CDocumentOutputObject::KT_TAG, wxT("TITLE"));
	CDocumentOutputObject::AddKey(&mapKey, CDocumentOutputObject::KT_TAG, wxT("HEAD"));
	CDocumentOutputObject::AddKey(&mapKey, CDocumentOutputObject::KT_TAG, wxT("BODY"));

	tos2 << wxT("<HTML><HEAD><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"/><STYLE>A:link{TEXT-DECORATION:none;}</STYLE></HEAD><BODY>");
	tos2 << wxT("<DIV style=\"PADDING-TOP: 5px; PADDING-LEFT: 10px; FONT-FAMILY: 'Tahoma';\"><FONT style=\"FONT-SIZE:13pt\"><B>");

	wxString ret = wxEmptyString;
	int srcid = -1, psrcid = -1;
	wxString psrcname = wxEmptyString;	
	wxFileOutputStream* ofs = NULL;
	while(res.NextRow())
	{
		srcid = res.GetInt(2);
		if(srcid != psrcid)
		{
			ret = wxEmptyString;
			FilterHtml(res.GetString(3), ret, mapKey);
			if(ret.empty())
			{
				continue;
			}

			psrcname = res.GetString(0);
			ofs = new wxFileOutputStream(MakeExportPath() + psrcname + wxT(".html"));
			wxTextOutputStream tos(*ofs);

		//ret.insert(12, wxT("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"/>"));
			tos << wxT("<HTML><HEAD><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"/></HEAD><BODY>");
			tos << ret;
			tos << wxT("</BODY></HTML>");
			ofs->Close();
			//it is my fist 'delete' in past three years
			delete ofs;

			psrcid = srcid;
		}

		tos2 << wxT("<A href=\"") << psrcname << wxT(".html\" target=\"d\">") << res.GetString(0) << wxT("</A><BR/>");
	}

	tos2 << wxT("</B></FONT></DIV><DIV style=\"position:absolute;right:10px;bottom:25px;FONT-FAMILY: 'Tahoma';\"><FONT sytle=\"FONT-SIZE:12\"><B>Powered by <A href=\"http://codejie.tk\" target=\"d\">LingosHook</A></B></FONT></DIV></BODY></HTML>");
	ofs2.Close();

	return 0;
}

int WordExportV2Dialog::ExportHtml(CDBAccess::TResult& res)
{
	wxFileOutputStream ofs(GetExportFilename() + wxT(".html"));
	if(!ofs.IsOk())
		return -1;

    wxTextOutputStream tos(ofs);

	CDocumentOutputObject::TKeyMap mapKey = _mapOptimumKey;

	CDocumentOutputObject::AddKey(&mapKey, CDocumentOutputObject::KT_TAG, wxT("HTML"));
	CDocumentOutputObject::AddKey(&mapKey, CDocumentOutputObject::KT_TAG, wxT("TITLE"));
	CDocumentOutputObject::AddKey(&mapKey, CDocumentOutputObject::KT_TAG, wxT("HEAD"));
	CDocumentOutputObject::AddKey(&mapKey, CDocumentOutputObject::KT_TAG, wxT("BODY"));
//	CDocumentOutputObject::AddKey(&_mapOptimumKey, CDocumentOutputObject::KT_ATTRIB, wxT("style"));

	tos << wxT("<HTML><HEAD><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"/></HEAD><BODY>");

	wxString ret = wxEmptyString;
	while(res.NextRow())
	{
		ret = wxEmptyString;
		FilterHtml(res.GetString(3), ret, mapKey);
		if(ret.empty())
		{
			continue;
		}

		tos << wxT("<FONT style=\"FONT-SIZE: 11pt\"><B>") << res.GetString(0) << wxT("</B></FONT>");

		tos << ret;//res.GetString(3);
		tos << wxT("<DIV style=\"BORDER-TOP:#7070dd 2px solid; PADDING-TOP:5px\"></DIV>");
	}
//	tos << ret;

	tos << wxT("</BODY></HTML>");
	ofs.Close();

	return 0;
}

int WordExportV2Dialog::ExportLAC(CDBAccess::TResult& res)
{
	wxString filename = GetExportFilename();

	CDocumentOutputObject::TKeyMap mapKey;

	CDocumentOutputObject::AddKey(&mapKey, CDocumentOutputObject::KT_TAG, wxT("IMG"));
    CDocumentOutputObject::AddKey(&mapKey, CDocumentOutputObject::KT_TAG, wxT("A"));
    CDocumentOutputObject::AddKey(&mapKey, CDocumentOutputObject::KT_TAG, wxT("PARAM"));
    CDocumentOutputObject::AddKey(&mapKey, CDocumentOutputObject::KT_TAG, wxT("EMBED"));
    CDocumentOutputObject::AddKey(&mapKey, CDocumentOutputObject::KT_TAG, wxT("OBJECT"));
    CDocumentOutputObject::AddKey(&mapKey, CDocumentOutputObject::KT_ATTRIB, wxT("class"));	
	CDocumentOutputObject::AddKey(&mapKey, CDocumentOutputObject::KT_ATTRIB, wxT("title"));
	CDocumentOutputObject::AddKey(&mapKey, CDocumentOutputObject::KT_ATTRIB, wxT("onclick"));
	CDocumentOutputObject::AddKey(&mapKey, CDocumentOutputObject::KT_ATTRIB, wxT("onmouseout"));
	CDocumentOutputObject::AddKey(&mapKey, CDocumentOutputObject::KT_ATTRIB, wxT("onmouseup"));
	CDocumentOutputObject::AddKey(&mapKey, CDocumentOutputObject::KT_ATTRIB, wxT("onmousedown"));
	CDocumentOutputObject::AddKey(&mapKey, CDocumentOutputObject::KT_ATTRIB, wxT("onselectstart"));
	CDocumentOutputObject::AddKey(&mapKey, CDocumentOutputObject::KT_ATTRIB, wxT("lingoes_pendfind"));
	CDocumentOutputObject::AddKey(&mapKey, CDocumentOutputObject::KT_ATTRIB, wxT("lingoes_pend"));
	CDocumentOutputObject::AddKey(&mapKey, CDocumentOutputObject::KT_ATTRIB, wxT("lingoes_find"));

	//open db
	try
	{
		CDBAccess::TDatabase db;
		db.Open(filename + wxT(".db3"), wxEmptyString);
		
        RemoveTable(db, wxT("info"));
        const char* sql = "CREATE TABLE info (item INTEGER PRIMARY KEY AUTOINCREMENT, value VARCHAR(32))";
        db.ExecuteUpdate(sql);

        RemoveTable(db, wxT("word"));
        sql = "CREATE TABLE word (srcid INTEGER, word VARCHAR(32))";
        db.ExecuteUpdate(sql);

        RemoveTable(db, wxT("src"));
        sql = "CREATE TABLE src (srcid INTEGER PRIMARY KEY AUTOINCREMENT, html TEXT)";
        db.ExecuteUpdate(sql);

        CDBAccess::TQuery qsrc = db.PrepareStatement("INSERT INTO src (html) VALUES (?)");
        CDBAccess::TQuery qword = db.PrepareStatement("INSERT INTO word (srcid, word) VALUES (?, ?)");

        int count = 0;
        int p = -1, n = -1;
        int srcid = 0;

		wxString ret = wxEmptyString;
        while(res.NextRow())
        {
            n = res.GetInt(2);
            if(p != n)
            {
                qsrc.ClearBindings();

                ret = wxEmptyString;
                FilterHtml(res.GetString(3), ret, mapKey);
				if(ret.empty())
				{
					continue;
				}
				ret = wxT("<HTML><HEAD><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"/></HEAD><BODY>") + ret;
				ret += wxT("</BODY></HTML>");

                qsrc.Bind(1, ret/*res.GetString(2)*/);
                qsrc.ExecuteUpdate();

                p = n;
                srcid = db.GetLastRowId().ToLong();
            }

            qword.ClearBindings();
            qword.Bind(1, srcid);
            qword.Bind(2, res.GetString(0));
            qword.ExecuteUpdate();
        }

        CDBAccess::TQuery qinfo = db.PrepareStatement("INSERT INTO info (item, value) VALUES (?, ?)");
        qinfo.Bind(1, 1);
        qinfo.Bind(2, 1);//version
        qinfo.ExecuteUpdate();

        qinfo.ClearBindings();
        qinfo.Bind(1, 2);
        qinfo.Bind(2, wxT("cde"));
        qinfo.ExecuteQuery();

		db.Close();

	}
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }

	return 0;
}

int WordExportV2Dialog::ExportText(CDBAccess::TResult& res)
{
	wxFileOutputStream ofs(GetExportFilename() + wxT(".txt"));
	if(!ofs.IsOk())
		return -1;

    wxTextOutputStream tos(ofs);

	while(res.NextRow())
	{
		tos << res.GetString(0) << _strSeparator;
	}

	ofs.Close();

	return 0;
}

///

int WordExportV2Dialog::FilterHtml(const wxString &input, wxString &output, const CDocumentOutputObject::TKeyMap& mapKey) const
{
	CDocumentObject doc;

    try
    {
		if(doc.Load(input.c_str(), true, true) != 0)
        {
            return -1;
        }
    }
    catch(CExceptionObject& e)
    {
        return -1;
    }

	const CElementObject* div = doc.Root(), *dict = NULL;
	div = doc.FindFirstElement(L"DIV");
	const CAttributeObject * id = NULL;
	while(div != NULL)
	{
		id = doc.FindAttribute(div, L"id");
		if(id != NULL && id->value == L"\"lingoes_dictarea\"")
		{
			if(div == NULL)
			{
				return -1;
			}
			dict = div->sibling;
			if(dict == NULL)
			{
				return -1;
			}
			if(!_strFilterDict.empty())
			{
				id = doc.FindAttribute(dict, L"dictid");
				if(id == NULL || id->value != _strFilterDict.c_str())
				{
					div = doc.FindNextElement();
					continue;
				}
			}

			if(CDocumentOutputObject::Rewrite(doc, output, dict, &mapKey) != 0)
				return -1;
		}

		div = doc.FindNextElement();
	}

	return 0;
}

void WordExportV2Dialog::RemoveTable(CDBAccess::TDatabase& db, const wxString& name) const
{
    if(db.TableExists(name))
    {
        wxString sql = wxT("DROP TABLE ") + name;
        db.ExecuteUpdate(sql);
    }
}
