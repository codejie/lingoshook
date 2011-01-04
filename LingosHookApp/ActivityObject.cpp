
#ifndef __TEST_PLUGINS__
#include "DBAccess.h"
#endif

#include "ActivityObject.h"

ActivityObject::ActivityObject()
#ifndef __TEST_PLUGINS__
: _objDBAccess(NULL)
#endif
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

#ifndef __TEST_PLUGINS__

void ActivityObject::SetDBObject(CDBAccess *obj)
{
    _objDBAccess = obj;
}

#endif
