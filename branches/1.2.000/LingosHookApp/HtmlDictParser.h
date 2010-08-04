/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/
#ifndef __HTMLDICTPARSER_H__
#define __HTMLDICTPARSER_H__

#include <map>
#include <string>
#include <vector>

#include "wx/wx.h"

#include "TinyHtmlParser.h"
#include "DBAccess.h"
#include "DictStructure.h"


class CHtmlDictParser;

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
    int GetDictID(int index, std::wstring& id) const;
    int GetDictStoreParam(int index) const;

    size_t Size() const { return _mapDictID.size(); }
protected:
    friend class CHtmlDictParser;
    TDictIDMap _mapDictID;
    TDictIndexMap _mapDictIndex;
};

class CDictKnownAttrObject
{
protected:
    typedef std::map<std::wstring, int> TDefTypeMap;//dictid + deftype
public:
    CDictKnownAttrObject() {}

    int Init(CDBAccess::TDatabase& db);

    int GetDefType(const std::wstring& dictid) const;
    int SetDefType(CDBAccess::TDatabase& db, const std::wstring& dictid, int type);
private:
    int LoadDefType(CDBAccess::TDatabase& db);
    int UpdateDefType(CDBAccess::TDatabase& db, const std::wstring& dictid, int type) const;
    int InsertDefType(CDBAccess::TDatabase& db, const std::wstring& dictid, int type) const;
private:
    TDefTypeMap _mapDefType;
};

}

class CHtmlDictLoadChoiceDialog;
class CHtmlDictStoreChoiceDialog;

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

    //const HtmlDictParser::TDictInfo* GetDictInfo(int dictindex) const;
    virtual int GenHtmlResult(const HtmlDictParser::TDictResultMap& dictresult, const std::wstring& html, std::wstring& htmlresult) const;

    void ShowDictLoadInfo(int usehtmldict, CHtmlDictLoadChoiceDialog &dlg) const;
    int GetDictLoadInfo(CDBAccess::TDatabase &db, int& usehtmldict, const CHtmlDictLoadChoiceDialog& dlg);

    void ShowDictStoreInfo(CHtmlDictStoreChoiceDialog &dlg) const;
    int GetDictStoreInfo(CDBAccess::TDatabase &db, const CHtmlDictStoreChoiceDialog& dlg);
    int ResetDictStoreInfo(CDBAccess::TDatabase &db, CHtmlDictStoreChoiceDialog& dlg);
    void ShowDictStoreInfoItemContextMenu(const CHtmlDictStoreChoiceDialog& dlg, long item, int menubase, wxMenu* submenu) const;
    void RefreshDictStoreInfo(CHtmlDictStoreChoiceDialog &dlg, long item, int type) const;
    int UpdateDictStoreInfoDefType(CDBAccess::TDatabase &db, CHtmlDictStoreChoiceDialog &dlg, long item);
protected:
    int CheckDictHtml();
    int UpdateDictInfo(CDBAccess::TDatabase &db, const std::wstring& dictid, const std::wstring& html, const TinyHtmlParser::CDocumentObject& doc, const TinyHtmlParser::CElementObject* dict);
    int InsertDictInfo(CDBAccess::TDatabase &db, const std::wstring& dictid, const std::wstring& title, int load, int store);
    int UpdateDictConfig(CDBAccess::TDatabase &db, int dictindex, int loadparam, int storeparam);

    //int GetWord(const std::wstring& html, const std::wstring &dictid, const TinyHtmlParser::CDocumentObject &doc, const TinyHtmlParser::CElementObject *dict, const TinyHtmlParser::CElementObject* pdiv, int storeparam, std::wstring& word) const;

    void AppendDictStoreInfoTypeMenu(wxMenu* menu, int menuid, int dictindex, int type, int deftype) const;
private:
    int PushResult(const std::wstring& word, const HtmlDictParser::TDictResult& res, TResultMap &result) const;

    int HtmlDataType1Proc(const std::wstring& html, const std::wstring &dictid, const TinyHtmlParser::CDocumentObject &doc, const TinyHtmlParser::CElementObject *dict, const TinyHtmlParser::CElementObject* pdiv, const HtmlDictParser::TDictResult& res, TResultMap& result) const;
    int HtmlDataType2Proc(const std::wstring& html, const std::wstring &dictid, const TinyHtmlParser::CDocumentObject &doc, const TinyHtmlParser::CElementObject *dict, const TinyHtmlParser::CElementObject* pdiv, const HtmlDictParser::TDictResult& res, TResultMap& result) const;
    int HtmlDataType3Proc(const std::wstring& html, const std::wstring &dictid, const TinyHtmlParser::CDocumentObject &doc, const TinyHtmlParser::CElementObject *dict, const TinyHtmlParser::CElementObject* pdiv, const HtmlDictParser::TDictResult& res, TResultMap& result) const;
protected:
    HtmlDictParser::CDictInfoObject _objDictInfo;
private:
    HtmlDictParser::CDictKnownAttrObject _objDictKnownAttr;
};



#endif
