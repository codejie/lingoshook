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
#include "DictObject.h"


//CDictResult::CDictResult(const CParserResult* result)
//: _result(NULL)
//{
//    if(result != NULL)
//    {
//        _result.reset(result);
//    }
//}
//
//CDictResult::~CDictResult()
//{
//}
//
//const CParserResult* CDictResult::Result() const
//{
//    return _result.get();;
//}
//
//void CDictResult::Attach(const CParserResult* result)
//{
//    _result.reset(result);
//}
//
//CDictResult::CDictResult(const CDictResult& right)
//{
//    if(this != &right)
//    {
//        _result.reset((const_cast<CDictResult&>(right))._result.release());
//    }
//}

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

        //if(AddKnownParser() != 0)
        //    return -1;

        //if(LoadParser() != 0)
        //    return -1;
 
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
//
//int CDictObject::AddKnownParser()
//{
//    if(RegisterParser(SpecialDictParser::VICON::CECParser::ID, SpecialDictParser::VICON::CECParser::TITLE) != 0)
//        return -1;
//    if(RegisterParser(SpecialDictParser::LANGDAO::CECParser::ID, SpecialDictParser::LANGDAO::CECParser::TITLE) != 0)
//        return -1;
//    if(RegisterParser(SpecialDictParser::FUNDSET::CDCParser::ID, SpecialDictParser::FUNDSET::CDCParser::TITLE) != 0)
//        return -1;
//    return 0;
//}
//
//int CDictObject::RegisterParser(const wxString& id, const wxString& title)
//{
//    try
//    {
//        CDBAccess::TQuery query = _db.PrepareStatement("SELECT COUNT(*) FROM DictTable WHERE DictID = ?");
//        query.Bind(1, id);
//        CDBAccess::TResult res = query.ExecuteQuery();
//        if(res.GetInt(0) != 0)
//            return 0;
//
//        query.Reset();
//        query = _db.PrepareStatement("INSERT INTO DictTable (DictID, Title) VALUES(?, ?)");
//        query.Bind(1, id);
//        query.Bind(2, title);
//        if(query.ExecuteUpdate() == 0)
//            return -1;
//
//        int index = _db.GetLastRowId().ToLong();
//
//        query.Reset();
//        query = _db.PrepareStatement("INSERT INTO DictConfigTable (DictIndex, LoadParam, StoreParam) VALUES(?, ?, ?)");
//        query.Bind(1, index);
//        query.Bind(2, 1);
//        query.Bind(3, 1);
//        if(query.ExecuteUpdate() == 0)
//            return -1;
//    }
//    catch(const CDBAccess::TException& e)
//    {
//        return -1;
//    }
//    return 0;
//}
//
//int CDictObject::LoadParser()
//{
//    try
//    {
//        CDBAccess::TResult res = _db.ExecuteQuery("SELECT ID, DictID, Title, CreateTime FROM DictTable");
//        if(!res.IsOk())
//            return -1;
//        while(res.NextRow())
//        {
//            wxString dictid = res.GetString(1);
//            std::auto_ptr<SpecialDictParser::CDictParser> p(NULL);
//
//            if(dictid == SpecialDictParser::VICON::CECParser::ID)
//            {
//                p.reset(new SpecialDictParser::VICON::CECParser(res.GetInt(0), res.GetString(1), res.GetString(2), res.GetTimestamp(3)));
//            }
//            else if(dictid == SpecialDictParser::LANGDAO::CECParser::ID)
//            {
//                p.reset(new SpecialDictParser::LANGDAO::CECParser(res.GetInt(0), res.GetString(1), res.GetString(2), res.GetTimestamp(3)));
//            }
//            else if(dictid == SpecialDictParser::FUNDSET::CDCParser::ID)
//            {
//                p.reset(new SpecialDictParser::FUNDSET::CDCParser(res.GetInt(0), res.GetString(1), res.GetString(2), res.GetTimestamp(3)));
//            }
//            else
//            {
//                g_objTrigger.OnParserUnknown(res.GetString(1), res.GetString(2));
//            }
//            if(p.get() == NULL)
//                continue;
//
//            if(p->Init(_db) == 0)
//            {
//                _mapParser.insert(std::make_pair(dictid, p.release()));
//                g_objTrigger.OnParserLoad(res.GetInt(0), dictid, res.GetString(2));
//            }
//            else
//            {
//                g_objTrigger.OnParserInitFail(res.GetString(1), res.GetString(2));
//            }
//        }
//    }
//    catch(const CDBAccess::TException& e)
//    {
//        return -1;
//    }
//    return 0;
//}

