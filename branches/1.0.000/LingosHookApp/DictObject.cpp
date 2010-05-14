/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#include <string>
#include <memory>

#include "ConfigData.h"
#include "HtmlTidyObject.h"
#include "ViconDictObject.h"
#include "LangdaoDictObject.h"
#include "FundsetDictObject.h"
#include "TriggerObject.h"
#include "DictObject.h"


CDictResult::CDictResult(const CParserResult* result)
: _result(NULL)
{
    if(result != NULL)
    {
        _result.reset(result);
    }
}

CDictResult::~CDictResult()
{
}

const CParserResult* CDictResult::Result() const
{
    return _result.get();;
}

void CDictResult::Attach(const CParserResult* result)
{
    _result.reset(result);
}

CDictResult::CDictResult(const CDictResult& right)
{
    if(this != &right)
    {
        _result.reset((const_cast<CDictResult&>(right))._result.release());
    }
}

////////////////////////////////////////
CDictObject::CDictObject(CDBAccess &db, CConfigData& config)
: _db(db.Database())
, _config(config)
, _strCacheWord(wxEmptyString)
{
}

CDictObject::~CDictObject()
{
    FreeParser();
}

int CDictObject::Init()
{
    //word table
    try
    {
        if(!_db.TableExists(_("DictTable")))
        {
		    const char* dicttable = "CREATE TABLE DictTable (ID INTEGER PRIMARY KEY AUTOINCREMENT,DictID VARCHAR(64), Title VARCHAR(128),CreateTime TIMESTAMP DEFAULT (DATETIME('now', 'localtime')))";
		    _db.ExecuteUpdate(dicttable);

        }
        if(AddKnownParser() != 0)
            return -1;

        if(LoadParser() != 0)
            return -1;
 
		const char* wordtable = "CREATE TABLE IF NOT EXISTS WordTable (ID INTEGER PRIMARY KEY AUTOINCREMENT,Word VARCHAR(32) UNIQUE,Counter INTEGER, CheckinTime TIMESTAMP DEFAULT (DATETIME('now', 'localtime')), UpdateTime TIMESTAMP DEFAULT (DATETIME('now', 'localtime')),HTML TEXT)";
		_db.ExecuteUpdate(wordtable);

    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

int CDictObject::AddKnownParser()
{
    if(RegisterParser(VICON::CECParser::ID, VICON::CECParser::TITLE) != 0)
        return -1;
    if(RegisterParser(LANGDAO::CECParser::ID, LANGDAO::CECParser::TITLE) != 0)
        return -1;
    if(RegisterParser(FUNDSET::CDCParser::ID, FUNDSET::CDCParser::TITLE) != 0)
        return -1;
    return 0;
}

int CDictObject::RegisterParser(const wxString& id, const wxString& title)
{
    try
    {
        CDBAccess::TQuery query = _db.PrepareStatement("SELECT COUNT(*) FROM DictTable WHERE DictID = ?");
        query.Bind(1, id);
        CDBAccess::TResult res = query.ExecuteQuery();
        if(res.GetInt(0) != 0)
            return 0;

        query.Reset();
        query = _db.PrepareStatement("INSERT INTO DictTable (DictID, Title) VALUES(?, ?)");
        query.Bind(1, id);
        query.Bind(2, title);
        if(query.ExecuteUpdate() == 0)
            return -1;
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

int CDictObject::LoadParser()
{
    try
    {
        CDBAccess::TResult res = _db.ExecuteQuery("SELECT ID, DictID, Title, CreateTime FROM DictTable");
        if(!res.IsOk())
            return -1;
        while(res.NextRow())
        {
            wxString dictid = res.GetString(1);
            std::auto_ptr<CDictParser> p(NULL);

            if(dictid == VICON::CECParser::ID)
            {
                p.reset(new VICON::CECParser(res.GetInt(0), res.GetString(1), res.GetString(2), res.GetTimestamp(3)));
            }
            else if(dictid == LANGDAO::CECParser::ID)
            {
                p.reset(new LANGDAO::CECParser(res.GetInt(0), res.GetString(1), res.GetString(2), res.GetTimestamp(3)));
            }
            else if(dictid == FUNDSET::CDCParser::ID)
            {
                p.reset(new FUNDSET::CDCParser(res.GetInt(0), res.GetString(1), res.GetString(2), res.GetTimestamp(3)));
            }
            else
            {
                g_objTrigger.OnParserUnknown(res.GetString(1), res.GetString(2));
            }
            if(p.get() == NULL)
                continue;

            if(p->Init(_db) == 0)
            {
                _mapParser.insert(std::make_pair(dictid, p.release()));
                g_objTrigger.OnParserLoad(res.GetInt(0), dictid, res.GetString(2));
            }
            else
            {
                g_objTrigger.OnParserInitFail(res.GetString(1), res.GetString(2));
            }
        }
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

//int CDictObject::AddParser(const wxString& id)
//{
//    if(GetParser(id) == NULL)
//        return -1;
//    return 0;
//}

CDictParser* CDictObject::GetParser(const wxString &id)
{
    TParserMap::iterator it = _mapParser.find(id);
    if(it != _mapParser.end())
        return it->second;

    return NULL;
}

void CDictObject::FreeParser()
{
    TParserMap::iterator it = _mapParser.begin();
    while(it != _mapParser.end())
    {
        delete it->second;
        _mapParser.erase(it ++ );
    }
}

void CDictObject::CacheWord(const wxString& word)
{
    _strCacheWord = word;
}

int CDictObject::HTMLProc(const wxString &str, int mode)
{
    if(mode == 0)
    {
        return HTMLProc(str);
    }
    else if(mode == 1)
    {
        if(HTMLProc(str) != 0)
        {
            return ForceSaveHTML(str);
        }
    }
    else
    {
        return ForceSaveHTML(str);
    }
    return 0;
}

int CDictObject::HTMLProc(const wxString &str)
{
    std::wstring html(str.begin(), str.end());

    //find dict
    TinyHtmlParser::CDocumentObject doc;

    if(_config.m_iUseTidy == 1)
    {
        if(CHtmlTidyObject::Tidy(html, html) != 0)
            return -1;
    }

    try
    {
        if(doc.Load(html, false) != 0)
        {
            g_objTrigger.OnHTMLParserFail(html);
            return -1;
        }
    }
    catch(TinyHtmlParser::CExceptionObject& e)
    {
        g_objTrigger.OnHTMLParserException(html, e);
        return -1;
    }

    TWordResultMap result;

    const TinyHtmlParser::CElementObject* pe = doc.FindFirstElement(L"DIV");
    while(pe != NULL)
    {
        const TinyHtmlParser::CAttributeObject* pa = pe->FindAttribute(L"style");
        if(pa != NULL && pa->value == L"\"PADDING-RIGHT: 10px; PADDING-LEFT: 10px; FONT-SIZE: 10.5pt; PADDING-BOTTOM: 0px; WIDTH: 100%; LINE-HEIGHT: 1.2em; PADDING-TOP: 10px; FONT-FAMILY: 'Tahoma'\"")
        {
            pa = pe->FindAttribute(L"dictid");
            if(pa != NULL)
            {
                wxString dictid(pa->value.c_str(), wxConvISO8859_1);
                dictid = dictid.substr(1, dictid.size() - 2);
                CDictParser* parser = GetParser(dictid);
                if(parser != NULL)
                {
                    if(parser->ParserHTML(html, doc, pe, result) != 0)
                    {
                       
                    }
                }
                else
                {
                }
            }
        }
        pe = doc.FindNextElement();
    }

    //str = html.c_str();

    if(SaveResult(str, result) == 0)
    {
        g_objTrigger.OnResultSave(result);
        return 0;
    }

    return -1;
}

int CDictObject::ForceSaveHTML(const wxString& str)
{
    if(_strCacheWord.empty())
        return -1;
    int wordid = -1;

    int ret = SaveWord(_strCacheWord, str, wordid);

    _strCacheWord = wxEmptyString;

    return ret;
}

int CDictObject::SaveWord(const wxString& word, const wxString& html, int& wordid)
{
    CDBAccess::TQuery query = _db.PrepareStatement("SELECT ID, Counter FROM WordTable WHERE Word = ?");
	query.Bind(1, word);
    CDBAccess::TResult res = query.ExecuteQuery();
	if(!res.IsOk())
        throw CDBAccess::TException(255, _("SELECT ID of WordTable FAILED."));
	if(!res.Eof())
	{
		wordid = res.GetInt(0);
        int counter = res.GetInt(1);

        UpdateWordData(wordid, counter + 1);

        g_objTrigger.OnWordUpdate(wordid, word);
	}
	else
	{
        if(_config.m_iHTMLSave == 1)
        {
		    CDBAccess::TQuery query = _db.PrepareStatement("INSERT INTO WordTable (Word, Counter, HTML) VALUES(?, 1, ?)");
		    query.Bind(1, word);
		    query.Bind(2, html);
		    query.ExecuteUpdate();

		    wordid = _db.GetLastRowId().ToLong();
        }
        else
        {
		    CDBAccess::TQuery query = _db.PrepareStatement("INSERT INTO WordTable (Word, Counter) VALUES(?, 1)");
		    query.Bind(1, word);
		    query.ExecuteUpdate();

		    wordid = _db.GetLastRowId().ToLong();
        }

        g_objTrigger.OnWordSave(wordid, word);
 	}

	return 0;    
}

int CDictObject::UpdateWordData(int wordid, int counter)
{
    CDBAccess::TQuery query = _db.PrepareStatement("UPDATE WordTable SET Counter = ?, UpdateTime = DATETIME('NOW', 'LOCALTIME') WHERE ID = ?");
    query.Bind(1, counter);
    query.Bind(2, wordid);
    query.ExecuteUpdate();

    return 0;
}

int CDictObject::SaveResult(const wxString& html, const TWordResultMap &result)
{
    if(result.size() == 0)
        return -1;

    try
    {
        _db.Begin();

        for(TWordResultMap::const_iterator it = result.begin(); it != result.end(); ++ it)
        {
            int wordid = -1;
            SaveWord(it->first, html, wordid);
            for(TDictResultMap::const_iterator i = it->second.begin(); i != it->second.end(); ++ i)
            {
                TParserMap::iterator p = _mapParser.find(i->first);
                if(p != _mapParser.end())
                {
                    p->second->SaveResult(_db, wordid, i->second);

                    g_objTrigger.OnResultSave(wordid, p->second, i->second);
                }
                else
                {
                }
            }
        }
        _db.Commit();
    }
    catch(CDBAccess::TException& e)
    {
        _db.Rollback();
        return -1;
    }
    return 0;
}



int CDictObject::GetWordData(int wordid, TWordData& data)
{
    try
    {
        if(_config.m_iHTMLLoad == 1)
        {
	        CDBAccess::TQuery query = _db.PrepareStatement("SELECT Word, Counter, CheckinTime, UpdateTime, HTML FROM WordTable WHERE ID = ?");
	        query.Bind(1, wordid);
	        CDBAccess::TResult res = query.ExecuteQuery();
	        if(!res.IsOk())
		        return -1;
	        if(!res.Eof())
	        {
                data.m_iID = wordid;
                data.m_strWord = res.GetString(0);
                data.m_iCounter = res.GetInt(1);
                data.m_dtCheckin = res.GetDateTime(2);
                data.m_dtUpdate = res.GetDateTime(3);
                data.m_strHTML = res.GetString(4);

//                g_objTrigger.OnWordDataGet(data);
            }
            else
            {
                return -1;
            }
        }
        else
        {
	        CDBAccess::TQuery query = _db.PrepareStatement("SELECT Word, Counter, CheckinTime, UpdateTime FROM WordTable WHERE ID = ?");
	        query.Bind(1, wordid);
	        CDBAccess::TResult res = query.ExecuteQuery();
	        if(!res.IsOk())
		        return -1;
	        if(!res.Eof())
	        {
                data.m_iID = wordid;
                data.m_strWord = res.GetString(0);
                data.m_iCounter = res.GetInt(1);
                data.m_dtCheckin = res.GetDateTime(2);
                data.m_dtUpdate = res.GetDateTime(3);
                data.m_strHTML = _("<HTML></HTML>");

//                g_objTrigger.OnWordDataGet(data);
            }
            else
            {
                return -1;
            }
        }
    }
    catch(CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

int CDictObject::GetResult(int wordid, TWordData &data, TDictResultMap &result)
{
    if(GetWordData(wordid, data) != 0)
        return -1;

    g_objTrigger.OnWordFound(wordid, data.m_strWord);

    for(TParserMap::iterator it = _mapParser.begin(); it != _mapParser.end(); ++ it)
    {
        if(it->second->GetResult(_db, wordid, result) != 0)
        {
            return -1;
        }
    }
    g_objTrigger.OnWordResultGetOver(wordid, data);
    return 0;
}

int CDictObject::GetResult(int wordid)
{
    try
    {
        TWordData data;
        if(GetWordData(wordid, data) != 0)
            return -1;

        g_objTrigger.OnWordFound(wordid, data.m_strWord);

        for(TParserMap::iterator it = _mapParser.begin(); it != _mapParser.end(); ++ it)
        {
            CDictResult result;
            if(it->second->GetResult(_db, wordid, result) == 0)
            {
                g_objTrigger.OnResultGet(wordid, it->second, result);
            }
        }
        g_objTrigger.OnWordResultGetOver(wordid, data);
    }
    catch(CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

int CDictObject::RemoveWord(int wordid)
{
    try
    {
        CDBAccess::TQuery query = _db.PrepareStatement("DELETE FROM WordTable WHERE ID = ?");
        query.Bind(1, wordid);
        if(query.ExecuteUpdate() != 0)
        {
            for(TParserMap::iterator it = _mapParser.begin(); it != _mapParser.end(); ++ it)
            {
                it->second->RemoveResult(_db, wordid);
            }
        }
        g_objTrigger.OnWordRemove(wordid);
    }
    catch(CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

int CDictObject::GetWordID(const wxString& word, int& wordid)
{
    try
    {
        CDBAccess::TQuery query = _db.PrepareStatement("SELECT ID FROM WordTable WHERE Word = ?");
        query.Bind(1, word);
        CDBAccess::TResult res = query.ExecuteQuery();
        if(!res.IsOk())
            return -1;
        if(res.Eof())
            return -1;
        wordid = res.GetInt(0);
    }
    catch(CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

int CDictObject::GetAllWords()
{
    try
    {
        CDBAccess::TResult res = _db.ExecuteQuery("SELECT ID, Word FROM WordTable");
        if(!res.IsOk())
            return -1;
        while(res.NextRow())
        {
            g_objTrigger.OnWordLoad(res.GetInt(0), res.GetString(1));
        }
    }
    catch(CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;  

}

//////////////////////////////////////////////////////////////



