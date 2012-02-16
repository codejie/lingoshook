/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#include "DBAccess.h"
#include "ConfigData.h"
#include "TriggerObject.h"

#include "TagObject.h"

const wxString CTagObject::DEFAULT_SYSTEM_TAG       =   _("Default");

CTagObject::CTagObject(CDBAccess& db)
: _db(db.Database())
, _iDefaultTag(-1)
, _iSysDefTag(-1)
{
}

CTagObject::~CTagObject()
{
}

int CTagObject::Init()
{
    try
    {
        //tags table
        if(!_db.TableExists(wxT("TagTable")))
        {
            const char* tagstable = "CREATE TABLE TagTable (TagID INTEGER PRIMARY KEY AUTOINCREMENT, Title VARCHAR(32), CreateTime TIMESTAMP DEFAULT (DATETIME('now', 'localtime')), Description VARCHAR(255))";
            _db.ExecuteUpdate(tagstable);
            const char* deftag = "INSERT INTO TagTable (Title, Description) VALUES ('Default', 'This is default system tag.')";
            _db.ExecuteUpdate(deftag);
            _iSysDefTag = _db.GetLastRowId().ToLong();
            CDBAccess::TQuery query = _db.PrepareStatement("INSERT INTO ConfigTable (Attr, IntVal) VALUES(?, ?)");
            query.Bind(1, CConfigData::CA_DEFAULTTAG);
            query.Bind(2, _iSysDefTag);
            query.ExecuteUpdate();
            _iDefaultTag = _iSysDefTag;
        }

        const char* tagindextable = "CREATE TABLE IF NOT EXISTS TagIndexTable (WordID INTEGER, TagID INTEGER, CheckinTime TIMESTAMP DEFAULT (DATETIME('now', 'localtime')))";
        _db.ExecuteUpdate(tagindextable);
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

int CTagObject::Load()
{
    _mapRecord.clear();

    try
    {
        CDBAccess::TResult res = _db.ExecuteQuery("SELECT TagID, Title, CreateTime, Description FROM TagTable");
        if(!res.IsOk())
            return -1;
        while(res.NextRow())
        {
            TRecord rec;
            rec.m_strTitle = res.GetAsString(1);
            
            if(rec.m_strTitle == DEFAULT_SYSTEM_TAG)
            {
                _iSysDefTag = res.GetInt(0);
            }

            rec.m_dtCreateTime = res.GetTimestamp(2);
            rec.m_strDesc = res.GetAsString(3);
            rec.m_uiCounter = 0;

            _mapRecord.insert(std::make_pair(res.GetInt(0), rec));
        }

        res = _db.ExecuteQuery("SELECT TagID, COUNT(*) FROM TagIndexTable GROUP BY TagID");
        if(!res.IsOk())
            return -1;
        while(res.NextRow())
        {
            TRecordMap::iterator it = _mapRecord.find(res.GetInt(0));
            if(it != _mapRecord.end())
            {
                it->second.m_uiCounter = res.GetInt(1);
            }
        }

        if(_iDefaultTag == -1)
        {
            CDBAccess::TQuery query = _db.PrepareStatement("SELECT IntVal FROM ConfigTable WHERE Attr = ?");
            query.Bind(1, CConfigData::CA_DEFAULTTAG);
            res = query.ExecuteQuery();
            if(!res.IsOk())
                return -1;
            if(!res.Eof())
            {
                _iDefaultTag = res.GetInt(0);
            }
            else
            {
                return -1;
            }
        }

        for(TRecordMap::const_iterator it = _mapRecord.begin(); it != _mapRecord.end(); ++ it)
        {
            g_objTrigger.OnTagLoad(it->first, it->second);
            if(it->first == _iDefaultTag)
                g_objTrigger.OnTagDefLoad(it->first, it->second);
        }
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

int CTagObject::InsertTag(const wxString& title, const wxString& desc)
{
    if(title == DEFAULT_SYSTEM_TAG)
    {
        return -1;
    }
    try
    {
        CDBAccess::TQuery query = _db.PrepareStatement("INSERT INTO TagTable (Title, Description) VALUES (?, ?)");
        query.Bind(1, title);
        query.Bind(2, desc);
        if(query.ExecuteUpdate() == 0)
  		    throw wxSQLite3Exception(255, _("Insert Tag Table failed."));
        
        int id = _db.GetLastRowId().ToLong();

        TRecord rec;
        rec.m_strTitle = title;
        rec.m_dtCreateTime = ::time(NULL);
        rec.m_strDesc = desc;
        rec.m_uiCounter = 0; 
    
        _mapRecord.insert(std::make_pair(id, rec));

        g_objTrigger.OnTagInsert(id, _mapRecord[id]);
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

int CTagObject::RemoveTag(int id)
{
    if(id == _iSysDefTag)
    {
        return -1;
    }
    try
    {
        CDBAccess::TQuery query = _db.PrepareStatement("DELETE FROM TagTable WHERE TagID = ?");
        query.Bind(1, id);
        if(query.ExecuteUpdate() != 0)
        {
            _mapRecord.erase(id);

            if(id == _iDefaultTag)
            {
                UpdateDefaultTag(_iSysDefTag);
            }
            
            query.Reset();
            query = _db.PrepareStatement("DELETE FROM TagIndexTable where TagID=? AND WordID IN (SELECT WordID FROM TagIndexTable WHERE TagID=?)");
            query.Bind(1, id);
            query.Bind(2, _iSysDefTag);
            query.ExecuteUpdate();

            CDBAccess::TQuery qry = _db.PrepareStatement("UPDATE TagIndexTable SET TagID = ? WHERE TagID = ?");
            qry.Bind(1, _iSysDefTag);
            qry.Bind(2, id);
            int rows = qry.ExecuteUpdate();

            _mapRecord[_iDefaultTag].m_uiCounter += rows;
            g_objTrigger.OnTagUpdateCount(_iSysDefTag, _mapRecord[_iSysDefTag]);

            g_objTrigger.OnTagRemove(id);
        }
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

int CTagObject::RenameTag(int tagid, const wxString& title, const wxString& desc)
{
    if(tagid == _iSysDefTag)
    {
        return -1;
    }

    try
    {
        CDBAccess::TQuery query = _db.PrepareStatement("UPDATE TagTable SET Title=?, Description=? WHERE TagID = ?");
        query.Bind(1, title);
        query.Bind(2, desc);
        query.Bind(3, tagid);
        if(query.ExecuteUpdate() != 0)
        {
            TRecordMap::iterator it = _mapRecord.find(tagid);
            if(it != _mapRecord.end())
            {
                it->second.m_strTitle = title;
                it->second.m_strDesc = desc;
            }
            else
            {
                return -1;
            }

            g_objTrigger.OnTagRename(tagid, it->second);
        }
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

int CTagObject::UpdateDefaultTag(int id)
{
    try
    {
        CDBAccess::TQuery query = _db.PrepareStatement("UPDATE ConfigTable SET IntVal = ? WHERE Attr = ?");
        query.Bind(1, id);
        query.Bind(2, CConfigData::CA_DEFAULTTAG);
        if(query.ExecuteUpdate() == 0)
            return -1;
        _iDefaultTag = id;

        g_objTrigger.OnTagDefLoad(_iDefaultTag, _mapRecord[_iDefaultTag]);
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

int CTagObject::AddIndex(int wordid, int tagid)
{
    if(IsIndexExist(wordid, tagid) == 0)
        return 0;
    if(InsertIndex(wordid, tagid) != 0)
        return -1;

    g_objTrigger.OnTagUpdateCount(tagid, _mapRecord[tagid]);

    g_objTrigger.OnTagIndexUpdate(wordid, tagid);

    return 0;
}

int CTagObject::DeleteIndex(int wordid, int tagid)
{
    if(IsOnlySysDefTag(wordid) == 0)
        return 0;
    if(RemoveIndex(wordid, tagid) != 0)
        return -1;

    g_objTrigger.OnTagUpdateCount(tagid, _mapRecord[tagid]);

    if(IsNoTag(wordid) == 0)
    {
        if(InsertIndex(wordid, _iSysDefTag) != 0)
        {
            return -1;
        }
        g_objTrigger.OnTagUpdateCount(_iSysDefTag, _mapRecord[_iSysDefTag]);
    }

    g_objTrigger.OnTagIndexUpdate(wordid, tagid);

    return 0;
}

int CTagObject::DeleteWord(int wordid)
{
    try
    {
        CDBAccess::TQuery query = _db.PrepareStatement("SELECT TagID FROM TagIndexTable WHERE WordID = ?");
        query.Bind(1, wordid);
        CDBAccess::TResult res = query.ExecuteQuery();
        if(!res.IsOk())
            return -1;
        while(res.NextRow())
        {
            int tagid = res.GetInt(0);
            if(RemoveIndex(wordid, tagid) != 0)
                return -1;
            g_objTrigger.OnTagUpdateCount(tagid, _mapRecord[tagid]);    
        }
        g_objTrigger.OnTagIndexUpdate(wordid, -1);
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

int CTagObject::IsIndexExist(int wordid, int tagid)
{
    try
    {
        CDBAccess::TQuery query = _db.PrepareStatement("SELECT COUNT(*) FROM TagIndexTable WHERE WordID = ? AND TagID = ?");
        query.Bind(1, wordid);
        query.Bind(2, tagid);
        CDBAccess::TResult res = query.ExecuteQuery();
        if(res.GetInt(0) == 0)
            return -1;
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

int CTagObject::IsOnlySysDefTag(int wordid)
{
    try
    {
        CDBAccess::TQuery query = _db.PrepareStatement("SELECT COUNT(*) FROM TagIndexTable WHERE WordID = ? AND TagID != ?");
        query.Bind(1, wordid);
        query.Bind(2, _iSysDefTag);
        CDBAccess::TResult res = query.ExecuteQuery();
        if(res.GetInt(0) != 0)
            return -1;
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

int CTagObject::IsNoTag(int wordid)
{
    try
    {
        CDBAccess::TQuery query = _db.PrepareStatement("SELECT COUNT(*) FROM TagIndexTable WHERE WordID = ?");
        query.Bind(1, wordid);
        CDBAccess::TResult res = query.ExecuteQuery();
        if(res.GetInt(0) != 0)
            return -1;
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

int CTagObject::InsertIndex(int wordid, int tagid)
{
    try
    {
        CDBAccess::TQuery query = _db.PrepareStatement("INSERT INTO TagIndexTable (WordID, TagID) VALUES (? , ?)");
        query.Bind(1, wordid);
        query.Bind(2, tagid);
        if(query.ExecuteUpdate() == 0)
            return -1;
        TRecordMap::iterator it = _mapRecord.find(tagid);
        if(it != _mapRecord.end())
            ++ it->second.m_uiCounter;
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

int CTagObject::RemoveIndex(int wordid, int tagid)
{
    try
    {
        CDBAccess::TQuery query = _db.PrepareStatement("DELETE FROM TagIndexTable WHERE WordID = ? AND TagID = ?");
        query.Bind(1, wordid);
        query.Bind(2, tagid);
        if(query.ExecuteUpdate() != 0)
        {
            TRecordMap::iterator it = _mapRecord.find(tagid);
            if(it != _mapRecord.end())
                -- it->second.m_uiCounter;
        }
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

int CTagObject::GetWordByTag(int tagid)
{
    try
    {
        CDBAccess::TQuery query = _db.PrepareStatement("SELECT WordID FROM TagIndexTable WHERE TagID = ? ORDER BY TagID");
        query.Bind(1, tagid);
        CDBAccess::TResult res = query.ExecuteQuery();
        if(!res.IsOk())
            return -1;
        while(res.NextRow())
        {
            g_objTrigger.OnTagGetWord(tagid, res.GetInt(0));
        }
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;    
}

int CTagObject::GetTagByWord(int wordid)
{
    try
    {
        CDBAccess::TQuery query = _db.PrepareStatement("SELECT TagID FROM TagIndexTable WHERE WordID = ? ORDER BY WordID DESC");
        query.Bind(1, wordid);
        CDBAccess::TResult res = query.ExecuteQuery();
        if(!res.IsOk())
            return -1;
        
        g_objTrigger.OnTagFoundByWord(wordid);        

        while(res.NextRow())
        {
            g_objTrigger.OnTagGetByWord(res.GetInt(0), wordid);
        }
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;    
}

int CTagObject::GetWordByTag(int tagid, TWordIDVector& vctwordid)
{
    try
    {
        CDBAccess::TQuery query = _db.PrepareStatement("SELECT WordID FROM TagIndexTable WHERE TagID = ?");
        query.Bind(1, tagid);
        CDBAccess::TResult res = query.ExecuteQuery();
        if(!res.IsOk())
            return -1;
        while(res.NextRow())
        {
            vctwordid.push_back(res.GetInt(0));
        }
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;    
}

int CTagObject::GetTagByWord(int wordid, TTagIDVector& vcttagid)
{
    try
    {
        CDBAccess::TQuery query = _db.PrepareStatement("SELECT TagID FROM TagIndexTable WHERE WordID = ?");
        query.Bind(1, wordid);
        CDBAccess::TResult res = query.ExecuteQuery();
        if(!res.IsOk())
            return -1;
        while(res.NextRow())
        {
            vcttagid.push_back(res.GetInt(0));
        }
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;    
}

int CTagObject::DefaultTag() const
{
    return _iDefaultTag;
}

int CTagObject::SysDefTag() const
{
    return _iSysDefTag;
}

const wxString CTagObject::GetTitle(int tagid) const
{
    TRecordMap::const_iterator it = _mapRecord.find(tagid);
    if(it != _mapRecord.end())
        return it->second.m_strTitle;
    return _("Undefined");
}

const wxString CTagObject::GetDescription(int tagid) const
{
    TRecordMap::const_iterator it = _mapRecord.find(tagid);
    if(it != _mapRecord.end())
        return it->second.m_strDesc;
    return _("");
}

int CTagObject::ShowTagSubMenu(int menubase, wxMenu*& submenu) const
{
    submenu = new wxMenu;
    for(TRecordMap::const_iterator it = _mapRecord.begin(); it != _mapRecord.end(); ++ it)
    {
        if(it->first != _iDefaultTag)
        {
            submenu->Append(menubase + it->first, it->second.m_strTitle);
        }
        else
        {
            submenu->AppendRadioItem(menubase + it->first, it->second.m_strTitle);
        }
    }

    return 0;
}
