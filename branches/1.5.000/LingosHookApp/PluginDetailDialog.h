// -*- C++ -*- generated by wxGlade 0.6.3 on Mon Jan 31 14:54:10 2011

#include <wx/wx.h>
#include <wx/image.h>
// begin wxGlade: ::dependencies
// end wxGlade


#ifndef PLUGINDETAILDIALOG_H
#define PLUGINDETAILDIALOG_H

#include "ActivityObject.h"

// begin wxGlade: ::extracode
// end wxGlade


class PluginDetailDialog: public wxDialog {
public:
    // begin wxGlade: PluginDetailDialog::ids
    // end wxGlade

    PluginDetailDialog(const ActivityObject::PropertyData& data, wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

private:
    // begin wxGlade: PluginDetailDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: PluginDetailDialog::attributes
    wxStaticBox* sizer_3_staticbox;
    wxStaticText* label_1;
    wxStaticText* m_labelName;
//    wxStaticText* label_3;
//    wxStaticText* m_labelLabel;
    wxStaticText* label_5;
    wxStaticText* m_labelVersion;
    wxStaticText* label_7;
    wxStaticText* m_labelAuthor;
    wxStaticText* label_10;
    wxTextCtrl* m_editDesc;
    wxPanel* panel_2;
    wxButton* button_1;
    wxPanel* panel_3;
    // end wxGlade

    DECLARE_EVENT_TABLE();

public:
    virtual void OnBtnClose(wxCommandEvent &event); // wxGlade: <event_handler>
private:
    const ActivityObject::PropertyData &_data;
}; // wxGlade: end class


#endif // PLUGINDETAILDIALOG_H