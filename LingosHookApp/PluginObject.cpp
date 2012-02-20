
#include "wx/stdpaths.h"
#include "wx/dir.h"
#include "wx/dynlib.h"

#include "DBAccess.h"

#include "LingosHookApp.h"
#include "TriggerObject.h"
#include "ActivityObject.h"
#include "PluginDetailDialog.h"

#include "PluginObject.h"

CPluginObject::CPluginObject(LingosHookFrame* frame, CDBAccess* db)
: _frame(frame)
, _objDB(db)
, _bLoaded(false)
{
}

CPluginObject::~CPluginObject()
{
    Free();
}

void CPluginObject::Free()
{
}

int CPluginObject::Load()
{
    if(_bLoaded == true)
        return 0;

    wxStandardPaths sp;
    wxString path = sp.GetPluginsDir() + wxT("\\plugins");
    
    wxDir dir;
    if(!dir.Exists(path))
        return 0;
    if(!dir.Open(path))
        return 0;
    if(!dir.IsOpened())
        return 0;

    int index = 0;
    wxString dllfile;
    bool flag = dir.GetFirst(&dllfile, wxT("*.dll"), wxDIR_DEFAULT);
    while(flag)
    {
        LoadPlugin(++ index, path + wxT("\\") + dllfile);

        flag = dir.GetNext(&dllfile);
    }
    
    _bLoaded = true;

    return 0;
}

int CPluginObject::LoadPlugin(int index, const wxString &plugfile)
{
    wxDynamicLibrary lib(plugfile);
    if(!lib.IsLoaded())
        return -1;

     GetActivityPtr ptr = (GetActivityPtr)lib.GetSymbol(wxT("GetActivity"));
     ActivityObject* act = ptr();
     if(act != NULL)
     {
         ActivityObject::PropertyData data;
         if(act->LoadProperty(data) == 0)
         {
             _mapActivity.insert(std::make_pair(index, plugfile));

             g_objTrigger.OnPluginLoad(index, data, act->GetInterfaceVersion() > __LH_PLUGINS_INTERFACEVERSION__);
         }
         delete act, act = NULL;
     }
     lib.Unload();

     return 0;
}

int CPluginObject::ActivePlugin(int index)
{
    TActivityMap::const_iterator it = _mapActivity.find(index);
    if(it == _mapActivity.end())
        return -1;

    wxDynamicLibrary lib(it->second);
    if(!lib.IsLoaded())
        return -1;

    int ret = -1;
    GetActivityPtr ptr = (GetActivityPtr)lib.GetSymbol(wxT("GetActivity"));
    ActivityObject* act = ptr();
    if(act != NULL)
    {
        int ver = act->GetInterfaceVersion();
        if(ver > __LH_PLUGINS_INTERFACEVERSION__)
        {
            if(wxMessageBox(wxT("This interface version of this plugin is imcompatible with current LingosHook application, are you sure to run it ?"), wxT("LingosHook Warning"), wxYES_NO | wxCENTER | wxICON_WARNING) != wxYES)
            {
                delete act, act = NULL;
                lib.Unload();
                return 0;
            }
        }
        if(act->Init() == 0)
        {
            if(act->NeedDBAccess())
                act->SetDBObject(_objDB);
            //if(act->NeedTagAccess())
            //    act->SetTagObject(_objTag);
            if(ver == 1)
            {
                if(act->Active(&wxGetApp(), _frame) == 0)
                    ret = 0;
            }
            else
            {
                int fparam = -1, sparam = -1;
                if(act->ActiveEx(&wxGetApp(), _frame, &fparam, &sparam) == 0)
                    ret = 0;
                ResultProc(act->GetID(), fparam, sparam);
            }
            act->Final();
            delete act, act = NULL;
        }
    }
    lib.Unload();    
    return ret;
}

void CPluginObject::ShowPlugin(int index)
{
    TActivityMap::const_iterator it = _mapActivity.find(index);
    if(it == _mapActivity.end())
        return;

    wxDynamicLibrary lib(it->second);
    if(!lib.IsLoaded())
        return;

    GetActivityPtr ptr = (GetActivityPtr)lib.GetSymbol(wxT("GetActivity"));
    ActivityObject* act = ptr();
    if(act != NULL)
    {
         ActivityObject::PropertyData data;
         if(act->LoadProperty(data) == 0)
         {
            PluginDetailDialog dlg(data, _frame, wxID_ANY, wxEmptyString);
            dlg.ShowModal();
         }
         delete act, act = NULL;
    }
    lib.Unload();
}