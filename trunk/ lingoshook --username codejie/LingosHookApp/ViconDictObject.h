#ifndef __VICONDICTOBJECT_H__
#define __VICONDICTOBJECT_H__

#include <vector>

#include "DictObject.h"

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
        wxString m_strResult;
    };
    typedef std::vector<_record_t> TRecordVector;
public:
    CECResult() {}
    virtual ~CECResult() {}

public:
    wxString m_strSymbol;
    TRecordVector m_vctRecord;
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

    virtual int ParserHTML(const wxString& html, TinyHtmlParser::CDocumentObject& doc, const TinyHtmlParser::CElementObject* dict, TWordResultMap& result) const;
    
    virtual int GetResult(CDBAccess::TDatabase& db, int wordid, TDictResultMap& result);
    virtual int GetResult(CDBAccess::TDatabase& db, int wordid, CDictResult& result);
    virtual int SaveResult(CDBAccess::TDatabase& db, int wordid, const CDictResult& result); 

    virtual int RemoveResult(CDBAccess::TDatabase& db, int wordid);

public:
	WordClass StrToWC(const wxString& str) const;
	const wxString WCToStr(WordClass wc) const;
private:
    int GetRecord(TinyHtmlParser::CDocumentObject* doc, const TinyHtmlParser::CElementObject* pr, TWordResultMap& result) const;
    int IsWordExist(CDBAccess::TDatabase &db, int wordid);
};

}


#endif
