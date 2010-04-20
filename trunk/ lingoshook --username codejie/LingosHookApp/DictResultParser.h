#ifndef __DICTRESULTPARSER_H__
#define __DICTRESULTPARSER_H__

#include "wx/wx.h"

#include "DBAccess.h"
#include "TinyHtmlParser.h"

class CDictResultParser
{
public:
    CDictResultParser() {}
	virtual ~CDictResultParser() {}

	static CDictResultParser* MakeParser(const wxString& str);

	virtual int ParserText(const wxString& text, const wxString& html, CDBAccess::TRecordDataVector& vct) const = 0;
	virtual int ParserHTML(const wxString& html, CDBAccess::TRecordDataVector& vct) const = 0;
    virtual int ParserHTML(const wxString& html, TinyHtmlParser::CDocumentObject& doc, const TinyHtmlParser::CElementObject* dict, CDBAccess::TRecordDataVector& vct) const = 0;


    virtual const wxString GetTitle() const = 0;
    virtual const wxString GetID() const = 0;

	virtual CDBAccess::WordClass StrToWC(const wxString& str) const = 0;
	virtual const wxString WCToStr(CDBAccess::WordClass wc) const = 0;
};


class CViconECDictResultParser : public CDictResultParser
{
public:
    static wxString TITLE;
    static wxString ID;
public:
	CViconECDictResultParser();
	virtual ~CViconECDictResultParser() {}

	virtual int ParserText(const wxString& text, const wxString& html, CDBAccess::TRecordDataVector& vct) const;
	virtual int ParserHTML(const wxString& html, CDBAccess::TRecordDataVector& vct) const;
    virtual int ParserHTML(const wxString& html, TinyHtmlParser::CDocumentObject& doc, const TinyHtmlParser::CElementObject* dict, CDBAccess::TRecordDataVector& vct) const;

    virtual const wxString GetTitle() const { return CViconECDictResultParser::TITLE; }
    virtual const wxString GetID() const { return CViconECDictResultParser::ID; }

	virtual CDBAccess::WordClass StrToWC(const wxString& str) const;
	virtual const wxString WCToStr(CDBAccess::WordClass wc) const;
protected:
    int GetRecord(TinyHtmlParser::CDocumentObject* doc, const TinyHtmlParser::CElementObject* pr, CDBAccess::TRecordData& rec) const;
};




#endif
