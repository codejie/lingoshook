#include <windows.h>

#include "Exports.h"
#include "WordExport.h"

LINGOSHOOK_PLUGINS_API ActivityObject* GetActivity()
{
    _theActivity = new WordExport();
    return _theActivity;
}

int DLLInit()
{
    return 0;
}

void DLLFinal()
{
    if(_theActivity != NULL)
        delete _theActivity, _theActivity = NULL;
}

///////////
BOOL APIENTRY DllMain(HANDLE hModule, DWORD uCallReason, LPVOID pReserved)
{
    switch(uCallReason)
    {
	case DLL_PROCESS_ATTACH:
        if(DLLInit() != 0)
            return FALSE;
        break;
	case DLL_THREAD_ATTACH:
        break;
	case DLL_THREAD_DETACH:
        break;
	case DLL_PROCESS_DETACH:
        DLLFinal();
        break;
    default:
        return FALSE;
    };

    return TRUE;
}

////

WordExport* _theActivity = NULL;


WordExport::WordExport()
: ActivityObject()
{
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
    data.m_strDetail = wxT("NONE.");

    return 0;
}

int WordExport::Run(wxApp* app, wxWindow* parent)
{    
    wxMessageDialog dlg(parent, wxT("Hello Plugins."));
    //app->SetTopWindow(&dlg);
    dlg.ShowModal();

    return 0;
}