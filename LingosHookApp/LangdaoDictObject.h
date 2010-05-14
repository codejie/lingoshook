#ifndef __LANGDAODICTOBJECT_H__
#define __LANGDAODICTOBJECT_H__

#include <vector>

#include "DictObject.h"

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
        wxString m_strResult;
    };
    typedef std::vector<_result_t> TResultRecordVector;

    struct _special_t
    {
        wxString m_strSpecial;
        wxString m_strResult;
    };
    typedef std::vector<_special_t> TSpecialRecordVector;

    typedef std::vector<wxString> TCommonlyRecordVector;

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
    wxString m_strSymbol;
    TRecord m_stRecord;
};

class CECParser : public CDictParser
{
public:
    static const wxString ID;
    static const wxString TITLE;
public:
    CECParser(int index, const wxString& id, const wxString& title, const wxDateTime& create);
    virtual ~CECParser();

    virtual int Init(CDBAccess::TDatabase& db);

    virtual int ParserHTML(const std::wstring& html, TinyHtmlParser::CDocumentObject& doc, const TinyHtmlParser::CElementObject* dict, TWordResultMap& result) const;
    
    virtual int GetResult(CDBAccess::TDatabase& db, int wordid, TDictResultMap& result);
    virtual int GetResult(CDBAccess::TDatabase& db, int wordid, CDictResult& result);
    virtual int SaveResult(CDBAccess::TDatabase& db, int wordid, const CDictResult& result); 

    virtual int RemoveResult(CDBAccess::TDatabase& db, int wordid);

public:
	WordClass StrToWC(const wxString& str) const;
	const wxString WCToStr(WordClass wc) const;
private:
    int GetRecord(TinyHtmlParser::CDocumentObject* doc, const TinyHtmlParser::CElementObject* pr, TWordResultMap& result) const;
    int GetResult(TinyHtmlParser::CDocumentObject* doc, const TinyHtmlParser::CElementObject* pr, CECResult& result) const;
    int GetCommonly(TinyHtmlParser::CDocumentObject* doc, const TinyHtmlParser::CElementObject* pr, CECResult& result) const;

    int IsWordExist(CDBAccess::TDatabase &db, int wordid);
};


}

#endif
