/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#include "Consts.h"
#include "ConfigData.h"

std::wstring CConfigData::m_strDBFile = APP_DEFAULT_DBFILE;

CConfigData::CConfigData(CDBAccess& db)
: _db(db.Database())
, m_iIfLanguage(0)
, m_iAutoHook(1)
, m_iOpenTrace(0)
, m_iOpenHotkey(1)
, m_iHotkey(0)
, m_iDataSyncTag(1)
, m_iDataSyncMem(1)
, m_iHTMLSave(1)
, m_iHTMLLoad(1)
, m_iExpandDict(-1)
//, m_iIgnoreDict(0)
, m_iAutoSpeak(0)
, m_iUseTidy(1)
//, m_iSaveUseTidy(0)
, m_iSkipDict(0)
, m_iSkipHtml(0)
, m_iFavoriteTab(1)
//, m_iDataProcFlag(1)
, m_iSkipError(1)
, m_iLoadHtmlDict(0)
, m_strLingoesExec(wxT("C:\\Program Files\\Lingoes\\Translator2\\Lingoes.exe"))
, m_strLingoesPath(wxT("C:\\Program Files\\Lingoes\\Translator2"))
, m_iRetrieveDelay(0)
, m_iAFCaseInsensitive(0)
, m_iAFOneWordEachDict(0)
, m_iAFOneWordAllDict(0)
, m_iStopAutoRetrieve(0)
, m_iHookLanguage(0)
{
}

