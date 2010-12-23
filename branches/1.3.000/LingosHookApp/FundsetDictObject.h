/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#ifndef __FUNDSETDICTOBJECT_H__
#define __FUNDSETDICTOBJECT_H__

#include <vector>

#include "SpecialDictParser.h"

namespace SpecialDictParser
{

namespace FUNDSET
{

class CDCResult : public CParserResult
{
public:
    typedef std::vector<std::wstring> TRecordVector;
public:
    CDCResult() {}
    virtual ~CDCResult() {}
public:
    std::wstring m_strKasus;
    TRecordVector m_vctRecord;
};

class CDCParser : public CDictParser
{
public:
    static const std::wstring ID;
    static const std::wstring TITLE;
public:
    CDCParser(int index, const std::wstring& id, const std::wstring& title, const wxDateTime& create);
    virtual ~CDCParser();

    virtual int Init(CDBAccess::TDatabase& db);

    virtual int ParserHTML(const std::wstring& html, const TinyHtmlParser::CDocumentObject& doc, const TinyHtmlParser::CElementObject* dict, TResultMap& result) const;
    
    virtual int GetResult(CDBAccess::TDatabase& db, int wordid, CDictResult& result) const;
    virtual int SaveResult(CDBAccess::TDatabase& db, int wordid, const CDictResult& result) const; 

    virtual int RemoveResult(CDBAccess::TDatabase& db, int wordid) const;
private:
    int GetRecord(const TinyHtmlParser::CDocumentObject* doc, const TinyHtmlParser::CElementObject* pr, TResultMap& result) const;
    int IsWordExist(CDBAccess::TDatabase &db, int wordid) const;
};

}

}

#endif
