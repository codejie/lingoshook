#ifndef __CLIENTDATAEXPORT_H__
#define __CLIENTDATAEXPORT_H__

#include "Exports.h"
#include "ActivityObject.h"

class ClientDataExport : public ActivityObject
{
public:
    ClientDataExport();
    virtual ~ClientDataExport();

    virtual int Init(int fparam = 0, int sparam = 0) { return 0; }
    virtual void Final() {}

    virtual int LoadProperty(PropertyData& data) const;

    virtual int Active(wxApp* papp, wxWindow* pwindow);

    virtual bool NeedDBAccess() const { return true; }

};

#endif
