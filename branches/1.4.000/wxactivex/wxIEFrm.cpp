/*
                wxActiveX Library Licence, Version 3
                ====================================

  Copyright (C) 2003 Lindsay Mathieson [, ...]

  Everyone is permitted to copy and distribute verbatim copies
  of this licence document, but changing it is not allowed.

                       wxActiveX LIBRARY LICENCE
     TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
  
  This library is free software; you can redistribute it and/or modify it
  under the terms of the GNU Library General Public Licence as published by
  the Free Software Foundation; either version 2 of the Licence, or (at
  your option) any later version.
  
  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library
  General Public Licence for more details.

  You should have received a copy of the GNU Library General Public Licence
  along with this software, usually in a file named COPYING.LIB.  If not,
  write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA 02111-1307 USA.

  EXCEPTION NOTICE

  1. As a special exception, the copyright holders of this library give
  permission for additional uses of the text contained in this release of
  the library as licenced under the wxActiveX Library Licence, applying
  either version 3 of the Licence, or (at your option) any later version of
  the Licence as published by the copyright holders of version 3 of the
  Licence document.

  2. The exception is that you may use, copy, link, modify and distribute
  under the user's own terms, binary object code versions of works based
  on the Library.

  3. If you copy code from files distributed under the terms of the GNU
  General Public Licence or the GNU Library General Public Licence into a
  copy of this library, as this licence permits, the exception does not
  apply to the code that you add in this way.  To avoid misleading anyone as
  to the status of such modified files, you must delete this exception
  notice from such code and/or adjust the licensing conditions notice
  accordingly.

  4. If you write modifications of your own for this library, it is your
  choice whether to permit this exception to apply to your modifications. 
  If you do not wish that, you must delete the exception notice from such
  code and/or adjust the licensing conditions notice accordingly.
*/

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
// For compilers that support precompilation, includes "wx/wx.h".
#if defined(__WXGTK__) || defined(__WXMOTIF__)
        #include "wx/wx.h"
#endif
#include "wx/wxprec.h"
#include "wx/filedlg.h"
#include "wxIEApp.h"
#include "wxIEFrm.h"
#include "wxActiveXFrame.h"
#include <istream>
#include <fstream>
using namespace std;
#include <exdispid.h>

// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------
// the application icon
#if defined(__WXGTK__) || defined(__WXMOTIF__)
    #include "wxIE.xpm"
#endif

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // menu items
    FILE_QUIT = 1,
	FILE_OPEN,
	FILE_BROWSE,
    FILE_HTML_EDITMODE,
	FILE_TEST_HTML,
	FILE_TEST_SELECT,
	FILE_TEST_HTMLSELECT,
	FILE_TEST_GETTEXT,
	FILE_TEST_HTMLGETTEXT,
    FILE_TEST_HOME,
    FILE_TEST_ACTIVEX,
    FILE_ABOUT,

	// controls
	ID_MSHTML = 501,
	ID_PROGRESS_GAUGE
};

// ----------------------------------------------------------------------------
// event tables and other macros for wxWindows
// ----------------------------------------------------------------------------

// the event tables connect the wxWindows events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE(wxIEFrame, wxFrame)
    EVT_SIZE(wxIEFrame::OnSize)
    EVT_MENU(FILE_QUIT,		wxIEFrame::OnQuit)
	EVT_MENU(FILE_BROWSE,		wxIEFrame::OnBrowse)
	EVT_MENU(FILE_OPEN,			wxIEFrame::OnOpen)
    EVT_MENU(FILE_HTML_EDITMODE, wxIEFrame::OnEditMode)
    EVT_UPDATE_UI(FILE_HTML_EDITMODE, wxIEFrame::OnEditModeUI)
    EVT_MENU(FILE_TEST_HTML,	wxIEFrame::OnTestHTML)
    EVT_MENU(FILE_TEST_SELECT,  wxIEFrame::OnTestSelect)
    EVT_MENU(FILE_TEST_HTMLSELECT,  wxIEFrame::OnTestHTMLSelect)
    EVT_MENU(FILE_TEST_GETTEXT,  wxIEFrame::OnTestGetText)
    EVT_MENU(FILE_TEST_HTMLGETTEXT,  wxIEFrame::OnTestHTMLGetText)
    EVT_MENU(FILE_TEST_HOME, wxIEFrame::OnTestHome)
    EVT_MENU(FILE_TEST_ACTIVEX, wxIEFrame::OnTestActiveX)
    EVT_MENU(FILE_ABOUT,		wxIEFrame::OnAbout)

    // ActiveX Events
    EVT_ACTIVEX_DISPID(ID_MSHTML, DISPID_STATUSTEXTCHANGE,  OnMSHTMLStatusTextChangeX)
    EVT_ACTIVEX(ID_MSHTML, "BeforeNavigate2",   OnMSHTMLBeforeNavigate2X)
	EVT_ACTIVEX(ID_MSHTML, "TitleChange",       OnMSHTMLTitleChangeX)
	EVT_ACTIVEX(ID_MSHTML, "NewWindow2",        OnMSHTMLNewWindow2X)
	EVT_ACTIVEX(ID_MSHTML, "ProgressChange",    OnMSHTMLProgressChangeX)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
