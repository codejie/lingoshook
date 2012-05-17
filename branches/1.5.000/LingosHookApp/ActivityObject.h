#ifndef __ACTIVITYOBJECT_H__
#define __ACTIVITYOBJECT_H__

#include "wx/wx.h"

#include "Consts.h"

#ifndef __TEST_PLUGINS__
#include "DBAccess.h"
#include "DictObject.h"
#include "TagObject.h"
#endif

class ActivityObject //: public wxEvtHandler
{
public:
    struct PropertyData
    {
        wxString m_strLabel;
        wxString m_strName;
        wxString m_strVersion;
        wxString m_strAuthor;
        wxString m_strDescription;
//        wxString m_strDetail;
    };
public:
    ActivityObject();
    virtual ~ActivityObject();

    virtual int Init(int fparam = 0, int sparam = 0) { return 0; }
    virtual void Final() {}

    virtual int LoadProperty(PropertyData& data) const = 0;
    unsigned int GetInterfaceVersion() const { return _iInterfaceVersion; }

    virtual int Active(wxApp* papp, wxWindow* pwindow) { return -1; }
#ifndef __TEST_PLUGINS__
    virtual bool NeedDBAccess() const { return false; }
//    virtual bool NeedTagAccess() const { return false; }

    void SetDBObject(CDBAccess* obj);
#endif
//keep function order
    unsigned int GetID() const { return _iID; }
    virtual int ActiveEx(wxApp* papp, wxWindow* pwindow, int* fparam, int* sparam) { return -1; }

#ifndef __TEST_PLUGINS__
	virtual bool NeedDictObject() const { return false; }
	void SetDictObject(CDictObject* obj);
	virtual bool NeedTagObject() const { return false; }
	void SetTagObject(CTagObject* obj);
#endif

protected:

#ifndef __TEST_PLUGINS__
    CDBAccess* _objDBAccess;
	CDictObject* _objDict;
	CTagObject* _objTag;
#endif
protected:
    unsigned int _iInterfaceVersion;
    unsigned int _iID;
};

#endif


