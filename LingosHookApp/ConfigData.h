/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#ifndef __CONFIGDATA_H__
#define __CONFIGDATA_H__

#include <string>

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
    static const int CA_USETIDY         =   14;
    static const int CA_SAVEUSETIDY     =   15;
    static const int CA_SKIPDICT        =   16;//special dict
    static const int CA_SKIPHTML        =   17;//html dict
    static const int CA_FAVORITETAB     =   18;
    static const int CA_DATAPROCFLAG    =   19;
    static const int CA_SKIPERROR       =   20;
    static const int CA_LOADHTMLDICT    =   21;
    static const int CA_LINGOESEXEC     =   22;
    static const int CA_LINGOESPATH     =   23;
    static const int CA_RETRIEVEDELAY   =   24;
    static const int CA_AF_CASEINSENSITIVE  =   25;
    static const int CA_AF_ONEWORDEACHDICT  =   26;
    static const int CA_AF_ONEWORDALLDICT   =   27;
public:
    CConfigData(CDBAccess& db);
	virtual ~CConfigData() {}

    int Init();

    int Load();
    int Save();

    unsigned int GetContolKey() const;
    unsigned int GetHotKey() const;

    int GetLingoesParam(const std::wstring& local);

    int SetLoadHtmlDict(int value);
    int SetAnalysisFilter();
protected:
    int GetData(int attr, int& value);
    int GetData(int attr, std::wstring& value);
    int SetData(int attr, int& value);
    int SetData(int attr, const std::wstring& value);
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
    //int m_iIgnoreDict;
    int m_iAutoSpeak;
    int m_iUseTidy;
//    int m_iSaveUseTidy;
    int m_iSkipDict;
    int m_iSkipHtml;
    int m_iFavoriteTab;
//    int m_iDataProcFlag;//0:normal; 1: Ignore Dict error; 2: Skip Dict; 3: Skip HTML
    int m_iSkipError;
    int m_iLoadHtmlDict;//0: only html; 1: only dict; 2: dict -> html
    std::wstring m_strLingoesExec;
    std::wstring m_strLingoesPath;
    int m_iRetrieveDelay;
    int m_iAFCaseInsensitive;
    int m_iAFOneWordEachDict;
    int m_iAFOneWordAllDict;
};


#endif
