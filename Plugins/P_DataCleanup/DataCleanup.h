#ifndef __DATACLEANUP_H__
#define __DATACLEANUP_H__

#include "Exports.h"
#include "ActivityObject.h"

class DataCleanup : public ActivityObject
{
public:
	DataCleanup();
	virtual ~DataCleanup();

    virtual int Init(int fparam = 0, int sparam = 0) { return 0; }
    virtual void Final() {}

    virtual int LoadProperty(PropertyData& data) const;

    virtual int Active(wxApp* papp, wxWindow* pwindow);

    virtual bool NeedDBAccess() const { return true; }

};

#endif
