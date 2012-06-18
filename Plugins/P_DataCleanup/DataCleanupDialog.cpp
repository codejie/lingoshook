// -*- C++ -*- generated by wxGlade 0.6.5 (standalone edition) on Thu May 31 14:12:27 2012

#include <vector>

#include "Consts.h"

#include "DBAccess.h"
#include "TinyHtmlParser.h"
#include "DataCleanupDialog.h"

// begin wxGlade: ::extracode
// end wxGlade

using namespace TinyHtmlParser;

const wxString DataCleanupDialog::PLUGINS_TITLE		=	wxT("LingosHook Plugin - Data Clean-up");

DataCleanupDialog::DataCleanupDialog(CDBAccess* dbaccess, wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):
    wxDialog(parent, id, title, pos, size, wxDEFAULT_DIALOG_STYLE)
, _objDBAccess(dbaccess)
{
    // begin wxGlade: DataCleanupDialog::DataCleanupDialog
    label_1 = new wxStaticText(this, wxID_ANY, wxT("It is recommended to backup your LingosHook database file before cleaning-up."));
    btnBackup = new wxButton(this, 7000, wxT("Backup"));
    static_line_1 = new wxStaticLine(this, wxID_ANY);
    barProcess = new wxGauge(this, 7002, 0, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL|wxGA_SMOOTH);
    btnCleanup = new wxButton(this, 7001, wxT("Clean-up"));
	checkRemoveNotMatch = new wxCheckBox(this, wxID_ANY, wxT("Remove 'Not Match' word and data also"));
    static_line_2 = new wxStaticLine(this, wxID_ANY);
    panel_1 = new wxPanel(this, wxID_ANY);
    btnClose = new wxButton(this, wxID_CLOSE, wxT("Close"));

    set_properties();
    do_layout();
    // end wxGlade
}


BEGIN_EVENT_TABLE(DataCleanupDialog, wxDialog)
    // begin wxGlade: DataCleanupDialog::event_table
    EVT_BUTTON(7000, DataCleanupDialog::OnBtnBackup)
    EVT_BUTTON(7001, DataCleanupDialog::OnBtnCleanup)
	EVT_BUTTON(wxID_CLOSE, DataCleanupDialog::OnBtnClose)
    // end wxGlade
END_EVENT_TABLE();


void DataCleanupDialog::OnBtnBackup(wxCommandEvent &event)
{
	wxString ffile = wxT("LingosHook.db3");

	if(::wxFileExists(ffile) == false)
	{
		wxMessageBox(wxT("Can't find database file - LingosHook.db3."), PLUGINS_TITLE, wxOK | wxCENTER | wxICON_ERROR);
		return;
	}

	wxDateTime date = wxDateTime::Now();
	wxString tfile = date.Format(wxT("LingosHook_[%Y-%m-%d_%H-%M].db3"));

	if(::wxCopyFile(ffile, tfile, true) == false)
	{
		wxMessageBox(wxT("Backup database file failed."), PLUGINS_TITLE, wxOK | wxCENTER | wxICON_ERROR);
	}
	else
	{
		wxMessageBox(wxString::Format(wxT("Backup database successfully - %s."), tfile), PLUGINS_TITLE, wxOK | wxCENTER | wxICON_INFORMATION);
	}
}


void DataCleanupDialog::OnBtnCleanup(wxCommandEvent &event)
{
	EnableButtons(false);

	if(Cleanup() != 0)
	{
		wxMessageBox(wxT("Can't find database file - LingosHook.db3."), PLUGINS_TITLE, wxOK | wxCENTER | wxICON_ERROR);
	}

	EnableButtons(true);
}

void DataCleanupDialog::OnBtnClose(wxCommandEvent &event)
{
	Close();
}



// wxGlade: add DataCleanupDialog event handlers


void DataCleanupDialog::set_properties()
{
    // begin wxGlade: DataCleanupDialog::set_properties
    SetTitle(PLUGINS_TITLE);
    label_1->SetFont(wxFont(9, wxDEFAULT, wxNORMAL, wxBOLD, 0, wxT("")));
    // end wxGlade
}


