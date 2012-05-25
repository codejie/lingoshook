// -*- C++ -*- generated by wxGlade 0.6.5 (standalone edition) on Mon May 21 17:16:58 2012

#include <wx/wx.h>
#include <wx/image.h>
// begin wxGlade: ::dependencies
#include <wx/statline.h>
// end wxGlade


#ifndef SEPARATORDIALOG_H
#define SEPARATORDIALOG_H


// begin wxGlade: ::extracode
// end wxGlade


class SeparatorDialog: public wxDialog {
public:
    // begin wxGlade: SeparatorDialog::ids
    // end wxGlade

    SeparatorDialog(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

private:
    // begin wxGlade: SeparatorDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: SeparatorDialog::attributes
    wxStaticText* label_6;
    wxStaticLine* static_line_7;
    wxRadioButton* m_rdSepCRLF;
    wxRadioButton* m_rdSepComma;
    wxRadioButton* m_rdSepSemi;
    wxRadioButton* m_rdSepVertical;
    wxRadioButton* m_rdSepCustom;
    wxTextCtrl* m_editSepCustom;
    wxStaticLine* static_line_6;
    wxButton* button_5;
    wxPanel* panel_1;
    wxButton* button_6;
    // end wxGlade

    DECLARE_EVENT_TABLE();

public:
    virtual void OnRadioSeparatorClick(wxCommandEvent &event); // wxGlade: <event_handler>
	virtual void OnBtnOK(wxCommandEvent &event); // wxGlade: <event_handler>
public:
	wxString m_strSeparator;
}; // wxGlade: end class


#endif // SEPARATORDIALOG_H