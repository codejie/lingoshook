
#include "DBAccess.h"

#include "ActivityObject.h"

ActivityObject::ActivityObject()
: _objDBAccess(NULL)
{
}

ActivityObject::~ActivityObject()
{
}

void ActivityObject::SetDBObject(CDBAccess *obj)
{
    _objDBAccess = obj;
}