void DataCleanupDialog::do_layout()
{
    // begin wxGlade: DataCleanupDialog::do_layout
    wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_4 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_3 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer_5 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_2 = new wxBoxSizer(wxHORIZONTAL);
    sizer_2->Add(label_1, 1, wxALIGN_CENTER_VERTICAL, 0);
    sizer_2->Add(btnBackup, 0, wxLEFT|wxRIGHT|wxALIGN_CENTER_VERTICAL, 4);
    sizer_1->Add(sizer_2, 0, wxALL|wxEXPAND, 16);
    sizer_1->Add(static_line_1, 0, wxLEFT|wxRIGHT|wxEXPAND, 128);
    sizer_3->Add(barProcess, 1, wxALIGN_CENTER_VERTICAL, 0);
    sizer_3->Add(btnCleanup, 0, wxLEFT|wxRIGHT|wxALIGN_CENTER_VERTICAL, 4);
    sizer_1->Add(sizer_3, 0, wxALL|wxEXPAND, 16);
    sizer_5->Add(checkRemoveNotMatch, 0, 0, 0);
    sizer_1->Add(sizer_5, 0, wxLEFT|wxRIGHT|wxBOTTOM|wxEXPAND, 16);
    sizer_1->Add(static_line_2, 0, wxLEFT|wxRIGHT|wxEXPAND, 16);
    sizer_4->Add(panel_1, 1, wxEXPAND, 0);
    sizer_4->Add(btnClose, 0, wxLEFT|wxRIGHT, 12);
    sizer_1->Add(sizer_4, 0, wxALL|wxEXPAND, 8);
    SetSizer(sizer_1);
    sizer_1->Fit(this);
    Layout();
    // end wxGlade
}


//SELECT SrcDataTable.SrcID FROM SrcDataTable LEFT JOIN WordTable ON SrcDataTable.SrcID = WordTable.SrcID WHERE WordTable.SrcID IS NULL
int DataCleanupDialog::Cleanup()
{
	if(_objDBAccess == NULL)
		return -1;

	int total = 0, found = 0;
	wxString ret = wxEmptyString;
	try
	{
		wxString sql = wxT("SELECT COUNT(*) FROM SrcDataTable");
		CDBAccess::TResult res = _objDBAccess->Database().ExecuteQuery(sql);
		if(!res.IsOk())
			return -1;
		if(res.Eof())
			return -1;

		total = res.GetInt(0);
		barProcess->SetRange(total);

		if(total > 0)
		{
			sql = wxT("SELECT SrcDataTable.SrcID FROM SrcDataTable LEFT JOIN WordTable ON SrcDataTable.SrcID = WordTable.SrcID WHERE WordTable.SrcID IS NULL");
			res = _objDBAccess->Database().ExecuteQuery(sql);
			if(!res.IsOk())
				return -1;
			found = 0;
			while(res.NextRow())
			{
				DeleteData(res.GetInt(0));
				barProcess->SetValue(found ++);
			}

			if(found > 0)
			{
				ret = wxString::Format(wxT("Cleanup database successfully - delete %d records from all %d records."), found, total);
			}
			else
			{
				ret = wxT("Cleanup database successfully - all records are ok.");
			}
		}
		else
		{
			ret = wxT("Cleanup database successfully - no any record in database.");
		}

		barProcess->SetValue(total);
	}
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
	
	if(!checkRemoveNotMatch->GetValue())
	{
		if(found > 0)
		{
			ret = wxString::Format(wxT("Cleanup database successfully - delete %d records from all %d records."), found, total);
		}
		else
		{
			ret = wxT("Cleanup database successfully - all records are ok.");
		}
	}
	else
	{
		int match = 0;
		if(RemoveNotMatchData(match) == 0)
		{
			if(found > 0)
			{
				ret = wxString::Format(wxT("Cleanup database successfully - delete %d records and %d 'not match' words from all %d records."), found, match, total);
			}
			else
			{
				if(match == 0)
				{
					ret = wxT("Cleanup database successfully - all records are ok.");
				}
				else
				{
					ret = wxString::Format(wxT("Cleanup database successfully - delete %d 'not match' words from all %d records."), match, total);
				}
			}
		}
		else
		{
			if(found > 0)
			{
				ret = wxString::Format(wxT("Cleanup database failed - delete %d records from all %d records, but try to delete 'not match' words failed."), found, total);
			}
			else
			{
				ret = wxT("Cleanup database failed - all records are ok, but try to delete 'not match' words failed.");
			}
		}
	}

	wxMessageBox(ret, PLUGINS_TITLE, wxOK | wxCENTER | wxICON_INFORMATION);

	barProcess->SetValue(0);

	return 0;
}

