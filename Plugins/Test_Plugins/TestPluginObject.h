#ifndef __TESTPLUGINOBJECT_H__
#define __TESTPLUGINOBJECT_H__

#include <map>

#include "ActivityObject.h"

class TestFrame;

class CTestPluginObject
{
protected:
    typedef std::map<int, wxString> TActivityMap;
public:
    CTestPluginObject(TestFrame* frame);
    virtual ~CTestPluginObject();

    int Init() { return 0; }
    int Load();
    int ActivePlugin(int index);
    void ShowPlugin(int index);
protected:
    void Free();
    int LoadPlugin(int index, const wxString& plugfile);

    void OnPluginLoad(int index, const ActivityObject::PropertyData& data);
private:
    TestFrame* _frame;
    bool _bLoaded;
    TActivityMap _mapActivity;
};

typedef ActivityObject* (*GetActivityPtr)(void);

#endif
