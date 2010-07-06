/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#include <string>
#include <memory>

#include "wx/wfstream.h"
#include "wx/txtstrm.h"

#include "ConfigData.h"
#include "HtmlTidyObject.h"
#include "TriggerObject.h"
#include "SpecialDictParser.h"
#include "HtmlDictParser.h"
#include "DictChoiceDialog.h"
#include "DictObject.h"

////////////////////////////////////////
CDictObject::CDictObject(CDBAccess &db, CConfigData& config)
: _db(db.Database())
, _config(config)
, _strCacheWord(wxEmptyString)
, _objSpecialDictParser(NULL)
, _objHtmlDictParser(NULL)
{
}

CDictObject::~CDictObject()
{
}

int CDictObject::Init()
{
    _objSpecialDictParser.reset(new CSpecialDictParser);
    _objHtmlDictParser.reset(new CHtmlDictParser);

    //word table
    try
    {
        if(!_db.TableExists(wxT("DictTable")))
        {
		    const char* dicttable = "CREATE TABLE DictTable (ID INTEGER PRIMARY KEY AUTOINCREMENT,DictID VARCHAR(64), Title VARCHAR(128),CreateTime TIMESTAMP DEFAULT (DATETIME('now', 'localtime')))";
		    _db.ExecuteUpdate(dicttable);
        }

        if(!_db.TableExists(wxT("DictConfigTable")))
        {
		    const char* dicttable = "CREATE TABLE DictConfigTable (DictIndex INTEGER, LoadParam INTEGER, StoreParam INTEGER)";
		    _db.ExecuteUpdate(dicttable);
        }
 
		const char* wordtable = "CREATE TABLE IF NOT EXISTS WordTable (ID INTEGER PRIMARY KEY AUTOINCREMENT,Word VARCHAR(32) UNIQUE,Counter INTEGER, CheckinTime TIMESTAMP DEFAULT (DATETIME('now', 'localtime')), UpdateTime TIMESTAMP DEFAULT (DATETIME('now', 'localtime')),HTML TEXT)";
		_db.ExecuteUpdate(wordtable);

    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }

    if(_objSpecialDictParser->Init(_db) != 0)
        return -1;
    if(_objHtmlDictParser->Init(_db) != 0)
        return -1;

    return 0;
}

void CDictObject::CacheWord(const wxString& word)
{
    _strCacheWord = word.c_str();
}

int CDictObject::HTMLProc(const wxString& str)
{
    std::wstring html(str.begin(), str.end());

    TinyHtmlParser::CDocumentObject doc;
    
    if(_config.m_iUseTidy == 1)
    {
        if(CHtmlTidyObject::Tidy(html, html) != 0)
            return -1;
    }

    std::wstring prochtml;

    for(std::wstring::const_iterator it = html.begin(); it != html.end(); ++ it)
    {
        if((*it) == L'\r' || (*it) == L'\n')
        {
            continue;
        }
        prochtml += (*it);
    }

    if(_config.m_iSkipDict != 1 || _config.m_iSkipHtml != 1)
    {
        if(ParserHTML(prochtml) != 0)
        {
            if(_config.m_iSkipError == 1)
            {
                return ForceSaveHTML(prochtml);
            }
            else
            {
                return -1;
            }
        }
    }
    else
    {
        return ForceSaveHTML(prochtml);
    }
    return 0;
}

