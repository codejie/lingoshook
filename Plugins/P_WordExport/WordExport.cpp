
#include "Consts.h"

#include "WordExportDialog.h"
#include "WordExportV2Dialog.h"

#include "WordExport.h"

IMPLEMENT_PLUGIN(WordExport)

WordExport::WordExport()
: ActivityObject()
{
    _iInterfaceVersion = INTERFACE_VERSION_1;
    _iID = PLUGIN_ID_WORDEXPORT;
}

WordExport::~WordExport()
{
}

int WordExport::LoadProperty(PropertyData& data) const
{
    data.m_strLabel = wxT("WordExport");
    data.m_strName = wxT("Word Export");
    data.m_strVersion = wxT("1.0.0");
    data.m_strAuthor = wxT("codejie");
    data.m_strDescription = wxT("Export words from LingosHook to a HTML/Text file.");
//    data.m_strDetail = wxT("NONE.");

    return 0;
}

int WordExport::Active(wxApp* papp, wxWindow* pwindow)
{
    //wxMessageBox(_objDBAccess->Database().GetVersion());

    //WordExportDialog dlg(_objDBAccess, pwindow, wxID_ANY, wxEmptyString);
	WordExportV2Dialog dlg(_objDBAccess, pwindow, wxID_ANY, wxEmptyString);
    dlg.ShowModal();

    return 0;
}