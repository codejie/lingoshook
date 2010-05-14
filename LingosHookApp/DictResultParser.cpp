
#include "DictResultParser.h"


CDictResultParser* CDictResultParser::MakeParser(const wxString& str)
{
	if(str == CViconECDictResultParser::TITLE
        || str == CViconECDictResultParser::ID)
		return new CViconECDictResultParser();
	else
		return NULL;
}

///

wxString CViconECDictResultParser::TITLE = _("Vicon English-Chinese(S) Dictionary");
wxString CViconECDictResultParser::ID = _("E1C27E806399D047822B6650194A3D32");

CViconECDictResultParser::CViconECDictResultParser()
: CDictResultParser()
{
}

int CViconECDictResultParser::ParserText(const wxString &text, const wxString &html, CDBAccess::TRecordDataVector &record) const
{
//	if(text.find(m_strNotFound) != wxString::npos)
//		return -1;
//
//	wxString str = text;
//	wxString::size_type pos = str.find(_("\n"));
//
//	if(pos == wxString::npos)
//		return -1;
//
//	//dict
//	wxString tmp = str.substr(0, pos - 1);
//	tmp.Trim();
//	tmp.Trim(false);
////	if(tmp != m_strTitle)
////		return -1;
//	record.m_vctResult.push_back(std::make_pair(m_strTitle, CDBAccess::TResultVector()));
//
//	//blank line
//	str = str.substr(pos + 1);
//	pos = str.find(_("\n"));
//	if(pos == wxString::npos)
//		return -1;
//	
//	//word and symbol
//	str = str.substr(pos + 1);
//	pos = str.find(_("\n"));
//	if(pos == wxString::npos)
//		return -1;
//	tmp = str.substr(0, pos - 1);
//	wxString::size_type p = tmp.find(_("["));
//	if(p == wxString::npos)
//		return -1;
//	record.m_strWord = tmp.substr(0, p - 1);
//	record.m_strWord.Trim();
//	record.m_strWord.Trim(false);
//	record.m_strSymbol = tmp.substr(p);	
//
//	//result
//	str = str.substr(pos + 1);
//	pos = str.find(_("\n"));
//	while(pos != wxString::npos)
//	{
//		wxString result = str.substr(0, pos - 1);
//		wxString::size_type p = result.find(_("  "));
//		if(p == wxString::npos)
//			return -1;
//		tmp = result.substr(0, p);
//		CDBAccess::WordClass wc = StrToWC(tmp);
//
//		result = result.substr(p + 1);
//		result.Trim();
//		result.Trim(false);
//		
//		record.m_vctResult[record.m_vctResult.size() - 1].second.push_back(std::make_pair(wc, result));
//
//		str = str.substr(pos + 1);
//		pos = str.find(_("\n"));
//	}
//	if(str.size() > 0)
//	{
//		wxString result = str.substr(0, pos - 1);
//		wxString::size_type p = result.find(_("  "));
//		if(p == wxString::npos)
//			return -1;
//		tmp = result.substr(0, p);
//		CDBAccess::WordClass wc = StrToWC(tmp);
//
//		result = result.substr(p + 1);
//		result.Trim();
//		result.Trim(false);
//		
//		record.m_vctResult[record.m_vctResult.size() - 1].second.push_back(std::make_pair(wc, result));
//	}
//	//html
//	record.m_strHTML = html;

	return 0;
}

int CViconECDictResultParser::ParserHTML(const std::wstring& html, CDBAccess::TRecordDataVector &vct) const
{
    std::wstring str(html.begin(), html.end());

    TinyHtmlParser::CDocumentObject doc;
    if(doc.Load(str) != 0)
        return -1;

    vct.clear();

    std::wstring body(ID.begin(), ID.end());
    body = L"dict_body_" + body;
    const TinyHtmlParser::CElementObject* pe = doc.FindFirstElement(L"DIV");
    while(pe != NULL)
    {
        const TinyHtmlParser::CAttributeObject* pa = pe->FindAttribute(L"id");
        if(pa != NULL && pa->value == body)
        {
            TinyHtmlParser::CDocumentObject::TElementStack tmpstack;
            const TinyHtmlParser::CElementObject* pr = doc.FindFirstElement(pe, L"DIV", tmpstack);
            while(pr != NULL)
            {
                pa = pr->FindAttribute(L"style");
                if(pa != NULL && pa->value == L"\"MARGIN: 5px 0px\"")
                {
                    CDBAccess::TRecordData rec;
                    if(GetRecord(&doc, pr, rec) != 0)
                        return -1;
                    rec.m_strHTML = html;
                    vct.push_back(rec);  
                }
                pr = doc.FindNextElement(pe, L"DIV", tmpstack);
            }          
        }
        pe = doc.FindNextElement();
    }    

	return 0;
}

