#ifndef __ACTIVITYOBJECT_H__
#define __ACTIVITYOBJECT_H__

#include <string>

class CDBAccess;

class ActivityObject
{
public:
    ActivityObject();
    virtual ~ActivityObject();

    virtual int Init(int fparam = 0, int sparam = 0) = 0;

    virtual int Run() = 0;

    virtual bool NeedDBAccess() const { return false; }
    virtual bool NeedTagAccess() const { return false; }

    void SetDBObject(CDBAccess* obj);

    const std::string& Label() const { return _strLabel; }
    const std::string& Name() const { return _strName; }
    const std::string& Version() const { return _strVersion; }
    const std::string& Author() const { return _strAuthor; }
    const std::string& Description() const { return _strDescription; }
protected:
    virtual void InitProperty() {};
    virtual void Final() {};
protected:
    CDBAccess* _objDBAccess;
protected:
    std::string _strLabel;
    std::string _strName;
    std::string _strVersion;
    std::string _strAuthor;
    std::string _strDescription;
};

#endif
