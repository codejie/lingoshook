
#include "Consts.h"

#include "SetHomepageDialog.h"
#include "SetHomepage.h"


IMPLEMENT_PLUGIN(SetHomepage)

SetHomepage::SetHomepage()
: ActivityObject()
{
    _iInterfaceVersion = INTERFACE_VERSION_2;
    _iID = PLUGIN_ID_SETHOMEPAGE;
}

SetHomepage::~SetHomepage()
{
}

int SetHomepage::LoadProperty(PropertyData &data) const
{
    data.m_strLabel = wxT("SetHomePage");
    data.m_strName = wxT("Homepage Setting");
    data.m_strVersion = wxT("0.0.1");
    data.m_strAuthor = wxT("codejie");
    data.m_strDescription = wxT("Use to set the startup page of LingosHook.");
    return 0;
}

int SetHomepage::ActiveEx(wxApp *papp, wxWindow *pwindow, int* fparam, int* sparam)
{
    SetHomepageDialog dlg(_objDBAccess, pwindow, wxID_ANY, wxEmptyString);
    dlg.ShowModal();

    return 0;
}

