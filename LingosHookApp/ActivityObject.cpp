
#include "DBAccess.h"

#include "ActivityObject.h"

ActivityObject::ActivityObject()
: _objDBAccess(NULL)
{
}

ActivityObject::~ActivityObject()
{
}

int ActivityObject::Active(wxApp *app, wxWindow *parent)
{
    wxAppConsole* old = wxAppConsole::GetInstance();
    wxAppConsole::SetInstance(app);
    int ret = Run(app, parent);
    wxAppConsole::SetInstance(old);

    app->SetTopWindow(parent);
    parent->Show();

    return ret;
}

void ActivityObject::SetDBObject(CDBAccess *obj)
{
    _objDBAccess = obj;
}
