
#include "wx/arrstr.h"

#include "DictResultParser.h"
#include "LingosHookApp.h"

#include "SearchProcObject.h"

CSearchProcObject::CSearchProcObject(LingosHookFrame *frame, CDBAccess &db)
: _objFrame(frame)
, _objDB(db)
, _objDictParser(NULL)
{
}

CSearchProcObject::~CSearchProcObject()
{
}

void CSearchProcObject::GetAllIndex()
{
	if(_objFrame != NULL)
	{
		wxArrayString vct;
		if(_objDB.GetAllWords(vct) == 0)
		{
			for(wxArrayString::const_iterator it = vct.begin(); it != vct.end(); ++ it)
			{
				_objFrame->SearchIndex(*it);
			}
		}
	}
}

int CSearchProcObject::GetResult(const wxString &index)
{
	if(_objFrame != NULL)
	{
		CDBAccess::TSearchData data;
		if(_objDB.Search(index, data) == 0)
		{
			_objFrame->SearchResult(data);
		}
	}
	return -1;
}

const wxString CSearchProcObject::GetWordClassName(const wxString& dict, CDBAccess::WordClass wc)
{
	if(_objDictParser.get() == NULL)
	{
		_objDictParser.reset(CDictResultParser::MakeParser(dict));
		if(_objDictParser.get() == NULL)
			return _("invaild.");
	}
	return _objDictParser->WCToStr(wc);
}

int CSearchProcObject::RemoveWord(const wxString &word)
{
    return _objDB.Remove(word);
}