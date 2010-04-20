#ifndef __LHSPEAK_H__
#define __LHSPEAK_H__

#include <windows.h>

#define DllExport __declspec(dllexport)

#if defined(__cplusplus)
extern "C"
{
#endif

DllExport BOOL Speak(LPCTSTR word);

#if defined(__cplusplus)
}
#endif

extern BOOL Init();
extern VOID Final();
extern BOOL Speak(LPCTSTR word);

#endif
