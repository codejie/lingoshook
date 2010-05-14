
#include "LangdaoDictObject.h"

namespace LANGDAO
{

const wxString CECParser::ID        =   _("7AB175CC5F622A44A0DECE976AF22A16");
const wxString CECParser::TITLE     =   _("Langdao E-C Dictionary");

CECParser::CECParser(int index, const wxString& id, const wxString& title, const wxDateTime& create)
: CDictParser(index, id, title, create)
{
}

CECParser::~CECParser()
{
}

int CECParser::Init(CDBAccess::TDatabase &db)
{
    try
    {
		const char* sybmoltable = "CREATE TABLE IF NOT EXISTS LangdaoECSymbolTable (WordID INTEGER PRIMARY KEY, Symbol VARCHAR(64))";
		db.ExecuteUpdate(sybmoltable);

		const char* resulttable = "CREATE TABLE IF NOT EXISTS LangdaoECResultTable (WordID INTEGER, ClassID INTEGER,Result VARCHAR(255))";
		db.ExecuteUpdate(resulttable);

        const char* specialtable = "CREATE TABLE IF NOT EXISTS LangdaoECSpecialResultTable (WordID INTEGER, SpecialID VARCHAR(16),Result VARCHAR(255))";
        db.ExecuteUpdate(specialtable);

        const char* commonlytable = "CREATE TABLE IF NOT EXISTS LangdaoECCommonlyPhraseTable (WordID INTEGER, Pattern VARCHAR(255))";
        db.ExecuteUpdate(commonlytable);
    }
    catch(CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

int CECParser::ParserHTML(const std::wstring& html, TinyHtmlParser::CDocumentObject& doc, const TinyHtmlParser::CElementObject* dict, TWordResultMap& result) const
{
    if(dict == NULL)
        return -1;

    if(doc.Root() == NULL)
        return -1;

    TinyHtmlParser::CDocumentObject::TElementStack tmpstack;
    const TinyHtmlParser::CElementObject* pr = doc.FindFirstElement(dict, L"DIV", tmpstack);
    while(pr != NULL)
    {
        const TinyHtmlParser::CAttributeObject* pa = pr->FindAttribute(L"style");
        if(pa != NULL && pa->value == L"\"MARGIN: 5px 0px\"")
        {
            if(GetRecord(&doc, pr, result) != 0)
                return -1;
        }
        pr = doc.FindNextElement(dict, L"DIV", tmpstack);
    } 

	return 0;
}

int CECParser::GetRecord(TinyHtmlParser::CDocumentObject* doc, const TinyHtmlParser::CElementObject* pr, TWordResultMap& result) const
{
    wxString word;
    std::auto_ptr<CECResult> res(new CECResult);

    TinyHtmlParser::CDocumentObject::TElementStack tmpstack;
    const TinyHtmlParser::CElementObject* p = doc->FindFirstElement(pr, L"DIV", tmpstack);
    while(p != NULL)
    {
        const TinyHtmlParser::CAttributeObject* pa = p->FindAttribute(L"style");
        if(pa == NULL)
            return -1;

        if(pa->value == L"\"MARGIN: 0px 0px 5px; COLOR: #808080; LINE-HEIGHT: normal\"")
        {//word and symbol
            if(p->child == NULL || p->child->child == NULL || p->child->child->type != TinyHtmlParser::ET_ELEMENT)
                return -1;
            //word
            word = wxString(p->child->child->value.c_str(), wxConvISO8859_1);
            //symbo
            if(p->child->sibling == NULL || p->child->sibling->child == NULL || p->child->sibling->child->type != TinyHtmlParser::ET_ELEMENT)
            {
                res->m_strSymbol = _("null");
                //return -1;
            }
            else
            {
                res->m_strSymbol = wxString(p->child->sibling->child->value.c_str(), wxConvISO8859_1);
            }
        }
        else if(pa->value == L"\"MARGIN: 0px 0px 5px\"")
        {//result
            const TinyHtmlParser::CAttributeObject* pdir = p->FindAttribute(L"dir");
            if(pdir == NULL)
            {//result
                if(GetResult(doc, p, *res) != 0)
                    return -1;
            }
            else
            {//commonly
                if(GetCommonly(doc, p, *res) != 0)
                    return -1;
            }
        }
        p = doc->FindNextElement(pr, L"DIV", tmpstack);
    }
    TWordResultMap::iterator it = result.insert(std::make_pair(word, TDictResultMap())).first;
    it->second.insert(std::make_pair(ID, CDictResult(res.release())));

    return 0;
}

int CECParser::GetResult(TinyHtmlParser::CDocumentObject* doc, const TinyHtmlParser::CElementObject* pr, CECResult& result) const
{
    TinyHtmlParser::CDocumentObject::TElementStack tmpstack;
    const TinyHtmlParser::CElementObject* pdiv = doc->FindFirstElement(pr, L"DIV", tmpstack);
    while(pdiv != NULL)
    {
        if(pdiv->type == TinyHtmlParser::ET_ELEMENT)
        {
            if(pdiv->child == NULL)
                return -1;
            if(pdiv->child->tag == L"SPAN")
            {//result
                CECResult::_result_t res;
                if(pdiv->child->child == NULL || pdiv->child->child->type != TinyHtmlParser::ET_ELEMENT)
                    return -1;
                res.m_eClass = StrToWC(wxString(pdiv->child->child->value.c_str(), wxConvISO8859_1));
                if(pdiv->child->sibling == NULL || pdiv->child->sibling->type != TinyHtmlParser::ET_ELEMENT)
                    return -1;
                res.m_strResult = wxString(pdiv->child->sibling->value.c_str(),wxConvISO8859_1);

                result.m_stRecord.m_vctResult.push_back(res);
            }
            else
            {//special
                CECResult::_special_t res;
                if(pdiv->child->type != TinyHtmlParser::ET_ELEMENT)
                    return -1;
                res.m_strSpecial = wxString(pdiv->child->value.c_str(), wxConvISO8859_1);
                res.m_strResult = wxString(pdiv->value.c_str(), wxConvISO8859_1);

                result.m_stRecord.m_vctSpecial.push_back(res);
            }
        }

        pdiv = doc->FindNextElement(pr, L"DIV", tmpstack);
    }
    return 0;
}

int CECParser::GetCommonly(TinyHtmlParser::CDocumentObject* doc, const TinyHtmlParser::CElementObject* pr, CECResult& result) const
{
    TinyHtmlParser::CDocumentObject::TElementStack tmpstack;
    const TinyHtmlParser::CElementObject* pdiv = doc->FindFirstElement(pr, L"DIV", tmpstack);
    while(pdiv != NULL)
    {
        if(pdiv->type == TinyHtmlParser::ET_ELEMENT)
        {
            result.m_stRecord.m_vctCommonly.push_back(wxString(pdiv->value.c_str(), wxConvISO8859_1));
        }

        pdiv = doc->FindNextElement(pr, L"DIV", tmpstack);
    }
    return 0;
}
WordClass CECParser::StrToWC(const wxString &str) const
{
	WordClass wc = WC_UNKNOWN;
	if(str == _("n."))
		return WC_NOUN;
	else if(str == _("a."))
		return WC_ADJECTIVE;
	else if(str == _("v."))
		return WC_VERB;
	else if(str == _("vt."))
		return WC_VERB_TRANSITIVE;
	else if(str == _("vi."))
		return WC_VERB_INTRANSITIVE;
	else if(str == _("ad."))
		return WC_ADVERB;
	else if(str == _("conj."))
		return WC_CONJ;
    else if(str == _("interj."))
        return WC_INTERJECTION;
    else if(str == _("prep."))
        return WC_PREPOSITION;
    else if(str == _("pron."))
        return WC_PRONOUN;
	else
		return WC_UNKNOWN;
}

const wxString CECParser::WCToStr(WordClass wc) const
{
	switch(wc)
	{
	case WC_NOUN:
		return _("n.");
	case WC_ADJECTIVE:
		return _("a.");
	case WC_VERB:
		return _("v.");
	case WC_VERB_TRANSITIVE:
		return _("vt.");
	case WC_VERB_INTRANSITIVE:
		return _("vi.");
	case WC_ADVERB:
		return _("ad.");
	case WC_CONJ:
		return _("conj.");
    case WC_INTERJECTION:
        return _("interj.");
    case WC_PREPOSITION:
        return _("prep.");
    case WC_PRONOUN:
        return _("pron.");
	default:
		return _("unkn.");
	}
}

int CECParser::GetResult(CDBAccess::TDatabase &db, int wordid, TDictResultMap &result)
{
    return -1;
}

int CECParser::GetResult(CDBAccess::TDatabase &db, int wordid, CDictResult &result)
{
    //symbol
    CDBAccess::TQuery query = db.PrepareStatement("SELECT Symbol FROM LangdaoECSymbolTable WHERE WordID = ?");
    query.Bind(1, wordid);
    CDBAccess::TResult res = query.ExecuteQuery();
    if(res.Eof())
        return -1;

    std::auto_ptr<CECResult> ecres(new CECResult());
    ecres->m_strSymbol = res.GetString(0);

    //results
    query.Reset();
    query = db.PrepareStatement("SELECT ClassID, Result FROM LangdaoECResultTable WHERE WordID = ?");
    query.Bind(1, wordid);
    res = query.ExecuteQuery();
    while(res.NextRow())
    {
        CECResult::_result_t r;
        r.m_eClass = (WordClass)(res.GetInt(0));
        r.m_strResult = res.GetString(1);

        ecres->m_stRecord.m_vctResult.push_back(r);
    }
    //special
    query.Reset();
    query = db.PrepareStatement("SELECT SpecialID, Result FROM LangdaoECSpecialResultTable WHERE WordID = ?");
    query.Bind(1, wordid);
    res = query.ExecuteQuery();
    while(res.NextRow())
    {
        CECResult::_special_t r;
        r.m_strSpecial = res.GetString(0);
        r.m_strResult = res.GetString(1);

        ecres->m_stRecord.m_vctSpecial.push_back(r);
    }
    //commonly
    query.Reset();
    query = db.PrepareStatement("SELECT Pattern FROM LangdaoECCommonlyPhraseTable WHERE WordID = ?");
    query.Bind(1, wordid);
    res = query.ExecuteQuery();
    while(res.NextRow())
    {
        ecres->m_stRecord.m_vctCommonly.push_back(res.GetString(0));
    }
    
    result.Attach(ecres.release());

    return 0;
}

int CECParser::IsWordExist(CDBAccess::TDatabase &db, int wordid)
{
    CDBAccess::TQuery query = db.PrepareStatement("SELECT COUNT(*) FROM LangdaoECSymbolTable WHERE WordID = ?");
    query.Bind(1, wordid);
    CDBAccess::TResult res = query.ExecuteQuery();
    if(res.GetInt(0) == 0)
        return -1;
    return 0;
}

int CECParser::SaveResult(CDBAccess::TDatabase &db, int wordid, const CDictResult &result)
{
    if(IsWordExist(db, wordid) == 0)
        return 0;

    const CECResult* res = dynamic_cast<const CECResult*>(result.Result());
    
    CDBAccess::TQuery query = db.PrepareStatement("INSERT INTO LangdaoECSymbolTable VALUES(?, ?)");
    query.Bind(1, wordid);
    query.Bind(2, res->m_strSymbol);
    query.ExecuteUpdate();
    
    //result
    for(CECResult::TResultRecordVector::const_iterator it = res->m_stRecord.m_vctResult.begin(); it != res->m_stRecord.m_vctResult.end(); ++ it)
    {
        query.Reset();
        query = db.PrepareStatement("INSERT INTO LangdaoECResultTable VALUES(?, ?, ?)");
	    query.Bind(1, wordid);
        query.Bind(2, it->m_eClass);
        query.Bind(3, it->m_strResult);
	    query.ExecuteUpdate();	
    }
    //special
    for(CECResult::TSpecialRecordVector::const_iterator it = res->m_stRecord.m_vctSpecial.begin(); it != res->m_stRecord.m_vctSpecial.end(); ++ it)
    {
        query.Reset();
        query = db.PrepareStatement("INSERT INTO LangdaoECSpecialResultTable VALUES(?, ?, ?)");
	    query.Bind(1, wordid);
        query.Bind(2, it->m_strSpecial);
        query.Bind(3, it->m_strResult);
	    query.ExecuteUpdate();	
    }
    //commonly
    for(CECResult::TCommonlyRecordVector::const_iterator it = res->m_stRecord.m_vctCommonly.begin(); it != res->m_stRecord.m_vctCommonly.end(); ++ it)
    {
        query.Reset();
        query = db.PrepareStatement("INSERT INTO LangdaoECCommonlyPhraseTable VALUES(?, ?)");
	    query.Bind(1, wordid);
        query.Bind(2, (*it));
	    query.ExecuteUpdate();	
    }

    return 0;
}

int CECParser::RemoveResult(CDBAccess::TDatabase &db, int wordid)
{
    CDBAccess::TQuery query = db.PrepareStatement("DELETE FROM LangdaoECSymbolTable WHERE WordID = ?");
    query.Bind(1, wordid);
    query.ExecuteUpdate();

    query.Reset();
    query = db.PrepareStatement("DELETE FROM LangdaoECResultTable WHERE WordID = ?");
    query.Bind(1, wordid);
    query.ExecuteUpdate();

    query.Reset();
    query = db.PrepareStatement("DELETE FROM LangdaoECSpecialResultTable WHERE WordID = ?");
    query.Bind(1, wordid);
    query.ExecuteUpdate();

    query.Reset();
    query = db.PrepareStatement("DELETE FROM LangdaoECCommonlyPhraseTable WHERE WordID = ?");
    query.Bind(1, wordid);
    query.ExecuteUpdate();

    return 0;
}

}