wxIEFrame::wxIEFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
       : wxFrame((wxFrame *)NULL, -1, title, pos, size)
{
    // set the frame icon
    SetIcon(wxICON(wxIE));

    // create a menu bar
    wxMenu *menuFile = new wxMenu(wxT(""), wxMENU_TEAROFF);

    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(FILE_ABOUT, wxT("&About...\tCtrl-A"), wxT("Show about dialog"));

	menuFile->Append(FILE_TEST_HTML, wxT("Test HTML"), wxT("Demonstrates LoadString()"));
	menuFile->Append(FILE_OPEN, wxT("Open HTML File"), wxT("Demonstrates LoadStream(istream *)"));
	menuFile->Append(FILE_BROWSE, wxT("Browse Web Page"), wxT("Demonstrates LoadUrl(url)"));
	menuFile->Append(FILE_HTML_EDITMODE, wxT("Edit Mode"), wxT("Demonstrates editing html"), true);
	menuFile->AppendSeparator();
	menuFile->Append(FILE_TEST_SELECT, wxT("Get Selected Text"), wxT("Demonstrates GetStringSelection(false)"));
	menuFile->Append(FILE_TEST_HTMLSELECT, wxT("Get HTML Selected Text"), wxT("Demonstrates GetStringSelection(true)"));
	menuFile->AppendSeparator();
	menuFile->Append(FILE_TEST_GETTEXT, wxT("Get Text"), wxT("Demonstrates GetText(false)"));
	menuFile->Append(FILE_TEST_HTMLGETTEXT, wxT("Get HTML Text"), wxT("Demonstrates GetText(true)"));
	menuFile->Append(FILE_TEST_HOME, wxT("Open Home Page"), wxT("Demonstrates GoHome()"));
	menuFile->AppendSeparator();
	menuFile->Append(FILE_TEST_ACTIVEX, wxT("Display a ActiveX control"), wxT("Demonstrates the Generic ActiveX Container"));
	menuFile->AppendSeparator();
    menuFile->Append(FILE_QUIT, wxT("E&xit\tAlt-X"), wxT("Quit this program"));

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(menuFile, wxT("&File"));
    menuBar->Append(helpMenu, wxT("&Help"));

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);

    // create a status bar just for fun (by default with 1 pane only)
    wxStatusBar * sb = CreateStatusBar(2);
    SetStatusText(wxT("Ready"));

	// progress gauge (belongs to status bar)
	m_gauge = new wxGauge(sb, ID_PROGRESS_GAUGE, 100);

	// IE Control
	m_ie = new wxIEHtmlWin(this, ID_MSHTML);

}


// event handlers

void wxIEFrame::OnSize(wxSizeEvent& event)
{
	wxFrame::OnSize(event);

	wxStatusBar* sb = GetStatusBar();
	if (! sb)
		return;

	wxRect rc;
	sb->GetFieldRect(1, rc);

	m_gauge->SetSize(rc);
};

void wxIEFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // TRUE is to force the frame to close
    Close(TRUE);
}

void wxIEFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxString msg;
    msg.Printf(wxT("About wxIE...\n"));
    wxMessageBox(msg, wxT("About wxIE"), wxOK | wxICON_INFORMATION, this);
}

void wxIEFrame::OnTestHTML(wxCommandEvent& WXUNUSED(event))
{
	wxString html = 
		wxT("<HTML><BODY><H1>Hello World中文系统哈哈哈</H1>Plain Text</BODY>dddd</HTML>");
    m_ie->SetCharset(wxT("utf-8"));
	m_ie->LoadwxString(html);
}


void wxIEFrame::OnTestSelect(wxCommandEvent& WXUNUSED(event))
{
	wxString s = m_ie->GetStringSelection();

	wxMessageBox(s);
}

void wxIEFrame::OnTestHTMLSelect(wxCommandEvent& WXUNUSED(event))
{
	wxString s = m_ie->GetStringSelection(true);

	wxMessageBox(s);
}

void wxIEFrame::OnTestGetText(wxCommandEvent& WXUNUSED(event))
{
	wxString s = m_ie->GetText();

	wxMessageBox(s);
}

void wxIEFrame::OnTestHTMLGetText(wxCommandEvent& WXUNUSED(event))
{
	wxString s = m_ie->GetText(true);

	wxMessageBox(s);
}

