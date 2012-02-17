// -*- C++ -*- generated by wxGlade 0.6.3 on Tue Apr 06 18:27:07 2010

#include <wx/wx.h>
#include <wx/image.h>
// begin wxGlade: ::dependencies
#include <wx/statline.h>
// end wxGlade


#ifndef __EXCEPTIONRAISEDDIALOG_H__
#define __EXCEPTIONRAISEDDIALOG_H__


// begin wxGlade: ::extracode
// end wxGlade


class CExceptionRaisedDialog: public wxDialog {
public:
    // begin wxGlade: CExceptionRaisedDialog::ids
    // end wxGlade

    CExceptionRaisedDialog(wxWindow* parent, int id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

    void SetInfo(const wxString& info);
private:
    // begin wxGlade: CExceptionRaisedDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: CExceptionRaisedDialog::attributes
    wxStaticText* label_2;
    wxStaticText* label_3;
    wxStaticLine* static_line_1;
    wxTextCtrl* m_textInfo;
    wxStaticLine* static_line_2;
    wxButton* m_btnCopy;
    wxButton* m_btnSend;
    wxButton* m_btnPost;
    wxPanel* panel_1;
    wxButton* m_btnOK;
    // end wxGlade

    DECLARE_EVENT_TABLE();

public:
    virtual void OnBtnCopy(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnBtnSend(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnBtnPost(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnBtnOK(wxCommandEvent &event); // wxGlade: <event_handler>
}; // wxGlade: end class


#endif // LH-EXCEPTIONDLG_H