int CConfigData::Init()
{
    try
    {
        if(!_db.TableExists(wxT("ConfigTable")))
        {
            const char* conftable = "CREATE TABLE ConfigTable (Attr INTEGER PRIMARY KEY, IntVal INTEGER, StrVal VARCHAR(255))";
            _db.ExecuteUpdate(conftable);

            return Save();
        }
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

int CConfigData::GetData(int attr, int& value)
{
    try
    {
        CDBAccess::TQuery query = _db.PrepareStatement("SELECT IntVal FROM ConfigTable WHERE Attr = ?");
	    query.Bind(1, attr);
        CDBAccess::TResult res = query.ExecuteQuery();
	    if(!res.IsOk())
            throw CDBAccess::TException(255, wxT("SELECT Attr of ConfigTable FAILED."));
	    if(!res.Eof())
	    {
            value = res.GetInt(0);
	    }
	    else 
	    {
            return -1;
	    }
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
	return 0;
}

int CConfigData::GetData(int attr, std::wstring& value)
{
    try
    {
        CDBAccess::TQuery query = _db.PrepareStatement("SELECT StrVal FROM ConfigTable WHERE Attr = ?");
	    query.Bind(1, attr);
        CDBAccess::TResult res = query.ExecuteQuery();
	    if(!res.IsOk())
            throw CDBAccess::TException(255, wxT("SELECT Attr of ConfigTable FAILED."));
	    if(!res.Eof())
	    {
            value = res.GetAsString(0);
	    }
	    else 
	    {
            return -1;
	    }
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
	return 0;
}

int CConfigData::SetData(int attr, int &value)
{
    try
    {
        CDBAccess::TQuery query = _db.PrepareStatement("SELECT IntVal FROM ConfigTable WHERE Attr = ?");
	    query.Bind(1, attr);
        CDBAccess::TResult res = query.ExecuteQuery();
	    if(!res.IsOk())
            throw CDBAccess::TException(255, wxT("SELECT Attr of ConfigTable FAILED."));
        if(!res.Eof())
        {
            query.Reset();
            query = _db.PrepareStatement("UPDATE ConfigTable SET IntVal = ? WHERE Attr = ?");
            query.Bind(1, value);
            query.Bind(2, attr);
            if(query.ExecuteUpdate() == 0)
                return -1;
        }
        else
        {
            query.Reset();
            query = _db.PrepareStatement("INSERT INTO ConfigTable (Attr, IntVal, StrVal) VALUES(?, ?, NULL)");
            query.Bind(1, attr);
            query.Bind(2, value);
            if(query.ExecuteUpdate() == 0)
                return -1;
        }
    }
	catch(const CDBAccess::TException& e)
	{
		return -1;
	}
    return 0;
}

int CConfigData::SetData(int attr, const std::wstring &value)
{
    try
    {
        CDBAccess::TQuery query = _db.PrepareStatement("SELECT StrVal FROM ConfigTable WHERE Attr = ?");
	    query.Bind(1, attr);
        CDBAccess::TResult res = query.ExecuteQuery();
	    if(!res.IsOk())
            throw CDBAccess::TException(255, wxT("SELECT Attr of ConfigTable FAILED."));
        if(!res.Eof())
        {
            query.Reset();
            query = _db.PrepareStatement("UPDATE ConfigTable SET StrVal = ? WHERE Attr = ?");
            query.Bind(1, value);
            query.Bind(2, attr);
            if(query.ExecuteUpdate() == 0)
                return -1;
        }
        else
        {
            query.Reset();
            query = _db.PrepareStatement("INSERT INTO ConfigTable (Attr, IntVal, StrVal) VALUES(?, -1, ?)");
            query.Bind(1, attr);
            query.Bind(2, value);
            if(query.ExecuteUpdate() == 0)
                return -1;
        }
    }
	catch(const CDBAccess::TException& e)
	{
		return -1;
	}
    return 0;
}

int CConfigData::Load()
{
    if(GetData(CA_IFLANGUAGE, m_iIfLanguage) != 0)
        m_iIfLanguage = 0;
    if(GetData(CA_OPENTRACE, m_iOpenTrace) != 0)
        m_iOpenTrace = 0;
    if(GetData(CA_OPENHOTKEY, m_iOpenHotkey) != 0)
        m_iOpenHotkey = 1;
    if(GetData(CA_HOTKEY, m_iHotkey) != 0)
        m_iHotkey = 0;
    if(GetData(CA_AUTOHOOK, m_iAutoHook) != 0)
        m_iAutoHook = 1;
    if(GetData(CA_DATASYNCTAG, m_iDataSyncTag) != 0)
        m_iDataSyncTag = 1;
    if(GetData(CA_DATASYNCMEM, m_iDataSyncMem) != 0)
        m_iDataSyncMem = 1;
    if(GetData(CA_HTMLSAVE, m_iHTMLSave) != 0)
        m_iHTMLSave = 1;
    if(GetData(CA_HTMLLOAD, m_iHTMLLoad) != 0)
        m_iHTMLLoad = 1;
    if(GetData(CA_EXPANDDICT, m_iExpandDict) != 0)
        m_iExpandDict = -1;
    //if(GetData(CA_IGNOREDICT, m_iIgnoreDict) != 0)
    //    m_iIgnoreDict = 0;
    if(GetData(CA_AUTOSPEAK, m_iAutoSpeak) != 0)
        m_iAutoSpeak = 0;
    if(GetData(CA_USETIDY, m_iUseTidy) != 0)
        m_iUseTidy = 1;
    //if(GetData(CA_SAVEUSETIDY, m_iSaveUseTidy) != 0)
    //    m_iSaveUseTidy = 0;
    if(GetData(CA_SKIPDICT, m_iSkipDict) != 0)
        m_iSkipDict = 0;
    if(GetData(CA_SKIPHTML, m_iSkipHtml) != 0)
        m_iSkipHtml = 0;
    if(GetData(CA_FAVORITETAB, m_iFavoriteTab) != 0)
        m_iFavoriteTab = 0;
    //if(GetData(CA_DATAPROCFLAG, m_iDataProcFlag) != 0)
    //    m_iDataProcFlag = 1;
    if(GetData(CA_SKIPERROR, m_iSkipError) != 0)
        m_iSkipError = 1;
    if(GetData(CA_LOADHTMLDICT, m_iLoadHtmlDict) != 0)
        m_iLoadHtmlDict = 0;

    if(GetData(CA_LINGOESEXEC, m_strLingoesExec) != 0)
        m_strLingoesExec = wxT("C:\\Program Files\\Lingoes\\Translator2\\Lingoes.exe");
    if(GetData(CA_LINGOESPATH, m_strLingoesPath) != 0)
        m_strLingoesPath = wxT("C:\\Program Files\\Lingoes\\Translator2");

    if(GetData(CA_RETRIEVEDELAY, m_iRetrieveDelay) != 0)
        m_iRetrieveDelay = 0;

    if(GetData(CA_AF_CASEINSENSITIVE, m_iAFCaseInsensitive) != 0)
        m_iAFCaseInsensitive = 0;
    if(GetData(CA_AF_ONEWORDEACHDICT, m_iAFOneWordEachDict) != 0)
        m_iAFOneWordEachDict = 0;
    if(GetData(CA_AF_ONEWORDALLDICT, m_iAFOneWordAllDict) != 0)
        m_iAFOneWordAllDict = 0;

    if(GetData(CA_STOPAUTORETRIEVE, m_iStopAutoRetrieve) != 0)
        m_iStopAutoRetrieve = 0;

    if(GetData(CA_HOOKLANGUAGE, m_iHookLanguage) != 0)
        m_iHookLanguage = 0;
    return 0;
}

int CConfigData::Save()
{
    if(SetData(CA_IFLANGUAGE, m_iIfLanguage) != 0)
        return -1;
    if(SetData(CA_OPENTRACE, m_iOpenTrace) != 0)
        return -1;
    if(SetData(CA_OPENHOTKEY, m_iOpenHotkey) != 0)
        return -1;
    if(SetData(CA_HOTKEY, m_iHotkey) != 0)
        return -1;
    if(SetData(CA_AUTOHOOK, m_iAutoHook) != 0)
        return -1;
    if(SetData(CA_DATASYNCTAG, m_iDataSyncTag) != 0)
        return -1;
    if(SetData(CA_DATASYNCMEM, m_iDataSyncMem) != 0)
        return -1;
    if(SetData(CA_HTMLSAVE, m_iHTMLSave) != 0)
        return -1;
    if(SetData(CA_HTMLLOAD, m_iHTMLLoad) != 0)
        return -1;
    if(SetData(CA_EXPANDDICT, m_iExpandDict) != 0)
        return -1;
    //if(SetData(CA_IGNOREDICT, m_iIgnoreDict) != 0)
    //    return -1;
    if(SetData(CA_AUTOSPEAK, m_iAutoSpeak) != 0)
        return -1;
    if(SetData(CA_USETIDY, m_iUseTidy) != 0)
        return -1;
    //if(SetData(CA_SAVEUSETIDY, m_iSaveUseTidy) != 0)
    //    return -1;
    if(SetData(CA_SKIPDICT, m_iSkipDict) != 0)
        return -1;
    if(SetData(CA_SKIPHTML, m_iSkipHtml) != 0)
        return -1;
    if(SetData(CA_FAVORITETAB, m_iFavoriteTab) != 0)
        return -1;
    //if(SetData(CA_DATAPROCFLAG, m_iDataProcFlag) != 0)
    //    return -1;
    if(SetData(CA_SKIPERROR, m_iSkipError) != 0)
        return -1;
    if(SetData(CA_LOADHTMLDICT, m_iLoadHtmlDict) != 0)
        return -1;

    if(SetData(CA_LINGOESEXEC, m_strLingoesExec) != 0)
        return -1;
    if(SetData(CA_LINGOESPATH, m_strLingoesPath) != 0)
        return -1;

    if(SetData(CA_RETRIEVEDELAY, m_iRetrieveDelay) != 0)
        return -1;

    if(SetData(CA_AF_CASEINSENSITIVE, m_iAFCaseInsensitive) != 0)
        return -1;
    if(SetData(CA_AF_ONEWORDEACHDICT, m_iAFOneWordEachDict) != 0)
        return -1;
    if(SetData(CA_AF_ONEWORDALLDICT, m_iAFOneWordAllDict) != 0)
        return -1;

    if(SetData(CA_STOPAUTORETRIEVE, m_iStopAutoRetrieve) != 0)
        return -1;

    if(SetData(CA_HOOKLANGUAGE, m_iHookLanguage) != 0)
        return -1;

    return 0;
}

unsigned int CConfigData::GetContolKey() const
{
    switch(m_iHotkey)
    {
    case 0:
    case 2:
    case 4:
    case 6:
        return MOD_CONTROL;
    case 1:
    case 3:
    case 5:
        return MOD_CONTROL | MOD_ALT;
    default:
        return MOD_CONTROL;
    }
}

unsigned int CConfigData::GetHotKey() const
{
    switch(m_iHotkey)
    {
    case 0:
    case 1:
        return VK_F10;
    case 2:
    case 3:
        return VK_F11;
    case 4:
    case 5:
        return VK_F11;
    case 6:
        return 0x31;
    default:
        return VK_F10;
    }
}

int CConfigData::GetLingoesParam(const std::wstring &local)
{
    m_strLingoesExec = local;
    std::wstring::size_type pos = local.find_last_of(wxT("\\"));
    if(pos != std::wstring::npos)
        m_strLingoesPath = local.substr(0, pos);
    else
        m_strLingoesPath = wxT("");

    return 0;
}

int CConfigData::SetLoadHtmlDict(int value)
{
    return SetData(CA_LOADHTMLDICT, value);
}

int CConfigData::SetAnalysisFilter()
{
    if(SetData(CA_AF_CASEINSENSITIVE, m_iAFCaseInsensitive) != 0)
        return -1;
    if(SetData(CA_AF_ONEWORDEACHDICT, m_iAFOneWordEachDict) != 0)
        return -1;
    if(SetData(CA_AF_ONEWORDALLDICT, m_iAFOneWordAllDict) != 0)
        return -1;
    return 0;
}
