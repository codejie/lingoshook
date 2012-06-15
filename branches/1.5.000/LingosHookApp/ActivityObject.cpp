
#ifndef __TEST_PLUGINS__
#include "DBAccess.h"
#endif

#include "ActivityObject.h"

ActivityObject::ActivityObject()
#ifndef __TEST_PLUGINS__
: _objDBAccess(NULL)
//, _objDict(NULL)
//, _objTag(NULL)
#endif
{
    _iInterfaceVersion = __LH_PLUGINS_INTERFACEVERSION__;
    _iID = 0;
}

ActivityObject::~ActivityObject()
{
}

#ifndef __TEST_PLUGINS__

void ActivityObject::SetDBObject(CDBAccess *obj)
{
    _objDBAccess = obj;
}

//void ActivityObject::SetDictObject(CDictObject *obj)
//{
//	_objDict = obj;
//}
//
//void ActivityObject::SetTagObject(CTagObject *obj)
//{
//	_objTag = obj;
//}

#endif
