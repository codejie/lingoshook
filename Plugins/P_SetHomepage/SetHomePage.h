#ifndef __SETHOMEPAGE_H__
#define __SETHOMEPAGE_H__

#include "Exports.h"
#include "ActivityObject.h"

class SetHomepage : public ActivityObject
{
public:
    SetHomepage();
    virtual ~SetHomepage();

    virtual int Init(int fparam = 0, int sparam = 0) { return 0; }
    virtual void Final() {}

    virtual int LoadProperty(PropertyData& data) const;

    virtual int ActiveEx(wxApp* papp, wxWindow* pwindow, int* fparam = NULL, int* sparam = NULL);

    virtual bool NeedDBAccess() const { return true; }

};

#endif
