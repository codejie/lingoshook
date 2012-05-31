
#include "PluginsConsts.h"

#include "DataCleanupDialog.h"

#include "DataCleanup.h"

IMPLEMENT_PLUGIN(DataCleanup)

DataCleanup::DataCleanup()
: ActivityObject()
{
	_iInterfaceVersion = INTERFACE_VERSION_1;
	_iID = PLUGIN_ID_DATACLEANUP;
}

DataCleanup::~DataCleanup()
{
}

int DataCleanup::LoadProperty(ActivityObject::PropertyData &data) const
{
    data.m_strLabel = wxT("DataCleanup");
    data.m_strName = wxT("Data Clean-up");
    data.m_strVersion = wxT("0.0.1");
    data.m_strAuthor = wxT("codejie");
	data.m_strDescription = wxT("Cleaning up useless data from LingosHook database file.");

	return 0;
}

int DataCleanup::Active(wxApp *papp, wxWindow *pwindow)
{
	DataCleanupDialog dlg(_objDBAccess, pwindow, wxID_ANY, wxEmptyString);
	dlg.ShowModal();

	return 0;
}