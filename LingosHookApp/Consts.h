/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#ifndef __CONSTS_H__
#define __CONSTS_H__

#include <windows.h>

#include "wx/wx.h"

//#define __LH_DEBUG__

//App
const wxString APP_TITLE        = _("LingosHook"); 

#ifdef __LH_DEBUG__
const wxString APP_VERSION      = _("1.0.004(Debug)");
#else
const wxString APP_VERSION      = _("1.0.004");
#endif

//App message
#define WM_OBJECT_INIT      WM_USER + 1000


#endif
