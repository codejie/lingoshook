/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#ifndef __SPECIALDICTPARSER_H__
#define __SPECIALDICTPARSER_H__

#include <map>
#include <string>
#include <memory>

#include "wx/wx.h"

#include "TinyHtmlParser.h"
#include "DBAccess.h"
#include "DictObject.h"

namespace SpecialDictParser
{

class CDictParser
{
public:
    CDictParser(int index, const std::wstring& id, const std::wstring& title, const wxDateTime& create)
        : _iIndex(index), _strID(id), _strTitle(title), _dtCreateTime(create)
    {
    }
    virtual ~CDictParser() {}

    virtual int Init(CDBAccess::TDatabase& db) = 0;

    virtual int ParserHTML(const std::wstring& html, const TinyHtmlParser::CDocumentObject& doc, const TinyHtmlParser::CElementObject* dict, TResultMap& result) const = 0;
    
    virtual int GetResult(CDBAccess::TDatabase& db, int wordid, CDictResult& result) const = 0;
    virtual int SaveResult(CDBAccess::TDatabase& db, int wordid, const CDictResult& result) const = 0;    
    virtual int RemoveResult(CDBAccess::TDatabase& db, int wordid) const = 0;

    const int GetIndex() const { return _iIndex; }
    const std::wstring& GetID() const { return _strID; }
    const std::wstring& GetTitle() const { return _strTitle; }
protected:
    int _iIndex;
    std::wstring _strID;
    std::wstring _strTitle;
    wxDateTime _dtCreateTime;
};

}

//
class CSpecialDictParser
{
protected:
    typedef std::map<std::wstring, SpecialDictParser::CDictParser*> TParserMap;//id + object
public:
    CSpecialDictParser() {}
    virtual ~CSpecialDictParser();

    int Init(CDBAccess::TDatabase& db);

    int ParserHTML(CDBAccess::TDatabase& db, const std::wstring& html, const std::wstring& dictid, const TinyHtmlParser::CDocumentObject& doc, const TinyHtmlParser::CElementObject* dict, TResultMap& result) const;
    int SaveResult(CDBAccess::TDatabase& db, int wordid, const SpecialDictParser::TDictResultMap& result) const;
    int GetResult(CDBAccess::TDatabase& db, int wordid, SpecialDictParser::TDictResultMap& result) const;
    int RemoveResult(CDBAccess::TDatabase& db, int wordid) const;

    const SpecialDictParser::CDictParser* GetParser(const std::wstring& id) const;
protected:
    int AddKnownParser(CDBAccess::TDatabase& db);
    int LoadParser(CDBAccess::TDatabase& db);
    int RegisterParser(CDBAccess::TDatabase& db, const std::wstring& id, const std::wstring& title, int loadparam, int storeparam);
    void FreeParser();

private:
    TParserMap _mapParser;
};

#endif
