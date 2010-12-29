#ifndef __ACTIVITYOBJECT_H__
#define __ACTIVITYOBJECT_H__

#include "wx/wx.h"

class CDBAccess;

class ActivityObject
{
public:
    struct PropertyData
    {
        wxString m_strLabel;
        wxString m_strName;
        wxString m_strVersion;
        wxString m_strAuthor;
        wxString m_strDescription;
        wxString m_strDetail;
    };
public:
    ActivityObject();
    virtual ~ActivityObject();

    virtual int Init(int fparam = 0, int sparam = 0) { return 0; }
    virtual void Final() {}

    virtual int LoadProperty(PropertyData& data) const = 0;

    virtual int Active(wxApp* app, wxWindow* parent);

    virtual bool NeedDBAccess() const { return false; }
    virtual bool NeedTagAccess() const { return false; }

    void SetDBObject(CDBAccess* obj);
protected:
    virtual int Run(wxApp* app, wxWindow* parent) { return 0; }
protected:
    CDBAccess* _objDBAccess;
};

#endif