void wxIEFrame::OnTestHome(wxCommandEvent& WXUNUSED(event))
{
	m_ie->GoHome();
};


void wxIEFrame::OnOpen(wxCommandEvent& WXUNUSED(event))
{
	wxFileDialog dlg(this, wxT("Chooose a HTML File"), wxT(""), wxT(""), wxT("HTML files (*.html; *.htm)|*.html;*.htm"),wxOPEN);

	if (dlg.ShowModal() == wxID_OK)
	{
		wxString fname = dlg.GetPath();

        std::ifstream *is = new std::ifstream(fname.mb_str());
		m_ie->LoadStream(is);
	};
}

void wxIEFrame::OnEditMode(wxCommandEvent& WXUNUSED(event))
{
	m_ie->SetEditMode(! m_ie->GetEditMode());
}

void wxIEFrame::OnEditModeUI(wxUpdateUIEvent& event)
{
    if (m_ie)
	    event.Check(m_ie->GetEditMode());
}

void wxIEFrame::OnBrowse(wxCommandEvent& WXUNUSED(event))
{
	wxString url = wxGetTextFromUser(wxT("Enter URL:"), wxT("Browse"), wxT(""), this);

	m_ie->LoadUrl(url);
}

void wxIEFrame::OnMSHTMLStatusTextChangeX(wxActiveXEvent& event)
{
    SetStatusText(event[wxT("Text")]);
};


void wxIEFrame::OnMSHTMLBeforeNavigate2X(wxActiveXEvent& event)
{
    wxString url = event[wxT("Url")];
	if (url == wxT("about:blank"))
		return;

	int rc = wxMessageBox(url, wxT("Allow open url ?"), wxYES_NO);

	if (rc != wxYES)
		event[wxT("Cancel")] = true;
};

void wxIEFrame::OnMSHTMLTitleChangeX(wxActiveXEvent& event)
{
	SetTitle(event[wxT("Text")]);
};


void wxIEFrame::OnMSHTMLNewWindow2X(wxActiveXEvent& event)
{
	int rc = wxMessageBox(wxT("New Window requested"), wxT("Allow New Window ?"), wxYES_NO);

	if (rc != wxYES)
		event[wxT("Cancel")] = true;
};


void wxIEFrame::OnMSHTMLProgressChangeX(wxActiveXEvent& event)
{
	if ((long) event[wxT("ProgressMax")] != m_gauge->GetRange())
		m_gauge->SetRange((long) event[wxT("ProgressMax")]);

	m_gauge->SetValue((long) event[wxT("Progress")]);
};

void wxIEFrame::OnTestActiveX(wxCommandEvent& WXUNUSED(event))
{
// Some known prog ids
//#define PROGID "Shell.Explorer"
//#define PROGID CLSID_WebBrowser
//#define PROGID CLSID_MozillaBrowser
//#define PROGID CLSID_HTMLDocument
//#define PROGID "MSCAL.Calendar"
//#define PROGID "WordPad.Document"
//#define PROGID "SoftwareFX.ChartFX"
//#define PROGID "PDF.PdfCtrl"
#define PROGID "ShockwaveFlash.ShockwaveFlash"

    wxDialog dlg(this, -1, wxString(wxT("Test ActiveX")));

    wxFlexGridSizer *sz = new wxFlexGridSizer(2);
    sz->Add(new wxStaticText(&dlg, -1, wxT("Enter a ActiveX ProgId")), 0, wxALL, 5 );

    wxComboBox *cb = new wxComboBox(&dlg, 101, wxEmptyString);
    cb->Append(wxT("ShockwaveFlash.ShockwaveFlash"));
    cb->Append(wxT("MSCAL.Calendar"));
    cb->Append(wxT("Shell.Explorer"));
    cb->Append(wxT("WordPad.Document.1"));
    cb->Append(wxT("SoftwareFX.ChartFX.20"));
    cb->Append(wxT("PDF.PdfCtrl.5"));
    cb->SetSelection(0);

    sz->Add(cb, 0, wxALL, 5 );

    // next row
    sz->Add(new wxButton(&dlg, wxID_CANCEL, wxT("Cancel")), 0, wxALIGN_RIGHT|wxALL, 5 );
    sz->Add(new wxButton(&dlg, wxID_OK, wxT("Ok")), 0, wxALIGN_RIGHT|wxALL, 5 );

    dlg.SetAutoLayout( TRUE );
    dlg.SetSizer(sz);
    sz->Fit(&dlg);
    sz->SetSizeHints(&dlg);


    if (dlg.ShowModal() == wxID_OK)
    {
        wxString progId = cb->GetValue();
        wxActiveXFrame *frame = new wxActiveXFrame(this, progId);
        frame->Show();
    };
}

