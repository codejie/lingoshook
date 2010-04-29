#ifndef __SEARCHPROCOBJECT_H__
#define __SEARCHPROCOBJECT_H__

#include "wx/wx.h"

#include "DBAccess.h"

class LingosHookFrame;

class CDictResultParser;

class CSearchProcObject
{
public:
	CSearchProcObject(LingosHookFrame* frame, CDBAccess& db);
	virtual ~CSearchProcObject();

	void GetAllIndex();
	int GetResult(const wxString& index);
	const wxString GetWordClassName(const wxString& dict, CDBAccess::WordClass wc);

    int RemoveWord(const wxString& word);
private:
	LingosHookFrame* _objFrame;
	CDBAccess& _objDB;
	std::auto_ptr<CDictResultParser> _objDictParser;
};


#endif
