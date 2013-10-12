/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#ifndef __VICONDICTOBJECT_H__
#define __VICONDICTOBJECT_H__

#include <vector>

#include "DictStructure.h"
#include "SpecialDictParser.h"

namespace SpecialDictParser
{

namespace VICON
{

enum WordClass { WC_UNKNOWN = -1, WC_NOUN = 0, WC_ADJECTIVE, WC_VERB, WC_ADVERB, WC_CONJ, WC_INTERJECTION, WC_PREPOSITION
                    , WC_PRONOUN};

class CECResult : public CParserResult
{
public:
    struct _record_t
    {
        WordClass m_eClass;
        std::wstring m_strResult;
    };
    typedef std::vector<_record_t> TRecordVector;
public:
    CECResult() {}
    virtual ~CECResult() {}

public:
    std::wstring m_strSymbol;
    TRecordVector m_vctRecord;
};

class CECParser : public CDictParser
{
public:
    static const std::wstring ID;
    static const std::wstring TITLE;
public:
    CECParser(int index, const std::wstring& id, const std::wstring& title, const wxDateTime& create);
    virtual ~CECParser();

    virtual int Init(CDBAccess::TDatabase& db);

    virtual int ParserHTML(const std::wstring& html, const TinyHtmlParser::CDocumentObject& doc, const TinyHtmlParser::CElementObject* dict, TResultMap& result) const;
    
    virtual int GetResult(CDBAccess::TDatabase& db, int wordid, CDictResult& result) const;
    virtual int SaveResult(CDBAccess::TDatabase& db, int wordid, const CDictResult& result) const; 

    virtual int RemoveResult(CDBAccess::TDatabase& db, int wordid) const;

public:
	WordClass StrToWC(const std::wstring& str) const;
	const std::wstring WCToStr(WordClass wc) const;
private:
    int GetRecord(const TinyHtmlParser::CDocumentObject* doc, const TinyHtmlParser::CElementObject* pr, TResultMap& result) const;
    int IsWordExist(CDBAccess::TDatabase &db, int wordid) const;
};

}

}


#endif