int CViconECDictResultParser::ParserHTML(const std::wstring& html, TinyHtmlParser::CDocumentObject& doc, const TinyHtmlParser::CElementObject* dict, CDBAccess::TRecordDataVector& vct) const
{
    if(dict == NULL)
        return -1;

    if(doc.Root() == NULL)
        return -1;

    vct.clear();

    TinyHtmlParser::CDocumentObject::TElementStack tmpstack;
    const TinyHtmlParser::CElementObject* pr = doc.FindFirstElement(dict, L"DIV", tmpstack);
    while(pr != NULL)
    {
        const TinyHtmlParser::CAttributeObject* pa = pr->FindAttribute(L"style");
        if(pa != NULL && pa->value == L"\"MARGIN: 5px 0px\"")
        {
            CDBAccess::TRecordData rec;
            if(GetRecord(&doc, pr, rec) != 0)
                return -1;
            rec.m_strHTML = html;
            vct.push_back(rec);  
        }
        pr = doc.FindNextElement(dict, L"DIV", tmpstack);
    } 

	return 0;
}

int CViconECDictResultParser::GetRecord(TinyHtmlParser::CDocumentObject* doc, const TinyHtmlParser::CElementObject* pr, CDBAccess::TRecordData& rec) const
{
    CDBAccess::TResultVector vct;

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
            rec.m_strWord = wxString(p->child->child->value.c_str(), wxConvISO8859_1);

            if(p->child->sibling == NULL || p->child->sibling->child == NULL || p->child->sibling->child->type != TinyHtmlParser::ET_ELEMENT)
            {
                rec.m_strSymbol = _("null");
                //return -1;
            }
            else
            {
                rec.m_strSymbol = wxString(p->child->sibling->child->value.c_str(), wxConvISO8859_1);
            }
        }
        else if(pa->value == L"\"MARGIN: 0px 0px 5px\"")
        {//result and class
            CDBAccess::TResultPair result;
            if(p->child == NULL || p->child->type != TinyHtmlParser::ET_ELEMENT)
                return -1;
            result.second = wxString(p->child->value.c_str(),wxConvISO8859_1);
            result.second.Trim(false);

            if(p->child->child == NULL || p->child->child->type != TinyHtmlParser::ET_ELEMENT)
            {
                result.first = CDBAccess::WC_UNKNOWN;
                //return -1;
            }
            else
            {
                result.first = StrToWC(wxString(p->child->child->value.c_str(), wxConvISO8859_1));
            }
            vct.push_back(result);
        }
        
        p = doc->FindNextElement(pr, L"DIV", tmpstack);
    }

    rec.m_vctResult.push_back(std::make_pair(TITLE, vct));

    return 0;
}

CDBAccess::WordClass CViconECDictResultParser::StrToWC(const wxString &str) const
{
	CDBAccess::WordClass wc = CDBAccess::WC_UNKNOWN;
	if(str == _("n."))
		return CDBAccess::WC_NOUN;
	else if(str == _("adj."))
		return CDBAccess::WC_ADJECTIVE;
	else if(str == _("v."))
		return CDBAccess::WC_VERB;
	else if(str == _("adv."))
		return CDBAccess::WC_ADVERB;
	else if(str == _("conj."))
		return CDBAccess::WC_CONJ;
    else if(str == _("interj."))
        return CDBAccess::WC_INTERJECTION;
	else
		return CDBAccess::WC_UNKNOWN;
}

const wxString CViconECDictResultParser::WCToStr(CDBAccess::WordClass wc) const
{
	switch(wc)
	{
	case CDBAccess::WC_NOUN:
		return _("n.");
	case CDBAccess::WC_ADJECTIVE:
		return _("adj.");
	case CDBAccess::WC_VERB:
		return _("v.");
	case CDBAccess::WC_ADVERB:
		return _("adv.");
	case CDBAccess::WC_CONJ:
		return _("conj.");
    case CDBAccess::WC_INTERJECTION:
        return _("interj.");
	default:
		return _("unkn.");
	}
}