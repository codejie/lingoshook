#ifndef __EXPORTS_H__
#define __EXPORTS_H__

#ifdef LINGOSHOOK_PLUGINS_EXPORT
#define LINGOSHOOK_PLUGINS_API __declspec(dllexport)
#endif

#include "ActivityObject.h"

LINGOSHOOK_PLUGINS_API ActivityObject* GetActivity();

#endif
