
#include "WordExportDialog.h"

#include "WordExport.h"

IMPLEMENT_PLUGIN(WordExport)

WordExport::WordExport()
: ActivityObject()
{
//    _iInterfaceVersion = 2;
}

WordExport::~WordExport()
{
}

int WordExport::LoadProperty(PropertyData& data) const
{
    data.m_strLabel = wxT("WordExport");
    data.m_strName = wxT("Word Export");
    data.m_strVersion = wxT("0.0.1");
    data.m_strAuthor = wxT("codejie");
    data.m_strDescription = wxT("Export words from LingosHook to a Text file.");
//    data.m_strDetail = wxT("NONE.");

    return 0;
}

int WordExport::Active(wxApp* papp, wxWindow* pwindow)
{
    //wxMessageBox(_objDBAccess->Database().GetVersion());

    WordExportDialog dlg(_objDBAccess, pwindow, wxID_ANY, wxEmptyString);
    dlg.ShowModal();

    return 0;
}