//int CDictObject::AddParser(const wxString& id)
//{
//    if(GetParser(id) == NULL)
//        return -1;
//    return 0;
//}

//SpecialDictParser::CDictParser* CDictObject::GetParser(const wxString &id)
//{
//    TParserMap::iterator it = _mapParser.find(id);
//    if(it != _mapParser.end())
//        return it->second;
//
//    return NULL;
//}
//
//void CDictObject::FreeParser()
//{
//    TParserMap::iterator it = _mapParser.begin();
//    while(it != _mapParser.end())
//    {
//        delete it->second;
//        _mapParser.erase(it ++ );
//    }
//}

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
    HtmlDictParser::TDictResultMap htmlresult;
    if(_objHtmlDictParser->GetResult(_db, data.m_iID, htmlresult) != 0)
        return -1;

    std::wstring html;
    if(_objHtmlDictParser->GenHtmlResult(htmlresult, data.m_strHTML, html) != 0)
        return -1;

    g_objTrigger.OnResultHtmlDictGet(data.m_iID, html.c_str());
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
//
//
//int CDictObject::HTMLProc(const wxString &str, int mode)
//{
//    if(mode == 0)
//    {//normal
//        return HTMLProc(str, false);
//    }
//    else if(mode == 1)
//    {//ignore error
//        if(HTMLProc(str, false) != 0)
//        {
//            return ForceSaveHTML(str);
//        }
//    }
//    else if(mode == 2)
//    {//Skip dict
//        if(HTMLProc(str, true) != 0)
//        {
//            return ForceSaveHTML(str);
//        }
//    }
//    else if(mode == 3)
//    {//Skip HTML
//        return ForceSaveHTML(str);
//    }
//    else
//    {
//        return -1;
//        //return ForceSaveHTML(str);
//    }
//    return 0;
//}
//
//int CDictObject::HTMLProc(const wxString &str, bool skipdict)
//{
//    std::wstring html(str.begin(), str.end());
//
//    //find dict
//    TinyHtmlParser::CDocumentObject doc;
//
//    if(_config.m_iUseTidy == 1)
//    {
//        if(CHtmlTidyObject::Tidy(html, html) != 0)
//            return -1;
//    }
//
//    try
//    {
//        if(doc.Load(html, true) != 0)
//        {
//            g_objTrigger.OnHTMLParserFail(html);
//            return -1;
//        }
//    }
//    catch(TinyHtmlParser::CExceptionObject& e)
//    {
//        g_objTrigger.OnHTMLParserException(html, e);
//        return -1;
//    }
//
//    SpecialDictParser::TWordResultMap result;
//    HtmlDictParser::TDictResultVector htmlresult;
//
//    const TinyHtmlParser::CElementObject* pe = doc.FindFirstElement(L"DIV");
//    while(pe != NULL)
//    {
//        const TinyHtmlParser::CAttributeObject* pa = pe->FindAttribute(L"dictid");
//        if(pa != NULL)
//        {
//            wxString dictid = pa->value.c_str();//, pa->value.size());//, wxConvISO8859_1);
//            dictid = dictid.substr(1, dictid.size() - 2);
//
//            if(skipdict == false)
//            {//Dict
//                SpecialDictParser::CDictParser* parser = GetParser(dictid);
//                if(parser != NULL)
//                {
//                    if(parser->ParserHTML(html, doc, pe, result) != 0)
//                    {
//                       
//                    }
//                }
//                else
//                {
//                }
//            }
//            //HTML dict
//            if(_config.m_iHTMLSave == 1)
//                _objHtmlDictParser.ParserHTML(_db, dictid.c_str(),html, doc, pe, htmlresult);
//        }
//        pe = doc.FindNextElement();
//    }
//
//    //str = html.c_str();
//
//    if(SaveResult(html.c_str(), result, htmlresult) == 0)
//    {
//        g_objTrigger.OnResultSave(result);
//        return 0;
//    }
//
//    return -1;
//}
//
//int CDictObject::SaveWord(const wxString& word, const wxString& html, int& wordid, bool& isexist)
//{
//    //wxFileOutputStream output(wxT("C:\\T1.html"));
//    //wxTextOutputStream ofs(output);
//    //ofs.WriteString(html);
//
//    CDBAccess::TQuery query = _db.PrepareStatement("SELECT ID, Counter FROM WordTable WHERE Word = ?");
//	query.Bind(1, word);
//    CDBAccess::TResult res = query.ExecuteQuery();
//	if(!res.IsOk())
//        throw CDBAccess::TException(255, _("SELECT ID of WordTable FAILED."));
//
//    isexist = !res.Eof();
//
//	if(!res.Eof())
//	{
//		wordid = res.GetInt(0);
//        int counter = res.GetInt(1);
//
//        UpdateWordData(wordid, counter + 1);
//
//        g_objTrigger.OnWordUpdate(wordid, word);
//	}
//	else
//	{
//        if(_config.m_iHTMLSave == 1)
//        {
//		    CDBAccess::TQuery query = _db.PrepareStatement("INSERT INTO WordTable (Word, Counter, HTML) VALUES(?, 1, ?)");
//		    query.Bind(1, word);
//		    query.Bind(2, html);
//		    query.ExecuteUpdate();
//
//		    wordid = _db.GetLastRowId().ToLong();
//        }
//        else
//        {
//		    CDBAccess::TQuery query = _db.PrepareStatement("INSERT INTO WordTable (Word, Counter) VALUES(?, 1)");
//		    query.Bind(1, word);
//		    query.ExecuteUpdate();
//
//		    wordid = _db.GetLastRowId().ToLong();
//        }
//
//        g_objTrigger.OnWordSave(wordid, word);
// 	}
//
//	return 0;    
//}
//
//int CDictObject::UpdateWordData(int wordid, int counter)
//{
//    CDBAccess::TQuery query = _db.PrepareStatement("UPDATE WordTable SET Counter = ?, UpdateTime = DATETIME('NOW', 'LOCALTIME') WHERE ID = ?");
//    query.Bind(1, counter);
//    query.Bind(2, wordid);
//    query.ExecuteUpdate();
//
//    return 0;
//}
//
//int CDictObject::SaveResult(const wxString& html, const SpecialDictParser::TWordResultMap &result, const HtmlDictParser::TDictResultVector& htmlresult)
//{
//    try
//    {
//        int wordid = -1;
//        bool isexist = false; 
//        for(SpecialDictParser::TWordResultMap::const_iterator it = result.begin(); it != result.end(); ++ it)
//        {
//            SaveWord(it->first, html, wordid, isexist);
//            if(isexist)
//                continue;
//
//            for(SpecialDictParser::TDictResultMap::const_iterator i = it->second.begin(); i != it->second.end(); ++ i)
//            {
//                TParserMap::iterator p = _mapParser.find(i->first);
//                if(p != _mapParser.end())
//                {
//                    p->second->SaveResult(_db, wordid, i->second);
//
//                    g_objTrigger.OnResultSave(wordid, p->second, i->second);
//                }
//                else
//                {
//                }
//            }
//            _objHtmlDictParser.SaveResult(_db, wordid, htmlresult);
//        }
//    }
//    catch(CDBAccess::TException& e)
//    {
//        return -1;
//    }
//
//    return 0;
//}
//
//
//
//int CDictObject::GetWordData(int wordid, TWordData& data)
//{
//    try
//    {
//        if(_config.m_iHTMLLoad == 1)
//        {
//	        CDBAccess::TQuery query = _db.PrepareStatement("SELECT Word, Counter, CheckinTime, UpdateTime, HTML FROM WordTable WHERE ID = ?");
//	        query.Bind(1, wordid);
//	        CDBAccess::TResult res = query.ExecuteQuery();
//	        if(!res.IsOk())
//		        return -1;
//	        if(!res.Eof())
//	        {
//                data.m_iID = wordid;
//                data.m_strWord = res.GetString(0);
//                data.m_iCounter = res.GetInt(1);
//                data.m_dtCheckin = res.GetDateTime(2);
//                data.m_dtUpdate = res.GetDateTime(3);
//                data.m_strHTML = res.GetString(4);
//
////                g_objTrigger.OnWordDataGet(data);
//            }
//            else
//            {
//                return -1;
//            }
//        }
//        else
//        {
//	        CDBAccess::TQuery query = _db.PrepareStatement("SELECT Word, Counter, CheckinTime, UpdateTime FROM WordTable WHERE ID = ?");
//	        query.Bind(1, wordid);
//	        CDBAccess::TResult res = query.ExecuteQuery();
//	        if(!res.IsOk())
//		        return -1;
//	        if(!res.Eof())
//	        {
//                data.m_iID = wordid;
//                data.m_strWord = res.GetString(0);
//                data.m_iCounter = res.GetInt(1);
//                data.m_dtCheckin = res.GetDateTime(2);
//                data.m_dtUpdate = res.GetDateTime(3);
//                data.m_strHTML = _("<HTML></HTML>");
//
////                g_objTrigger.OnWordDataGet(data);
//            }
//            else
//            {
//                return -1;
//            }
//        }
//    }
//    catch(CDBAccess::TException& e)
//    {
//        return -1;
//    }
//    return 0;
//}
//
//int CDictObject::GetResult(int wordid, TWordData &data, SpecialDictParser::TDictResultMap &result, HtmlDictParser::TDictResultVector& htmlresult)
//{
//    if(GetWordData(wordid, data) != 0)
//        return -1;
//
//    g_objTrigger.OnWordFound(wordid, data.m_strWord);
//
//    for(TParserMap::iterator it = _mapParser.begin(); it != _mapParser.end(); ++ it)
//    {
//        if(it->second->GetResult(_db, wordid, result) != 0)
//        {
//            return -1;
//        }
//    }
//
//    if(_config.m_iHTMLLoad == 1)
//    {
//        if(_objHtmlDictParser.GetResult(_db, wordid, htmlresult) == 0)
//        {
//            _objHtmlDictParser.GenHtmlResult(htmlresult, data.m_strHTML.c_str(), data.m_strHTML);
//        }
//    }
//
//    g_objTrigger.OnWordResultGetOver(wordid, data);
//    return 0;
//}
//
//int CDictObject::GetResult(int wordid)
//{
//    try
//    {
//        TWordData data;
//        if(GetWordData(wordid, data) != 0)
//            return -1;
//
//        g_objTrigger.OnWordFound(wordid, data.m_strWord);
//
//        for(TParserMap::iterator it = _mapParser.begin(); it != _mapParser.end(); ++ it)
//        {
//            SpecialDictParser::CDictResult result;
//            if(it->second->GetResult(_db, wordid, result) == 0)
//            {
//                g_objTrigger.OnResultGet(wordid, it->second, result);
//            }
//        }
//
//        if(_config.m_iHTMLLoad == 1)
//        {
//            HtmlDictParser::TDictResultVector htmlresult;
//            if(_objHtmlDictParser.GetResult(_db, wordid, htmlresult) == 0)
//            {
////                g_objTrigger.OnHTMLResultGet(wordid, data, htmlresult);
//                _objHtmlDictParser.GenHtmlResult(htmlresult, data.m_strHTML.c_str(), data.m_strHTML);
//            }
//        }
//
//        g_objTrigger.OnWordResultGetOver(wordid, data);
//    }
//    catch(CDBAccess::TException& e)
//    {
//        return -1;
//    }
//    return 0;
//}
//
//int CDictObject::RemoveWord(int wordid)
//{
//    try
//    {
//        CDBAccess::TQuery query = _db.PrepareStatement("DELETE FROM WordTable WHERE ID = ?");
//        query.Bind(1, wordid);
//        if(query.ExecuteUpdate() != 0)
//        {
//            for(TParserMap::iterator it = _mapParser.begin(); it != _mapParser.end(); ++ it)
//            {
//                it->second->RemoveResult(_db, wordid);
//            }
//
//            _objHtmlDictParser.RemoveResult(_db, wordid);
//        }
//        g_objTrigger.OnWordRemove(wordid);
//    }
//    catch(CDBAccess::TException& e)
//    {
//        return -1;
//    }
//    return 0;
//}
//
//int CDictObject::GetWordID(const wxString& word, int& wordid)
//{
//    try
//    {
//        CDBAccess::TQuery query = _db.PrepareStatement("SELECT ID FROM WordTable WHERE Word = ?");
//        query.Bind(1, word);
//        CDBAccess::TResult res = query.ExecuteQuery();
//        if(!res.IsOk())
//            return -1;
//        if(res.Eof())
//            return -1;
//        wordid = res.GetInt(0);
//    }
//    catch(CDBAccess::TException& e)
//    {
//        return -1;
//    }
//    return 0;
//}
//
//int CDictObject::GetAllWords()
//{
//    try
//    {
//        CDBAccess::TResult res = _db.ExecuteQuery("SELECT ID, Word FROM WordTable");
//        if(!res.IsOk())
//            return -1;
//        while(res.NextRow())
//        {
//            g_objTrigger.OnWordLoad(res.GetInt(0), res.GetString(1));
//        }
//    }
//    catch(CDBAccess::TException& e)
//    {
//        return -1;
//    }
//    return 0;  
//
//}

//////////////////////////////////////////////////////////////



