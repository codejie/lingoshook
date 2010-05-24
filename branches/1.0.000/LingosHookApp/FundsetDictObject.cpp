/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#include <memory>

#include "FundsetDictObject.h"

namespace FUNDSET
{

const wxString CDCParser::ID        =   _("1E388F323A0BD943B30FC68598772814");
const wxString CDCParser::TITLE     =   _("Fundset Deutsch2Chinese(S)");

CDCParser::CDCParser(int index, const wxString& id, const wxString& title, const wxDateTime& create)
: CDictParser(index, id, title, create)
{
}

CDCParser::~CDCParser()
{
}

int CDCParser::Init(CDBAccess::TDatabase& db)
{
    try
    {
		const char* sybmoltable = "CREATE TABLE IF NOT EXISTS FundsetDCKasusTable (WordID INTEGER PRIMARY KEY, Kasus VARCHAR(32))";
		db.ExecuteUpdate(sybmoltable);

		const char* resulttable = "CREATE TABLE IF NOT EXISTS FundsetDCResultTable (WordID INTEGER, Result VARCHAR(255))";
		db.ExecuteUpdate(resulttable);
    }
    catch(CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

int CDCParser::ParserHTML(const std::wstring& html, TinyHtmlParser::CDocumentObject &doc, const TinyHtmlParser::CElementObject *dict, TWordResultMap &result) const
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

int CDCParser::GetRecord(TinyHtmlParser::CDocumentObject* doc, const TinyHtmlParser::CElementObject* pr, TWordResultMap& result) const
{
    wxString word;
    std::auto_ptr<CDCResult> res(new CDCResult);

    TinyHtmlParser::CDocumentObject::TElementStack tmpstack;
    const TinyHtmlParser::CElementObject* p = doc->FindFirstElement(pr, L"DIV", tmpstack);
    while(p != NULL)
    {
        const TinyHtmlParser::CAttributeObject* pa = p->FindAttribute(L"style");
        if(pa == NULL)
            return -1;

        if(pa->value == L"\"MARGIN: 0px 0px 5px; COLOR: #808080; LINE-HEIGHT: normal\"")
        {//word
            if(p->child == NULL)
                return -1;
            if(p->child->child == NULL || p->child->child->type != TinyHtmlParser::ET_ELEMENT)
                return -1;
            word = wxString(p->child->child->value.c_str());//, wxConvISO8859_1);
        }
        else if(pa->value == L"\"MARGIN: 0px 0px 5px\"")
        {//result
            if(p->child == NULL || p->child->type != TinyHtmlParser::ET_ELEMENT)
                return -1;
            res->m_vctRecord.push_back(wxString(p->child->value.c_str()));//, wxConvISO8859_1);

            if(p->child->child != NULL)
            {
                const TinyHtmlParser::CElementObject* pc = p->child->child;
                while(pc != NULL)
                {
                    if(pc->tag == L"FONT")
                    {
                        res->m_strKasus = wxString(pc->value.c_str());//, wxConvISO8859_1);
                    }
                    else
                    {
                        res->m_vctRecord.push_back(wxString(pc->value.c_str()));//, wxConvISO8859_1);
                    }
                   
                    pc = pc->sibling;
                }
            }      
        }
        
        p = doc->FindNextElement(pr, L"DIV", tmpstack);
    }

    TWordResultMap::iterator it = result.insert(std::make_pair(word, TDictResultMap())).first;
    it->second.insert(std::make_pair(ID, CDictResult(res.release())));

    return 0;
}

int CDCParser::IsWordExist(CDBAccess::TDatabase &db, int wordid)
{
    CDBAccess::TQuery query = db.PrepareStatement("SELECT COUNT(*) FROM FundsetDCResultTable WHERE WordID = ?");
    query.Bind(1, wordid);
    CDBAccess::TResult res = query.ExecuteQuery();
    if(res.GetInt(0) == 0)
        return -1;
    return 0;
}

int CDCParser::GetResult(CDBAccess::TDatabase &db, int wordid, TDictResultMap &result)
{
    return -1;
}

int CDCParser::GetResult(CDBAccess::TDatabase &db, int wordid, CDictResult &result)
{
    std::auto_ptr<CDCResult> dcres(new CDCResult());

    CDBAccess::TQuery query = db.PrepareStatement("SELECT Result FROM FundsetDCResultTable WHERE WordID = ?");
    query.Bind(1, wordid);
    CDBAccess::TResult res = query.ExecuteQuery();
    if(res.Eof())
        return -1;

    while(res.NextRow())
    {
        dcres->m_vctRecord.push_back(res.GetString(0));
    }

    query.Reset();
    query = db.PrepareStatement("SELECT Kasus FROM FundsetDCKasusTable WHERE WordID = ?");
    query.Bind(1, wordid);
    res = query.ExecuteQuery();
    if(!res.Eof())
    {
        dcres->m_strKasus = res.GetString(0);
    }

    result.Attach(dcres.release());

    return 0;
}

int CDCParser::SaveResult(CDBAccess::TDatabase &db, int wordid, const CDictResult &result)
{
    if(IsWordExist(db, wordid) == 0)
        return 0;

    const CDCResult* res = dynamic_cast<const CDCResult*>(result.Result());

    if(!res->m_strKasus.empty())
    {
        CDBAccess::TQuery query = db.PrepareStatement("INSERT INTO FundsetDCKasusTable VALUES(?, ?)");
        query.Bind(1, wordid);
        query.Bind(2, res->m_strKasus);
        query.ExecuteUpdate();
    }

    for(CDCResult::TRecordVector::const_iterator it = res->m_vctRecord.begin(); it != res->m_vctRecord.end(); ++ it)
    {
        CDBAccess::TQuery query = db.PrepareStatement("INSERT INTO FundsetDCResultTable VALUES(?, ?)");
	    query.Bind(1, wordid);
        query.Bind(2, *it);
	    query.ExecuteUpdate();	
    }

    return 0;
}

int CDCParser::RemoveResult(CDBAccess::TDatabase &db, int wordid)
{
    CDBAccess::TQuery query = db.PrepareStatement("DELETE FROM FundsetDCKasusTable WHERE WordID = ?");
    query.Bind(1, wordid);
    query.ExecuteUpdate();

    query.Reset();
    query = db.PrepareStatement("DELETE FROM FundsetDCResultTable WHERE WordID = ?");
    query.Bind(1, wordid);
    query.ExecuteUpdate();

    return 0;
}

}