int CDictObject::ParserHTML(const std::wstring& html)
{
    TinyHtmlParser::CDocumentObject doc;

    try
    {
        if(doc.Load(html, true) != 0)
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

    TResultMap result;

    const TinyHtmlParser::CElementObject* pe = doc.FindFirstElement(L"DIV");
    while(pe != NULL)
    {
        const TinyHtmlParser::CAttributeObject* pa = pe->FindAttribute(L"dictid");
        if(pa != NULL)
        {
            std::wstring dictid = pa->value;//, pa->value.size());//, wxConvISO8859_1);
            dictid = dictid.substr(1, dictid.size() - 2);

            if(_config.m_iSkipDict != 1)
            {
                if(ParserSpecialDict(html, dictid, doc, pe, result) != 0)
                    return -1;
            }
            if(_config.m_iSkipHtml != 1)
            {
                if(ParserHtmlDict(html, dictid, doc, pe, result) != 0)
                    return -1;
            }
        }
        pe = doc.FindNextElement();
    }

    if(SaveResult(html, result) == 0)
    {
        //g_objTrigger.OnResultSave(result);
        return 0;
    }

    return -1;    
}

int CDictObject::ForceSaveHTML(const std::wstring& html)
{
    if(_strCacheWord.empty())
        return -1;
    int wordid = -1;

    bool isexist = false;
    int ret = SaveWord(html, _strCacheWord, wordid, isexist);

    _strCacheWord = wxEmptyString;

    return ret;
}

int CDictObject::ParserSpecialDict(const std::wstring& html, const std::wstring& dictid, const TinyHtmlParser::CDocumentObject& doc, const TinyHtmlParser::CElementObject* dict, TResultMap& result)
{
    return _objSpecialDictParser->ParserHTML(_db, html, dictid, doc, dict, result);
}

int CDictObject::ParserHtmlDict(const std::wstring &html, const std::wstring& dictid, const TinyHtmlParser::CDocumentObject &doc, const TinyHtmlParser::CElementObject *dict, TResultMap &result)
{
    return _objHtmlDictParser->ParserHTML(_db, html, dictid, doc, dict, result);
}

int CDictObject::SaveResult(const std::wstring& html, const TResultMap& result)
{
    try
    {
        int wordid = -1;
        bool isexist = false; 
        for(TResultMap::const_iterator it = result.begin(); it != result.end(); ++ it)
        {
            SaveWord(html, it->first, wordid, isexist);
            if(isexist)
                continue;
            if(_objSpecialDictParser->SaveResult(_db, wordid, it->second.m_resultDict) != 0)
                return -1;
            if(_objHtmlDictParser->SaveResult(_db, wordid, it->second.m_resultHtml) != 0)
                return -1;
        }
    }
    catch(CDBAccess::TException& e)
    {
        return -1;
    }

    return 0;
}

int CDictObject::SaveWord(const std::wstring& html, const std::wstring& word, int& wordid, bool& isexist)
{
    CDBAccess::TQuery query = _db.PrepareStatement("SELECT ID, Counter FROM WordTable WHERE Word = ?");
	query.Bind(1, word);
    CDBAccess::TResult res = query.ExecuteQuery();
	if(!res.IsOk())
        throw CDBAccess::TException(255, _("SELECT ID of WordTable FAILED."));

    isexist = !res.Eof();

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
            query.Bind(1, word.c_str());
            query.Bind(2, html.c_str());
		    query.ExecuteUpdate();

		    wordid = _db.GetLastRowId().ToLong();
        }
        else
        {
		    CDBAccess::TQuery query = _db.PrepareStatement("INSERT INTO WordTable (Word, Counter) VALUES(?, 1)");
            query.Bind(1, word.c_str());
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

int CDictObject::GetResult(int wordid)
{
    try
    {
        TWordData data;
        if(GetWordData(wordid, data) != 0)
            return -1;

        g_objTrigger.OnWordFound(wordid, data.m_strWord);
    
        GetSpecialDictResult(data);
        GetHtmlDictResult(data);

        g_objTrigger.OnWordResultGetOver(wordid, data);
    }
    catch(CDBAccess::TException& e)
    {
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
                data.m_strWord = res.GetString(0).c_str();
                data.m_iCounter = res.GetInt(1);
                data.m_dtCheckin = res.GetDateTime(2);
                data.m_dtUpdate = res.GetDateTime(3);
                data.m_strHTML = res.GetString(4).c_str();

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
                data.m_strWord = res.GetString(0).c_str();
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

int CDictObject::GetSpecialDictResult(const TWordData& data)
{
    SpecialDictParser::TDictResultMap dictresult;
    if(_objSpecialDictParser->GetResult(_db, data.m_iID, dictresult) != 0)
        return -1;

    for(SpecialDictParser::TDictResultMap::const_iterator it = dictresult.begin(); it != dictresult.end(); ++ it)
    {
        const SpecialDictParser::CDictParser* parser = _objSpecialDictParser->GetParser(it->first);
        if(parser != NULL)
        {
            g_objTrigger.OnResultSpecialDictGet(data.m_iID, parser, it->second);
        }
    }

    return 0;
}

int CDictObject::GetHtmlDictResult(const TWordData& data)
{
    if(_config.m_iLoadHtmlDict == 0)
    {
        g_objTrigger.OnResultHtmlDictGet(data.m_iID, data.m_strHTML);
    }
    else
    {
        HtmlDictParser::TDictResultMap htmlresult;
        if(_objHtmlDictParser->GetResult(_db, data.m_iID, htmlresult) != 0)
            return -1;

        std::wstring html;
        if(_objHtmlDictParser->GenHtmlResult(htmlresult, data.m_strHTML, html) != 0)
            return -1;
        if(!html.empty())
            g_objTrigger.OnResultHtmlDictGet(data.m_iID, html.c_str());
        else if(_config.m_iLoadHtmlDict == 2)
            g_objTrigger.OnResultHtmlDictGet(data.m_iID, data.m_strHTML);
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
            _objSpecialDictParser->RemoveResult(_db, wordid);
            _objHtmlDictParser->RemoveResult(_db, wordid);
        }
        g_objTrigger.OnWordRemove(wordid);
    }
    catch(CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}


int CDictObject::GetWordID(const std::wstring& word, int& wordid)
{
    try
    {
        CDBAccess::TQuery query = _db.PrepareStatement("SELECT ID FROM WordTable WHERE Word = ?");
        query.Bind(1, word.c_str());
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

////////////////////////////////////////////
void CDictObject::ShowHtmlDictInfo(CHtmlDictChoiceDialog &dlg) const
{
    _objHtmlDictParser->ShowDictInfo(_config.m_iLoadHtmlDict, dlg);
}

int CDictObject::GetHtmlDictInfo(const CHtmlDictChoiceDialog& dlg)
{
    return _objHtmlDictParser->GetDictInfo(_db, _config.m_iLoadHtmlDict, dlg);
}

