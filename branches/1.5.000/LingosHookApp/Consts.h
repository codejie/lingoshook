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
const wxString APP_TITLE        = wxT("LingosHook"); 

#ifdef __LH_DEBUG__
const wxString APP_VERSION      = wxT("1.5.xxx(Debug)");
#else
const wxString APP_VERSION      = wxT("1.5.xxx");
#endif

//App message
#define WM_OBJECT_INIT      WM_USER + 1000
#define WM_SET_HOOK         WM_USER + 1001
#define WM_SET_AUTORETRIEVE WM_USER + 1002

//wxHtmlWindow or wxIE
#define __LH_USE_WXIE__

const wxString APP_DEFAULT_DBFILE = wxT("LingosHook.db3");

const unsigned int __LH_PLUGINS_INTERFACEVERSION__    =   1;

#endif