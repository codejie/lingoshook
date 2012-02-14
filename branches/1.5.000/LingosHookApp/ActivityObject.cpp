
#ifndef __TEST_PLUGINS__
#include "DBAccess.h"
#endif

#include "ActivityObject.h"

ActivityObject::ActivityObject()
#ifndef __TEST_PLUGINS__
: _objDBAccess(NULL)
#endif
{
    _iInterfaceVersion = __LH_PLUGINS_INTERFACEVERSION__;
}

ActivityObject::~ActivityObject()
{
}

#ifndef __TEST_PLUGINS__

void ActivityObject::SetDBObject(CDBAccess *obj)
{
    _objDBAccess = obj;
}

#endif
