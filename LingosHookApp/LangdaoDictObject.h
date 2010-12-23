#ifndef __LANGDAODICTOBJECT_H__
#define __LANGDAODICTOBJECT_H__

#include <vector>
#include <string>

#include "SpecialDictParser.h"

namespace SpecialDictParser
{

namespace LANGDAO
{

enum WordClass { WC_UNKNOWN = -1, WC_NOUN = 0, WC_ADJECTIVE, WC_VERB, WC_VERB_TRANSITIVE, WC_VERB_INTRANSITIVE, WC_ADVERB
                , WC_CONJ, WC_INTERJECTION, WC_PREPOSITION, WC_PRONOUN };

class CECResult : public CParserResult
{
public:
    struct _result_t
    {
        WordClass m_eClass;
        std::wstring m_strResult;
    };
    typedef std::vector<_result_t> TResultRecordVector;

    struct _special_t
    {
        std::wstring m_strSpecial;
        std::wstring m_strResult;
    };
    typedef std::vector<_special_t> TSpecialRecordVector;

    typedef std::vector<std::wstring> TCommonlyRecordVector;

    typedef struct _record_t
    {
        TResultRecordVector m_vctResult;
        TSpecialRecordVector m_vctSpecial;
        TCommonlyRecordVector m_vctCommonly;
    } TRecord;
public:
    CECResult() {}
    virtual ~CECResult() {}
public:
    std::wstring m_strSymbol;
    TRecord m_stRecord;
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
    int GetResult(const TinyHtmlParser::CDocumentObject* doc, const TinyHtmlParser::CElementObject* pr, CECResult& result) const;
    int GetCommonly(const TinyHtmlParser::CDocumentObject* doc, const TinyHtmlParser::CElementObject* pr, CECResult& result) const;

    int IsWordExist(CDBAccess::TDatabase &db, int wordid) const;
};


}

}

#endif
