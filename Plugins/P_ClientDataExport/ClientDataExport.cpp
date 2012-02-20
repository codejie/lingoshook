
#include "Consts.h"

#include "ClientDataExportDialog.h"
#include "ClientDataExport.h"

IMPLEMENT_PLUGIN(ClientDataExport)

ClientDataExport::ClientDataExport()
: ActivityObject()
{
    _iInterfaceVersion = INTERFACE_VERSION_1;
    _iID = PLUGIN_ID_CLIENTDATAEXPORT;
}

ClientDataExport::~ClientDataExport()
{
}

int ClientDataExport::LoadProperty(PropertyData &data) const
{
    data.m_strLabel = wxT("ClientDataExport");
    data.m_strName = wxT("Client Data Export");
    data.m_strVersion = wxT("0.0.1");
    data.m_strAuthor = wxT("codejie");
    data.m_strDescription = wxT("Export data from LingosHook to a db3 file that Android or IOS client application required.");
    return 0;
}

int ClientDataExport::Active(wxApp *papp, wxWindow *pwindow)
{
    ClientDataExportDialog dlg(_objDBAccess, pwindow, wxID_ANY, wxEmptyString);
    dlg.ShowModal();

    return 0;
}

