
#include "TriggerObject.h"
#include "ViconDictObject.h"
#include "LangdaoDictObject.h"
#include "FundsetDictObject.h"
#include "SpecialDictParser.h"

namespace SpecialDictParser
{

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

}

///
CSpecialDictParser::~CSpecialDictParser()
{
    FreeParser();
}

int CSpecialDictParser::Init(CDBAccess::TDatabase &db)
{
    if(AddKnownParser(db) != 0)
        return -1;

    if(LoadParser(db) != 0)
        return -1;    

    return 0;
}


int CSpecialDictParser::AddKnownParser(CDBAccess::TDatabase& db)
{
    if(RegisterParser(db, SpecialDictParser::VICON::CECParser::ID, SpecialDictParser::VICON::CECParser::TITLE) != 0)
        return -1;
    if(RegisterParser(db, SpecialDictParser::LANGDAO::CECParser::ID, SpecialDictParser::LANGDAO::CECParser::TITLE) != 0)
        return -1;
    if(RegisterParser(db, SpecialDictParser::FUNDSET::CDCParser::ID, SpecialDictParser::FUNDSET::CDCParser::TITLE) != 0)
        return -1;
    return 0;
}

int CSpecialDictParser::RegisterParser(CDBAccess::TDatabase& db, const wstring& id, const wstring& title)
{
    try
    {
        CDBAccess::TQuery query = db.PrepareStatement("SELECT COUNT(*) FROM DictTable WHERE DictID = ?");
        query.Bind(1, id.c_str());
        CDBAccess::TResult res = query.ExecuteQuery();
        if(res.GetInt(0) != 0)
            return 0;

        query.Reset();
        query = db.PrepareStatement("INSERT INTO DictTable (DictID, Title) VALUES(?, ?)");
        query.Bind(1, id.c_str());
        query.Bind(2, title.c_str());
        if(query.ExecuteUpdate() == 0)
            return -1;

        int index = db.GetLastRowId().ToLong();

        query.Reset();
        query = db.PrepareStatement("INSERT INTO DictConfigTable (DictIndex, LoadParam, StoreParam) VALUES(?, ?, ?)");
        query.Bind(1, index);
        query.Bind(2, 0);
        query.Bind(3, 0);
        if(query.ExecuteUpdate() == 0)
            return -1;
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

int CSpecialDictParser::LoadParser(CDBAccess::TDatabase& db)
{
    try
    {
        CDBAccess::TResult res = db.ExecuteQuery("SELECT ID, DictID, Title, CreateTime FROM DictTable");
        if(!res.IsOk())
            return -1;
        while(res.NextRow())
        {
            wxString dictid = res.GetString(1);
            std::auto_ptr<SpecialDictParser::CDictParser> p(NULL);

            if(dictid == SpecialDictParser::VICON::CECParser::ID)
            {
                p.reset(new SpecialDictParser::VICON::CECParser(res.GetInt(0), res.GetString(1).c_str(), res.GetString(2).c_str(), res.GetTimestamp(3)));
            }
            else if(dictid == SpecialDictParser::LANGDAO::CECParser::ID)
            {
                p.reset(new SpecialDictParser::LANGDAO::CECParser(res.GetInt(0), res.GetString(1).c_str(), res.GetString(2).c_str(), res.GetTimestamp(3)));
            }
            else if(dictid == SpecialDictParser::FUNDSET::CDCParser::ID)
            {
                p.reset(new SpecialDictParser::FUNDSET::CDCParser(res.GetInt(0), res.GetString(1).c_str(), res.GetString(2).c_str(), res.GetTimestamp(3)));
            }
            else
            {
                g_objTrigger.OnParserUnknown(res.GetString(1), res.GetString(2));
            }
            if(p.get() == NULL)
                continue;

            if(p->Init(db) == 0)
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

void CSpecialDictParser::FreeParser()
{
    TParserMap::iterator it = _mapParser.begin();
    while(it != _mapParser.end())
    {
        delete it->second;
        _mapParser.erase(it ++ );
    }
}

const SpecialDictParser::CDictParser* CSpecialDictParser::GetParser(const std::wstring &id) const
{
    TParserMap::const_iterator it = _mapParser.find(id);
    if(it != _mapParser.end())
        return it->second;

    return NULL;
}

int CSpecialDictParser::ParserHTML(CDBAccess::TDatabase &db, const std::wstring &html, const std::wstring &dictid, const TinyHtmlParser::CDocumentObject &doc, const TinyHtmlParser::CElementObject *dict, TResultMap &result) const
{
    const SpecialDictParser::CDictParser* parser = GetParser(dictid);
    if(parser == NULL)
        return 0;
    return parser->ParserHTML(html, doc, dict, result);
}

int CSpecialDictParser::SaveResult(CDBAccess::TDatabase &db, int wordid, const SpecialDictParser::TDictResultMap &result) const
{
    const SpecialDictParser::CDictParser* parser = NULL;
    for(SpecialDictParser::TDictResultMap::const_iterator it = result.begin(); it != result.end(); ++ it)
    {
        parser = GetParser(it->first);
        if(parser == NULL)
            return -1;
        if(parser->SaveResult(db, wordid, it->second) != 0)
            return -1;
    }
    return 0;
}

int CSpecialDictParser::GetResult(CDBAccess::TDatabase &db, int wordid, SpecialDictParser::TDictResultMap &result) const
{
    for(TParserMap::const_iterator it = _mapParser.begin(); it != _mapParser.end(); ++ it)
    {
        SpecialDictParser::CDictResult res;
        if(it->second->GetResult(db, wordid, res) == 0)
        {
            result.insert(std::make_pair(it->first, res));
        }
    }
    return 0;
}

int CSpecialDictParser::RemoveResult(CDBAccess::TDatabase &db, int wordid) const
{
    for(TParserMap::const_iterator it = _mapParser.begin(); it != _mapParser.end(); ++ it)
    {
        if(it->second->RemoveResult(db, wordid) != 0)
            return -1;
    }
    return 0;
}

