#ifndef __PLUGINOBJECT_H__
#define __PLUGINOBJECT_H__

#include <map>

#include "ActivityObject.h"

class LingosHookFrame;
class CDBAccess;

class CPluginObject
{
protected:
    typedef std::map<int, wxString> TActivityMap;
public:
    CPluginObject(LingosHookFrame* frame, CDBAccess* db);
    virtual ~CPluginObject();

    int Init() { return 0; }
    int Load();
    int ActivePlugin(int index);
    void ShowPlugin(int index);
protected:
    void Free();
    int LoadPlugin(int index, const wxString& plugfile);
private:
    LingosHookFrame* _frame;
    CDBAccess * _objDB;
    bool _bLoaded;
    TActivityMap _mapActivity;
};

typedef ActivityObject* (*GetActivityPtr)(void);

#endif
