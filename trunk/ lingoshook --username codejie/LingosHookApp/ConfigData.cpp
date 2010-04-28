/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#include "ConfigData.h"

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
, m_iIgnoreDict(0)
, m_iAutoSpeak(0)
{
}

int CConfigData::Init()
{
    try
    {
        if(!_db.TableExists(_("ConfigTable")))
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
            throw CDBAccess::TException(255, _("SELECT Attr of ConfigTable FAILED."));
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

int CConfigData::GetData(int attr, wxString& value)
{
    try
    {
        CDBAccess::TQuery query = _db.PrepareStatement("SELECT StrVal FROM ConfigTable WHERE Attr = ?");
	    query.Bind(1, attr);
        CDBAccess::TResult res = query.ExecuteQuery();
	    if(!res.IsOk())
            throw CDBAccess::TException(255, _("SELECT Attr of ConfigTable FAILED."));
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
            throw CDBAccess::TException(255, _("SELECT Attr of ConfigTable FAILED."));
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

int CConfigData::SetData(int attr, wxString &value)
{
    try
    {
        CDBAccess::TQuery query = _db.PrepareStatement("SELECT COOUNT(*) FROM ConfigTable WHERE Attr = ?");
	    query.Bind(1, attr);
        CDBAccess::TResult res = query.ExecuteQuery();
	    if(!res.IsOk())
            throw CDBAccess::TException(255, _("SELECT Attr of ConfigTable FAILED."));
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
    if(GetData(CA_IGNOREDICT, m_iIgnoreDict) != 0)
        m_iIgnoreDict = 0;
    if(GetData(CA_AUTOSPEAK, m_iAutoSpeak) != 0)
        m_iAutoSpeak = 0;
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
    if(SetData(CA_IGNOREDICT, m_iIgnoreDict) != 0)
        return -1;
    if(SetData(CA_AUTOSPEAK, m_iAutoSpeak) != 0)
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
    default:
        return VK_F10;
    }
}
