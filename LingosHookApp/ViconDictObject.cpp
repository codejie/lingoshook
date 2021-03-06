/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#include <memory>

#include "ViconDictObject.h"

namespace VICON
{

const wxString CECParser::ID        =   _("E1C27E806399D047822B6650194A3D32");
const wxString CECParser::TITLE     =   _("Vicon English-Chinese(S) Dictionary");

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

int CECParser::ParserHTML(const wxString& html, TinyHtmlParser::CDocumentObject& doc, const TinyHtmlParser::CElementObject* dict, TWordResultMap& result) const
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
            if(p->child == NULL)
                return -1;
            if(p->child->child == NULL || p->child->child->type != TinyHtmlParser::ET_ELEMENT)
                return -1;
            word = wxString(p->child->child->value.c_str(), wxConvISO8859_1);

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
        {//result and class
            CECResult::_record_t rec;

            if(p->child == NULL || p->child->type != TinyHtmlParser::ET_ELEMENT)
                return -1;
            rec.m_strResult = wxString(p->child->value.c_str(),wxConvISO8859_1);
            rec.m_strResult.Trim(false);

            if(p->child->child == NULL || p->child->child->type != TinyHtmlParser::ET_ELEMENT)
            {
                rec.m_eClass = WC_UNKNOWN;
                //return -1;
            }
            else
            {
                rec.m_eClass = StrToWC(wxString(p->child->child->value.c_str(), wxConvISO8859_1));
            }
            res->m_vctRecord.push_back(rec);            
        }
        
        p = doc->FindNextElement(pr, L"DIV", tmpstack);
    }

    TWordResultMap::iterator it = result.insert(std::make_pair(word, TDictResultMap())).first;
    it->second.insert(std::make_pair(ID, CDictResult(res.release())));

    return 0;
}

WordClass CECParser::StrToWC(const wxString &str) const
{
	WordClass wc = WC_UNKNOWN;
	if(str == _("n."))
		return WC_NOUN;
	else if(str == _("adj."))
		return WC_ADJECTIVE;
	else if(str == _("v."))
		return WC_VERB;
	else if(str == _("adv."))
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
		return _("adj.");
	case WC_VERB:
		return _("v.");
	case WC_ADVERB:
		return _("adv.");
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

int CECParser::IsWordExist(CDBAccess::TDatabase &db, int wordid)
{
    CDBAccess::TQuery query = db.PrepareStatement("SELECT COUNT(*) FROM ViconECSymbolTable WHERE WordID = ?");
    query.Bind(1, wordid);
    CDBAccess::TResult res = query.ExecuteQuery();
    if(res.GetInt(0) == 0)
        return -1;
    return 0;
}

int CECParser::GetResult(CDBAccess::TDatabase &db, int wordid, TDictResultMap &result)
{
    return -1;
}

int CECParser::GetResult(CDBAccess::TDatabase &db, int wordid, CDictResult &result)
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

    return 0;
}

int CECParser::SaveResult(CDBAccess::TDatabase &db, int wordid, const CDictResult &result)
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

    return 0;
}

int CECParser::RemoveResult(CDBAccess::TDatabase &db, int wordid)
{
    CDBAccess::TQuery query = db.PrepareStatement("DELETE FROM ViconECSymbolTable WHERE WordID = ?");
    query.Bind(1, wordid);
    query.ExecuteUpdate();

    query.Reset();
    query = db.PrepareStatement("DELETE FROM ViconECResultTable WHERE WordID = ?");
    query.Bind(1, wordid);
    query.ExecuteUpdate();

    return 0;
}

}