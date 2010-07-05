/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#ifndef __DICTOBJECT_H__
#define __DICTOBJECT_H__

//#include <map>
//#include <vector>
//#include <memory>
//

#include <string>

//
#include "TinyHtmlParser.h"
#include "DBAccess.h"
#include "ConfigData.h"
#include "DictStructure.h"
//#include "LHControls.h"
//#include "HtmlDictParser.h"
//#include "SpecialDictParser.h"


//
//class CParserResult
//{
//public:
//    CParserResult() {}
//    virtual ~CParserResult() {}
//};
//
//class CDictResult
//{
//public:
//    CDictResult(const CParserResult* result = NULL);
//    virtual ~CDictResult();
//
//    CDictResult(const CDictResult& right);
//
//    void Attach(const CParserResult* result);
//    const CParserResult* Result() const;
//protected:
//    std::auto_ptr<const CParserResult> _result;
//};
//
//typedef std::map<wxString, CDictResult> SpecialDictParser::TDictResultMap;//dictid + result
//typedef std::map<wxString, SpecialDictParser::TDictResultMap> SpecialDictParser::TWordResultMap;//word + result
//typedef struct _WordData_t
//{
//    int m_iID;
//    wxString m_strWord;
//    int m_iCounter;
//    wxDateTime m_dtCheckin;
//    wxDateTime m_dtUpdate;
//    wxString m_strHTML;
//} TWordData;
//
//class SpecialDictParser::CDictParser
//{
//public:
//    SpecialDictParser::CDictParser(int index, const wxString& id, const wxString& title, const wxDateTime& create)
//        : _iIndex(index), _strID(id), _strTitle(title), _dtCreateTime(create)
//    {
//    }
//    virtual ~SpecialDictParser::CDictParser() {}
//
//    virtual int Init(CDBAccess::TDatabase& db) = 0;
//
//    virtual int ParserHTML(const std::wstring& html, TinyHtmlParser::CDocumentObject& doc, const TinyHtmlParser::CElementObject* dict, SpecialDictParser::TWordResultMap& result) const = 0;
//    
//    virtual int GetResult(CDBAccess::TDatabase& db, int wordid, SpecialDictParser::TDictResultMap& result) = 0;
//
//    virtual int GetResult(CDBAccess::TDatabase& db, int wordid, CDictResult& result) = 0;
//    virtual int SaveResult(CDBAccess::TDatabase& db, int wordid, const CDictResult& result) = 0;    
//    virtual int RemoveResult(CDBAccess::TDatabase& db, int wordid) = 0;
//
//    const int GetIndex() const { return _iIndex; }
//    const wxString& GetID() const { return _strID; }
//    const wxString& GetTitle() const { return _strTitle; }
//protected:
//    int _iIndex;
//    wxString _strID;
//    wxString _strTitle;
//    wxDateTime _dtCreateTime;
//};

////////////////////////////

class CSpecialDictParser;
class CHtmlDictParser;
class CHtmlDictChoiceDialog;

class CDictObject
{
public:
    CDictObject(CDBAccess& db, CConfigData& config);
    virtual ~CDictObject();

    int Init();

    void CacheWord(const wxString& word);
    int HTMLProc(const wxString& str);

    int GetAllWords();
    int GetResult(int wordid);

    int RemoveWord(int wordid);

    int GetWordID(const std::wstring& word, int& wordid);  

    void ShowHtmlDictInfo(CHtmlDictChoiceDialog& dlg) const;
    int GetHtmlDictInfo(const CHtmlDictChoiceDialog& dlg);
private:
    int ParserHTML(const std::wstring& html);
    int ForceSaveHTML(const std::wstring& html);

    int ParserSpecialDict(const std::wstring& html, const std::wstring& dictid, const TinyHtmlParser::CDocumentObject& doc, const TinyHtmlParser::CElementObject* dict, TResultMap& result);
    int ParserHtmlDict(const std::wstring& html, const std::wstring& dictid, const TinyHtmlParser::CDocumentObject& doc, const TinyHtmlParser::CElementObject* dict, TResultMap& result);

    int GetSpecialDictResult(const TWordData& data);
    int GetHtmlDictResult(const TWordData& data);

    int SaveResult(const std::wstring& html, const TResultMap& result);
    int SaveWord(const std::wstring& html, const std::wstring& word, int& wordid, bool& isexist);
    int UpdateWordData(int wordid, int counter);

    int GetWordData(int wordid, TWordData& data);
private:
    CDBAccess::TDatabase& _db;
    CConfigData& _config;
    std::wstring _strCacheWord;
private:
    std::auto_ptr<CSpecialDictParser> _objSpecialDictParser;
    std::auto_ptr<CHtmlDictParser> _objHtmlDictParser;
    //SpecialDictParser::CParser _objSpecialDictParser;
    //HtmlDictParser::CParser _objHtmlDictParser;
};

///////////

    //int AddParser(const wxString& id);
//
//    int HTMLProc(const wxString& str, int mode);//0: dict anlyse, 1: ignore, 2: close dict
//
////    int GetWordData(const wxString& word, TWordData& data);
//    int GetWordData(int wordid, TWordData& data);
//    int GetResult(int wordid, TWordData& data, SpecialDictParser::TDictResultMap& result, HtmlDictParser::TDictResultVector& htmlresult);
//    int GetResult(int wordid);
//
//    int RemoveWord(int wordid);
//
//    int GetWordID(const wxString& word, int& wordid);    
//private:
//    int HTMLProc(const wxString& str, bool skipdict);
//    int ForceSaveHTML(const wxString& str);
//
////    int AddKnownParser();
////    int LoadParser();
////    SpecialDictParser::CDictParser* GetParser(const wxString& id);
//
//    int SaveWord(const wxString& word, const wxString& html, int& wordid, bool& isexist);
//    int UpdateWordData(int wordid, int counter);
//    int SaveResult(const wxString& html, const SpecialDictParser::TWordResultMap& result, const HtmlDictParser::TDictResultVector& htmlresult);
//    
////    void FreeParser();
//private:
////    int RegisterParser(const wxString& id, const wxString& title);
//private:
//    CDBAccess::TDatabase& _db;
//    CConfigData& _config;    
////    TParserMap _mapParser;
//private:
//    wxString _strCacheWord;

//};



#endif
