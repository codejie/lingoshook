/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#ifndef __DICTOBJECT_H__
#define __DICTOBJECT_H__

#include <map>
#include <vector>
#include <memory>

#include "wx/wx.h"

#include "TinyHtmlParser.h"
#include "DBAccess.h"


class CParserResult
{
public:
    CParserResult() {}
    virtual ~CParserResult() {}
};

class CDictResult
{
public:
    CDictResult(const CParserResult* result = NULL);
    virtual ~CDictResult();

    CDictResult(const CDictResult& right);

    void Attach(const CParserResult* result);
    const CParserResult* Result() const;
protected:
    std::auto_ptr<const CParserResult> _result;
};

typedef std::map<wxString, CDictResult> TDictResultMap;//dictid + result
typedef std::map<wxString, TDictResultMap> TWordResultMap;//word + result
typedef struct _WordData_t
{
    int m_iID;
    wxString m_strWord;
    int m_iCounter;
    wxDateTime m_dtCheckin;
    wxDateTime m_dtUpdate;
    wxString m_strHTML;
} TWordData;

class CDictParser
{
public:
    CDictParser(int index, const wxString& id, const wxString& title, const wxDateTime& create)
        : _iIndex(index), _strID(id), _strTitle(title), _dtCreateTime(create)
    {
    }
    virtual ~CDictParser() {}

    virtual int Init(CDBAccess::TDatabase& db) = 0;

    virtual int ParserHTML(const std::wstring& html, TinyHtmlParser::CDocumentObject& doc, const TinyHtmlParser::CElementObject* dict, TWordResultMap& result) const = 0;
    
    virtual int GetResult(CDBAccess::TDatabase& db, int wordid, TDictResultMap& result) = 0;

    virtual int GetResult(CDBAccess::TDatabase& db, int wordid, CDictResult& result) = 0;
    virtual int SaveResult(CDBAccess::TDatabase& db, int wordid, const CDictResult& result) = 0;    
    virtual int RemoveResult(CDBAccess::TDatabase& db, int wordid) = 0;

    const int GetIndex() const { return _iIndex; }
    const wxString& GetID() const { return _strID; }
    const wxString& GetTitle() const { return _strTitle; }
protected:
    int _iIndex;
    wxString _strID;
    wxString _strTitle;
    wxDateTime _dtCreateTime;
};

////////////////////////////
class CConfigData;

class CDictObject
{
protected:
    typedef std::map<wxString, CDictParser*> TParserMap;//id + object
public:
    CDictObject(CDBAccess& db, CConfigData& config);
    virtual ~CDictObject();

    int Init();
    //int AddParser(const wxString& id);

    void CacheWord(const wxString& word);
    int HTMLProc(const wxString& str, int mode);//0: dict anlyse, 1: ignore, 2: close dict

    int GetAllWords();
//    int GetWordData(const wxString& word, TWordData& data);
    int GetWordData(int wordid, TWordData& data);
    int GetResult(int wordid, TWordData& data, TDictResultMap& result);
    int GetResult(int wordid);

    int RemoveWord(int wordid);

    int GetWordID(const wxString& word, int& wordid);    
private:
    int HTMLProc(const wxString& str);
    int ForceSaveHTML(const wxString& str);

    int AddKnownParser();
    int LoadParser();
    CDictParser* GetParser(const wxString& id);

    int SaveWord(const wxString& word, const wxString& html, int& wordid);
    int UpdateWordData(int wordid, int counter);
    int SaveResult(const wxString& html, const TWordResultMap& result);
    
    void FreeParser();
private:
    int RegisterParser(const wxString& id, const wxString& title);
private:
    CDBAccess::TDatabase& _db;
    CConfigData& _config;    
    TParserMap _mapParser;
private:
    wxString _strCacheWord;
};



#endif
