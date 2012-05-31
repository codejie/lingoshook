/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#include <string>
#include <memory>

//#include "wx/wfstream.h"
//#include "wx/txtstrm.h"
#include "wx/listctrl.h"

#include "ConfigData.h"
#include "HtmlTidyObject.h"
#include "TriggerObject.h"
#include "SpecialDictParser.h"
#include "HtmlDictParser.h"
#include "DictLoadChoiceDialog.h"
#include "DictStoreChoiceDialog.h"
#include "HtmlOptimumObject.h"
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
		    const char* dicttable = "CREATE TABLE DictTable (DictIndex INTEGER PRIMARY KEY AUTOINCREMENT,DictID VARCHAR(64), Title VARCHAR(128),CreateTime TIMESTAMP DEFAULT (DATETIME('now', 'localtime')))";
		    _db.ExecuteUpdate(dicttable);
        }

        if(!_db.TableExists(wxT("DictConfigTable")))
        {
		    const char* dicttable = "CREATE TABLE DictConfigTable (DictIndex INTEGER, LoadParam INTEGER, StoreParam INTEGER)";
		    _db.ExecuteUpdate(dicttable);
        }
        
        if(!_db.TableExists(wxT("SrcDataTable")))
        {
		    const char* datatable = "CREATE TABLE SrcDataTable (SrcID INTEGER PRIMARY KEY AUTOINCREMENT, CheckinTime TIMESTAMP DEFAULT (DATETIME('now', 'localtime')), HTML TEXT)";
    		_db.ExecuteUpdate(datatable);
        }        
        
        if(!_db.TableExists(wxT("WordTable")))
        {
		    const char* wordtable = "CREATE TABLE WordTable (WordID INTEGER PRIMARY KEY AUTOINCREMENT, SrcID INTEGER, Word VARCHAR(32) UNIQUE,Counter INTEGER, UpdateTime TIMESTAMP DEFAULT (DATETIME('now', 'localtime')))";
    		_db.ExecuteUpdate(wordtable);
        }
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

    //TinyHtmlParser::CDocumentObject doc;
    
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

	if(_config.m_iHtmlOptimum == 1)
	{
		if(CHtmlOptimumObject::Clear(prochtml, prochtml, _config.m_mapHtmlOptimumKey) != 0)
			return -1;
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

    const TinyHtmlParser::CElementObject* pdiv = doc.FindFirstElement(L"DIV");
    const TinyHtmlParser::CAttributeObject* pa = NULL;


    while(pdiv != NULL)
    {
        pa = pdiv->FindAttribute(L"id");
        if(pa != NULL && pa->value == L"\"lingoes_dictarea\"")
        {
            pdiv = pdiv->sibling;
            if(pdiv == NULL)
                break;
            pa = pdiv->FindAttribute(L"dictid");
            if(pa == NULL)
                break;

            std::wstring dictid = pa->value;
            dictid = dictid.substr(1, dictid.size() - 2);

            if(_config.m_iSkipDict != 1)
            {
                if(ParserSpecialDict(html, dictid, doc, pdiv, result) != 0)
                    return -1;
            }
            if(_config.m_iSkipHtml != 1)
            {
                if(ParserHtmlDict(html, dictid, doc, pdiv, result) != 0)
                    return -1;
            }
        }
        pdiv = doc.FindNextElement();
    }

//It will be faster by the below way

    //while(pdiv != NULL)
    //{
    //    pa = pdiv->FindAttribute(L"id");
    //    if(pa != NULL && pa->value == L"\"lingoes_dictarea\"")
    //        break;
    //    pdiv = doc.FindNextElement();
    //}

    //while(pdiv != NULL)
    //{
    //    pdiv = pdiv->sibling;
    //    if(pdiv == NULL)
    //        break;
    //    pa = pdiv->FindAttribute(L"dictid");
    //    if(pa == NULL)
    //        break;

    //    std::wstring dictid = pa->value;
    //    dictid = dictid.substr(1, dictid.size() - 2);

    //    if(_config.m_iSkipDict != 1)
    //    {
    //        if(ParserSpecialDict(html, dictid, doc, pdiv, result) != 0)
    //            return -1;
    //    }
    //    if(_config.m_iSkipHtml != 1)
    //    {
    //        if(ParserHtmlDict(html, dictid, doc, pdiv, result) != 0)
    //            return -1;
    //    }

    //    pdiv = pdiv->sibling;
    //    if(pdiv == NULL)
    //        break;
    //    pa = pdiv->FindAttribute(L"id");
    //    if(pa == NULL || pa->value != L"\"lingoes_dictarea\"")
    //        break;
    //}

    if(result.size() == 0)
        return -1;

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
    int count = 0;

    if(CheckWord(_strCacheWord, wordid, count) != 0)
        return -1;

    if(wordid != -1)
        return 0;

    int srcid = -1;
    
    if(SaveSrcData(html, srcid) != 0)
        return -1;
    if(SaveWord(srcid, _strCacheWord, wordid) != 0)
        return -1;

    _strCacheWord = wxEmptyString;

    return 0;
}

