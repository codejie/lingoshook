#ifndef __HTMLDICTPARSER_H__
#define __HTMLDICTPARSER_H__

#include <map>
#include <string>

#include "wx/wx.h"

#include "TinyHtmlParser.h"
#include "DBAccess.h"
#include "DictStructure.h"

namespace HtmlDictParser
{

struct TDictConfig
{
    int m_iLoadParam;
    int m_iStoreParam;
};

typedef std::map<int, TDictConfig> TDictConfigMap;//index + config

struct TDictInfo
{
    std::wstring m_strDictID;
    std::wstring m_strTitle;

    TDictConfig m_stConfig;
};

typedef std::map<std::wstring, int> TDictIDMap;//dictid + dictindex
typedef std::map<int, TDictInfo> TDictIndexMap;//dictindex + info

class CDictInfoObject
{
public:
    CDictInfoObject() {}
    virtual ~CDictInfoObject() {}

    int Init(CDBAccess::TDatabase& db);

    int Insert(int index, const TDictInfo& info);
    int GetDictIndex(const std::wstring& id) const;
protected:
    TDictIDMap _mapDictID;
    TDictIndexMap _mapDictIndex;
};
//
//struct TDictResult
//{
//    int m_iDictIndex;
//
//    int m_iDictStart;
//    int m_iDictEnd;
//};
//
//typedef std::map<int, TDictResult> TDictResultMap;//order + result

//typedef std::vector<TDictResult> TDictResultVector;
//typedef std::map<std::wstring, TDictResultVector> TDictResultMap;

}

class CHtmlDictParser
{
public:
    CHtmlDictParser() {}
    virtual ~CHtmlDictParser() {}

    virtual int Init(CDBAccess::TDatabase& db);
    //virtual int ParserHTML(const std::wstring& html, TDictResultVector& result);
    virtual int ParserHTML(CDBAccess::TDatabase &db, const std::wstring& html, const std::wstring& dictid, const TinyHtmlParser::CDocumentObject& doc, const TinyHtmlParser::CElementObject* dict, TResultMap& result);

    virtual int SaveResult(CDBAccess::TDatabase& db, int wordid, const HtmlDictParser::TDictResultMap& result);
    virtual int GetResult(CDBAccess::TDatabase& db, int wordid, HtmlDictParser::TDictResultMap& result);
    virtual int RemoveResult(CDBAccess::TDatabase& db, int wordid);

    const HtmlDictParser::TDictInfo* GetDictInfo(int dictindex) const;
    virtual int GenHtmlResult(const HtmlDictParser::TDictResultMap& dictresult, const std::wstring& html, std::wstring& htmlresult) const;
protected:
    int CheckDictHtml();
    int UpdateDictInfo(CDBAccess::TDatabase &db, const std::wstring& dictid, const std::wstring& html, const TinyHtmlParser::CDocumentObject& doc, const TinyHtmlParser::CElementObject* dict);
    int UpdateDictInfo(CDBAccess::TDatabase &db, const std::wstring& dictid, const std::wstring& title);

protected:
    HtmlDictParser::CDictInfoObject _objDictInfo;
};



#endif
