/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#ifndef __CONSTS_H__
#define __CONSTS_H__

#include <windows.h>

#include "wx/wx.h"

//Debug or Relase
//#define __LH_DEBUG__

//App
const wxString APP_TITLE        = _("LingosHook"); 

#ifdef __LH_DEBUG__
const wxString APP_VERSION      = _("1.1.000(Debug)");
#else
const wxString APP_VERSION      = _("1.1.000");
#endif

//App message
#define WM_OBJECT_INIT      WM_USER + 1000

//wxHtmlWindow or wxIE
#define __LH_USE_WXIE__

#endif
