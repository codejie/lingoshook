
#include "LingosHookApp.h"

#include "DictResultParser.h"
#include "ResultProcObject.h"
#include "TinyHtmlParser.h"

CResultProcObject::CResultProcObject(LingosHookFrame *frame, CDBAccess &db)
: _objFrame(frame)
, _objDB(db)
, _objDictParser(NULL)
{
}

CResultProcObject::~CResultProcObject()
{
}


void CResultProcObject::HTMLProc(const wxString &str)
{
    std::wstring html(str.begin(), str.end());

    //find dict
    TinyHtmlParser::CDocumentObject doc;
    if(doc.Load(html) != 0)
        return;

    const TinyHtmlParser::CElementObject* pe = doc.FindFirstElement(L"DIV");
    while(pe != NULL)
    {
        const TinyHtmlParser::CAttributeObject* pa = pe->FindAttribute(L"style");
        if(pa != NULL && pa->value == L"\"PADDING-RIGHT: 10px; PADDING-LEFT: 10px; FONT-SIZE: 10.5pt; PADDING-BOTTOM: 0px; WIDTH: 100%; LINE-HEIGHT: 1.2em; PADDING-TOP: 10px; FONT-FAMILY: 'Tahoma'\"")
        {
            pa = pe->FindAttribute(L"dictid");
            if(pa != NULL)
            {
                wxString dictid(pa->value.c_str());//, wxConvISO8859_1);
                dictid = dictid.substr(1, dictid.size() - 2);
                if(_objDictParser.get() == NULL || _objDictParser.get()->GetID() != dictid)
                {
                    _objDictParser.reset(CDictResultParser::MakeParser(dictid));
                }
                if(_objDictParser.get() == NULL)
                    return;
                //parser
                CDBAccess::TRecordDataVector vct;
                if(_objDictParser->ParserHTML(str, doc, pe, vct) == 0)
                {
                    for(CDBAccess::TRecordDataVector::const_iterator it = vct.begin(); it != vct.end(); ++ it)
                    {
                        bool insert = false;
		                if(_objDB.Insert(*it, insert) == 0)
		                {
			                if(_objFrame != NULL)
                                _objFrame->ResultProc(insert, it->m_strWord);
		                }
                    }
                }
                return;
            }
        }
        pe = doc.FindNextElement();
    }
}

void CResultProcObject::TextProc(const wxString &str)
{
	//if(_objDictParser.get() == NULL)
	//{
	//	_objDictParser.reset(CDictResultParser::MakeParser(_("Vicon English-Chinese(S) Dictionary")));// new CViconECDictResultParser());
	//	if(_objDictParser.get() == NULL)
	//		return;
	//}
	//CDBAccess::TRecordData record;
	//if(_objDictParser->ParserText(str, _strHTML, record) == 0)
	//{
	//	if(_objDB.Insert(record) == 0)
	//	{
	//		if(_objFrame != NULL)
	//			_objFrame->ResultProc(true, record.m_strWord);
	//	}
	//}
}