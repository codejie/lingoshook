#ifndef __WORDEXPORT_H__
#define __WORDEXPORT_H__

#include "ActivityObject.h"

class WordExport : public ActivityObject
{
public:
    WordExport();
    virtual ~WordExport();

    virtual int Init(int fparam = 0, int sparam = 0) { return 0; }

    virtual int Run();

    virtual bool NeedDBAccess() const { return true; }
protected:
    virtual void InitProperty();
    virtual void Final() {}
};

extern WordExport* _theActivity;



#endif
