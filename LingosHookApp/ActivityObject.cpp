
#include "DBAccess.h"

#include "ActivityObject.h"

ActivityObject::ActivityObject()
: _objDBAccess(NULL)
{
    InitProperty();
}

ActivityObject::~ActivityObject()
{
    Final();
}

void ActivityObject::SetDBObject(CDBAccess *obj)
{
    _objDBAccess = obj;
}