/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#include <memory>

#include "ViconDictObject.h"

namespace SpecialDictParser
{

namespace VICON
{

const std::wstring CECParser::ID        =   L"E1C27E806399D047822B6650194A3D32";
const std::wstring CECParser::TITLE     =   L"Vicon English-Chinese(S) Dictionary";

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
		const char* sybmoltable = "CREATE TABLE IF NOT EXISTS ViconECSymbolTable (WordID INTEGER PRIMARY KEY, Symbol VARCHAR(64))";
		db.ExecuteUpdate(sybmoltable);

		const char* resulttable = "CREATE TABLE IF NOT EXISTS ViconECResultTable (WordID INTEGER, ClassID INTEGER,Result VARCHAR(255))";
		db.ExecuteUpdate(resulttable);
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
            if(p->child == NULL)
                return -1;
            if(p->child->child == NULL || p->child->child->type != TinyHtmlParser::ET_ELEMENT)
                return -1;
            word = p->child->child->value;//.c_str());//, wxConvISO8859_1);

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
        {//result and class
            CECResult::_record_t rec;

            if(p->child == NULL || p->child->type != TinyHtmlParser::ET_ELEMENT)
                return -1;
            rec.m_strResult = p->child->value;//.c_str());//,wxConvISO8859_1);
            //rec.m_strResult.Trim(false);

            if(p->child->child == NULL || p->child->child->type != TinyHtmlParser::ET_ELEMENT)
            {
                rec.m_eClass = WC_UNKNOWN;
                //return -1;
            }
            else
            {
                rec.m_eClass = StrToWC(p->child->child->value);//.c_str()));//, wxConvISO8859_1);
            }
            res->m_vctRecord.push_back(rec);            
        }
        
        p = doc->FindNextElement(pr, L"DIV", tmpstack);
    }


    TResultMap::iterator it = result.insert(std::make_pair(word, TResult())).first;
    it->second.m_resultDict.insert(std::make_pair(ID, CDictResult(res.release())));

    return 0;
}

WordClass CECParser::StrToWC(const std::wstring &str) const
{
	WordClass wc = WC_UNKNOWN;
	if(str == wxT("n."))
		return WC_NOUN;
	else if(str == wxT("adj."))
		return WC_ADJECTIVE;
	else if(str == wxT("v."))
		return WC_VERB;
	else if(str == wxT("adv."))
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
		return wxT("adj.");
	case WC_VERB:
		return wxT("v.");
	case WC_ADVERB:
		return wxT("adv.");
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

int CECParser::IsWordExist(CDBAccess::TDatabase &db, int wordid) const
{
    CDBAccess::TQuery query = db.PrepareStatement("SELECT COUNT(*) FROM ViconECSymbolTable WHERE WordID = ?");
    query.Bind(1, wordid);
    CDBAccess::TResult res = query.ExecuteQuery();
    if(res.GetInt(0) == 0)
        return -1;
    return 0;
}

int CECParser::GetResult(CDBAccess::TDatabase &db, int wordid, CDictResult &result) const
{
    try
    {
        CDBAccess::TQuery query = db.PrepareStatement("SELECT Symbol FROM ViconECSymbolTable WHERE WordID = ?");
        query.Bind(1, wordid);
        CDBAccess::TResult res = query.ExecuteQuery();
        if(res.Eof())
            return -1;

        std::auto_ptr<CECResult> ecres(new CECResult());
        ecres->m_strSymbol = res.GetString(0);

        query.Reset();
        query = db.PrepareStatement("SELECT ClassID, Result FROM ViconECResultTable WHERE WordID = ?");
        query.Bind(1, wordid);
        res = query.ExecuteQuery();
        while(res.NextRow())
        {
            CECResult::_record_t rec;
            rec.m_eClass = (WordClass)(res.GetInt(0));
            rec.m_strResult = res.GetString(1);

            ecres->m_vctRecord.push_back(rec);
        }
        
        result.Attach(ecres.release());
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }

    return 0;
}

int CECParser::SaveResult(CDBAccess::TDatabase &db, int wordid, const CDictResult &result) const
{
    try
    {
        if(IsWordExist(db, wordid) == 0)
            return 0;

        const CECResult* res = dynamic_cast<const CECResult*>(result.Result());
        
        CDBAccess::TQuery query = db.PrepareStatement("INSERT INTO ViconECSymbolTable VALUES(?, ?)");
        query.Bind(1, wordid);
        query.Bind(2, res->m_strSymbol);
        query.ExecuteUpdate();
        
        for(CECResult::TRecordVector::const_iterator it = res->m_vctRecord.begin(); it != res->m_vctRecord.end(); ++ it)
        {
            query.Reset();
            query = db.PrepareStatement("INSERT INTO ViconECResultTable VALUES(?, ?, ?)");
	        query.Bind(1, wordid);
            query.Bind(2, it->m_eClass);
            query.Bind(3, it->m_strResult);
	        query.ExecuteUpdate();	
        }
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }

    return 0;
}

int CECParser::RemoveResult(CDBAccess::TDatabase &db, int wordid)  const
{
    try
    {
        CDBAccess::TQuery query = db.PrepareStatement("DELETE FROM ViconECSymbolTable WHERE WordID = ?");
        query.Bind(1, wordid);
        query.ExecuteUpdate();

        query.Reset();
        query = db.PrepareStatement("DELETE FROM ViconECResultTable WHERE WordID = ?");
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