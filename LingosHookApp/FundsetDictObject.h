/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#ifndef __FUNDSETDICTOBJECT_H__
#define __FUNDSETDICTOBJECT_H__

#include <vector>

#include "DictObject.h"

namespace FUNDSET
{

class CDCResult : public CParserResult
{
public:
    typedef std::vector<wxString> TRecordVector;
public:
    CDCResult() {}
    virtual ~CDCResult() {}
public:
    wxString m_strKasus;
    TRecordVector m_vctRecord;
};

class CDCParser : public CDictParser
{
public:
    static const wxString ID;
    static const wxString TITLE;
public:
    CDCParser(int index, const wxString& id, const wxString& title, const wxDateTime& create);
    virtual ~CDCParser();

    virtual int Init(CDBAccess::TDatabase& db);

    virtual int ParserHTML(const std::wstring& html, TinyHtmlParser::CDocumentObject& doc, const TinyHtmlParser::CElementObject* dict, TWordResultMap& result) const;
    
    virtual int GetResult(CDBAccess::TDatabase& db, int wordid, TDictResultMap& result);
    virtual int GetResult(CDBAccess::TDatabase& db, int wordid, CDictResult& result);
    virtual int SaveResult(CDBAccess::TDatabase& db, int wordid, const CDictResult& result); 

    virtual int RemoveResult(CDBAccess::TDatabase& db, int wordid);
private:
    int GetRecord(TinyHtmlParser::CDocumentObject* doc, const TinyHtmlParser::CElementObject* pr, TWordResultMap& result) const;
    int IsWordExist(CDBAccess::TDatabase &db, int wordid);
};

}


#endif
