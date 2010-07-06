/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#ifndef __DICTOBJECT_H__
#define __DICTOBJECT_H__

#include <string>

#include "TinyHtmlParser.h"
#include "DBAccess.h"
#include "ConfigData.h"
#include "DictStructure.h"


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
};

#endif