int CDictObject::ParserSpecialDict(const std::wstring& html, const std::wstring& dictid, const TinyHtmlParser::CDocumentObject& doc, const TinyHtmlParser::CElementObject* dict, TResultMap& result)
{
    return _objSpecialDictParser->ParserHTML(_db, html, dictid, doc, dict, result);
}

int CDictObject::ParserHtmlDict(const std::wstring &html, const std::wstring& dictid, const TinyHtmlParser::CDocumentObject &doc, const TinyHtmlParser::CElementObject *dict, TResultMap &result)
{
    return _objHtmlDictParser->ParserHTML(_config, _db, html, dictid, doc, dict, result);
}

int CDictObject::CheckWord(const std::wstring &word, int &wordid, int &count)
{
    try
    {
        CDBAccess::TQuery query = _db.PrepareStatement("SELECT WordID, Counter FROM WordTable WHERE Word = ?");
        query.Bind(1, word);
        CDBAccess::TResult res = query.ExecuteQuery();
        if(!res.IsOk())
            throw CDBAccess::TException(255, wxT("SELECT WordID of WordTable FAILED."));
        if(res.Eof())
        {
            wordid = -1;
            count = 0;
        }
        else
        {
            wordid = res.GetInt(0);
            count = res.GetInt(1);
        }
    }
    catch(CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

int CDictObject::FilterResult(TResultMap& result)
{
    int wordid = -1, count = 0;
    TResultMap::iterator it = result.begin();
    while(it != result.end())
    {
        if(CheckWord(it->first, wordid, count) != 0)
            return -1;
        if(wordid == -1)
        {
            ++ it;
        }
        else
        {
            UpdateWordData(wordid, count + 1);

            g_objTrigger.OnWordUpdate(wordid, it->first);

            result.erase(it ++);
            continue;
        }
    }
    return 0;
}

int CDictObject::SaveSrcData(const std::wstring& html, int& srcid)
{
    try
    {
        CDBAccess::TQuery query = _db.PrepareStatement("INSERT INTO SrcDataTable (HTML) VALUES (?)");
        query.Bind(1, html);

        if(query.ExecuteUpdate() == 0)
            return -1;
        
        srcid = _db.GetLastRowId().ToLong();
    }
    catch(CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

int CDictObject::RemoveSrcData(int srcid)
{
    try
    {
        CDBAccess::TQuery query = _db.PrepareStatement("SELECT COUNT(*) FROM WordTable WHERE SrcID = ?");
        query.Bind(1, srcid);
        
        CDBAccess::TResult res = query.ExecuteQuery();
        if(!res.IsOk())
            return -1;
        if(res.Eof())
            return -1;
        int count = res.GetInt(0);
        if(count == 0)
        {
            query.Reset();
            query = _db.PrepareStatement("DELETE FROM SrcDataTable WHERE SrcID = ?");
            query.Bind(1, srcid);

            if(query.ExecuteUpdate() == 0)
                return -1;
        }
    }
    catch(CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

int CDictObject::SaveWord(int srcid, const std::wstring& word, int& wordid)
{
    try
    {
	    CDBAccess::TQuery query = _db.PrepareStatement("INSERT INTO WordTable (SrcID, Word, Counter) VALUES(?, ?, 1)");
        query.Bind(1, srcid);
        query.Bind(2, word);
	    if(query.ExecuteUpdate() == 0)
            return -1;

	    wordid = _db.GetLastRowId().ToLong();

        g_objTrigger.OnWordSave(wordid, word);
    }
    catch(CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

int CDictObject::RemoveWord(const wxString& word)
{
    int wordid = -1, srcid = -1;
    if(GetWordID(word.c_str(), wordid, srcid) != 0)
        return -1;
    return RemoveWord(wordid, srcid);
}

int CDictObject::SaveResult(int srcid, const TResultMap &result)
{
    try
    {
        for(TResultMap::const_iterator it = result.begin(); it != result.end(); ++ it)
        {
            int wordid = -1;
            if(SaveWord(srcid, it->first, wordid) != 0)
                return -1;
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

int CDictObject::SaveResult(const std::wstring& html, TResultMap& result)
{
    if(FilterResult(result) != 0)
        return -1;
	if(result.size() == 0)
		return 0;
    int srcid = -1;
    if(SaveSrcData(html, srcid) != 0)
        return -1;
    if(SaveResult(srcid, result) != 0)
    {
        RemoveSrcData(srcid);
        return -1;
    }
    return 0;
}

int CDictObject::UpdateWordData(int wordid, int counter)
{
    CDBAccess::TQuery query = _db.PrepareStatement("UPDATE WordTable SET Counter = ?, UpdateTime = DATETIME('NOW', 'LOCALTIME') WHERE WordID = ?");
    query.Bind(1, counter);
    query.Bind(2, wordid);
    query.ExecuteUpdate();

    return 0;
}

int CDictObject::GetAllWords()
{
    try
    {
        CDBAccess::TResult res = _db.ExecuteQuery("SELECT WordID, Word FROM WordTable");
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
        CDBAccess::TQuery query = _db.PrepareStatement("SELECT WordTable.SrcID, WordTable.Word, WordTable.Counter, WordTable.UpdateTime, SrcDataTable.CheckinTime, SrcDataTable.HTML FROM WordTable, SrcDataTable WHERE WordTable.WordID = ? and WordTable.SrcID = SrcDataTable.SrcID");
        query.Bind(1, wordid);
        CDBAccess::TResult res = query.ExecuteQuery();
        if(!res.IsOk())
	        return -1;
        if(!res.Eof())
        {
            data.m_iWordID = wordid;
            data.m_iSrcID = res.GetInt(0);
            data.m_strWord = res.GetString(1);
            data.m_iCounter = res.GetInt(2);
            data.m_dtUpdate = res.GetDateTime(3);
            data.m_dtCheckin = res.GetDateTime(4);
            data.m_strHTML = res.GetString(5);
        }
        else
        {
            return -1;
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
    if(_objSpecialDictParser->GetResult(_db, data.m_iWordID, dictresult) != 0)
        return -1;

    for(SpecialDictParser::TDictResultMap::const_iterator it = dictresult.begin(); it != dictresult.end(); ++ it)
    {
        const SpecialDictParser::CDictParser* parser = _objSpecialDictParser->GetParser(it->first);
        if(parser != NULL)
        {
            g_objTrigger.OnResultSpecialDictGet(data.m_iWordID, parser, it->second);
        }
    }

    return 0;
}

int CDictObject::GetHtmlDictResult(const TWordData& data)
{
    if(_config.m_iLoadHtmlDict == 0)
    {
        g_objTrigger.OnResultHtmlDictGet(data.m_iWordID, data.m_strHTML);
    }
    else
    {
        HtmlDictParser::TDictResultMap htmlresult;
        if(_objHtmlDictParser->GetResult(_db, data.m_iWordID, htmlresult) != 0)
            return -1;

        std::wstring html;
        if(_objHtmlDictParser->GenHtmlResult(htmlresult, data.m_strHTML, html) != 0)
            return -1;
        if(!html.empty())
            g_objTrigger.OnResultHtmlDictGet(data.m_iWordID, html.c_str());
        else if(_config.m_iLoadHtmlDict == 2)
            g_objTrigger.OnResultHtmlDictGet(data.m_iWordID, data.m_strHTML);
    }
    return 0;
}

int CDictObject::RemoveWord(int wordid)
{
    int srcid = -1;
    if(GetWordSrcID(wordid, srcid) != 0)
        return -1;
    return RemoveWord(wordid, srcid);
}

int CDictObject::RemoveWord(int wordid, int srcid)
{
    try
    {
        CDBAccess::TQuery query = _db.PrepareStatement("DELETE FROM WordTable WHERE WordID = ?");
        query.Bind(1, wordid);
        if(query.ExecuteUpdate() != 0)
        {
            _objSpecialDictParser->RemoveResult(_db, wordid);
            _objHtmlDictParser->RemoveResult(_db, wordid);
        }

        if(RemoveSrcData(srcid) != 0)
			return -1;

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
        CDBAccess::TQuery query = _db.PrepareStatement("SELECT WordID FROM WordTable WHERE Word = ?");
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

int CDictObject::GetWordSrcID(int wordid, int& srcid)
{
    try
    {
        CDBAccess::TQuery query = _db.PrepareStatement("SELECT SrcID FROM WordTable WHERE WordID = ?");
        query.Bind(1, wordid);
        CDBAccess::TResult res = query.ExecuteQuery();
        if(!res.IsOk())
            return -1;
        if(res.Eof())
            return -1;
        srcid = res.GetInt(0);
    }
    catch(CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

int CDictObject::GetWordID(const std::wstring& word, int& wordid, int& srcid)
{
    try
    {
        CDBAccess::TQuery query = _db.PrepareStatement("SELECT WordID, SrcID FROM WordTable WHERE Word = ?");
        query.Bind(1, word);
        CDBAccess::TResult res = query.ExecuteQuery();
        if(!res.IsOk())
            return -1;
        if(res.Eof())
            return -1;
        wordid = res.GetInt(0);
        srcid = res.GetInt(1);
    }
    catch(CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

////////////////////////////////////////////
void CDictObject::ShowHtmlDictLoadInfo(CHtmlDictLoadChoiceDialog &dlg) const
{
    _objHtmlDictParser->ShowDictLoadInfo(_config.m_iLoadHtmlDict, dlg);
}

int CDictObject::GetHtmlDictLoadInfo(const CHtmlDictLoadChoiceDialog& dlg)
{
    return _objHtmlDictParser->GetDictLoadInfo(_db, _config.m_iLoadHtmlDict, dlg);
}

void CDictObject::ShowHtmlDictStoreInfo(CHtmlDictStoreChoiceDialog &dlg) const
{
    _objHtmlDictParser->ShowDictStoreInfo(dlg);
}

int CDictObject::GetHtmlDictStoreInfo(const CHtmlDictStoreChoiceDialog& dlg)
{
    return _objHtmlDictParser->GetDictStoreInfo(_db, dlg);
}

int CDictObject::ResetDictStoreInfo(CHtmlDictStoreChoiceDialog& dlg)
{
    return _objHtmlDictParser->ResetDictStoreInfo(_db, dlg);
}

void CDictObject::ShowDictStoreInfoItemContextMenu(const CHtmlDictStoreChoiceDialog &dlg, long item, int menubase, wxMenu* submenu) const
{
    _objHtmlDictParser->ShowDictStoreInfoItemContextMenu(dlg, item, menubase, submenu);
}

void CDictObject::RefreshDictStoreInfo(CHtmlDictStoreChoiceDialog &dlg, long item, int type) const
{
    _objHtmlDictParser->RefreshDictStoreInfo(dlg, item, type);
}

int CDictObject::UpdateDictStoreInfoDefType(CHtmlDictStoreChoiceDialog &dlg, long item)
{
    return _objHtmlDictParser->UpdateDictStoreInfoDefType(_db, dlg, item);
}

