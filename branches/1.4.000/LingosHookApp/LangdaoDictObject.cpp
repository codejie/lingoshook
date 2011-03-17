
#include "LangdaoDictObject.h"

namespace SpecialDictParser
{

namespace LANGDAO
{

const std::wstring CECParser::ID        =   L"7AB175CC5F622A44A0DECE976AF22A16";
const std::wstring CECParser::TITLE     =   L"Langdao E-C Dictionary";

CECParser::CECParser(int index, const std::wstring& id, const std::wstring& title, const wxDateTime& create)
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

int CECParser::ParserHTML(const std::wstring& html, const TinyHtmlParser::CDocumentObject& doc, const TinyHtmlParser::CElementObject* dict, TResultMap& result) const
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

int CECParser::GetRecord(const TinyHtmlParser::CDocumentObject* doc, const TinyHtmlParser::CElementObject* pr, TResultMap& result) const
{
    std::wstring word;
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
            word = p->child->child->value;//.c_str());//, wxConvISO8859_1);
            //symbo
            if(p->child->sibling == NULL || p->child->sibling->child == NULL || p->child->sibling->child->type != TinyHtmlParser::ET_ELEMENT)
            {
                res->m_strSymbol = wxT("null");
                //return -1;
            }
            else
            {
                res->m_strSymbol = std::wstring(p->child->sibling->child->value.c_str());//, wxConvISO8859_1);
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
    TResultMap::iterator it = result.insert(std::make_pair(word, TResult())).first;
    it->second.m_resultDict.insert(std::make_pair(ID, CDictResult(res.release())));

    return 0;
}

int CECParser::GetResult(const TinyHtmlParser::CDocumentObject* doc, const TinyHtmlParser::CElementObject* pr, CECResult& result) const
{
    const TinyHtmlParser::CElementObject* pdiv = pr->child;
    while(pdiv != NULL && pdiv->attrib != NULL && pdiv->attrib->value == L"\"MARGIN: 4px 0px\"")
    {
        if(pdiv->child != NULL && pdiv->child->child != NULL)
        {
            if(pdiv->child->child->tag == L"SPAN")
            {//result
                CECResult::_result_t res;
                if(pdiv->child->child->child == NULL || pdiv->child->child->child->type != TinyHtmlParser::ET_ELEMENT)
                    return -1;
                res.m_eClass = StrToWC(pdiv->child->child->child->value);//.c_str()));//, wxConvISO8859_1);
                if(pdiv->child->child->sibling == NULL || pdiv->child->child->sibling->type != TinyHtmlParser::ET_ELEMENT)
                    return -1;
                res.m_strResult = pdiv->child->child->sibling->value;//.c_str());//,wxConvISO8859_1);

                result.m_stRecord.m_vctResult.push_back(res);
            }
            else if(pdiv->child->child->tag == L"FONT")
            {
                if(pdiv->child->child->attrib != NULL && pdiv->child->child->attrib->value == L"\"#000080\"")
                {//special
                    CECResult::_special_t res;
                    if(pdiv->child->child->type != TinyHtmlParser::ET_ELEMENT)
                        return -1;
                    res.m_strSpecial = pdiv->child->child->value;//.c_str());//, wxConvISO8859_1);
                    res.m_strResult = pdiv->child->value;//.c_str();//);//, wxConvISO8859_1);

                    result.m_stRecord.m_vctSpecial.push_back(res);
                }
                else
                {//multi-result
                    CECResult::_result_t res;
                    res.m_eClass = StrToWC(pdiv->child->child->value);
                    const TinyHtmlParser::CElementObject* pc = pdiv->child->sibling;
                    while(pc != NULL && pc->child->FindAttribute(L"noWrap") != NULL)
                    {
                        res.m_strResult = pc->child->child->child->value + pc->child->child->value;
                        res.m_strResult += pc->child->sibling->value;
                        result.m_stRecord.m_vctResult.push_back(res);                    

                        pc = pc->sibling;
                    }                    
                }
            }
        }

        pdiv = pdiv->sibling;
    }

    return 0;
}

int CECParser::GetCommonly(const TinyHtmlParser::CDocumentObject* doc, const TinyHtmlParser::CElementObject* pr, CECResult& result) const
{
    TinyHtmlParser::CDocumentObject::TElementStack tmpstack;
    const TinyHtmlParser::CElementObject* pdiv = doc->FindFirstElement(pr, L"DIV", tmpstack);
    while(pdiv != NULL)
    {
        if(pdiv->type == TinyHtmlParser::ET_ELEMENT)
        {
            result.m_stRecord.m_vctCommonly.push_back(pdiv->value);//.c_str()));//, wxConvISO8859_1);
        }

        pdiv = doc->FindNextElement(pr, L"DIV", tmpstack);
    }
    return 0;
}

WordClass CECParser::StrToWC(const std::wstring &str) const
{
	WordClass wc = WC_UNKNOWN;
	if(str == wxT("n."))
		return WC_NOUN;
	else if(str == wxT("a."))
		return WC_ADJECTIVE;
	else if(str == wxT("v."))
		return WC_VERB;
	else if(str == wxT("vt."))
		return WC_VERB_TRANSITIVE;
	else if(str == wxT("vi."))
		return WC_VERB_INTRANSITIVE;
	else if(str == wxT("ad."))
		return WC_ADVERB;
	else if(str == wxT("conj."))
		return WC_CONJ;
    else if(str == wxT("interj."))
        return WC_INTERJECTION;
    else if(str == wxT("prep."))
        return WC_PREPOSITION;
    else if(str == wxT("pron."))
        return WC_PRONOUN;
	else
		return WC_UNKNOWN;
}

const std::wstring CECParser::WCToStr(WordClass wc) const
{
	switch(wc)
	{
	case WC_NOUN:
		return wxT("n.");
	case WC_ADJECTIVE:
		return wxT("a.");
	case WC_VERB:
		return wxT("v.");
	case WC_VERB_TRANSITIVE:
		return wxT("vt.");
	case WC_VERB_INTRANSITIVE:
		return wxT("vi.");
	case WC_ADVERB:
		return wxT("ad.");
	case WC_CONJ:
		return wxT("conj.");
    case WC_INTERJECTION:
        return wxT("interj.");
    case WC_PREPOSITION:
        return wxT("prep.");
    case WC_PRONOUN:
        return wxT("pron.");
	default:
		return wxT("unkn.");
	}
}

int CECParser::GetResult(CDBAccess::TDatabase &db, int wordid, CDictResult &result) const 
{
    try
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
            r.m_strResult = res.GetString(1).c_str();

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
            r.m_strSpecial = res.GetString(0).c_str();
            r.m_strResult = res.GetString(1).c_str();

            ecres->m_stRecord.m_vctSpecial.push_back(r);
        }
        //commonly
        query.Reset();
        query = db.PrepareStatement("SELECT Pattern FROM LangdaoECCommonlyPhraseTable WHERE WordID = ?");
        query.Bind(1, wordid);
        res = query.ExecuteQuery();
        while(res.NextRow())
        {
            ecres->m_stRecord.m_vctCommonly.push_back(res.GetString(0).c_str());
        }
        
        result.Attach(ecres.release());
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }

    return 0;
}

int CECParser::IsWordExist(CDBAccess::TDatabase &db, int wordid) const 
{
    CDBAccess::TQuery query = db.PrepareStatement("SELECT COUNT(*) FROM LangdaoECSymbolTable WHERE WordID = ?");
    query.Bind(1, wordid);
    CDBAccess::TResult res = query.ExecuteQuery();
    if(res.GetInt(0) == 0)
        return -1;
    return 0;
}

int CECParser::SaveResult(CDBAccess::TDatabase &db, int wordid, const CDictResult &result) const
{
    try
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
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }

    return 0;
}

int CECParser::RemoveResult(CDBAccess::TDatabase &db, int wordid) const
{
    try
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
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }

    return 0;
}

}

}