int DataCleanupDialog::DeleteData(int srcid)
{
	try
	{
		wxString sql = wxT("DELETE FROM SrcDataTable WHERE SrcID = ?");
		CDBAccess::TQuery query = _objDBAccess->Database().PrepareStatement(sql);
		query.Bind(1, srcid);
		if(query.ExecuteUpdate() == 0)
			return -1;
	}
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
	return 0;
}

void DataCleanupDialog::EnableButtons(bool flag)
{
	btnBackup->Enable(flag);
	btnCleanup->Enable(flag);
	btnClose->Enable(flag);
}

int DataCleanupDialog::RemoveNotMatchData(int& found)
{

	int total = 0;

	//std::vector<int> words;

	found = 0;
	try
	{
		wxString sql = wxT("SELECT COUNT(*) FROM SrcDataTable");
		CDBAccess::TResult res = _objDBAccess->Database().ExecuteQuery(sql);
		if(!res.IsOk())
			return -1;
		if(res.Eof())
			return -1;

		total = res.GetInt(0);
		barProcess->SetRange(total);

		barProcess->SetValue(0);
		if(total > 0)
		{
			sql = wxT("SELECT WordTable.WordID, SrcDataTable.HTML FROM WordTable,SrcDataTable WHERE WordTable.SrcID = SrcDataTable.SrcID");
			res = _objDBAccess->Database().ExecuteQuery(sql);
			if(!res.IsOk())
				return -1;
			found = 0;
			while(res.NextRow())
			{
				if(IsNotMatchData(res.GetString(1)))
				{
					//words.push_back(res.GetInt(0));
					DeleteWord(res.GetInt(0));
					++ found;
				}
				barProcess->SetValue(barProcess->GetValue() + 1);
			}
		}

		barProcess->SetValue(total);
	}
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }

	//barProcess->SetRange(words.size());
	//barProcess->SetValue(0);
	//for(std::vector<int>::const_iterator it = words.begin(); it != words.end(); ++ it)
	//{
	//	DeleteWord(*it);
	//	barProcess->SetValue(++ found);
	//}

	//found = words.size();

	return 0;
}

bool DataCleanupDialog::IsNotMatchData(const wxString &html) const
{
	CDocumentObject doc;

    try
    {
		if(doc.Load(html.c_str(), true, true) != 0)
        {
            return false;
        }
    }
    catch(CExceptionObject& e)
    {
        return false;
    }

	const CElementObject* div = doc.Root(), *dict = NULL;
	div = doc.FindFirstElement(L"DIV");
	const CAttributeObject * id = NULL;
	while(div != NULL)
	{
		id = doc.FindAttribute(div, L"id");
		if(id != NULL && id->value == L"\"lingoes_dictarea\"")
		{
			return false;
		}

		div = doc.FindNextElement();
	}

	return true;
}

int DataCleanupDialog::DeleteWord(int wordid)
{
	::SendMessage((HWND)this->GetParent()->GetHWND(), WM_PLAGINREQ_DELETE_WORD, wordid, 0);

	return 0;//_objDict->RemoveWord(wordid);
}