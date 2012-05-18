#ifndef __WORDEXPORT_H__
#define __WORDEXPORT_H__

#include "Exports.h"
#include "ActivityObject.h"

class WordExport : public ActivityObject
{
public:
    WordExport();
    virtual ~WordExport();

    virtual int Init(int fparam = 0, int sparam = 0) { return 0; }
    virtual void Final() {}

    virtual int LoadProperty(PropertyData& data) const;

    virtual int Active(wxApp* papp, wxWindow* pwindow);

    virtual bool NeedDBAccess() const { return true; }
};

//extern WordExport* _theActivity;



#endif
