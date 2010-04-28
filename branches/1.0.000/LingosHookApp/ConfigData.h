/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#ifndef __CONFIGDATA_H__
#define __CONFIGDATA_H__

#include "wx/wx.h"

#include "DBAccess.h"

class CConfigData
{
public:
    static const int CA_IFLANGUAGE      =   1;
    static const int CA_OPENTRACE       =   2;
    static const int CA_OPENHOTKEY      =   3;
    static const int CA_HOTKEY          =   4;
    static const int CA_AUTOHOOK        =   5;
    static const int CA_DEFAULTTAG      =   6;
    static const int CA_DATASYNCTAG     =   7;
    static const int CA_DATASYNCMEM     =   8;
    static const int CA_HTMLSAVE        =   9;
    static const int CA_HTMLLOAD        =   10;
    static const int CA_EXPANDDICT      =   11;
    static const int CA_IGNOREDICT      =   12;
    static const int CA_AUTOSPEAK       =   13;
public:
    CConfigData(CDBAccess& db);
	virtual ~CConfigData() {}

    int Init();

    int Load();
    int Save();

    unsigned int GetContolKey() const;
    unsigned int GetHotKey() const;
protected:
    int GetData(int attr, int& value);
    int GetData(int attr, wxString& value);
    int SetData(int attr, int& value);
    int SetData(int attr, wxString& value);
protected:
    CDBAccess::TDatabase& _db;
public:
	int m_iIfLanguage;
    int m_iAutoHook;
	int m_iOpenTrace;
    int m_iOpenHotkey;
    int m_iHotkey;
    int m_iDataSyncTag;
    int m_iDataSyncMem;
    int m_iHTMLSave;
    int m_iHTMLLoad;
    int m_iExpandDict;
    int m_iIgnoreDict;
    int m_iAutoSpeak;
};


#endif
